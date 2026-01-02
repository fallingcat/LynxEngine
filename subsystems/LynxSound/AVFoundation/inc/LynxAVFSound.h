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

#ifndef __LYNXAVFSOUND_H__
#define __LYNXAVFSOUND_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSound.h>

LYNX_FORWARD_DECLARATION(NSURL)
LYNX_FORWARD_DECLARATION(AVAudioPlayer)
LYNX_FORWARD_DECLARATION(AVAudioRecorder)

namespace LynxSound 
{
	namespace AVFoundation 
	{
        /*
        class LYNXENGCLASS CBuffer
		{
			LYNX_DECLARE_CLASS(CBuffer);
		private:
		protected:
			NSData*                     m_Data;
		public:
			CBuffer();
			CBuffer();
            
            NSData*                     GetData() const {return m_Data; }
		};
        */
        
		class LYNXENGCLASS CSound : public LynxEngine::SoundSystem::CSound
		{
			LYNX_DECLARE_CLASS(CSound);
		private:			
		protected:
			NSURL*						m_RecordedFileURL;
			AVAudioPlayer*				m_lpAudioPlayer;
			AVAudioRecorder*			m_lpAudioRecorder;
		public:			
			CSound(LynxEngine::CEngine* const lpengine);
			~CSound();			

			LYNXBOOL					vIsPlaying();
            void                        vInstance(const CResource& rhs);
			LYNXBOOL					vCreate(const LynxEngine::CCreationDesc* desc);	
			LYNXBOOL					vPlay(void);	
			LYNXBOOL					vPlay(float pitch);	
			void						vPause(void);	
			void						vStop(void);
			float						vGetVolume();
			void						vSetVolume(float v);
            void                        vSetLoops(int n);
			
			LYNXBOOL					vIsRecording();
			LYNXBOOL					vOpenRecorded();
			LYNXBOOL					vRecord(void);
			void						vPauseRecording(void);
			void						vStopRecording(void);
		protected:
		};
	}
}

#endif