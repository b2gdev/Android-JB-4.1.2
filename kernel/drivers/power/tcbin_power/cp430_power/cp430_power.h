#ifndef _CP430_POWER_H
#define _CP430_POWER_H


#ifndef CP430_POWER_MINOR
#define CP430_POWER_MINOR	0
#endif
#undef CP430_POWER_MINOR	/* {KW} define this if specific Minor is required  */

#ifndef CP430_POWER_NR_DEVS
#define CP430_POWER_NR_DEVS 	1
#endif

#define DRIVER_VERSION			"1.00"

#define CP430_POWER_PLATFORM_NAME	"tcbin_cp430_power"
#define CP430_POWER_PROC_DIR		"cp430_power"
#define CP430_POWER_DEV_NAME		"cp430_power"

/* Power status defines. These should match with the power status values used in MSP430 firmware source */
#define POWER_ACTIVE	0x01
#define POWER_MIDDLE	0x02
#define POWER_SUSPEND	0x03
#define POWER_OFF		0x04


#define CMD_POWER_GET_STATUS	0x01

#define CP430_PACKET_OVERHEAD	8

#define CP430_PWR_STAT_GPIO     11

//struct cp430_power_status 	/* {KW} power status */
//{
//	unsigned char power_status;			
//};

struct cp430_power_dev_info
{
	int major;
	int minor;
	struct semaphore sem;
	struct platform_device *platform_dev;
	struct proc_dir_entry *proc_dir;
	unsigned char power_status;	
};

/* ioclts here */


#endif /* _CP430_POWER_H */
