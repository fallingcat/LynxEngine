//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/
//
//  LynxEngine Ver 1.00
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Kasaki Tzeng : 2016/07/06
//  Last Update :
//--------------------------------------------------------------------------
//  ?°?˙:
//
//###########################################################################

#ifndef __LYNXANDROIDSTACKUNWIND_H__
#define __LYNXANDROIDSTACKUNWIND_H__

#include <LynxEngineClasses.h>

namespace LynxEngine
{
	namespace Android
    {
		typedef std::function<void(const LYNXCHAR* logStr)> OUTPUT_DELEGATE;
		class LYNXENGCLASS CAndroidStackUnwind
		{
			LYNX_DECLARE_CLASS(CAndroidStackUnwind);

		public:			
			CAndroidStackUnwind();
			~CAndroidStackUnwind();			
			
			static void DumpStack(OUTPUT_DELEGATE out_delegate = std::bind(&CAndroidStackUnwind::DefaultLogCatOutput, std::placeholders::_1));
			static void DefaultLogCatOutput(const LYNXCHAR* logStr);
		};
    }
}

#endif // __LYNXANDROIDSTACKUNWIND_H__
