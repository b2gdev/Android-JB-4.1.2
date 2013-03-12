#ifndef _BATT_CHARGER_IOCTL_H
#define _BATT_CHARGER_IOCTL_H

#define BATT_CHARGER_IOC_MAGIC			0xBA


#define BATT_CHARGER_GET_DRIVER_VERSION		_IOC(_IOC_READ,  BATT_CHARGER_IOC_MAGIC, 0x01, 4)
#define BATT_CHARGER_GET_BATT_STATUS		_IOC(_IOC_READ,  BATT_CHARGER_IOC_MAGIC, 0x02, 4)
	
#define BATT_CHARGER_IOC_MAXNR			0x02


struct battery_status 	/* {PK} Used in BATT_CHARGER_GET_STATUS ioctl */
{
	unsigned char status_flags;	/* Staus flags			*/	
	unsigned char rsoc;		/* Relative State Of Charge 	*/
	unsigned short temperature;	/* Temperature in Kelvin	*/
	unsigned short voltage;		/* Voltage in milli-Volts	*/
};

#endif /* _BATT_CHARGER_IOCTL_H */