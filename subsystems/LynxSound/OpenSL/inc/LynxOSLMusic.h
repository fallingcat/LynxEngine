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

#ifndef __LYNXOSLMUSIC_H__
#define __LYNXOSLMUSIC_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSound.h>
#if defined __ANDROID__
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#else
#endif

typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );

namespace LynxSound
{
	namespace OpenSL 
	{
		class LYNXENGCLASS CMusic : public LynxEngine::SoundSystem::CMusic
		{
			LYNX_DECLARE_CLASS(CMusic);
		public:
			LYNXBOOL					m_bDelete;
		private:			
		protected:
            WAVEFORMATEXTENSIBLE		m_WavFormat; 				

			LynxEngine::CStream*		m_Stream;
			OggVorbis_File				m_OGGFile;

			SLObjectItf					m_PlayerObject;
			SLPlayItf					m_PlayerPlay;
			SLVolumeItf					m_PlayerVolume;
			SLAndroidSimpleBufferQueueItf m_PlayerBufferQueue;

			char*						m_Buffer;
			LYNXLONG					m_DataSize;
			//char*						m_BufferPointer;

			SLuint32					m_Channels;
			SLuint32					m_Frequency;//mHz
			SLuint32					m_BitsPerSample;
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
			//void						EnqueueBuffer(LYNXBOOL breset);
		protected:
			unsigned long				GetWaveALBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue);
            LYNXBOOL					LoadSoundWAV(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					OpenSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc);
			LYNXBOOL					StreamOGG(unsigned int buffer);		

			void						OnEndOfBuffer();
			static void					PlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context);
		};
	}
}

#endif