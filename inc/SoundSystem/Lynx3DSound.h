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

#ifndef __LYNX3DSOUND_H__
#define __LYNX3DSOUND_H__

#include <LynxEngineClasses.h>
#include <LynxSound.h>

namespace LynxEngine 
{
	namespace SoundSystem 
	{
		class LYNXENGCLASS C3DSound : public CSound
		{
			LYNX_DECLARE_CLASS(C3DSound);
		private:			
		public:			
			C3DSound(CEngine* lpeng);
			~C3DSound();			

			LYNXBOOL						vCreate(void);	
		};	
	}
}

#endif