
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
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <linux/gpio.h>

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>
/* {SW} BEGIN: header files needed to control power regulator */
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
/* {SW} END: */

#include "w2sg0006.h"
#include "w2sg0006_ioctl.h"
#include "debug.h"

int proc_enable = 1;

static struct w2sg0006_dev_info *w2sg0006_dev;

/* {SW} BEGIN:regulator pointer definition */
static struct regulator *w2sg0006_reg;
/* {SW} END: */ 


static void gps_pwr_enable(void){
	gpio_request(163, "GPS_PWR_EN");			/* GPS_PWR_EN		*/
	gpio_direction_output(163, 0);		/* GPS_PWR_EN		- LOW	- Turn off GPS power supply */
	gpio_set_value(163, 1);		/* GPS_PWR_EN		- HIGH	- Turn on GPS power supply */
	
	/*u16 reg;
	u32 val;
	reg = OMAP2_CONTROL_DEVCONF0;
	val = omap_ctrl_readl(reg);
	val = val | 0x18; 
	pad_mux_value &= (~AM33XX_PULL_DISA);
	omap_ctrl_writel(val, reg);*/
	
	mdelay(300);
	return;
}

static void gps_pwr_disable(void){
	gpio_request(163, "GPS_PWR_EN");			/* GPS_PWR_EN		*/
	gpio_direction_output(163, 0);		/* GPS_PWR_EN		- LOW	- Turn off GPS power supply */
	gpio_set_value(163, 0);		/* GPS_PWR_EN		- LOW	- Turn off GPS power supply */	
	
	/*u16 reg;
	u32 val;
	reg = OMAP2_CONTROL_DEVCONF0;
	val = omap_ctrl_readl(reg);
	val = val | 0x18; 
	pad_mux_value |= AM33XX_PULL_DISA;
	omap_ctrl_writel(val, reg);*/
	
	mdelay(300);
	return;
}
	
static void gps_enable_toggle(void)
{
    gpio_request(140, "GPS_nEN");				/* GPS_nEN			*/
    gpio_direction_output(140, 0);		/* GPS_nEN			- HIGH	- Disable GPS */    
	gpio_set_value(140, 0);		/* GPS_EN			- LOW	- Disable GPS */
    mdelay(400);
	gpio_set_value(140, 1);		/* GPS_EN			- HIGH	- Enable GPS */   
    return;
}

static int
w2sg0006_shutdown(void)
{
	return 0;
}

static ssize_t 
w2sg0006_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG("w2sg0006: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Reading is not permitted */
	
	return ret;
}

static ssize_t 
w2sg0006_write(struct file *filp, const char __user *buf,size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG("w2sg0006: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Writting is not permitted */
	
	return ret;
}

static long 
w2sg0006_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int err = 0;
	struct w2sg0006_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	PDEBUG("w2sg0006: %s\n",__FUNCTION__);
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != W2SG0006_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > W2SG0006_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;
		
	switch(cmd) {
	case W2SG0006_IS_ENABLE: {
		unsigned char t_is_enable = dev->is_enable;
		ret = copy_to_user((void *)arg, (const void *)(&t_is_enable), 1) ? -EFAULT : 0;
		if (ret) 
			PDEBUG("w2sg0006: copy_to_user failed.\r\n");
	}
	break;
	case W2SG0006_ENABLE: {		
		if(!dev->is_enable){
			gps_pwr_enable();
			dev->is_enable = 1;	
		}		
		gps_enable_toggle();		
		PDEBUG("w2sg0006: ENABLED\r\n");
		printk(KERN_INFO "w2sg0006_ioctl: GPS ENABLED\n");		
	}
	break;
	case W2SG0006_DISABLE:{		
		gps_enable_toggle();
		PDEBUG("w2sg0006: DISABLED\r\n");
		printk(KERN_INFO "w2sg0006_ioctl: GPS DISABLED\n");
	}
	break;
	case W2SG0006_PWR_ON: {		
		gps_pwr_enable();		
		dev->is_enable = 1;
		PDEBUG("w2sg0006: PWR ON\r\n");
		printk(KERN_INFO "w2sg0006_ioctl: GPS PWR ON\n");		
	}
	break;
	case W2SG0006_PWR_OFF:{		
		gps_pwr_disable();
		dev->is_enable = 0;
		PDEBUG("w2sg0006: PWR OFF\r\n");
		printk(KERN_INFO "w2sg0006_ioctl: GPS PWR OFF\n");
	}
	break;
	default:
		ret = -ENOTTY;
	break;
    }
    
    up(&dev->sem);
    
    return ret;
}

static int 
w2sg0006_open(struct inode *inode, struct file *filp )
{
	int ret = 0;
	
	PDEBUG("w2sg0006_open: %s\n",__FUNCTION__);
	
	filp->private_data = w2sg0006_dev;
	
	/* {SW} BEGIN: open should not set default settings or shutdown the device */
   //w2sg0006_set_defaults(w2sg0006_dev);
	//w2sg0006_shutdown();
   /* {SW} END: */
	
	return ret;
}

static int 
w2sg0006_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	
	PDEBUG("w2sg0006_release: %s\n",__FUNCTION__);
	
	/* {SW} BEGIN: release should not shutdown the device */
   //w2sg0006_shutdown();
   /* {SW} END: */
	
	return ret;
}

/* {SW} BEGIN: functions needed to control power regulator */
/*
static int w2sg0006_reg_get (void)
{
   w2sg0006_reg = regulator_get (NULL, "vcc_vaux4_2v5");
	
   if (IS_ERR(w2sg0006_reg))
   {
      printk (KERN_ERR "%s: get vcc_vaux4_2v5 regulator failed\n", __FUNCTION__);
      return PTR_ERR(w2sg0006_reg);
   }
   
   return 0;
}

static void w2sg0006_reg_put (void)
{
   regulator_put (w2sg0006_reg);
}

static int w2sg0006_reg_power_on (void)
{
   int ret = 0;
   
   ret = regulator_enable (w2sg0006_reg);
   if (ret < 0)
   {
      printk (KERN_ERR "%s: vcc_vaux4_2v5 regulator_enable failed\n", __FUNCTION__);
      return ret;
   }
   //{SW} IMPORTANT: delay added in order to give enough time to the regulator + compass chip to be stabalize
   //after turning on the regulator before any i2c communication is performed, can be fine tuned as neccessary
   mdelay(10);
   
   return ret;
}

static int w2sg0006_reg_power_off (void)
{
   int ret = 0;
   
   ret = regulator_disable (w2sg0006_reg);
   if (ret < 0)
   {
      printk (KERN_ERR "%s: vcc_vaux4_2v5 regulator_disable failed\n", __FUNCTION__);
   }
   
   return ret;
}
*/
/* {SW} END: */

static const struct file_operations w2sg0006_fops = {
	.owner	 		= THIS_MODULE,
	.read	 		= w2sg0006_read,
	.write	 		= w2sg0006_write,
	.open	 		= w2sg0006_open,
	.release 		= w2sg0006_release,
	.unlocked_ioctl = w2sg0006_ioctl,
};

static int 
w2sg0006_platform_probe(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
   
   /* {SW} BEGIN: driver probe will enable the power regulator at the beginning */
   /*ret = w2sg0006_reg_get();
   if (ret < 0)
   {
      goto err0;
   }*/
   
   //ret = w2sg0006_reg_power_on();
   if (ret < 0)
   {
      goto err1;
   }
   
   gps_pwr_disable();
   w2sg0006_dev->is_enable = 0;
   PDEBUG("w2sg0006_probe: DISABLED\r\n");
   printk(KERN_INFO "w2sg0006_probe: GPS DISABLED\n");
   
   return 0;
   
err1:
   //w2sg0006_reg_put();
//err0:
   printk(KERN_ERR "%s: failed to probe w2sg0006\n", __FUNCTION__);
   return ret;
   /* {SW} END: */
}

static int 
w2sg0006_platform_remove(struct platform_device * device)
{
	int ret = 0;
	
	printk(KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
   
   /* {SW} BEGIN: driver release will power off the regulator */
   //w2sg0006_reg_power_off();
   //w2sg0006_reg_put();
   /* {SW} END: */
	
	return ret;
}

static int 
w2sg0006_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
	
	//w2sg0006_shutdown();
   /* {SW} BEGIN: power off the regulator */
   //w2sg0006_reg_power_off();
   /* {SW} END: */
	
	return ret;
}

static int 
w2sg0006_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
	
	/* {SW} BEGIN: turn on the regulator and set the device settings back */
   //ret = w2sg0006_reg_power_on();
   if (ret < 0)
   {
      goto err0;
   }
   
   if (ret < 0)
   {
      goto err0;
   }
   
   return 0;
   
err0:
   printk(KERN_ERR "%s: failed to resume w2sg0006\n", __FUNCTION__);
   return ret;
   /* {SW} END: */
}

static void 
w2sg0006_platform_shutdown(struct platform_device *device)
{
	printk(KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
	gps_pwr_disable();
	printk(KERN_INFO "GPS Powered Down\n",__FUNCTION__);
}

static struct platform_driver w2sg0006_platform_driver = {
	.probe	  = w2sg0006_platform_probe,
	.remove   = w2sg0006_platform_remove,
	.suspend  = w2sg0006_platform_suspend,
	.resume	  = w2sg0006_platform_resume,
	.shutdown = w2sg0006_platform_shutdown,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= W2SG0006_PLATFORM_NAME,
	},
};

static struct miscdevice w2sg0006_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = W2SG0006_DEV_NAME,
	.fops  = &w2sg0006_fops,
};

static int
w2sg0006_add_sysfs(struct w2sg0006_dev_info *dev_info)
{
	int ret  = 0;
	
#ifdef W2SG0006_MINOR
	w2sg0006_miscdevice.minor = W2SG0006_MINOR;
#endif
	ret = misc_register(&w2sg0006_miscdevice);
	if (ret < 0 ) {
		printk(KERN_INFO "w2sg0006: misc_register failed\r\n");
		return ret;
	}
	
	dev_info->major = MISC_MAJOR;
	dev_info->minor = w2sg0006_miscdevice.minor;
	
	return ret;
}

static int
w2sg0006_remove_sysfs(void)
{
	int ret = 0;
	
	ret = misc_deregister(&w2sg0006_miscdevice);
	if (ret < 0)
		printk(KERN_INFO "w2sg0006: misc_deregister failed\r\n");
	
	return ret;
}

static int 
w2sg0006_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(W2SG0006_DEV_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	w2sg0006_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
w2sg0006_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(W2SG0006_DEV_PROC_DIR, NULL);
	
	return ret;
}

static void
w2sg0006_init_variables(struct w2sg0006_dev_info *dev)
{	/* {PK} Do any future initializations here */
	dev->is_enable = 0;
}

static int __init
w2sg0006_init(void)
{
	int ret = 0;
	
	printk (KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
	
	w2sg0006_dev = (struct w2sg0006_dev_info *)kmalloc(sizeof(*w2sg0006_dev), GFP_KERNEL);
	if (NULL == w2sg0006_dev) {
		ret = -ENOMEM;
		goto err_fail;
	}
	memset (w2sg0006_dev, 0, sizeof(*w2sg0006_dev));
	
	w2sg0006_init_variables(w2sg0006_dev);
	
	/* {PK} Register platform driver */
	ret = platform_driver_register(&w2sg0006_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "w2sg0006: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}
	
	/* {PK} Register platform device */
	w2sg0006_dev->platform_dev = platform_device_register_simple(W2SG0006_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(w2sg0006_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "w2sg0006: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}
	
	/* {PK} Create sysfs device and register misc device */
	if (w2sg0006_add_sysfs(w2sg0006_dev) < 0) {
		goto err_sysfs;
	}

	sema_init(&w2sg0006_dev->sem, 1);

	/* {PK} Create the proc file */
	if (w2sg0006_create_proc() < 0) {
		printk(KERN_INFO "w2sg0006: w2sg0006create_proc failed\r\n");
	}

	printk("w2sg0006: Driver Version: %s\n", DRIVER_VERSION);

	return 0; /* {PK} success */

//err_i2c_driver:
	w2sg0006_remove_proc();
	w2sg0006_remove_sysfs();
err_sysfs:
	platform_device_unregister(w2sg0006_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&w2sg0006_platform_driver);
err_platform_driver:
	kfree(w2sg0006_dev);
err_fail:
	
	return ret;
}


static void __exit 
w2sg0006_exit(void)
{
	printk (KERN_INFO "w2sg0006: %s\n",__FUNCTION__);
	
	/* {PK} Remove procfs entry */
	if (w2sg0006_remove_proc() < 0) {
		printk(KERN_ERR "w2sg0006: lm3553_remove_proc failed\r\n");
	}
	
	/* {PK} Remove sysfs entry and deregister misc device */
	w2sg0006_remove_sysfs();
	
	/* {PK} Remove platform device */
	platform_device_unregister(w2sg0006_dev->platform_dev);
	platform_driver_unregister(&w2sg0006_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(w2sg0006_dev);
}

module_init(w2sg0006_init);
module_exit(w2sg0006_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Driver for TCBIN w2sg0006 compass driver");
MODULE_AUTHOR("Thilina Bandara <thilinab@zone24x7.com> and Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
