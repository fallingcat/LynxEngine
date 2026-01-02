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
//  Created by Kasaki Tzeng : 2016/07/18
//  Last Update :
//
//###########################################################################

#ifndef __LYNXIOSFEEDBACKDIALOG_H__
#define __LYNXIOSFEEDBACKDIALOG_H__

#import <Foundation/Foundation.h>

namespace LynxEngine
{
    namespace iOS
    {
        typedef enum
        {
            BUTTON_POSITIVE = -1,
            BUTTON_NEGATIVE = -2,
            BUTTON_NEUTRAL = -3,
        } DIALOGBUTTONINDEX;
    }
}

@interface LynxFeedbackDialog : NSObject {
}

// @property(nonatomic, assign) NSUInteger appStoreID;
@property(nonatomic, copy) NSString* appStoreID;
@property(nonatomic, copy) NSString* bundleID;
@property(nonatomic, copy) NSString* countryCode;
@property(nonatomic, copy) NSString* title;
@property(nonatomic, copy) NSString* message;
@property(nonatomic, copy) NSString* positiveText;
@property(nonatomic, copy) NSString* negativeText;
@property(nonatomic, copy) NSString* neutralText;

+ (LynxFeedbackDialog*) sharedInstance;
- (void) fetchAppInfo;
- (NSURL*) ratingURL;
- (void) show;
- (void) dismissAlert:(__unused id)alertView withButtonAtIndex:(NSInteger)index;

@end

#endif /* __LYNXIOSFEEDBACKDIALOG_H__ */
