
// #include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/sched.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <asm/atomic.h>     /* {KW} */

#include <linux/interrupt.h>
#include <mach/gpio.h>

#include "cp430_core.h"		/* local definitions */
#include "cp430_ioctl.h"
#include "debug.h"
#include "cp430.h"	        /* {KW} */


MODULE_AUTHOR("Prasad Samaraweera");
MODULE_LICENSE("Dual BSD/GPL");


/* module parameters */
int 		proc_enable     = 1;				/* enable proc files */
module_param(proc_enable, int, S_IRUGO);
static int 	use_rx_wq = 1;				/* mode 0 - use tasklet, mode 1 - use workqueue */
module_param(use_rx_wq, int, S_IRUGO);
static int 	use_tx_wq = 1;				/* mode 0 - use tasklet, mode 1 - use workqueue */
module_param(use_tx_wq, int, S_IRUGO);

struct cp430_core_device	*core_device;				/* allocated in cp430_core_init_module */
struct cp430_device 		*devices;

int active_rx_device;
int active_tx_device;
enum rx_states rx_state;
unsigned char active_rx_packet_head[6];
unsigned short active_rx_packet_length;
unsigned short active_rx_data_count;
unsigned char active_rx_packet_checksum;
/* packet logging related */
static int rx_logging_count = 0;
static int tx_logging_count = 0;

/* cp430 device */
static DECLARE_WAIT_QUEUE_HEAD(command_response_received_wq);
static int command_response_received_flag = 0;

/* {KW}: Maintain suspend resume status, Initiallized to suspend */
static atomic_t is_sys_suspend = ATOMIC_INIT(1);
/* {KW}: RX event handler Flag to be sent */
static unsigned int rx_event_handler_flag = SYS_NOFLAG;

unsigned short masked_flag = SYS_NOFLAG; /*{KW}: separate flag from rx event arg */
/** {KW}:
 * Called by cp430_core driver on data reception for this device.
 * @arg: 32bit value containing 2 fields as follows.
 * -----------------------------------------
 * |   16bit FLAG      | 16bit data length |
 * -----------------------------------------
 * FLAG could be one of: SYS_NOFLAG   (0x0000)
 * 						 SYS_RESUMING (0x0001) 
 **/

int cp430_dev_receive_event_handler(unsigned int arg)
{	
	unsigned int masked_arg = arg & 0x0000FFFF; /*{KW}: ignore the MSB 16bit FLAG */
	
	unsigned char* buffer = kmalloc(1024, GFP_KERNEL);
	
	masked_flag = (arg & 0xFFFF0000) >> 16;
	
	if (buffer != NULL) {
		if (cp430_core_read(CP430_CORE, buffer, masked_arg) < 0) {
			PDEBUG("cp430_dev : receive data fail\r\n");
		}
		else {
			PDEBUG("cp430_dev : packet received\r\n");
			
			if(buffer[6] != 0x00) {
				PDEBUG("Error in Command Response %02X\r\n", buffer[6]);
				command_response_received_flag = -1;
				wake_up(&command_response_received_wq);
			} 
			else {
				command_response_received_flag = 1;
				wake_up(&command_response_received_wq);
			}
		}

		kfree(buffer);
	}
	else {
		PDEBUG("cp430_dev : packet received, but could not read to the buffer\r\n");
	}

	return 0;
}

int cp430_dev_transmit_event_handler (unsigned int arg)
{
	switch (arg)
	{
		case TX_EVENT_IDLE:
		{
			PDEBUG("cp430_dev : tx idle event\r\n");
			break;
		}

		default:
		{
			break;
		}
	}

	return 0;
}

struct device_data cp430_dev_data;

int is_valid_device(unsigned int id)
{
	return (0 < id && id < CP430_DEVICE_COUNT) ? 1 : 0;
}

int cp430_device_register(unsigned int id, struct device_data* data)
{
	int ret = -EFAULT;

	if (is_valid_device(id)) {
		if(devices[id].id == 0) {
			devices[id].id = id;
			devices[id].data = data;
			PDEBUG("devices[id].id = %d, devices[id].data = %08x\r\n", id, (int)data);
			ret = 0;
		}
	}

	return ret;
}
EXPORT_SYMBOL(cp430_device_register);

int cp430_device_unregister(unsigned int id)
{
	int ret = -EFAULT;

	if (is_valid_device(id)) {
		if(devices[id].id != 0) {
			devices[id].id = 0;
			devices[id].data = NULL;
			ret = 0;
		}
	}

	return ret;	
}
EXPORT_SYMBOL(cp430_device_unregister);

int cp430_core_read(unsigned int id, unsigned char* buffer, unsigned int size)
{
	int retval = 0;

	PDEBUG("> : %s\r\n", __FUNCTION__);

	if (is_valid_device(id)) {
		if (down_interruptible(&devices[id].rx_sema)) {
			return -EINTR;
		}

		if (size > 0) {
			retval = kfifo_out_spinlocked(&devices[id].rx_fifo, buffer, size, &devices[id].rx_lock);
		}
		else {
			// invalid size
			retval = -EINVAL;
		}
	
		up(&devices[id].rx_sema);
	}
	else {
		retval = -EFAULT;
	}

	PDEBUG("< : %s, retval = %d\r\n", __FUNCTION__, retval);

	return retval;
}
EXPORT_SYMBOL(cp430_core_read);

int cp430_core_write(unsigned int id, const unsigned char* buffer, unsigned int size)
{
	int retval = 0;

	PDEBUG("> : %s\r\n", __FUNCTION__);

	if (is_valid_device(id)){
		if (down_interruptible(&devices[id].tx_sema)){
			return -EINTR;
		}
			
		if ((kfifo_len(&devices[id].tx_fifo) == 0) && (atomic_read(&devices[id].tx_state) == TX_STATE_IDLE)){ 
			if (size <= tx_fifo_sizes[id]){
				kfifo_reset(&devices[id].tx_fifo);
				kfifo_in_spinlocked(&devices[id].tx_fifo, (unsigned char*)buffer, size, &devices[id].tx_lock);
	
				if (use_tx_wq) {
					queue_work(core_device->tx_wq, &core_device->tx_work);
				}
				else{
					/* schedule tasklet */
				}
			}			
			else{
				// error - large packet
				retval = -E2BIG;
			}
		}
		else{
			// error - previous packet is still in the tx_fifo
			PERROR("error - previous packet of device[%d]is still in the tx_fifo, tx_fifo len = %d\r\n", id, kfifo_len(&devices[id].tx_fifo)); 
			retval = -EBUSY;
		}
			
		up(&devices[id].tx_sema);
	}
	else {
		retval = -EFAULT;
	}
	
	PDEBUG("< : %s, retval = %d\r\n", __FUNCTION__, retval);

	return retval;
}
EXPORT_SYMBOL(cp430_core_write);

int get_rx_fifo_len (unsigned int id)
{
	int ret = 0;

	if (is_valid_device(id)) {
		if (down_interruptible(&devices[id].rx_sema)) {
			return -EINTR;
		}

		ret = kfifo_len(&devices[id].rx_fifo);

		up(&devices[id].rx_sema);
	}
	else {
		ret = -EINVAL;
	}
	
	return ret;	
}

int get_tx_fifo_len (unsigned int id)
{
	int ret = 0;

	if (is_valid_device(id)) {
		if (down_interruptible(&devices[id].tx_sema)) {
			return -EINTR;
		}

		ret = kfifo_len(&devices[id].tx_fifo);

		up(&devices[id].tx_sema);
	}
	else {
		ret = -EINVAL;
	}
	
	return ret;			
}

int flush_rx_fifo (unsigned int id)
{
	int ret = 0;

	if (is_valid_device(id)) {
		if (down_interruptible(&devices[id].rx_sema)) {
			return -EINTR;
		}

		kfifo_reset(&devices[id].rx_fifo);

		up(&devices[id].rx_sema);
	}
	else {
		ret = -EINVAL;
	}
	
	return ret;	
}

int flush_tx_fifo (unsigned int id)
{
	int ret = 0;

	if (is_valid_device(id)) {
		if (down_interruptible(&devices[id].tx_sema)) {
			return -EINTR;
		}

		kfifo_reset(&devices[id].tx_fifo);

		up(&devices[id].tx_sema);
	}
	else {
		ret = -EINVAL;
	}
	
	return ret;	
}

struct core_operations cp430_core_ops = {
	.get_rx_fifo_len	= get_rx_fifo_len,
	.get_tx_fifo_len	= get_tx_fifo_len,
	.flush_rx_fifo		= flush_rx_fifo,
	.flush_tx_fifo		= flush_tx_fifo,
};
EXPORT_SYMBOL(cp430_core_ops);

void process_rx_fifo(unsigned long unused)
{
	unsigned char c = 0x00;
	int retval = 0;
	unsigned int rx_arg;

	PDEBUG("> : %s\r\n", __FUNCTION__);
    PDEBUG("kfifo_len(&core_device->rx_fifo) = %d\n", kfifo_len(&core_device->rx_fifo));
	
	while (kfifo_len(&core_device->rx_fifo) > 0) {
		retval = kfifo_out_spinlocked(&core_device->rx_fifo, &c, 1, &core_device->rx_lock);

		PDEBUG("rx byte = %02x\n", c);
		
		if (rx_state != RX_STATE_HEADER1 && rx_state != RX_STATE_HEADER2 && rx_state != RX_STATE_DEVICE) {
			if (is_valid_device(active_rx_device)) { // this check is done to make sure to avoid any problems due to invalid device number received
				if (devices[active_rx_device].rx_logging == PACKET_LOGGING_PARTIAL) {
					if (rx_logging_count < 16) {
						PDEBUG_PACKET("%02x,", c);
						rx_logging_count ++;
					}
					else if (rx_logging_count == 16) {
						PDEBUG_PACKET(".....");
						rx_logging_count ++;
					}
				}
	
				if (devices[active_rx_device].rx_logging == PACKET_LOGGING_FULL) {
					PDEBUG_PACKET("%02x,", c);
				}
			}
		}

		switch(rx_state) {
			case RX_STATE_HEADER1 : {
				if (0x43 == c) {
					// PDEBUG("c = %02x, rx_state = RX_STATE_HEADER1, next rx_state = RX_STATE_HEADER2\n", c);
					rx_state = RX_STATE_HEADER2;
				}
				else {
					// out of sync
					// PDEBUG("c = %02x, rx_state = RX_STATE_HEADER1, next rx_state = RX_STATE_HEADER1\n", c);
					rx_state = RX_STATE_HEADER1;
				}		
				
				break;
			}

			case RX_STATE_HEADER2 : {
				if (0x50 == c) {
					PDEBUG("c = %02x, rx_state = RX_STATE_HEADER2, next rx_state = RX_STATE_DEVICE\n", c);
					rx_state = RX_STATE_DEVICE;
				}
				else if (0x43 == c) {
					PDEBUG("c = %02x, rx_state = RX_STATE_HEADER2, next rx_state = RX_STATE_HEADER2\n", c);
					rx_state = RX_STATE_HEADER2;
				}
				else {
					// out of sync
					PDEBUG("c = %02x, rx_state = RX_STATE_HEADER2, next rx_state = RX_STATE_HEADER1\n", c);
					rx_state = RX_STATE_HEADER1;
				}
				
				break;
			}

			case RX_STATE_DEVICE : {
				if (0 < c && c < CP430_DEVICE_COUNT) {
					if (devices[c].id != 0) {
						if (kfifo_len(&devices[c].rx_fifo) > 0) {
							// still the previous packet is in the rx_fifo
							// we lose data here
							PDEBUG("Error ! previous packet is still in the rx_fifo of device = %d)\r\n", c);
							rx_state = RX_STATE_HEADER1;
						}
						else {
							active_rx_device = c;
							active_rx_packet_checksum = c;

							kfifo_reset(&devices[active_rx_device].rx_fifo);
							memset(active_rx_packet_head, 0, 6);
							active_rx_packet_head[0] = 0x43;
							active_rx_packet_head[1] = 0x50;
							active_rx_packet_head[2] = c;
							kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, active_rx_packet_head, 3, &devices[active_rx_device].rx_lock);
							
							rx_state = RX_STATE_COMMAND;

							if (is_valid_device(active_rx_device)) { // this check is done to make sure to avoid any problems due to invalid device number received
								if (devices[active_rx_device].rx_logging == PACKET_LOGGING_PARTIAL || 
								    devices[active_rx_device].rx_logging == PACKET_LOGGING_FULL) {
									PDEBUG_PACKET("\r\n< [%u] 43,50,%02x,", (unsigned int)jiffies, c);
									rx_logging_count = 3;
								}
							}
						}
					}
					else {
						// device is not registered
						// we loose data here
						PERROR("Error ! packet for unregistered device\r\n");
						rx_state = RX_STATE_HEADER1;	
					}
				}
				else {
					// PDEBUG("Error ! unknown device\r\n");
					// out of sync
					rx_state = RX_STATE_HEADER1;
				}				
				break;
			}

			case RX_STATE_COMMAND : {
				if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
					PDEBUG("Error ! rx fifo is full\r\n");
					// out of sync
					kfifo_reset(&devices[active_rx_device].rx_fifo);
					rx_state = RX_STATE_HEADER1;
				}
				else {
					active_rx_packet_head[RX_STATE_COMMAND] = c;
					active_rx_packet_checksum += c;
					rx_state = RX_STATE_LENGTH_H;
				}
				break;
			}

			case RX_STATE_LENGTH_H : {
				if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
					PDEBUG("Error ! rx fifo is full\r\n");
					// out of sync
					kfifo_reset(&devices[active_rx_device].rx_fifo);
					rx_state = RX_STATE_HEADER1;
				}
				else {
					active_rx_packet_head[RX_STATE_LENGTH_H] = c;
					active_rx_packet_checksum += c;
					rx_state = RX_STATE_LENGTH_L;
				}
				break;
			}

			case RX_STATE_LENGTH_L : {
				if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
					PDEBUG("Error ! rx fifo is full\r\n");
					// out of sync
					kfifo_reset(&devices[active_rx_device].rx_fifo);
					rx_state = RX_STATE_HEADER1;
				}
				else {
					active_rx_packet_head[RX_STATE_LENGTH_L] = c;
					active_rx_packet_checksum += c;

					active_rx_packet_length = active_rx_packet_head[RX_STATE_LENGTH_H];
					active_rx_packet_length <<= 8;
					active_rx_packet_length &= 0xFF00;
					active_rx_packet_length |= active_rx_packet_head[RX_STATE_LENGTH_L];

					active_rx_data_count = 0;

					if (active_rx_packet_length > 0) {
						rx_state = RX_STATE_DATA;
					}
					else {
						rx_state = RX_STATE_CHECKSUM;
					}
				}
				break;
			}

			case RX_STATE_DATA : {
				if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
					PDEBUG("Error ! rx fifo is full\r\n");
					// out of sync
					kfifo_reset(&devices[active_rx_device].rx_fifo);
					rx_state = RX_STATE_HEADER1;
				}
				else {
					active_rx_data_count++;
					active_rx_packet_checksum += c;

					if (active_rx_data_count == active_rx_packet_length) {
						rx_state = RX_STATE_CHECKSUM;
					}
				}
				break;
			}

			case RX_STATE_CHECKSUM : {
				if (c == active_rx_packet_checksum) {
					if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
						PDEBUG("Error ! rx fifo is full\r\n");
						// out of sync
						kfifo_reset(&devices[active_rx_device].rx_fifo);
						rx_state = RX_STATE_HEADER1;
					}
					else {
						rx_state = RX_STATE_END;
					}
				}
				else {
					PDEBUG("Error ! rx packet checksum\r\n");
					kfifo_reset(&devices[active_rx_device].rx_fifo);
					rx_state = RX_STATE_HEADER1;	
				}
				break;
			}

			case RX_STATE_END : {
				if (0x45 == c) {
					if (!kfifo_in_spinlocked(&devices[active_rx_device].rx_fifo, &c, 1, &devices[active_rx_device].rx_lock)) {
						PDEBUG("Error ! rx fifo is full\r\n");
						// out of sync
						kfifo_reset(&devices[active_rx_device].rx_fifo);
						rx_state = RX_STATE_HEADER1;
					}
					else {
						if (devices[active_rx_device].data != NULL) {
							rx_arg = (rx_event_handler_flag << 16) | kfifo_len(&devices[active_rx_device].rx_fifo);							
							devices[active_rx_device].data->receive_event_handler(rx_arg);							
							rx_event_handler_flag = SYS_NOFLAG;
						}
						else {
							PERROR("Error ! trying to call receive_event_handler in unregistered device\r\n");
						}
						rx_state = RX_STATE_HEADER1;
					}
				}
// 				else if (0x43 == c) {
// 					// looks like start of a new packet, even though the END of previous packet is missing
// 					PDEBUG("Error ! packet END is missing\r\n");
// 
// 					if (devices[active_rx_device].data != NULL) {
// 						devices[active_rx_device].data->receive_event_handler(kfifo_len(devices[active_rx_device].rx_fifo));
// 					}
// 					else {
// 						PERROR("Error ! trying to call receive_event_handler in unregistered device\r\n");
// 					}						
// 					rx_state = RX_STATE_HEADER2;
// 				}
				else {
					// out of sync
					rx_state = RX_STATE_HEADER1;	
				}

				break;
			}

			default : {
				break;
			}
		}
	}
}

void select_active_tx_device(void)
{
	int i = 0;
	int select_next = 0;
	int next_device = 0;

 	if (active_tx_device != -1) {
 		PDEBUG("kfifo_len(&devices[%d].tx_fifo = %d\r\n", active_tx_device, kfifo_len(&devices[active_tx_device].tx_fifo));
 	}
 	else {
 		PDEBUG("active_tx_device = -1\r\n");
 	}

	if (active_tx_device == -1) {
		select_next = 1;
	}
	else if (kfifo_len(&devices[active_tx_device].tx_fifo) == 0) {
		select_next = 1;
	}
	else {
		select_next = 0;
	}

	if (select_next) {
		if (active_tx_device == -1) {
			next_device = 0;
		}
		else {
			next_device = active_tx_device;
			atomic_set(&devices[active_tx_device].tx_state, TX_STATE_IDLE);

			if (devices[active_tx_device].data != NULL) {
				devices[active_tx_device].data->transmit_event_handler(TX_EVENT_IDLE);
			}
			else {
				PERROR("Error ! trying to call transmit_event_handler in unregistered device\r\n");
			}
		}

		for (i = 0; i < (CP430_DEVICE_COUNT - 1); i++) {
			next_device++;
			if (next_device == CP430_DEVICE_COUNT) {
				next_device = 1;
			}

			PDEBUG("next_device = %d\r\n", next_device);

			if (kfifo_len(&devices[next_device].tx_fifo) > 0) {
				atomic_set(&devices[next_device].tx_state, TX_STATE_BUSY);
				active_tx_device = next_device;
				break;
			}
	
			if (i == 5) {
				active_tx_device = -1;
			}
		}

		if (active_tx_device != -1) {
			if (devices[active_tx_device].tx_logging == PACKET_LOGGING_PARTIAL || 
			    devices[active_tx_device].tx_logging == PACKET_LOGGING_FULL) {
				PDEBUG_PACKET("\r\n> [%u] ", (unsigned int)jiffies);

				tx_logging_count = 0;
			}
		}

		PDEBUG("active_tx_device = %d\r\n", active_tx_device);
	}
}

static void rx_work(struct work_struct *work)
{	
	int 				retval = 0;
	unsigned long 			flags  = 0;
	struct cp430_core_device 	*dev;
	unsigned char			buf[MAX_RX_PACKET_SIZE];
	int i = 0;
	
	
	PDEBUG("> : %s\r\n", __FUNCTION__);	
	
	dev = container_of(work, struct cp430_core_device, rx_work);

	spin_lock_irqsave(&dev->spi.lock, flags);

	if (atomic_read(&dev->spi.tx_state) == SPI_STATE_IDLE){

		atomic_set(&dev->spi.rx_state, SPI_STATE_BUSY);
		PDEBUG("dev->spi.tx_state = SPI_STATE_IDLE\r\n");	
	
		spin_unlock_irqrestore(&dev->spi.lock, flags);

		memset(buf, 0, sizeof(buf));
		kfifo_reset(&dev->rx_fifo);
		
		for (i = 0; i < MAX_RX_PACKET_SIZE; i++) {
			spi_message_init(&dev->spi.msg);

			dev->spi.xfer.tx_buf = buf;
			dev->spi.xfer.rx_buf = dev->spi.rx_buf;
			dev->spi.xfer.len    = 1;

			spi_message_add_tail(&dev->spi.xfer, &dev->spi.msg);
			
	//	 	spin_lock_irqsave(&dev->spi.lock, flags);
			PDEBUG("> : spi_sync(dev->spi.dev, &dev->spi.msg)in %s\r\n", __FUNCTION__);
			retval = spi_sync(dev->spi.dev, &dev->spi.msg);
			PDEBUG("< : spi_sync(dev->spi.dev, &dev->spi.msg), retval = %d in %s\r\n", retval, __FUNCTION__);
	//	 	spin_unlock_irqrestore(&dev->spi.lock, flags);
			
			if (!kfifo_in_spinlocked(&dev->rx_fifo, dev->spi.rx_buf, 1, &dev->rx_lock)) {
				PDEBUG("error ! cp430_core rx buffer full\n");
			}
			
			usleep_range(100, 500);
		}
		
		atomic_set(&dev->spi.rx_state, SPI_STATE_IDLE);
		
		process_rx_fifo(0);
	}
	else{
		PDEBUG("dev->spi.tx_state = SPI_STATE_BUSY\r\n");
		
		schedule();
		queue_work(dev->rx_wq, &dev->rx_work);
	}
}

static void tx_work(struct work_struct *work)
{
	int                i, len, retval = 0;
	unsigned long 				flags = 0;
	struct cp430_core_device 	*dev;

	
	PDEBUG("> : %s\r\n", __FUNCTION__);	
	
	dev = container_of(work, struct cp430_core_device, tx_work);
	

	spin_lock_irqsave(&dev->spi.lock, flags);

	if (atomic_read(&dev->spi.rx_state) == SPI_STATE_IDLE){

		atomic_set(&dev->spi.tx_state, SPI_STATE_BUSY);
		PDEBUG("dev->spi.rx_state = SPI_STATE_IDLE\r\n");	
		
		spin_unlock_irqrestore(&dev->spi.lock, flags);
		
		select_active_tx_device();

		if (active_tx_device != -1) {
			len = kfifo_len(&devices[active_tx_device].tx_fifo);
			
			retval = kfifo_out_spinlocked(&devices[active_tx_device].tx_fifo, dev->spi.tx_buf, len, &devices[active_tx_device].tx_lock);

			for (i = 0; i < len; i++) {
				if (devices[active_tx_device].tx_logging == PACKET_LOGGING_PARTIAL) {
					if (tx_logging_count < 16) {
						PDEBUG_PACKET("%02x,", dev->spi.tx_buf[i]);
						tx_logging_count ++;
					}
					else if (tx_logging_count == 16) {
						PDEBUG_PACKET(".....");
						tx_logging_count ++;					
					}
				}						

				if (devices[active_tx_device].tx_logging == PACKET_LOGGING_FULL) {
					PDEBUG_PACKET("%02x,", dev->spi.tx_buf[i]);
				}
			}
			
			for (i = 0; i < len; i++) {
				spi_message_init(&dev->spi.msg);
	
				dev->spi.xfer.tx_buf = (dev->spi.tx_buf)+i;
				dev->spi.xfer.rx_buf = NULL;
				dev->spi.xfer.len    = 1;
	
				spi_message_add_tail(&dev->spi.xfer, &dev->spi.msg);
				
	//	 		spin_lock_irqsave(&dev->spi.lock, flags);
				PDEBUG("> : spi_sync(dev->spi.dev, &dev->spi.msg)in %s\r\n", __FUNCTION__);
				retval = spi_sync(dev->spi.dev, &dev->spi.msg);
				PDEBUG("< : spi_sync(dev->spi.dev, &dev->spi.msg), retval = %d in %s\r\n", retval, __FUNCTION__);
	//	 		spin_unlock_irqrestore(&dev->spi.lock, flags);
				
				usleep_range(100, 500);
			}

			atomic_set(&dev->spi.tx_state, SPI_STATE_IDLE);
			queue_work(dev->tx_wq, &dev->tx_work);			
		}
		else{
			atomic_set(&dev->spi.tx_state, SPI_STATE_IDLE);
		}
	}
	else{
		PDEBUG("dev->spi.rx_state = SPI_STATE_BUSY\r\n");
		
		schedule();
		queue_work(dev->tx_wq, &dev->tx_work);
	}
}

static int cp430_core_create_proc(void)
{
	struct proc_dir_entry	*proc_dir = proc_mkdir(DEVICE_NAME, NULL);

	
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	if (!proc_dir) {
		return -EFAULT;
	}

	if (create_proc_file_debug(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}

	if (create_proc_file_error(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}
	
	if (create_proc_file_packet(proc_dir, MAX_PROC_BUFFER_SIZE) < 0) {
		return -EFAULT;
	}	

	return 0;
}

static int cp430_core_remove_proc(void)
{
	int result = 0;

	
	PDEBUG("> : %s\r\n",__FUNCTION__);	
	
	result = remove_proc_file(&debug_file);
	if (result < 0){
		return result;
	}
	
	result = remove_proc_file(&error_file);
	if (result < 0){
		return result;
	}
	
	result = remove_proc_file(&packet_file);
	if (result < 0) {
		return result;
	}
	
	remove_proc_entry(DEVICE_NAME, NULL);

	return result;
}

int cp430_core_open(struct inode *inode, struct file *filp)
{
	struct cp430_core_device *dev; /* device information */

	
	PDEBUG("> : %s\r\n",__FUNCTION__);	
	
	dev = container_of(inode->i_cdev, struct cp430_core_device, cdev);
	filp->private_data = dev; /* for other methods */

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	/* .. */
	
	/* .. */
	
	up(&dev->sem);
	
	return 0;          /* success */
}

int cp430_core_release(struct inode *inode, struct file *filp)
{
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	return 0;
}

//ssize_t cp430_core_read(struct file *filp, char __user *buf, size_t count,
//                loff_t *f_pos)
//{
//	struct cp430_core_device *dev = filp->private_data; 
//	ssize_t retval = 0;
//
//	
//	PDEBUG("> : %s\r\n",__FUNCTION__);
//	
//	if (down_interruptible(&dev->sem))
//		return -ERESTARTSYS;
//
//	/* .. */
//	
//	/* .. */
//	
//	*f_pos += count;
//	retval = count;
//
//	up(&dev->sem);
//	return retval;
//}

//ssize_t cp430_core_write(struct file *filp, const char __user *buf, size_t count,
//                loff_t *f_pos)
//{
//	struct cp430_core_device *dev = filp->private_data;
//	ssize_t retval = -ENOMEM;
//
//
//	PDEBUG("> : %s\r\n",__FUNCTION__);
//	
//	if (down_interruptible(&dev->sem))
//		return -ERESTARTSYS;
//
//	/* .. */
//	
//	/* .. */
//	
//	*f_pos += count;
//	retval = count;
//
//	up(&dev->sem);
//	return retval;
//}

long cp430_core_ioctl(struct file *filp,
                 unsigned int cmd, unsigned long arg)
{

	int err = 0;
	int retval = 0;
    

	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	if (_IOC_TYPE(cmd) != CP430_CORE_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > CP430_CORE_IOC_MAXNR) return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {

	  case CP430_CORE_GET_DRIVER_VERSION:
		break;
	
	/* .. */
	
	/* .. */

	  default:
		return -ENOTTY;
	}

	return retval;
}

struct file_operations cp430_core_fops = {
	.owner 			=    THIS_MODULE,
//	.read 			=     cp430_core_read,
//	.write 			=    cp430_core_write,
	.unlocked_ioctl	=    cp430_core_ioctl,
	.open 			=     cp430_core_open,
	.release 		=  cp430_core_release,
};

static int __devinit cp430_spi_probe (struct spi_device *spi_device)
{
	unsigned long flags;

	
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	spin_lock_irqsave(&core_device->spi.lock, flags);
	core_device->spi.dev = spi_device;
	spin_unlock_irqrestore(&core_device->spi.lock, flags);
	
	dev_set_drvdata(&spi_device->dev, core_device);
	
	return 0;
}

static int __devexit cp430_spi_remove(struct spi_device *spi_device)
{
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	return 0;
}

static void cp430_spi_shutdown(struct spi_device *spi_device)
{
	PDEBUG("> : %s\r\n",__FUNCTION__);
	/*{KW}: Set suspend resume status. Set to suspend */
	atomic_set(&is_sys_suspend, 1);
}

static int cp430_spi_suspend(struct spi_device *spi_device, pm_message_t mesg)
{
	int i       = 0;
	PDEBUG("> : %s\r\n",__FUNCTION__);
	for (i = 1; i < CP430_DEVICE_COUNT; i++) {
		
		if ((kfifo_len(&devices[i].tx_fifo) > 0) || (kfifo_len(&devices[i].rx_fifo) > 0) ){
								
				PERROR("%s : suspend FAILED! Device is busy...\r\n", __FUNCTION__);
				return -EBUSY;
		}			
	}
	/*{KW}: Set suspend resume status. Set to suspend */
	atomic_set(&is_sys_suspend, 1);
	
	return 0;	
}

static int cp430_spi_resume(struct spi_device *spi_device)
{
	PDEBUG("> : %s\r\n",__FUNCTION__);

	/*{KW}: Set suspend resume status. Set to not suspend */
	atomic_set(&is_sys_suspend, 0);
	
	return 0;	
}

static struct spi_driver cp430_spi_driver = {
	.driver = {
               .name           = "cp430",
               .bus            = &spi_bus_type,
               .owner          = THIS_MODULE,
	},

	.probe		= cp430_spi_probe,
	.remove		= __devexit_p(cp430_spi_remove),
	.shutdown	= cp430_spi_shutdown,
	.suspend	= cp430_spi_suspend,
	.resume		= cp430_spi_resume,
};

void cp430_core_cleanup_module(void)
{
	int i       = 0;
	dev_t devno = MKDEV(core_device->major, core_device->minor);
	
	/* cp430_dev unregister */
	if (cp430_device_unregister(CP430_CORE) < 0) {
		PDEBUG("cp430_dev unregistering fail\r\n");
	}
	else {
		PDEBUG("cp430_dev unregistering success\r\n");
	}
	
	if (!core_device) {
		goto exit;
	}

	if (core_device->irq >= 0) {
		free_irq(core_device->irq, core_device);
	}	

	if (!(&core_device->rx_fifo)) {
		kfifo_free(&core_device->rx_fifo);
	}

	for (i = 1; i < CP430_DEVICE_COUNT; i++) {
		kfifo_free(&devices[i].rx_fifo);
		kfifo_free(&devices[i].tx_fifo);
	}
	
	/* cleanup tasklets/workqueues */
	if (use_rx_wq) {
		if (core_device->rx_wq) {
			cancel_work_sync(&core_device->rx_work);
			flush_workqueue(core_device->rx_wq);
			destroy_workqueue(core_device->rx_wq);			
		}
	}
	else {
		//	tasklet_disable(&process_interrupt_tasklet);
		//	tasklet_disable(&process_rx_fifo_tasklet);
	}
	
	if (use_tx_wq) {
		if (core_device->tx_wq) {
			cancel_work_sync(&core_device->tx_work);
			flush_workqueue(core_device->tx_wq);
			destroy_workqueue(core_device->tx_wq);
		}
	}
	else {
		//	tasklet_disable(&process_tx_fifo_tasklet);
	}	
	
	/* unregister spi driver */
	if (core_device->spi.tx_buf) {
		kfree(core_device->spi.tx_buf);
	}
	
	if (core_device->spi.rx_buf) {
		kfree(core_device->spi.rx_buf);
	}

	spi_unregister_driver(&cp430_spi_driver);

	/* remove device and class */
	device_destroy(core_device->class, devno);
	class_destroy(core_device->class);

	/* remove proc */
	if (cp430_core_remove_proc() < 0) {
		printk(KERN_ERR "%s : cp430_core_remove_proc failed\r\n", DEVICE_NAME);
		goto exit;
	}

	/* unregister char driver */
	unregister_chrdev_region(devno, 1);
	
	if (core_device->devices) {
		kfree(core_device->devices);
	}
	
	if (core_device) {
		cdev_del(&core_device->cdev);
		kfree(core_device);
	}

exit:
	do{}while(0);
}

static irqreturn_t cp430_core_interrupt(int irq, void *dev_id)
{
	struct cp430_core_device *dev  = (struct cp430_core_device*)dev_id;
	
	PDEBUG("> : %s\r\n", __FUNCTION__);
	
	#if 0
	unsigned long PM_WKST_PER      = 0x483070B0;
	unsigned long GPIO5_IRQSTATUS1 = 0x49056018;
	void *voidp;
	unsigned int data;
	
	
	PDEBUG("> : %s\r\n", __FUNCTION__);	

	voidp = ioremap(GPIO5_IRQSTATUS1,4);
	data  = ioread32(voidp);
	
	PDEBUG("Interrupt received GPIO5_IRQSTATUS1=0X%x\r\n",data);
	
	iounmap(voidp); 

	voidp = ioremap(PM_WKST_PER,4);
	data  = ioread32(voidp);
	
	PDEBUG("PM_WKST_PER=0X%x\n",data);
	
	iounmap(voidp);
	#endif
	
	if (use_rx_wq){
		PDEBUG("> : queue_work(dev->rx_wq, &dev->rx_work)\r\n");	
		if(atomic_read(&is_sys_suspend) > 0)
		{
			rx_event_handler_flag = SYS_RESUMING;
		}
		queue_work(dev->rx_wq, &dev->rx_work);
	}
	else{
		/* schedule tasklets */
	}

	return IRQ_HANDLED;
}

static int init_interrupt(struct cp430_core_device *dev)
{
	int result = 0;
	
	PDEBUG("> : %s\r\n", __FUNCTION__);

	result = gpio_request(CP430_IRQ_GPIO , "CP_INT");
	if (result < 0)
		goto fail;

	result = gpio_direction_input(CP430_IRQ_GPIO);
	if (result < 0)
		goto fail_irq;

	dev->irq = gpio_to_irq(CP430_IRQ_GPIO);
	if (dev->irq < 0)
	{
		result = -EFAULT;
		goto fail_irq;
	}

	result = request_irq(dev->irq, cp430_core_interrupt, IRQF_TRIGGER_FALLING, DEVICE_NAME, dev);
	if (result < 0)
		goto fail_irq;
		
	result = enable_irq_wake(dev->irq);
	if (result < 0)
		goto fail_irq_req;
	
	PDEBUG("< : %s - result = %d\r\n", __FUNCTION__, result);
	
	return result;
    
fail_irq_req:
	free_irq(dev->irq, dev);
fail_irq:
	gpio_free(CP430_IRQ_GPIO);
fail:
	printk(KERN_ERR "%s: can't get assigned irq %i\n", DEVICE_NAME, dev->irq);
	dev->irq = -1;

    return result;
}

static int init_variables(struct cp430_core_device *dev)
{
 	int i = 0;
 	int retval = 0;

	memset(active_rx_packet_head, 0, 6);
	active_rx_device          = -1;
	active_tx_device          = -1;
	active_rx_packet_length   = 0;
	active_rx_data_count      = 0;
	active_rx_packet_checksum = 0;
	rx_state                  = RX_STATE_HEADER1;
	
	for (i = 1; i < CP430_DEVICE_COUNT; i++) {
		atomic_set(&devices[i].tx_state, TX_STATE_IDLE);
		atomic_set(&devices[i].rx_state, RX_STATE_IDLE);
	}

	/* set logging format */
	for (i = 1; i < CP430_DEVICE_COUNT; i++) { // full logging
		devices[i].rx_logging = PACKET_LOGGING_FULL;
		devices[i].tx_logging = PACKET_LOGGING_FULL;
	} 	
 	
	// core buffers
	retval = kfifo_alloc(&dev->rx_fifo, CP430_CORE_RX_FIFO_SIZE, GFP_KERNEL);
	if (retval < 0) {
		return retval;
	}

	// device buffers
	for (i = 1; i < CP430_DEVICE_COUNT; i++) {
		retval = kfifo_alloc(&devices[i].rx_fifo, rx_fifo_sizes[i], GFP_KERNEL);
		if (retval < 0) {
			return retval;
		}

		retval = kfifo_alloc(&devices[i].tx_fifo, tx_fifo_sizes[i], GFP_KERNEL);
		if (retval < 0) {
			kfifo_free(&devices[i].rx_fifo);
			return retval;
		}

		sema_init(&devices[i].rx_sema, 1);
		sema_init(&devices[i].tx_sema, 1);
	}
	
	/*{KW}: Init suspend resume status. Set to not suspend */
	atomic_set(&is_sys_suspend, 0);

	return 0;
}

static int cp430_core_init_spi(struct cp430_core_device *dev)
{
	int result = 0;
	
	
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	dev->spi.tx_buf = kmalloc(SPI_BUF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!dev->spi.tx_buf) {
		return -ENOMEM;
	}
	
	dev->spi.rx_buf = kmalloc(SPI_BUF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!dev->spi.rx_buf) {
		return -ENOMEM;
	}

	spin_lock_init(&dev->spi.lock);
	sema_init(&dev->spi.sem, 1);

	atomic_set(&dev->spi.tx_state, SPI_STATE_IDLE);
	atomic_set(&dev->spi.rx_state, SPI_STATE_IDLE);
	
	result = spi_register_driver(&cp430_spi_driver); 	

	return result;
}

static int cp430_core_init_class(struct cp430_core_device *dev)
{
	PDEBUG("> : %s\r\n",__FUNCTION__);
		
	dev->class = class_create(THIS_MODULE, CLASS_NAME);

	if (!dev->class) {
		printk(KERN_ALERT "class_create() failed\n");
		return -EFAULT;
	}

	if (!device_create(dev->class, NULL, dev->cdev.dev, NULL, DEVICE_NAME)) {
		printk(KERN_ALERT "device_create(..., %s) failed\n", DEVICE_NAME);
		class_destroy(dev->class);
		return -EFAULT;
	}

	return 0;
}

static int cp430_core_init_cdev(struct cp430_core_device *dev, struct file_operations *fops)
{
	int result   = 0;
	dev_t devno  = 0;


	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	/* register char driver */
	if (CP430_CORE_MAJOR) {
		dev->major = CP430_CORE_MAJOR;
		dev->minor = CP430_CORE_MINOR;
		devno      = MKDEV(dev->major, dev->minor);
		result     = register_chrdev_region(devno, 1, DEVICE_NAME);
	}
	else {
		result     = alloc_chrdev_region(&devno, dev->minor, 1, DEVICE_NAME);
		dev->major = MAJOR(devno);
		devno      = MKDEV(dev->major, dev->minor);
	}

	if (result < 0) {
		printk(KERN_ERR "%s: can't get major %d\n", DEVICE_NAME, dev->major);
		return result;
	}

	sema_init(&dev->sem, 1);
	
	cdev_init(&dev->cdev, fops);
	
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops   = fops;
		
	result = cdev_add (&dev->cdev, devno, 1);

	if (result) {
		printk(KERN_ERR "%s: error adding %d", DEVICE_NAME, devno);
	}
	
	return result;
}

int cp430_core_init_module(void)
{
	int result = 0;
	struct cp430_core_device *dev;
	unsigned char packet[8];
	
	PDEBUG("> : %s\r\n",__FUNCTION__);
	
	dev = kmalloc(sizeof(struct cp430_core_device), GFP_KERNEL);
	if (!dev) {
		core_device = NULL;
		result         = -ENOMEM;
		goto fail;
	}

	core_device = dev;
	memset(dev, 0, sizeof(struct cp430_core_device));
	
	dev->devices = kmalloc(CP430_DEVICE_COUNT * sizeof(struct cp430_device), GFP_KERNEL);
	if (!dev->devices) {
		result = -ENOMEM;
		goto fail;
	}
	
	memset(dev->devices, 0, CP430_DEVICE_COUNT * sizeof(struct cp430_device));
	devices = dev->devices;
	
	/* register char driver */
	result = cp430_core_init_cdev(dev, &cp430_core_fops);
	if (result < 0){
		goto fail;
	}
	
	/* create proce files */
	result = cp430_core_create_proc();
	if (result < 0){
		goto fail;
	}
	
	/* register class */
	result = cp430_core_init_class(dev);
	if (result < 0){
		goto fail;
	}
	
	/* register spi driver */
	result = cp430_core_init_spi(dev);
	if (result < 0){
		goto fail;
	}

	/* init rx/tx process */
	if (use_rx_wq){
		dev->rx_wq = create_singlethread_workqueue("cp430_core-rx_wq");
		
		if (dev->rx_wq == NULL) {
			result = -EFAULT;
			goto fail;
		}
		
		INIT_WORK(&dev->rx_work, rx_work);
	}
	else {
		/* tasklet initializaton */
	}

	if (use_tx_wq){
		dev->tx_wq = create_singlethread_workqueue("cp430_core-tx_wq");
		
		if (dev->tx_wq == NULL) {
			result = -EFAULT;
			goto fail;
		}
		
		INIT_WORK(&dev->tx_work, tx_work);
	}
	else {
		/* tasklet initializaton */
	}	
	
	/* initialize variables */
	result = init_variables(dev);
	if (result < 0){
		goto fail;
	}
	
	/* initialize interrupts */
	result = init_interrupt(dev);
	if(result < 0) {
		goto fail;
	}
	
	/* register cp430 device with the bus driver */
	cp430_dev_data.receive_event_handler  = cp430_dev_receive_event_handler;
	cp430_dev_data.transmit_event_handler = cp430_dev_transmit_event_handler;

	result = cp430_device_register(CP430_CORE, &cp430_dev_data);
	
	if (result < 0) {
		PDEBUG("cp430_dev registering fail\r\n");
		goto fail;
	}
	else {
		PDEBUG("cp430_dev registering success\r\n");
	}
	
	/* send initial cp430_core get status */
	
	cp430_create_packet(CP430_CORE, CMD_CP430_CORE_GET_STATUS, 0, NULL, packet);
	result = cp430_core_write(CP430_CORE, packet, sizeof(packet));
	
	if (result < 0) {
		PDEBUG("cp430_dev : error cp430_core_write\r\n");
		goto fail;
	}
	
	command_response_received_flag = 0;
	wait_event_timeout(command_response_received_wq, command_response_received_flag != 0, (1 * HZ));

	if(command_response_received_flag){
		if(command_response_received_flag < 0){
			PDEBUG("cp430_dev : get_status failed\r\n");
		}
	}
	else{
		PDEBUG("cp430_dev : time out\r\n");
	}

	return 0;

  fail:
	cp430_core_cleanup_module();
	return result;
}

module_init(cp430_core_init_module);
module_exit(cp430_core_cleanup_module);
