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

#ifndef __LYNXOALSOUND_H__
#define __LYNXOALSOUND_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSound.h>
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

typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );

namespace LynxSound
{
	namespace OpenAL 
	{
		class CBuffer;	
		class LYNXENGCLASS CSound : public LynxEngine::SoundSystem::CSound
		{
			LYNX_DECLARE_CLASS(CSound);
		private:			
		protected:
            WAVEFORMATEXTENSIBLE		m_Format;
            LynxEngine::CCountedPtr<CBuffer> m_Buffer;
			ALuint						m_Source;  	
		public:			
			CSound(LynxEngine::CEngine* const lpengine);
			~CSound();			

			LYNXFORCEINLINE ALuint		GetSource() { return m_Source; }
			LYNXFORCEINLINE void		SetSource(ALuint s) { m_Source = s; }

			LYNXBOOL					vIsPlaying();
			void						vInstance(const CResource& rhs);
			LYNXBOOL					vCreate(const LynxEngine::CCreationDesc* desc);	
			LYNXBOOL					vPlay(void);				
			LYNXBOOL					vPlay(float pitch);	
			void						vPause(void);	
			void						vStop(void);
			float						vGetVolume();
			void						vSetVolume(float v);
			void						vSetLoops(int n);
			
			LYNXBOOL					vIsRecording();
			LYNXBOOL					vOpenRecorded();
			LYNXBOOL					vRecord(void);
			void						vPauseRecording(void);
			void						vStopRecording(void);		
		protected:
            unsigned long				GetWaveALBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue);
            LYNXBOOL					LoadSoundWAV(const LynxEngine::CString& filename, const LynxEngine::CSoundCreationDesc* desc);
			LYNXBOOL					LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CSoundCreationDesc* desc);
		};
	}
}

#endif