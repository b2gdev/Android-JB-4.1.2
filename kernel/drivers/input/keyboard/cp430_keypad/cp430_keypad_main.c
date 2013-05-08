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
#include <linux/input.h>

/* {PK} Local includes */
#include "keypad.h"		
#include "debug.h"
#include "cp430.h"
#include <linux/delay.h> // {RD}

struct tcbin_keys {
	unsigned int main_keys;
	unsigned int cursor_keys_blk1;
	unsigned int cursor_keys_blk2;
};

static struct keypad_dev_info {
	struct input_dev *input;
	struct tcbin_keys last_key_pattern;
	struct tcbin_keys last_key_event;
	struct platform_device *pdev;
	struct proc_dir_entry *proc_dir;
} *keypad_dev;

static struct device_data keypad_bus_data;

int proc_enable = 1; /* {PK} Proc file. Enabled by default. Can be changed via the module param. used in dubug.h */

unsigned short key_combination  = 0; /*{KW}*/
unsigned short active_key_count = 0; /*{KW}*/

unsigned short masked_flag = SYS_NOFLAG; /*{KW}: separate flag from rx event arg */


static int keypad_process_key_input(struct tcbin_keys *key_pattern);

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
	
	if(masked_arg > 0) {
		unsigned char *buffer = kmalloc(masked_arg, GFP_KERNEL);
		
		if (buffer) {
			if (cp430_core_read(CP430_DEV_KEYPAD, buffer, masked_arg) < 0) {
				PDEBUG("keypad: cp430_core_read failed\r\n");
			}
			else {
				PDEBUG("keypad: packet received\r\n");
				if(CP430_DEV_KEYPAD == buffer[CP430_DEVICE]) {
					switch(buffer[CP430_COMMAND]) {
					case 0x01: {
//						if(0x00 == buffer[CP430_DATA]) {
							struct tcbin_keys key_pattern;
							memset(&key_pattern, 0, sizeof(key_pattern));
							memcpy(&key_pattern, &buffer[CP430_DATA], sizeof(key_pattern));
							keypad_process_key_input(&key_pattern);
						}
// 						else {
// 							PDEBUG("keypad: Error in Command Response %02X\r\n", buffer[CP430_DATA]);
// 						}
					break;
					default:
						PDEBUG("keypad: packet received, but unknown command\r\n");
					break;
					}
				}
				else {
					PDEBUG("keypad: packet received, but not for us\r\n");
				}
			}
			kfree(buffer);
		}
		else {
			PDEBUG("keypad: packet received, but could not read to the buffer\r\n");
		}
	}
	else {
		PDEBUG("keypad: receive packet length invalid\r\n");
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
			PDEBUG("keypad: tx idle event\r\n");
			break;
		}
		default:
		{
			break;
		}
	}
	
	return 0;
}

/* {PK} Key Pattern */

/*    Byte 0-3     | Byte 4-7           | Byte 8-11 
 *    (main_keys)  | (cursor_keys_blk1) | (cursor_keys_blk2)
 *    
 *    main_keys -> bit  0-7  -> 8 braille keys
 *		   bit  8    -> space bar key
 *		   bit  9-13 -> navigation keys
 *		   bit 14-15 -> next/prev keys
 *		   bit 16-17 -> volume up/down keys
 *		   bit 18    -> power key
 *
 *    cursor_keys_blk1 -> bit 0-19 -> cursor keys 
 */

inline int 
is_key_pressed(unsigned int key_pattern, unsigned int bit_pos)
{
	return !!(key_pattern & (unsigned int)(1 << bit_pos)); 
}

inline void
xor_bit(unsigned int *pnr, unsigned int bit_pos)
{
	*pnr ^= (unsigned int)(1 << bit_pos);
}

static int 
keypad_process_key_input(struct tcbin_keys *key_pattern)
{
	int i = 0;
	int ret = 0; 
	int group_key_pressed = 0;
	unsigned short scan_code = 0;
	unsigned short braille_chord = 0; 			/*{KW}: This was unsinged char */
	unsigned short last_braille_chord = 0; 		/*{KW}*/
	unsigned short latest_active_key_chord = 0;	/*{KW}*/	
	unsigned char is_key_down = 0;				/*{KW}*/
	
	struct input_dev *input = keypad_dev->input;
	struct tcbin_keys *last_key_event = &keypad_dev->last_key_event;
	struct tcbin_keys *last_key_pattern = &keypad_dev->last_key_pattern;
	
	unsigned int main_keys = key_pattern->main_keys;
	unsigned int last_main_keys_pattern = last_key_pattern->main_keys;
	
	unsigned int cursor_keys_blk1 = key_pattern->cursor_keys_blk1;
	unsigned int last_cursor_keys_blk1_pattern = keypad_dev->last_key_pattern.cursor_keys_blk1;

	/********** Main Keys **********/
	PDEBUG("{KW}: keyboard_driver: keypad_process_key_input() is called\n");
	
	/*{KW}: Send dummy scan code to wake the system up, on resumin from keypad condition */
	if(masked_flag == SYS_RESUMING){		
		scan_code = KEY_DUMMY_WAKEUP; /*{KW} defined in input.h */
		input_event(input, EV_KEY, scan_code, 1);
		input_event(input, EV_KEY, scan_code, 0);
	}
	
	/* {PK} POWER Key */
	if (is_key_pressed(last_key_event->main_keys, KEY_POWER_POS) ^ is_key_pressed(main_keys, KEY_POWER_POS)) {
		if ((!is_key_pressed(last_key_event->main_keys, KEY_POWER_POS)) & is_key_pressed(main_keys, KEY_POWER_POS)) {
			/*{KW}: Power key events are ignored here. They are handled by twl4030 via toggling PWRON*/
			//scan_code = misc_keys[KEY_POWER_POS];
			//input_event(input, EV_KEY, scan_code, 1);
			//input_event(input, EV_KEY, scan_code, 0);
			//printk("{KW}: power key %04x \n", scan_code);
			xor_bit(&last_key_event->main_keys, KEY_POWER_POS);
		}
		else if (is_key_pressed(last_key_event->main_keys, KEY_POWER_POS) & (!is_key_pressed(main_keys, KEY_POWER_POS))) {
			/*{KW}: Power key events are ignored here. They are handled by twl4030 via toggling PWRON*/
			//scan_code = misc_keys[KEY_POWER_POS];
			//input_event(input, EV_KEY, scan_code, 1);
			//input_event(input, EV_KEY, scan_code, 0);
			//printk("{KW}: power key %04x \n", scan_code);
			xor_bit(&last_key_event->main_keys, KEY_POWER_POS);
		}
	}

	/* {PK} Mutual-Ex Keys (KEY_VOL_DOWN, KEY_VOL_UP, KEY_PREV, KEY_NEXT) */
	for( i = KEY_VOL_DOWN_POS; i >= KEY_NEXT_POS; i--) {
		if (is_key_pressed(last_key_event->main_keys, i) ^ is_key_pressed(main_keys, i)) {
			scan_code = misc_keys[i];
			input_event(input, EV_KEY, scan_code, is_key_pressed(main_keys, i));
			
			xor_bit(&last_key_event->main_keys, i);
			//PDEBUG("keypad: event fired key: %d value: %d\r\n",scan_code, is_key_pressed(main_keys, i));
		}
		else
		{
			//PDEBUG("keypad: no event fired key: %d\r\n",i);
		}
	}

	/* {PK} Navigation Keys */
	group_key_pressed = 0;
	for (i = KEY_NAV_OK_POS ; i >= KEY_NAV_LEFT_POS; i--) {
		if (is_key_pressed(last_main_keys_pattern, i) ^ is_key_pressed(main_keys, i)) {
			if(!group_key_pressed) {
				if ((!is_key_pressed(last_main_keys_pattern, i)) & is_key_pressed(main_keys, i)) {
					scan_code = misc_keys[i];
					input_event(input, EV_KEY, scan_code, 1);
					xor_bit(&last_key_event->main_keys, i);
					group_key_pressed = 1;
					continue;
				}
			}
		}
		if (is_key_pressed(last_key_event->main_keys, i) ^ is_key_pressed(main_keys, i)) {
			scan_code = misc_keys[i];
			input_event(input, EV_KEY, scan_code, 0);
			xor_bit(&last_key_event->main_keys, i);
		}
	}
	last_key_pattern->main_keys = key_pattern->main_keys;

	
	/********** {KW} Braille Keys **********/
		
	braille_chord = main_keys & 0x1FF;
	
	last_braille_chord = last_key_event->main_keys & 0x1FF; 	
	last_key_event->main_keys = (last_key_event->main_keys & (~0x1FF)) | braille_chord;
	
	if(!last_braille_chord) { 													// this is the first key (safe step)
		key_combination = 0;
		active_key_count = 0;
	}
	
	latest_active_key_chord = last_braille_chord ^ braille_chord;	
	
	for (i = KEY_BRL_DOT1_POS; i <= KEY_BRL_DOT9_POS; i++)
	{	
		if(latest_active_key_chord & (1<<i)) 										//if i th bit is active
		{
			scan_code = individual_braille_dots[i];		
			is_key_down = ((1<<i) & braille_chord) ? 1 : 0; 						// active bit is a down?
			
			if(is_key_down) {
				input_event(input, EV_KEY, scan_code, 1);	
				//printk("{KW}: ind down %04x \n", scan_code);						// individual key down event		
				key_combination |= braille_chord; 									// add new key to the combination
				active_key_count ++;
				/*{KW} : PDEBUG("{DOWN}  main_keys=0x%08x, braille_chord=0x%04x , key_combination=0x%04x, latest_active_key_chord=0x%04x, active_key_count=0x%04x\n", 
								main_keys, braille_chord, key_combination, latest_active_key_chord, active_key_count); */
			}
			else{
				input_event(input, EV_KEY, scan_code, 0);							// individual key up event		
				//printk("{KW}: ind up %04x \n", scan_code);	
				active_key_count --;
				/*{KW}: PDEBUG("{UP}  main_keys=0x%08x, braille_chord=0x%04x , key_combination=0x%04x, latest_active_key_chord=0x%04x, active_key_count=0x%04x\n", 
								main_keys, braille_chord, key_combination, latest_active_key_chord, active_key_count); */
				if(active_key_count == 0){
					//send combined key events here					
					scan_code = braille_keys[key_combination]; 
					input_event(input, EV_KEY, scan_code, 1);						// combined key down event
					//printk("{KW}: comb down %04x \n", scan_code);	
					//input_sync(input);
					//msleep(3000); // {RD}
					input_event(input, EV_KEY, scan_code, 0);						// combined key up event
					//printk("{KW}: comb up %04x \n", scan_code);	
					/*{KW}: PDEBUG("\n{COMBINED}  key_combination=0x%08x\n\n", key_combination); */
				}
				
			}
		}
	}
	
	/********** {KW} Braille Keys End  **********/
	
	
	
	/********** Cursor Keys Block1 **********/
	i = 0;
	group_key_pressed = 0;
	for (i = KEY_CURSOR_19_POS ; i >= KEY_CURSOR_0_POS; i--) {
		if (is_key_pressed(last_cursor_keys_blk1_pattern, i) ^ is_key_pressed(cursor_keys_blk1, i)) {
			if(!group_key_pressed) {
				if ((!is_key_pressed(last_cursor_keys_blk1_pattern, i)) & is_key_pressed(cursor_keys_blk1, i)) {
					scan_code = cursor_keys[i];
					input_event(input, EV_KEY, scan_code, 1);
					xor_bit(&last_key_event->cursor_keys_blk1, i);
					group_key_pressed = 1;
					continue;
				}
			}
		}
		
		if (is_key_pressed(last_key_event->cursor_keys_blk1, i) ^ is_key_pressed(cursor_keys_blk1, i)) {
			scan_code = cursor_keys[i];
			input_event(input, EV_KEY, scan_code, 0);
			xor_bit(&last_key_event->cursor_keys_blk1, i);
		}
	}
	last_key_pattern->cursor_keys_blk1 = key_pattern->cursor_keys_blk1;

	return ret;
}

static int 
keypad_create_proc(void)
{	
	struct proc_dir_entry *proc_dir = NULL;
	
	proc_dir = proc_mkdir(KEYPAD_PROC_DIR, NULL);
	if (NULL == proc_dir) {
		return -EFAULT;
	}
	
	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	keypad_dev->proc_dir = proc_dir;
	
	return 0;
}

static int 
keypad_remove_proc(void)
{
	int ret = 0;
	
	ret = remove_proc_file(&debug_file);
	ret = remove_proc_file(&error_file);
	remove_proc_entry(KEYPAD_PROC_DIR, NULL);
	
	return ret;
}

static int 
keypad_platform_probe(struct platform_device *device)
{
	int ret = 0;
	
	struct input_dev *input_dev;
	
	printk(KERN_INFO "keypad: %s\r\n",__FUNCTION__);

	keypad_dev = kzalloc(sizeof(*keypad_dev), GFP_KERNEL);
	if (NULL == keypad_dev) {
		printk(KERN_INFO "keypad: kzalloc failed \r\n");
		ret = -ENOMEM;
		goto err_fail;
	}
	
	/* {PK} Allocate memory for the keypad input device */
	input_dev = input_allocate_device();
	if (NULL == input_dev) {
		printk(KERN_INFO "keypad: input_allocate_device failed \r\n");
		ret = -ENOMEM;
		goto err_input_dev;
	}
	
	input_dev->name = device->name;
	input_dev->phys = "keypad/input0";
	input_dev->dev.parent = &device->dev;
	
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor  = VENDOR_ID;
	input_dev->id.product = PRODUCT_ID;
	input_dev->id.version = PRODUCT_VERSION;

	set_bit(EV_KEY, input_dev->evbit); /* {PK} Generating key events */
	//set_bit(EV_REP, input_dev->evbit); /* {PK} Software AutoRepeat enabled */
	//set_bit(EV_PWR, input_dev->evbit); 
		
	/* indicate that we generate *any* key event */
	bitmap_fill(input_dev->keybit, KEY_MAX);
	
	keypad_dev->input = input_dev;
	platform_set_drvdata(device, keypad_dev);
	input_set_drvdata(input_dev, keypad_dev);
	
	ret = input_register_device(input_dev);
	if (NULL == input_dev) {
		printk(KERN_INFO "keypad: input_register_device failed \r\n");
		ret = -ENOMEM;
		goto err_input_register_device;
	}
	
	/* {PK} Register device with the bus driver */
	memset(&keypad_bus_data, 0, sizeof(keypad_bus_data));
	keypad_bus_data.receive_event_handler = bus_receive_event_handler;
	keypad_bus_data.transmit_event_handler = bus_transmit_event_handler;
	if (cp430_device_register(CP430_DEV_KEYPAD, &keypad_bus_data) < 0) {
		printk(KERN_INFO "keypad: cp430_device_register failed\r\n");
		goto err_cp430_device_register;
	}
	else {
		printk(KERN_INFO "keypad: cp430_device_register success\r\n");
	}
		
	/* {PK} Create the proc file */
	if (keypad_create_proc() < 0) {
		printk(KERN_INFO "keypad: keypad_create_proc failed\r\n");
	}
	
	return 0;
	
err_cp430_device_register:
	input_unregister_device(input_dev);
err_input_register_device:
	input_free_device(input_dev);
err_input_dev:
	kfree(keypad_dev);
err_fail:

	return ret;
}

static int 
keypad_platform_remove(struct platform_device *device)
{
	int ret = 0;
	struct keypad_dev_info *kdev;
	
	printk(KERN_INFO "keypad: %s\n",__FUNCTION__);
	
	/* {PK} Remove procfs entry */
	if (keypad_remove_proc() < 0) {
		printk(KERN_ERR "keypad: keypad_remove_proc failed\r\n");
	}
	
	if (cp430_device_unregister(CP430_DEV_KEYPAD) < 0) {
		PDEBUG("keypad: cp430_device_unregister failed\r\n");
	}
	else {
		PDEBUG("keypad: cp430_device_unregister success\r\n");
	}
	
	kdev = platform_get_drvdata(device);
	if (kdev) {
		input_unregister_device(kdev->input);
		kfree(kdev);
	}
	
	return ret;
}

static struct platform_driver keypad_platform_driver = {
	.probe	  = keypad_platform_probe,
	.remove   = keypad_platform_remove,
	.driver	= {
		.owner  = THIS_MODULE,
		.name	= KEYPAD_PLATFORM_NAME,
	},
};

static int __init
keypad_mod_init(void)
{
	int ret;
	
	printk(KERN_INFO "keypad: %s\r\n",__FUNCTION__);
	
	/* {PK} Register platform driver */
	ret = platform_driver_register(&keypad_platform_driver);
	if( ret ) {
		ret = -ENODEV;
		printk(KERN_INFO "keypad: platform_driver_register failed\r\n");
	}
	
	return ret;
}
	
	
void __exit
keypad_mod_exit(void)
{
	printk(KERN_INFO "keypad: %s\n",__FUNCTION__);

	platform_driver_unregister(&keypad_platform_driver); 
}

module_param (proc_enable, int, S_IRUGO);

module_init(keypad_mod_init);
module_exit(keypad_mod_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Driver for Keypad for TCBIN");
MODULE_AUTHOR("Pubudu Karunaratna <pubuduk@zone24x7.com>");
MODULE_VERSION(DRIVER_VERSION);
