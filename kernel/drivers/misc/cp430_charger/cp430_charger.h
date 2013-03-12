#ifndef _BATT_CHARGER_H
#define _BATT_CHARGER_H

#include "cp430_charger_ioctl.h"	

#ifndef BATT_CHARGER_MINOR
#define BATT_CHARGER_MINOR	0
#endif
#undef BATT_CHARGER_MINOR	/* {PK} define this if specific Minor is required  */

#ifndef BATT_CHARGER_NR_DEVS
#define BATT_CHARGER_NR_DEVS 	1
#endif

#define DRIVER_VERSION			"1.00"

#define BATT_CHARGER_PLATFORM_NAME	"tcbin_cp430_charger"
#define BATT_CHARGER_PROC_DIR		"cp430_charger"
#define BATT_CHARGER_DEV_NAME		"cp430_charger"


struct batt_charger_dev_info
{
	int major;
	int minor;
	struct semaphore sem;
	struct platform_device *platform_dev;
	struct proc_dir_entry *proc_dir;
	struct battery_status batt_status;
};

#define CMD_CHARGER_GET_STATUS		0x01

#define CP430_PACKET_OVERHEAD		8


#endif /* _BATT_CHARGER_H */
