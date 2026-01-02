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
//  說明:
//
//###########################################################################

#ifndef __LYNXIOSGAMEFRAMEWORKKERNEL_H__
#define __LYNXIOSGAMEFRAMEWORKKERNEL_H__

#include <LynxObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GameFramework/LynxUIPage.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#import <Platform/iOS/LynxView.h>
#import <Platform/iOS/LynxViewController.h>
#import <Platform/iOS/LynxiOSStoreKit.h>
#import <CoreMotion/CoreMotion.h>
#import <CoreLocation/CoreLocation.h>

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

namespace LynxEngine
{
    namespace GameFramework
    {
        namespace iOS
        {
            class LYNXENGCLASS CKernel : public LynxEngine::GameFramework::CKernel
            {
                LYNX_DECLARE_CLASS(CKernel);
            public:
            protected:
                LynxViewController*					m_lpViewController;
                LynxView*							m_lpView;
                CMMotionManager*                    m_MotionManager;
                CLLocationManager*					m_LocationManager;
                LYNXBOOL                            m_bCameraCapture;
                DWORD                               m_CameraCaptureWidth;
                DWORD                               m_CameraCaptureHeight;
                const void*                         m_CameraCaptureRawData;
                CMap< CString, UIView* >            m_OSNViewMap;
                LynxStoreKit*                       m_lpStoreKit;
                
            public:
                CKernel();
                virtual ~CKernel();
                
                virtual LYNXBOOL					vInit(const int argc, const char **argv, INIT_DESC* desc = NULL, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);
                virtual CEngine*					vCreateEngine();
                LYNXBOOL                            vLoop();
                void								vPause();
                void								vResume();
                void								vTerminate();
                void                                vSwapBackBuffer();
                void								vSetMainContext();
                void								vSetLoadingContext();
                
                CString                             vGetGameVersion();
                void								vGenerateUDID(LYNXBOOL bforce_generate_new_udid = LYNX_FALSE);

                float                               GetViewScaleFactor();
                LYNXFORCEINLINE LynxViewController*	GetlpViewController() {return m_lpViewController; };
                LYNXFORCEINLINE void				SetlpViewController(LynxViewController* lpvc) {m_lpViewController = lpvc; };
                LYNXFORCEINLINE LynxView*			GetlpView() {return m_lpView; };
                LYNXFORCEINLINE void				SetlpView(LynxView* lpv) {m_lpView = lpv; };
                LYNXFORCEINLINE CMMotionManager*	GetMotionManager() {return m_MotionManager; };
                LYNXFORCEINLINE CLLocationManager*	GetLocationManager() {return m_LocationManager; };
                void                                vSetCurrentContext();
                
                // Gesture
                void								vSetGestureRecognization(LYNXBOOL b);
                void								vSetTapGestureRecognization(LYNXBOOL b);
                void								vSetSwipeGestureRecognization(LYNXBOOL b);
                void								vSetPinchGestureRecognization(LYNXBOOL b);
                void								vSetRotationGestureRecognization(LYNXBOOL b);
                void								vSetLongPressGestureRecognization(LYNXBOOL b);
                void								vSetPanGestureRecognization(LYNXBOOL b);
                void                                vSetGestureState(int s);
                
                void                                EnableCameraCapture(LYNXBOOL b);
                void                                StartCameraCapture();
                void                                StopCameraCapture();
                LYNXFORCEINLINE const DWORD         GetCameraCaptureWidth() const {return m_CameraCaptureWidth; };
                LYNXFORCEINLINE const DWORD         GetCameraCaptureHeight() const {return m_CameraCaptureHeight; };
                LYNXFORCEINLINE void                SetCameraCaptureWidth(DWORD w) {m_CameraCaptureWidth = w; };
                LYNXFORCEINLINE void                SetCameraCaptureHeight(DWORD h) {m_CameraCaptureHeight = h; };
                LYNXFORCEINLINE void                SetCameraCaptureRawData(const void* data) {m_CameraCaptureRawData = data; };
                LYNXFORCEINLINE const void*         GetCameraCaptureRawData() const {return m_CameraCaptureRawData; };
                
                void								PrintEngineInfo();
                
                void                                SetDeviceOrientationByInterfaceOrientation(UIInterfaceOrientation o);
                void                                SetDeviceOrientation(UIDeviceOrientation o);

				CString								vGetSystemLanguageCode();

				// 
                void								vOpenWebView(const CString& url, const CString& title, LYNXRECT& rect);
                void                                vOpenCustomWebview(const CString& name, LYNXRECT& rect, const CString& title, const CString& htmlOrUrl);
                void								vOpenTextInput(const CString& name, GUISystem::CText* text);
				void                                vOSN_OpenMessageBox(const CString& title, const CString& text);
                
				// IAP
                void                                vQueryIAPList(const CString& base64EncodedPublicKey, const CArray<CString>& list);
				void                                vPurchaseIAP(const CString& productId, int requestCode, const CString& playerSN);
				
				// Push Notification
				void                                vRegisterRemoteNotification();
				void								vScheduleLocalNotification(int notif_id, const CString& title, const CString& context, const CString& ringtone, int invokeTimeInSeconds);
				void								vCancelLocalNotification(int notif_id);

				// FaceBook
				void								vFacebookLoginWithPermission(const CString& permissions);
				//void								vOnFinishFacebookLogin(const CString& result);
				void								vFacebookLogout();
				void								vQueryFacebookAccountData(const CString& permissions, const CString& fields);
				void								vOnFinishQueryFacebookAccountData(const CString& JSONResult);
                void								vQueryFacebookFriendsData(const CString& permissions, const CString& fields); // ex: vQueryFacebookFriendsData("email,user_friends", "id");
				void								vOnFinishQueryFacebookFriendsData(const CString& JSONResult);
				void								vFacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl);
				void								vOnFinishFacebookShareLink(const CString& JSONResult);
				void								vFacebookUploadPic(const CString& filepath);
				void								vOnFinishFacebookUploadPic(const CString& JSONResult);
				void                                FacebookAccountDataRequest(const CString& permission, const CString& fields);
				LYNXBOOL                            HasPermissionFacebook(const CString& permissions);
                void                                FacebookFriendsDataRequest(const CString& permission,const CString& fields);
				void								FacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl);
				void								FacebookUploadPic(const CString& filepath);
                CString                             vFacebookGetAccessToken();
				virtual void                        vShareWithSelectApp(const CString& msg, const CString& imagePath, LYNXBOOL isInternalPath = LYNX_TRUE);
                
                // firebase.
                CString                             vFirebaseGetAccessToken();
                
                // OS native UI
				void                                vOSN_AddColorBackground(const CString& name, LYNXCOLORRGBA color);
				void                                vOSN_AddAnimationView(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect);
				void                                vOSN_AddTextView(const CString& name, const CString& filename, LYNXRECT& rect);
				void                                vOSN_AddWebview(const CString& name, LYNXRECT& rect, const CString& htmlOrUrl, const LYNXBOOL bNotFullScreen = LYNX_FALSE);
				void                                vOSN_RemoveView(const CString& name);
				void                                vOSN_ShowView(const CString& name, LYNXBOOL bshow);
				void						        vOSN_HideAllViews();
				void                                vOSN_MoveView(const CString& name, LYNXRECT& rect, const LYNXBOOL bNotFullScreen = LYNX_FALSE);
				void                                vOSN_ReleaseView(const CString& name);
                void								vOSN_OpenURL(const CString& url);
                
                
                void                                vRenderingThreadBegin();
                void                                vRenderingThreadEnd();

				// OS sleep mode
                void                                vSetDeviceSleepMode(LYNXBOOL b);

				// rating dialog
				void								vOpenRatingDialog(const CString& title, const CString& text, const CString& positive_text, const CString& negative_text, CString neutral_text = CString(_T("")));

            protected:
                void                                SetupDeviceType();
                void                                SetupCPUClass();
                void                                SetupGPUClass();
                void                                SetupMemoryClass();
            };
        }
    }
}
#endif
