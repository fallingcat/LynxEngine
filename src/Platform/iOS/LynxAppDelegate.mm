//
//  Stage2AppDelegate.m
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include <LynxEngine_PCH.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <LynxEngine.h>
#include <LynxDebugMemAllocator.h>
#include <Platform/iOS/LynxiOSConsoleLogger.h>
#include <Platform/iOS/LynxiOSConsoleOutputDevice.h>
#import <UIKit/UIApplication.h>
#import <LynxAppDelegate.h>

@implementation LynxAppDelegate

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [viewController.view Activate:LYNX_TRUE];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [viewController.view Activate:LYNX_FALSE];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [viewController.view Activate:LYNX_FALSE];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
}

- (void)dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end
