#ifndef _METEC_FLAT20_H
#define _METEC_FLAT20_H

#ifndef METEC_FLAT20_MAJOR
#define METEC_FLAT20_MAJOR	240
#endif
#undef METEC_FLAT20_MAJOR 	/* {PK} define this if specific Major is required  */

#ifndef METEC_FLAT20_MINOR
#define METEC_FLAT20_MINOR	0
#endif
#undef METEC_FLAT20_MINOR	/* {PK} define this if specific Minor is required  */

#ifndef METEC_FLAT20_NR_DEVS
#define METEC_FLAT20_NR_DEVS 	1
#endif

#define DRIVER_VERSION			"1.00"

#define METEC_FLAT20_PLATFORM_NAME	"tcbin_braille_metec_flat20"
#define METEC_FLAT20_PROC_DIR		"braille_display"
#define METEC_FLAT20_DEV_CLASS_NAME	"accessibility"
#define METEC_FLAT20_DEV_NAME		"braille0"

struct metec_flat20_dev_info
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

#define CMD_DISPLAY_ON_OFF		0x01
#define CMD_DISPLAY_WRITE		0x02

#define METEC_FLAT20_PACKET_OVERHEAD	4
#define CP430_PACKET_OVERHEAD		8

#define METEC_FLAT20_PACKET_UOUT_INDEX	18

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

#endif /* _METEC_FLAT20_H */
