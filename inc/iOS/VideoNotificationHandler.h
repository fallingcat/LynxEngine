//
//  VideoNotificationHandler.h
//  LynxEngine
//
//  Created by owen on 2010/12/31.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <LynxEngineClasses.h>

@interface VideoNotificationHandler : NSObject 
{
	LynxEngine::CVideo*			m_lpVideo;
}

-(void) SetlpVideo: (LynxEngine::CVideo*) lpv;
-(void) MovieFinishedCallback: (NSNotification*) aNotification;

@end
