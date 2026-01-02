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
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXLISTENER_H__
#define __LYNXLISTENER_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	namespace SoundSystem 
	{
		class LYNXENGCLASS CListener : public CObj
		{
			LYNX_DECLARE_CLASS(CListener);
		private:			
		public:			
			CListener(CEngine* lpeng);
			~CListener();			

			LYNXBOOL						vCreate(void);	
		};	
	}
}

#endif