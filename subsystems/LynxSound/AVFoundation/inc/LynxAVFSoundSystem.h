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

#ifndef __LYNXAVFSOUNDSYSTEM_H__
#define __LYNXAVFSOUNDSYSTEM_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSoundSystem.h>

namespace LynxSound 
{
	namespace AVFoundation 
	{
		class LYNXENGCLASS CSoundSystem : public LynxEngine::SoundSystem::CSoundSystem
		{
			LYNX_DECLARE_CLASS(CSoundSystem);
		private:			
		protected:
		public:			
			CSoundSystem(LynxEngine::CEngine* const lpengine);
			~CSoundSystem();			

            const LYNXCHAR*                     vGetAPIString() {return _T("AV Foundation"); }

			LYNXBOOL							vCreate(void);			
			LYNXBOOL							vInit();

			LYNXBOOL							vCreateMusic(LynxEngine::SoundSystem::CMusicPtr& s);						
			LynxEngine::SoundSystem::CMusic*	vCreateMusic();
			

			LYNXBOOL							vCreateSound(LynxEngine::SoundSystem::CSoundPtr& s);
			LynxEngine::SoundSystem::CSound*	vCreateSound();            
		};
	}
}

#endif