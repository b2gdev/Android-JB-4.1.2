/*
 * \ingroup dcs
 *
 * \file    qaDcsQCWWANEnumerateDevices.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsQCWWANEnumerateDevices.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_QCWWANENUMERATEDEVICES_H__
#define __DCS_QCWWANENUMERATEDEVICES_H__

typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
} device_info_t;

/* Prototypes */
extern ULONG  QCWWANEnumerateDevices(
    BYTE * pDevicesSize,
    BYTE * pDevices );

#endif /* __DCS_QCWWANENUMERATEDEVICES_H__ */
