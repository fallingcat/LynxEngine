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

#include <SoundSystem/LynxSoundSystem.h>
#include <LynxAVFMusic.h>

namespace LynxSound 
{
	namespace AVFoundation 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::CMusic(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CMusic(lpengine)
		{				
			m_lpAudioPlayer = nil;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::~CMusic(void)
		{
			if (m_lpAudioPlayer)
				[m_lpAudioPlayer release];			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vCreate(const LynxEngine::CCreationDesc* desc)
		{
			LynxEngine::CSoundCreationDesc* lpDesc = (LynxEngine::CSoundCreationDesc*)desc;
			LynxEngine::CString Filename;
			LYNXCHAR FName[256], Ext[32];			
			
            if (desc->m_FileName.c_str()[0] == _T('.') && desc->m_FileName.c_str()[1] == _T('.') && desc->m_FileName.c_str()[2] == _T('/'))
                Filename = &(desc->m_FileName.c_str()[3]);
            else
                Filename = desc->m_FileName;
            
            LynxEngine::CPlatformFileStream Stream;
            static LYNXCHAR ExtList[][8] = { _T(".aac"),
                _T(".m4a"),
                _T(".caf"),
                _T(".mp3")};
            
            lynxSeparateFileExtByPath(FName, Ext, Filename.c_str());
            
            for (int i=0; i<4; i++)
            {
                Filename = LynxEngine::CString(FName) + LynxEngine::CString(ExtList[i]);
                if (Stream.vOpen(Filename.c_str(), LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
                {
                    Stream.vClose();
                    NSString* Name = [[NSString alloc] initWithCString:(char*)(Filename.c_str()) encoding:iOS_STRING_ENCODING];
                    NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: [[NSBundle mainBundle] pathForResource:Name ofType:@""]];
                
                    m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:nil];
                
                    [Name release];
                    [fileURL release];
                }
                if (m_lpAudioPlayer)
                    break;
            }
            
            if (!m_lpAudioPlayer)
                return LYNX_FALSE;
            
            vSetLoops(lpDesc->m_NumLoops);
            vSetVolume(lpDesc->m_Volume);
            [m_lpAudioPlayer prepareToPlay];
			
			if (desc->m_Name.empty())
				SetName(Filename);
			else
				SetName(desc->m_Name);
            
            if (desc->m_lpStream)
                desc->m_lpStream->vClose();
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vIsPlaying() 
		{
			return m_lpAudioPlayer.playing; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vPlay(void)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
            
            vSetVolume(m_Volume);
            
			return [m_lpAudioPlayer play];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vPause(void)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			[m_lpAudioPlayer pause];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vStop(void)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			[m_lpAudioPlayer stop];
            m_lpAudioPlayer.currentTime = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		float CMusic::vGetVolume()
		{
			//LYNX_ASSERT(m_lpAudioPlayer);		
			//return m_lpAudioPlayer.volume;
            
            return m_Volume;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetVolume(float v)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			m_Volume = v;
			float Volume = v * m_lpEngine->GetlpSoundSystem()->GetMusicVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsMusicEnabled()); 
			m_lpAudioPlayer.volume = Volume;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetLoops(int n)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			m_lpAudioPlayer.numberOfLoops = n;
		}        
	}
}