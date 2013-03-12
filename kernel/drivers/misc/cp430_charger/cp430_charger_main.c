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

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>

/* {PK} Local includes */
#include "cp430_charger.h"		
#include "cp430_charger_ioctl.h"		
#include "debug.h"
#include "cp430.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static struct device_data 	batt_charger_bus_data;
static struct batt_charger_dev_info	*batt_charger_dev;

atomic_t cmd_reply_received_flag = ATOMIC_INIT(0);

static DECLARE_WAIT_QUEUE_HEAD(command_response_received_wq);

int proc_enable = 1;


/************************** CP430 Callbacks **************************/

int bus_receive_event_handler(unsigned int arg)
{
	if (arg > 0) {
		unsigned char *buffer = kmalloc(arg, GFP_KERNEL);
		
		if (buffer) {
			if (cp430_core_read(CP430_DEV_CHARGER, buffer, arg) < 0) {
				PDEBUG("batt_charger: cp430_core_read failed\r\n");
			}
			else {
				PDEBUG("batt_charger: packet received\r\n");
				if(CP430_DEV_CHARGER == buffer[CP430_DEVICE]) {
					
					switch(buffer[CP430_COMMAND]) {
					case CMD_CHARGER_GET_STATUS:
						if( (0x00 == buffer[CP430_LENGTH_H]) && (0x06 == buffer[CP430_LENGTH_L]) ) {
							
							memcpy(&batt_charger_dev->batt_status.temperature, &buffer[CP430_DATA + 0], sizeof(short));
							batt_charger_dev->batt_status.temperature /= 4;
							
							memcpy(&batt_charger_dev->batt_status.voltage    , &buffer[CP430_DATA + 2], sizeof(short));
							batt_charger_dev->batt_status.status_flags = buffer[CP430_DATA + 4];
							batt_charger_dev->batt_status.rsoc 	   = buffer[CP430_DATA + 5];
							
							atomic_set(&cmd_reply_received_flag, 1);
							wake_up(&command_response_received_wq);
						}
						else {
							PDEBUG("batt_charger: CMD_CHARGER_GET_STATUS invalid data length\r\n");
							
							atomic_set(&cmd_reply_received_flag, -1);
							wake_up(&command_response_received_wq);
						}
					break;
					default:
						PDEBUG("batt_charger: packet received, but unknown command\r\n");
					break;
					}
				}
				else {
					PDEBUG("batt_charger: packet received, but not for us\r\n");
				}
			}
			kfree(buffer);
		}
		else {
			PDEBUG("batt_charger: packet received, but could not read to the buffer\r\n");
		}
	}
	else {
		PDEBUG("batt_charger: receive packet length invalid\r\n");
	}
	return 0;
}

int 
bus_transmit_event_handler(unsigned int arg)
{
	switch (arg)
	{
		case TX_EVENT_IDLE:
		{
			PDEBUG("batt_charger: tx idle event\r\n");
			break;
		}
		default:
		{
			break;
		}
	}
	
	return 0;
}

/************************** Char Driver **************************/
static int
batt_charger_open(struct inode *inode, struct file *filp)
{
	filp->private_data = batt_charger_dev;;
	
	return 0;          
}

static int
batt_charger_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t
batt_charger_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG(KERN_INFO "batt_charger: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Writting is not permitted */
	
	return ret;
}

static ssize_t
batt_charger_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG(KERN_INFO "batt_charger: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Writting is not permitted */
	
	return ret;
}

static long
batt_charger_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	
	int err = 0;
	int ret = 0;
	struct batt_charger_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != BATT_CHARGER_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > BATT_CHARGER_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;

	switch(cmd) {
		case BATT_CHARGER_GET_DRIVER_VERSION: {
			const char * version_string;
			
			version_string = DRIVER_VERSION;
			ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
			if (ret) 
				PDEBUG("batt_charger: copy_to_user failed.\r\n");
		}
		break;
		case BATT_CHARGER_GET_BATT_STATUS: {
			unsigned char cmd_packet[CP430_PACKET_OVERHEAD];
			
			memset(cmd_packet, 0, sizeof(cmd_packet));
			
			ret = cp430_create_packet(CP430_DEV_CHARGER, CMD_CHARGER_GET_STATUS, 0, NULL, cmd_packet);
			if (ret < 0) {
				PDEBUG("batt_charger: cp430_create_packet failed.\r\n");
				ret = -EFAULT;
			}
			
			atomic_set(&cmd_reply_received_flag, 0);
			ret = cp430_core_write(CP430_DEV_DISPLAY, cmd_packet, sizeof(cmd_packet));
			if (ret < 0) {
				PDEBUG("batt_charger: cp430_core_write failed.\r\n");
				ret = -EFAULT;
			}
			
			if (ret >= 0) {
				
				wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (5 * HZ));
				if( 1 == atomic_read(&cmd_reply_received_flag) ) {	
					PDEBUG("batt_charger: command response received\r\n");
					
					ret = copy_to_user((void __user *)arg, &dev->batt_status, sizeof(dev->batt_status));
					if(ret)
						PDEBUG("batt_charger: copy_to_user failed.\r\n");
				}
				else if( -1 == atomic_read(&cmd_reply_received_flag) ) {
					ret = -EFAULT;
					PDEBUG("batt_charger: command response error\r\n");
				}
				else {
					ret = -EFAULT;
					PDEBUG("batt_charger: command response timed out\r\n");
				}
			}
		}
		break;
		default:
			PDEBUG("batt_charger: Invalid ioctl\r\n");
			ret = -ENOTTY;
		break;
	   }
	   
	up(&dev->sem);
	
	return ret;
}

static const struct file_operations batt_charger_fops = {
	.owner 			= THIS_MODULE,
	.read 			= batt_charger_read,
	.write 			= batt_charger_write,
	.open 			= batt_charger_open,
	.release 		= batt_charger_release,
	.unlocked_ioctl = batt_charger_ioctl,
};

/************************** Platform Driver **************************/


static int 
batt_charger_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "batt_charger: %s\r\n",__FUNCTION__);
	
	return ret;
}

static int 
batt_charger_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "batt_charger: %s\r\n",__FUNCTION__);
	
	return ret;
}

static struct platform_driver batt_charger_platform_driver = {
	.suspend  = batt_charger_platform_suspend,
	.resume	  = batt_charger_platform_resume,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= BATT_CHARGER_PLATFORM_NAME,
	},
};

/************************** Device Init/Exit **************************/


static struct miscdevice batt_charger_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = BATT_CHARGER_DEV_NAME,
	.fops  = &batt_charger_fops,
};

static int
batt_charger_add_sysfs(struct batt_charger_dev_info *dev_info)
{
	int ret  = 0;
	
#ifdef BATT_CHARGER_MINOR
	batt_charger_miscdevice.minor = BATT_CHARGER_MINOR;
#endif
	ret = misc_register(&batt_charger_miscdevice);
	if (ret < 0 ) {
		printk(KERN_INFO "batt_charger: misc_register failed\r\n");
		return ret;
	}
	
	dev_info->major = MISC_MAJOR;
	dev_info->minor = batt_charger_miscdevice.minor;
	
	return ret;
}

static int
batt_charger_remove_sysfs(void)
{
	int ret = 0;
	
	ret = misc_deregister(&batt_charger_miscdevice);
	if (ret < 0)
		printk(KERN_INFO "batt_charger: misc_deregister failed\r\n");
	
	return ret;
}

static int 
batt_charger_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(BATT_CHARGER_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	batt_charger_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
batt_charger_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(BATT_CHARGER_PROC_DIR, NULL);
	
	return ret;
}

static void
batt_charger_init_variables(struct batt_charger_dev_info *dev)
{
	dev->major = 0;
	dev->minor = 0;
	dev->platform_dev = NULL;
	dev->proc_dir = NULL;
	memset(&dev->batt_status, 0, sizeof(dev->batt_status));
}

static int __init
batt_charger_mod_init(void)
{
	int ret;

	printk(KERN_INFO "batt_charger: %s\r\n",__FUNCTION__);
	
	batt_charger_dev = (struct batt_charger_dev_info *)kzalloc(BATT_CHARGER_NR_DEVS * sizeof(*batt_charger_dev), GFP_KERNEL);
	if (NULL == batt_charger_dev) {
		printk(KERN_INFO "batt_charger: kmalloc failed\r\n");	
		ret = -ENOMEM;
		goto err_fail;
	}
	
	/* {PK} Initialize the device data structure */
	batt_charger_init_variables(batt_charger_dev);

	/* {PK} Register platform driver */
	ret = platform_driver_register(&batt_charger_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "batt_charger: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}

	/* {PK} Register platform device */
	batt_charger_dev->platform_dev = platform_device_register_simple(BATT_CHARGER_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(batt_charger_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "batt_charger: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}


	/* {PK} Create sysfs device and register misc device */
	if (batt_charger_add_sysfs(batt_charger_dev) < 0) {
		goto err_sysfs;
	}
	
	sema_init(&batt_charger_dev->sem, 1);
	
	/* {PK} Create the proc file */
	if (batt_charger_create_proc() < 0) {
		printk(KERN_INFO "batt_charger: batt_charger_create_proc failed\r\n");
	}

	
	memset(&batt_charger_bus_data, 0, sizeof(batt_charger_bus_data));
	batt_charger_bus_data.receive_event_handler = bus_receive_event_handler;
	batt_charger_bus_data.transmit_event_handler = bus_transmit_event_handler;
	if (cp430_device_register(CP430_DEV_CHARGER, &batt_charger_bus_data) < 0) {
		printk(KERN_INFO "batt_charger: cp430_device_register failed\r\n");
		goto err_bus_register;
	}
	else {
		printk(KERN_INFO "batt_charger: cp430_device_register success\r\n");
	}
	
	printk("batt_charger: Driver Version: %s\n", DRIVER_VERSION);
	
	return 0; /* {PK} success */
	
err_bus_register:
	batt_charger_remove_proc();
	batt_charger_remove_sysfs();
err_sysfs:
	platform_device_unregister(batt_charger_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&batt_charger_platform_driver);
err_platform_driver:
	kfree(batt_charger_dev);
err_fail:
	return ret;
}

void __exit
batt_charger_mod_exit(void)
{

	dev_t dev = 0;
	
	printk(KERN_INFO"batt_charger: %s\r\n",__FUNCTION__);
	
	dev = MKDEV(batt_charger_dev->major, batt_charger_dev->minor);
	
	if (cp430_device_unregister(CP430_DEV_CHARGER) < 0) {
		PDEBUG("batt_charger: cp430_device_unregister failed\r\n");
	}
	else {
		PDEBUG("batt_charger: cp430_device_unregister success\r\n");
	}

	/* {PK} Remove procfs entry */
	if (batt_charger_remove_proc() < 0) {
		printk(KERN_ERR "batt_charger: batt_charger_remove_proc failed\r\n");
	}
	
	/* {PK} Remove sysfs entry and deregister misc device */
	batt_charger_remove_sysfs();

	/* {PK} Remove platform device */
	platform_device_unregister(batt_charger_dev->platform_dev);
	platform_driver_unregister(&batt_charger_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(batt_charger_dev);
}

module_param (proc_enable, int, S_IRUGO);

module_init(batt_charger_mod_init);
module_exit(batt_charger_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Battery Charger Driver for TCBIN");
MODULE_AUTHOR("Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
