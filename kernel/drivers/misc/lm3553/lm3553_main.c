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

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>

/* {PK} Local includes */
#include "lm3553_ioctl.h"
#include "lm3553.h"
#include "debug.h"

int proc_enable = 1; /* {PK} Proc file. Enabled by default. Can be changed via the module param. used in dubug.h */

static struct lm3553_dev_info *lm3553_dev;

static int
lm3553_i2c_read(struct register_info *reg_info)
{
	int ret = 0;
	struct i2c_client *client = lm3553_dev->i2c_client;
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
		PDEBUG("lm3553: i2c transfer(read) failed address: 0x%02X\r\n",reg_info->address) ;
	
	return ret;
}

static int
lm3553_i2c_write(struct register_info *reg_info)
{
	int ret = 0;
	struct i2c_client *client = lm3553_dev->i2c_client;
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
		PDEBUG("lm3553: i2c transfer(write) failed address: 0x%02X value: 0x%02X\r\n"
				,reg_info->address, reg_info->value) ;
			
		
	return ret;
}

static int
lm3553_read_register(struct register_info *reg_info)
{
	int ret = 0;
	
	switch(reg_info->address) {
		case REG_ADDRESS_GP:
		case REG_ADDRESS_MF_PIN_CTRL:
		case REG_ADDRESS_CURRENT_STEP_TIME:
		case REG_ADDRESS_TORCH_CURRENT_CTRL:
		case REG_ADDRESS_FLASH_CURRENT_CTRL:
		case REG_ADDRESS_FLASH_DURATION_CTRL:
			ret = lm3553_i2c_read(reg_info);
			break;
		default:
			ret = -EINVAL;
			break;
	}
	
	return ret;
}

static int
lm3553_write_register(struct register_info *reg_info)
{
	int ret = 0;
	
	switch(reg_info->address) {
		case REG_ADDRESS_GP:
			reg_info->value |= REG_GP_FIXED_BITS;
			reg_info->value &= 0x3F;
			break;
		case REG_ADDRESS_MF_PIN_CTRL:
			reg_info->value |= REG_MF_PIN_CTRL_FIXED_BITS;
			break;
		case REG_ADDRESS_CURRENT_STEP_TIME:
			reg_info->value |= REG_CURRENT_STEP_TIME_FIXED_BITS;
			break;
		case REG_ADDRESS_TORCH_CURRENT_CTRL:
			reg_info->value |= REG_TORCH_CURRENT_CTRL_FIXED_BITS;
			reg_info->value &= 0x9F;
			break;
		case REG_ADDRESS_FLASH_CURRENT_CTRL:
			reg_info->value |= REG_FLASH_CURRENT_CTRL_FIXED_BITS;
			break;
		case REG_ADDRESS_FLASH_DURATION_CTRL:
			reg_info->value |= REG_FLASH_DURATION_CTRL_FIXED_BITS;
			break;
		default:
			ret = -EINVAL;
			break;
	}
	
	if (0 == ret)
		ret = lm3553_i2c_write(reg_info);
	
	return ret;
}

static int
lm3553_set_default_values(void)
{
	int ret = 0;
	struct register_info reg_info;
	
	memset(&reg_info, 0, sizeof(reg_info));
	
	reg_info.address = REG_ADDRESS_CURRENT_STEP_TIME;
	reg_info.value = DEFAULT_CURRENT_STEP_TIME;
	ret = lm3553_write_register(&reg_info);
	
	reg_info.address = REG_ADDRESS_TORCH_CURRENT_CTRL;
	reg_info.value = DEFAULT_TORCH_CURRENT;
	ret = lm3553_write_register(&reg_info);
	
	reg_info.address = REG_ADDRESS_FLASH_CURRENT_CTRL;
	reg_info.value = DEFAULT_FLASH_CURRENT;
	ret = lm3553_write_register(&reg_info);
	
	reg_info.address = REG_ADDRESS_FLASH_DURATION_CTRL;
	reg_info.value = DEFAULT_FLASH_SAFETY_DURATION;
	ret = lm3553_write_register(&reg_info);
	
	return ret;
}

static int
lm3553_shutdown(void)
{
	int ret = 0;
	struct register_info reg_info;
	
	memset(&reg_info, 0, sizeof(reg_info));
	
	reg_info.address = REG_ADDRESS_GP;
	reg_info.value =  0x00;
	ret = lm3553_write_register(&reg_info);
	
	return ret;
}

static ssize_t
lm3553_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	ssize_t ret = 0;
	
	PDEBUG("lm3553: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Reading is not permitted */
	
	return ret;
}

static ssize_t
lm3553_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	ssize_t ret = 0;
	
	PDEBUG("lm3553: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Writting is not permitted */
	
	return ret;
}

static long
lm3553_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int err = 0;
	struct register_info reg_info;
	struct lm3553_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	PDEBUG("lm3553: %s\n",__FUNCTION__);
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != LM3553_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > LM3553_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;
	
	switch (cmd) {
	case LM3553_READ_REGISTER:
		memset(&reg_info, 0, sizeof(reg_info));
		
		if (copy_from_user(&reg_info, (void __user *)arg, sizeof(reg_info))) {
			ret = -EFAULT;
			break;
		}
		
		ret = lm3553_read_register(&reg_info);
		if (ret)
			break;
		
		if (copy_to_user((void __user *)arg, &reg_info, sizeof(reg_info)))
			ret = -EFAULT;
	break;
	case LM3553_WRITE_REGISTER:
		memset(&reg_info, 0, sizeof(reg_info));
		
		if (copy_from_user(&reg_info, (void __user *)arg, sizeof(reg_info))) {
			ret = -EFAULT;
			break;
		}
		
		ret = lm3553_write_register(&reg_info);
	break;
	case LM3553_SHUTDOWN:
		ret = lm3553_shutdown();
	break;
	case LM3553_FLASH_CONTROL:
		switch(arg) {
		case FLASH_ENABLE:
			memset(&reg_info, 0, sizeof(reg_info));
			
			reg_info.address = REG_ADDRESS_GP;
			reg_info.value = (VFB_BIT_BITMASK| FLASH_MODE_BITMASK);
			ret = lm3553_write_register(&reg_info);
			break;
		case FLASH_DISABLE: 
			ret = lm3553_shutdown();
			break;
		default:
			ret = -EINVAL;
			break;
		}
	break;
	case LM3553_TORCH_CONTROL:
		switch(arg) {
		case TORCH_ENABLE:
			memset(&reg_info, 0, sizeof(reg_info));
			
			reg_info.address = REG_ADDRESS_MF_PIN_CTRL;
			ret = lm3553_read_register(&reg_info);
			if (ret != 0)
				break;
			
			reg_info.value &= (~OVP_BITMASK);
			ret = lm3553_write_register(&reg_info);
			if (ret != 0)
				break;
			
			reg_info.address = REG_ADDRESS_GP;
			reg_info.value = (VFB_BIT_BITMASK| TORCH_MODE_BITMASK);
			ret = lm3553_write_register(&reg_info);
			break;
		case TORCH_DISABLE: 
			ret = lm3553_shutdown();
			break;
		default:
			ret = -EINVAL;
			break;
		}
	break;
	case LM3553_SET_FLASH_LEVEL:
		memset(&reg_info, 0, sizeof(reg_info));
		ret = get_user(reg_info.value, (unsigned char *)arg);
		if (ret != 0)
			break;
		
		if (reg_info.value > MAX_FLASH_CURRENT_VALUE) {
			ret = -EINVAL;
			break;
		}
		
		reg_info.address = REG_ADDRESS_FLASH_CURRENT_CTRL;
		ret = lm3553_write_register(&reg_info);
	break;
	case LM3553_GET_FLASH_LEVEL:
		memset(&reg_info, 0, sizeof(reg_info));
		
		reg_info.address = REG_ADDRESS_FLASH_CURRENT_CTRL;
		ret = lm3553_read_register(&reg_info);
		if (ret != 0)
			break;
		
		reg_info.value &= MAX_FLASH_CURRENT_VALUE;
		ret = put_user(reg_info.value, (unsigned char *)arg);
	break;
	case LM3553_SET_FLASH_SAFETY_DURATION:
		memset(&reg_info, 0, sizeof(reg_info));
		
		ret = get_user(reg_info.value, (unsigned char *)arg);
		if (ret !=0)
			break;
		
		if (reg_info.value > MAX_FLASH_SAFETY_DURATION) {
			ret = -EINVAL;
			break;
		}
		
		reg_info.address = REG_ADDRESS_FLASH_DURATION_CTRL;
		ret = lm3553_write_register(&reg_info);
	break;
	case LM3553_GET_FLASH_SAFETY_DURATION:
		memset(&reg_info, 0, sizeof(reg_info));
		
		reg_info.address = REG_ADDRESS_FLASH_DURATION_CTRL;
		ret = lm3553_read_register(&reg_info);
		if (ret != 0)
			break;
		
		reg_info.value &= MAX_FLASH_SAFETY_DURATION;
		ret = put_user(reg_info.value, (unsigned char *)arg);
	break;
	case LM3553_SET_CURRENT_STEP_TIME:
		memset(&reg_info, 0, sizeof(reg_info));
		
		ret = get_user(reg_info.value, (unsigned char *)arg);
		if (ret !=0)
			break;
		
		if (reg_info.value > MAX_CURRENT_STEP_TIME) {
			ret = -EINVAL;
			break;
		}
		
		reg_info.address = REG_ADDRESS_CURRENT_STEP_TIME;
		ret = lm3553_write_register(&reg_info);
	break;
	case LM3553_GET_CURRENT_STEP_TIME:
		memset(&reg_info, 0, sizeof(reg_info));
		
		reg_info.address = REG_ADDRESS_CURRENT_STEP_TIME;
		ret = lm3553_read_register(&reg_info);
		if (ret != 0)
			break;
		
		reg_info.value &= MAX_CURRENT_STEP_TIME;
		ret = put_user(reg_info.value, (unsigned char *)arg);
	break;
	case LM3553_SET_TORCH_LEVEL:
		memset(&reg_info, 0, sizeof(reg_info));
		
		ret = get_user(reg_info.value, (unsigned char *)arg);
		if (ret !=0)
			break;
		
		if (reg_info.value > MAX_TORCH_CURRENT_VALUE) {
			ret = -EINVAL;
			break;
		}
		
		reg_info.address = REG_ADDRESS_TORCH_CURRENT_CTRL;
		ret = lm3553_write_register(&reg_info);
	break;
	case LM3553_GET_TORCH_LEVEL:
		memset(&reg_info, 0, sizeof(reg_info));
		
		reg_info.address = REG_ADDRESS_TORCH_CURRENT_CTRL;
		ret = lm3553_read_register(&reg_info);
		if (ret != 0)
			break;
		
		reg_info.value &= MAX_TORCH_CURRENT_VALUE;
		ret = put_user(reg_info.value, (unsigned char *)arg);
	break;
	case LM3553_SET_OVER_VOLTAGE_PROTECTION:
		switch(arg) {
		case HIGH_LEVEL_OVP: 
			memset(&reg_info, 0, sizeof(reg_info));
			
			reg_info.address = REG_ADDRESS_GP;
			ret = lm3553_read_register(&reg_info);
			if (ret != 0)
				break;
			
			if (0x03 == (reg_info.value & FLASH_MODE_BITMASK)) {
				memset(&reg_info, 0, sizeof(reg_info));
				reg_info.address = REG_ADDRESS_MF_PIN_CTRL;
				ret = lm3553_read_register(&reg_info);
				if (ret != 0)
					break;
				
				reg_info.value |= OVP_BITMASK;
				ret = lm3553_write_register(&reg_info);
			}
			else {
				ret = -EPERM;
			}
			break;
		case LOW_LEVEL_OVP: 
			memset(&reg_info, 0, sizeof(reg_info));
			
			reg_info.address = REG_ADDRESS_MF_PIN_CTRL;
			ret = lm3553_read_register(&reg_info);
			if (ret != 0)
				break;
			
			reg_info.value &= (~OVP_BITMASK);
			ret = lm3553_write_register(&reg_info);
			break;
		default:
			ret = -EFAULT;
			break;
		}
	break;
	case LM3553_GET_DRIVER_VERSION: {
		const char * version_string;
		
		version_string = DRIVER_VERSION;
		ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("lm3553: copy_to_user failed.\r\n");
	}
	break;
	default:
		PDEBUG(KERN_ALERT"lm3553: invalid ioctl case\r\n");
		ret = -ENOTTY;
	break;
	}

	up(&dev->sem);
	
	return ret;
}

static int
lm3553_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	
	PDEBUG("lm3553: %s\n",__FUNCTION__);
	
	filp->private_data = lm3553_dev;
		
	lm3553_set_default_values();
	lm3553_shutdown();
	
	return ret;
}

static int
lm3553_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	
	PDEBUG("lm3553: %s\n",__FUNCTION__);
	
	ret = lm3553_shutdown();
	
	return ret;
}

static const struct file_operations lm3553_fops = {
	.owner	 		= THIS_MODULE,
	.read	 		= lm3553_read,
	.write	 		= lm3553_write,
	.open			= lm3553_open,
	.release 		= lm3553_release,
	.unlocked_ioctl	= lm3553_ioctl,
};


static int 
lm3553_platform_probe(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	return ret;
}

static int 
lm3553_platform_remove(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	return ret;
}

static int 
lm3553_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	lm3553_shutdown();
	
	return ret;
}

static int 
lm3553_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	return ret;
}

static void 
lm3553_platform_shutdown(struct platform_device *device)
{
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
}

static struct platform_driver lm3553_platform_driver = {
	.probe	  = lm3553_platform_probe,
	.remove   = lm3553_platform_remove,
	.suspend  = lm3553_platform_suspend,
	.resume	  = lm3553_platform_resume,
	.shutdown = lm3553_platform_shutdown,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= LM3553_PLATFORM_NAME,
	},
};

static int
lm3553_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = -ENODEV;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	if (LM3553_I2C_ADDRESS == client->addr) /* {PK}: Make sure we probe the correct device  */
	{
		lm3553_dev->i2c_client = client;
		printk (KERN_INFO "lm3553: lm3553 i2c probed\n");

		//ret = lm3553_set_default_values();
		ret = lm3553_shutdown();
		
		ret = 0; /* {PK}: Probing success  */
	}
	
	return ret;
}

static int
lm3553_i2c_remove(struct i2c_client *client)
{
	int ret = 0;
	
	printk(KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	ret = lm3553_shutdown();
	
	return ret;
}

static const struct i2c_device_id lm3553_id[] = {
	{ LM3553_I2C_NAME, 0 },
	{ }
};

static struct i2c_driver lm3553_i2c_driver = {
	.driver = {
		.name  = LM3553_I2C_NAME,
		.owner = THIS_MODULE,
	},
	.probe = lm3553_i2c_probe,
	.remove = lm3553_i2c_remove,
	.id_table = lm3553_id,
};

static struct miscdevice lm3553_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = LM3553_DEV_NAME,
	.fops  = &lm3553_fops,
};

static int
lm3553_add_sysfs(struct lm3553_dev_info *dev_info)
{
	int ret  = 0;
	
#ifdef LM3553_MINOR
	lm3553_miscdevice.minor = LM3553_MINOR;
#endif
	ret = misc_register(&lm3553_miscdevice);
	if (ret < 0 ) {
		printk(KERN_INFO "lm3553: misc_register failed\r\n");
		return ret;
	}
	
	dev_info->major = MISC_MAJOR;
	dev_info->minor = lm3553_miscdevice.minor;
	
	return ret;
}

static int
lm3553_remove_sysfs(void)
{
	int ret = 0;
	
	ret = misc_deregister(&lm3553_miscdevice);
	if (ret < 0)
		printk(KERN_INFO "lm3553: misc_deregister failed\r\n");
	
	return ret;
}

static int 
lm3553_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(LM3553_DEV_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	lm3553_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
lm3553_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(LM3553_DEV_PROC_DIR, NULL);
	
	return ret;
}

#if 0
static int
lm3553_init_cdev(struct lm3553_dev_info *dev_info, const struct file_operations *fops)
{	
	int ret = 0;
	dev_t dev = 0;
	
	dev = MKDEV(dev_info->major, dev_info->minor);
	printk("lm3553: dev_info->major: %d    dev_info->minor: %d\r\n",dev_info->major,dev_info->minor);
	ret = register_chrdev_region(dev, LM3553_NR_DEVS, LM3553_DEV_NAME);
	if (ret < 0) {
		printk(KERN_INFO "lm3553: register_chrdev_region failed\r\n");
		goto err_chr_driver;
	}
	
	cdev_init (&dev_info->cdev, fops);
	dev_info->cdev.owner = THIS_MODULE;
	dev_info->cdev.ops   = fops;
	ret = cdev_add (&dev_info->cdev, dev, 1);
	if (ret) {
		printk(KERN_INFO "lm3553: cdev_add failed\r\n");
		unregister_chrdev_region (dev,1);
	}
	
	err_chr_driver:
	return ret;
}
#endif

static void
lm3553_init_variables(struct lm3553_dev_info *dev)
{	/* {PK} Do any future initializations here */

}

static int __init
lm3553_mod_init(void)
{
	int ret = 0;
	//dev_t dev = 0;
	
	printk (KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	lm3553_dev = (struct lm3553_dev_info *)kmalloc(sizeof(*lm3553_dev), GFP_KERNEL);
	if (NULL == lm3553_dev) {
		ret = -ENOMEM;
		goto err_fail;
	}
	memset (lm3553_dev, 0, sizeof(*lm3553_dev));
	
	lm3553_init_variables(lm3553_dev);
	
	/* {PK} Register platform driver */
	ret = platform_driver_register(&lm3553_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "lm3553: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}
	
	/* {PK} Register platform device */
	lm3553_dev->platform_dev = platform_device_register_simple(LM3553_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(lm3553_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "lm3553: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}
	
	/* {PK} Create sysfs device and register misc device */
	if (lm3553_add_sysfs(lm3553_dev) < 0) {
		goto err_sysfs;
	}
#if 0	
	/* {PK} Initialize the char device */
	ret = lm3553_init_cdev(lm3553_dev, &lm3553_fops);
	if (ret < 0) {
		goto err_chr_driver;
	}
#endif
	sema_init(&lm3553_dev->sem, 1);
	
	/* {PK} Create the proc file */
	if (lm3553_create_proc() < 0) {
		printk(KERN_INFO "lm3553: lm3553_create_proc failed\r\n");
	}
	
	/* {PK} add i2c driver */
	ret = i2c_add_driver(&lm3553_i2c_driver);
	if (ret) {
		printk (KERN_NOTICE "lm3553: driver registration failed\n");
		goto err_i2c_driver;
	}
	
	printk("lm3553: Driver Version: %s\n", DRIVER_VERSION);
	
	return 0; /* {PK} success */
	
err_i2c_driver:
	lm3553_remove_proc();
	//cdev_del(&lm3553_dev->cdev);
	//unregister_chrdev_region(dev,1);
	//err_chr_driver:
	lm3553_remove_sysfs();
err_sysfs:
	platform_device_unregister(lm3553_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&lm3553_platform_driver);
err_platform_driver:
	kfree(lm3553_dev);
err_fail:
	
	return ret;
}

static void __exit
lm3553_mod_exit(void)
{
	//dev_t dev;
	
	printk (KERN_INFO "lm3553: %s\n",__FUNCTION__);
	
	//dev = MKDEV(lm3553_dev->major, lm3553_dev->minor);
	
	/* {PK} Remove i2c driver */
	i2c_del_driver(&lm3553_i2c_driver);
	
	/* {PK} Remove procfs entry */
	if (lm3553_remove_proc() < 0) {
		printk(KERN_ERR "lm3553: lm3553_remove_proc failed\r\n");
	}
	
	/* {PK} Remove char device */
	//cdev_del(&lm3553_dev->cdev);
	//unregister_chrdev_region(dev, LM3553_NR_DEVS);
	
	/* {PK} Remove sysfs entry and deregister misc device */
	lm3553_remove_sysfs();
	
	/* {PK} Remove platform device */
	platform_device_unregister(lm3553_dev->platform_dev);
	platform_driver_unregister(&lm3553_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(lm3553_dev);
}

module_init(lm3553_mod_init);
module_exit(lm3553_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Driver for TCBIN lm3553 led driver");
MODULE_AUTHOR("Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);