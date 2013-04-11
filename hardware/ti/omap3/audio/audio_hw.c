/*
 * Copyright (C) 2012 Wolfson Microelectronics plc
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Liberal inspiration drawn from the AOSP code for Toro.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "tcbin_audio_hw"
#define LOG_NDEBUG 0

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <cutils/str_parms.h>

#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio.h>

#include <expat.h>

#include <tinyalsa/asoundlib.h>
#include <audio_utils/resampler.h>
#include <hardware/audio_effect.h>

#include <pthread.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>
#include <string.h>

#include <unistd.h>

struct route_setting
{
    char *ctl_name;
    int intval;
    char *strval;
};

/* The enable flag when 0 makes the assumption that enums are disabled by
 * "Off" and integers/booleans by 0 */
static int set_route_by_array(struct mixer *mixer, struct route_setting *route,
			      unsigned int len)
{
    struct mixer_ctl *ctl;
    unsigned int i, j, ret;

    /* Go through the route array and set each value */
    for (i = 0; i < len; i++) {
        ctl = mixer_get_ctl_by_name(mixer, route[i].ctl_name);
        if (!ctl) {
	    ALOGE("Unknown control '%s'\n", route[i].ctl_name);
            return -EINVAL;
	}

        if (route[i].strval) {
	    ret = mixer_ctl_set_enum_by_string(ctl, route[i].strval);
	    if (ret != 0) {
		ALOGE("Failed to set '%s' to '%s'\n",
		     route[i].ctl_name, route[i].strval);
	    } else {
		//ALOGV("Set '%s' to '%s'\n",
		//     route[i].ctl_name, route[i].strval);
	    }
	    
        } else {
            /* This ensures multiple (i.e. stereo) values are set jointly */
            for (j = 0; j < mixer_ctl_get_num_values(ctl); j++) {
		ret = mixer_ctl_set_value(ctl, j, route[i].intval);
		if (ret != 0) {
		    ALOGE("Failed to set '%s'.%d to %d\n",
			 route[i].ctl_name, j, route[i].intval);
		} else {
		    //ALOGV("Set '%s'.%d to %d\n",
			// route[i].ctl_name, j, route[i].intval);
		}
	    }
        }
    }

    return 0;
}

struct tiny_dev_cfg {
    int mask;

    struct route_setting *on;
    unsigned int on_len;

    struct route_setting *off;
    unsigned int off_len;
};

struct tiny_audio_device {
    struct audio_hw_device device;
    struct mixer *mixer;

    int mode;

    pthread_mutex_t route_lock;
    struct tiny_dev_cfg *dev_cfgs;
    int num_dev_cfgs;
    unsigned int active_devices;
    unsigned int devices;	
    bool mic_mute;
    
    int in_call;
    struct tiny_stream_in *active_input;
    struct tiny_stream_out *active_output;	
	int is_pcm_in_active;
	int is_pcm_out_active;
};

struct tiny_stream_out {
    struct audio_stream_out stream;

	pthread_mutex_t lock;
    struct tiny_audio_device *adev;

    struct pcm_config config;
    struct pcm *pcm;
};

#define MAX_PREPROCESSORS 10

struct tiny_stream_in {
    struct audio_stream_in stream;

    pthread_mutex_t lock;

    struct tiny_audio_device *adev;

    struct pcm_config config;
    struct pcm *pcm;

    struct resampler_itfe *resampler;
    struct resampler_buffer_provider buf_provider;
    int16_t *buffer;
    size_t frames_in;
    unsigned int requested_rate;
    int standby;
    int source;
    effect_handle_t preprocessors[MAX_PREPROCESSORS];
    int num_preprocessors;
    int16_t *proc_buf;
    size_t proc_buf_size;
    size_t proc_frames_in;
    int read_status;    
    //struct echo_reference_itfe *echo_reference;
    int stereo_to_mono;
};

// pcm read returned with -1
struct pcm_config pcm_config_c= {
    .channels = 2,
    .rate = 44100,
    .period_size = 1024,
    .period_count = 4,
    .format = PCM_FORMAT_S16_LE,
};
    
// pcm read returned with -1
//struct pcm_config pcm_config_c= {
//    .channels = 2,
//    .rate = 44100,
//    .period_count = 4,
//    .period_size = 320,    
//    .format = PCM_FORMAT_S16_LE,
//};

// pcm read returned with -1  
//struct pcm_config pcm_config_c= {
//    .channels = 2,
//    .rate = 8000,
//    .period_size = 160,
//    .period_count = 2,
//    .format = PCM_FORMAT_S16_LE,
//};

/* Must be called with route_lock */
void select_input_devices(struct tiny_audio_device *adev)
{
    int i;
	unsigned int output = ((((unsigned int)adev->active_devices) & ((unsigned int)AUDIO_DEVICE_OUT_ALL)) | (((unsigned int)adev->devices) & ((unsigned int)AUDIO_DEVICE_IN_ALL)));
	
    if (adev->active_devices == output)
	return;

    ALOGV("Changing devices %x => %x\n", adev->active_devices, output);

    /* Turn on new devices first so we don't glitch due to powerdown... */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if ((output & adev->dev_cfgs[i].mask) &&
	    !(adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].on,
			       adev->dev_cfgs[i].on_len);

    /* ...then disable old ones. */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if (!(output & adev->dev_cfgs[i].mask) &&
	    (adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].off,
			       adev->dev_cfgs[i].off_len);

    adev->active_devices = output;
}

/* Must be called with route_lock */
void deselect_all_output_devices(struct tiny_audio_device *adev)
{
    int i;
	unsigned int output = (((unsigned int)adev->devices) & (~((unsigned int)AUDIO_DEVICE_OUT_ALL)));
	
    if (adev->active_devices == output)
	return;

    ALOGV("Changing devices %x => %x\n", adev->active_devices, output);

    /* Turn on new devices first so we don't glitch due to powerdown... */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if ((output & adev->dev_cfgs[i].mask) &&
	    !(adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].on,
			       adev->dev_cfgs[i].on_len);

    /* ...then disable old ones. */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if (!(output & adev->dev_cfgs[i].mask) &&
	    (adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].off,
			       adev->dev_cfgs[i].off_len);

    adev->active_devices = output;
}
  
/* Must be called with route_lock */
void select_devices(struct tiny_audio_device *adev)
{
    int i;

    if (adev->active_devices == adev->devices)
	return;

    ALOGV("Changing devices %x => %x\n", adev->active_devices, adev->devices);

    /* Turn on new devices first so we don't glitch due to powerdown... */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if ((adev->devices & adev->dev_cfgs[i].mask) &&
	    !(adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].on,
			       adev->dev_cfgs[i].on_len);

    /* ...then disable old ones. */
    for (i = 0; i < adev->num_dev_cfgs; i++)
	if (!(adev->devices & adev->dev_cfgs[i].mask) &&
	    (adev->active_devices & adev->dev_cfgs[i].mask))
	    set_route_by_array(adev->mixer, adev->dev_cfgs[i].off,
			       adev->dev_cfgs[i].off_len);

    adev->active_devices = adev->devices;
}

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    return 44100;
}

static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    if (rate == out_get_sample_rate(stream))
		return 0;
    else
		return -EINVAL;
}

static size_t out_get_buffer_size(const struct audio_stream *stream)
{
    return 4096;
}

static uint32_t out_get_channels(const struct audio_stream *stream)
{
    return AUDIO_CHANNEL_OUT_STEREO;
}

static audio_format_t out_get_format(const struct audio_stream *stream)
{
    return AUDIO_FORMAT_PCM_16_BIT;
}

static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    return 0;
}

static int out_standby(struct audio_stream *stream)
{
    struct tiny_stream_out *out = (struct tiny_stream_out *)stream;
    struct tiny_audio_device *adev = out->adev;
    int ret;

	pthread_mutex_lock(&adev->route_lock);
	pthread_mutex_lock(&out->lock);
    
    if (out->pcm) {
		
		if(!adev->in_call){			
			deselect_all_output_devices(adev);
			usleep(5000);
		}else{
			ALOGV("out_standby(%p) INCALL not deselecting output\n", stream);
		}
				
		ALOGV("out_standby(%p) closing PCM\n", stream);
		ret = pcm_close(out->pcm);
					
		if (ret != 0) {
			ALOGE("out_standby(%p) failed: %d\n", stream, ret);
			pthread_mutex_unlock(&out->lock);
			pthread_mutex_unlock(&adev->route_lock);
			return ret;
		}	
		
		adev->is_pcm_out_active = 0;
		out->pcm = NULL;							
    }
	pthread_mutex_unlock(&out->lock);
	pthread_mutex_unlock(&adev->route_lock);
    return 0;
}

static int out_dump(const struct audio_stream *stream, int fd)
{
    return 0;
}

static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    struct tiny_stream_out *out = (struct tiny_stream_out *)stream;
    struct tiny_audio_device *adev = out->adev;
    struct str_parms *parms;
    char *str;
    char value[32];
    int ret, val = 0;
    bool force_input_standby = false;

    ALOGV("%s\n",__FUNCTION__);
    ALOGV("%s: kvpairs:%s\n",__FUNCTION__,kvpairs);
    
    parms = str_parms_create_str(kvpairs);

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_ROUTING,
			    value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
		ALOGV("%s: stream routing val:%d\n",__FUNCTION__,val);
		if (val != 0) {
			//pthread_mutex_lock(&adev->route_lock);
			//    adev->devices &= ~AUDIO_DEVICE_OUT_ALL;
			//    adev->devices |= val;
			//    select_devices(adev);
			//pthread_mutex_unlock(&adev->route_lock);
		} else {
			ALOGW("output routing with no devices\n");
		}
    }

    str_parms_destroy(parms);

    return ret;
}

static char * out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    return strdup("");
}

static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
    return 0;
}

static int out_set_volume(struct audio_stream_out *stream, float left,
                          float right)
{
    /* Use the soft volume control for now; AudioFlinger rarely
     * actually calls down. */
    return -EINVAL;
}

static ssize_t out_write(struct audio_stream_out *stream, const void* buffer,
                         size_t bytes)
{
    struct tiny_stream_out *out = (struct tiny_stream_out *)stream;
    int ret;
    struct tiny_audio_device *adev = out->adev;

	//ALOGV("out_write(%p)\n", stream);
	
	pthread_mutex_lock(&adev->route_lock);
	pthread_mutex_lock(&out->lock);
	
	adev->active_output = out;
	
    if (!out->pcm) {
		//ALOGV("routing output1\n");
		//select_devices(adev);
		//pthread_mutex_unlock(&adev->route_lock);
		//usleep(5000);
		//pthread_mutex_lock(&adev->route_lock);
		ALOGV("out_write(%p) opening PCM\n", stream);				
		out->pcm = pcm_open(0, 0, PCM_OUT | PCM_MMAP, &out->config);							
		if (!pcm_is_ready(out->pcm)) {
			ALOGE("Failed to open output PCM: %s", pcm_get_error(out->pcm));
			pcm_close(out->pcm);			
			deselect_all_output_devices(adev);
			pthread_mutex_unlock(&out->lock);
			pthread_mutex_unlock(&adev->route_lock);
			return -EBUSY;
		}		
				
		pthread_mutex_unlock(&adev->route_lock);
		usleep(5000);
		//pthread_mutex_lock(&adev->route_lock);
		//deselect_all_output_devices(adev);
		//pthread_mutex_unlock(&adev->route_lock);
		//usleep(5000);
		pthread_mutex_lock(&adev->route_lock);
		ALOGV("routing output2\n");
		select_devices(adev);
		adev->is_pcm_out_active = 1;
		
    }	
	
    pthread_mutex_unlock(&adev->route_lock);
    ret = pcm_mmap_write(out->pcm, buffer, bytes);
    
    pthread_mutex_unlock(&out->lock);
        
    if (ret != 0) {
		ALOGE("out_write(%p) failed: %d\n", stream, ret);
		return ret;
    }

    return bytes;
}

static int out_get_render_position(const struct audio_stream_out *stream,
                                   uint32_t *dsp_frames)
{
    return -EINVAL;
}

static int out_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return 0;
}

static int out_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return 0;
}

/** audio_stream_in implementation **/

static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    //return 8000;
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;
    return in->requested_rate;
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    return 0;
}

static uint32_t in_get_channels(const struct audio_stream *stream)
{
    //return AUDIO_CHANNEL_IN_MONO;    
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;

   // ALOGV("%s: channels:%d\n",__FUNCTION__,in->config.channels);
    
    //if (in->config.channels == 1) {
	if (in->stereo_to_mono == 1) {
        return AUDIO_CHANNEL_IN_MONO;
    } else {
        return AUDIO_CHANNEL_IN_STEREO;
    }
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    return AUDIO_FORMAT_PCM_16_BIT;
}

static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    return 0;
}

static int in_standby(struct audio_stream *stream)
{
    //{RD}       
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;
    struct tiny_audio_device *adev = in->adev;
    
    int ret;
	pthread_mutex_lock(&adev->route_lock);
	pthread_mutex_lock(&in->lock);
    
    if (in->pcm) {
		//ALOGV("in_standby(%p) NOT closing PCM\n", stream);
		//deselect_all_output_devices(adev);
		//usleep(200000);
		//adev->is_standby = 1;
				
		ALOGV("in_standby(%p) closing PCM\n", stream);
		ret = pcm_close(in->pcm);
		
		//if (in->echo_reference != NULL) {
		//	stop reading from echo reference
		//	in->echo_reference->read(in->echo_reference, NULL);
		//	put_echo_reference(adev, in->echo_reference);
		//	in->echo_reference = NULL;
		//}
					
		if (ret != 0) {
			ALOGE("in_standby(%p) failed: %d\n", stream, ret);
			pthread_mutex_unlock(&in->lock);
			pthread_mutex_unlock(&adev->route_lock);
			return ret;
		}
		
		adev->is_pcm_in_active = 0;
		in->pcm = NULL;
	
    }
	pthread_mutex_unlock(&in->lock);
	pthread_mutex_unlock(&adev->route_lock);
    return 0;
}

static int in_dump(const struct audio_stream *stream, int fd)
{
    return 0;
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    //ALOGV("%s: empty\n",__FUNCTION__);
    //return 0;
    
    ALOGV("%s\n",__FUNCTION__);
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;
    struct tiny_audio_device *adev = in->adev;
    struct str_parms *parms;
    char *str;
    char value[32];
    int ret, val = 0;
    //bool do_standby = false;

	ALOGV("%s: kvpairs:%s\n",__FUNCTION__,kvpairs);
    parms = str_parms_create_str(kvpairs);

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_INPUT_SOURCE, value, sizeof(value));

    //pthread_mutex_lock(&adev->lock);
    //pthread_mutex_lock(&in->lock);
    if (ret >= 0) {
        val = atoi(value);
        ALOGV("%s: input source val:%d\n",__FUNCTION__,val);
        /* no audio source uses val == 0 */
        if ((in->source != val) && (val != 0)) {
            in->source = val;
            //do_standby = true;
        }
    }

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_ROUTING, value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
        ALOGV("%s: stream routing val:%d :ignoring\n",__FUNCTION__,val);
        /*if ((adev->devices != val) && (val != 0)) {
            adev->devices = val;
            //do_standby = true;
        }*/
    }

    //if (do_standby)
    //    do_input_standby(in);
    //pthread_mutex_unlock(&in->lock);
    //pthread_mutex_unlock(&adev->lock);

    str_parms_destroy(parms);
    
    return 0;
}

static char * in_get_parameters(const struct audio_stream *stream,
                                const char *keys)
{
    return strdup("");
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    return 0;
}

static void release_buffer(struct resampler_buffer_provider *buffer_provider,
                                  struct resampler_buffer* buffer)
{
    struct tiny_stream_in *in;

    if (buffer_provider == NULL || buffer == NULL)
        return;

    in = (struct tiny_stream_in *)((char *)buffer_provider -
                                   offsetof(struct tiny_stream_in, buf_provider));

    in->frames_in -= buffer->frame_count;
}

static int get_next_buffer(struct resampler_buffer_provider *buffer_provider,
                                   struct resampler_buffer* buffer)
{
    struct tiny_stream_in *in;

    if (buffer_provider == NULL || buffer == NULL)
        return -EINVAL;

    in = (struct tiny_stream_in *)((char *)buffer_provider -
                                   offsetof(struct tiny_stream_in, buf_provider));

    if (in->pcm == NULL) {
        buffer->raw = NULL;
        buffer->frame_count = 0;
        in->read_status = -ENODEV;
        return -ENODEV;
    }

    if (in->frames_in == 0) {
        in->read_status = pcm_read(in->pcm,
                                   (void*)in->buffer,
                                   in->config.period_size *
                                       audio_stream_frame_size(&in->stream.common));
        if (in->read_status != 0) {
            //ALOGE("get_next_buffer() pcm_read error %d", in->read_status);
            ALOGE("get_next_buffer() pcm_read error: %s", pcm_get_error(in->pcm));
            buffer->raw = NULL;
            buffer->frame_count = 0;
            return in->read_status;
        }
        in->frames_in = in->config.period_size;
    }

    buffer->frame_count = (buffer->frame_count > in->frames_in) ?
                                in->frames_in : buffer->frame_count;
    buffer->i16 = in->buffer + (in->config.period_size - in->frames_in) *
                                                in->config.channels;

    return in->read_status;

}

/* read_frames() reads frames from kernel driver, down samples to capture rate
 * if necessary and output the number of frames requested to the buffer specified */
static ssize_t read_frames(struct tiny_stream_in *in, void *buffer, ssize_t frames)
{
    ssize_t frames_wr = 0;

    while (frames_wr < frames) {
        size_t frames_rd = frames - frames_wr;
        if (in->resampler != NULL) {
            in->resampler->resample_from_provider(in->resampler,
                    (int16_t *)((char *)buffer +
                            frames_wr * audio_stream_frame_size(&in->stream.common)),
                    &frames_rd);
        } else {
			ALOGV("!!! in->resampler == NULL");
            struct resampler_buffer buf = {
                    { raw : NULL, },
                    frame_count : frames_rd,
            };
            get_next_buffer(&in->buf_provider, &buf);
            if (buf.raw != NULL) {
                memcpy((char *)buffer +
                           frames_wr * audio_stream_frame_size(&in->stream.common),
                        buf.raw,
                        buf.frame_count * audio_stream_frame_size(&in->stream.common));
                frames_rd = buf.frame_count;
            }
            release_buffer(&in->buf_provider, &buf);
        }
        /* in->read_status is updated by getNextBuffer() also called by
         * in->resampler->resample_from_provider() */
        if (in->read_status != 0)
            return in->read_status;

        frames_wr += frames_rd;
    }
    return frames_wr;
}

  static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes)
{   
    int ret = 0;
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;
    struct tiny_audio_device *adev = in->adev;
    size_t frames_rq = bytes / audio_stream_frame_size(&stream->common);    
    
    //ALOGV("in_read(%p)\n", stream);
    //ALOGV("bytes: %d\n", bytes);
    //ALOGV("frames_rq: %d\n", frames_rq);
    
    pthread_mutex_lock(&adev->route_lock);
    pthread_mutex_lock(&in->lock);
    
    //if (in->need_echo_reference && in->echo_reference == NULL)
    //    in->echo_reference = get_echo_reference(adev,
    //                                    AUDIO_FORMAT_PCM_16_BIT,
    //                                    in->config.channels,
    //                                    in->requested_rate);

	adev->active_input = in;
	                                        
    if (!in->pcm) {
		ALOGV("in_read(%p) opening PCM\n", stream);	
		in->pcm = pcm_open(0, 0, PCM_IN, &in->config);	
		
		usleep(5000);
		
		if (!pcm_is_ready(in->pcm)) {
			ALOGE("cannot open pcm_in driver: %s", pcm_get_error(in->pcm));
			pcm_close(in->pcm);
			ret = -ENOMEM;
			pthread_mutex_unlock(&adev->route_lock);
			goto exit;
		}
		
		ALOGV("routing output\n");		
		select_input_devices(adev);	
		
		adev->is_pcm_in_active = 1;
		ALOGV("in_read(%p) opened PCM successfully\n", stream);	
    }

	if (in->resampler) {
        in->resampler->reset(in->resampler);
        in->frames_in = 0;
    }
    
    pthread_mutex_unlock(&adev->route_lock);
    
    if (in->resampler != NULL)
    {        
        if(in->stereo_to_mono){
			ALOGV("stereo_to_mono");
			
			ALOGV("read bytes : %d",bytes);
			unsigned char rdbuff[bytes<<1];
			ALOGV("actual read bytes<<1: %d",bytes<<1);
			
			ALOGV("frames_rq %d",frames_rq);
			//frames_rq =frames_rq<<1;
			//ALOGV("new frames_rq %d",frames_rq);
			
			ret = read_frames(in, rdbuff, frames_rq);			
			
			int tmpsize = ret * audio_stream_frame_size(&in->stream.common);
			ALOGV("read buff size : %d",tmpsize);

			unsigned char *outbuff = (unsigned char *)buffer;
			memset(outbuff,0,bytes);
			ALOGV("CP y1");
			int itr=0;
			int itr2=0;
			
			//for(; itr2 < tmpsize;itr2++){
			//	ALOGV("%d 0x%04x",itr2, rdbuff[itr2]);								
			//}
			
			for(itr2=0; (itr2 < tmpsize) && (itr < bytes);){
				//outbuff[itr] = (unsigned char)((rdbuff[itr2]>>1) + (rdbuff[itr2]>>1));							
				
				itr2 += 2;
				outbuff[itr++] = rdbuff[itr2++];
				outbuff[itr++] = rdbuff[itr2++];				
								
				//outbuff[itr++] = rdbuff[itr2++];
				//itr2++;
				//outbuff[itr++] = rdbuff[itr2++];				
				//itr2++;
			} 			
			ALOGV("CP z1");
		}else{
			ALOGV("!stereo_to_mono");
			ALOGV("read bytes : %d",bytes);
			ALOGV("frames_rq %d",frames_rq);
			ret = read_frames(in, buffer, frames_rq);
			int tmpsize = ret * audio_stream_frame_size(&in->stream.common);
			ALOGV("read buff size : %d",tmpsize);			
		}
	}
    else{
        ret = pcm_read(in->pcm, buffer, bytes);
        if(ret != 0){
			ALOGE("in_read() pcm_read error: %s", pcm_get_error(in->pcm));
		}
    }
            
    if (ret > 0)
        ret = 0;    
    
    //if (ret == 0 && adev->mic_mute)
    //    memset(buffer, 0, bytes);
    
exit:
    if (ret < 0)
        usleep(bytes * 1000000 / audio_stream_frame_size(&stream->common) /
               in_get_sample_rate(&stream->common));    

    pthread_mutex_unlock(&in->lock);
    
    
    return bytes;
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    return 0;
}

static int in_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    ALOGV("%s: empty\n",__FUNCTION__);
    return 0;
}

static int in_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    ALOGV("%s: empty\n",__FUNCTION__);
    return 0;
}

static int out_get_next_write_timestamp(const struct audio_stream_out *stream,
                                        int64_t *timestamp)
{
    return -EINVAL;
}

static int adev_open_output_stream(struct audio_hw_device *dev,
                                   audio_io_handle_t handle,
                                   audio_devices_t devices,
                                   audio_output_flags_t flags,
                                   struct audio_config *config,
                                   struct audio_stream_out **stream_out)
{
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    struct tiny_stream_out *out;
    int ret;    

    out = calloc(1, sizeof(struct tiny_stream_out));
    if (!out)
        return -ENOMEM;

	pthread_mutex_init(&out->lock, NULL);
	
    out->stream.common.get_sample_rate = out_get_sample_rate;
    out->stream.common.set_sample_rate = out_set_sample_rate;
    out->stream.common.get_buffer_size = out_get_buffer_size;
    out->stream.common.get_channels = out_get_channels;
    out->stream.common.get_format = out_get_format;
    out->stream.common.set_format = out_set_format;
    out->stream.common.standby = out_standby;
    out->stream.common.dump = out_dump;
    out->stream.common.set_parameters = out_set_parameters;
    out->stream.common.get_parameters = out_get_parameters;
    out->stream.common.add_audio_effect = out_add_audio_effect;
    out->stream.common.remove_audio_effect = out_remove_audio_effect;
    out->stream.get_latency = out_get_latency;
    out->stream.set_volume = out_set_volume;
    out->stream.write = out_write;
    out->stream.get_render_position = out_get_render_position;
	out->stream.get_next_write_timestamp = out_get_next_write_timestamp;

    out->adev = adev;

    config->format = out_get_format(&out->stream.common);
    config->channel_mask = out_get_channels(&out->stream.common);
    config->sample_rate = out_get_sample_rate(&out->stream.common);

    /* Should query the driver for parameters and compute defaults
     * from those; should also support configuration from file and
     * buffer resizing.
     */
    out->config.channels = 2;
    out->config.rate = out_get_sample_rate(&out->stream.common);
    out->config.period_count = 4;
    out->config.period_size = 1024;
    out->config.format = PCM_FORMAT_S16_LE;

    ALOGV("%s\n",__FUNCTION__);
    ALOGV("%s sample_rate: %d\n",__FUNCTION__,config->sample_rate);
    ALOGV("%s format: %d\n",__FUNCTION__,config->format);
    ALOGV("%s channel_count: %d\n",__FUNCTION__,config->channel_mask);
    ALOGV("Opened output stream %p\n", out);

    *stream_out = &out->stream;
    return 0;

err_open:
    free(out);
    *stream_out = NULL;
    return ret;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{    
    struct tiny_stream_out *out = (struct tiny_stream_out *)stream;
    ALOGV("%s: stream %p\n",__FUNCTION__, stream);
    //ALOGV("Closing output stream %p\n", stream);
    
    pthread_mutex_lock(&out->adev->route_lock);
    pthread_mutex_lock(&out->lock);
    
    if (out->pcm){
		pcm_close(out->pcm);
		out->adev->is_pcm_out_active = 0;
		out->pcm = NULL;
    }
    out->adev->active_output = NULL;
    
    pthread_mutex_unlock(&out->lock);
    pthread_mutex_unlock(&out->adev->route_lock);
    
    free(stream);
    
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    //ALOGV("%s: error\n",__FUNCTION__);se
    //return -ENOSYS;
    
    ALOGV("%s\n",__FUNCTION__);
    
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    struct str_parms *parms;
    char *str;
    char value[32];
    int ret = 0;

	parms = str_parms_create_str(kvpairs);
	ALOGV("%s: kvpairs:%s\n",__FUNCTION__,kvpairs);
	
	ret = str_parms_get_str(parms, AUDIO_PARAMETER_KEY_BT_NREC, value, sizeof(value));
    if (ret >= 0) {
		ALOGV("%s: AUDIO_PARAMETER_KEY_BT_NREC\n",__FUNCTION__);
        if (strcmp(value, AUDIO_PARAMETER_VALUE_ON) == 0){
            //adev->bluetooth_nrec = true;
            ALOGV("%s: BT ON\n",__FUNCTION__);
		}
        else{
            //adev->bluetooth_nrec = false;
            ALOGV("%s: BT OFF\n",__FUNCTION__);
		}
    }
    
	str_parms_destroy(parms);
    return ret;
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    //return NULL;
    return strdup("");
}

static int adev_init_check(const struct audio_hw_device *dev)
{
    return 0;
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    ALOGV("%s: error\n",__FUNCTION__);
    return -ENOSYS;
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    return -ENOSYS;
}

static void force_all_standby(struct tiny_audio_device *adev)
{
    struct tiny_stream_in *in;
    struct tiny_stream_out *out;

    if (adev->active_output) {
        out = adev->active_output;
        
        pthread_mutex_lock(&out->lock);        
        if (out->pcm){
			deselect_all_output_devices(adev);
			usleep(5000);
			pcm_close(out->pcm);
			out->pcm = NULL;		
		}
		adev->is_pcm_out_active = 0;		
        pthread_mutex_unlock(&out->lock);
    }

    if (adev->active_input) {
        in = adev->active_input;
        
        pthread_mutex_lock(&in->lock);
        if (in->pcm){		
			pcm_close(in->pcm);
			in->pcm = NULL;
		}
		adev->is_pcm_in_active = 0;
        pthread_mutex_unlock(&in->lock);
    }
}

static void select_mode(struct tiny_audio_device *adev, int new_mode)
{
	ALOGV("%s\n",__FUNCTION__);
	//{RD} ToDo input output channel opening/closing with change of mode
    
    if ((new_mode == AUDIO_MODE_IN_CALL)||(new_mode == AUDIO_MODE_RINGTONE)) {
		ALOGV("Entering IN_CALL/RingTone state");
		adev->mode == new_mode;
							
		//To-Do Close PCM's ?
		if (!adev->in_call) {                                                          
			ALOGI("enabling 3G audio!\n");
			adev->devices |= AUDIO_DEVICE_IN_VOICE_CALL;				
			select_devices(adev);			            
            adev->in_call = 1;
        }
    } else if(new_mode == AUDIO_MODE_NORMAL){
		adev->mode == new_mode;
		ALOGV("Entering normal state");
        if (adev->in_call) {
            adev->in_call = 0;
            ALOGI("disabling 3G audio!\n");
            adev->devices &= ~AUDIO_DEVICE_IN_VOICE_CALL;				
			deselect_all_output_devices(adev);
			usleep(5000);						
			if(adev->is_pcm_out_active){																	
				ALOGV("closing PCM\n");							
				if (pcm_close(adev->active_output->pcm))
					ALOGE("failed to close PCM \n");
				else{
					adev->active_output->pcm = NULL;
					adev->is_pcm_out_active = 0;				
				}
			}
				
        }
    }else {
		ALOGW("Unknown Mode:%d! ignoring change request",new_mode);
     }
}

static int adev_set_mode(struct audio_hw_device *dev, int mode)
{
    //ALOGV("%s: empty\n",__FUNCTION__);
    //return 0;
    
    //ALOGV("%s\n",__FUNCTION__);    
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    ALOGV("%s Mode old:%d new:%d\n",__FUNCTION__,adev->mode,mode);
    pthread_mutex_lock(&adev->route_lock);
    if (adev->mode != mode) {
        //adev->mode = mode;
        select_mode(adev,mode);
    }
    pthread_mutex_unlock(&adev->route_lock);

    return 0;
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    //ALOGV("%s\n: error",__FUNCTION__);
    //return -ENOSYS;
    
    ALOGV("%s\n",__FUNCTION__);
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    adev->mic_mute = state;    
    return 0;
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    //ALOGV("%s: error\n",__FUNCTION__);
    //return -ENOSYS;
    ALOGV("%s\n",__FUNCTION__);
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    *state = adev->mic_mute;
    return 0;    
}

static size_t get_input_buffer_size(uint32_t sample_rate, int format, int channel_count)
{
    //size_t size;    
    /* take resampling into account and return the closest majoring
    multiple of 16 frames, as audioflinger expects audio buffers to
    be a multiple of 16 frames */
    //size = (320 * sample_rate) / 44100;
    //size = ((size + 15) / 16) * 16;
    //return size * channel_count * sizeof(short);
    
    size_t size;
    size = (pcm_config_c.period_size * sample_rate) / pcm_config_c.rate;
    size = ((size + 15) / 16) * 16;
    size = size * channel_count * sizeof(short);    
    
    //size_t size;
    //if(sample_rate == 8000)
	//	size = pcm_config_c.period_size/4;
    //else
	//	size = pcm_config_c.period_size;
    //size = size * channel_count * sizeof(short);
    
    ALOGV("%s: channel_count:%d\n",__FUNCTION__,channel_count);
    ALOGV("%s: sample_rate:%d\n",__FUNCTION__,sample_rate);
    ALOGV("%s: size:%d\n",__FUNCTION__,size);
    
    return size;
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
	// {RD}
	//return 320;
	struct tiny_stream_in *in = (struct tiny_stream_in *)stream;

	if(in->stereo_to_mono)
		return get_input_buffer_size(in->requested_rate,
                                 AUDIO_FORMAT_PCM_16_BIT,
                                 1);
	 else
		return get_input_buffer_size(in->requested_rate,
                                 AUDIO_FORMAT_PCM_16_BIT,
                                 2);                                                                     
}

static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                         const struct audio_config *config)
{
    //return 320;
    return get_input_buffer_size(config->sample_rate, config->format, config->channel_mask);
}

static int adev_open_input_stream(struct audio_hw_device *dev,
                                  audio_io_handle_t handle,
                                  audio_devices_t devices,
                                  struct audio_config *config,
                                  struct audio_stream_in **stream_in)
{
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    struct tiny_stream_in *in;
    int ret;
    int channel_count = popcount(config->channel_mask);

    ALOGV("%s\n",__FUNCTION__);
    ALOGV("%s sample_rate: %d\n",__FUNCTION__,config->sample_rate);
    ALOGV("%s format: %d\n",__FUNCTION__,config->format);
    ALOGV("%s channel_count: %d\n",__FUNCTION__,channel_count);

    in = calloc(1, sizeof(struct tiny_stream_in));
    if (!in)
        return -ENOMEM;

    pthread_mutex_init(&in->lock, NULL);
    in->adev = adev;

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    in->config.channels = 2;
    in->config.rate = pcm_config_c.rate;
    in->config.period_count = pcm_config_c.period_count;
    in->config.period_size = pcm_config_c.period_size;
    in->config.format = PCM_FORMAT_S16_LE;
    ALOGV("%s in->config.rate: %d\n",__FUNCTION__,in->config.rate);
    ALOGV("%s in->config.period_count: %d\n",__FUNCTION__,in->config.period_count);
    ALOGV("%s in->config.period_size: %d\n",__FUNCTION__,in->config.period_size);
               
    // {RD}
    // *stream_in = &in->stream;
    in->requested_rate = config->sample_rate;
    
	if((in->requested_rate != in->config.rate)&&(channel_count != 2))
		in->stereo_to_mono = 1;
	else
		in->stereo_to_mono = 0;
	
    in->buffer = malloc(in->config.period_size *
                        audio_stream_frame_size(&in->stream.common));
    if (!in->buffer) {
        ret = -ENOMEM;
        goto err;
    }

    if (in->requested_rate != in->config.rate) {
        in->buf_provider.get_next_buffer = get_next_buffer;
        in->buf_provider.release_buffer = release_buffer;

	/*if(in->stereo_to_mono)
        ret = create_resampler(in->config.rate,                               
                               in->requested_rate,
                               1,
                               RESAMPLER_QUALITY_DEFAULT,
                               &in->buf_provider,
                               &in->resampler);
	   else*/
		   ret = create_resampler(in->config.rate,                               
                               in->requested_rate,
                               2,
                               RESAMPLER_QUALITY_DEFAULT,
                               &in->buf_provider,
                               &in->resampler);
                               
        if (ret != 0) {
            ret = -EINVAL;
            goto err;
        }
    }
    
    *stream_in = &in->stream;
    
    return 0;

err:	
	if (in->resampler)
        release_resampler(in->resampler);
        
err_open:	        
    free(in);
    *stream_in = NULL;
    
    ALOGE("%s returning with Error:%d\n",__FUNCTION__,ret);
    
    return ret;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                   struct audio_stream_in *stream)
{	
    struct tiny_stream_in *in = (struct tiny_stream_in *)stream;
    
    ALOGV("%s\n",__FUNCTION__);
	pthread_mutex_lock(&in->adev->route_lock);
    pthread_mutex_lock(&in->lock);
    if (in->pcm){
		pcm_close(in->pcm);
		in->pcm = NULL;
		in->adev->is_pcm_in_active = 0;
    } 
    in->adev->active_input = NULL;
	pthread_mutex_unlock(&in->lock);
    pthread_mutex_unlock(&in->adev->route_lock);     
    
    if (in->resampler) {
        free(in->buffer);
        release_resampler(in->resampler);
    }
    
    free(in);
    return;
}

static int adev_dump(const audio_hw_device_t *device, int fd)
{
    ALOGV("%s: empty ok\n",__FUNCTION__);
    return 0;
}

static int adev_close(hw_device_t *device)
{	
    struct tiny_audio_device *adev = (struct tiny_audio_device *)device;
    
    ALOGV("%s\n",__FUNCTION__);    
    mixer_close(adev->mixer);
    free(device);
    return 0;
}

static uint32_t adev_get_supported_devices(const struct audio_hw_device *dev)
{
    struct tiny_audio_device *adev = (struct tiny_audio_device *)dev;
    uint32_t supported = 0;
    int i;

    ALOGV("%s\n",__FUNCTION__);
	
    //for (i = 0; i < adev->num_dev_cfgs; i++)
	//supported |= adev->dev_cfgs[i].mask;
    //return supported;
    
    return (/* OUT */
            AUDIO_DEVICE_OUT_EARPIECE |
            AUDIO_DEVICE_OUT_SPEAKER |
            AUDIO_DEVICE_OUT_WIRED_HEADSET |
            AUDIO_DEVICE_OUT_WIRED_HEADPHONE |
            AUDIO_DEVICE_OUT_ALL_SCO |
            AUDIO_DEVICE_OUT_DEFAULT |
            /* IN */
            AUDIO_DEVICE_IN_COMMUNICATION |
            AUDIO_DEVICE_IN_AMBIENT |
            AUDIO_DEVICE_IN_BUILTIN_MIC |
            AUDIO_DEVICE_IN_WIRED_HEADSET |
            AUDIO_DEVICE_IN_BACK_MIC |
            AUDIO_DEVICE_IN_ALL_SCO |
            AUDIO_DEVICE_IN_DEFAULT);
}

struct config_parse_state {
    struct tiny_audio_device *adev;
    struct tiny_dev_cfg *dev;
    bool on;

    struct route_setting *path;
    unsigned int path_len;
};

static const struct {
    int mask;
    const char *name;
} dev_names[] = {
    { AUDIO_DEVICE_OUT_SPEAKER, "speaker" },
    { AUDIO_DEVICE_OUT_WIRED_HEADSET | AUDIO_DEVICE_OUT_WIRED_HEADPHONE,
      "headphone" },
    { AUDIO_DEVICE_OUT_EARPIECE, "earpiece" },
    { AUDIO_DEVICE_OUT_ANLG_DOCK_HEADSET, "analog-dock" },
    { AUDIO_DEVICE_OUT_DGTL_DOCK_HEADSET, "digital-dock" },

    { AUDIO_DEVICE_IN_COMMUNICATION, "comms" },
    { AUDIO_DEVICE_IN_AMBIENT, "ambient" },
    { AUDIO_DEVICE_IN_BUILTIN_MIC, "builtin-mic" },
    { AUDIO_DEVICE_IN_VOICE_CALL, "3g-output"},
    { AUDIO_DEVICE_IN_WIRED_HEADSET, "headset" },
    { AUDIO_DEVICE_IN_AUX_DIGITAL, "digital" },
    { AUDIO_DEVICE_IN_BACK_MIC, "back-mic" },
};

static void adev_config_start(void *data, const XML_Char *elem,
			      const XML_Char **attr)
{
    struct config_parse_state *s = data;
    struct tiny_dev_cfg *dev_cfg;
    const XML_Char *name = NULL;
    const XML_Char *val = NULL;
    unsigned int i, j;

    for (i = 0; attr[i]; i += 2) {
	if (strcmp(attr[i], "name") == 0)
	    name = attr[i + 1];

	if (strcmp(attr[i], "val") == 0)
	    val = attr[i + 1];
    }

    if (strcmp(elem, "device") == 0) {
	if (!name) {
	    ALOGE("Unnamed device\n");
	    return;
	}

	for (i = 0; i < sizeof(dev_names) / sizeof(dev_names[0]); i++) {
	    if (strcmp(dev_names[i].name, name) == 0) {
		//ALOGI("Allocating device %s\n", name);
		dev_cfg = realloc(s->adev->dev_cfgs,
				  (s->adev->num_dev_cfgs + 1)
				  * sizeof(*dev_cfg));
		if (!dev_cfg) {
		    ALOGE("Unable to allocate dev_cfg\n");
		    return;
		}

		s->dev = &dev_cfg[s->adev->num_dev_cfgs];
		memset(s->dev, 0, sizeof(*s->dev));
		s->dev->mask = dev_names[i].mask;

		s->adev->dev_cfgs = dev_cfg;
		s->adev->num_dev_cfgs++;
	    }
	}

    } else if (strcmp(elem, "path") == 0) {
	if (s->path_len)
	    ALOGW("Nested paths\n");

	/* If this a path for a device it must have a role */
	if (s->dev) {
	    /* Need to refactor a bit... */
	    if (strcmp(name, "on") == 0) {
		s->on = true;
	    } else if (strcmp(name, "off") == 0) {
		s->on = false;
	    } else {
		ALOGW("Unknown path name %s\n", name);
	    }
	}

    } else if (strcmp(elem, "ctl") == 0) {
	struct route_setting *r;

	if (!name) {
	    ALOGE("Unnamed control\n");
	    return;
	}

	if (!val) {
	    ALOGE("No value specified for %s\n", name);
	    return;
	}

	//ALOGV("Parsing control %s => %s\n", name, val);

	r = realloc(s->path, sizeof(*r) * (s->path_len + 1));
	if (!r) {
	    ALOGE("Out of memory handling %s => %s\n", name, val);
	    return;
	}

	r[s->path_len].ctl_name = strdup(name);
	r[s->path_len].strval = NULL;

	/* This can be fooled but it'll do */
	r[s->path_len].intval = atoi(val);
	if (!r[s->path_len].intval && strcmp(val, "0") != 0)
	    r[s->path_len].strval = strdup(val);

	s->path = r;
	s->path_len++;
    }
}

static void adev_config_end(void *data, const XML_Char *name)
{
    struct config_parse_state *s = data;
    unsigned int i;

    if (strcmp(name, "path") == 0) {
	if (!s->path_len)
	    ALOGW("Empty path\n");

	if (!s->dev) {
	    //ALOGV("Applying %d element default route\n", s->path_len);

	    set_route_by_array(s->adev->mixer, s->path, s->path_len);

	    for (i = 0; i < s->path_len; i++) {
		free(s->path[i].ctl_name);
		free(s->path[i].strval);
	    }

	    free(s->path);

	    /* Refactor! */
	} else if (s->on) {
	    //ALOGV("%d element on sequence\n", s->path_len);
	    s->dev->on = s->path;
	    s->dev->on_len = s->path_len;

	} else {
	    //ALOGV("%d element off sequence\n", s->path_len);

	    /* Apply it, we'll reenable anything that's wanted later */
	    set_route_by_array(s->adev->mixer, s->path, s->path_len);

	    s->dev->off = s->path;
	    s->dev->off_len = s->path_len;
	}

	s->path_len = 0;
	s->path = NULL;

    } else if (strcmp(name, "device") == 0) {
	s->dev = NULL;
    }
}

static int adev_config_parse(struct tiny_audio_device *adev)
{
    struct config_parse_state s;
    FILE *f;
    XML_Parser p;
    char property[PROPERTY_VALUE_MAX];
    char file[80];
    int ret = 0;
    bool eof = false;
    int len;

    property_get("ro.product.device", property, "tiny_hw");
    snprintf(file, sizeof(file), "/system/etc/sound/%s.xml", property);

    ALOGV("Reading configuration from %s\n", file);
    f = fopen(file, "r");
    if (!f) {
	ALOGE("Failed to open %s\n", file);
	return -ENODEV;
    }

    p = XML_ParserCreate(NULL);
    if (!p) {
	ALOGE("Failed to create XML parser\n");
	ret = -ENOMEM;
	goto out;
    }

    memset(&s, 0, sizeof(s));
    s.adev = adev;
    XML_SetUserData(p, &s);

    XML_SetElementHandler(p, adev_config_start, adev_config_end);

    while (!eof) {
	len = fread(file, 1, sizeof(file), f);
	if (ferror(f)) {
	    ALOGE("I/O error reading config\n");
	    ret = -EIO;
	    goto out_parser;
	}
	eof = feof(f);

	if (XML_Parse(p, file, len, eof) == XML_STATUS_ERROR) {
	    ALOGE("Parse error at line %u:\n%s\n",
		 (unsigned int)XML_GetCurrentLineNumber(p),
		 XML_ErrorString(XML_GetErrorCode(p)));
	    ret = -EINVAL;
	    goto out_parser;
	}
    }

 out_parser:
    XML_ParserFree(p);
 out:
    fclose(f);

    return ret;
}

// {RD} BEGIN:
void *event_listner(void *arg){
	
	struct input_event ev[64];
	int fd, rc, rd, value, i, size = sizeof (struct input_event);
	size_t numEventsRead = 0;
	short flag_mic_short, flag_mic_detect, flag_valid;	
	struct tiny_audio_device *adev;
	char buf[32];
	char path_h[] ="/dev/input/";
	char node_name[] ="omap3beagle Headset";
	DIR           *d;
	struct dirent *dir;
	char path[40];
	
	flag_mic_short = 0;
    flag_mic_detect = 0;
	adev = (struct tiny_audio_device *)arg;
	
	ALOGI("Starting listener thread\n");
		
	d = opendir(path_h);
	bool node_found = false;
  
	if(d){
		while ((dir = readdir(d)) != NULL){
			
			//ALOGI("dir->d_name: %s\n",dir->d_name);
			
			if(strncmp(dir->d_name,"event",5)==0){
				
				strcpy (path,path_h);
				strcat (path,dir->d_name);
				//ALOGI("path: %s\n",path);
					
				if ((fd = open (path, O_RDONLY)) == -1){
					ALOGW("%s is not readable!\n",path);
					continue;
				}
				
				rc = ioctl(fd,EVIOCGNAME(sizeof(buf)),buf);
				if (rc >= 0){
					//ALOGI("EVIOCGNAME name: \"%.*s\"\n", rc, buf);
					if(strncmp(buf,node_name,((rc>19)?19:rc))==0){
						//ALOGI("found %s!!!\n",node_name);
						node_found = true;
						break;
					}else{
						//ALOGI("not omap3beagle Headset\n");
						continue;						
					}
				}
				else{
					ALOGW("EVIOCGNAME failed at: %s\n",path);
					continue;
				}
			}else{
				//ALOGI("not an event node\n");
				continue;
			}
		}
		
		closedir(d);
		if(!node_found){
			ALOGE("\"%s\" deive could not be found at %s\n",node_name,path_h);
			ALOGE("listener thread is OFFLINE\n");
			return NULL;
		}
    }else{
		ALOGE("%s is not a readable directory\n",path_h);
		ALOGE("listener thread is OFFLINE\n");
		return NULL;
	}

	ALOGI("listener thread is online\n");
	
    while (1){

      if ((rd = read (fd, ev, size * 64)) < size){
          ALOGE("event_listener event read error");
          return NULL;
	  }            
      
      numEventsRead = rd / sizeof(struct input_event);
                  
      // {RD} dont reset
      //flag_mic_short = 0;
      //flag_mic_detect = 0;
      flag_valid = 0;
      
      for(i = 0 ; i < numEventsRead; i++){
		  //ALOGI("ev[%d].code :%u\n", i, ev[i].code);
		  //ALOGI("ev[%d].type :%u\n", i, ev[i].type);
		  //ALOGI("ev[%d].value :%d\n", i, ev[i].value);
		  if(ev[i].type == 5){
			  if(ev[i].code == 2){
				  flag_valid = 1;
					if(ev[i].value)
						flag_mic_short = 1;
					else
						flag_mic_short = 0;
						
				}else if(ev[i].code == 4){
					flag_valid = 1;
					if(ev[i].value)
						flag_mic_detect = 1;
					else
						flag_mic_detect = 0;
				}
		  }		        
		}
		
		if(flag_valid){
			
			pthread_mutex_lock(&adev->route_lock);
			
			if(flag_mic_short){								
				ALOGI(" HEADPHONES pluged in!\n");
				adev->devices = AUDIO_DEVICE_OUT_WIRED_HEADSET | AUDIO_DEVICE_IN_BUILTIN_MIC;								
			}
			else if(flag_mic_detect){				
				ALOGI(" HEADSET pluged in!\n");
				adev->devices = AUDIO_DEVICE_OUT_WIRED_HEADSET | AUDIO_DEVICE_IN_WIRED_HEADSET;				
			}
			else{				
				ALOGI(" NOTHING pluged in!\n");
				adev->devices = AUDIO_DEVICE_OUT_SPEAKER | AUDIO_DEVICE_IN_BUILTIN_MIC;				
			}
			
			if(adev->in_call){
				adev->devices |= AUDIO_DEVICE_IN_VOICE_CALL;
				select_devices(adev);			
			}else{
				if(adev->is_pcm_out_active){
					
					//deselect_all_output_devices(adev);
					//usleep(5000);
					ALOGV("selecting device while PCM active\n");
					select_devices(adev);
								
					/*deselect_all_output_devices(adev);
					usleep(5000);				
					ALOGV("Closing PCM\n");
					if (pcm_close(adev->active_output->pcm)){
						ALOGE("Failed to close PCM\n");
						select_devices(adev);	
					}else{
						adev->is_pcm_out_active = 0;
						adev->active_output->pcm = NULL;
						usleep(10000);		
					}*/
	
				}else if(adev->is_pcm_in_active){
					ALOGV("selecting input device while PCM active\n");
					select_input_devices(adev);
				}
			}	
				
			pthread_mutex_unlock(&adev->route_lock);		
			
		}else{
			ALOGI("No valid flags: status is unchanged\n");
		}	
	}
	
	return NULL;
}
// {RD} END:

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)
{
    struct tiny_audio_device *adev;
    int ret;
    pthread_t thread_ID;

    ALOGV("%s\n",__FUNCTION__);
    
    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    adev = calloc(1, sizeof(struct tiny_audio_device));
    if (!adev)
        return -ENOMEM;
	
	/* Bootstrap routing */
    pthread_mutex_init(&adev->route_lock, NULL);
    pthread_mutex_lock(&adev->route_lock);
    adev->mode = AUDIO_MODE_NORMAL;
    adev->devices = AUDIO_DEVICE_OUT_SPEAKER | AUDIO_DEVICE_IN_BUILTIN_MIC;
    adev->active_devices = 0;
    adev->is_pcm_out_active = 0;
    adev->is_pcm_in_active = 0;
    
    ret = pthread_create(&thread_ID, NULL, event_listner, adev);

	if(ret){
		ALOGE("pthread_create FAILED with erro:%d\n",ret);	
		goto err;
	}
			
	ret = pthread_detach(thread_ID);	
	
	if(ret){
		ALOGE("pthread_detach FAILED with erro:%d\n",ret);
		goto err;
	}
	
    adev->device.common.tag = HARDWARE_DEVICE_TAG;
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_1_0;
    adev->device.common.module = (struct hw_module_t *) module;
    adev->device.common.close = adev_close;
    
    adev->device.get_supported_devices = adev_get_supported_devices;
    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
    adev->device.set_mode = adev_set_mode;
    adev->device.set_mic_mute = adev_set_mic_mute;
    adev->device.get_mic_mute = adev_get_mic_mute;
    adev->device.set_parameters = adev_set_parameters;
    adev->device.get_parameters = adev_get_parameters;
    adev->device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->device.open_output_stream = adev_open_output_stream;
    adev->device.close_output_stream = adev_close_output_stream;
    adev->device.open_input_stream = adev_open_input_stream;
    adev->device.close_input_stream = adev_close_input_stream;
    adev->device.dump = adev_dump;

    adev->mixer = mixer_open(0);
    if (!adev->mixer) {
	ALOGE("Failed to open mixer 0\n");
	goto err;
    }
    
    ret = adev_config_parse(adev);
    if (ret != 0)
	goto err_mixer;

    //select_devices(adev);
    deselect_all_output_devices(adev);    	
    *device = &adev->device.common;

	pthread_mutex_unlock(&adev->route_lock);

    return 0;

err_mixer:
    mixer_close(adev->mixer);
err:
    free(adev);
    return -EINVAL;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "TinyHAL",
        .author = "Mark Brown <broonie@opensource.wolfsonmicro.com>",
        .methods = &hal_module_methods,
    },
};
