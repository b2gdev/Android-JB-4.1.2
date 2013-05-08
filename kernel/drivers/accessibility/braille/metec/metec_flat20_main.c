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
#include <linux/earlysuspend.h>

#include <asm/system.h>		
#include <asm/uaccess.h>	
#include <linux/sched.h>

/* {PK} Local includes */
#include "metec_flat20.h"		
#include "debug.h"
#include "cp430.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static struct device_data 	metec_flat20_bus_data;
static struct metec_flat20_dev_info	*metec_flat20_dev;

atomic_t cmd_reply_received_flag = ATOMIC_INIT(0);

static DECLARE_WAIT_QUEUE_HEAD(command_response_received_wq);

int proc_enable = 1; /* {PK} Proc file. Enabled by default. Can be changed via the module param. used in dubug.h */

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
int bus_receive_event_handler(unsigned int arg)
{
	unsigned int masked_arg = arg & 0x0000FFFF; /*{KW}: ignore the MSB 16bit FLAG */
	masked_flag = (arg & 0xFFFF0000) >> 16;
		
	if (masked_arg > 0) {
		unsigned char *buffer = kmalloc(masked_arg, GFP_KERNEL);
		
		if (buffer) {
			if (cp430_core_read(CP430_DEV_DISPLAY, buffer, masked_arg) < 0) {
				PDEBUG("metec_flat20: cp430_core_read failed\r\n");
			}
			else {
				PDEBUG("metec_flat20: packet received\r\n");
				if(CP430_DEV_DISPLAY == buffer[CP430_DEVICE]) {
					switch(buffer[CP430_COMMAND]) {
					case CMD_DISPLAY_ON_OFF:
					case CMD_DISPLAY_WRITE:
						if(buffer[CP430_DATA] != 0x00) {
							PDEBUG("metec_flat20: Error in Command Response %02X\r\n", buffer[CP430_DATA]);
							atomic_set(&cmd_reply_received_flag, -1);
							//command_response_received_flag = -1;
							wake_up(&command_response_received_wq);
						} 
						else {
							atomic_set(&cmd_reply_received_flag, 1);
							//command_response_received_flag = 1;
							wake_up(&command_response_received_wq);
						}
						break;
					default:
						PDEBUG("metec_flat20: packet received, but unknown command\r\n");
						break;
					}
				}
				else {
					PDEBUG("metec_flat20: packet received, but not for us\r\n");
				}
			}
			kfree(buffer);
		}
		else {
			PDEBUG("metec_flat20: packet received, but could not read to the buffer\r\n");
		}
	}
	else {
		PDEBUG("metec_flat20: receive packet length invalid\r\n");
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
			PDEBUG("metec_flat20: tx idle event\r\n");
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
control_display(unsigned char control)
{
	int ret = 0;
	
	unsigned char cmd_packet[1 + CP430_PACKET_OVERHEAD];
	
	memset(cmd_packet, 0, sizeof(cmd_packet));
	
	ret = cp430_create_packet(CP430_DEV_DISPLAY, CMD_DISPLAY_ON_OFF, 1, &control, cmd_packet);
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_create_packet failed.\r\n");
		ret = -EFAULT;
	}
	
	atomic_set(&cmd_reply_received_flag, 0);
	ret = cp430_core_write(CP430_DEV_DISPLAY, cmd_packet, sizeof(cmd_packet));
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_core_write failed.\r\n");
		ret = -EFAULT;
	}
	
	if (ret >= 0) {
		wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (5 * HZ));
		if( 1 == atomic_read(&cmd_reply_received_flag))
		{	
			PDEBUG("metec_flat20: command response received\r\n");
		}
		else
		{
			ret = -EFAULT;
			PDEBUG("metec_flat20: command response timed out\r\n");
		}
	}
	
	return ret;
}

static int
clear_display(struct metec_flat20_dev_info *dev, int is_suspend)
{
	int ret = 0;
	unsigned char cmd_packet[MAX_BRAILLE_LINE_SIZE + METEC_FLAT20_PACKET_OVERHEAD + CP430_PACKET_OVERHEAD];
	unsigned char cmd_msg[MAX_BRAILLE_LINE_SIZE + METEC_FLAT20_PACKET_OVERHEAD];
	
	memset(cmd_packet, 0, sizeof(cmd_packet));
	memset(cmd_msg, 0, sizeof(cmd_msg));
	
	ret = cp430_create_packet(CP430_DEV_DISPLAY, CMD_DISPLAY_WRITE, sizeof(cmd_msg), cmd_msg, cmd_packet);
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_create_packet failed.\r\n");
		ret = -EFAULT;
	}
	
	atomic_set(&cmd_reply_received_flag, 0);
	ret = cp430_core_write(CP430_DEV_DISPLAY, cmd_packet, sizeof(cmd_packet));
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_core_write failed.\r\n");
		ret = -EFAULT;
	}
	
	if (ret >= 0) {
		if(is_suspend)
			wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (HZ/2)); //{RD} Reduced time-out from 5sec to 500ms to allow suspend
		else			
			wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (5 * HZ));
			
		if( 1 == atomic_read(&cmd_reply_received_flag))
		{	
			PDEBUG("metec_flat20: command response received\r\n");
			if(!is_suspend)
				memset(dev->display_data, 0, sizeof(dev->display_data));
		}
		else
		{
			ret = -EFAULT;
			PDEBUG("metec_flat20: command response timed out\r\n");
		}
	}
	
	return ret;
}

static int 
write_to_braille(struct metec_flat20_dev_info *dev, unsigned char * user_data, int is_resume)
{
	int ret = 0;
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	unsigned char a = 0;
	unsigned char b = 0;
	unsigned char cmd_packet[MAX_BRAILLE_LINE_SIZE + METEC_FLAT20_PACKET_OVERHEAD + CP430_PACKET_OVERHEAD];
	unsigned char cmd_msg[MAX_BRAILLE_LINE_SIZE + METEC_FLAT20_PACKET_OVERHEAD];
	
	memset(cmd_packet, 0, sizeof(cmd_packet));
	memset(cmd_msg, 0, sizeof(cmd_msg));
	
	/* Prepare the packet for braille display */
	for (i = 0; i < MAX_BRAILLE_LINE_SIZE; i++) {
		if (0x00 != user_data[i]) {
			k = i / 4;
			for (j = 1; j <= 8; j++) {
				switch(j) {
					case 8: case 7: a = 5; break;	
					case 6: case 3: a = 11; break;	
					case 5: case 2: a = 17; break;
					case 4: case 1: a = 23; break;
					default: break;
				}
				switch(j) {	
					case 8: case 6: case 5: case 4:
						b = 1 + (i % 4) * 2; break;	
					case 7: case 3: case 2: case 1:
						b = 0 + (i % 4) * 2; break;
					default: break;
				}
				if (user_data[i] & (1 << (j - 1))) {
					cmd_msg[a-k] |= (1 << b);
				}
			}
		}
	}
	
	cmd_msg[METEC_FLAT20_PACKET_UOUT_INDEX] = dev->braille_dot_strength;
	
	ret = cp430_create_packet(CP430_DEV_DISPLAY, CMD_DISPLAY_WRITE, sizeof(cmd_msg), cmd_msg, cmd_packet);
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_create_packet failed.\r\n");
		ret = -EFAULT;
	}
	
	atomic_set(&cmd_reply_received_flag, 0);
	ret = cp430_core_write(CP430_DEV_DISPLAY, cmd_packet, sizeof(cmd_packet));
	if (ret < 0) {
		PDEBUG("metec_flat20: cp430_core_write failed.\r\n");
		ret = -EFAULT;
	}
	
	if (ret >= 0) {
		/* Wait for the command response */
		if(is_resume)
			wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (HZ/2));//{RD} Reduced time-out from 5sec to 500ms to allow resume
		else
			wait_event_timeout(command_response_received_wq, (atomic_read(&cmd_reply_received_flag) != 0), (5 * HZ));
			
		if( 1 == atomic_read(&cmd_reply_received_flag))
		{	
			PDEBUG("metec_flat20: command response received\r\n");
			
			if(!is_resume){ //store written value
				memset(dev->display_data, 0, sizeof(dev->display_data));
				memcpy(dev->display_data, user_data, sizeof(dev->display_data));
			}
		}
		else
		{
			ret = -EFAULT;
			PDEBUG("metec_flat20: command response timed out\r\n");
		}
	}
	
	return ret;
}

static int
metec_flat20_open(struct inode *inode, struct file *filp)
{
	struct metec_flat20_dev_info *dev;
	
	dev = container_of(inode->i_cdev, struct metec_flat20_dev_info, cdev);
	filp->private_data = dev;
	
	return 0;          
}

static int
metec_flat20_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t
metec_flat20_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	struct metec_flat20_dev_info *dev = NULL;
	
	dev = filp->private_data; 
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	up(&dev->sem);
	
	return retval;
}

static ssize_t
metec_flat20_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	unsigned char *buffer = NULL;
	struct metec_flat20_dev_info *dev = NULL;
	
	dev = filp->private_data;
		
	buffer = kmalloc(count, GFP_KERNEL);
	if(NULL == buffer)
		return -ENOMEM;
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (buffer) {
		int ret = 0;
		unsigned int max_char_count = 0;
		unsigned char user_data[MAX_BRAILLE_LINE_SIZE];
		
		memset(user_data, 0, sizeof(user_data));
		memset(buffer, 0, count);

		if (copy_from_user(buffer, user_buf, count)) {
			PDEBUG("metec_flat20: copy_from_user failed\r\n");
			goto out;
		}
		
		max_char_count = MIN(sizeof(user_data), count);
		memcpy(user_data, buffer, max_char_count);
				
		ret = write_to_braille(dev, user_data,0);
		if (ret < 0) {
			retval = -EFAULT;
			goto out;
		}
		
		*f_pos += max_char_count;
		retval = max_char_count;
	}
	
out:
	if (buffer) {
		kfree(buffer);
	}
	
	up(&dev->sem);
	
	return retval;
}

static long
metec_flat20_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	
	int err = 0;
	int ret = 0;
	struct metec_flat20_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != METEC_FLAT20_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > METEC_FLAT20_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;

	switch(cmd) {
		case METEC_FLAT20_GET_DRIVER_VERSION: {
			const char * version_string;
			
			version_string = DRIVER_VERSION;
			ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
			if (ret) 
				PDEBUG("metec_flat20: copy_to_user failed.\r\n");
		}
		break;
		case METEC_FLAT20_DISPLAY_CONTROL:
			switch(arg) {
				case DISPLAY_ENABLE:
					ret = control_display(DISPLAY_ENABLE);
					if (ret < 0)
						ret = -EFAULT;
					break;
				case DISPLAY_DISABLE:
					ret = control_display(DISPLAY_DISABLE);
					if (ret < 0)
						ret = -EFAULT;
					break;
				default:
					ret = -EINVAL;
					break;
			}
		break;
		case METEC_FLAT20_CLEAR_DISPLAY:
			ret = clear_display(dev, 0);
			if (ret < 0) 
				ret = -EFAULT;
		break;
		case METEC_FLAT20_DISPLAY_WRITE:
		{	
			unsigned char user_data[MAX_BRAILLE_LINE_SIZE];
			
			memset(user_data, 0, sizeof(user_data));
			
			ret = copy_from_user(user_data, (void*)arg, sizeof(user_data));
			if (ret) {
				PDEBUG("metec_flat20: copy_from_user failed.\r\n");
				ret = -EFAULT;
				break;
			}
			
			ret = write_to_braille(dev, user_data,0);
			if (ret < 0) {
				ret = -EFAULT;
			}
		}
		break;
		case METEC_FLAT20_SET_DOT_STRENGTH:
		{
			int ret = 0;
			
			PDEBUG("metec_flat20: dot_strength value: %02X\r\n", (unsigned char)arg);
			
			switch (arg) {
				case UOUT_155V_CONFIG_VALUE:
				case UOUT_162V_CONFIG_VALUE:
				case UOUT_168V_CONFIG_VALUE:
				case UOUT_174V_CONFIG_VALUE:
				case UOUT_177V_CONFIG_VALUE:
				case UOUT_184V_CONFIG_VALUE:
				case UOUT_191V_CONFIG_VALUE:
				case UOUT_199V_CONFIG_VALUE:
					dev->braille_dot_strength = (unsigned char)arg;
				break;
				default:
					PDEBUG("metec_flat20: Invalid value\r\n");
					ret = -EINVAL;	
				break;
			}
		}
		break;
		default:
			PDEBUG("metec_flat20: Invalid ioctl\r\n");
			ret = -ENOTTY;
		break;
	   }
	   
	up(&dev->sem);
	
	return ret;
}

static const struct file_operations metec_flat20_fops = {
	.owner 			= THIS_MODULE,
	.read 			= metec_flat20_read,
	.write 			= metec_flat20_write,
	.unlocked_ioctl	= metec_flat20_ioctl,
	.open 			= metec_flat20_open,
	.release 		= metec_flat20_release,
};

/************************** Platform Driver **************************/


static int 
metec_flat20_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	printk(KERN_INFO "metec_flat20: %s\r\n",__FUNCTION__);
	
//	clear_display(metec_flat20_dev,1);
	
	return ret;
}

static int 
metec_flat20_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "metec_flat20: %s\r\n",__FUNCTION__);
	
	//write_to_braille(metec_flat20_dev, metec_flat20_dev->display_data,1);
	
	return ret;
}

static struct platform_driver metec_flat20_platform_driver = {
	.suspend  = metec_flat20_platform_suspend,
	.resume	  = metec_flat20_platform_resume,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= METEC_FLAT20_PLATFORM_NAME,
	},
};

/************************ Early suspend functions *********************/
static void 
metec_flat20_early_suspend(struct early_suspend *h)
{	
	printk(KERN_INFO "metec_flat20: %s\r\n",__FUNCTION__);
	
//	clear_display(metec_flat20_dev,1);
	
	return;
}

static void 
metec_flat20_late_resume(struct early_suspend *h)
{	
	printk(KERN_INFO "metec_flat20: %s\r\n",__FUNCTION__);
	
	write_to_braille(metec_flat20_dev, metec_flat20_dev->display_data,1);
	
	return;
}

static struct early_suspend metec_flat20_early_suspend_handler = {
	.suspend = metec_flat20_early_suspend,
	.resume = metec_flat20_late_resume,
};

/************************** Device Init/Exit **************************/

static int
metec_flat20_add_sysfs(void)
{
	struct class *dev_class = NULL;
	
	dev_class = class_create(THIS_MODULE, METEC_FLAT20_DEV_CLASS_NAME);
	if (IS_ERR(dev_class)) {
		printk(KERN_INFO "metec_flat20: class_create failed\r\n");
		return -EFAULT;
	}
	
	if (!device_create(dev_class, NULL, metec_flat20_dev->cdev.dev, NULL, METEC_FLAT20_DEV_NAME)) {
		printk(KERN_INFO "metec_flat20: device_create failed\r\n");
		class_destroy(metec_flat20_dev->dev_class);
		return -EFAULT;
	}
	
	metec_flat20_dev->dev_class = dev_class;
	
	return 0;
}

static int 
metec_flat20_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(METEC_FLAT20_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	metec_flat20_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
metec_flat20_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(METEC_FLAT20_PROC_DIR, NULL);
	
	return ret;
}

static int
metec_flat20_init_cdev(struct metec_flat20_dev_info *dev_info, const struct file_operations *fops)
{	
	int ret = 0;
	dev_t dev = 0;
	
#ifdef METEC_FLAT20_MAJOR
dev_info->major = METEC_FLAT20_MAJOR;
#endif

#ifdef METEC_FLAT20_MINOR
dev_info->minor = METEC_FLAT20_MINOR;
#endif

	if(dev_info->major) {
		dev = MKDEV(dev_info->major, dev_info->minor);
		ret = register_chrdev_region(dev, METEC_FLAT20_NR_DEVS, METEC_FLAT20_DEV_NAME);
		if (ret < 0) {
			printk(KERN_INFO "metec_flat20: register_chrdev_region failed\r\n");
			goto err_chr_driver;
		}
	}
	else {
		dev_info->minor = 0;
		ret = alloc_chrdev_region(&dev, dev_info->minor, METEC_FLAT20_NR_DEVS, METEC_FLAT20_DEV_NAME);
		if (ret < 0) {
			printk(KERN_INFO "metec_flat20: alloc_chrdev_region failed\r\n");
			goto err_chr_driver;
		}
		dev_info->major = MAJOR(dev);
	}	
	
	cdev_init (&dev_info->cdev, fops);
	dev_info->cdev.owner = THIS_MODULE;
	dev_info->cdev.ops   = fops;
	ret = cdev_add (&dev_info->cdev, dev, 1);
	if (ret) {
		printk(KERN_INFO "metec_flat20: cdev_add failed\r\n");
		unregister_chrdev_region (dev,1);
	}
	
err_chr_driver:
	return ret;
}

static void
metec_flat20_init_variables(struct metec_flat20_dev_info *dev)
{
	dev->major = 0;
	dev->minor = 0;
	dev->braille_dot_strength = UOUT_177V_CONFIG_VALUE;
	dev->dev_class = NULL;
	dev->platform_dev = NULL;
	dev->proc_dir = NULL;
}

static int __init
metec_flat20_mod_init(void)
{
	int ret;
	dev_t dev = 0;
	
	printk(KERN_INFO "metec_flat20: %s\r\n",__FUNCTION__);
	
	metec_flat20_dev = (struct metec_flat20_dev_info *)kzalloc(METEC_FLAT20_NR_DEVS * sizeof(*metec_flat20_dev), GFP_KERNEL);
	if (NULL == metec_flat20_dev) {
		printk(KERN_INFO "metec_flat20: kmalloc failed\r\n");	
		ret = -ENOMEM;
		goto err_fail;
	}
	memset (metec_flat20_dev, 0, sizeof(METEC_FLAT20_NR_DEVS * sizeof(*metec_flat20_dev)));
	
	/* {PK} Initialize the device data structure */
	metec_flat20_init_variables(metec_flat20_dev);

	/* {PK} Register platform driver */
	ret = platform_driver_register(&metec_flat20_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "metec_flat20: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}

	/* {PK} Register platform device */
	metec_flat20_dev->platform_dev = platform_device_register_simple(METEC_FLAT20_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(metec_flat20_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_INFO "metec_flat20: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}

	/* {PK} Initialize the char device */
	ret = metec_flat20_init_cdev(metec_flat20_dev, &metec_flat20_fops);
	if (ret < 0) {
		goto err_chr_driver;
	}
	
		
	/*{KW}: early suspend */
	register_early_suspend(&metec_flat20_early_suspend_handler);

	sema_init(&metec_flat20_dev->sem, 1);
	
	/* {PK} Create the proc file */
	if (metec_flat20_create_proc() < 0) {
		printk(KERN_INFO "metec_flat20: metec_flat20_create_proc failed\r\n");
	}

	/* {PK} Create sysfs entry */
	if (metec_flat20_add_sysfs() < 0) {
		printk(KERN_INFO "metec_flat20: metec_flat20_add_sysfs failed\r\n");
	}
	
	/* {PK} Register device with the core driver */
	memset(&metec_flat20_bus_data, 0, sizeof(metec_flat20_bus_data));
	metec_flat20_bus_data.receive_event_handler = bus_receive_event_handler;
	metec_flat20_bus_data.transmit_event_handler = bus_transmit_event_handler;
	if (cp430_device_register(CP430_DEV_DISPLAY, &metec_flat20_bus_data) < 0) {
		printk(KERN_INFO "metec_flat20: cp430_device_register failed\r\n");
		goto err_bus_register;
	}
	else {
		printk(KERN_INFO "metec_flat20: cp430_device_register success\r\n");
	}
	
	clear_display(metec_flat20_dev, 0); /* {PK} Clear the display when driver is ready */

	printk("metec_flat20: Driver Version: %s\n", DRIVER_VERSION);
	
	return 0; /* {PK} success */
	
err_bus_register:
	device_destroy(metec_flat20_dev->dev_class, dev);
	class_destroy(metec_flat20_dev->dev_class);
	metec_flat20_remove_proc();
	cdev_del(&metec_flat20_dev->cdev);
	unregister_chrdev_region(dev,1);
err_chr_driver:
	platform_device_unregister(metec_flat20_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&metec_flat20_platform_driver);
err_platform_driver:
	kfree(metec_flat20_dev);
err_fail:
	return ret;
}

void __exit
metec_flat20_mod_exit(void)
{

	dev_t dev = 0;
	
	printk(KERN_INFO"metec_flat20: %s\r\n",__FUNCTION__);
	
	dev = MKDEV(metec_flat20_dev->major, metec_flat20_dev->minor);
	
	if (cp430_device_unregister(CP430_DEV_DISPLAY) < 0) {
		PDEBUG("metec_flat20: Ccp430_device_unregister failed\r\n");
	}
	else {
		PDEBUG("metec_flat20: cp430_device_unregister success\r\n");
	}

	/* {PK} Remove sysfs entry */
	device_destroy(metec_flat20_dev->dev_class, dev);
	class_destroy(metec_flat20_dev->dev_class);
	
	/* {PK} Remove procfs entry */
	if (metec_flat20_remove_proc() < 0) {
		printk(KERN_ERR "metec_flat20: metec_flat20_remove_proc failed\r\n");
	}
	
		
	/*{KW}: early suspend remove*/
	unregister_early_suspend(&metec_flat20_early_suspend_handler);
	
	/* {PK} Remove char device */
	cdev_del(&metec_flat20_dev->cdev);
	unregister_chrdev_region(dev, METEC_FLAT20_NR_DEVS);

	/* {PK} Remove platform device */
	platform_device_unregister(metec_flat20_dev->platform_dev);
	platform_driver_unregister(&metec_flat20_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(metec_flat20_dev);
	
}

module_param (proc_enable, int, S_IRUGO);

module_init(metec_flat20_mod_init);
module_exit(metec_flat20_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Braille display driver for Metec Braille-line Flat20");
MODULE_AUTHOR("Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
