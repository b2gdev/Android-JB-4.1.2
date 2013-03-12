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

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>

#include <linux/regulator/consumer.h>	/* {PS} */
#include <linux/delay.h>		/* {PS} */

/* {PS} Local includes */
#include "tcbin_power.h"		
#include "tcbin_power_ioctl.h"		
#include "debug.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static struct tcbin_power_dev_info	*tcbin_power_dev;
int proc_enable = 1; /* {PS} Proc file. Enabled by default. Can be changed via the module param. used in dubug.h */



/************************** Char Driver **************************/

static int
tcbin_power_open(struct inode *inode, struct file *filp)
{
	struct tcbin_power_dev_info *dev;
	
	dev = container_of(inode->i_cdev, struct tcbin_power_dev_info, cdev);
	filp->private_data = dev;
	
	return 0;          
}

static int
tcbin_power_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t
tcbin_power_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	struct tcbin_power_dev_info *dev = NULL;
	
	dev = filp->private_data; 
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	up(&dev->sem);
	
	return retval;
}

static ssize_t
tcbin_power_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	struct tcbin_power_dev_info *dev = NULL;
	
	dev = filp->private_data;
		
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	up(&dev->sem);
	
	return retval;
}

static long
tcbin_power_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	
	int err = 0;
	int ret = 0;
	struct tcbin_power_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != TCBIN_POWER_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > TCBIN_POWER_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;

	switch(cmd) {
		case TCBIN_POWER_GET_DRIVER_VERSION: {
			const char * version_string;
			
			version_string = DRIVER_VERSION;
			ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
			if (ret) 
				PDEBUG("tcbin_power: copy_to_user failed.\r\n");
		}
		break;
		default:
			PDEBUG("tcbin_power: Invalid ioctl\r\n");
			ret = -ENOTTY;
		break;
	   }
	   
	up(&dev->sem);
	
	return ret;
}

static const struct file_operations tcbin_power_fops = {
	.owner 			= THIS_MODULE,
	.read 			= tcbin_power_read,
	.write 			= tcbin_power_write,
	.unlocked_ioctl		= tcbin_power_ioctl,
	.open 			= tcbin_power_open,
	.release 		= tcbin_power_release,
};

/************************** Platform Driver **************************/


static int 
tcbin_power_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "tcbin_power: %s\r\n",__FUNCTION__);
	
	return ret;
}

static int 
tcbin_power_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "tcbin_power: %s\r\n",__FUNCTION__);
	
	return ret;
}

static struct platform_driver tcbin_power_platform_driver = {
	.suspend  = tcbin_power_platform_suspend,
	.resume	  = tcbin_power_platform_resume,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= TCBIN_POWER_PLATFORM_NAME,
	},
};

/************************** Device Init/Exit **************************/

static int
tcbin_power_add_sysfs(void)
{
	struct class *dev_class = NULL;
	
	dev_class = class_create(THIS_MODULE, TCBIN_POWER_DEV_CLASS_NAME);
	if (IS_ERR(dev_class)) {
		printk(KERN_INFO "tcbin_power: class_create failed\r\n");
		return -EFAULT;
	}
	
	if (!device_create(dev_class, NULL, tcbin_power_dev->cdev.dev, NULL, TCBIN_POWER_DEV_NAME)) {
		printk(KERN_INFO "tcbin_power: device_create failed\r\n");
		class_destroy(tcbin_power_dev->dev_class);
		return -EFAULT;
	}
	
	tcbin_power_dev->dev_class = dev_class;
	
	return 0;
}

static int 
tcbin_power_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(TCBIN_POWER_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	tcbin_power_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
tcbin_power_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(TCBIN_POWER_PROC_DIR, NULL);
	
	return ret;
}

static int
tcbin_power_init_cdev(struct tcbin_power_dev_info *dev_info, const struct file_operations *fops)
{	
	int ret = 0;
	dev_t dev = 0;
	
#ifdef TCBIN_POWER_MAJOR
dev_info->major = TCBIN_POWER_MAJOR;
#endif

#ifdef TCBIN_POWER_MINOR
dev_info->minor = TCBIN_POWER_MINOR;
#endif

	if(dev_info->major) {
		dev = MKDEV(dev_info->major, dev_info->minor);
		ret = register_chrdev_region(dev, TCBIN_POWER_NR_DEVS, TCBIN_POWER_DEV_NAME);
		if (ret < 0) {
			printk(KERN_INFO "tcbin_power: register_chrdev_region failed\r\n");
			goto err_chr_driver;
		}
	}
	else {
		dev_info->minor = 0;
		ret = alloc_chrdev_region(&dev, dev_info->minor, TCBIN_POWER_NR_DEVS, TCBIN_POWER_DEV_NAME);
		if (ret < 0) {
			printk(KERN_INFO "tcbin_power: alloc_chrdev_region failed\r\n");
			goto err_chr_driver;
		}
		dev_info->major = MAJOR(dev);
	}	
	
	cdev_init (&dev_info->cdev, fops);
	dev_info->cdev.owner = THIS_MODULE;
	dev_info->cdev.ops   = fops;
	ret = cdev_add (&dev_info->cdev, dev, 1);
	if (ret) {
		printk(KERN_INFO "tcbin_power: cdev_add failed\r\n");
		unregister_chrdev_region (dev,1);
	}
	
err_chr_driver:
	return ret;
}

static void
tcbin_power_init_variables(struct tcbin_power_dev_info *dev)
{
	dev->major = 0;
	dev->minor = 0;
	dev->dev_class = NULL;
	dev->platform_dev = NULL;
	dev->proc_dir = NULL;
}

static int
tcbin_power_init_regulators(void)
{
	int ret = 0;
	struct regulator *reg;

	/* {PS} : enable VCC_VAUX4_2V5 supply */
	
	/* {SW} BEGIN:vcc_vaux4_2v5 regulator will be enabled/disabled in the relevant drivers */
   /*
   reg = regulator_get(NULL, "vcc_vaux4_2v5");
	
	if (IS_ERR(reg)) {
		printk(KERN_ERR "get vcc_vaux4_2v5 regulator failed\n");
		PERROR("get vcc_vaux4_2v5 regulator failed\n");
		return PTR_ERR(reg);
	}
        
	if (!regulator_is_enabled(reg))
	{
		ret = regulator_enable(reg);
		if (ret) {
			regulator_put(reg);
			PERROR("vcc_vaux4_2v5 regulator_enable failed\n");
			return ret;
		}

		PDEBUG("vcc_vaux4_2v5 regulator enabled\n");
	}
	else {
		PDEBUG("vcc_vaux4_2v5 regulator is already enabled\n");
	}
   */
   /* {SW} END: */

	/* {PS} : enable VSIM supply */
	
	ret = 0;
	reg = regulator_get(NULL, "vsim");
	
	if (IS_ERR(reg)) {
		printk(KERN_ERR "get vsim regulator failed\n");
		PERROR("get vsim regulator failed\n");
		return PTR_ERR(reg);
	}
        
	if (!regulator_is_enabled(reg))
	{
		ret = regulator_enable(reg);
		if (ret) {
			regulator_put(reg);
			PERROR("vsim regulator_enable failed\n");
			return ret;
		}

		PDEBUG("vsim regulator enabled\n");
	}
	else {
		PDEBUG("vsim regulator is already enabled\n");
	}
	
	return ret;
}

static int __init
tcbin_power_mod_init(void)
{
	int ret;
	dev_t dev = 0;		/* {PS} */


	printk(KERN_INFO "tcbin_power: %s\r\n",__FUNCTION__);
	
	tcbin_power_dev = (struct tcbin_power_dev_info *)kzalloc(TCBIN_POWER_NR_DEVS * sizeof(*tcbin_power_dev), GFP_KERNEL);
	if (NULL == tcbin_power_dev) {
		printk(KERN_INFO "tcbin_power: kmalloc failed\r\n");	
		ret = -ENOMEM;
		goto err_fail;
	}
	memset (tcbin_power_dev, 0, sizeof(TCBIN_POWER_NR_DEVS * sizeof(*tcbin_power_dev)));
	
	/* {PS} Initialize the device data structure */
	tcbin_power_init_variables(tcbin_power_dev);

	/* {PS} Register platform driver */
	ret = platform_driver_register(&tcbin_power_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "tcbin_power: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}

	/* {PS} Register platform device */
	tcbin_power_dev->platform_dev = platform_device_register_simple(TCBIN_POWER_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(tcbin_power_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "tcbin_power: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}

	/* {PS} Initialize the char device */
	ret = tcbin_power_init_cdev(tcbin_power_dev, &tcbin_power_fops);
	if (ret < 0) {
		goto err_chr_driver;
	}

	sema_init(&tcbin_power_dev->sem, 1);
	
	/* {PS} Create the proc file */
	if (tcbin_power_create_proc() < 0) {
		printk(KERN_INFO "tcbin_power: tcbin_power_create_proc failed\r\n");
	}

	/* {PS} Create sysfs entry */
	if (tcbin_power_add_sysfs() < 0) {
		printk(KERN_INFO "tcbin_power: tcbin_power_add_sysfs failed\r\n");
	}
	
	/* {PS} Initialize regulators */
	ret = tcbin_power_init_regulators();
	if (ret < 0) {
		goto err_regulators;
	}
	
	printk("tcbin_power: Driver Version: %s\n", DRIVER_VERSION);
	
	return 0; /* {PS} success */

err_regulators:
	dev = MKDEV(tcbin_power_dev->major, tcbin_power_dev->minor);	/* {PS} */
	device_destroy(tcbin_power_dev->dev_class, dev);
	class_destroy(tcbin_power_dev->dev_class);
	tcbin_power_remove_proc();
	cdev_del(&tcbin_power_dev->cdev);
	unregister_chrdev_region(dev,1);
err_chr_driver:
	platform_device_unregister(tcbin_power_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&tcbin_power_platform_driver);
err_platform_driver:
	kfree(tcbin_power_dev);
err_fail:
	return ret;
}

void __exit
tcbin_power_mod_exit(void)
{

	dev_t dev = 0;
	
	printk(KERN_INFO"tcbin_power: %s\r\n",__FUNCTION__);
	
	dev = MKDEV(tcbin_power_dev->major, tcbin_power_dev->minor);
	
	/* {PS} Remove sysfs entry */
	device_destroy(tcbin_power_dev->dev_class, dev);
	class_destroy(tcbin_power_dev->dev_class);
	
	/* {PS} Remove procfs entry */
	if (tcbin_power_remove_proc() < 0) {
		printk(KERN_ERR "tcbin_power: tcbin_power_remove_proc failed\r\n");
	}
	
	/* {PS} Remove char device */
	cdev_del(&tcbin_power_dev->cdev);
	unregister_chrdev_region(dev, TCBIN_POWER_NR_DEVS);

	/* {PS} Remove platform device */
	platform_device_unregister(tcbin_power_dev->platform_dev);
	platform_driver_unregister(&tcbin_power_platform_driver);
	
	/* {PS} Free allocated data structures */
	kfree(tcbin_power_dev);
	
}

module_param (proc_enable, int, S_IRUGO);

module_init(tcbin_power_mod_init);
module_exit(tcbin_power_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("TCBIN System Power");
MODULE_AUTHOR("Prasad Samaraweera <prasads@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
