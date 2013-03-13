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
#include <linux/gpio.h>
#include <linux/earlysuspend.h>

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>
#include <mach/gpio.h>

/* {KW} Local includes */
#include "cp430_power.h"		
#include "debug.h"
#include "cp430.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static struct device_data 	cp430_power_bus_data;
static struct cp430_power_dev_info	*cp430_power_dev;

atomic_t cmd_reply_received_flag = ATOMIC_INIT(0);

static DECLARE_WAIT_QUEUE_HEAD(command_response_received_wq);

int proc_enable = 1;

unsigned short masked_flag = SYS_NOFLAG; /*{KW}: separate flag from rx event arg */

/************************** CP430 Callbacks **************************/
/** {KW}:
 * Called by cp430_core driver on data reception for this device.
 * @arg: 32bit value containing 2 fields as follows.
 * -----------------------------------------
 * |   16bit FLAG      | 16bit data length |
 * -----------------------------------------
 * FLAG could be one of: SYS_NOFLAG   (0x0000)
 * 						 SYS_RESUMING (0x0001) 
 **/
int cp430_power_receive_event_handler(unsigned int arg)
{	
	unsigned int masked_arg = arg & 0x0000FFFF; /*{KW}: ignore the MSB 16bit FLAG */
	masked_flag = (arg & 0xFFFF0000) >> 16;
	
	if (masked_arg > 0) {
		unsigned char *buffer = kmalloc(masked_arg, GFP_KERNEL);
		
		if (buffer) {
			if (cp430_core_read(CP430_DEV_POWER, buffer, masked_arg) < 0) {
				PDEBUG("power: cp430_core_read failed\r\n");
			}
			else {
				PDEBUG("power: packet received\r\n");
				if(CP430_DEV_POWER== buffer[CP430_DEVICE]) {
					
					// process commmand here {KW}
					switch(buffer[CP430_COMMAND]) {
					case CMD_POWER_GET_STATUS:
						if( (0x00 == buffer[CP430_LENGTH_H]) && (0x01 == buffer[CP430_LENGTH_L]) ) {							
							if(0x00 == buffer[CP430_DATA])
							{
								atomic_set(&cmd_reply_received_flag, 1);
								wake_up(&command_response_received_wq);
							}
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
					
					PDEBUG("power: packet received, process it here !\r\n");
				}
				else {
					PDEBUG("power: packet received, but not for us\r\n");
				}
			}
			kfree(buffer);
		}
		else {
			PDEBUG("power: packet received, but could not read to the buffer\r\n");
		}
	}
	else {
		PDEBUG("power: receive packet length invalid\r\n");
	}
	return 0;
}

int 
cp430_power_transmit_event_handler(unsigned int arg)
{
	switch (arg)
	{
		case TX_EVENT_IDLE:
		{
			PDEBUG("power: tx idle event\r\n");
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
cp430_power_open(struct inode *inode, struct file *filp)
{
	filp->private_data = cp430_power_dev;;
	
	return 0;          
}

static int
cp430_power_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t
cp430_power_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG(KERN_INFO "power: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {KW}: Writting is not permitted */
	
	return ret;
}

static ssize_t
cp430_power_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG(KERN_INFO "power: %s\n",__FUNCTION__);
	
	ret = -EPERM; /* {KW}: Writting is not permitted */
	
	return ret;
}
static const struct file_operations cp430_power_fops = {
	.owner 			= THIS_MODULE,
	.read 			= cp430_power_read,
	.write 			= cp430_power_write,
	.open 			= cp430_power_open,
	.release 		= cp430_power_release,	
};
/************************** Power Status Update **********************/

static int
cp430_power_status_update(unsigned char power_status){
	
	int ret = 0;
	unsigned char current_power_status;
	unsigned char cmd_packet[CP430_PACKET_OVERHEAD + 1];
	
	current_power_status = power_status;	
	memset(cmd_packet, 0, sizeof(cmd_packet));	
	
	ret = cp430_create_packet(CP430_DEV_POWER, CMD_POWER_GET_STATUS, 1, &current_power_status, cmd_packet);
	if (ret < 0) {
		PDEBUG("power: cp430_create_packet failed.\r\n");
		ret = -EFAULT;
	}
	
	atomic_set(&cmd_reply_received_flag, 0);
	ret = cp430_core_write(CP430_DEV_POWER, cmd_packet, sizeof(cmd_packet));
	if (ret < 0) {
		PDEBUG("power: cp430_core_write failed.\r\n");
		ret = -EFAULT;
	}
	
	if (ret >= 0) {
		
		wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (1 * HZ)); //5*HZ
		if( 1 == atomic_read(&cmd_reply_received_flag) ) {	
			PDEBUG("power: command response received\r\n");			
		}
		else {
			ret = -EFAULT;
			PDEBUG("power: command response timed out\r\n");			
		}
	}
	return ret;
}

/************************** Platform Driver **************************/
static int 
cp430_power_platform_probe(struct platform_device *device)
{
	int ret = 0;	
	
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);		
		
	//power status gpio set
	gpio_set_value(CP430_PWR_STAT_GPIO, 1);
	
	return ret;
}

static int 
cp430_power_platform_remove(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "cp430_power: %s\n",__FUNCTION__);	
	
	return ret;
}

static int 
cp430_power_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);	
	
	//update power state
	cp430_power_dev->power_status = POWER_SUSPEND;
	
	//send power status update command here
	ret = cp430_power_status_update(cp430_power_dev->power_status);
	if(ret < 0)
	{
		PDEBUG("power: cp430_power_status_update failed\r\n");
	}
	
	return ret;
}

static int 
cp430_power_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);
	
	//update power state
	cp430_power_dev->power_status = POWER_MIDDLE;
	
	//send power status update command here
	ret = cp430_power_status_update(cp430_power_dev->power_status);
	if(ret < 0)
	{
		PDEBUG("power: cp430_power_status_update failed\r\n");
	}
	
	return ret;
}



static void 
cp430_power_platform_shutdown(struct platform_device *device)
{
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);
	
	//power status gpio set
	/*{KW}: This is moved to twl4030-power.c twl4030_power_off function.
	gpio_set_value(CP430_PWR_STAT_GPIO, 0);
	* */
}

static struct platform_driver cp430_power_platform_driver = {
	.probe    = cp430_power_platform_probe,
	.remove   = cp430_power_platform_remove,
	.suspend  = cp430_power_platform_suspend,
	.resume	  = cp430_power_platform_resume,
	.shutdown = cp430_power_platform_shutdown,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= CP430_POWER_PLATFORM_NAME,
	},
};

/************************ Early suspend functions *********************/
static void 
cp430_power_early_suspend(struct early_suspend *h)
{
	int ret = 0;
	
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);	
	
	//update power state
	cp430_power_dev->power_status = POWER_MIDDLE;
	
	//send power status update command here
	ret = cp430_power_status_update(cp430_power_dev->power_status);
	if(ret < 0)
	{
		PDEBUG("power: cp430_power_status_update failed\r\n");
	}
	
	return;
}

static void 
cp430_power_late_resume(struct early_suspend *h)
{
	int ret = 0;
	
	printk(KERN_INFO "power: %s\r\n",__FUNCTION__);
	
	//update power state
	cp430_power_dev->power_status = POWER_ACTIVE;
	
	//send power status update command here
	ret = cp430_power_status_update(cp430_power_dev->power_status);
	if(ret < 0)
	{
		PDEBUG("power: cp430_power_status_update failed\r\n");
	}
	
	return;
}
static struct early_suspend cp430_power_early_suspend_handler = {
	.suspend = cp430_power_early_suspend,
	.resume = cp430_power_late_resume,
};

/************************** Device Init/Exit **************************/


static struct miscdevice cp430_power_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = CP430_POWER_DEV_NAME,
	.fops  = &cp430_power_fops,
};

static int
cp430_power_add_sysfs(struct cp430_power_dev_info *dev_info)
{
	int ret  = 0;
	
#ifdef CP430_POWER_MINOR
	cp430_power_miscdevice.minor = CP430_POWER_MINOR;
#endif
	ret = misc_register(&cp430_power_miscdevice);
	if (ret < 0 ) {
		printk(KERN_INFO "power: misc_register failed\r\n");
		return ret;
	}
	
	dev_info->major = MISC_MAJOR;
	dev_info->minor = cp430_power_miscdevice.minor;
	
	return ret;
}

static int
cp430_power_remove_sysfs(void)
{
	int ret = 0;
	
	ret = misc_deregister(&cp430_power_miscdevice);
	if (ret < 0)
		printk(KERN_INFO "power: misc_deregister failed\r\n");
	
	return ret;
}

static int 
cp430_power_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(CP430_POWER_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	cp430_power_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
cp430_power_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(CP430_POWER_PROC_DIR, NULL);
	
	return ret;
}

static void
cp430_power_init_variables(struct cp430_power_dev_info *dev)
{
	dev->major = 0;
	dev->minor = 0;
	dev->platform_dev = NULL;
	dev->proc_dir = NULL;
	dev->power_status = 0;	
}

static int __init
cp430_power_mod_init(void)
{
	int ret;
	
	cp430_power_dev = (struct cp430_power_dev_info *)kzalloc(CP430_POWER_NR_DEVS * sizeof(*cp430_power_dev), GFP_KERNEL);
	if (NULL == cp430_power_dev) {
		printk(KERN_INFO "power: kmalloc failed\r\n");	
		ret = -ENOMEM;
		goto err_fail;
	}
	
	/* {KW} Initialize the device data structure */
	cp430_power_init_variables(cp430_power_dev);

	/* {KW} Register platform driver */
	ret = platform_driver_register(&cp430_power_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "power: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}

	/* {KW} Register platform device */
	cp430_power_dev->platform_dev = platform_device_register_simple(CP430_POWER_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(cp430_power_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "power: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}
	
	/*{KW}: early suspend/ resume */
	register_early_suspend(&cp430_power_early_suspend_handler);
	
	/* {KW} Create sysfs device and register misc device */
	if (cp430_power_add_sysfs(cp430_power_dev) < 0) {
		goto err_sysfs;
	}
	
	sema_init(&cp430_power_dev->sem, 1);
	
	/* {KW} Create the proc file */
	if (cp430_power_create_proc() < 0) {
		printk(KERN_INFO "power: power_create_proc failed\r\n");
	}

	
	memset(&cp430_power_bus_data, 0, sizeof(cp430_power_bus_data));
	cp430_power_bus_data.receive_event_handler = cp430_power_receive_event_handler;
	cp430_power_bus_data.transmit_event_handler = cp430_power_transmit_event_handler;
	if (cp430_device_register(CP430_DEV_POWER, &cp430_power_bus_data) < 0) {
		printk(KERN_INFO "power: cp430_device_register failed\r\n");
		goto err_bus_register;
	}
	else {
		printk(KERN_INFO "power: cp430_device_register success\r\n");
	}
	
	printk("power: Driver Version: %s\n", DRIVER_VERSION);
	
	return 0;  /* {KW} success */
	
err_bus_register:
	cp430_power_remove_proc();
	cp430_power_remove_sysfs();
err_sysfs:
	platform_device_unregister(cp430_power_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&cp430_power_platform_driver);
err_platform_driver:
	kfree(cp430_power_dev);
err_fail:
	return ret;
}

void __exit cp430_power_mod_exit(void)
{

	dev_t dev = 0;
	
	printk(KERN_INFO"power: %s\r\n",__FUNCTION__);
	
	dev = MKDEV(cp430_power_dev->major, cp430_power_dev->minor);
	
	if (cp430_device_unregister(CP430_DEV_POWER) < 0) {
		PDEBUG("power: cp430_device_unregister failed\r\n");
	}
	else {
		PDEBUG("power: cp430_device_unregister success\r\n");
	}

	/* {KW} Remove procfs entry */
	if (cp430_power_remove_proc() < 0) {
		printk(KERN_ERR "power: power_remove_proc failed\r\n");
	}
	
	/* {KW} Remove sysfs entry and deregister misc device */
	cp430_power_remove_sysfs();

	/*{KW}: early suspend/ resume */
	unregister_early_suspend(&cp430_power_early_suspend_handler);
	/* {KW} Remove platform device */
	platform_device_unregister(cp430_power_dev->platform_dev);
	platform_driver_unregister(&cp430_power_platform_driver);
	
	/* {KW} Free allocated data structures */
	kfree(cp430_power_dev);
}

module_param (proc_enable, int, S_IRUGO);

module_init(cp430_power_mod_init);
module_exit(cp430_power_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Cp430 Power Driver for TCBIN");
MODULE_AUTHOR("Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
