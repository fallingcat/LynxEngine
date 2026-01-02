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

#ifndef	__LYNXASSERT_H__
#define	__LYNXASSERT_H__

//#ifdef __WATCOM__
//#elif  __MSVC__

	//#include <crtdbg.h>

	#ifdef	_DEBUG

		#include <assert.h>

		#define		LYNX_ASSERT(f)						assert(f);	//_ASSERT(f)
	#else
		#define		LYNX_ASSERT(f)						;

	#endif

//#endif

#endif
