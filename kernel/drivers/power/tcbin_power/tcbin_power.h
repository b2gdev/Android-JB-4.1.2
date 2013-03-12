#ifndef _TCBIN_POWER_H
#define _TCBIN_POWER_H

#ifndef TCBIN_POWER_MAJOR
#define TCBIN_POWER_MAJOR	240
#endif
#undef TCBIN_POWER_MAJOR 	/* {PS} define this if specific Major is required  */

#ifndef TCBIN_POWER_MINOR
#define TCBIN_POWER_MINOR	0
#endif
#undef TCBIN_POWER_MINOR	/* {PS} define this if specific Minor is required  */

#ifndef TCBIN_POWER_NR_DEVS
#define TCBIN_POWER_NR_DEVS 	1
#endif

#define DRIVER_VERSION			"1.00"

#define TCBIN_POWER_PLATFORM_NAME	"tcbin_power"
#define TCBIN_POWER_PROC_DIR		"tcbin_power"
#define TCBIN_POWER_DEV_CLASS_NAME	"power"
#define TCBIN_POWER_DEV_NAME		"tcbin_power"

struct tcbin_power_dev_info
{
	int major;
	int minor;
	unsigned char braille_dot_strength;
	struct cdev cdev;
	struct semaphore sem;
	struct class *dev_class;
	struct platform_device *platform_dev;
	struct proc_dir_entry *proc_dir;
};

#if 0
/*
 * Split minors in two parts
 */
#define TYPE(minor)	(((minor) >> 4) & 0xf)	/* high nibble */
#define NUM(minor)	((minor) & 0xf)		/* low  nibble */


/* 
 * The different configurable parameters
 */
extern int brd_major;     /* main.c */
extern int brd_nr_devs;

/*
 * Prototypes for shared functions
 */

ssize_t brd_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t brd_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
int     brd_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

const char *brd_auto_version(void);
#endif

#endif /* _TCBIN_POWER_H */
