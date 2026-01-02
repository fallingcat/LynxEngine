//
//  VideoNotificationHandler.m
//  LynxEngine
//
//  Created by owen on 2010/12/31.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Platform/iOS/VideoNotificationHandler.h>
#import <MediaPlayer/MediaPlayer.h>
#include <Platform/iOS/LynxiOSVideo.h>

@implementation VideoNotificationHandler

-(void) SetlpVideo: (LynxEngine::CVideo*) lpv
{
	m_lpVideo = lpv;
}

// When the movie is done, release the controller.
-(void) MovieFinishedCallback: (NSNotification*) aNotification
{
    MPMoviePlayerController* theMovie = [aNotification object];
	
    [[NSNotificationCenter defaultCenter]
	 removeObserver: self
	 name: MPMoviePlayerPlaybackDidFinishNotification
	 object: theMovie];
	
	m_lpVideo->SetFinished(LYNX_TRUE);
}

@end
