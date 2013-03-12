#ifndef _HMC5883L_H
#define _HMC5883L_H

#include "hmc5883l_ioctl.h"

#ifndef HMC5883L_MINOR
#define HMC5883L_MINOR				101
#endif
#undef HMC5883L_MINOR


/*
* Version Information
*/
#define DRIVER_VERSION				"1.00"

#define HMC5883L_PLATFORM_NAME			"tcbin_compass_hmc5883l"
#define HMC5883L_DEV_PROC_DIR			"compass"
#define HMC5883L_DEV_NAME			"compass"

#define HMC5883L_I2C_ADDRESS			0x1E
#define HMC5883L_I2C_NAME			"hmc5883l"

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

struct hmc5883l_platform_data {

	u8 axis_map_x;
	u8 axis_map_y;
	u8 axis_map_z;

	u8 negate_x;
	u8 negate_y;
	u8 negate_z;
};

#endif /* _HMC5883L_H */
