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

#ifndef __LYNXANDROIDCRASHDUMP_H__
#define __LYNXANDROIDCRASHDUMP_H__


namespace LynxEngine
{
	namespace Android
    {
		class LYNXENGCLASS CCrashDumpSystem // : public LynxEngine::CCrashDumpSystem
		{
			LYNX_DECLARE_CLASS(CCrashDumpSystem);
		private:
		protected:
		public:
			CCrashDumpSystem();
			~CCrashDumpSystem();

			static LYNXBOOL vInit(const LYNXCHAR* dumpfilepath);
			static LYNXBOOL ExecDumpLogcat(const LYNXCHAR* dumpfilepath, CString& dumpfilename);
		};
    }
}

#endif
