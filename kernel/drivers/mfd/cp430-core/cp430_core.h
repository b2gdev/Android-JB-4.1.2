
#ifndef _CP430_CORE_H_
#define _CP430_CORE_H_

#include <linux/ioctl.h>
#include <linux/kfifo.h>
#include <linux/spi/spi.h>
#include <linux/workqueue.h>
#include  "cp430.h"


#ifndef CP430_CORE_MAJOR
#define CP430_CORE_MAJOR 0		/* dynamic major by default */
#endif

#ifndef CP430_CORE_MINOR
#define CP430_CORE_MINOR 0
#endif

#ifndef CP430_CORE_NR_DEVS
#define CP430_CORE_NR_DEVS 1
#endif

#define	CLASS_NAME	"tcbin"
#define	DEVICE_NAME	"cp430_core"
#define	CP430_IRQ_GPIO	12


struct spi_data {
	struct spi_device 		*dev; 		/* {PS} : TODO - update as necessary 		*/
	struct spi_message 		msg;
	struct spi_transfer 	xfer;
	unsigned char 			*tx_buf; 
	unsigned char			*rx_buf;	
	spinlock_t 				lock;
	struct semaphore		sem;
	atomic_t 				tx_state;
	atomic_t 				rx_state;
};

struct cp430_core_device {
	int							major;
	int 						minor;
	int							irq;
	struct semaphore 			sem; 
	struct cdev 				cdev;	  	/* Char device structure		*/
	struct class 				*class;
	struct spi_data				spi;
	struct cp430_device 		*devices;
	struct kfifo				rx_fifo;
	spinlock_t					rx_lock;
	struct workqueue_struct 	*rx_wq;
	struct workqueue_struct 	*tx_wq;
	struct work_struct 			rx_work;
	struct work_struct 			tx_work;	
};


#define CP430_CORE_RX_FIFO_SIZE	4096

#define	SPI_BUF_SIZE		1024
#define SPI_STATE_IDLE		0
#define SPI_STATE_BUSY		1

#define TX_STATE_IDLE		0
#define TX_STATE_BUSY		1

#define RX_STATE_IDLE		0
#define RX_STATE_BUSY		1


struct cp430_device{
	unsigned int		id;
	struct device_data*	data;
	struct kfifo		rx_fifo;
	struct kfifo		tx_fifo;
	spinlock_t		rx_lock;
	spinlock_t		tx_lock;
	struct semaphore 	rx_sema;
	struct semaphore 	tx_sema;
	atomic_t 		rx_state;
	atomic_t 		tx_state;
	unsigned int		rx_logging;
	unsigned int 		tx_logging;
};

enum rx_states {
	RX_STATE_HEADER1	= 0,
	RX_STATE_HEADER2	= 1,
	RX_STATE_DEVICE		= 2,
	RX_STATE_COMMAND	= 3,
	RX_STATE_LENGTH_H	= 4,
	RX_STATE_LENGTH_L	= 5,
	RX_STATE_DATA		= 6,
	RX_STATE_CHECKSUM	= 7,
	RX_STATE_END		= 8
};

const unsigned int rx_fifo_sizes[] = {
					0,	/* core driver */
					1024,	/* device 1 */
					1024,	/* device 2 */
					1024,	/* device 3 */
					1024,	/* device 4 */
					1024,	/* device 5 */
					1024,	/* device 6 */
					1024,	/* device 7 */
					};

const unsigned int tx_fifo_sizes[] = {
					0,	/* core driver */
					1024,	/* device 1 */
					1024,	/* device 2 */
					1024,	/* device 3 */
					1024,	/* device 4 */
					1024,	/* device 5 */
					1024,	/* device 6 */
					1024,	/* agent    */
					};

#define	MAX_RX_PACKET_SIZE		64


/* cp430_core commands */
#define	CMD_CP430_CORE_GET_STATUS		0x01



#endif /* _CP430_CORE_H_ */
