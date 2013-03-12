#ifndef _HMC5883L_IOCTL_H
#define _HMC5883L_IOCTL_H

#define	HMC5883L_IOC_MAGIC		        0xE3

#define HMC5883L_GET_DRIVER_VERSION		_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x01, 4)
#define HMC5883L_SET_CONFIGS			_IOC(_IOC_WRITE, HMC5883L_IOC_MAGIC, 0x02, 4)
#define HMC5883L_GET_CONFIGS			_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x03, 4)
#define HMC5883L_GET_SENSOR_READINGS		_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x04, 4)
#define HMC5883L_GET_STATUS			_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x05, 4)
#define HMC5883L_TEST_I2C_COMMUNICATION		_IOC(_IOC_NONE,  HMC5883L_IOC_MAGIC, 0x06, 0)
#define HMC5883L_SHUTDOWN			_IOC(_IOC_NONE,  HMC5883L_IOC_MAGIC, 0x07, 0)
#define HMC5883L_SET_CALIBRATION_VALUES		_IOC(_IOC_WRITE, HMC5883L_IOC_MAGIC, 0x08, 4)
#define HMC5883L_GET_CALIBRATION_VALUES		_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x09, 4)
#define HMC5883L_CONTROL_CALIBRATION		_IOC(_IOC_WRITE, HMC5883L_IOC_MAGIC, 0x0A, 1)
	/* {PK} parameters for HMC5883L_CONTROL_CALIBRATION ioctl */
	#define CALIBRATION_ENABLE				1
	#define CALIBRATION_DISABLE				0
#define HMC5883L_READ_REGISTER			_IOC(_IOC_READ,  HMC5883L_IOC_MAGIC, 0x0B, 4)
#define HMC5883L_WRITE_REGISTER			_IOC(_IOC_WRITE, HMC5883L_IOC_MAGIC, 0x0C, 4)
#define HMC5883L_SET_TO_DEFAULTS		_IOC(_IOC_NONE,  HMC5883L_IOC_MAGIC, 0x0D, 0)

#define HMC5883L_IOC_MAXNR                        0x0D

/* {PK} Configuration parameter structure for HMC5883L_SET_CONFIGS/HMC5883L_GET_CONFIGS ioctl */
/* {PK} Passed as the pointer */
struct hmc5883l_configs {
	unsigned char data_rate;  	
	unsigned char measurement_mode;
	unsigned char sample_average;
	unsigned char field_strength;
	unsigned char operating_mode;
	unsigned char flag;
}__attribute__((__packed__));

/* {PK} Valid values for 'data_rate' */
#define DATA_RATE_0_75_HZ			0x00
#define DATA_RATE_1_50_HZ			0x01
#define DATA_RATE_3_00_HZ			0x02
#define DATA_RATE_7_50_HZ			0x03
#define DATA_RATE_15_00_HZ			0x04
#define DATA_RATE_30_00_HZ			0x05
#define DATA_RATE_75_00_HZ			0x06

/* {PK} Valid values for 'measurement_mode' */
#define NORMAL_MEASUREMENT_MODE			0x00
#define POSITIVE_BIAS_MEASUREMENT_MODE		0x01
#define NEGATIVE_BIAS_MEASUREMENT_MODE		0x02

/* {PK} Valid values for 'sample_average' */
#define _1_SAMPLE_PER_MEASUREMENT		0x00
#define _2_SAMPLE_PER_MEASUREMENT		0x01
#define _4_SAMPLE_PER_MEASUREMENT		0x02
#define _8_SAMPLE_PER_MEASUREMENT		0x03

/* {PK} Valid values for 'field_strength' */
#define FIELD_STRENGTH_0_88_Ga			0x00
#define FIELD_STRENGTH_1_30_Ga			0x01
#define FIELD_STRENGTH_1_90_Ga			0x02
#define FIELD_STRENGTH_2_50_Ga			0x03
#define FIELD_STRENGTH_4_00_Ga			0x04
#define FIELD_STRENGTH_4_70_Ga			0x05
#define FIELD_STRENGTH_5_60_Ga			0x06
#define FIELD_STRENGTH_8_10_Ga			0x07

/* {PK} Valid values for 'operating_mode' */
#define CONTINUOUS_MEASUREMENT_MODE		0x00
#define SINGLE_MEASUREMENT_MODE			0x01
#define IDLE_MODE				0x02

/* {PK} Valid values for 'flag' */
/* {PK} Set Relevant bit fields to select the element to set/get from the ioctl */
#define	DATA_RATE				0x01
#define MEASUREMENT_MODE			0x02
#define	SAMPLE_AVERAGE				0x04
#define	FIELD_STRENGTH				0x10
#define OPERATING_MODE				0x20

struct register_info
{
	unsigned char address;  /* {PK}: target register address */
	unsigned char value;
}__attribute__((__packed__));

/* {PK} Sensor Readings data structure for HMC5883L_GET_SENSOR_READINGS ioctl */
/* {PK} Passed as the pointer */
struct sensor_readings {
	unsigned short x;
	unsigned short z;
	unsigned short y;
}__attribute__((__packed__));

/* {PK} calibration data structure for HMC5883L_SET_CALIBRATION_VALUES/HMC5883L_GET_CALIBRATION_VALUES ioctl */
struct calibration_info {
	short x_offset;
	short y_offset;
	short z_offset;
	short x_gain;
	short y_gain;
	short z_gain;
}__attribute__((__packed__));

/* {PK}: HMC5883L register addresses */
#define REG_CONFIG_A				0x00
#define REG_CONFIG_B				0x01
#define REG_MODE				0x02
#define REG_X_MSB				0x03
#define REG_X_LSB				0x04
#define REG_Z_MSB				0x05
#define REG_Z_LSB				0x06
#define REG_Y_MSB				0x07
#define REG_Y_LSB				0x08
#define REG_STATUS				0x09
#define REG_IDENT_A				0x0A
#define REG_IDENT_B				0x0B
#define REG_IDENT_C				0x0C

#endif /* _HMC5883L_IOCTL_H */