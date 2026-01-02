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

#ifndef __LYNXOSLSOUND_H__
#define __LYNXOSLSOUND_H__

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
		class CBuffer;	
		class LYNXENGCLASS CSound : public LynxEngine::SoundSystem::CSound
		{
			LYNX_DECLARE_CLASS(CSound);
		public:
			LYNXBOOL					m_bDelete;
		private:			
		protected:
            WAVEFORMATEXTENSIBLE		m_Format;

			SLObjectItf					m_PlayerObject;
			SLPlayItf					m_PlayerPlay;
			SLVolumeItf					m_PlayerVolume;
			SLAndroidSimpleBufferQueueItf m_PlayerBufferQueue;

			LynxEngine::CCountedPtr<CBuffer> m_Buffer;
			LYNXLONG					m_DataSize;
			//char*						m_BufferPointer;

			SLuint32					m_Channels;
			SLuint32					m_Frequency;//mHz
			SLuint32					m_BitsPerSample;
		public:			
			CSound(LynxEngine::CEngine* const lpengine);
			~CSound();			

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

			void						OnEndOfBuffer();
			static void					PlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context);
		};
	}
}

#endif