//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/
//
//  LynxEngine Ver 1.00
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update :
//--------------------------------------------------------------------------
//  ª°©?:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <sys/sysctl.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <GameFramework/LynxUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <InputSystem/LynxInputSystem.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#include <Platform/iOS/LynxiOSEngine.h>
#import <Platform/iOS/LynxView.h>
#import <Platform/iOS/LynxViewController.h>
#import <Platform/iOS/LynxBrowserViewController.h>

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

#import <FirebaseInstanceID/FirebaseInstanceID.h>
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
    #define SUPPORT_NOTIFICATION_IOS_10_0   1
    #import <UserNotifications/UserNotifications.h>
#else
    #define SUPPORT_NOTIFICATION_IOS_10_0   0
#endif

#import <CoreTelephony/CTCarrier.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <SystemConfiguration/SystemConfiguration.h> 

namespace LynxEngine
{
    namespace GameFramework
    {
        namespace iOS
        {
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            CKernel::CKernel(void)
            {
                m_bGestureRecognization = LYNX_TRUE;
                m_MotionManager = nil;
                m_LocationManager = nil;
                m_bCameraCapture = LYNX_FALSE;
                m_CameraCaptureRawData = NULL;
                m_GestureState = 0;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            CKernel::~CKernel(void)
            {
                //[m_MotionManager release];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            CEngine* CKernel::vCreateEngine()
            {
                CEngine* lpEngine = LYNXNEW LynxEngine::iOS::CEngine(this);
                
                return  lpEngine;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetupDeviceType()
            {
                NSString *DeviceModel = nil;
                size_t size = 1;
                
                m_DeviceType = DT_IPOD_4;
                
                sysctlbyname("hw.machine", NULL, &size, NULL, 0);
                if (size > 0)
                {
                    char* DeviceName = LYNXNEW char[size+1];
                    sysctlbyname("hw.machine", DeviceName, &size, NULL, 0);
                    lynxcString2NSString(&DeviceModel, DeviceName);
                    
                    if ([DeviceModel isEqual:@"iPhone1,1"]) m_DeviceType = DT_IPHONE_1;          //iPhone 1G
                    else if ([DeviceModel isEqual:@"iPhone1,2"]) m_DeviceType = DT_IPHONE_1;     //iPhone 3G
                    else if ([DeviceModel isEqual:@"iPhone2,1"]) m_DeviceType = DT_IPHONE_2;     //iPhone 3GS
                    else if ([DeviceModel isEqual:@"iPhone3,1"]) m_DeviceType = DT_IPHONE_3;     //iPhone 4 - AT&T
                    else if ([DeviceModel isEqual:@"iPhone3,2"]) m_DeviceType = DT_IPHONE_3;     //iPhone 4 - Other carrier
                    else if ([DeviceModel isEqual:@"iPhone3,3"]) m_DeviceType = DT_IPHONE_3;     //iPhone 4 - Other carrier
                    else if ([DeviceModel isEqual:@"iPhone4,1"]) m_DeviceType = DT_IPHONE_4;     //iPhone 4S
                    else if ([DeviceModel isEqual:@"iPhone5,1"]) m_DeviceType = DT_IPHONE_5;     //iPhone 5
                    else if ([DeviceModel isEqual:@"iPhone5,2"]) m_DeviceType = DT_IPHONE_5;     //iPhone 5
                    else if ([DeviceModel isEqual:@"iPhone5,3"]) m_DeviceType = DT_IPHONE_5;     //iPhone 5C
                    else if ([DeviceModel isEqual:@"iPhone5,4"]) m_DeviceType = DT_IPHONE_5;     //iPhone 5C
                    else if ([DeviceModel isEqual:@"iPhone6,1"]) m_DeviceType = DT_IPHONE_6;     //iPhone 5S
                    else if ([DeviceModel isEqual:@"iPhone6,2"]) m_DeviceType = DT_IPHONE_6;     //iPhone 5S
                    else if ([DeviceModel isEqual:@"iPhone7,1"]) m_DeviceType = DT_IPHONE_7;     //iPhone 6 Plus (A1522/A1524)
                    else if ([DeviceModel isEqual:@"iPhone7,2"]) m_DeviceType = DT_IPHONE_7;	 //iPhone 6 (A1549/A1586)
                    
                    else if ([DeviceModel isEqual:@"iPod1,1"])   m_DeviceType = DT_IPOD_1;       //iPod Touch 1G
                    else if ([DeviceModel isEqual:@"iPod2,1"])   m_DeviceType = DT_IPOD_2;       //iPod Touch 2G
                    else if ([DeviceModel isEqual:@"iPod3,1"])   m_DeviceType = DT_IPOD_3;       //iPod Touch 3G
                    else if ([DeviceModel isEqual:@"iPod4,1"])   m_DeviceType = DT_IPOD_4;       //iPod Touch 4G
                    else if ([DeviceModel isEqual:@"iPod5,1"])   m_DeviceType = DT_IPOD_5;       //iPod Touch 5G
                    
                    else if ([DeviceModel isEqual:@"iPad1,1"])   m_DeviceType = DT_IPAD_1;       //iPad Wifi
                    else if ([DeviceModel isEqual:@"iPad1,2"])   m_DeviceType = DT_IPAD_1;       //iPad 3G
                    else if ([DeviceModel isEqual:@"iPad2,1"])   m_DeviceType = DT_IPAD_2;       //iPad 2 (WiFi)
                    else if ([DeviceModel isEqual:@"iPad2,2"])   m_DeviceType = DT_IPAD_2;       //iPad 2 (GSM)
                    else if ([DeviceModel isEqual:@"iPad2,3"])   m_DeviceType = DT_IPAD_2;       //iPad 2 (CDMA)
                    else if ([DeviceModel isEqual:@"iPad2,5"])   m_DeviceType = DT_IPAD_2;       //iPad Mini
                    else if ([DeviceModel isEqual:@"iPad2,6"])   m_DeviceType = DT_IPAD_2;       //iPad Mini
                    else if ([DeviceModel isEqual:@"iPad2,7"])   m_DeviceType = DT_IPAD_2;       //iPad Mini
                    else if ([DeviceModel isEqual:@"iPad3,1"])   m_DeviceType = DT_IPAD_3;       //iPad 3 (Wifi)
                    else if ([DeviceModel isEqual:@"iPad3,5"])   m_DeviceType = DT_IPAD_3;       //iPad 4
                    else if ([DeviceModel isEqual:@"iPad3,6"])   m_DeviceType = DT_IPAD_3;       //iPad 4
                    else if ([DeviceModel isEqual:@"iPad4,1"])   m_DeviceType = DT_IPAD_4;       //iPad Air
                    else if ([DeviceModel isEqual:@"iPad4,2"])   m_DeviceType = DT_IPAD_4;       //iPad Air
                    else if ([DeviceModel isEqual:@"iPad4,4"])   m_DeviceType = DT_IPAD_4;       //iPad mini 2G
                    else if ([DeviceModel isEqual:@"iPad4,5"])   m_DeviceType = DT_IPAD_4;       //iPad mini 2G
                    else if ([DeviceModel isEqual:@"iPad4,6"])   m_DeviceType = DT_IPAD_4;       //iPad mini 2G
                    else if ([DeviceModel isEqual:@"iPad4,7"])   m_DeviceType = DT_IPAD_4;       //iPad mini 3G
                    else if ([DeviceModel isEqual:@"iPad4,8"])   m_DeviceType = DT_IPAD_4;       //iPad mini 3G
                    else if ([DeviceModel isEqual:@"iPad4,9"])   m_DeviceType = DT_IPAD_4;       //iPad mini 3G
                    else if ([DeviceModel isEqual:@"iPad5,3"])   m_DeviceType = DT_IPAD_4;       //iPad Air 2
                    else if ([DeviceModel isEqual:@"iPad5,4"])   m_DeviceType = DT_IPAD_4;       //iPad Air 2
                    
                    else if ([DeviceModel hasPrefix:@"iPhone1"]) m_DeviceType = DT_IPHONE_1;
                    else if ([DeviceModel hasPrefix:@"iPhone2"]) m_DeviceType = DT_IPHONE_2;
                    else if ([DeviceModel hasPrefix:@"iPhone3"]) m_DeviceType = DT_IPHONE_3;
                    else if ([DeviceModel hasPrefix:@"iPhone4"]) m_DeviceType = DT_IPHONE_4;
                    else if ([DeviceModel hasPrefix:@"iPhone5"]) m_DeviceType = DT_IPHONE_5;
                    else if ([DeviceModel hasPrefix:@"iPhone6"]) m_DeviceType = DT_IPHONE_6;
                    else if ([DeviceModel hasPrefix:@"iPhone7"]) m_DeviceType = DT_IPHONE_7;
                    else if ([DeviceModel hasPrefix:@"iPhone8"]) m_DeviceType = DT_IPHONE_8;
                    else if ([DeviceModel hasPrefix:@"iPhone9"]) m_DeviceType = DT_IPHONE_9;
                    else if ([DeviceModel hasPrefix:@"iPod1"]) m_DeviceType = DT_IPOD_1;
                    else if ([DeviceModel hasPrefix:@"iPod2"]) m_DeviceType = DT_IPOD_2;
                    else if ([DeviceModel hasPrefix:@"iPod3"]) m_DeviceType = DT_IPOD_3;
                    else if ([DeviceModel hasPrefix:@"iPod4"]) m_DeviceType = DT_IPOD_4;
                    else if ([DeviceModel hasPrefix:@"iPod5"]) m_DeviceType = DT_IPOD_5;
                    else if ([DeviceModel hasPrefix:@"iPad1"]) m_DeviceType = DT_IPAD_1;
                    else if ([DeviceModel hasPrefix:@"iPad2"]) m_DeviceType = DT_IPAD_2;
                    else if ([DeviceModel hasPrefix:@"iPad3"]) m_DeviceType = DT_IPAD_3;
                    else if ([DeviceModel hasPrefix:@"iPad4"]) m_DeviceType = DT_IPAD_4;
                    else if ([DeviceModel hasPrefix:@"iPad5"]) m_DeviceType = DT_IPAD_5;
                    else if ([DeviceModel hasPrefix:@"iPad6"]) m_DeviceType = DT_IPAD_6;
                    else if ([DeviceModel hasPrefix:@"iPad7"]) m_DeviceType = DT_IPAD_7;
                    else if ([DeviceModel hasPrefix:@"iPad8"]) m_DeviceType = DT_IPAD_8;
                    else m_DeviceType = DT_IPHONE_5;
                    
                    m_DeviceManufacturer = "Apple";
                    
                    m_DeviceModel = DeviceName;
                    int Dotfind = (int)m_DeviceModel.find(",");
                    if (Dotfind != CString::npos)
                    {
                        m_DeviceModel = m_DeviceModel.substr(0, Dotfind);
                    }
                    
                    
                    m_DeviceProduct = DeviceName;
                    // 產品名預設.
                    //iPhone.
                    if ([DeviceModel isEqualToString:@"iPhone1,1"])    m_DeviceModel = "iPhone 1G";
                    else if ([DeviceModel isEqualToString:@"iPhone1,2"])    m_DeviceModel = "iPhone 3G";
                    else if ([DeviceModel isEqualToString:@"iPhone2,1"])    m_DeviceModel = "iPhone 3GS";
                    else if ([DeviceModel isEqualToString:@"iPhone3,1"])    m_DeviceModel = "iPhone 4";
                    else if ([DeviceModel isEqualToString:@"iPhone3,2"])    m_DeviceModel = "Verizon iPhone 4";
                    else if ([DeviceModel isEqualToString:@"iPhone4,1"])    m_DeviceModel = "iPhone 4S";
                    else if ([DeviceModel isEqualToString:@"iPhone5,1"])    m_DeviceModel = "iPhone 5";
                    else if ([DeviceModel isEqualToString:@"iPhone5,2"])    m_DeviceModel = "iPhone 5";
                    else if ([DeviceModel isEqualToString:@"iPhone5,3"])    m_DeviceModel = "iPhone 5C";
                    else if ([DeviceModel isEqualToString:@"iPhone5,4"])    m_DeviceModel = "iPhone 5C";
                    else if ([DeviceModel isEqualToString:@"iPhone6,1"])    m_DeviceModel = "iPhone 5S";
                    else if ([DeviceModel isEqualToString:@"iPhone6,2"])    m_DeviceModel = "iPhone 5S";
                    else if ([DeviceModel isEqualToString:@"iPhone7,1"])    m_DeviceModel = "iPhone 6 Plus";
                    else if ([DeviceModel isEqualToString:@"iPhone7,2"])    m_DeviceModel = "iPhone 6";
                    else if ([DeviceModel isEqualToString:@"iPhone8,1"])    m_DeviceModel = "iPhone 6s";
                    else if ([DeviceModel isEqualToString:@"iPhone8,2"])    m_DeviceModel = "iPhone 6s Plus";
                    else if ([DeviceModel isEqualToString:@"iPhone9,1"])    m_DeviceModel = "iPhone 7 (CDMA)";
                    else if ([DeviceModel isEqualToString:@"iPhone9,3"])    m_DeviceModel = "iPhone 7 (GSM)";
                    else if ([DeviceModel isEqualToString:@"iPhone9,2"])    m_DeviceModel = "iPhone 7 Plus (CDMA)";
                    else if ([DeviceModel isEqualToString:@"iPhone9,4"])    m_DeviceModel = "iPhone 7 Plus (GSM)";
                    
                    //iPod.
                    else if ([DeviceModel isEqualToString:@"iPod1,1"])      m_DeviceModel = "iPod Touch 1G";
                    else if ([DeviceModel isEqualToString:@"iPod2,1"])      m_DeviceModel = "iPod Touch 2G";
                    else if ([DeviceModel isEqualToString:@"iPod3,1"])      m_DeviceModel = "iPod Touch 3G";
                    else if ([DeviceModel isEqualToString:@"iPod4,1"])      m_DeviceModel = "iPod Touch 4G";
                    else if ([DeviceModel isEqualToString:@"iPod5,1"])      m_DeviceModel = "iPod Touch 5G";
                    
                    //iPad.
                    else if ([DeviceModel isEqualToString:@"iPad1,1"])      m_DeviceModel = "iPad";
                    else if ([DeviceModel isEqualToString:@"iPad2,1"])      m_DeviceModel = "iPad 2 (Welse ifi)";
                    else if ([DeviceModel isEqualToString:@"iPad2,2"])      m_DeviceModel = "iPad 2 (GSM)";
                    else if ([DeviceModel isEqualToString:@"iPad2,3"])      m_DeviceModel = "iPad 2 (CDMA)";
                    else if ([DeviceModel isEqualToString:@"iPad2,4"])      m_DeviceModel = "iPad 2 (32nm)";
                    else if ([DeviceModel isEqualToString:@"iPad2,5"])      m_DeviceModel = "iPad mini (Welse ifi)";
                    else if ([DeviceModel isEqualToString:@"iPad2,6"])      m_DeviceModel = "iPad mini (GSM)";
                    else if ([DeviceModel isEqualToString:@"iPad2,7"])      m_DeviceModel = "iPad mini (CDMA)";
                    
                    else if ([DeviceModel isEqualToString:@"iPad3,1"])      m_DeviceModel = "iPad 3(Welse ifi)";
                    else if ([DeviceModel isEqualToString:@"iPad3,2"])      m_DeviceModel = "iPad 3(CDMA)";
                    else if ([DeviceModel isEqualToString:@"iPad3,3"])      m_DeviceModel = "iPad 3(4G)";
                    else if ([DeviceModel isEqualToString:@"iPad3,4"])      m_DeviceModel = "iPad 4 (Welse ifi)";
                    else if ([DeviceModel isEqualToString:@"iPad3,5"])      m_DeviceModel = "iPad 4 (4G)";
                    else if ([DeviceModel isEqualToString:@"iPad3,6"])      m_DeviceModel = "iPad 4 (CDMA)";
                    
                    else if ([DeviceModel isEqualToString:@"iPad4,1"])      m_DeviceModel = "iPad Air";
                    else if ([DeviceModel isEqualToString:@"iPad4,2"])      m_DeviceModel = "iPad Air";
                    else if ([DeviceModel isEqualToString:@"iPad4,3"])      m_DeviceModel = "iPad Air";
                    else if ([DeviceModel isEqualToString:@"iPad5,3"])      m_DeviceModel = "iPad Air 2";
                    else if ([DeviceModel isEqualToString:@"iPad5,4"])      m_DeviceModel = "iPad Air 2";
                    else if ([DeviceModel isEqualToString:@"i386"])         m_DeviceModel = "Simulator";
                    else if ([DeviceModel isEqualToString:@"x86_64"])       m_DeviceModel = "Simulator";
                    
                    else if ([DeviceModel isEqualToString:@"iPad4,4"]
                             ||[DeviceModel isEqualToString:@"iPad4,5"]
                             ||[DeviceModel isEqualToString:@"iPad4,6"])      m_DeviceModel = "iPad mini 2";
                    
                    else if ([DeviceModel isEqualToString:@"iPad4,7"]
                             ||[DeviceModel isEqualToString:@"iPad4,8"]
                             ||[DeviceModel isEqualToString:@"iPad4,9"])      m_DeviceModel = "iPad mini 3";
                    
                    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
                    [userDefaults setValue:DeviceModel forKey:@"DeviceMode"];
                    
                    NSString *systemVersion = [[UIDevice currentDevice] systemVersion];
                    LYNXCHAR Buf[128];
                    lynxNSString2cString(Buf, systemVersion);
                    m_DeviceOSVersion = Buf;
                    
                    CTTelephonyNetworkInfo *telephonyInfo = [[CTTelephonyNetworkInfo alloc] init];
                    CTCarrier *carrier = telephonyInfo.subscriberCellularProvider;
                    if (carrier != nil)
                    {
                        lynxNSString2cString(Buf, carrier.carrierName);
                        m_TelCompany = Buf;
                    }
                    
                    // xg or wifi
                    //http://stackoverflow.com/questions/7938650/ios-detect-3g-or-wifi
                    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, "8.8.8.8");
                    SCNetworkReachabilityFlags flags;
                    BOOL success = SCNetworkReachabilityGetFlags(reachability, &flags);
                    CFRelease(reachability);
                    if (success)
                    {
                        BOOL isReachable = ((flags & kSCNetworkReachabilityFlagsReachable) != 0);
                        BOOL needsConnection = ((flags & kSCNetworkReachabilityFlagsConnectionRequired) != 0);
                        BOOL isNetworkReachable = (isReachable && !needsConnection);
                        
                        if (isNetworkReachable)
                        {
                            if ((flags & kSCNetworkReachabilityFlagsIsWWAN) != 0)
                            {
                                NSString *technologyString = telephonyInfo.currentRadioAccessTechnology;
                                
                                if (technologyString != nil)
                                {
                                    //http://www.jianshu.com/p/7b98fb9dad45
                                    //http://stackoverflow.com/questions/25405566/mapping-ios-7-constants-to-2g-3g-4g-lte-etc
                                
                                    NSArray *typeStrings2G = @[CTRadioAccessTechnologyEdge,
                                                           CTRadioAccessTechnologyGPRS,
                                                           CTRadioAccessTechnologyCDMA1x];
                                
                                    NSArray *typeStrings3G = @[CTRadioAccessTechnologyHSDPA,
                                                           CTRadioAccessTechnologyWCDMA,
                                                           CTRadioAccessTechnologyHSUPA,
                                                           CTRadioAccessTechnologyCDMAEVDORev0,
                                                           CTRadioAccessTechnologyCDMAEVDORevA,
                                                           CTRadioAccessTechnologyCDMAEVDORevB,
                                                           CTRadioAccessTechnologyeHRPD];
                                
                                    NSArray *typeStrings4G = @[CTRadioAccessTechnologyLTE];
                                
                                    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
                                    {
                                        CTTelephonyNetworkInfo *teleInfo= [[CTTelephonyNetworkInfo alloc] init];
                                        NSString *accessString = teleInfo.currentRadioAccessTechnology;
                                        
										if (accessString != nil)
										{										
											if ([typeStrings4G containsObject:accessString])
											{
												m_NetworkType = "4G";
											}
											else if ([typeStrings3G containsObject:accessString])
											{
												m_NetworkType = "3G";
											}
											else if ([typeStrings2G containsObject:accessString])
											{
												m_NetworkType = "2G";
											}
											else
											{
												lynxNSString2cString(Buf, technologyString);
												m_NetworkType = Buf;
											}
										}
										else
										{
											lynxNSString2cString(Buf, technologyString);
											m_NetworkType = Buf;											
										}
                                    }
                                    else
                                    {
                                        lynxNSString2cString(Buf, technologyString);
                                        m_NetworkType = Buf;
                                    }
                                }
                            }
                            else
                            {
                                m_NetworkType = "WIFI";
                            }
                        }
                    }
                    
                    
                    //[DeviceModel release];
                    LYNXDEL_ARRAY(DeviceName);
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  :
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetupCPUClass()
            {
                switch (GetDeviceType())
                {
                    case DT_IPOD_1:
                    case DT_IPOD_2:
                    case DT_IPOD_3:
                    case DT_IPOD_4:
                    case DT_IPHONE_1:
                    case DT_IPHONE_2:
                    case DT_IPHONE_3:
                    case DT_IPAD_1:
                        m_CPUClass = DC_LOW;
                        break;
                        
                    case DT_IPOD_5:
                    case DT_IPHONE_4:
                    case DT_IPAD_3:
                        m_CPUClass = DC_MEDIUM;
                        break;
                        
                    case DT_IPHONE_5:
                    case DT_IPHONE_6:
                    case DT_IPHONE_7:
                    case DT_IPAD_4:
                    case DT_PC:
                        m_CPUClass = DC_HIGH;
                        break;
                        
                    default:
                        m_CPUClass = DC_UNKNOWN;
                        break;
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  :
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetupGPUClass()
            {
                switch (GetDeviceType())
                {
                    case DT_IPOD_1:
                    case DT_IPOD_2:
                    case DT_IPOD_3:
                    case DT_IPOD_4:
                    case DT_IPHONE_1:
                    case DT_IPHONE_2:
                    case DT_IPHONE_3:
                    case DT_IPHONE_4:
                    case DT_IPAD_1:
                    case DT_IPAD_2:
                        m_GPUClass = DC_LOW;
                        break;
                        
                    case DT_IPOD_5:
                    case DT_IPAD_3:
                        m_GPUClass = DC_MEDIUM;
                        break;
                        
                    case DT_IPHONE_5:
                    case DT_IPHONE_6:
                    case DT_IPAD_4:
                        m_GPUClass = DC_HIGH;
                        break;
                        
                    case DT_IPHONE_7:
                    case DT_PC:
                        m_GPUClass = DC_ULTRA;
                        break;
                        
                    default:
                        m_GPUClass = DC_UNKNOWN;
                        break;
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  :
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetupMemoryClass()
            {
                switch (GetDeviceType())
                {
                    case DT_IPOD_1:
                    case DT_IPOD_2:
                    case DT_IPOD_3:
                    case DT_IPOD_4:
                    case DT_IPHONE_1:
                    case DT_IPHONE_2:
                    case DT_IPAD_1:
                        m_MemoryClass = DC_LOW;
                        break;
                        
                    case DT_IPOD_5:
                    case DT_IPHONE_3:
                    case DT_IPHONE_4:
                    case DT_IPAD_3:
                        m_MemoryClass = DC_MEDIUM;
                        break;
                        
                    case DT_IPHONE_5:
                    case DT_IPHONE_6:
                    case DT_IPAD_4:
                    case DT_PC:
                        m_MemoryClass = DC_HIGH;
                        break;
                        
                    default:
                        m_MemoryClass = DC_UNKNOWN;
                        break;
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetDeviceOrientationByInterfaceOrientation(UIInterfaceOrientation o)
            {
                switch (o)
                {
                    case UIInterfaceOrientationPortrait:
                        m_DeviceOrientation = DO_PORTRAIT;
                        break;
                    case UIInterfaceOrientationPortraitUpsideDown:
                        m_DeviceOrientation = DO_PORTRAIT_UPSIDE_DOWN;
                        break;
                    case UIInterfaceOrientationLandscapeLeft:
                        m_DeviceOrientation = DO_LANDSCAPE_RIGHT;
                        break;
                    case UIInterfaceOrientationLandscapeRight:
                        m_DeviceOrientation = DO_LANDSCAPE_LEFT;
                        break;
                    default:
                        m_DeviceOrientation = DO_UNKNOWN;
                        break;
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::SetDeviceOrientation(UIDeviceOrientation o)
            {
                switch (o)
                {
                    case UIDeviceOrientationPortrait:
                        m_DeviceOrientation = DO_PORTRAIT;
                        break;
                    case UIDeviceOrientationPortraitUpsideDown:
                        m_DeviceOrientation = DO_PORTRAIT_UPSIDE_DOWN;
                        break;
                    case UIDeviceOrientationLandscapeLeft:
                        m_DeviceOrientation = DO_LANDSCAPE_LEFT;
                        break;
                    case UIDeviceOrientationLandscapeRight:
                        m_DeviceOrientation = DO_LANDSCAPE_RIGHT;
                        break;
                    default:
                        m_DeviceOrientation = DO_UNKNOWN;
                        break;
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            CString CKernel::vGetGameVersion()
            {
                CString Version = _T("0.00.000");
                
                NSDictionary *infoDictionary = [[NSBundle mainBundle]infoDictionary];
                NSString *version = infoDictionary[@"CFBundleShortVersionString"];
                NSString *build = infoDictionary[(NSString*)kCFBundleVersionKey];
                if (version)
                {
                    LYNXCHAR Ver[256], Build[256];
                    lynxNSString2cString(Ver, version);
                    //lynxNSString2cString(Build, build);
                    Version = CString(Ver);// + CString(_T(".")) + CString(Build);
                }
                return Version;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vGenerateUDID(LYNXBOOL bforce_generate_new_udid)
            {
                NSString *udid = nil;
                if (!bforce_generate_new_udid && [[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)])
                {
                    NSUUID* uuid = [[UIDevice currentDevice] identifierForVendor];
                    if (uuid != nil)
                    {
                        udid = [uuid UUIDString];
                    }
                    else
                    {
                        // Create universally unique identifier (object)
                        CFUUIDRef uuidObject = CFUUIDCreate(kCFAllocatorDefault);
                        // Get the string representation of CFUUID object.
                        udid = (NSString *)CFBridgingRelease(CFUUIDCreateString(kCFAllocatorDefault, uuidObject));
                        CFRelease(uuidObject);
                    }
                }
                else
                {
                    // Create universally unique identifier (object)
                    CFUUIDRef uuidObject = CFUUIDCreate(kCFAllocatorDefault);
                    // Get the string representation of CFUUID object.
                    udid = (NSString *)CFBridgingRelease(CFUUIDCreateString(kCFAllocatorDefault, uuidObject));
                    CFRelease(uuidObject);
                }
                m_UDIDString = [udid cStringUsingEncoding:iOS_STRING_ENCODING];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            LYNXBOOL CKernel::vInit(const int argc, const char **argv, INIT_DESC* desc, const int w, const int h, const LYNXBOOL bfull)
            {
                LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create memory allocator...\r\n")));
                vCreateMemAllocator();
                LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create memory allocator OK.\r\n")));
                
                SetupDeviceType();
                SetupCPUClass();
                SetupGPUClass();
                SetupMemoryClass();
                
                m_MotionManager = [[CMMotionManager alloc] init];
                [m_MotionManager startAccelerometerUpdates];
                [m_MotionManager startGyroUpdates];
                
                m_LocationManager = [[CLLocationManager alloc] init];
                m_LocationManager.headingFilter = kCLHeadingFilterNone;
                // setup delegate callbacks
                //m_LocationManager.delegate = self;
                // start the compass
                [m_LocationManager startUpdatingHeading];
                
                m_lpStoreKit = [LynxStoreKit sharedInstance];
                
                m_lpEngine = vCreateEngine();
                m_lpEngine->vCreate(desc);
                
                if (desc)
                {
                    if (!((CEngine*)m_lpEngine)->vInit(m_Window, &(desc->GraphicParam)))
                        return LYNX_FALSE;
                }
                else
                {
                    if (!((CEngine*)m_lpEngine)->vInit(m_Window, w, h))
                        return LYNX_FALSE;
                }
                
                //m_lpEngine->vInitLogoSprite();
                SetRunning(LYNX_TRUE);
                
                if (m_OffscreenSprite)
                    m_lpEngine->DestroyContainer(m_OffscreenSprite);
                
                m_OffscreenSprite = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
                LynxEngine::CSpriteCreationDesc Desc;
                GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
                Desc.Default();
                Desc.m_TextureDesc.MipMapLevel = 1;
                Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
                Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
                Desc.m_lpStream = NULL;
                Desc.m_FileName = _T("");
                Desc.m_bUseBufferObject = LYNX_TRUE;
                Desc.m_Number = 0;
#ifdef __OGLES2__
                Desc.m_bInvertTexCoordV = LYNX_TRUE;
#else
                Desc.m_bInvertTexCoordV = LYNX_FALSE;
#endif
                Desc.m_Name = CString(_T("OffscreenSpriteContainer"));
                Desc.m_MaterialName = CString(_T("OffscreenSpriteContainer.Material"));
                Desc.m_Width = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth());
                Desc.m_Height = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
                m_OffscreenSprite->vCreate(&Desc);
                m_OffscreenSprite->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());
                m_OffscreenSprite->SetAlignmentType(0);
                m_OffscreenSprite->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
                m_OffscreenSprite->GetlpCurrentMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
                
                if (m_ScreenFilterSprite)
                    m_lpEngine->DestroyContainer(m_ScreenFilterSprite);
                
                m_ScreenFilterSprite = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
                GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
                Desc.Default();
                Desc.m_TextureDesc.MipMapLevel = 1;
                Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
                Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
                Desc.m_lpStream = NULL;
                Desc.m_FileName = _T("white.png");
                Desc.m_bUseBufferObject = LYNX_TRUE;
                Desc.m_Number = 0;
#ifdef __OGLES2__
                Desc.m_bInvertTexCoordV = LYNX_TRUE;
#else
                Desc.m_bInvertTexCoordV = LYNX_FALSE;
#endif
                Desc.m_Name = CString(_T("ScreenFilterSpriteContainer"));
                Desc.m_MaterialName = CString(_T("ScreenFilterSpriteContainer.Material"));
                Desc.m_Width = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth());
                Desc.m_Height = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
                m_ScreenFilterSprite->vCreate(&Desc);
                m_ScreenFilterSprite->SetAlignmentType(0);
                m_ScreenFilterSprite->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
                m_ScreenFilterSprite->GetlpCurrentMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
                m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Red = 0;
                m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Green = 0;
                m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Blue = 0;
                m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha = 0;
                
				vLoadBorder();
                
                m_bFadeIn = LYNX_FALSE;
                m_GestureState = 0;
                
                return LYNX_TRUE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetGestureRecognization(LYNXBOOL b)
            {
                m_bGestureRecognization = b;
                
                vSetTapGestureRecognization(b);
                vSetSwipeGestureRecognization(b);
                vSetPinchGestureRecognization(b);
                vSetRotationGestureRecognization(b);
                vSetLongPressGestureRecognization(b);
                vSetPanGestureRecognization(b);
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetTapGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetTapGestureRecognization:b];
                if (b)
                    m_GestureState |= TAP_GESTURE;
                else
                    m_GestureState &= ~TAP_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetSwipeGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetSwipeGestureRecognization:b];
                if (b)
                    m_GestureState |= SWIPE_GESTURE;
                else
                    m_GestureState &= ~SWIPE_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetPinchGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetPinchGestureRecognization:b];
                if (b)
                    m_GestureState |= PINCH_GESTURE;
                else
                    m_GestureState &= ~PINCH_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetRotationGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetRotationGestureRecognization:b];
                if (b)
                    m_GestureState |= ROTATION_GESTURE;
                else
                    m_GestureState &= ~ROTATION_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetLongPressGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetLongPressGestureRecognization:b];
                if (b)
                    m_GestureState |= LONGPRESS_GESTURE;
                else
                    m_GestureState &= ~LONGPRESS_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetPanGestureRecognization(LYNXBOOL b)
            {
                [m_lpView SetPanGestureRecognization:b];
                if (b)
                    m_GestureState |= PAN_GESTURE;
                else
                    m_GestureState &= ~PAN_GESTURE;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetGestureState(int s)
            {
                m_GestureState = s;
                
                if (m_GestureState & TAP_GESTURE)
                    vSetTapGestureRecognization(LYNX_TRUE);
                else
                    vSetTapGestureRecognization(LYNX_FALSE);
                
                if (m_GestureState & SWIPE_GESTURE)
                    vSetSwipeGestureRecognization(LYNX_TRUE);
                else
                    vSetSwipeGestureRecognization(LYNX_FALSE);
                
                if (m_GestureState & PINCH_GESTURE)
                    vSetPinchGestureRecognization(LYNX_TRUE);
                else
                    vSetPinchGestureRecognization(LYNX_FALSE);
                
                if (m_GestureState & ROTATION_GESTURE)
                    vSetRotationGestureRecognization(LYNX_TRUE);
                else
                    vSetRotationGestureRecognization(LYNX_FALSE);
                
                if (m_GestureState & LONGPRESS_GESTURE)
                    vSetLongPressGestureRecognization(LYNX_TRUE);
                else
                    vSetLongPressGestureRecognization(LYNX_FALSE);
                
                if (m_GestureState & PAN_GESTURE)
                    vSetPanGestureRecognization(LYNX_TRUE);
                else
                    vSetPanGestureRecognization(LYNX_FALSE);
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vRegisterRemoteNotification()
            {
                float systemVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
                
                LYNXCHAR Buf[64];
                lynxSprintf(Buf, _T("Registering for remote notifications, OS version = %,3f"), systemVersion);
                LYNX_LOG(glpLogger, Buf);

                if (systemVersion >= 10.0f)
                {
#if SUPPORT_NOTIFICATION_IOS_10_0
                    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
                    
                    // TODO: set app delegate to QubotWar LynxAppDelegate for remote notification
                    // center.delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
                    
                    [center requestAuthorizationWithOptions:UNAuthorizationOptionAlert + UNAuthorizationOptionSound completionHandler:^(BOOL granted, NSError* _Nullable error){
                            // Enable or disable features based on authorization
                        }];
                    
                    [center getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings* _Nonnull settings){
                        
                     }];
#endif
                }
                else
                {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
                    // systemVersion >= 8.0f
                    if (systemVersion >= 8.0f && [[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)])
                    {
                        // UIUserNotificationType types = (UIUserNotificationType)(UIUserNotificationTypeSound | UIUserNotificationTypeBadge | UIUserNotificationTypeAlert);
                        // UIUserNotificationSettings* settings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
                        // [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
                        
                        // This is for iOS_7_0 dyld:Symbol not found issue.($_UIUserNotificationSettings)
                        Class UserNotificationClass = NSClassFromString(@"UIUserNotificationSettings");
                        if (nil != UserNotificationClass)
                        {
                            UIUserNotificationType types = (UIUserNotificationType)(UIUserNotificationTypeSound | UIUserNotificationTypeBadge | UIUserNotificationTypeAlert);
                            NSArray* Args = [NSArray arrayWithObjects:[NSNumber numberWithInteger:types], nil];
                            
                            if ([[UserNotificationClass class] respondsToSelector:@selector(settingsForType:)])
                            {
                                id NotificationInstance = [[UserNotificationClass class] performSelector:@selector(settingsForType:) withObject:Args];
                                [[UIApplication sharedApplication] registerUserNotificationSettings:NotificationInstance];
                            }                            
                        }
                    }
                    // systemVersion 7.x
                    else if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerForRemoteNotificationTypes:)])
                    {
                        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeBadge | UIUserNotificationTypeAlert)];
                        [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
                    }
                    
#else
                    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeBadge | UIUserNotificationTypeAlert)];
                    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
#endif
                }
             
            }
			//-------------------------------------------------------------------------------------------------------
            //
            //  
            //-------------------------------------------------------------------------------------------------------
			void CKernel::vScheduleLocalNotification(int notif_id, const CString& title, const CString& context, const CString& ringtone, int invokeTimeInSeconds)
			{
                // NSCalendar *calendar = [NSCalendar autoupdatingCurrentCalendar];
                // NSDateComponents *dateComps = [[NSDateComponents alloc] init];
                NSString *nscontexts, *nstitle, *nsringtone;
                lynxcString2NSString(&nstitle, title);
                lynxcString2NSString(&nscontexts, context);
                lynxcString2NSString(&nsringtone, ringtone);
                NSDate* dateForDateNow = [NSDate date];
                NSDate* fireDate = [dateForDateNow dateByAddingTimeInterval:invokeTimeInSeconds];
                
                float systemVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
                
                if (systemVersion >= 10.0f)
                {
#if SUPPORT_NOTIFICATION_IOS_10_0
                    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
                    
                    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
                    content.title = nstitle;
                    content.body = nscontexts;
                    content.sound = (ringtone.empty())
                        ? [UNNotificationSound defaultSound]
                        : [UNNotificationSound soundNamed:nsringtone];
                    
                    UNTimeIntervalNotificationTrigger* trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:invokeTimeInSeconds repeats:NO];
                 
                    UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier:@"LynxNotification" content:content trigger:trigger];
                    
                    [center addNotificationRequest:request withCompletionHandler:^(NSError* _Nullable error)
                     {
                         // Error handle
                     }];
#endif
                }
                else
                {
                    UILocalNotification *localNotif = [[UILocalNotification alloc] init];				
                    localNotif.fireDate = fireDate;
                    localNotif.timeZone = [NSTimeZone defaultTimeZone]; 
                    localNotif.alertBody = nscontexts;
                    localNotif.alertAction = @"Action Message";
                    // localNotif.alertTitle = nstitle;
     
                    
                    localNotif.soundName = (ringtone.empty())
                        ? UILocalNotificationDefaultSoundName
                        : [NSString stringWithFormat:@"%s.caf", ringtone.c_str()];
                    // localNotif.applicationIconBadgeNumber = 1;
     
                    NSLog(@"local sound = %@", localNotif.soundName);
                    
                    NSDictionary* userInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"LynxEngineiOSNotificationTAG",             @"notify_tag",
                            [NSString stringWithFormat:@"%d", notif_id], @"notify_id",
                            nstitle,                                     @"notify_title",
                            nscontexts,                                  @"notify_msg",
                            nsringtone,                                  @"notify_sound",
                            nil];
                    localNotif.userInfo = userInfo;

                    [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
                }
			}
			//-------------------------------------------------------------------------------------------------------
            //
            //  
            //-------------------------------------------------------------------------------------------------------
			void CKernel::vCancelLocalNotification(int notif_id)
			{
                float systemVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
                
                if (systemVersion >= 10.0f)
                {
#if SUPPORT_NOTIFICATION_IOS_10_0
                    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
                    [center removeDeliveredNotificationsWithIdentifiers:[NSArray arrayWithObjects:[NSString stringWithFormat:@"%d", notif_id], nil]];
#endif
                }
                else
                {
                    UIApplication *app = [UIApplication sharedApplication];
                    NSArray *eventArray = [app scheduledLocalNotifications];
                    NSString* nid = [NSString stringWithFormat:@"%d", notif_id];
                    for (int i=0; i<[eventArray count]; i++)
                    {
                        UILocalNotification* oneEvent = [eventArray objectAtIndex:i];
                        NSDictionary *userInfo = oneEvent.userInfo;
                        NSString *uid = [NSString stringWithFormat:@"%@",[userInfo valueForKey:@"notify_id"]];
                        if ([uid isEqualToString:nid])
                        {
                            //Cancelling local notification
                            [app cancelLocalNotification:oneEvent];
                            break;
                        }
                    }
                }
			}
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::PrintEngineInfo()
            {
                LYNXCOLORRGBA FontColor;
                LYNXRECT Rect;
                
                FontColor.Red = 204;
                FontColor.Green = 152;
                FontColor.Blue = 49;
                FontColor.Alpha = 190;
                
                GetlpEngine()->GetSystemFontString()->SetFont(GetlpEngine()->GetSystemSmallFont());
                
                /*
                 GetlpEngine()->GetSystemFontString()->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                 GetlpEngine()->GetSystemFontString()->SetOriginType(CSpriteContainer::OT_CENTER);
                 Rect.x1 = 0; Rect.x2 = 320; Rect.y1 = 360; Rect.y2 = 390;
                 GetlpEngine()->GetSystemFontString()->SetAlignmentRect(&Rect);
                 GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("LynxEngine"));
                 Rect.x1 = 0; Rect.x2 = 320; Rect.y1 = 390; Rect.y2 = 420;
                 GetlpEngine()->GetSystemFontString()->SetAlignmentRect(&Rect);
                 GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("iPhone Version"));
                 Rect.x1 = 0; Rect.x2 = 320; Rect.y1 = 420; Rect.y2 = 450;
                 GetlpEngine()->GetSystemFontString()->SetAlignmentRect(&Rect);
                 GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("%d.%d.%d"), GetlpEngine()->GetMajorVersion(), GetlpEngine()->GetMinorVersion(), GetlpEngine()->GetBuildVersion());
                 
                 Rect.x1 = 0; Rect.x2 = 320; Rect.y1 = 450; Rect.y2 = 480;
                 GetlpEngine()->GetSystemFontString()->SetAlignmentRect(&Rect);
                 glpKernel->GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("FPS:%03d, Step:%02.2f"), LYNX_ROUND(1000.0f/glpKernel->GetTotalFrameElapsedTime()), glpKernel->m_Step);
                 */
                
                GetlpEngine()->GetSystemFontString()->SetFont(GetlpEngine()->GetSystemFont());
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            float CKernel::GetViewScaleFactor()
            {
                return m_lpView.contentScaleFactor;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::EnableCameraCapture(LYNXBOOL b)
            {
                /*
                 if (b)
                 {
                 [m_lpViewController setupAVCapture];
                 }
                 else
                 {
                 [m_lpViewController tearDownAVCapture];
                 }
                 m_bCameraCapture = b;
                 */
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::StartCameraCapture()
            {
                /*
                 if (m_bCameraCapture)
                 {
                 [m_lpViewController startAVCapture];
                 }
                 */
                
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::StopCameraCapture()
            {
                /*
                 if (m_bCameraCapture)
                 {
                 [m_lpViewController stopAVCapture];
                 m_CameraCaptureRawData = NULL;
                 }
                 */
                
            }
			//-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            CString CKernel::vGetSystemLanguageCode()
			{
				#ifdef _DEBUG
					CIniReader IniReader(_T("./config/game.ini"));
					return (IniReader.ReadString(_T("System"), _T("Language"), _T("eng")));
				#else
					LYNXCHAR LanguageStr[128];
					NSString* Language = [[NSLocale preferredLanguages] objectAtIndex:0];
					lynxNSString2cString(LanguageStr, Language);
                
                    LYNX_LOG(glpLogger, CString(_T("iOS Language Code : ")) + CString(LanguageStr) + CString(_T("\r\n")));
					
                return CGame::GetLanguageCodeFromISO_639_1(LanguageStr);
				#endif
			}
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetMainContext()
            {
#ifndef __GLKVIEW__
                [GetlpView() SetCurrentContext];
#endif
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetLoadingContext()
            {
#ifndef __GLKVIEW__
                [GetlpView() SetLoadingContext];
#endif
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            LYNXBOOL CKernel::vLoop()
            {
                LYNXBOOL Ret = LynxEngine::GameFramework::CKernel::vLoop();
#ifndef __GLKVIEW__
                [GetlpView().m_Context presentRenderbuffer:GL_RENDERBUFFER];
#endif
                return Ret;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSwapBackBuffer()
            {
#ifndef __GLKVIEW__
                [GetlpView().m_Context presentRenderbuffer:GL_RENDERBUFFER];
#endif
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vPause()
            {
                if (!GetlpEngine()->GetlpSoundSystem()->IsPaused())
                {
                    AudioSessionSetActive(NO);
                    
                    if (GetlpEngine()->GetlpSoundSystem())
                        GetlpEngine()->GetlpSoundSystem()->vPause();
                }
                [m_lpView Activate:LYNX_FALSE];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vResume()
            {
                [m_lpView SetCurrentContext];
                
                if (GetlpEngine()->GetlpSoundSystem()->IsPaused())
                {
                    // Reset audio session
                    UInt32 Category = kAudioSessionCategory_AmbientSound;
                    AudioSessionSetProperty ( kAudioSessionProperty_AudioCategory, sizeof (Category), &Category );
                    
                    // Reactivate the current audio session
                    AudioSessionSetActive(YES);
                    
                    if (GetlpEngine()->GetlpSoundSystem())
                        GetlpEngine()->GetlpSoundSystem()->vResume();
                }
                [m_lpView Activate:LYNX_TRUE];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vTerminate()
            {
                if (!GetlpEngine()->GetlpSoundSystem()->IsPaused())
                {
                    AudioSessionSetActive(NO);
                    
                    if (GetlpEngine()->GetlpSoundSystem())
                        GetlpEngine()->GetlpSoundSystem()->vPause();
                }
                [m_lpView Activate:LYNX_FALSE];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vRenderingThreadBegin()
            {
                [m_lpView SetCurrentContext];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vRenderingThreadEnd()
            {
                [m_lpView ReleaseCurrentContext];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOpenTextInput(const CString& name, GUISystem::CText* text)
            {
                text->Activate(LYNX_FALSE);
                [m_lpViewController OpenTextInputViewController:name.c_str() withText:text];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOpenWebView(const CString& url, const CString& title, LYNXRECT& rect)
            {
                [m_lpViewController OpenBrowserViewController:url.c_str() withTitle:title.c_str()];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOpenCustomWebview(const CString& name, LYNXRECT& rect, const CString& title, const CString& htmlOrUrl)
            {
				vOSN_ReleaseView(name);
				
                float Factor = GetlpView().contentScaleFactor;
                UIView *baseView = [[UIView alloc] initWithFrame:CGRectMake(rect.x1/Factor, rect.y1/Factor, (rect.x2-rect.x1)/Factor, (rect.y2-rect.y1)/Factor)];
                m_OSNViewMap[name] = baseView;
                [GetlpView() addSubview:baseView];
                
                UIImageView* backgroundView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, baseView.frame.size.width, baseView.frame.size.height)];
                [GetlpView() addSubview:baseView];
                [backgroundView setImage:[UIImage imageNamed:@"texture/ui/infobox24.png"]];
                [baseView addSubview:backgroundView];
                
                UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, baseView.frame.size.width, 44/Factor)];
                [titleLable setText:@"?®ô??"];
                [titleLable setTextColor:[UIColor blackColor]];
                [titleLable setBackgroundColor:[UIColor clearColor]];
                CString cfont = GetlpEngine()->m_ConfigIniReader->ReadString(GetlpGame()->GetCurrentLanguage(), _T("SystemTrueTypeFont"), _T(""));
                NSString *font;
                lynxcString2NSString(&font, cfont.c_str());
                [titleLable setFont:[UIFont fontWithName:font size:14]];
                titleLable.textAlignment = NSTextAlignmentCenter;
                [baseView addSubview:titleLable];
                
                UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectMake((GetlpView().frame.size.width-560/Factor)/2, 50/Factor, 560/Factor, 540/Factor)];
                webView.backgroundColor = [UIColor clearColor];
                webView.opaque = NO;
                NSString *nsHtmlOrUrl;
                lynxcString2NSString(&nsHtmlOrUrl, htmlOrUrl.c_str());
                if ([nsHtmlOrUrl hasPrefix:@"http"])
                    [webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:nsHtmlOrUrl]]];
                else
                    [webView loadHTMLString:nsHtmlOrUrl baseURL:nil];
                [GetlpView() addSubview:webView];
                
                UIButton *button =  [[UIButton alloc] initWithFrame:CGRectMake(((baseView.frame.size.width/2)-168/Factor)/2, baseView.frame.size.height-72/Factor, 168/Factor, 68/Factor)];
                [button setTitle:@"Close" forState:UIControlStateNormal];
                button.titleLabel.font = [UIFont systemFontOfSize:12];
                [button.titleLabel setTextColor:[UIColor blackColor]];
                [button setBackgroundImage:[UIImage imageNamed:@"texture/ui/button_green06.png"] forState:UIControlStateNormal];
                //[button addTarget:self action:@selector(BtnClick:) forControlEvents:UIControlEventTouchUpInside];
                [baseView addSubview:button];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_AddColorBackground(const CString& name, LYNXCOLORRGBA color)
            {
				vOSN_ReleaseView(name);

                UIView* baseView = nil;
				if (!m_OSNViewMap.is_exist(name))
				{
					baseView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] applicationFrame].size.width, [[UIScreen mainScreen] applicationFrame].size.height)];
					m_OSNViewMap[name] = baseView;					
				}
                else
                {
                    baseView = m_OSNViewMap[name];
                }
                [GetlpView() addSubview:m_OSNViewMap[name]];
                [baseView setBackgroundColor:[UIColor colorWithRed:color.Red/255.0f green:color.Green/255.0f blue:color.Blue/255.0f alpha:1.0f]];
                baseView.userInteractionEnabled = FALSE;
                baseView.alpha = color.Alpha/255.0f;
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_AddAnimationView(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect)
            {
				vOSN_ReleaseView(name);
				
                NSMutableArray *images = [[NSMutableArray alloc] init];
                LYNXCHAR Name[256];
                NSString *ImageString = nil;
                for (int i = 0; i < num; i++)
                {
                    lynxSprintf(Name, _T("%s%02d.%s"), &(filename.c_str()[3]), i, ext.c_str());
                    lynxcString2NSString(&ImageString, Name);
                    [images addObject:[UIImage imageNamed:ImageString]];
                }

				LYNXRECT Rect = rect;
				Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
				Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
                
                // Normal Animation
                CGRect GRect = CGRectMake(Rect.x1/GetlpView().contentScaleFactor, Rect.y1/GetlpView().contentScaleFactor, (Rect.x2-Rect.x1)/GetlpView().contentScaleFactor, (Rect.y2-Rect.y1)/GetlpView().contentScaleFactor);
                UIImageView *animationImageView = [[UIImageView alloc] initWithFrame:GRect];//[GetlpView() convertRect:Rect toView:GetlpView()]];
                m_OSNViewMap[name] = animationImageView;
                animationImageView.animationImages = images;
                animationImageView.animationDuration = (dur*num)/1000.0f;
                
                LynxView* View = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetlpView();
                [View addSubview:animationImageView];
                [animationImageView startAnimating];

				[images removeAllObjects];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_AddTextView(const CString& name, const CString& filename, LYNXRECT& rect)
            {
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_AddWebview(const CString& name, LYNXRECT& rect, const CString& htmlOrUrl, const LYNXBOOL bNotFullScreen)
            {
				vOSN_ReleaseView(name);
				
				LYNXRECT Rect = rect;
				Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
				Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;

				float Factor = GetlpView().contentScaleFactor;
                
                UIWebView *webView = nil;
				webView = [[UIWebView alloc] initWithFrame:CGRectMake(Rect.x1/Factor, Rect.y1/Factor, (Rect.x2-Rect.x1)/Factor, (Rect.y2-Rect.y1)/Factor)];
				m_OSNViewMap[name] = webView;
				
                webView.backgroundColor = [UIColor clearColor];
                webView.opaque = NO;
                webView.scrollView.frame = CGRectMake(0, 0, webView.frame.size.width, webView.frame.size.height);
                webView.scalesPageToFit = true;
                [webView setDelegate:GetlpViewController()];
                NSString *nsHtmlOrUrl;
                lynxcString2NSString(&nsHtmlOrUrl, htmlOrUrl.c_str());
                if ([nsHtmlOrUrl hasPrefix:@"http"])
                    [webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:nsHtmlOrUrl]]];
                else
                    [webView loadHTMLString:nsHtmlOrUrl baseURL:nil];
                
                [GetlpView() addSubview:webView];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_RemoveView(const CString& name)
            {
				if (!m_OSNViewMap.is_exist(name))
					return;

                [m_OSNViewMap[name] removeFromSuperview];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_ShowView(const CString& name, LYNXBOOL bshow)
            {
				if (name == CString("LoadingAnimation") || name == CString("ConnectingAnimation") || name == CString("LoadingAnimationRun"))
				{
					if (bshow)
						m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_TRUE);
					else
						m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
				}

				if (!m_OSNViewMap.is_exist(name))
					return;

                m_OSNViewMap[name].hidden = !bshow;
     
#if 0
                NSLog(@"CKernel::vOSN_ShowView name = %@, show = %d, callstack = %@",
                      [NSString stringWithUTF8String:name.c_str()], bshow, [NSThread callStackSymbols]);
#endif
                if (bshow)
                {
                    UIView* view = m_OSNViewMap[name];
                    UIView* parentView = [view superview];
                    if (nil == parentView)
                    {
                        LYNX_LOG(glpLogger, _T("CKernel::vOSN_ShowView nil == parentView re-attach view = ") + CString(name));
                        
                        LynxView* GameView = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetlpView();
                        [GameView addSubview:view];
                        parentView = [view superview];
                    }
#if 0
                    NSLog(@"CKernel::vOSN_ShowView dumpview = %@, superview = %@",
                          [view description], [parentView description]);
#endif
                    if (view && parentView)
                    {
                        LYNX_LOG(glpLogger, _T("CKernel::vOSN_ShowView name = ") + CString(name));
                        [parentView bringSubviewToFront:view];
                    }
                }
            }
			//-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_HideAllViews()
			{
				m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
				for (CMap< CString, UIView* >::CIterator View = m_OSNViewMap.begin(); View != m_OSNViewMap.end(); View++)
				{
					(*View).second.hidden = LYNX_TRUE;
				}
			}
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_MoveView(const CString& name, LYNXRECT& rect, const LYNXBOOL bNotFullScreen)
            {
				if (!m_OSNViewMap.is_exist(name))
					return;

                LYNXRECT Rect = rect;
                Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
                Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
                Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
                Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;

                m_OSNViewMap[name].frame = CGRectMake(Rect.x1/GetlpView().contentScaleFactor, Rect.y1/GetlpView().contentScaleFactor, (Rect.x2-Rect.x1)/GetlpView().contentScaleFactor, (Rect.y2-Rect.y1)/GetlpView().contentScaleFactor);
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_ReleaseView(const CString& name)
            {
				if (name == CString("LoadingAnimation") || name == CString("ConnectingAnimation") || name == CString("LoadingAnimationRun"))
				{
					m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
				}

				if (m_OSNViewMap.is_exist(name))
				{
					[m_OSNViewMap[name] removeFromSuperview];
					m_OSNViewMap[name] = nil;
					m_OSNViewMap.erase(name);
				}
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_OpenURL(const CString& url)
            {
                NSString* Link = nil;
                lynxcString2NSString(&Link, url.c_str());
                
                NSURL *appURL = [NSURL URLWithString:Link];
                if ([[UIApplication sharedApplication] canOpenURL:appURL])
                {
                    [[UIApplication sharedApplication] openURL:appURL];
                }
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:   
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vOSN_OpenMessageBox(const CString& title, const CString& text)
            {
                NSString* Title = nil;
                lynxcString2NSString(&Title, title.c_str());
                NSString* Message = nil;
                lynxcString2NSString(&Message, text.c_str());
                UIAlertView *Alert = [[UIAlertView alloc] initWithTitle:Title message:Message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
                [Alert show];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vQueryIAPList(const CString& base64EncodedPublicKey, const CArray<CString>& list)
            {
                NSMutableArray* productList = [[NSMutableArray alloc] init];
                for (int i=0;i<list.size();++i)
                {
                    NSString* nsString = [NSString stringWithCString:list[i].c_str()
                                                            encoding:[NSString defaultCStringEncoding]];
                    NSLog(@"Add product id = %@", nsString);
                    [productList addObject:nsString];
                }
                [m_lpStoreKit queryInventory:productList];
            }
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vPurchaseIAP(const CString& productId, int requestCode, const CString& playerSN)
            {
                CString Payload;
                LynxEngine::ComputePurchasePayload(playerSN, productId, Payload);
                
                [m_lpStoreKit purchaseInventory:[NSString stringWithCString:productId.c_str() encoding:[NSString           defaultCStringEncoding]]
                                    withPayload:[NSString stringWithCString:Payload.c_str() encoding:[NSString defaultCStringEncoding]]];
            }
            
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:check has fb permission.
            //-------------------------------------------------------------------------------------------------------
            LYNXBOOL CKernel::HasPermissionFacebook(const CString& permissions)
            {
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                
                if (ast == nil)
                {
                    return LYNX_FALSE;
                }
                
                
                NSString* Permissions = nil;
                lynxcString2NSString(&Permissions, permissions.c_str());

                NSArray *stringArray = [Permissions componentsSeparatedByString: @","];

                for (NSString* perm in stringArray)
                {
                    
                    if (![ast hasGranted:perm] && ![perm isEqualToString:@""])
                    {
                        return LYNX_FALSE;
                    }
                }
                
                return LYNX_TRUE;
            }
            
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:fb get account data
            //-------------------------------------------------------------------------------------------------------
            void CKernel::FacebookAccountDataRequest(const CString& permission, const CString& fields)
            {
                if ([FBSDKAccessToken currentAccessToken])
                {
                    NSString* Fields = nil;
                    lynxcString2NSString(&Fields, fields.c_str());

                    
                    NSMutableDictionary* parameters = [NSMutableDictionary dictionary];
                    [parameters setValue:Fields forKey:@"fields"];
                    
                    [[[FBSDKGraphRequest alloc] initWithGraphPath:@"me" parameters:parameters]
                     startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error)
                     {
                         if (!error)
                         {
                             
                             NSError *dsecError;
                             NSData* data = [NSJSONSerialization dataWithJSONObject:result options:NSJSONWritingPrettyPrinted error:&dsecError];
                             NSString* jsonString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

                             NSLog(@"fetched user:%@", [result description]);
                             
                             const char* cp = [jsonString cStringUsingEncoding:NSUTF8StringEncoding];
                             
                             glpKernel->vOnFinishQueryFacebookAccountData(cp);
                             
                         }
                         else
                         {
                             glpKernel->vOnFinishQueryFacebookAccountData("onError");
                         }
                     }];
                }
            }
            
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:fb login
            //-------------------------------------------------------------------------------------------------------

            void CKernel::vFacebookLoginWithPermission(const CString& permissions)
            {
                LYNXBOOL needlogin = LYNX_FALSE;
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    if (!HasPermissionFacebook(permissions))
                    {
                        needlogin = LYNX_TRUE;
                    }
                }
                else
                {
                    needlogin = LYNX_TRUE;
                }
                
                if (needlogin)
                {
                    NSString* Permissions = nil;
                    lynxcString2NSString(&Permissions, permissions.c_str());
                    
                    NSArray *stringArray = [Permissions componentsSeparatedByString: @","];
                    
                    
                    FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
                    [login
                     logInWithReadPermissions: stringArray
                     fromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                     handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                         if (error)
                         {
                             NSLog(@"Process error");
                             
                         }
                         else if (result.isCancelled)
                         {
                             NSLog(@"Cancelled");
                         }
                         else
                         {
                             NSLog(@"Logged in");
                         }
                     }];
                
                }
                
                
            }
            
            //-------------------------------------------------------------------------------------------------------
            //
            //  說明:fb logout
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vFacebookLogout()
            {
                FBSDKLoginManager *LoginMgr = [[FBSDKLoginManager alloc] init];
                [LoginMgr logOut];
                [FBSDKAccessToken setCurrentAccessToken:nil];
                [FBSDKProfile setCurrentProfile:nil];
            }

            void CKernel::vQueryFacebookAccountData(const CString& permissions, const CString& fields)
            {
                LYNXBOOL needsLogin = false;
                LYNXBOOL alreadyLogin = false;
                
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    NSLog(@"FB already Login ");
                    alreadyLogin = true;
                    if (!HasPermissionFacebook(permissions))
                    {
                        NSLog(@"FB Not HasPermissionFacebook ");
                        needsLogin = true;
                    }
                    else
                    {
                        NSLog(@"FB FacebookAccountDataRequest ");
                        FacebookAccountDataRequest(permissions, fields);
                    }
                    
                }
                else
                {
                    needsLogin = true;
                }
                
                
                if (needsLogin)
                {
                    NSLog(@"FB QueryFacebookAccountData");
                    // §vµn§J,¶A•h≠n≈v≠≠Æ…, •u∑|¶^∂« onCancel.
                    LYNXBOOL doThingsAfterLogin = alreadyLogin;
                    
                    NSString* Permissions = nil;
                    lynxcString2NSString(&Permissions, permissions.c_str());
                    
                    NSArray *stringArray = [Permissions componentsSeparatedByString: @","];
                    
                    
                    FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
                    [login
                     logInWithReadPermissions: stringArray
                     fromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                     handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                         if (error)
                         {
                             NSLog(@"FB onError");
                             glpKernel->vOnFinishQueryFacebookAccountData("onError");
                             
                         }
                         else if (result.isCancelled)
                         {
                             NSLog(@"FB onCancel");
                             if (doThingsAfterLogin)
                             {
                                 FacebookAccountDataRequest(permissions, fields);
                             }
                             else
                             {
                                 glpKernel->vOnFinishQueryFacebookAccountData("onCancel");
                             }

                         }
                         else
                         {
                             NSLog(@"FB onSuccess");
                             FacebookAccountDataRequest(permissions, fields);

                         }
                     }];
                    
                    
                }
                
            
            }
            
            
            void CKernel::vOnFinishQueryFacebookAccountData(const CString& JSONResult)
            {
                NSLog(@"FB CKernel::vOnFinishQueryFacebookAccountData");
                
            }
            
            void CKernel::FacebookFriendsDataRequest(const CString& permission,const CString& fields)
            {
                
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    
                    NSString* Fields = nil;
                    lynxcString2NSString(&Fields, fields.c_str());
                    
                    
                    NSMutableDictionary* parameters = [NSMutableDictionary dictionary];
                    [parameters setValue:Fields forKey:@"fields"];

                    
                    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc]
                                                  initWithGraphPath:@"me/friends"
                                                  parameters:parameters
                                                  HTTPMethod:@"GET"];
                    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection,
                                                          id result,
                                                          NSError *error)
                     {
                         // Handle the result
                         if (!error)
                         {
                             NSDictionary *Friends = [result valueForKey:@"data"];
                                                      // [(NSDictionary*)result objectForKey:@"data"] ];
                             
                             NSError *dsecError;
                             NSData* data = [NSJSONSerialization dataWithJSONObject:Friends options:NSJSONWritingPrettyPrinted error:&dsecError];
                             NSString* jsonString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                             
                             
                             NSLog(@"fb friend data:%@", [Friends description]);
                             
                             const char* cp = [jsonString cStringUsingEncoding:NSUTF8StringEncoding];
                             
                             
                             glpKernel->vOnFinishQueryFacebookFriendsData(cp);
                         }
                         else
                         {
                             
                             NSLog(@"%@", [error localizedDescription]);
                             glpKernel->vOnFinishQueryFacebookFriendsData("onError");
                         
                         }
                    }];

                }
              
            }
            
            void CKernel::vQueryFacebookFriendsData(const CString& permissions, const CString& fields)
            {
                NSLog(@"FB CKernel::vQueryFacebookFriendsData");
                
                
                LYNXBOOL needsLogin = false;
                LYNXBOOL alreadyLogin = false;
                
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    NSLog(@"FB already Login ");
                    alreadyLogin = true;
                    if (!HasPermissionFacebook(permissions))
                    {
                        NSLog(@"FB Not HasPermissionFacebook ");
                        needsLogin = true;
                    }
                    else
                    {
                        NSLog(@"FB FacebookFriendsDataRequest ");
                        FacebookFriendsDataRequest(permissions, fields);
                    }
                    
                }
                else
                {
                    needsLogin = true;
                }
                
                
                if (needsLogin)
                {
                    if (needsLogin)
                    {
                        NSLog(@"FB QueryFacebookFriendsData");
                        // §vµn§J,¶A•h≠n≈v≠≠Æ…, •u∑|¶^∂« onCancel.
                        LYNXBOOL doThingsAfterLogin = alreadyLogin;
                        
                        NSString* Permissions = nil;
                        lynxcString2NSString(&Permissions, permissions.c_str());
                        
                        NSArray *stringArray = [Permissions componentsSeparatedByString: @","];
                        
                        
                        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
                        [login
                         logInWithReadPermissions: stringArray
                         fromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                         handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                             if (error)
                             {
                                 NSLog(@"FB onError");
                                 glpKernel->vOnFinishQueryFacebookFriendsData("onError");
                                 
                             }
                             else if (result.isCancelled)
                             {
                                 NSLog(@"FB onCancel");
                                 if (doThingsAfterLogin)
                                 {
                                     FacebookFriendsDataRequest(permissions, fields);
                                 }
                                 else
                                 {
                                     glpKernel->vOnFinishQueryFacebookFriendsData("onCancel");
                                 }
                                 
                             }
                             else
                             {
                                 NSLog(@"FB onSuccess");
                                 FacebookFriendsDataRequest(permissions, fields);
                                 
                             }
                         }];
                        
                        
                    }
                }
                
            }
            
            void CKernel::vOnFinishQueryFacebookFriendsData(const CString& JSONResult)
            {
                NSLog(@"FB CKernel::vOnFinishQueryFacebookFriendsData");
                
            }
            
            
            void CKernel::FacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl)
            {
                
                NSLog(@"FB CKernel::FacebookShareLink");
                
                
                if ([FBSDKAccessToken currentAccessToken])
                {
                    FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
                    
                    NSString* Title = nil;
                    lynxcString2NSString(&Title, title.c_str());
                    content.contentTitle = Title;
                    
                    NSString* ContentDesc = nil;
                    lynxcString2NSString(&ContentDesc, Desc.c_str());
                    content.contentDescription = ContentDesc;
                    
                    NSString* Url = nil;
                    lynxcString2NSString(&Url, contenturl.c_str());
                    content.contentURL = [NSURL URLWithString:Url];
                    
                    NSString* ImageUrl = nil;
                    lynxcString2NSString(&ImageUrl, contenturl.c_str());
                    content.imageURL = [NSURL URLWithString:ImageUrl];
                
                    lynxFBShareDelegate *fbdel = [[lynxFBShareDelegate alloc] init];
                    fbdel.m_ShareType = ST_LINK;
                    
                    [FBSDKShareDialog showFromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                                                 withContent:content
                                                    delegate:fbdel];
                
                }
                
            }
            
            void CKernel::vFacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl)
            {
                
                NSLog(@"FB CKernel::vFacebookShareLink");
                LYNXBOOL needsLogin = false;
                LYNXBOOL alreadyLogin = false;
                
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    alreadyLogin = true;
                }
                else
                {
                    needsLogin = true;
                }
                
                if (needsLogin)
                {
                    if (needsLogin)
                    {
                        NSLog(@"FB QueryFacebookFriendsData");
                        // §vµn§J,¶A•h≠n≈v≠≠Æ…, •u∑|¶^∂« onCancel.
                        LYNXBOOL doThingsAfterLogin = alreadyLogin;
                        
                        NSString* Permissions = @"";
                        
                        NSArray *stringArray = [Permissions componentsSeparatedByString: @","];
                        
                        
                        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
                        [login
                         logInWithReadPermissions: stringArray
                         fromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                         handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                             if (error)
                             {
                                 NSLog(@"FB onError");
                                 glpKernel->vOnFinishFacebookShareLink("onError");
                                 
                             }
                             else if (result.isCancelled)
                             {
                                 NSLog(@"FB onCancel");
                                 if (doThingsAfterLogin)
                                 {
                                     FacebookShareLink(title, Desc, contenturl, imageurl);
                                 }
                                 else
                                 {
                                     glpKernel->vOnFinishFacebookShareLink("onCancel");
                                 }
                                 
                             }
                             else
                             {
                                 NSLog(@"FB onSuccess");
                                 FacebookShareLink(title, Desc, contenturl, imageurl);
                                 
                             }
                         }];
                        
                        
                    }
                }
                else
                {
                    FacebookShareLink(title, Desc, contenturl, imageurl);	
                }


            }
            
            void CKernel::vOnFinishFacebookShareLink(const CString& JSONResult)
            {
                NSLog(@"FB CKernel::vOnFinishFacebookShareLink");
            
            }
            
            void CKernel::FacebookUploadPic(const CString& filepath)
            {
                 NSLog(@"FB CKernel::FacebookUploadPic");

                if ([FBSDKAccessToken currentAccessToken])
                {
                    
                    NSString* Fileppathname = nil;
                    lynxcString2NSString(&Fileppathname, filepath.c_str());

                    
                    UIImage *image = [UIImage imageNamed:Fileppathname];
                    
                    
                    FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
                    photo.image = image;
                    photo.userGenerated = YES;
                    
                    FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
                    content.photos = @[photo];
                    
                    lynxFBShareDelegate *fbdel = [[lynxFBShareDelegate alloc] init];
                    fbdel.m_ShareType = ST_PHOTO;
                    
                    [FBSDKShareDialog showFromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                                                 withContent:content
                                                    delegate:fbdel];
                    
                    
                }
                
            }

            CString CKernel::vFacebookGetAccessToken()
            {
                CString Ret = "";

                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    NSString* Nst = [ast tokenString];
                    LYNXCHAR Buffer[512];
                    lynxNSString2cString(Buffer, Nst);
                    Ret = Buffer;
                }

                return Ret;
            }
            
            void CKernel::vFacebookUploadPic(const CString& filepath)
            {
                NSLog(@"FB CKernel::vFacebookUploadPic");
                
                LYNXBOOL needsLogin = false;
                LYNXBOOL alreadyLogin = false;
                
                FBSDKAccessToken* ast = [FBSDKAccessToken currentAccessToken];
                if (ast != nil)
                {
                    alreadyLogin = true;
                }
                else
                {
                    needsLogin = true;
                }
                
                if (needsLogin)
                {
                    if (needsLogin)
                    {
                        NSLog(@"FB vFacebookUploadPic");
                        // §vµn§J,¶A•h≠n≈v≠≠Æ…, •u∑|¶^∂« onCancel.
                        LYNXBOOL doThingsAfterLogin = alreadyLogin;
                        
                        NSString* Permissions = @"";
                        
                        NSArray *stringArray = [Permissions componentsSeparatedByString: @","];
                        
                        
                        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
                        [login
                         logInWithReadPermissions: stringArray
                         fromViewController:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController()
                         handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                             if (error)
                             {
                                 NSLog(@"FB onError");
                                 glpKernel->vOnFinishFacebookUploadPic("onError");
                                 
                             }
                             else if (result.isCancelled)
                             {
                                 NSLog(@"FB onCancel");
                                 if (doThingsAfterLogin)
                                 {
                                     FacebookUploadPic(filepath);
                                 }
                                 else
                                 {
                                     glpKernel->vOnFinishFacebookUploadPic("onCancel");
                                 }
                                 
                             }
                             else
                             {
                                 NSLog(@"FB onSuccess");
                                 FacebookUploadPic(filepath);
                                 
                             }
                         }];
                        
                        
                    }
                }
                else
                {
                    FacebookUploadPic(filepath);
                }
                

            }
            
            void CKernel::vOnFinishFacebookUploadPic(const CString& JSONResult)
            {
                NSLog(@"FB CKernel::vOnFinishFacebookUploadPic");
                
            }
        
            CString CKernel::vFirebaseGetAccessToken()
            {
                LYNX_LOG(glpLogger, CString(_T("joybomb firebase get token start")));

                CString Ret = "";
                
                NSString *Token = [[FIRInstanceID instanceID] token];
                if (Token != nil || Token.length > 0)
                {
                    
                    LYNX_LOG(glpLogger, CString(_T("joybomb firebase get token not null")));
                    LYNXCHAR Buffer[512];
                    lynxNSString2cString(Buffer, Token);
                    Ret = Buffer;
                }
                
                LYNX_LOG(glpLogger, CString(_T("joybomb firebase get token end")));
                return Ret;
            }
            
            //-------------------------------------------------------------------------------------------------------
            //
            //  ª°©?: 挑選應用程式做分享.
			//	msg : 替玩家寫入的訊息.
			//	imagePath : 例: texture/ui/infobox24.png.
			//	isInternalPath : 內部或外部路徑.
			//			true = data/data/my.package.name/files/
			//			false = Andorid/data/my.package.name/files/
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vShareWithSelectApp(const CString& msg, const CString& imagePath, LYNXBOOL isInternalPath)
            {
                
                NSLog(@"FB CKernel::vShareWithSelectApp");
                
                NSString * Message = nil;
                lynxcString2NSString(&Message, msg.c_str());
                
                NSString* Fileppathname = nil;
                lynxcString2NSString(&Fileppathname, imagePath.c_str());
                
                UIImage *image = [UIImage imageNamed:Fileppathname];
                
                NSArray * shareItems = nil;
                if (image != nil)
                {
                    shareItems = @[Message, image];
                }
                else
                {
                    shareItems = @[Message];
                }
                
                UIActivityViewController * avc = [[UIActivityViewController alloc] initWithActivityItems:shareItems applicationActivities:nil];
                
                if (&UIActivityTypeAirDrop)
                {
                    avc.excludedActivityTypes  = @[UIActivityTypeAirDrop];
                }
                
                
                [((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpViewController() presentViewController:avc animated:YES completion:nil];
            }
			//-------------------------------------------------------------------------------------------------------
            //
            // 設定裝置是否進入休眠模式  
			//
            //-------------------------------------------------------------------------------------------------------
			void CKernel::vSetDeviceSleepMode(LYNXBOOL b)
			{
				float systemVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
                // if (systemVersion >= 8.0f)
				{
					[UIApplication sharedApplication].idleTimerDisabled = (!b);
				}
			}
            //-------------------------------------------------------------------------------------------------------
            //
            // 設定裝置是否進入休眠模式
            //
            //-------------------------------------------------------------------------------------------------------
            void CKernel::vSetCurrentContext()
            {
                [m_lpView SetCurrentContext];
            }
			//-------------------------------------------------------------------------------------------------------
            //
            // 設定開啟詢問使用者給予評價的提示
            //
            //-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenRatingDialog(const CString& title, const CString& text, const CString& positive_text, const CString& negative_text, CString neutral_text)
			{
                // NSString* lang = [[NSLocale preferredLanguages] objectAtIndex:0];
                NSString* lang = [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
                NSLog(@"Country code = %@", lang);
                
                LynxFeedbackDialog* dialog = [LynxFeedbackDialog sharedInstance];
                dialog.title = [NSString stringWithCString:title encoding:NSUTF8StringEncoding];
                dialog.message = [NSString stringWithCString:text encoding:NSUTF8StringEncoding];
                dialog.positiveText = [NSString stringWithCString:positive_text encoding:NSUTF8StringEncoding];
                dialog.negativeText = [NSString stringWithCString:negative_text encoding:NSUTF8StringEncoding];
                dialog.neutralText = [NSString stringWithCString:neutral_text encoding:NSUTF8StringEncoding];
                dialog.countryCode = lang;
                [dialog show];
            }
        }
    }
}
