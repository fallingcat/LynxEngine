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

#ifndef __LYNXSTACK_H__
#define __LYNXSTACK_H__

#include <LynxType.h>
#include <LynxMem.h>
#include <stack>

namespace LynxEngine 
{
	#ifdef __USE_STL__
		template <class Type> class CStack : public std::stack<Type>		
		{						
		};		
		#define CIterator					iterator
	#else
	#endif
}

#endif