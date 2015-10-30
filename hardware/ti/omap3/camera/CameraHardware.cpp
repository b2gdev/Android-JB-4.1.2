/*
**
** Copyright (C) 2009 0xlab.org - http://0xlab.org/
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#define LOG_TAG "CameraHardware"
#include <utils/Log.h>

#include "CameraHardware.h"
#include "converter.h"
#include <fcntl.h>
#include <sys/mman.h>

#include <cutils/native_handle.h>
#include <hal_public.h>
#include <ui/GraphicBufferMapper.h>
#include <gui/ISurfaceTexture.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define VIDEO_DEVICE_2      "/dev/video2"
#define VIDEO_DEVICE_0      "/dev/video0"
#define MEDIA_DEVICE        "/dev/media0"
#define PREVIEW_WIDTH       640
#define PREVIEW_HEIGHT      480
#define PICTURE_WIDTH       2592
#define PICTURE_HEIGHT      1944
#define PIXEL_FORMAT        V4L2_PIX_FMT_YUYV

#define CAMHAL_GRALLOC_USAGE GRALLOC_USAGE_HW_TEXTURE | \
                             GRALLOC_USAGE_HW_RENDER | \
                             GRALLOC_USAGE_SW_READ_RARELY | \
                             GRALLOC_USAGE_SW_WRITE_NEVER

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif
#define MAX_STR_LEN 35

#include <cutils/properties.h>
#ifndef UNLIKELY
#define UNLIKELY(exp) (__builtin_expect( (exp) != 0, false ))
#endif
static int mDebugFps = 0;
int version=0;
namespace android {

/* 29/12/10 : preview/picture size validation logic */
const char CameraHardware::supportedPictureSizes [] = "2592x1944";
const char CameraHardware::supportedPreviewSizes [] = "640x480";

const supported_resolution CameraHardware::supportedPictureRes[] = {{2592, 1944}};
const supported_resolution CameraHardware::supportedPreviewRes[] = {{640, 480}};

const char CameraHardware::supportedFlashModes [] = "auto,on,off,torch";
const char CameraHardware::supportedFocusModes [] = "continuous-picture";
const char CameraHardware::supportedSceneModes [] = "auto";
const char CameraHardware::supportedWhiteBalance [] = "auto";

char CameraHardware::currentFlashMode [] = "auto";
char CameraHardware::currentFocusMode[] = "continuous-picture";
char CameraHardware::currentSceneMode [] = "auto";
char CameraHardware::currentWhiteBalance [] = "auto";


CameraHardware::CameraHardware()
                  : mParameters(),
                    mHeap(0),
                    mPreviewHeap(0),
                    mRawHeap(0),
                    mCamera(0),
                    mPreviewFrameSize(0),
                    mNotifyCb(0),
                    mDataCb(0),
                    mDataCbTimestamp(0),
                    mCallbackCookie(0),
                    mMsgEnabled(0),
                    previewStopped(true)
{
	/* create camera */
	mCamera = new V4L2Camera();
	#ifdef _USE_USB_CAMERA_
	mCamera->Open(VIDEO_DEVICE_0,PREVIEW_PURPOSE);  /*USB camera use this node*/

	#else
	version = get_kernel_version();
	if(version <= 0)
		ALOGE("Failed to parse kernel version\n");
	if(version >= KERNEL_VERSION(2,6,37))
	{
		ALOGE("version >= KERNEL_VERSION(2,6,37)");
		mCamera->Open(VIDEO_DEVICE_2,PREVIEW_PURPOSE);
		mCamera->Open_media_device(MEDIA_DEVICE);
	}
	else
	{
		mCamera->Open(VIDEO_DEVICE_0,PREVIEW_PURPOSE);
	}
	#endif

	initDefaultParameters();
        mNativeWindow=NULL;

    /* whether prop "debug.camera.showfps" is enabled or not */
    char value[PROPERTY_VALUE_MAX];
    property_get("debug.camera.showfps", value, "0");
    mDebugFps = atoi(value);
    ALOGD_IF(mDebugFps, "showfps enabled");
}

void CameraHardware::initDefaultParameters()
{
    CameraParameters p;

    p.setPreviewSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);
    p.setPreviewFrameRate(DEFAULT_FRAME_RATE);
    p.setPreviewFormat(CameraParameters::PIXEL_FORMAT_YUV422SP);

    p.setPictureSize(PICTURE_WIDTH, PICTURE_HEIGHT);
    p.setPictureFormat(CameraParameters::PIXEL_FORMAT_JPEG);
    p.set(CameraParameters::KEY_JPEG_QUALITY, 100);

	p.set(CameraParameters::KEY_SUPPORTED_PICTURE_SIZES, CameraHardware::supportedPictureSizes);
	p.set(CameraParameters::KEY_SUPPORTED_PICTURE_FORMATS, CameraParameters::PIXEL_FORMAT_JPEG);
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES, CameraHardware::supportedPreviewSizes);
	p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS, CameraParameters::PIXEL_FORMAT_YUV422SP);
	p.set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, CameraParameters::PIXEL_FORMAT_YUV420SP);
	
	p.set(CameraParameters::KEY_FLASH_MODE,(const char *) CameraHardware::currentFlashMode);
	p.set(CameraParameters::KEY_FOCUS_MODE,(const char *) CameraHardware::currentFocusMode);
	p.set(CameraParameters::KEY_SCENE_MODE,(const char *) CameraHardware::currentSceneMode);
	p.set(CameraParameters::KEY_WHITE_BALANCE,(const char *) CameraHardware::currentWhiteBalance);
	
    p.set(CameraParameters::KEY_SUPPORTED_FLASH_MODES,CameraHardware::supportedFlashModes);
    p.set(CameraParameters::KEY_SUPPORTED_FOCUS_MODES,CameraHardware::supportedFocusModes);
    p.set(CameraParameters::KEY_SUPPORTED_SCENE_MODES,CameraHardware::supportedSceneModes);
    p.set(CameraParameters::KEY_SUPPORTED_WHITE_BALANCE,CameraHardware::supportedWhiteBalance);

    if (setParameters(p) != NO_ERROR) {
        ALOGE("Failed to set default parameters?!");
    }
    return;
}

int CameraHardware::get_kernel_version()
{
	char *verstring, *dummy;
	int fd;
	int major,minor,rev,ver=-1;
	if ((verstring = (char *) malloc(MAX_STR_LEN)) == NULL )
	{
		ALOGE("Failed to allocate memory\n");
		return -1;
	}
	if ((dummy = (char *) malloc(MAX_STR_LEN)) == NULL )
	{
		ALOGE("Failed to allocate memory\n");
		free (verstring);
		return -1;
	}

	if ((fd = open("/proc/version", O_RDONLY)) < 0)
	{
		ALOGE("Failed to open file /proc/version\n");
		goto ret;
	}

	if (read(fd, verstring, MAX_STR_LEN) < 0)
	{
		ALOGE("Failed to read kernel version string from /proc/version file\n");
		close(fd);
		goto ret;
	}
	close(fd);
	if (sscanf(verstring, "%s %s %d.%d.%d%s\n", dummy, dummy, &major, &minor, &rev, dummy) != 6)
	{
		ALOGE("Failed to read kernel version numbers\n");
		goto ret;
	}
	ver = KERNEL_VERSION(major, minor, rev);
ret:
	free(verstring);
	free(dummy);
	return ver;
}

CameraHardware::~CameraHardware()
{
	mCamera->Uninit();
	mCamera->StopStreaming();
	mCamera->Close();
    delete mCamera;
    mCamera = 0;
}

sp<IMemoryHeap> CameraHardware::getPreviewHeap() const
{
    ALOGV("Preview Heap");
    return mPreviewHeap;
}

sp<IMemoryHeap> CameraHardware::getRawHeap() const
{
    ALOGV("Raw Heap");
    return mRawHeap;
}

void CameraHardware::setCallbacks(camera_notify_callback notify_cb,
                                  camera_data_callback data_cb,
                                  camera_data_timestamp_callback data_cb_timestamp,
                                  camera_request_memory get_memory,
                                  void* user)
{
    Mutex::Autolock lock(mLock);
    mNotifyCb = notify_cb;
    mDataCb = data_cb;
    mDataCbTimestamp = data_cb_timestamp;
    mRequestMemory = get_memory;
    mCallbackCookie = user;
    return;
}

int CameraHardware::setPreviewWindow( preview_stream_ops_t *window)
{
    int err;
    Mutex::Autolock lock(mLock);
        if(mNativeWindow)
            mNativeWindow=NULL;
    if(window==NULL)
    {
        ALOGW("Window is Null");
        return 0;
    }
    int width, height;
    mParameters.getPreviewSize(&width, &height);
    mNativeWindow=window;
    mNativeWindow->set_usage(mNativeWindow,CAMHAL_GRALLOC_USAGE);
    mNativeWindow->set_buffers_geometry(
                mNativeWindow,
                width,
                height,
                HAL_PIXEL_FORMAT_RGB_565);
    err = mNativeWindow->set_buffer_count(mNativeWindow, 3);
    if (err != 0) {
        ALOGE("native_window_set_buffer_count failed: %s (%d)", strerror(-err), -err);

        if ( ENODEV == err ) {
            ALOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }
    }

    return 0;
}

void CameraHardware::enableMsgType(int32_t msgType)
{
	ALOGD("enableMsgType:%d",msgType);
    Mutex::Autolock lock(mLock);
    mMsgEnabled |= msgType;
}

void CameraHardware::disableMsgType(int32_t msgType)
{
	ALOGD("disableMsgType:%d",msgType);
    Mutex::Autolock lock(mLock);
    mMsgEnabled &= ~msgType;
}

bool CameraHardware::msgTypeEnabled(int32_t msgType)
{
	ALOGD("msgTypeEnabled:%d",msgType);
    Mutex::Autolock lock(mLock);
    return (mMsgEnabled & msgType);
}

bool CameraHardware::validateSize(size_t width, size_t height, const supported_resolution *supRes, size_t count)
{
    bool ret = false;
    status_t stat = NO_ERROR;
    unsigned int size;

    if ( NULL == supRes ) {
        ALOGE("Invalid resolutions array passed");
        stat = -EINVAL;
    }

    if ( NO_ERROR == stat ) {
        for ( unsigned int i = 0 ; i < count; i++ ) {
           // ALOGD( "Validating %d, %d and %d, %d", supRes[i].width, width, supRes[i].height, height);
            if ( ( supRes[i].width == width ) && ( supRes[i].height == height ) ) {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

// ---------------------------------------------------------------------------
static void showFPS(const char *tag)
{
    static int mFrameCount = 0;
    static int mLastFrameCount = 0;
    static nsecs_t mLastFpsTime = 0;
    static float mFps = 0;
    mFrameCount++;
    if (!(mFrameCount & 0x1F)) {
        nsecs_t now = systemTime();
        nsecs_t diff = now - mLastFpsTime;
        mFps =  ((mFrameCount - mLastFrameCount) * float(s2ns(1))) / diff;
        mLastFpsTime = now;
        mLastFrameCount = mFrameCount;
        ALOGD("[%s] %d Frames, %f FPS", tag, mFrameCount, mFps);
    }
}

int CameraHardware::previewThread()
{

    int width, height;
    int err;
    IMG_native_handle_t** hndl2hndl;
    IMG_native_handle_t* handle;
    int stride;
    mParameters.getPreviewSize(&width, &height);
    int framesize= width * height * 1.5 ; //yuv420sp


    if (!previewStopped) {

        mLock.lock();

        if (mNativeWindow != NULL) {

            if ((err = mNativeWindow->dequeue_buffer(mNativeWindow,(buffer_handle_t**) &hndl2hndl,&stride)) != 0) {
                ALOGW("Surface::dequeueBuffer returned error %d", err);
                return -1;
            }

            mNativeWindow->lock_buffer(mNativeWindow, (buffer_handle_t*) hndl2hndl);
            GraphicBufferMapper &mapper = GraphicBufferMapper::get();
            Rect bounds(width, height);

            void *tempbuf;
            void *dst;

            if(0 == mapper.lock((buffer_handle_t)*hndl2hndl,CAMHAL_GRALLOC_USAGE, bounds, &dst))
            {
                // Get preview frame
                tempbuf=mCamera->GrabPreviewFrame();
                convertYUYVtoRGB565((unsigned char *)tempbuf,(unsigned char *)dst, width, height);
                mapper.unlock((buffer_handle_t)*hndl2hndl);

                mNativeWindow->enqueue_buffer(mNativeWindow,(buffer_handle_t*) hndl2hndl);

                if ((mMsgEnabled & CAMERA_MSG_PREVIEW_FRAME) ||
                    (mMsgEnabled & CAMERA_MSG_VIDEO_FRAME)) {

                    camera_memory_t* picture = mRequestMemory(-1, framesize, 1, NULL);
                    convertYUYVtoRGB565((unsigned char *)tempbuf, (unsigned char*)picture->data, width, height);

                    if ((mMsgEnabled & CAMERA_MSG_VIDEO_FRAME ) &&
                         mRecordingEnabled ) {
                        nsecs_t timeStamp = systemTime(SYSTEM_TIME_MONOTONIC);
                        //mTimestampFn(timeStamp, CAMERA_MSG_VIDEO_FRAME,mRecordBuffer, mUser);
                    }
                    mDataCb(CAMERA_MSG_PREVIEW_FRAME,picture,0,NULL,mCallbackCookie);
		}

                mCamera->ReleasePreviewFrame();
            }

        }
        mLock.unlock();
    }

    return NO_ERROR;
}

status_t CameraHardware::startPreview()
{

    int width, height;
    int mHeapSize = 0;
    int ret = 0;

    if(!mCamera) {
        delete mCamera;
        mCamera = new V4L2Camera();
    }

    if(version >= KERNEL_VERSION(2,6,37)) {
        if (mCamera->Open(VIDEO_DEVICE_2,PREVIEW_PURPOSE) < 0)
            return INVALID_OPERATION;
    } else {
        if (mCamera->Open(VIDEO_DEVICE_0,PREVIEW_PURPOSE) < 0)
            return INVALID_OPERATION;
    }

    Mutex::Autolock lock(mPreviewLock);
    if (mPreviewThread != 0) {
        return INVALID_OPERATION;
    }

    mParameters.getPreviewSize(&mPreviewWidth, &mPreviewHeight);
    ALOGD("startPreview width:%d,height:%d",mPreviewWidth,mPreviewHeight);
    if(mPreviewWidth <=0 || mPreviewHeight <=0) {
        ALOGE("Preview size is not valid,aborting..Device can not open!!!");
        return INVALID_OPERATION;
    }

    ret = mCamera->Configure(mPreviewWidth,mPreviewHeight,PIXEL_FORMAT,30);
    if(ret < 0) {
	    ALOGE("Fail to configure camera device");
	    return INVALID_OPERATION;
    }

   /* clear previously buffers*/
    if(mPreviewHeap != NULL) {
        ALOGD("mPreviewHeap Cleaning!!!!");
        mPreviewHeap.clear();
    }

    if(mRawHeap != NULL) {
        ALOGD("mRawHeap Cleaning!!!!");
        mRawHeap.clear();
    }

    if(mHeap != NULL) {
        ALOGD("mHeap Cleaning!!!!");
        mHeap.clear();
    }

    mPreviewFrameSize = mPreviewWidth * mPreviewHeight * 2;
    mHeapSize = (mPreviewWidth * mPreviewHeight * 3) >> 1;

    /* mHead is yuv420 buffer, as default encoding is yuv420 */
    mHeap = new MemoryHeapBase(mHeapSize);
    mBuffer = new MemoryBase(mHeap, 0, mHeapSize);

    mPreviewHeap = new MemoryHeapBase(mPreviewFrameSize);
    mPreviewBuffer = new MemoryBase(mPreviewHeap, 0, mPreviewFrameSize);

    mRawHeap = new MemoryHeapBase(mPreviewFrameSize);
    mRawBuffer = new MemoryBase(mRawHeap, 0, mPreviewFrameSize);

    ret = mCamera->BufferMap();
    if (ret) {
        ALOGE("Camera Init fail: %s", strerror(errno));
        return UNKNOWN_ERROR;
    }

    ret = mCamera->StartStreaming();
    if (ret) {
        ALOGE("Camera StartStreaming fail: %s", strerror(errno));
        mCamera->Uninit();
        mCamera->Close();
        return UNKNOWN_ERROR;
    }

    /* start preview thread */
     previewStopped = false;
     mPreviewThread = new PreviewThread(this);
     
     mCamera->EmbeddedAFStart();

    return NO_ERROR;
}

void CameraHardware::stopPreview(bool releaseAF)
{
    sp<PreviewThread> previewThread;
    { /* scope for the lock */
        Mutex::Autolock lock(mPreviewLock);
        previewStopped = true;
        previewThread = mPreviewThread;
    }

    /* don't hold the lock while waiting for the thread to quit */
	if (previewThread != 0) {
		previewThread->requestExitAndWait();
	}

    if (mPreviewThread != 0) {
        mCamera->Uninit();
        if(releaseAF){
			mCamera->EmbeddedAFRelease();
		}
        mCamera->StopStreaming();
        mCamera->Close();
    }

    Mutex::Autolock lock(mPreviewLock);
    mPreviewThread.clear();
    return;
}

bool CameraHardware::previewEnabled()
{
    return mPreviewThread != 0;
}

status_t CameraHardware::startRecording()
{
    ALOGE("startRecording");
    mRecordingLock.lock();
    mRecordingEnabled = true;

    mParameters.getPreviewSize(&mPreviewWidth, &mPreviewHeight);
    ALOGD("getPreviewSize width:%d,height:%d",mPreviewWidth,mPreviewHeight);

    mRecordingLock.unlock();
    return NO_ERROR;

}

void CameraHardware::stopRecording()
{
    ALOGE("stopRecording");
    mRecordingLock.lock();

    mRecordingEnabled = false;
    mRecordingLock.unlock();
}

bool CameraHardware::recordingEnabled()
{
    return mRecordingEnabled == true;
}

void CameraHardware::releaseRecordingFrame(const void* opaque)
{
    if (UNLIKELY(mDebugFps)) {
        showFPS("Recording");
    }
    return;
}

// ---------------------------------------------------------------------------

int CameraHardware::beginAutoFocusThread(void *cookie)
{
    CameraHardware *c = (CameraHardware *)cookie;
    ALOGD("beginAutoFocusThread");
    return c->autoFocusThread();
}

int CameraHardware::autoFocusThread()
{
    if (mMsgEnabled & CAMERA_MSG_FOCUS)
        mNotifyCb(CAMERA_MSG_FOCUS, true, 0, mCallbackCookie);
    return NO_ERROR;
}

status_t CameraHardware::autoFocus()
{
    Mutex::Autolock lock(mLock);
    if (createThread(beginAutoFocusThread, this) == false) {
        ALOGE("beginAutoFocusThread failed!!");
        return UNKNOWN_ERROR;
    }

    return NO_ERROR;
}

status_t CameraHardware::cancelAutoFocus()
{
    return NO_ERROR;
}

int CameraHardware::beginPictureThread(void *cookie)
{
    CameraHardware *c = (CameraHardware *)cookie;
    ALOGE("begin Picture Thread");
    return c->pictureThread();
}
#include <unistd.h>
int CameraHardware::pictureThread()
{
    unsigned char *frame;
    int bufferSize;
    int w,h;
    int ret;
    struct v4l2_buffer buffer;
    struct v4l2_format format;
    struct v4l2_buffer cfilledbuffer;
    struct v4l2_requestbuffers creqbuf;
    struct v4l2_capability cap;
    int i;
    char devnode[12];
    camera_memory_t* picture = NULL;
    void *tempbuf;

//DSG:    ALOGV("Picture Thread:%d",mMsgEnabled);
    ALOGE("DSG: Picture Thread:%d",mMsgEnabled);	//DSG:++
    ALOGE("\nDSG: Picture Thread:%d\n",mMsgEnabled);	//DSG:++
    if (mMsgEnabled & CAMERA_MSG_SHUTTER)
        mNotifyCb(CAMERA_MSG_SHUTTER, 0, 0, mCallbackCookie);

     mParameters.getPictureSize(&w, &h);
     ALOGV("Picture Size: Width = %d \t Height = %d", w, h);

     int width, height;
     mParameters.getPictureSize(&width, &height);
     mParameters.getPreviewSize(&width, &height);

     if(version >= KERNEL_VERSION(2,6,37)) {
         if (mCamera->Open(VIDEO_DEVICE_2,CAPTURE_PURPOSE) < 0)
             return INVALID_OPERATION;
     } else {
         if (mCamera->Open(VIDEO_DEVICE_0,CAPTURE_PURPOSE) < 0)
             return INVALID_OPERATION;
     }
     
     ret = mCamera->Configure(w,h,PIXEL_FORMAT,30);
     if(ret < 0) {
	     ALOGE("Fail to configure camera device");
	     return INVALID_OPERATION;
     }

     ret = mCamera->BufferMap();
     if (ret) {
         ALOGE("Camera BufferMap fail: %s", strerror(errno));
         return UNKNOWN_ERROR;
     }

     ret = mCamera->StartStreaming();
     if (ret) {
        ALOGE("Camera StartStreaming fail: %s", strerror(errno));
        mCamera->Uninit();
        mCamera->Close();
        return UNKNOWN_ERROR;
     }

     //TODO xxx : Optimize the memory capture call. Too many memcpy
     if (mMsgEnabled & CAMERA_MSG_COMPRESSED_IMAGE) {
        ALOGV ("mJpegPictureCallback");
        
        tempbuf = mCamera->GrabPreviewFrame();		//Discard first two frames
        mCamera->ReleasePreviewFrame();
        tempbuf = mCamera->GrabPreviewFrame();
        mCamera->ReleasePreviewFrame();
        
        mCamera->FlashStrobe();						//Strobe flash if necessay
        
        ALOGE ("\nDSG: Grab JPEG image\n");
        picture = mCamera->GrabJpegFrame(mRequestMemory);
        
        mCamera->FlashStrobeStop();					//Stop if strobed
        
        mDataCb(CAMERA_MSG_COMPRESSED_IMAGE,picture,0,NULL ,mCallbackCookie);
    }

    /* Close operation */
    mCamera->Uninit();
    mCamera->StopStreaming();
    mCamera->Close();

    return NO_ERROR;
}

status_t CameraHardware::takePicture()
{
    stopPreview(false);
    pictureThread();
    return NO_ERROR;
}

status_t CameraHardware::cancelPicture()
{
    return NO_ERROR;
}

status_t CameraHardware::dump(int fd, const Vector<String16>& args) const
{
    return NO_ERROR;
}

status_t CameraHardware::setParameters(const CameraParameters& params)
{
    Mutex::Autolock lock(mLock);
	int width  = 0;
	int height = 0;
	int framerate = 0;
	int ret;
	params.getPreviewSize(&width,&height);

	ALOGD("Set Parameter...!! ");

	ALOGD("PreviewFormat %s", params.getPreviewFormat());
	if ( params.getPreviewFormat() != NULL ) {
		if (strcmp(params.getPreviewFormat(), (const char *) CameraParameters::PIXEL_FORMAT_YUV422SP) != 0) {
			ALOGE("Only yuv422sp preview is supported");
			return -EINVAL;
		}
	}

	ALOGD("PictureFormat %s", params.getPictureFormat());
	if ( params.getPictureFormat() != NULL ) {
		if (strcmp(params.getPictureFormat(), (const char *) CameraParameters::PIXEL_FORMAT_JPEG) != 0) {
			ALOGE("Only jpeg still pictures are supported");
			return -EINVAL;
		}
	}

	/* validate preview size */
	params.getPreviewSize(&width, &height);
	ALOGD("preview width:%d,height:%d",width,height);
	if ( validateSize(width, height, supportedPreviewRes, ARRAY_SIZE(supportedPreviewRes)) == false ) {
        ALOGE("Preview size not supported");
        return -EINVAL;
    }

    /* validate picture size */
	params.getPictureSize(&width, &height);
	ALOGD("picture width:%d,height:%d",width,height);
	if (validateSize(width, height, supportedPictureRes, ARRAY_SIZE(supportedPictureRes)) == false ) {
        ALOGE("Picture size not supported");
        return -EINVAL;
    }

    framerate = params.getPreviewFrameRate();
    ALOGD("FRAMERATE %d", framerate);

    mParameters = params;

    mParameters.getPictureSize(&width, &height);
    ALOGD("Picture Size by CamHAL %d x %d", width, height);

    mParameters.getPreviewSize(&width, &height);
    ALOGD("Preview Resolution by CamHAL %d x %d", width, height);
    
    
    if((params.get(CameraParameters::KEY_FLASH_MODE) != NULL) && (CameraHardware::currentFlashMode != NULL)){
		if(strcmp(params.get(CameraParameters::KEY_FLASH_MODE), (const char *) CameraHardware::currentFlashMode) != 0){
			ret = mCamera->FlashMode(params.get(CameraParameters::KEY_FLASH_MODE));
			if(ret == 0){
				strcpy(CameraHardware::currentFlashMode,params.get(CameraParameters::KEY_FLASH_MODE));
				ALOGD("FlashMode Changed : %s", params.get(CameraParameters::KEY_FLASH_MODE));
			}else{
				return ret;
			}
		}
	}
	
	if((params.get(CameraParameters::KEY_FOCUS_MODE) != NULL) && (CameraHardware::currentFocusMode != NULL)){
		if(strcmp(params.get(CameraParameters::KEY_FOCUS_MODE), (const char *)  currentFocusMode) != 0){
			ALOGD("FocusMode Changed : %s", params.get(CameraParameters::KEY_FOCUS_MODE));
		}
	}
	
	if((params.get(CameraParameters::KEY_SCENE_MODE) != NULL) && (CameraHardware::currentSceneMode != NULL)){
		if(strcmp(params.get(CameraParameters::KEY_SCENE_MODE), (const char *)  currentSceneMode) != 0){
			ALOGD("SceneMode Changed : %s", params.get(CameraParameters::KEY_SCENE_MODE));
		}
	}
	
	if((params.get(CameraParameters::KEY_WHITE_BALANCE) != NULL) && (CameraHardware::currentWhiteBalance != NULL)){
		if(strcmp(params.get(CameraParameters::KEY_WHITE_BALANCE), (const char *)  currentWhiteBalance) != 0){
			ALOGD("WhiteBalance Changed : %s", params.get(CameraParameters::KEY_WHITE_BALANCE));
		}
	}
    

    return NO_ERROR;
}

CameraParameters CameraHardware::getParameters() const
{
    CameraParameters params;

    {
        Mutex::Autolock lock(mLock);
        params = mParameters;
    }

    return params;
}

status_t CameraHardware::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2)
{
    return BAD_VALUE;
}

void CameraHardware::release()
{

}

}; // namespace android
