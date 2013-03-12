#ifndef _W2SG0006_H
#define _W2SG0006_H

#include "w2sg0006_ioctl.h"

#ifndef W2SG0006_MINOR
#define W2SG0006_MINOR				103
#endif
#undef W2SG0006_MINOR


/*
* Version Information
*/
#define DRIVER_VERSION				"1.00"

#define W2SG0006_PLATFORM_NAME			"tcbin_gps_w2sg0006"
#define W2SG0006_DEV_PROC_DIR			"gps"
#define W2SG0006_DEV_NAME			"gps"


struct w2sg0006_dev_info {
	int major;
	int minor;
	unsigned char is_enable;
	unsigned char is_pwr_enable;
	struct cdev cdev;
	struct semaphore sem;
	struct class *dev_class;
	struct platform_device *platform_dev;
	struct proc_dir_entry *proc_dir;
};

#endif /* _W2SG0006_H */
