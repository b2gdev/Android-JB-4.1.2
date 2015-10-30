#ifndef _LM3553_H
#define _LM3553_H

#ifndef LM3553_MINOR
#define LM3553_MINOR				100
#endif
#undef LM3553_MINOR

#if 0
#ifndef LM3553_NR_DEVS
#define LM3553_NR_DEVS 				1
#endif
#endif

/*
* Version Information
*/
#define DRIVER_VERSION				"1.00"

#define LM3553_PLATFORM_NAME			"tcbin_cam_lamp_lm3553"
#define LM3553_DEV_PROC_DIR			"cam_lamp"
#define LM3553_DEV_NAME				"cam_lamp"

#define LM3553_I2C_ADDRESS			0x53
#define LM3553_I2C_NAME				"lm3553"

#define REG_ADDRESS_GP				0x10
#define REG_ADDRESS_MF_PIN_CTRL			0x20
#define REG_ADDRESS_CURRENT_STEP_TIME		0x50
#define REG_ADDRESS_TORCH_CURRENT_CTRL		0xA0
#define REG_ADDRESS_FLASH_CURRENT_CTRL		0xB0
#define REG_ADDRESS_FLASH_DURATION_CTRL		0xC0

#define REG_GP_FIXED_BITS			0x18   
#define REG_MF_PIN_CTRL_FIXED_BITS		0xE0
#define REG_CURRENT_STEP_TIME_FIXED_BITS	0xFC
#define REG_TORCH_CURRENT_CTRL_FIXED_BITS	0x80
#define REG_FLASH_CURRENT_CTRL_FIXED_BITS	0x80
#define REG_FLASH_DURATION_CTRL_FIXED_BITS	0xF0

#define CURRENT_STEP_TIME_25US			0       
#define CURRENT_STEP_TIME_50US			1
#define CURRENT_STEP_TIME_100US			2
#define CURRENT_STEP_TIME_200US			3

#define DEFAULT_CURRENT_STEP_TIME		0x00   
#define DEFAULT_TORCH_CURRENT			0x18    
#define DEFAULT_FLASH_CURRENT			0x5C    
#define DEFAULT_FLASH_SAFETY_DURATION		0x05
#define MAX_FLASH_SAFETY_DURATION		0x0F

#define VFB_BIT_BITMASK				(1 << 5)
#define FLASH_MODE_BITMASK			(3 << 0)
#define TORCH_MODE_BITMASK			(1 << 1)
#define OVP_BITMASK				(1 << 3)

#define GP_REGISTER_INVALID_BIT_BITMASK		0xD8
#define MF_REGISTER_INVALID_BIT_BITMASK		0xE0
#define MAX_CURRENT_STEP_TIME			0x03    
#define MAX_TORCH_CURRENT_VALUE			0x1F    
#define MAX_FLASH_CURRENT_VALUE			0x7F    
#define MAX_FLASH_SAFETY_DURATION		0x0F    

struct lm3553_dev_info {
	int major;
	int minor;
	struct cdev cdev;
	struct semaphore sem;
	struct class *dev_class;
	struct platform_device *platform_dev;
	struct i2c_client *i2c_client;
	struct proc_dir_entry *proc_dir;
};


#endif /* _LM3553_H */
