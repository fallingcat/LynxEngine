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
//  Created by Kasaki Tzeng : 2016/07/18
//  Last Update :
//
//###########################################################################

#ifndef __LYNXIOSCCRASHDUMP_H__
#define __LYNXIOSCCRASHDUMP_H__

namespace LynxEngine
{
    namespace iOS
    {
        class LYNXENGCLASS CCrashDumpSystem
        {
            LYNX_DECLARE_CLASS(CCrashDumpSystem);
            
        public:
            CCrashDumpSystem() {}
            virtual ~CCrashDumpSystem() {}
            
            static LYNXBOOL vInit(const LYNXCHAR* dumpfilepath);
            
        private:
            static void ConfigCrashDumpSetting();
        };
    }
}

#endif // __LYNXIOSCCRASHDUMP_H__
