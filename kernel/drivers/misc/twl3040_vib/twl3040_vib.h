#ifndef _TWL3040_VIBRATOR_H
#define _TWL3040_VIBRATOR_H

#ifndef TWL3040_VIBRATOR_MINOR
#define TWL3040_VIBRATOR_MINOR				101
#endif
#undef TWL3040_VIBRATOR_MINOR  /* {PK} This should defined only if non-conflicting minor number is pre-known */

#define DRIVER_VERSION				"1.00"

#define TWL3040_VIBRATOR_PLATFORM_NAME		"twl3040_vibrator"
#define TWL3040_VIBRATOR_DEV_PROC_DIR		"twl3040_vibrator"
#define TWL3040_VIBRATOR_DEV_NAME		"vibrator"

struct twl3040_vibrator_dev_info {
	int major;
	int minor;
	struct cdev cdev;
	struct semaphore sem;
	struct semaphore sem_is_on;
	unsigned char is_on;
	struct class *dev_class;
	struct platform_device *platform_dev;
	struct proc_dir_entry *proc_dir;
};

/* TWL Register offsets */
#define REG_TWL_LEDEN				0x00	 /* led module offsets */

#define REG_TWL_VIBRATOR_CFG 			0x05	/* TWL_MODULE_PM_RECEIVER module offsets */

#define REG_TWL_CODEC_MODE			0x01	/* TWL_MODULE_AUDIO_VOICE offsets */
#define REG_TWL_VIBRA_CTRL  			0x45
#define REG_TWL_VIBRA_SET			0x46

/* Number of vibrator counts */
#define VIBRA_LEVEL_COUNT			4


#endif /* _TWL3040_VIBRATOR_H */
