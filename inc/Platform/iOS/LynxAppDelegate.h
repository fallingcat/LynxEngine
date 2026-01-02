//
//  Stage2AppDelegate.h
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define __WITHOUT_XIB__ 1

@class LynxViewController;

@interface LynxAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    LynxViewController *viewController;
}

#ifndef __WITHOUT_XIB__
    @property (nonatomic, retain) IBOutlet UIWindow *window;
    @property (nonatomic, retain) IBOutlet LynxViewController *viewController;
#endif

//- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

@end

