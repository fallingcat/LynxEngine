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

#define URL_APPLE_LOOKUP        @"http://itunes.apple.com/%@/lookup"
//#define URL_IOS_APPSTORE        @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software"
#define URL_IOS_APPSTORE        @"https://itunes.apple.com/tw/app/apple-store/id%@?mt=8"
#define URL_IOS_APPSTORE_IOS7   @"itms-apps://itunes.apple.com/app/id%@"

#if !SHIPPING
    #define ENABLE_TEST_BUNDLEID    1
#else
    #define ENABLE_TEST_BUNDLEID    0
#endif

#define TEST_BUNDLEID           @"tw.com.skysoft.iPhone"

#define MAKE_JSON_STRING_VALUE(x) \
    LynxEngine::CJsonValue([(x) cStringUsingEncoding:NSUTF8StringEncoding])

#define __IPHONE_10_3    100300
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_3
#define SUPPORT_STOREREVIEWCONTROLLER_IOS_10_3   1
#import <StoreKit/SKStoreReviewController.h>
#else
#define SUPPORT_STOREREVIEWCONTROLLER_IOS_10_3   0
#endif

#if 0
    #define FDLOG(...) NSLog(__VA_ARGS__)
#else
    #define FDLOG(...) do{} while(0)   
#endif

@interface LynxFeedbackDialog()<UIAlertViewDelegate>
@property(nonatomic, strong) id alert;
@property(nonatomic, assign) NSInteger positiveIndex;
@property(nonatomic, assign) NSInteger negativeIndex;
@property(nonatomic, assign) NSInteger neutralIndex;
@end

@implementation LynxFeedbackDialog;
@synthesize appStoreID;
@synthesize bundleID;
@synthesize title;
@synthesize message;
@synthesize positiveText;
@synthesize negativeText;
@synthesize neutralText;
@synthesize positiveIndex;
@synthesize negativeIndex;
@synthesize neutralIndex;

+ (LynxFeedbackDialog*) sharedInstance
{
    static dispatch_once_t once;
    static LynxFeedbackDialog * s_sharedInstance = nil;
    dispatch_once(&once, ^{
        s_sharedInstance = [[LynxFeedbackDialog alloc] init];
        
    });
    return s_sharedInstance;
}

- (void) dealloc
{
    // s_sharedInstance = nil;
}

#pragma mark - internel method

- (void) fetchAppInfo
{
    @autoreleasepool
    {
#if ENABLE_TEST_BUNDLEID
    NSString* bundleIdentifier = TEST_BUNDLEID;
#else
    NSString* bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
#endif
    NSString* iTuneServiceURL = [NSString stringWithFormat:URL_APPLE_LOOKUP, self.countryCode];
    iTuneServiceURL = ([appStoreID length]>0)
        ? [iTuneServiceURL stringByAppendingFormat:@"?id=%@", appStoreID]
        : iTuneServiceURL = [iTuneServiceURL stringByAppendingFormat:@"?bundleId=%@", bundleIdentifier];

    FDLOG(@"fetchAppInfo lookup URL = %@", iTuneServiceURL);
    
    NSError* error = nil;
    NSURLResponse* response = nil;
    NSURL* URL = [NSURL URLWithString:iTuneServiceURL];
    NSURLRequest* request = [NSURLRequest requestWithURL:URL cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:30];
    NSData* data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    NSInteger statusCode = [(NSHTTPURLResponse*)response statusCode];
    if (data && 200 == statusCode)
    {
        NSString* jsonStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        const LYNXCHAR* jsonCString = [jsonStr cStringUsingEncoding:NSUTF8StringEncoding];
        
        LynxEngine::CJsonValue JsonValue;
        LynxEngine::CEngine::GetJSONValues(JsonValue, jsonCString);
        
        if (JsonValue.isMember("results") && JsonValue["results"].size()>0)
        {
            for (int i=0;i<JsonValue["results"].size();++i)
            {
                if (JsonValue["results"][i].isMember(_T("bundleId")))
                {
                    NSString* bundleIDFromJSON = [NSString stringWithCString:JsonValue["results"][i]["bundleId"].JSON_VALUE_AS_CSTRING() encoding:NSUTF8StringEncoding ];
                    
                    FDLOG(@"[bundleId from json, from app] = %@:%@", self.bundleID, [[NSBundle mainBundle] bundleIdentifier]);
                    
                    if ([bundleIDFromJSON isEqualToString:bundleIdentifier])
                    {
                        self.bundleID = bundleIDFromJSON;
                        if (JsonValue["results"][i].isMember(_T("trackId")))
                        {
                            appStoreID = [NSString stringWithFormat:@"%d", JsonValue["results"][i]["trackId"].asInt()];
                        }
                        break;
                    }
                }
            }
        }
    }
    else if (statusCode >= 400)
    {
        FDLOG(@"%@", [NSString stringWithFormat:@"fetchAppInfo failed. error code = %@", @(statusCode)]);
    }
    
    } // End of autorelease block
}

- (NSURL*) ratingURL
{
    float version = [[UIDevice currentDevice].systemVersion floatValue];
    NSString* appleStoreURL  = (version >= 7.0f && version < 7.1f)
      ? URL_IOS_APPSTORE_IOS7
      : URL_IOS_APPSTORE;
    
    return [NSURL URLWithString:[NSString stringWithFormat:appleStoreURL, self.appStoreID]];
}

- (void)show
{
    if (self.alert == nil)
    {
        UIViewController* top = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
        while(top.presentedViewController) {
            top = top.presentedViewController;
        }
        
        if ([UIAlertController class] && top)
        {
            FDLOG(@"Init FeedbackDialog with UIAlertController.");
            
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                           message:message
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            
            [alert addAction:[UIAlertAction actionWithTitle:positiveText
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(__unused UIAlertAction* action) {
                                                        [self dismissAlert:alert withButtonAtIndex:LynxEngine::iOS::BUTTON_POSITIVE];
                                                    }]];
            
            if ([neutralText length]>0)
            {
                [alert addAction:[UIAlertAction actionWithTitle:neutralText
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(__unused UIAlertAction* action) {
                                                        [self dismissAlert:alert withButtonAtIndex:LynxEngine::iOS::BUTTON_NEUTRAL];
                                                    }]];
            }
            
            [alert addAction:[UIAlertAction actionWithTitle:negativeText
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(__unused UIAlertAction* action) {
                                                        [self dismissAlert:alert withButtonAtIndex:LynxEngine::iOS::BUTTON_NEGATIVE];
                                                    }]];
            
            [top presentViewController:alert animated:YES completion:nil];
            
            self.alert = alert;
        }
        else
        {
            FDLOG(@"Init FeedbackDialog with UIAlertView.");
            
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle:title
                                                            message:message
                                                           delegate:(id<UIAlertViewDelegate>)self
                                                  cancelButtonTitle:nil
                                                  otherButtonTitles:nil];
            
            positiveIndex = [alert addButtonWithTitle:positiveText];
            if ([neutralText length]>0) { neutralIndex = [alert addButtonWithTitle:neutralText];}
            negativeIndex = [alert addButtonWithTitle:negativeText];
            [alert show];
            
            self.alert = alert;
        }
    } // End if alert != nil
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == positiveIndex)
        [self dismissAlert:alertView withButtonAtIndex:LynxEngine::iOS::BUTTON_POSITIVE];
    else if (buttonIndex == neutralIndex)
        [self dismissAlert:alertView withButtonAtIndex:LynxEngine::iOS::BUTTON_NEUTRAL];
    else if (buttonIndex == negativeIndex)
        [self dismissAlert:alertView withButtonAtIndex:LynxEngine::iOS::BUTTON_NEGATIVE];
}

- (void) dismissAlert:(__unused id)alertView withButtonAtIndex:(NSInteger)index
{
    int buttonIndex = (int)index;
    FDLOG(@"LynxFeedbackDialog::dismissAlert withButtonIndex : %d", buttonIndex);
    
    glpKernel->vOnClickRatingDialog(buttonIndex);

    if (LynxEngine::iOS::BUTTON_POSITIVE == buttonIndex)
    {
        if (!appStoreID)
        {
            [self fetchAppInfo];
        }
        
        FDLOG(@"LynxFeedbackDialog:: ready openURL, appStoreID = %@", self.appStoreID);
        
        if ([appStoreID length]>0)
        {
#if SUPPORT_STOREREVIEWCONTROLLER_IOS_10_3
        [SKStoreReviewController requestReview];
#else
        NSURL* reviewURL = [self ratingURL];
        FDLOG(@"ratingURL = %@", [reviewURL absoluteString]);
        [[UIApplication sharedApplication] openURL:reviewURL];
#endif
        }
        else{
            FDLOG(@"LynxFeedbackDialog:: failed openURL, appStoreID = %@", self.appStoreID);
        }
    }
    
    self.alert = nil;
}
@end
