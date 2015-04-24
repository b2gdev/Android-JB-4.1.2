/*************
 *
 * Filename:    sdkudefs.h
 *
 * Purpose:     Global definitions used inside the SDK
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* Pragmas */
#pragma once

/*************
 *
 * Name:    sdkpkg_e
 *
 * Purpose: SDK package enumeration
 *
 * Members: AASDKPKG    -
 *          AMSDKPKG    -
 *          CISDKPKG    - interprocess communication facilities
 *          DLSDKPKG    - data logging
 *          DRSDKPKG    - data relay
 *          DSSDKPKG    - device services
 *          ERSDKPKG    - error handling
 *          ICSDKPKG    - intertask communication facilities
 *          MMSDKPKG    - memory management
 *          OSSDKPKG    - operating System interface
 *          PISDKPKG    - architecture independent integer insertion/extraction
 *                        to/from a byte array.
 *          QASDKPKG    - QMI API services
 *          QMSDKPKG    - QMI SDK services
 *          SDKSDKPKG   - entities common to all other SDK packages
 *          SLSDKPKG    - string Library
 *          SUSDKPKG    - start-up
 *          UISDKPKG    - user interface
 *          USSDKPKG    - usb subystem
 *
 * Notes:   None
 *
 **************/
enum sdkpkg_e{
    AASDKPKG,
    AMSDKPKG,
    CISDKPKG,
    DLSDKPKG,
    DRSDKPKG,
    DSSDKPKG,   /* 5 */
    ERSDKPKG,
    ICSDKPKG,
    MMSDKPKG,
    OSSDKPKG,
    PISDKPKG,   /* 10 */
    QASDKPKG,
    QMSDKPKG,
    SDKSDKPKG,
    SLSDKPKG,
    SUSDKPKG,   /* 15 */
    UISDKPKG,
    USSDKPKG,
};
