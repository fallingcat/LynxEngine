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

#ifndef __LYNXOALMUSIC_H__
#define __LYNXOALMUSIC_H__

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
		class LYNXENGCLASS CMusic : public LynxEngine::SoundSystem::CMusic
		{
			LYNX_DECLARE_CLASS(CMusic);
		private:			
		protected:
            WAVEFORMATEXTENSIBLE		m_WavFormat;
			ALuint						m_Buffer[2];
			ALuint						m_Source;  				

			LynxEngine::CStream*		m_Stream;
			OggVorbis_File				m_OGGFile;
			ALenum						m_Format;
			ALsizei						m_Freq;  
		public:			
			CMusic(LynxEngine::CEngine* const lpengine);
			~CMusic();			

			LYNXBOOL					vIsPlaying();
			LYNXBOOL					vCreate(const LynxEngine::CCreationDesc* desc);	
			LYNXBOOL					vPlay(void);	
			void						vPause(void);	
			void						vStop(void);
			float						vGetVolume();
			void						vSetVolume(float v);
			void						vSetLoops(int n);		

			LYNXBOOL					vUpdateOGG();
		protected:
			unsigned long				GetWaveALBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue);
            LYNXBOOL					LoadSoundWAV(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					OpenSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					StreamOGG(ALuint buffer);			
		};
	}
}

#endif