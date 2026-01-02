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
//  ª°©˙:
//
//###########################################################################

#include <LynxEngine_PCH.h>

#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

#include <LynxAVFSoundSystem.h>
#include <LynxAVFMusic.h>
#include <LynxAVFSound.h>

namespace LynxSound 
{
	namespace AVFoundation 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::CSoundSystem(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CSoundSystem(lpengine)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::~CSoundSystem(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreate()
		{
			LynxEngine::CSoundCreationDesc Desc;
			
			[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback error: nil];
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vInit()
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::SoundSystem::CMusic* CSoundSystem::vCreateMusic()
		{
			return LYNXNEW CMusic(m_lpEngine);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreateMusic(LynxEngine::SoundSystem::CMusicPtr& s)
		{
			m_lpEngine->CreateResource(s, LynxEngine::MUSIC);
			m_MusicList.push_back(s);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::SoundSystem::CSound* CSoundSystem::vCreateSound()
		{
			return LYNXNEW CSound(m_lpEngine);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreateSound(LynxEngine::SoundSystem::CSoundPtr& s)
		{
			m_lpEngine->CreateResource(s, LynxEngine::SOUND);
			m_SoundList.push_back(s);
			
			return LYNX_TRUE;
		}        
	}
}