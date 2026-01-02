//
//  VideoNotificationHandler.m
//  LynxEngine
//
//  Created by owen on 2010/12/31.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <VideoNotificationHandler.h>
#import <MediaPlayer/MediaPlayer.h>

@implementation VideoNotificationHandler

// When the movie is done, release the controller.
-(void) MovieFinishedCallback: (NSNotification*) aNotification
{
    MPMoviePlayerController* theMovie = [aNotification object];
	
    [[NSNotificationCenter defaultCenter]
	 removeObserver: self
	 name: MPMoviePlayerPlaybackDidFinishNotification
	 object: theMovie];
	
    // Release the movie instance created in playMovieAtURL:
    [theMovie release];
}

@end
