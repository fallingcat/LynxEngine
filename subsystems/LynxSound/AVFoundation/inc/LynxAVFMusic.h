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

#ifndef __LYNXAVFMUSIC_H__
#define __LYNXAVFMUSIC_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxMusic.h>

LYNX_FORWARD_DECLARATION(AVAudioPlayer)

namespace LynxSound 
{
	namespace AVFoundation 
	{
		class LYNXENGCLASS CMusic : public LynxEngine::SoundSystem::CMusic
		{
			LYNX_DECLARE_CLASS(CMusic);
		private:			
		protected:
			AVAudioPlayer*				m_lpAudioPlayer;			
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
            void                        vSetLoops(int n);
		protected:
		};
	}
}

#endif