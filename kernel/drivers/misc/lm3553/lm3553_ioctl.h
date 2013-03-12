#ifndef _LM3553_IOCTL_H
#define _LM3553_IOCTL_H

struct register_info
{
	unsigned char address;  /* {PK}: target register address */
	unsigned char value;
}__attribute__((__packed__));

#define	LM3553_IOC_MAGIC		        0xE1

#define LM3553_READ_REGISTER    		_IOC(_IOC_READ | _IOC_WRITE, LM3553_IOC_MAGIC, 0x01, 4)
#define LM3553_WRITE_REGISTER               	_IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x02, 4)
#define LM3553_SHUTDOWN                         _IOC(_IOC_NONE		   , LM3553_IOC_MAGIC, 0x03, 0)
#define LM3553_FLASH_CONTROL                    _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x04, 1)
	/* {PK} parameters for LM3553_FLASH_CONTROL ioctl */
	#define FLASH_ENABLE				1
	#define FLASH_DISABLE				0
#define LM3553_TORCH_CONTROL                    _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x05, 1)
	/* {PK} parameters for LM3553_TORCH_CONTROL ioctl */
	#define TORCH_ENABLE				1
	#define TORCH_DISABLE				0
#define LM3553_SET_FLASH_LEVEL                  _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x06, 1)
#define LM3553_GET_FLASH_LEVEL                  _IOC(_IOC_READ		   , LM3553_IOC_MAGIC, 0x07, 1)
	/* {PK}: value: 0 - 0x7F(=Fullscale) */
#define LM3553_SET_FLASH_SAFETY_DURATION        _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x08, 1)
#define LM3553_GET_FLASH_SAFETY_DURATION        _IOC(_IOC_READ		   , LM3553_IOC_MAGIC, 0x09, 1)
	/* parameters for LM3553_GET_FLASH_SAFETY_DURATION ioctl */
	/* {PK}: value  time (ms)
	*       0x00   50
	*       0x01   100
	*       0x02   200
	*       0x03   300
	*       0x04   400
	*       0x05   500
	*       0x06   600
	*       0x07   700
	*       0x08   800
	*       0x00   900
	*       0x0A   1000
	*       0x0B   1100
	*       0x0C   1200
	*       0x0D   1300
	*       0x0E   1400
	*       0x0F   3200
	*/
#define LM3553_SET_TORCH_LEVEL                  _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x0A, 1)
#define LM3553_GET_TORCH_LEVEL                  _IOC(_IOC_READ		   , LM3553_IOC_MAGIC, 0x0B, 1)
	/* {PK}: value: 0 - 0x1F(=Fullscale) */
#define LM3553_SET_CURRENT_STEP_TIME            _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x0C, 1)
#define LM3553_GET_CURRENT_STEP_TIME            _IOC(_IOC_READ		   , LM3553_IOC_MAGIC, 0x0D, 1)
	/* parameters for LM3553_GET_CURRENT_STEP_TIME ioctl */
	/* {PK}: value  time (us)
	*       0x00   25
	*       0x01   50
	*       0x02   100
	*       0x03   200
	*/
#define LM3553_SET_OVER_VOLTAGE_PROTECTION      _IOC(_IOC_WRITE		   , LM3553_IOC_MAGIC, 0x0E, 1)
	/* {PK} parameters for LM3553_SET_OVER_VOLTAGE_PROTECTION ioctl */
	#define HIGH_LEVEL_OVP				1
	#define LOW_LEVEL_OVP				0
#define LM3553_GET_DRIVER_VERSION		_IOC(_IOC_READ		   , LM3553_IOC_MAGIC, 0x0F, 4)

#define LM3553_IOC_MAXNR                        0x0F

#endif /* _LM3553_IOCTL_H */
