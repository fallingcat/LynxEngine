//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXCPU_H__
#define __LYNXCPU_H__

//********************************** Lynx CPU **************************************************************
/**
 *  @defgroup LynxCPU <CPU Module> 
 *
 *	CPU related functions.
 */
//**********************************************************************************************************
#include <LynxType.h>

#define LYNX_CPU_FEATURE_MMX									0x0001
#define LYNX_CPU_FEATURE_SSE									0x0002
#define LYNX_CPU_FEATURE_SSE2									0x0004
#define LYNX_CPU_FEATURE_3DNOW									0x0008

// These are the bit flags that get set on calling cpuid
// with register eax set to 1
#define LYNX_MMX_FEATURE_BIT			 				        0x00800000
#define LYNX_SSE_FEATURE_BIT									0x02000000
#define LYNX_SSE2_FEATURE_BIT									0x04000000
// This bit is set when cpuid is called with
// register set to 80000001h (only applicable to AMD)
#define LYNX_3DNOW_FEATURE_BIT									0x80000000

#define LYNX_IS_FPU												0x0001
#define LYNX_IS_MMX												0x0002
#define LYNX_IS_SSE												0x0004
#define LYNX_IS_SSE2											0x0008
#define LYNX_IS_3DNOW											0x0010
/**
 *  @ingroup LynxCPU 
 *	@struct LPLYNXCPUINFO
 *  @brief CPU資訊指標。
 *	@see LYNXCPUINFO
 */
/**
 *  @ingroup LynxCPU 
 *  @brief CPU資訊。
 *	@see LPLYNXCPUINFO
 */
LYNX_CACHE_ALIGN typedef struct {
    char								VendorName[16];					/*!< vendor name. */
	char								ModelName[32];					/*!< Name of model eg. Intel_Pentium_Pro. */
    int									Family;                         /*!< family of the processor, eg. Intel_Pentium_Pro is family 6 processor. */
    int									Model;                          /*!< madel of processor, eg. Intel_Pentium_Pro is model 1 of family 6 processor. */
    int									Stepping;                       /*!< Processor revision number. */
    int									Feature;                        /*!< processor Feature ( same as return value). */
    int									OSSupport;                      /*!< does OS Support the feature. */
    int									Checks;                         /*!< what all bits are checked are set to one. */
}LYNXCPUINFO, *LPLYNXCPUINFO;
typedef const LYNXCPUINFO* LPCLYNXCPUINFO;

#ifdef __cplusplus
extern "C" {
#endif														

LYNXCOREAPI void			lynxGetCPUInfo(LPLYNXCPUINFO lpinfo);
LYNXCOREAPI LYNXBOOL		lynxMungeFPCW(void);
LYNXCOREAPI void			lynxRestoreFPCW(void);
LYNXCOREAPI LYNXINT64		lynxCPUClock(void);

#ifdef __cplusplus
}
#endif			

#endif