
#ifndef _CP430_H_
#define _CP430_H_

#include <linux/poll.h>

#define EXPORT_FILE_OPERATIONS

#define STX1              	0x43
#define STX2                    0x50
#define ETX                     0x45

#define CP430_DEVICE_COUNT	7
#define CP430_CORE		1
#define CP430_DEV_KEYPAD	2
#define CP430_DEV_DISPLAY	3
#define CP430_DEV_CHARGER	4
#define CP430_DEV_POWER		5
#define CP430_DEV_MISC		6

/* packet format */
#define CP430_HEADER1		0
#define CP430_HEADER2		1
#define CP430_DEVICE		2
#define	CP430_COMMAND		3
#define	CP430_LENGTH_H		4
#define	CP430_LENGTH_L		5
#define	CP430_DATA		6
#define	CP430_CHECKSUM		7
#define	CP430_END		8

/* received data lengths */
#define RCVD_DATA_LEN_CMD_RESP		0x01
#define RCVD_DATA_LEN_CP430_VER		0x04
#define RCVD_DATA_LEN_KEYPAD_STAT	0x0C
#define RCVD_DATA_LEN_CHGR_STAT		0x06

/* received packet types */
#define PKT_TYP_CP430_GET_STATUS    	((CP430_CORE   << 8) + 0x01)
#define PKT_TYP_KEYPAD_GET_STATUS   	((CP430_DEV_KEYPAD  << 8) + 0x01)
#define PKT_TYP_DISPLAY_ON_OFF      	((CP430_DEV_DISPLAY << 8) + 0x01)
#define PKT_TYP_DISPLAY_WRITE       	((CP430_DEV_DISPLAY << 8) + 0x02)
#define PKT_TYP_CHARGER_GET_STATUS  	((CP430_DEV_CHARGER << 8) + 0x01)
#define PKT_TYP_UPDATE_CC_PWR_STATUS  	((CP430_DEV_POWER << 8) + 0x01)

/* transmit events */
#define TX_EVENT_IDLE		0

/* diag events */
#define DIAG_EVENT_STATUS_UPDATE	0

/*{KW}: RX event handler FLAGs */
#define SYS_NOFLAG		0x0000
#define SYS_RESUMING	0x0001

typedef int (event_handler) (unsigned int arg);

struct device_data{
	event_handler*	receive_event_handler;
	event_handler*	transmit_event_handler;
};

struct core_operations {
	int (* get_rx_fifo_len) (unsigned int id);
	int (* get_tx_fifo_len) (unsigned int id);
	int (* flush_rx_fifo) (unsigned int id);
	int (* flush_tx_fifo) (unsigned int id);
};

extern struct core_operations cp430_core_ops;

int cp430_device_register(unsigned int id, struct device_data* data);
int cp430_device_unregister(unsigned int id);
int cp430_core_write(unsigned int id, const unsigned char* buffer, unsigned int size);
int cp430_core_read(unsigned int id, unsigned char* buffer, unsigned int size);



// Note : The size of the packet buffer should be >= (length + 8)
int cp430_create_packet(unsigned char id, unsigned char command, unsigned short length, unsigned char* data, unsigned char* packet)
{
	int index = 0;
    	int i = 0;
    	unsigned char sum = 0;

	packet[index++] = STX1;
	packet[index++] = STX2;
	packet[index++] = id;
	packet[index++] = command;
	packet[index++] = length / 256;
	packet[index++] = length % 256;
	memcpy(&packet[index], data, length); 
	index += length;

    	for (i = 0; i < (index - 2); i++) {
		sum += *(packet+2+i);
	}

	packet[index++] = sum;
	packet[index++] = ETX;

	return index;
}


#endif /* _CP430_H_ */
