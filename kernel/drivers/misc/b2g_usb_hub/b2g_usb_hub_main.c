#include <linux/init.h>
#include <linux/gpio.h>
	
static int usb_hub_init(void)
{
	gpio_request(21,  "USB_PWR_EN");
	gpio_direction_output(21, 0);
	gpio_set_value(21, 1);			/* USB_PWR_EN		- HIGH	- Turn on USB Hub power supply  */
	return 0;
}

late_initcall(usb_hub_init);
