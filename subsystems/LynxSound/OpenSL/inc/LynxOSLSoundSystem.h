//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Shayne Wei : 2016/03/28
//  Last Update :
//
//###########################################################################

#ifndef __LYNXOSLSOUNDSYSTEM_H__
#define __LYNXOSLSOUNDSYSTEM_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSoundSystem.h>
#if defined __ANDROID__
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#else
#endif

namespace LynxSound 
{
	namespace OpenSL 
	{
		class LYNXENGCLASS CSoundSystem : public LynxEngine::SoundSystem::CSoundSystem
		{
			LYNX_DECLARE_CLASS(CSoundSystem);
		public:
			SLObjectItf							m_EngineObject;
			SLEngineItf							m_EngineEngine;
			SLObjectItf							m_OutputMixObject;
			LynxEngine::CArray<SLObjectItf>		m_PlayerObjects;
			LYNXBOOL*							m_PlayStateMap;
		private:			
		protected: 
		public:			
			CSoundSystem(LynxEngine::CEngine* const lpengine);
			~CSoundSystem();			

			LYNXINLINE const LYNXCHAR*			vGetAPIString() {return _T("OpenSL"); };

			LYNXBOOL							vCreate();			
			LYNXBOOL							vInit();

			void								vPause();
			void								vResume();

			LynxEngine::SoundSystem::CMusic*	vCreateMusic();
			LYNXBOOL							vCreateMusic(LynxEngine::SoundSystem::CMusicPtr& s);

			LYNXBOOL							vCreateSound(LynxEngine::SoundSystem::CSoundPtr& s);			
			LynxEngine::SoundSystem::CSound*	vCreateSound();

			void								DeleteMusic(const LynxEngine::SoundSystem::CMusic* music);
			void								DeleteMusic(const LynxEngine::SoundSystem::CMusicPtr& music);
			void								DeleteMusic(const LynxEngine::CString& name);
			void								DeleteSound(const LynxEngine::SoundSystem::CSound* sound);
			void								DeleteSound(const LynxEngine::SoundSystem::CSoundPtr& sound);
			void								DeleteSound(const LynxEngine::CString& name);

			SLObjectItf							GetFreeAudioPlayer(int frequency, int channels);
			void								DestroyAudioPlayer(SLObjectItf PlayerObject);
		};
	}
}

#endif