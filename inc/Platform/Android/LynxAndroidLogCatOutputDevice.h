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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANDROIDLOGCATOUTPUTEVICE_H__
#define __LYNXANDROIDLOGCATOUTPUTEVICE_H__

#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	namespace Android
	{
		class LYNXENGCLASS CLogCatOutputDevice : public COutputDevice
		{	
			LYNX_DECLARE_CLASS(CLogCatOutputDevice);
		public:			
		protected:									
		public:		
			CLogCatOutputDevice();
			virtual ~CLogCatOutputDevice();		

			virtual void			vOpen(const CString& name);
			virtual void			vClose();
			virtual void			vClear();
			virtual void			vAppend(const CString& str);
			virtual void			vOutput(const CString& str);			
		};		
	}
}

#endif