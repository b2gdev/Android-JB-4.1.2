#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>       
#include <linux/slab.h>        
#include <linux/fs.h>           
#include <linux/errno.h>        
#include <linux/types.h>        
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/fcntl.h>        
#include <asm/system.h>         
#include <asm/uaccess.h>        
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/i2c/twl.h>	

#include "twl3040_vib.h"
#include "twl3040_vib_ioctl.h"
#include "debug.h"

#include <linux/timer.h>
//#include <linux/kthread.h>
#include <linux/workqueue.h>

static struct twl3040_vibrator_dev_info *vibrator_dev;
static struct timer_list on_timeout_timer;
static struct workqueue_struct *off_wq;
//static struct task_struct * off_task;

int proc_enable = 1;
int g_atomic_counter =0;

/* PWM values of the desired vibrator levels */
static unsigned char vibrator_levels[] = { 0xC0, 
					   0x81, 
					   0x41, 
					   0x01};


static int 
vibrator_setup(void)
{
	int ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	
	ret = twl_i2c_write_u8(TWL4030_MODULE_LED, 0x00, REG_TWL_LEDEN);		/* dissable LED functions*/
	if (ret < 0) 
		goto err_led;
	
	ret = twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x02, REG_TWL_CODEC_MODE);	/* switch-on audio codec subsystem power */
	if (ret < 0)
		goto err_codec;
	
	ret = twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x00, REG_TWL_VIBRA_CTRL);	/* vibrator control default values */
	if (ret < 0)
		goto err_ctrl;
	
	ret = twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x80, REG_TWL_VIBRA_SET); 	/* AVG_VAL is set to middle of the range */
	if (ret < 0)
		goto err_set;
	
	return 0;
		
err_set:
err_ctrl:
err_codec:
	twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x00, REG_TWL_CODEC_MODE);
err_led:
	PDEBUG("Error setting up the vibrator\r\n");

	return ret;
}

/* 
 *  vibrator_on() - vibratior switch-on
 *  @direction:	Direction of the vibrator rotation {0 = positive ; 1 = negative}
 */
static int 
vibrator_on(unsigned char direction)
{
	char vibra_ctrl_val = 0;
	int ret = 0;
	
	//PDEBUG("%s\r\n",__FUNCTION__);
	
	if(vibrator_dev->is_on)
		return 0;
		
	if (direction > 1) 
		return -EINVAL;
		
	//printk("%s\r\n",__FUNCTION__);
		
	vibra_ctrl_val = (direction << 1) | 0x01;
	
	ret = twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, vibra_ctrl_val , REG_TWL_VIBRA_CTRL);
	
	if (!ret)
		vibrator_dev->is_on=1;
		
	return ret;
}

/* 
 *  vibrator_set_level() - sets vibraton level. controls the PWM duty cycle to the vibrator H-Bridge
 *  @level:	level of the vibration {0 to 3 - 0 is the lightest}
 */
static int 
vibrator_set_level(unsigned char level)
{
	PDEBUG("%s\r\n",__FUNCTION__);
	
	if (level >= VIBRA_LEVEL_COUNT) 
		return -EINVAL;
	
	return twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, vibrator_levels[level] , REG_TWL_VIBRA_SET);
}

static int 
vibrator_off(void)
{
	int ret = 0;
	//PDEBUG("%s\r\n",__FUNCTION__);		
	
	if(!vibrator_dev->is_on)
		return 0;
		
	//printk("%s\r\n",__FUNCTION__);	
	ret = twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x0, REG_TWL_VIBRA_CTRL);
	
	if (!ret)
		vibrator_dev->is_on=0;	
	
	return ret;
}

static int 
vibrator_on_ioctl(unsigned char direction){
	
	int ret = 0;
	
	//printk("%s\r\n",__FUNCTION__);
	
	if (down_interruptible(&vibrator_dev->sem_is_on))
		return -ERESTARTSYS;
		
	if(timer_pending(&on_timeout_timer)){
		del_timer(&on_timeout_timer);
		g_atomic_counter = 0;
	}	
					
	ret = vibrator_on(direction);
	
	up(&vibrator_dev->sem_is_on);
	
	return ret;
}

static int 
vibrator_off_ioctl(void){
	
	int ret = 0;
	
	//printk("%s\r\n",__FUNCTION__);
	
	if (down_interruptible(&vibrator_dev->sem_is_on))
		return -ERESTARTSYS;
		
	if(timer_pending(&on_timeout_timer)){
		del_timer(&on_timeout_timer);
	}	
					
	ret = vibrator_off();
	g_atomic_counter=0;
	
	up(&vibrator_dev->sem_is_on);
	
	return ret;
}

static void off_wq_function( struct work_struct *work)
{
	if (down_interruptible(&vibrator_dev->sem_is_on)){
		printk(KERN_ERR "%s down_interruptible FAILED!\r\n",__FUNCTION__);
		kfree( (void *)work );
		vibrator_off();
		g_atomic_counter=0;
		return;
	}

	if(g_atomic_counter == 1){
		vibrator_off();
		g_atomic_counter = 0;
	}
	else if(g_atomic_counter != 0){
		g_atomic_counter--;
		printk(KERN_INFO "{RD} %s: Atomic counter catch!\r\n",__FUNCTION__);
	}
		
	kfree( (void *)work );					
	up(&vibrator_dev->sem_is_on);	
}

void off_callback( unsigned long data )
{
	struct work_struct *wkStruct;
	
	wkStruct = (struct work_struct *)kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    
    if (wkStruct) {

      INIT_WORK( wkStruct, off_wq_function );

      queue_work( off_wq, wkStruct );           

    }else{
		printk(KERN_ERR "%s KMALLOC FAILED!\r\n",__FUNCTION__);
	}
	
	return;
}

static int 
vibrator_on_with_timeout(unsigned long timeout)
{
	int ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	//printk("%s =%ld\r\n",__FUNCTION__,timeout);
		
	if (down_interruptible(&vibrator_dev->sem_is_on))
		return -ERESTARTSYS;
	
	if(timer_pending(&on_timeout_timer)){
		del_timer(&on_timeout_timer);
		if(g_atomic_counter)
			g_atomic_counter--;
	}
						
	ret =  vibrator_on(DIR_FORWARD);
	g_atomic_counter++;
	
	if(ret < 0)
		goto err_twl_write;


	if(timeout > 5){
		
		setup_timer( &on_timeout_timer, off_callback, 0 );
		ret = mod_timer( &on_timeout_timer, jiffies + msecs_to_jiffies(timeout) );		
		if(ret){ 
			printk(KERN_ERR "%s: Error in mod_timer\r\n",__FUNCTION__);
			vibrator_off();
			g_atomic_counter = 0;
		}
						
	}else{
		mdelay(timeout);
		vibrator_off();
		g_atomic_counter = 0;
	}

err_twl_write:
	up(&vibrator_dev->sem_is_on);	
	return ret;
}

int 
twl3040_vibrator_open(struct inode *inode , struct file *filp )
{
	PDEBUG("%s\r\n",__FUNCTION__);
	
	filp->private_data = vibrator_dev;
		   
	return  0;
}

static int 
twl3040_vibrator_release(struct inode *inode, struct file *filp)
{
	PDEBUG("%s\r\n",__FUNCTION__);

	return 0;
}

static long 
twl3040_vibrator_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	
	int ret = 0;
	int err = 0;
	struct twl3040_vibrator_dev_info *dev = NULL;
	
	dev = filp->private_data;
	
	PDEBUG("%s\n",__FUNCTION__);
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if (_IOC_TYPE(cmd) != TWL3040_VIBRATOR_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > TWL3040_VIBRATOR_IOC_MAXNR) return -ENOTTY;
	
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;
	
	switch(cmd){
		case TWL3040_VIBRATOR_GET_DRIVER_VERSION: {
			const char * version_string;
			
			version_string = DRIVER_VERSION;
			ret = copy_to_user((void *)arg, (const void *)version_string, strlen(version_string) + 1) ? -EFAULT : 0;
			if (ret) 
				PDEBUG("copy_to_user failed\r\n");
		}
		break;
		case TWL3040_VIBRATOR_SET_LEVEL: 			
			switch(arg) {
				case PWM_LEVEL_0:
				case PWM_LEVEL_1: 
				case PWM_LEVEL_2: 
				case PWM_LEVEL_3: 
					ret = vibrator_set_level(arg & 0xFF);
					break;
				default:
					ret = -EINVAL;
					break;
			}
		break;
		case TWL3040_VIBRATOR_ON:
			arg = arg & 0xFF;
			switch(arg){
				case DIR_FORWARD:
				case DIR_BACKWARD:
					ret = vibrator_on_ioctl(arg);
					break;
				default:
					ret = -EINVAL;
					break;
			}
		break;
		case TWL3040_VIBRATOR_ON_WITH_TIMEOUT:
			if(arg > 0)
					ret = vibrator_on_with_timeout(arg);
			else
					ret = -EINVAL;						
		break;
		case TWL3040_VIBRATORL_OFF:			
			ret = vibrator_off_ioctl();
		break;
		default:
			ret = -ENOTTY;
	}
	
	up(&dev->sem);
	
	return ret;
}

static ssize_t 
twl3040_vibrator_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
	
	ssize_t ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Reading is not permitted */
	
	return ret;
}

static ssize_t 
twl3040_vibrator_write( struct file *filp, const char __user *buf,size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	
	ret = -EPERM; /* {PK}: Writting is not permitted */
	
	return ret;
}

/************************** Platform Driver **************************/

static int 
twl3040_vibrator_platform_probe(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "%s\r\n",__FUNCTION__);
	
	ret = vibrator_setup();
	
	return ret;
}

static int 
twl3040_vibrator_platform_remove(struct platform_device *device)
{
	int ret = 0;
	
	printk(KERN_INFO "%s\r\n",__FUNCTION__);
	
	twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x00, REG_TWL_CODEC_MODE);
	twl_i2c_write_u8(TWL_MODULE_AUDIO_VOICE, 0x00, REG_TWL_VIBRA_CTRL);
	
	return ret;
}

static int 
twl3040_vibrator_platform_suspend(struct platform_device *device, pm_message_t state)
{
	int ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	printk(KERN_INFO "%s\r\n",__FUNCTION__);
	
	if (down_interruptible(&vibrator_dev->sem_is_on))
		return -ERESTARTSYS;
		
	if(timer_pending(&on_timeout_timer))
		del_timer(&on_timeout_timer);	
					
	vibrator_off();
	g_atomic_counter = 0;
	
	up(&vibrator_dev->sem_is_on);
		
	return ret;
}

static int 
twl3040_vibrator_platform_resume(struct platform_device *device)
{
	int ret = 0;
	
	PDEBUG("%s\r\n",__FUNCTION__);
	printk(KERN_INFO "%s\r\n",__FUNCTION__);
	
	return ret;
}

static struct platform_driver twl3040_vibrator_platform_driver = {
	.probe    = twl3040_vibrator_platform_probe,
	.remove   = twl3040_vibrator_platform_remove,
	.suspend  = twl3040_vibrator_platform_suspend,
	.resume	  = twl3040_vibrator_platform_resume,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= TWL3040_VIBRATOR_PLATFORM_NAME,
	},
};

/************************** Device Init/Exit **************************/

struct file_operations twl3040_vibrator_fops = {
	.owner   		= THIS_MODULE,
	.read    		= twl3040_vibrator_read,
	.write   		= twl3040_vibrator_write,
	.open    		= twl3040_vibrator_open,
	.release 		= twl3040_vibrator_release,
	.unlocked_ioctl	= twl3040_vibrator_ioctl,
};

static struct miscdevice twl3040_vibrator_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = TWL3040_VIBRATOR_DEV_NAME,
	.fops  = &twl3040_vibrator_fops,
};

static int
twl3040_vibrator_add_sysfs(struct twl3040_vibrator_dev_info *dev_info)
{
	int ret  = 0;
	
#ifdef TWL3040_VIBRATOR_MINOR
	twl3040_vibrator_miscdevice.minor = TWL3040_VIBRATOR_MINOR;
#endif

	ret = misc_register(&twl3040_vibrator_miscdevice);
	if (ret < 0 ) {
		printk(KERN_ERR "twl3040_vibrator: misc_register failed\r\n");
		return ret;
	}
	
	dev_info->major = MISC_MAJOR;
	dev_info->minor = twl3040_vibrator_miscdevice.minor;
	
	return ret;
}

static int
twl3040_vibrator_remove_sysfs(void)
{
	int ret = 0;
	
	ret = misc_deregister(&twl3040_vibrator_miscdevice);
	if (ret < 0)
		printk(KERN_ERR "twl3040_vibrator: misc_deregister failed\r\n");
	
	return ret;
}

static int 
twl3040_vibrator_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(TWL3040_VIBRATOR_DEV_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	vibrator_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
twl3040_vibrator_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(TWL3040_VIBRATOR_DEV_PROC_DIR, NULL);
	
	return ret;
}

static void
twl3040_vibrator_init_variables(struct twl3040_vibrator_dev_info *dev)
{	
	dev->is_on = 0;
	g_atomic_counter = 0;		
}

int __init 
twl3040_vibrator_init(void)
{
	int ret = 0;
    
	printk(KERN_INFO "twl3040_vibrator: %s\r\n",__FUNCTION__);
	
	vibrator_dev = (struct twl3040_vibrator_dev_info *)kzalloc(sizeof(*vibrator_dev), GFP_KERNEL);
	if (NULL == vibrator_dev) {
		ret = -ENOMEM;
		goto err_fail;
	}	
	
	/* {PK} Register platform driver */
	ret = platform_driver_register(&twl3040_vibrator_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_ERR "twl3040_vibrator: platform_driver_register failed\r\n");
		goto err_platform_driver;
	}
	
	/* {PK} Register platform device */
	vibrator_dev->platform_dev = platform_device_register_simple(TWL3040_VIBRATOR_PLATFORM_NAME, 0, NULL, 0);
	if ( IS_ERR(vibrator_dev->platform_dev) ) {
		ret = -ENODEV;
		printk(KERN_ERR "twl3040_vibrator: platform_device_register_simple failed\r\n");
		goto err_platform_device;
	}
	
	/* {PK} Create sysfs device and register misc device */
	if (twl3040_vibrator_add_sysfs(vibrator_dev) < 0) {
		goto err_sysfs;
	}
	
	sema_init(&vibrator_dev->sem, 1);
	sema_init(&vibrator_dev->sem_is_on, 1);
	
	/* {PK} Create the proc file */
	if (twl3040_vibrator_create_proc() < 0) {
		printk(KERN_ERR "twl3040_vibrator: twl3040_vibrator_create_proc failed\r\n");
	}
	
	twl3040_vibrator_init_variables(vibrator_dev);
	
	printk(KERN_INFO "twl3040_vibrator: Driver Version: %s\n", DRIVER_VERSION);
   
	off_wq = create_workqueue("off_queue");
	
	if(off_wq)
		return 0;
		
    printk(KERN_ERR "twl3040_vibrator: create work queue failed\r\n");
    ret = -1;
    
err_sysfs:
	platform_device_unregister(vibrator_dev->platform_dev);
err_platform_device:
	platform_driver_unregister(&twl3040_vibrator_platform_driver);
err_platform_driver:
	kfree(vibrator_dev);
err_fail:

	return ret;
}

void __exit 
twl3040_vibrator_exit(void)
{
	printk (KERN_INFO "twl3040_vibrator: %s\n",__FUNCTION__);
	
	flush_workqueue( off_wq );

	destroy_workqueue( off_wq );
  
	/* {PK} Remove procfs entry */
	if (twl3040_vibrator_remove_proc() < 0) {
		printk(KERN_ERR "twl3040_vibrator: twl3040_vibrator_remove_proc failed\r\n");
	}
	
	/* {PK} Remove sysfs entry and deregister misc device */
	twl3040_vibrator_remove_sysfs();
	
	/* {PK} Remove platform device */
	platform_device_unregister(vibrator_dev->platform_dev);
	platform_driver_unregister(&twl3040_vibrator_platform_driver);
	
	/* {PK} Free allocated data structures */
	kfree(vibrator_dev);;
}

module_init(twl3040_vibrator_init);
module_exit(twl3040_vibrator_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Driver for twl3040 vibrator driver");
MODULE_AUTHOR("Uditha Kasthuriarachchi <udithak@zore24x7.com> and Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
