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

#include <LynxEngine_PCH.h>

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>


@implementation lynxFBShareDelegate
@synthesize m_ShareType;

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results
{
    switch (m_ShareType)
    {
        case ST_LINK:
            glpKernel->vOnFinishFacebookShareLink("OnSuccess");
            break;
            
        case ST_PHOTO:
            glpKernel->vOnFinishFacebookUploadPic("OnSuccess");
            break;
        default:
            break;
    }
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error
{
    
    switch (m_ShareType)
    {
        case ST_LINK:
            glpKernel->vOnFinishFacebookShareLink("onError");
            break;
            
        case ST_PHOTO:
            glpKernel->vOnFinishFacebookUploadPic("onError");
            break;
        default:
            break;
    }
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer
{
    switch (m_ShareType)
    {
        case ST_LINK:
            glpKernel->vOnFinishFacebookShareLink("onCancel");
            break;
            
        case ST_PHOTO:
            glpKernel->vOnFinishFacebookUploadPic("onCancel");
            break;
        default:
            break;
    }

}
@end
