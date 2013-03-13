#include "zone24x7_tcbin_Braille_BrailleDisplay.h"
#include "../../../../kernel/drivers/accessibility/braille/metec/metec_flat20_ioctl.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>

//#include <android/log.h>

#define LOGD(...) ;//__android_log_print(ANDROID_LOG_DEBUG , "BrailleDisplay", __VA_ARGS__)
#define LOGI(...) ;//__android_log_print(ANDROID_LOG_INFO   , "BrailleDisplay", __VA_ARGS__)

unsigned int braille_alphabel[] = {
0b00000000 /*00*/, 0b00000000 /*01*/, 0b00000000 /*02*/, 0b00000000 /*03*/,
0b00000000 /*04*/, 0b00000000 /*05*/, 0b00000000 /*06*/, 0b00000000 /*07*/,
0b00000000 /*08*/, 0b00000000 /*09*/, 0b00000000 /*0A*/, 0b00000000 /*0B*/,
0b00000000 /*0C*/, 0b00000000 /*0D*/, 0b00000000 /*0E*/, 0b00000000 /*0F*/,
0b00000000 /*10*/, 0b00000000 /*11*/, 0b00000000 /*12*/, 0b00000000 /*13*/,
0b00000000 /*14*/, 0b00000000 /*15*/, 0b00000000 /*16*/, 0b00000000 /*17*/,
0b00000000 /*18*/, 0b00000000 /*19*/, 0b00000000 /*1A*/, 0b00000000 /*1B*/,
0b00000000 /*1C*/, 0b00000000 /*1D*/, 0b00000000 /*1E*/, 0b00000000 /*1F*/,
0b00000000 /*20*/,
0b00101110	/*21*/, 0b00010000	/*22*/, 0b00111100	/*23*/, 0b00101011	/*24*/,
0b00101001	/*25*/, 0b00101111	/*26*/, 0b00000100	/*27*/, 0b00110111	/*28*/,
0b00111110	/*29*/, 0b00100001	/*2a*/, 0b00101100	/*2b*/, 0b00100000	/*2c*/,
0b00100100	/*2d*/, 0b00101000	/*2e*/, 0b00001100	/*2f*/, 0b00110100	/*30*/,
0b00000010	/*31*/, 0b00000110	/*32*/, 0b00010010	/*33*/, 0b00110010	/*34*/,
0b00100010	/*35*/, 0b00010110	/*36*/, 0b00110110	/*37*/, 0b00100110	/*38*/,
0b00010100	/*39*/, 0b00110001	/*3a*/, 0b00110000	/*3b*/, 0b00100001	/*3c*/,
0b00111111	/*3d*/, 0b00101100	/*3e*/, 0b00111001	/*3f*/, 0b01001000	/*40*/,
0b01000001	/*41*/, 0b01000011	/*42*/, 0b01001001	/*43*/, 0b01011001	/*44*/,
0b01010001	/*45*/, 0b01001011	/*46*/, 0b01011011	/*47*/, 0b01010011	/*48*/,
0b01001010	/*49*/, 0b01011010	/*4a*/, 0b01000101	/*4b*/, 0b01000111	/*4c*/,
0b01001101	/*4d*/, 0b01011101	/*4e*/, 0b01010101	/*4f*/, 0b01001111	/*50*/,
0b01011111	/*51*/, 0b01010111	/*52*/, 0b01001110	/*53*/, 0b01011110	/*54*/,
0b01100101	/*55*/, 0b01100111	/*56*/, 0b01111010	/*57*/, 0b01101101	/*58*/,
0b01111101	/*59*/, 0b01110101	/*5a*/, 0b01101010	/*5b*/, 0b00110011	/*5c*/,
0b01111011	/*5d*/, 0b01011000	/*5e*/, 0b00111000	/*5f*/, 0b00001000	/*60*/,
0b00000001	/*61*/, 0b00000011	/*62*/, 0b00001001	/*63*/, 0b00011001	/*64*/,
0b00010001	/*65*/, 0b00001011	/*66*/, 0b00011011	/*67*/, 0b00010011	/*68*/,
0b00001010	/*69*/, 0b00011010	/*6a*/, 0b00000101	/*6b*/, 0b00000111	/*6c*/,
0b00001101	/*6d*/, 0b00011101	/*6e*/, 0b00010101	/*6f*/, 0b00001111	/*70*/,
0b00011111	/*71*/, 0b00010111	/*72*/, 0b00001110	/*73*/, 0b00011110	/*74*/,
0b00100101	/*75*/, 0b00100111	/*76*/, 0b00111010	/*77*/, 0b00101101	/*78*/,
0b00111101	/*79*/, 0b00110101	/*7a*/, 0b00101010	/*7b*/, 0b01110011	/*7c*/,
0b00111011	/*7d*/, 0b00011000	/*7e*/
};

void
convert_to_braille(unsigned char *buffer, unsigned int buffer_len)
{
	int i = 0;

	for (i = 0; i < buffer_len; i++) {
		if( 0x20 == buffer[i] ) /* Space character*/
		{
			buffer[i] = 0;
		}
		else
		{
		//	buffer[i] = unified_braille_alphabel[buffer[i] - 0x61];
			buffer[i] = braille_alphabel[buffer[i]];
		}
	}

	return;
}

/*
 * Class:     zone24x7_tcbin_Braille_BrailleDisplay
 * Method:    nativeDisplayText
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_zone24x7_tcbin_Braille_BrailleDisplay_nativeDisplayText
  (JNIEnv * env, jobject obj, jstring text)
{
	int result = -1;
  	int fd = open("/dev/braille0", O_RDWR);
  	if (fd >= 0)
  	{
  		const char *nativeString = (*env)->GetStringUTFChars(env, text, 0);

		char buffer[20];
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, nativeString, strlen(nativeString));
		convert_to_braille((unsigned char *)buffer, strlen(nativeString));

		int ret = ioctl(fd, METEC_FLAT20_DISPLAY_WRITE, buffer);

  		(*env)->ReleaseStringUTFChars(env, text, nativeString);
  		close(fd);

		if(ret < 0) {
			LOGI("IOCTL METEC_FLAT20_DISPLAY_WRITE failed.\n");
		}
		else
		{
			LOGI("IOCTL METEC_FLAT20_DISPLAY_WRITE Successful.\n");
			result = 0;
		}
  	}
  	else
  	{
  		LOGI("Failed to open dev node /dev/braille0.\n");
  	}
	return result;
}

/*
 * Class:     zone24x7_tcbin_Braille_BrailleDisplay
 * Method:    nativeClear
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zone24x7_tcbin_Braille_BrailleDisplay_nativeClear
  (JNIEnv * env, jobject obj)
{
	int result = -1;
	int fd = open("/dev/braille0", O_RDWR);
  	if (fd >= 0)
  	{
		int ret = ioctl(fd, METEC_FLAT20_CLEAR_DISPLAY);
		close(fd);
		if(ret < 0) {
			LOGI("IOCTL METEC_FLAT20_CLEAR_DISPLAY failed.\n");
		}
		else
		{
			LOGI("IOCTL METEC_FLAT20_CLEAR_DISPLAY Successful.\n");
			result = 0;
		}

  	}
  	else
  	{
  		LOGI("Failed to open dev node /dev/braille0.\n");
  	}
	return result;
}

/*
 * Class:     zone24x7_tcbin_Braille_BrailleDisplay
 * Method:    nativeSetDotStrength
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zone24x7_tcbin_Braille_BrailleDisplay_nativeSetDotStrength
  (JNIEnv * env, jobject obj, jint dotStrength)
{
	int result = -1;
	int fd = open("/dev/braille0", O_RDWR);
  	if (fd >= 0)
  	{
		int ret = ioctl(fd, METEC_FLAT20_SET_DOT_STRENGTH);
		close(fd);
		if(ret < 0) {
			LOGI("IOCTL METEC_FLAT20_SET_DOT_STRENGTH failed.\n");
		}
		else
		{
			LOGI("IOCTL METEC_FLAT20_SET_DOT_STRENGTH Successful.\n");
			result = 0;
		}

  	}
  	else
  	{
  		LOGI("Failed to open dev node /dev/braille0.\n");
  	}
	return result;
}

/*
 * Class:     zone24x7_tcbin_Braille_BrailleDisplay
 * Method:    nativeGetRows
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zone24x7_tcbin_Braille_BrailleDisplay_nativeGetRows
  (JNIEnv * env, jobject obj)
{
	return 1;
}

/*
 * Class:     zone24x7_tcbin_Braille_BrailleDisplay
 * Method:    nativeGetColumns
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zone24x7_tcbin_Braille_BrailleDisplay_nativeGetColumns
  (JNIEnv * env, jobject obj)
{
	return 20;
}
