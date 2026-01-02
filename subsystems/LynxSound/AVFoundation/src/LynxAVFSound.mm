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
#include <LynxAVFSound.h>

namespace LynxSound 
{
	namespace AVFoundation 
	{
        //-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CSound::CSound(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CSound(lpengine)
		{				
			m_RecordedFileURL = nil;
			m_lpAudioPlayer = nil;
			m_lpAudioRecorder = nil;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		CSound::~CSound(void)
		{
			if (m_RecordedFileURL)
				[m_RecordedFileURL release];
			
            if (m_lpAudioPlayer)
                [m_lpAudioPlayer release];
            
			if (m_lpAudioRecorder)
				[m_lpAudioRecorder release];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CSound::vInstance(const CResource& rhs)
		{
			CResource::vInstance(rhs);
            
			CSound* RHS = (CSound*)(&rhs);
            
            //m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:RHS->m_lpAudioPlayer.url error:nil];
            m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithData:RHS->m_lpAudioPlayer.data error:nil];
            
			// Setup attributes
			vSetLoops(RHS->m_NumLoops);
			vSetVolume(RHS->m_Volume);
            
            [m_lpAudioPlayer prepareToPlay];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vCreate(const LynxEngine::CCreationDesc* desc)
		{
			LynxEngine::CSoundCreationDesc* lpDesc = (LynxEngine::CSoundCreationDesc*)desc;
			LynxEngine::CString Filename;
			LYNXCHAR FName[256], Ext[32];			
			
			if (lpDesc->m_bRecording)
			{
				NSString *TempDir = NSTemporaryDirectory ();
				NSString* Name = [[NSString alloc] initWithCString:(const char*)desc->m_Name.c_str() encoding:iOS_STRING_ENCODING];
				NSString *SoundFilePath =
                [TempDir stringByAppendingString: Name];
				[Name release];
				
				if (m_RecordedFileURL)
					[m_RecordedFileURL release];
				m_RecordedFileURL= [[NSURL alloc] initFileURLWithPath: SoundFilePath];
		
				AVAudioSession *AudioSession = [AVAudioSession sharedInstance];
				//AudioSession.delegate = self;
				[AudioSession setActive: YES error: nil];
			}
			else 
			{
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
                        NSData* SoundData = [NSData dataWithContentsOfURL:fileURL];
                        if (SoundData)
                        {
                            m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithData:SoundData error:nil];
                        }
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
			}
			
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
		LYNXBOOL CSound::vOpenRecorded()
		{
			m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:m_RecordedFileURL error:nil];	
			if (m_RecordedFileURL)
				[m_RecordedFileURL release];
			
			if (!m_lpAudioPlayer)
				return LYNX_FALSE;
			
			m_lpAudioPlayer.numberOfLoops = 0;
			m_lpAudioPlayer.volume = 1.0f;
			[m_lpAudioPlayer prepareToPlay];
		
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vIsPlaying() 
		{
			return m_lpAudioPlayer.playing; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vPlay(void)
		{
            //return 1;
            
			LYNX_ASSERT(m_lpAudioPlayer);
            
            vSetVolume(m_Volume);
            
			return [m_lpAudioPlayer play];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vPause(void)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			[m_lpAudioPlayer pause];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vStop(void)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			[m_lpAudioPlayer stop];
            m_lpAudioPlayer.currentTime = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		float CSound::vGetVolume()
		{
			//LYNX_ASSERT(m_lpAudioPlayer);			
			//return m_lpAudioPlayer.volume;
            
            return m_Volume;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vSetVolume(float v)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
			m_Volume = v;
			float Volume = v * m_lpEngine->GetlpSoundSystem()->GetSoundVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsSoundEnabled()); 
			m_lpAudioPlayer.volume = Volume;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vSetLoops(int n)
		{
			LYNX_ASSERT(m_lpAudioPlayer);
			
            m_NumLoops = n;
			m_lpAudioPlayer.numberOfLoops = n;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vIsRecording() 
		{
			return m_lpAudioRecorder.isRecording; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vRecord(void)
		{
			[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryRecord error: nil];
			
			NSDictionary *RecordSettings =
			[[NSDictionary alloc] initWithObjectsAndKeys:
			 [NSNumber numberWithFloat: 44100.0], AVSampleRateKey,
             [NSNumber numberWithInt: kAudioFormatAppleLossless], AVFormatIDKey,
             [NSNumber numberWithInt: 1], AVNumberOfChannelsKey,
             [NSNumber numberWithInt: AVAudioQualityMax],
			 AVEncoderAudioQualityKey,
             nil];
			
			m_lpAudioRecorder = 
            [[AVAudioRecorder alloc] initWithURL: m_RecordedFileURL
                                        settings: RecordSettings
                                           error: nil];
			[RecordSettings release];
			
			if (!m_lpAudioRecorder)
				return LYNX_FALSE;
			
			[m_lpAudioRecorder prepareToRecord];
			[m_lpAudioRecorder record];
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vPauseRecording(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vStopRecording(void)
		{
			LYNX_ASSERT(m_lpAudioRecorder);
			
			[m_lpAudioRecorder stop];
			
			[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback error: nil];
		}
	}
}