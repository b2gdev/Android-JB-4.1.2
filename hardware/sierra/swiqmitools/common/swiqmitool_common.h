/**
 *
 * @ingroup swiqmitool
 *
 * @file
 * common functions and data types for swi qmi tools
 *
 * @author
 * Copyright: © 2012 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */


#ifndef __SWI_QMI_TOOL_COMMON_H__
#define __SWI_QMI_TOOL_COMMON_H__

void qmiDeviceConnect( void );
void qmiDeviceDisconnect( void );
unsigned short piget16(unsigned char **packetpp);
unsigned long piget32(unsigned char **packetpp);

#endif /* __SWI_QMI_TOOL_COMMON_H__ */

