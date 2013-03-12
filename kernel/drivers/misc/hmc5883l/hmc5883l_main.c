
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	
#include <linux/fs.h>		
#include <linux/errno.h>	
#include <linux/types.h>	
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>
/* {SW} BEGIN: header files needed to control power regulator */
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
/* {SW} END: */

#include <linux/i2c/hmc5883l.h>
#include <linux/i2c/hmc5883l_ioctl.h>
#include "debug.h"
#include <linux/timer.h>
#include <linux/input-polldev.h>  // {RD}

struct hmc5883l_dev_info {
	int major;
	int minor;
	unsigned int measurement_wait_delay;
	unsigned char calibration_enable;
	struct cdev cdev;
	struct semaphore sem;
	struct class *dev_class;
	struct platform_device *platform_dev;
	struct i2c_client *i2c_client;
	struct proc_dir_entry *proc_dir;
	struct hmc5883l_configs sensor_configs;
	struct calibration_info calib_info;
	struct input_polled_dev *input_poll_dev;  // {RD} added polled input device
	atomic_t enabled; // {RD}
	int on_before_suspend; //{RD}
	struct mutex lock; // {RD}
	struct hmc5883l_platform_data *pdata; // {RD}
};

float sinarray[360] = 
{
	0.0000, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 0.1736, 0.1908,
	0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 0.3420, 0.3584, 0.3746, 0.3907,
	0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736,
	0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314,
	0.7431, 0.7547, 0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572,
	0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455,
	0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925,
	0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962,
	0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563,
	0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746,
	0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547,
	0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018,
	0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5000, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226,
	0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250,
	0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175,
	-0.0000, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908,
	-0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907,
	-0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5000, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736,
	-0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314,
	-0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572,
	-0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455,
	-0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925,
	-0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1.0000, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962,
	-0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563,
	-0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746,
	-0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547,
	-0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018,
	-0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5000, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226,
	-0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250,
	-0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175
};

int proc_enable = 1;

static struct hmc5883l_dev_info *hmc5883l_dev;

/* {SW} BEGIN:regulator pointer definition */
static struct regulator *hmc5883l_reg;
/* {SW} END: */ 

static int
hmc5883l_i2c_read(struct register_info *reg_info)
{
	int ret = 0;
	struct i2c_client *client = hmc5883l_dev->i2c_client;
	struct i2c_msg msg;
	
	if (client == NULL) {
		return -EIO;
	}
	
	memset(&msg, 0, sizeof(msg));
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 1;
	msg.buf = &reg_info->address;
	
	ret = i2c_transfer(client->adapter, &msg, 1);
	if (1 == ret) { /* {PK}: i2c transfer success */
		msg.flags = I2C_M_RD;
		msg.len = 1;
		msg.buf = &reg_info->value;
		
		ret = i2c_transfer(client->adapter, &msg, 1);
		if (1 == ret)  /* {PK}: i2c transfer success */
			ret = 0;
	}
	
	if (ret != 0)  
		PDEBUG("hmc5883l: i2c transfer(read) failed address: 0x%02X\r\n",reg_info->address) ;
	
	return ret;
}

static int
hmc5883l_i2c_write(struct register_info *reg_info)
{
	int ret = 0;
	struct i2c_client *client = hmc5883l_dev->i2c_client;
	struct i2c_msg msg;
	unsigned char buf[2];

	if (client == NULL) {
		return -EIO;
	}
		
	memset(&msg, 0, sizeof(msg));
	buf[0] = reg_info->address;
	buf[1] = reg_info->value;
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = buf;
	
	ret = i2c_transfer(client->adapter, &msg, 1);
	if (1 == ret) 
		ret = 0; /* {PK}: i2c transfer success */
	else
		PDEBUG("hmc5883l: i2c transfer(write) failed address: 0x%02X value: 0x%02X\r\n"
				,reg_info->address, reg_info->value) ;
		
	return ret;
}

static int
hmc5883l_read_register(struct register_info *reg_info)
{
	int ret = 0;
	
	switch(reg_info->address) {
		case REG_CONFIG_A:
		case REG_CONFIG_B:
		case REG_MODE:
		case REG_X_MSB:
		case REG_X_LSB:
		case REG_Z_MSB:
		case REG_Z_LSB:
		case REG_Y_MSB:
		case REG_Y_LSB:
		case REG_STATUS:
		case REG_IDENT_A:
		case REG_IDENT_B:
		case REG_IDENT_C:
			ret = hmc5883l_i2c_read(reg_info);
			break;
		default:
			ret = -EINVAL;
			break;
	}
	
	return ret;
}

static int
hmc5883l_write_register(struct register_info *reg_info)
{
	int ret = 0;
	
	switch(reg_info->address) {
		case REG_CONFIG_A:
			reg_info->value &= 0x7F;
			break;
		case REG_CONFIG_B:
			reg_info->value &= 0xE0;
			break;
		case REG_MODE:
			reg_info->value &= 0x03;
			break;
		default:
			ret = -EINVAL;
			break;
	}
	
	if (0 == ret)
		ret = hmc5883l_i2c_write(reg_info);
	
	return ret;
}

static int
hmc5883l_send_device_configs(struct hmc5883l_dev_info *dev)
{
	int ret = 0;
	struct register_info reg_info;
	
	memset(&reg_info, 0, sizeof(reg_info));
	
	/* {PK} REG_CONFIG_A */
	reg_info.address = REG_CONFIG_A;
	reg_info.value  = (dev->sensor_configs.measurement_mode & 0x03) << 0;
	reg_info.value |= (dev->sensor_configs.data_rate & 0x07) << 2;
	reg_info.value |= (dev->sensor_configs.sample_average & 0x03) << 5;
	ret = hmc5883l_write_register(&reg_info);
	
	
	/* {PK} REG_CONFIG_B */
	if (0 == ret) {
		reg_info.address = REG_CONFIG_B;
		reg_info.value  = 0;
		reg_info.value  = (dev->sensor_configs.field_strength & 0x07) << 5;
		ret = hmc5883l_write_register(&reg_info);
	}
	
	/* {PK} REG_MODE */
	if (0 == ret) {
		reg_info.address = REG_MODE;
		reg_info.value  = 0;
		reg_info.value  = dev->sensor_configs.operating_mode & 0x03;
		ret = hmc5883l_write_register(&reg_info);
	}
	
	return ret;

}

static int
hmc5883l_set_defaults(struct hmc5883l_dev_info *dev)
{
	int ret = 0;
	
	dev->sensor_configs.data_rate		= DATA_RATE_15_00_HZ;
	dev->sensor_configs.measurement_mode	= NORMAL_MEASUREMENT_MODE;
	dev->sensor_configs.sample_average	= _8_SAMPLE_PER_MEASUREMENT;
	dev->sensor_configs.field_strength	= FIELD_STRENGTH_1_30_Ga;
   /* {SW} BEGIN: modified to continuous operation */
	//dev->sensor_configs.operating_mode	= SINGLE_MEASUREMENT_MODE;
	dev->sensor_configs.operating_mode	= CONTINUOUS_MEASUREMENT_MODE;
   /* {SW} END: */
	dev->sensor_configs.flag = 0;
	
	dev->measurement_wait_delay = 67;
	dev->calibration_enable = CALIBRATION_DISABLE;
	
	memset(&dev->calib_info, 0, sizeof(dev->calib_info));
	
	ret = hmc5883l_send_device_configs(dev);
	
	return ret;
}

static int
hmc5883l_shutdown(void)
{
	int ret = 0;
	struct register_info reg_info;
	
	reg_info.address = REG_MODE;
	reg_info.value =  IDLE_MODE;
	ret = hmc5883l_write_register(&reg_info);
	
	return ret;
}

static int
hmc5883l_get_sensor_readings(struct hmc5883l_dev_info *dev, struct sensor_readings *readings)
{
	int ret = 0;
	int i = 0;
	struct register_info reg_info;
	unsigned char buf[6];	
	
	memset(buf, 0, sizeof(buf));
	for (i = 0x03 ; i <= 0x08 ;i++) 
	{
		reg_info.address = i;
		reg_info.value = 0;
		ret = hmc5883l_read_register(&reg_info);
		if(0 == ret)
			buf[i - 0x03] = reg_info.value;
		else
			break;
			
	}
	//mdelay(dev->measurement_wait_delay); {RD}
	
	if(0 == ret) /* {PK} all the values are successfully read */
	{   /*{MS} due to MSB is first byte, needs to change the endianess*/
		//memcpy(readings, buf, sizeof(*readings));
		// {RD} Add axis correction
		readings->x = (0xffff)&((buf[dev->pdata->axis_map_x*2]<<8)|(buf[(dev->pdata->axis_map_x*2)+1]));
		if(dev->pdata->negate_x)
			readings->x = (0xffff)&(~readings->x);
			
		readings->y = (0xffff)&((buf[dev->pdata->axis_map_y*2]<<8)|(buf[(dev->pdata->axis_map_y*2)+1]));
		if(dev->pdata->negate_y)
			readings->y = (0xffff)&(~readings->y);
			
		readings->z = (0xffff)&((buf[dev->pdata->axis_map_z*2]<<8)|(buf[(dev->pdata->axis_map_z*2)+1]));
		if(dev->pdata->negate_z)
			readings->z = (0xffff)&(~readings->z);
	}
	
	return ret;
}

static long 
hmc5883l_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int err = 0;
	struct hmc5883l_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	PDEBUG("hmc5883l: %s\n",__FUNCTION__);
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != HMC5883L_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > HMC5883L_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;
		
	switch(cmd) {
	case HMC5883L_GET_DRIVER_VERSION: {
		const char * version_string;
		
		version_string = DRIVER_VERSION;
		ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("hmc5883l: copy_to_user failed.\r\n");
	}
	break;
	case HMC5883L_SET_CONFIGS: {
		struct hmc5883l_configs *sensor_configs = &dev->sensor_configs;
		struct hmc5883l_configs user_configs;
		
		memset(&user_configs, 0, sizeof(user_configs));
		ret = copy_from_user(&user_configs, (void*)arg, sizeof(user_configs));
		if (ret) {
			PDEBUG("hmc5883l: copy_from_user failed.\r\n");
			ret = -EFAULT;
			break;
		}
		
		/* {PK} check 'data_rate' */
		if (user_configs.flag & DATA_RATE) {
			switch(user_configs.data_rate) {
				case DATA_RATE_0_75_HZ: dev->measurement_wait_delay = 1334; break;
				case DATA_RATE_1_50_HZ: dev->measurement_wait_delay = 667;  break;
				case DATA_RATE_3_00_HZ: dev->measurement_wait_delay = 334;  break;
				case DATA_RATE_7_50_HZ: dev->measurement_wait_delay = 134;  break;
				case DATA_RATE_15_00_HZ: dev->measurement_wait_delay = 67;  break;
				case DATA_RATE_30_00_HZ: dev->measurement_wait_delay = 34;  break;
				case DATA_RATE_75_00_HZ: dev->measurement_wait_delay = 14;  break;
				default: 
					ret = -EINVAL;
					break;
			}
			//{RD} Set data rate to the timer
			printk(KERN_INFO "%s: user_configs.data_rate:%d\r\n",__FUNCTION__,dev->measurement_wait_delay);
		}
		else {
			user_configs.data_rate = sensor_configs->data_rate;
		}
		
		/* {PK} check 'measurement_mode' */
		if (user_configs.flag & MEASUREMENT_MODE) {
			switch(user_configs.measurement_mode) {
				case NORMAL_MEASUREMENT_MODE:
				case POSITIVE_BIAS_MEASUREMENT_MODE:
				case NEGATIVE_BIAS_MEASUREMENT_MODE:
					break;
				default:
					ret = -EINVAL;
					break;
			}
		}
		else {
			user_configs.measurement_mode = sensor_configs->measurement_mode;
		}
		
		/* {PK} check 'sample_average' */
		if (user_configs.flag & SAMPLE_AVERAGE) {
			switch(user_configs.sample_average) {
				case _1_SAMPLE_PER_MEASUREMENT:
				case _2_SAMPLE_PER_MEASUREMENT:
				case _4_SAMPLE_PER_MEASUREMENT:
				case _8_SAMPLE_PER_MEASUREMENT:
					break;
				default:
					ret = -EINVAL;
					break;
			}
		}
		else {
			user_configs.sample_average = sensor_configs->sample_average;
		}
		
		/* {PK} check 'field_strength' */
		if (user_configs.flag & FIELD_STRENGTH) {
			switch(user_configs.field_strength) {
				case FIELD_STRENGTH_0_88_Ga:
				case FIELD_STRENGTH_1_30_Ga:
				case FIELD_STRENGTH_1_90_Ga:
				case FIELD_STRENGTH_2_50_Ga:
				case FIELD_STRENGTH_4_00_Ga:
				case FIELD_STRENGTH_4_70_Ga:
				case FIELD_STRENGTH_5_60_Ga:
				case FIELD_STRENGTH_8_10_Ga:
					break;
				default:
					ret = -EINVAL;
					break;
			}
		}
		else {
			user_configs.field_strength = sensor_configs->field_strength;
		}
		
		/* {PK} check 'operating_mode' */
		if (user_configs.flag & OPERATING_MODE) {
			switch(user_configs.operating_mode) {
				case CONTINUOUS_MEASUREMENT_MODE:
				case SINGLE_MEASUREMENT_MODE:
				case IDLE_MODE:
					break;
				default:
					ret = -EINVAL;
					break;
			}
		}
		else {
			user_configs.operating_mode = sensor_configs->operating_mode;
		}
		
		if(ret) /* {PK} input parameters are invalid */
			break;
		
		memcpy(sensor_configs, &user_configs, sizeof(*sensor_configs));
		
		ret = hmc5883l_send_device_configs(dev);
		if(ret < 0)
			PDEBUG("hmc5883l: hmc5883l_send_device_configs failed.\r\n");
	}	
	break;
	case HMC5883L_GET_CONFIGS:
		ret = copy_to_user((void *)arg, (const void *)&dev->sensor_configs, sizeof(dev->sensor_configs)) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("hmc5883l: copy_to_user failed.\r\n");
	break;
	case HMC5883L_GET_SENSOR_READINGS: {
		struct sensor_readings readings;
		
		ret = hmc5883l_get_sensor_readings(dev, &readings);
		if (0 == ret) {
			if (CALIBRATION_ENABLE == dev->calibration_enable) {
				struct calibration_info *calib_info;
				
				calib_info = &dev->calib_info;
				readings.x = (readings.x * calib_info->x_gain) +calib_info->x_offset;
				readings.y = (readings.y * calib_info->y_gain) +calib_info->y_offset;
				
			}
			
			ret = copy_to_user((void *)arg, (const void *)&readings, sizeof(readings)) ? -EFAULT : 0;
			if(ret)
				PDEBUG("hmc5883l: copy_to_user failed in HMC5883L_GET_SENSOR_READINGS.\r\n");
		}
		else {
			PDEBUG("hmc5883l: hmc5883l_get_sensor_readings failed.\r\n");
		}
	}
	break;
	case HMC5883L_GET_STATUS: {
		unsigned long status = 0;
		struct register_info reg_info;
		
		/* {PK} Read the status register */
		reg_info.address = REG_STATUS;
		reg_info.value = 0;
		ret = hmc5883l_read_register(&reg_info);
		if (0 == ret) {
			status = reg_info.value;
			ret = copy_to_user((void *)arg, (const void *)&status, sizeof(status)) ? -EFAULT : 0;
			if(ret)
				PDEBUG("hmc5883l: copy_to_user failed in HMC5883L_GET_STATUS.\r\n");
		}
		else {
			PDEBUG("hmc5883l: hmc5883l_read_register failed.\r\n");
		}
	}
	break;
	case HMC5883L_TEST_I2C_COMMUNICATION: {
		unsigned int success_count = 0;
		struct register_info reg_info;
		
		/* {PK} Read identification registers */
		reg_info.address = REG_IDENT_A;
		reg_info.value = 0;
		ret = hmc5883l_read_register(&reg_info);
		if (0 == ret) {
			if (0x48 == reg_info.value)
				success_count++;
			else
				PDEBUG("hmc5883l: REG_IDENT_A value mismatch failed.\r\n");
		}
		else {
			PDEBUG("hmc5883l: hmc5883l_read_register failed.\r\n");
		}
		
		reg_info.address = REG_IDENT_B;
		reg_info.value = 0;
		ret = hmc5883l_read_register(&reg_info);
		if (0 == ret) {
			if (0x34 == reg_info.value)
				success_count++;
			else
				PDEBUG("hmc5883l: REG_IDENT_B value mismatch failed.\r\n");
		}
		else {
			PDEBUG("hmc5883l: hmc5883l_read_register failed.\r\n");
		}
		
		reg_info.address = REG_IDENT_C;
		reg_info.value = 0;
		ret = hmc5883l_read_register(&reg_info);
		if (0 == ret) {
			if (0x33 == reg_info.value)
				success_count++;
			else
				PDEBUG("hmc5883l: REG_IDENT_C value mismatch failed.\r\n");
		}
		else {
			PDEBUG("hmc5883l: hmc5883l_read_register failed.\r\n");
		}
		
		if(success_count != 3)
			ret = -EIO;
	}
	break;
	case HMC5883L_SHUTDOWN:
		ret = hmc5883l_shutdown();
	break;
	case HMC5883L_SET_CALIBRATION_VALUES:
		ret = copy_from_user((void *)&dev->calib_info, (void*)arg, sizeof(dev->calib_info)) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("hmc5883l: copy_from_user failed.\r\n");
	break;
	case HMC5883L_GET_CALIBRATION_VALUES:
		ret = copy_to_user((void *)arg, (const void *)&dev->calib_info, sizeof(dev->calib_info)) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("hmc5883l: copy_to_user failed.\r\n");
	break;
	case HMC5883L_CONTROL_CALIBRATION:
		switch(arg) {
			case CALIBRATION_ENABLE:
				dev->calibration_enable = CALIBRATION_ENABLE;
				break;
			case CALIBRATION_DISABLE: 
				dev->calibration_enable = CALIBRATION_DISABLE;
				break;
			default:
				ret = -EINVAL;
				break;
		}
	break;
	case HMC5883L_READ_REGISTER: {
		struct register_info reg_info;
		
		memset(&reg_info, 0, sizeof(reg_info));
		
		if (copy_from_user(&reg_info, (void __user *)arg, sizeof(reg_info))) {
			ret = -EFAULT;
			break;
		}
		
		ret = hmc5883l_read_register(&reg_info);
		if (ret)
			break;
		
		if (copy_to_user((void __user *)arg, &reg_info, sizeof(reg_info)))
			ret = -EFAULT;
	}
	break;
	case HMC5883L_WRITE_REGISTER: {
		struct register_info reg_info;
		
		memset(&reg_info, 0, sizeof(reg_info));
		
		if (copy_from_user(&reg_info, (void __user *)arg, sizeof(reg_info))) {
			ret = -EFAULT;
			break;
		}
		
		ret = hmc5883l_write_register(&reg_info);
	}
	break;
	case HMC5883L_SET_TO_DEFAULTS:
		ret = hmc5883l_set_defaults(dev);
	break;
	default:
		ret = -ENOTTY;
	break;
    }
    
    up(&dev->sem);
    
    return ret;
}

/* {SW} BEGIN: functions needed to control power regulator */
static int hmc5883l_reg_get (void)
{
   hmc5883l_reg = regulator_get (NULL, "vcc_vaux4_2v5");
	
   if (IS_ERR(hmc5883l_reg))
   {
      printk (KERN_ERR "%s: get vcc_vaux4_2v5 regulator failed\n", __FUNCTION__);
      return PTR_ERR(hmc5883l_reg);
   }
   
   return 0;
}

static void hmc5883l_reg_put (void)
{
   regulator_put (hmc5883l_reg);
}

static int hmc5883l_reg_power_on (void)
{
   int ret = 0;
   
   ret = regulator_enable (hmc5883l_reg);
   if (ret < 0)
   {
      printk (KERN_ERR "%s: vcc_vaux4_2v5 regulator_enable failed\n", __FUNCTION__);
      return ret;
   }
   /* {SW} IMPORTANT: delay added in order to give enough time to the regulator + compass chip to be stabalize
    * after turning on the regulator before any i2c communication is performed, can be fine tuned as neccessary
    */
   mdelay(10);
   
   return ret;
}

static int hmc5883l_reg_power_off (void)
{
   int ret = 0;
   
   ret = regulator_disable (hmc5883l_reg);
   if (ret < 0)
   {
      printk (KERN_ERR "%s: vcc_vaux4_2v5 regulator_disable failed\n", __FUNCTION__);
   }
   
   return ret;
}
/* {SW} END: */

// {RD} input poll func
static void hmc5883l_input_poll_func(struct input_polled_dev *dev)
{
	int err;
	struct sensor_readings readings;
	struct input_dev *input = hmc5883l_dev->input_poll_dev->input;	
	
	mutex_lock(&hmc5883l_dev->lock);			
	
	if(atomic_read(&hmc5883l_dev->enabled)){	
		err = hmc5883l_get_sensor_readings(hmc5883l_dev, &readings);	
		
		if (err == 0) {
			if (CALIBRATION_ENABLE == hmc5883l_dev->calibration_enable) {
				struct calibration_info *calib_info;			
				calib_info = &hmc5883l_dev->calib_info;
				readings.x = (readings.x * calib_info->x_gain) +calib_info->x_offset;
				readings.y = (readings.y * calib_info->y_gain) +calib_info->y_offset;			
			}
			input_report_abs(input, ABS_X, readings.x);
			input_report_abs(input, ABS_Y, readings.y);
			input_report_abs(input, ABS_Z, readings.z);
			input_sync(input);	
		}
		else
			printk(KERN_ERR "%s: hmc5883l_get_sensor_readings failed.\r\n",__FUNCTION__);						
	}
	
	mutex_unlock(&hmc5883l_dev->lock);
		
	return;
}

// {RD} add enable and poll rate sys nodes
static ssize_t attr_get_polling_rate(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	int val;
	mutex_lock(&hmc5883l_dev->lock);
	val = hmc5883l_dev->measurement_wait_delay;
	mutex_unlock(&hmc5883l_dev->lock);
	return sprintf(buf, "%d\n", val);
}

static ssize_t attr_set_polling_rate(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	unsigned long interval_d;
	int ret=0;

	if (strict_strtoul(buf, 10, &interval_d))
		return -EINVAL;
	if (!interval_d)
		return -EINVAL;
	
	mutex_lock(&hmc5883l_dev->lock);
		switch(interval_d) {
			case DATA_RATE_0_75_HZ: hmc5883l_dev->measurement_wait_delay = 1334; break;
			case DATA_RATE_1_50_HZ: hmc5883l_dev->measurement_wait_delay = 667;  break;
			case DATA_RATE_3_00_HZ: hmc5883l_dev->measurement_wait_delay = 334;  break;
			case DATA_RATE_7_50_HZ: hmc5883l_dev->measurement_wait_delay = 134;  break;
			case DATA_RATE_15_00_HZ: hmc5883l_dev->measurement_wait_delay = 67;  break;
			case DATA_RATE_30_00_HZ: hmc5883l_dev->measurement_wait_delay = 34;  break;
			case DATA_RATE_75_00_HZ: hmc5883l_dev->measurement_wait_delay = 14;  break;
			default: 
				ret = -EINVAL;
				break;
		}
		if(ret){
			mutex_unlock(&hmc5883l_dev->lock);		
			printk(KERN_ERR "%s: compass error setting user_configs.data_rate:%d\r\n",__FUNCTION__,(int)interval_d);
			return -EINVAL;
		}	
	
	printk(KERN_INFO "%s: compass user_configs.data_rate:%d\r\n",__FUNCTION__,hmc5883l_dev->measurement_wait_delay);					
	mutex_unlock(&hmc5883l_dev->lock);
	
	return size;
}

static ssize_t attr_get_enable(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	int val;
	mutex_lock(&hmc5883l_dev->lock);
	val = atomic_read(&hmc5883l_dev->enabled);
	mutex_unlock(&hmc5883l_dev->lock);
	return sprintf(buf, "%d\n", val);
}

static ssize_t attr_set_enable(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t size)
{
	unsigned long val;
	int ret;

	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;

mutex_lock(&hmc5883l_dev->lock);

	if (val){
		if (!atomic_cmpxchg(&hmc5883l_dev->enabled, 0, 1)) {
			printk(KERN_INFO "Compass: set enable\n");
			ret = hmc5883l_reg_power_on();
		   if (ret < 0)
		   {
			  atomic_set(&hmc5883l_dev->enabled, 0); 
			  goto err0;
		   }
	   
		   ret = hmc5883l_send_device_configs(hmc5883l_dev);
		   if (ret < 0)
		   {
			   atomic_set(&hmc5883l_dev->enabled, 0);
			  goto err0;
		   }	   			    
      }
	}else{
		if (atomic_cmpxchg(&hmc5883l_dev->enabled, 1, 0))
		{
			printk(KERN_INFO "Compass: set disable\n");
							
			hmc5883l_shutdown();
		   /* {SW} BEGIN: power off the regulator */
		   hmc5883l_reg_power_off();
		   /* {SW} END: */
		}
	}
mutex_unlock(&hmc5883l_dev->lock);
return size;

err0:	
   printk(KERN_ERR "%s: failed to enable hmc5883l\n", __FUNCTION__);
   mutex_unlock(&hmc5883l_dev->lock);
   return ret;
	
}

static int 
hmc5883l_platform_probe(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
	
   /* {SW} BEGIN: driver probe will enable the power regulator at the beginning */
   ret = hmc5883l_reg_get();
   if (ret < 0)
   {
      goto err0;
   }
   
   ret = hmc5883l_reg_power_on();
   if (ret < 0)
   {
      goto err1;
   }
   
   mutex_init(&hmc5883l_dev->lock);  
	  
   return 0;
   
err1:
   hmc5883l_reg_put();
err0:
   printk(KERN_ERR "%s: failed to probe hmc5883l\n", __FUNCTION__);
   return ret;
   /* {SW} END: */
}

static int 
hmc5883l_platform_remove(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
   
   /* {SW} BEGIN: driver release will power off the regulator */
   hmc5883l_reg_power_off();
   hmc5883l_reg_put();
   /* {SW} END: */
	
	atomic_set(&hmc5883l_dev->enabled, 0);
	
	return ret;
}

static int 
hmc5883l_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);				
	
	hmc5883l_dev->on_before_suspend = atomic_read(&hmc5883l_dev->enabled);
	
	if(hmc5883l_dev->on_before_suspend){	
		
		hmc5883l_shutdown();
	   /* {SW} BEGIN: power off the regulator */
	   hmc5883l_reg_power_off();
	   /* {SW} END: */
		
		atomic_set(&hmc5883l_dev->enabled, 0);	
				
		// enable/disable polled input?
	}
	return ret;
}

static int 
hmc5883l_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);		
	
	if(hmc5883l_dev->on_before_suspend){
		
		/* {SW} BEGIN: turn on the regulator and set the device settings back */
	   ret = hmc5883l_reg_power_on();
	   if (ret < 0)
	   {
		  goto err0;
	   }
			 
	   ret = hmc5883l_send_device_configs(hmc5883l_dev);
	   if (ret < 0)
	   {
		  goto err0;
	   }
	   
	   atomic_set(&hmc5883l_dev->enabled, 1);
	   
	   // enable/disable polled input?
	}
	return 0;
   
err0:
   printk(KERN_ERR "%s: failed to resume hmc5883l\n", __FUNCTION__);
   return ret;
   /* {SW} END: */
}

static void 
hmc5883l_platform_shutdown(struct platform_device *device)
{
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
}

static struct platform_driver hmc5883l_platform_driver = {
	.probe	  = hmc5883l_platform_probe,
	.remove   = hmc5883l_platform_remove,
	.suspend  = hmc5883l_platform_suspend,
	.resume	  = hmc5883l_platform_resume,
	.shutdown = hmc5883l_platform_shutdown,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= HMC5883L_PLATFORM_NAME,
	},
};

static struct device_attribute attributes[] = {
	__ATTR(pollrate_ms, 0666, attr_get_polling_rate, attr_set_polling_rate),
	__ATTR(enable, 0666, attr_get_enable, attr_set_enable),
};

static int create_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		if (device_create_file(dev, attributes + i))
			goto error;
	return 0;

error:
	for ( ; i >= 0; i--)
		device_remove_file(dev, attributes + i);
	dev_err(dev, "%s:Unable to create interface\n", __func__);
	return -1;
}

static int remove_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		device_remove_file(dev, attributes + i);
	return 0;
}

int hmc5883l_input_open(struct input_dev *input)
{
	return 0; 
}

void hmc5883l_input_close(struct input_dev *dev)
{
	return ; 
}

static void hmc5883l_input_cleanup(struct hmc5883l_dev_info *acc)
{
	input_unregister_polled_device(acc->input_poll_dev);
	input_free_polled_device(acc->input_poll_dev);
}

static int hmc5883l_input_init(struct hmc5883l_dev_info *acc)
{
	int err;
	struct input_dev *input;

	acc->input_poll_dev = input_allocate_polled_device();
	if (!acc->input_poll_dev) {
		err = -ENOMEM;
		printk(KERN_ERR "%s: input device allocate failed!\n", __FUNCTION__);
		goto err0;
	}

	/* set input-polldev handlers */
	acc->input_poll_dev->private = acc;
	acc->input_poll_dev->poll = hmc5883l_input_poll_func;
	acc->input_poll_dev->poll_interval = acc->measurement_wait_delay;

	input = acc->input_poll_dev->input;

	input->open = hmc5883l_input_open;
	input->close = hmc5883l_input_close;
	input->name = HMC5883L_I2C_NAME;
	input->id.bustype = BUS_I2C;
	input->dev.parent = &acc->i2c_client->dev;

	input_set_drvdata(acc->input_poll_dev->input, acc);

	set_bit(EV_ABS, input->evbit);

	input_set_abs_params(input, ABS_X, -8000, 8000, 0, 0);
	input_set_abs_params(input, ABS_Y, -8000, 8000, 0, 0);
	input_set_abs_params(input, ABS_Z, -8000, 8000, 0, 0);

	input->name = "compass";

	err = input_register_polled_device(acc->input_poll_dev);
	if (err) {
		printk(KERN_ERR "%s: unable to register input device %s\n",__FUNCTION__,
			acc->input_poll_dev->input->name);
		goto err1;
	}

	return 0;

err1:
	input_free_polled_device(acc->input_poll_dev);
err0:
	return err;
}

static int
hmc5883l_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = -ENODEV;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
	
	if (HMC5883L_I2C_ADDRESS == client->addr) /* {PK}: Make sure we probe the correct device  */
	{
		hmc5883l_dev->i2c_client = client;
		printk (KERN_INFO "hmc5883l: hmc5883l i2c probed\n");
		
      /* {SW} BEGIN: */
		//hmc5883l_shutdown();
      /* {SW} END: */
		
		ret = 0; /* {PK}: Probing success  */
      
    // {RD} get platform data
   	hmc5883l_dev->pdata = kmalloc(sizeof(*hmc5883l_dev->pdata), GFP_KERNEL);
	if (hmc5883l_dev->pdata == NULL){
		printk(KERN_ERR "%s: failed to kmalloc pdata\n", __FUNCTION__);
         return ret;
	}
	memcpy(hmc5883l_dev->pdata, client->dev.platform_data, sizeof(*hmc5883l_dev->pdata));
	
      /* {SW} BEGIN: set the default settings*/
      ret = hmc5883l_set_defaults(hmc5883l_dev);
      if (ret < 0)
      {
         printk(KERN_ERR "%s: failed to set defaults\n", __FUNCTION__);
         return ret;
      }
      /* {SW} END: */     
      
      // {RD}
      atomic_set(&hmc5883l_dev->enabled, 1); 
         
      /* {RD} input polled device*/
		ret = hmc5883l_input_init(hmc5883l_dev);
		if (ret < 0){
			printk(KERN_ERR "%s: input init failed!\n", __FUNCTION__);
			return ret;
		}
		
      // {RD} sysfs
      ret = create_sysfs_interfaces(&client->dev);
	  if (ret < 0) {
		printk(KERN_ERR "%s: sysfs register failed\n", __FUNCTION__);
		hmc5883l_input_cleanup(hmc5883l_dev);
		return ret;
	  }	  	   	
	}
	return ret;
}

static int
hmc5883l_i2c_remove(struct i2c_client *client)
{
	int ret = 0;
	
	printk(KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
		
   // {RD} clean up of input driver
   hmc5883l_input_cleanup(hmc5883l_dev);
   
   	// {RD}
	remove_sysfs_interfaces(&client->dev);
	
   /* {SW} BEGIN: shutdown the device */
   hmc5883l_shutdown();
   /* {SW} END: */	
		
	mutex_destroy(&hmc5883l_dev->lock);
	
	return ret;
}

static const struct i2c_device_id hmc5883l_id[] = {
	{ HMC5883L_I2C_NAME, 0 },
	{ }
};

static struct i2c_driver hmc5883l_i2c_driver = {
	.driver = {
		.name  = HMC5883L_I2C_NAME,
		.owner = THIS_MODULE,
	},
	.probe = hmc5883l_i2c_probe,
	.remove = hmc5883l_i2c_remove,
	.id_table = hmc5883l_id,
};

static int 
hmc5883l_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(HMC5883L_DEV_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	hmc5883l_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
hmc5883l_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(HMC5883L_DEV_PROC_DIR, NULL);
	
	return ret;
}

static void
hmc5883l_init_variables(struct hmc5883l_dev_info *dev)
{	/* {PK} Do any future initializations here */

}

static int __init
hmc5883l_init(void)
{
	int ret = 0;
	
	printk (KERN_INFO "hmc5883l: %s\n",__FUNCTION__);
	
	hmc5883l_dev = (struct hmc5883l_dev_info *)kmalloc(sizeof(*hmc5883l_dev), GFP_KERNEL);
	if (NULL == hmc5883l_dev) {
		ret = -ENOMEM;
		goto err_fail;
	}
	memset (hmc5883l_dev, 0, sizeof(*hmc5883l_dev));
	
	hmc5883l_init_variables(hmc5883l_dev);
	
	/* {PK} Register platform driver */
	ret = platform_driver_register(&hmc5883l_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "hmc5883l: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}

	
	/* {PK} Register platform device */
	hmc5883l_dev->platform_dev = platform_device_register_simple(HMC5883L_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(hmc5883l_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "hmc5883l: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}	

	sema_init(&hmc5883l_dev->sem, 1);

	/* {PK} Create the proc file */
	if (hmc5883l_create_proc() < 0) {
		printk(KERN_INFO "hmc5883l: hmc5883lcreate_proc failed\r\n");
	}

	/* {PK} add i2c driver */
	ret = i2c_add_driver(&hmc5883l_i2c_driver);
	if (ret) {
		printk (KERN_NOTICE "hmc5883l: driver registration failed\n");
		goto err_i2c_driver;
	}

	printk("hmc5883l: Driver Version: %s\n", DRIVER_VERSION);

	return 0; /* {PK} success */

err_i2c_driver:
	hmc5883l_remove_proc();
	platform_device_unregister(hmc5883l_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&hmc5883l_platform_driver);
err_platform_driver:
	kfree(hmc5883l_dev);
err_fail:
	
	return ret;
}


static void __exit 
hmc5883l_exit(void)
{
	printk (KERN_INFO "hmc5883l: %s\n",__FUNCTION__);	
	
	/* {PK} Remove i2c driver */
	i2c_del_driver(&hmc5883l_i2c_driver);
	
	
	/* {PK} Remove procfs entry */
	if (hmc5883l_remove_proc() < 0) {
		printk(KERN_ERR "hmc5883l: lm3553_remove_proc failed\r\n");
	}
	
	/* {PK} Remove platform device */
	platform_device_unregister(hmc5883l_dev->platform_dev);
	platform_driver_unregister(&hmc5883l_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(hmc5883l_dev);
}

module_init(hmc5883l_init);
module_exit(hmc5883l_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Driver for TCBIN hmc5883l compass driver");
MODULE_AUTHOR("Thilina Bandara <thilinab@zone24x7.com> and Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
