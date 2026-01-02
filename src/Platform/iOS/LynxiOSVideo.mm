//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import <Platform/iOS/VideoNotificationHandler.h>
#include <Platform/iOS/LynxiOSVideo.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#include <Platform/iOS/LynxViewController.h>

namespace LynxEngine 
{	
	namespace iOS
	{			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVideo::CVideo(LynxEngine::CEngine* lpeng)
		: LynxEngine::CVideo(lpeng)
		{
			m_MoviePlayer = nil;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVideo::~CVideo(void)
		{
			if (m_MoviePlayer)
			{
				[m_MoviePlayer view].hidden = TRUE;
				//[m_MoviePlayer release];
			}
			//if (m_NotificationHandler)
			//	[m_NotificationHandler release];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVideo::vCreate(const CCreationDesc* desc)
		{
			CVideoCreationDesc* lpDesc = (CVideoCreationDesc*)desc;
			LynxEngine::CString Filename;
			
			if (desc->m_FileName.c_str()[0] == _T('.') && desc->m_FileName.c_str()[1] == _T('.') && desc->m_FileName.c_str()[2] == _T('/'))
				Filename = &(desc->m_FileName.c_str()[3]);
			else
				Filename = desc->m_FileName;
			
			#ifdef _UNICODE
				NSString* Name = [[NSString alloc] initWithCString:(char*)(Filename.c_str()) encoding:NSUTF8StringEncoding]; 		
			#else
				NSString* Name = [[NSString alloc] initWithCString:Filename.c_str() encoding:NSASCIIStringEncoding]; 
			#endif
			
			NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: [[NSBundle mainBundle] pathForResource:Name ofType:@""]];

			m_MoviePlayer = [[MPMoviePlayerController alloc] initWithContentURL: fileURL];
			if (!m_MoviePlayer)
				return LYNX_FALSE;
			
			//[Name release];
			//[fileURL release];
			
			switch (lpDesc->m_ScalingMode)
			{
				case VSM_NONE:
					m_MoviePlayer.scalingMode = MPMovieScalingModeNone;
					break;
					
				default:
				case VSM_ASPECT_FIT:
					m_MoviePlayer.scalingMode = MPMovieScalingModeAspectFit;
					break;
					
				case VSM_ASPECT_FILL:
					m_MoviePlayer.scalingMode = MPMovieScalingModeAspectFill;
					break;
					
				case VSM_FILL:
					m_MoviePlayer.scalingMode = MPMovieScalingModeFill;
					break;
			}
			
			switch (lpDesc->m_UIMode)
			{
				case VUM_NONE:
					m_MoviePlayer.controlStyle = MPMovieControlStyleNone;
					break;
					
				default:
				case VUM_DEFAULT:
					m_MoviePlayer.controlStyle = MPMovieControlStyleFullscreen;
					break;
					
				case VUM_EMBEDDED:
					m_MoviePlayer.controlStyle = MPMovieControlStyleEmbedded;
					break;
			}
			m_MoviePlayer.shouldAutoplay = YES;
			m_MoviePlayer.repeatMode = MPMovieRepeatModeNone;
			
			[[m_MoviePlayer view] setFrame: [((GameFramework::iOS::CKernel*)(m_lpEngine->GetlpKernel()))->GetlpView() bounds]];
			[((GameFramework::iOS::CKernel*)(m_lpEngine->GetlpKernel()))->GetlpViewController().view addSubview: [m_MoviePlayer view]];
			
			m_NotificationHandler = [VideoNotificationHandler alloc];
			// Register for the playback finished notification
			[[NSNotificationCenter defaultCenter]
			  addObserver: m_NotificationHandler
			  selector: @selector(MovieFinishedCallback:)
			  name: MPMoviePlayerPlaybackDidFinishNotification
			  object: m_MoviePlayer];
			[m_NotificationHandler SetlpVideo: this];
			m_bFinished = LYNX_FALSE;
			 
			[m_MoviePlayer prepareToPlay];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVideo::vIsPlaying()
		{
			switch (m_MoviePlayer.playbackState)
			{
				default:
				case  MPMoviePlaybackStatePlaying:
					return LYNX_TRUE;
					
				case MPMoviePlaybackStatePaused:
				case MPMoviePlaybackStateStopped:
					return LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVideo::vPlay(void)
		{
			[m_MoviePlayer play];
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVideo::vPause(void)
		{
			[m_MoviePlayer pause];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVideo::vStop(void)
		{
			[m_MoviePlayer stop];
			m_bFinished = LYNX_TRUE;
			[UIApplication sharedApplication].statusBarHidden = YES;
		}
	}
}