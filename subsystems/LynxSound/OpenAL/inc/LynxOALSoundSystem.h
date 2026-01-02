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
//  Created by Owen Wu : 2006/06/20
//  Last Update :
//
//###########################################################################

#ifndef __LYNXOALSOUNDSYSTEM_H__
#define __LYNXOALSOUNDSYSTEM_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSoundSystem.h>
#ifdef __iOS__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined __ANDROID__
	#include <AL/al.h>
	#include <AL/alc.h>
#else
	#include <al.h>
	#include <alc.h>
#endif

namespace LynxSound 
{
	namespace OpenAL 
	{
		class CSound;
		class LYNXENGCLASS CSoundSystem : public LynxEngine::SoundSystem::CSoundSystem
		{
			LYNX_DECLARE_CLASS(CSoundSystem);
		private:			
		protected:
			ALCdevice*							m_Device;
			ALCcontext*							m_Context;
			LynxEngine::CArray<ALuint>			m_SourceArray;
		public:			
			CSoundSystem(LynxEngine::CEngine* const lpengine);
			~CSoundSystem();			

			LYNXINLINE const LYNXCHAR*			vGetAPIString() {return _T("OpenAL"); };

			LYNXBOOL							vCreate(void);			
			LYNXBOOL							vInit();

			void								vPause();
			void								vResume();

			LynxEngine::SoundSystem::CMusic*	vCreateMusic();
			LYNXBOOL							vCreateMusic(LynxEngine::SoundSystem::CMusicPtr& s);

			LYNXBOOL							vCreateSound(LynxEngine::SoundSystem::CSoundPtr& s);			
			LynxEngine::SoundSystem::CSound*	vCreateSound();			

			ALuint								GetFreeSource(OpenAL::CSound* s);
			void								ResetSoundSource(ALuint s);
		};

		#ifdef __iOS__
			OSStatus iOSAudioFileRead(void *inClientData, SInt64 inPosition, UInt32 requestCount, void *buffer, UInt32 *actualCount);
			OSStatus iOSAudioFileWrite(void *inClientData, SInt64 inPosition, UInt32 requestCount, const void *buffer, UInt32 *actualCount);
			SInt64 iOSAudioFileGetSize(void *inClientData);
			OSStatus iOSAudioFileSetSize(void  *inClientData, SInt64 inSize);
		#endif
	}
}

#endif