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

#ifndef __LYNXH__
#define __LYNXH__

#include <LynxMath.h>
#include <LynxErrors.h>
#include <LynxIO.h>
#include <LynxOS.h>
#include <LynxCPU.h>
#include <LynxLog.h>
#include <LynxProfile.h>
#include <LynxCRC.h>
#include <LynxMem.h>
#include <LynxEncryption.h>

#ifdef __cplusplus
extern "C" {
#endif

LYNXCOREAPI LYNXBOOL		lynxInitCore(void);
LYNXCOREAPI LYNXBOOL		lynxReleaseCore(void);

#ifdef __cplusplus
}
#endif

#endif 