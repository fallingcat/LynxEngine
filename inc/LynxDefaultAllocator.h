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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXDEFAULTALLOCATOR_H__
#define __LYNXDEFAULTALLOCATOR_H__

#include <LynxType.h>
#include <LynxMem.h>

#ifdef __USE_STL__
	namespace LynxEngine 
	{
		class CDefaultAllocator
		{			
			LYNXINLINE static char* malloc(const size_t bytes) { return new char[bytes]; }
			LYNXINLINE static void free(char * const block) { delete [] block; }
		};
	}
#endif
#endif
