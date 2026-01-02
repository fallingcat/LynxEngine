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

#ifndef __LYNXANDROIDGAMEFRAMEWORKKERNEL_H__
#define __LYNXANDROIDGAMEFRAMEWORKKERNEL_H__

#include <LynxObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GameFramework/LynxUIPage.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <Platform/Android/android_native_app_glue.h>

namespace LynxEngine 
{
	namespace GameFramework 
	{		
		namespace Android
		{
			class LYNXENGCLASS CKernel : public LynxEngine::GameFramework::CKernel
			{	
				LYNX_DECLARE_CLASS(CKernel);
			protected:					
				//jobject							m_lpKernel;
				//jobject							m_lpView;		
				android_app*						m_App;
				AAssetManager*						m_AssetManager;
				//ASensorManager*					m_SensorManager;								
				//ASensor*							m_AccelerometerSensor;
				//ASensorEventQueue*				m_SensorEventQueue;
				JavaVM*								m_JavaVM;				
				JNIEnv*								m_JNIEnv;
				static CMap<CString, DEVICECLASS>	m_CPUClassTable;
				static CMap<CString, DEVICECLASS>	m_GPUClassTable;

				CList<CString>						m_OSNViewList;				
			public:			
				CKernel();
				virtual ~CKernel();			

				virtual void						vCreateMemAllocator();
				virtual LYNXBOOL					vInit(const int argc, const char **argv, INIT_DESC* desc = NULL, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);			
				virtual CEngine*					vCreateEngine();		
				virtual	LYNXBOOL					vLoop();
				void								vPause();
				void								vResume();
				virtual void						vExitApp();
				//LYNXFORCEINLINE void				SetlpKernel(jobject lpk) {m_lpKernel = lpk; };				
				//LYNXFORCEINLINE jobject				GetlpView() {return m_lpView; };
				//LYNXFORCEINLINE void				SetlpView(jobject lpv) {m_lpView = lpv; };								
				
				void								vSetMainContext();
				void								vSetLoadingContext();
				CString								vGetSystemLanguageCode();

				void								PrintEngineInfo();
				
				void								SetApp(android_app* app);
				LYNXFORCEINLINE android_app*		GetApp() const {return m_App; }		
				LYNXFORCEINLINE void				SetAssetManager(AAssetManager* am) {m_AssetManager = am; }		
				LYNXFORCEINLINE AAssetManager*		GetAssetManager() const {return m_AssetManager; }		
				
				LYNXFORCEINLINE JavaVM*				GetJavaVM() {return m_JavaVM; }
				LYNXFORCEINLINE JNIEnv*				GetJavaEnv() {return m_JNIEnv; }

				static void							GetJavaClassName(JavaVM* vm, jobject jobj);
				static jmethodID					GetJavaMethodID(JNIEnv *env, jobject jobj ,const CString& functionName, const CString& signature);

				void								vSetGestureRecognization(LYNXBOOL b);
				void								vSetTapGestureRecognization(LYNXBOOL b);
				void								vSetSwipeGestureRecognization(LYNXBOOL b);
				void								vSetPinchGestureRecognization(LYNXBOOL b);
				void								vSetRotationGestureRecognization(LYNXBOOL b);
				void								vSetLongPressGestureRecognization(LYNXBOOL b);
				void								vSetPanGestureRecognization(LYNXBOOL b);
                void                                vSetGestureState(int s);

				// 
				void								vOpenWebView(const CString& url, const CString& title, LYNXRECT& rect);
				void								vOpenCustomWebview(const CString name, LYNXRECT& rect, const CString& title, const CString& htmlOrUrl);
				void								vOpenTextInput(const CString& name, GUISystem::CText* text);
				void								vOpenTextInput(const CString& name, const CString& displayText, GUISystem::CText* text);
				void								vAddAnimationLayer(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect);
				void								vCloseAnimationLayer();

				// IAP
				void								vQueryIAPList(const CString& base64EncodedPublicKey, const CArray<CString>& list);
				void								vPurchaseIAP(const CString& productId, int requestCode, const CString& playerSN);
				void								vConsumePurchase();

				// Push Notification			
				void								vRegisterRemoteNotification() { /* DO NOTHING */ }
				void								vScheduleLocalNotification(int id, const CString& title, const CString& context, const CString& ringtone, int invokeTimeInSeconds);
				void								vCancelLocalNotification(int id);
				
				// FaceBook
				void								vFacebookLoginWithPermission(const CString& permissions);
				void								vFacebookLogout();
				void								vQueryFacebookAccountData(const CString& permissions, const CString& fields);
				void								vOnFinishQueryFacebookAccountData(const CString& JSONResult);
				void								vQueryFacebookFriendsData(const CString& permissions, const CString& fields); // ex: vQueryFacebookFriendsData("email,user_friends", "id");
				void								vOnFinishQueryFacebookFriendsData(const CString& JSONResult);
				void								vFacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl);
				void								vOnFinishFacebookShareLink(const CString& JSONResult);
				void								vFacebookUploadPic(const CString& filepath);
				CString								vFacebookGetAccessToken();
				void								vOnFinishFacebookUploadPic(const CString& JSONResult);
				
				// firebase.
				virtual	CString						vFirebaseGetAccessToken();
				
				// share.
				void							    vShareWithSelectApp(const CString& msg, const CString& imagePath, LYNXBOOL isInternalPath = LYNX_TRUE);

				// OS native UI
				void								vOSN_AddColorBackground(const CString& name, LYNXCOLORRGBA color);
				void								vOSN_AddAnimationView(const CString& name, const CString& animfile, const CString& ext, int num, int dur, LYNXRECT& rect);
				void								vOSN_AddTextView(const CString& name, const CString& filename, LYNXRECT& rect);
				void								vOSN_AddWebview(const CString& name, LYNXRECT& rect, const CString& htmlOrUrl, const LYNXBOOL bNotFullScreen = LYNX_FALSE);
				void								vOSN_RemoveView(const CString& name);
				void								vOSN_ShowView(const CString& name, LYNXBOOL bshow);
				void								vOSN_MoveView(const CString& name, LYNXRECT& rect, const LYNXBOOL bNotFullScreen = LYNX_FALSE);
				void								vOSN_HideAllViews();
				void								vOSN_ReleaseView(const CString& name);
				void								vOSN_OpenURL(const CString& url);
															
				// webView
				void								OpenWebView(const CString& url, LYNXRECT rect, const CString& title, const CString& button_title, int button_width, int button_height, LYNXCOLORRGBA bg_color);
				// alert dialog
				void								OpenAlertDialog(const CString& title, const CString& message, const CString& ok, const CString& middle, const CString& cancel, const CArray<CString>& vParamStr);
								
				CString                             vGetGameVersion();

				// OS sleep mode
				void								vSetDeviceSleepMode(LYNXBOOL b);
				
				// rating dialog
				void								vOpenRatingDialog(const CString& title, const CString& text, const CString& positive_text, const CString& negative_text, CString neutral_text = CString(_T("")));

			protected:
				void                                SetupDeviceType();
				void                                SetupCPUClass();
				void                                SetupGPUClass();
			};
		}
	}
}
#endif