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

#ifndef __LYNXIOSFACEBOOKSHAREDIALOGDELEGATE_H__
#define __LYNXIOSFACEBOOKSHAREDIALOGDELEGATE_H__

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>


typedef enum
{
    ST_LINK = 0,
    ST_PHOTO
} FBSHARETYPE;


@interface lynxFBShareDelegate : NSObject <FBSDKSharingDelegate> {
    
    FBSHARETYPE m_ShareType;
}

@property (nonatomic) FBSHARETYPE m_ShareType;

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results;
- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error;
- (void)sharerDidCancel:(id<FBSDKSharing>)sharer;
@end


#endif /* __LYNXIOSFACEBOOKSHAREDIALOGDELEGATE_H__ */