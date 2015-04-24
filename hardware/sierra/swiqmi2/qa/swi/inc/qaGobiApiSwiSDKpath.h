/**
 * \ingroup swi
 *
 * \file    qaGobiApiSwiSDKpath.h
 *
 * \brief   SWI API SDK path defines
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __SLQS_API_SWI_SDK_PATH_H__
#define __SLQS_API_SWI_SDK_PATH_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Native platforms: Laptop i686 PC, Powerbook G4, Pandaboard
 * or embedded: arm, ppc, mips, mipsel
 * SDK_EXE is constructed in makefile and passed in with -D
 */
#define SDKPATHDEFAULT SDK_EXE

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __SLQS_API_SWI_SDK_PATH_H__ */
