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

#ifndef __LYNXGAMEFRAMEWORKKERNEL_H__
#define __LYNXGAMEFRAMEWORKKERNEL_H__

#include <LynxObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GameFramework/LynxUIPage.h>
#include <LynxStoreKit.h>

namespace LynxEngine 
{
	class CwxTextOutputDevice;	
	class CGame;	
	namespace GameFramework 
	{		
		class LYNXENGCLASS CKernel
		{	
			LYNX_DECLARE_CLASS(CKernel);
		public:									
			struct INIT_DESC {								
				GraphicsSystem::CRenderer::INITGRAPHICPARAM GraphicParam;
			};
			WORD								m_CharPressed;
			BYTE								m_KeyPressed;	
			LYNXBOOL							m_KBD[256];	
			LYNXBOOL							m_KBDUP[256];
			float								m_WheelDist;
			Math::CVector2						m_OffscreenSwipeDir;

            static const int                    TAP_GESTURE         = 1;
            static const int                    SWIPE_GESTURE       = 2;
            static const int                    PINCH_GESTURE       = 4;
            static const int                    ROTATION_GESTURE    = 8;
            static const int                    LONGPRESS_GESTURE   = 16;
            static const int                    PAN_GESTURE         = 32;
            
			ControlPanel::CControlPanel*		m_lpControlPanel;
			float								m_Step;
			DEVICEORIENTATIONMASK				m_DeviceOrientationMask;
			LYNXMUTEX							m_Mutex;
            LYNXBOOL                            m_bFadeIn;
			LYNXLONGLONG						m_InitFreeMemSize;
			float								m_InputScaleFactor;
			LYNXBOOL							m_bRenderBorder;

			CAnsiString							m_DeviceBrand;
			CAnsiString							m_DeviceModel;
			CAnsiString							m_DeviceOSVersion;
			CAnsiString							m_DeviceCPU_ABI;
			CMap<CString, CString>				m_DeepLinkPostProcParms;//等建立 Game 後才會處理的參數.
			CAnsiString							m_NetworkType;			//(WIFI / 3G / 4G).
			CAnsiString							m_TelCompany;			//電信商名稱.
			CAnsiString							m_DeviceManufacturer;
			CAnsiString							m_DeviceProduct;

		protected:					
            DEVICETYPE                          m_DeviceType;			
			DEVICECLASS                         m_CPUClass;
			DEVICECLASS                         m_GPUClass;
			DEVICECLASS                         m_MemoryClass;
            DEVICEORIENTATION                   m_DeviceOrientation;			
			CAnsiString                         m_UDIDString;
			CAnsiString                         m_AdvertisingIDString;
			CAnsiString                         m_DeviceToken;			

			LYNXWINDOW							m_Window;
			LYNXBOOL							m_bRunning;
			LYNXBOOL							m_bMultiTask;
			LYNXBOOL							m_bWaitMessage;			
			LYNXBOOL							m_bActive;			
			LYNXCHAR							m_CallStack[4096];
			//static CMemAllocator*				m_lpMemAllocator;
			LYNXBOOL							m_bMemoryHackingDetected;

			CGame*								m_lpGame;
			CEngine*							m_lpEngine;	
			LYNXBOOL							m_bPause;
			LYNXBOOL							m_bPauseRendering;
			CFactory<CUIPage, CString, CKernel*> m_UIPageFactory;
			CList<CUIPage*>						m_lpUIPageList;
			CUIPage*							m_lpLastUIPage;
			CUIPage*							m_lpCurrentUIPage;			
			GUISystem::CContext*                m_CurrentInputFocusedContext;
            GUISystem::CWindow*					m_LastModalWindow;
			GUISystem::CWindow*					m_ModalWindow;

            LYNXBOOL							m_bGestureRecognization;
            int                                 m_GestureState;
            
			CSpriteContainer*					m_OffscreenSprite;
			CSpriteContainer*					m_ScreenFilterSprite;
			CSpriteContainer*					m_Border[2];

			UITRASITIONMODE						m_UITransitionMode;
			LYNXBOOL							m_bUITransiting;
			Math::CVector2						m_OffscreenPos;
			Math::CReal							m_OffscreenVelocity;
			Math::CReal							m_FadeOutSpeed;		
            Math::CReal                         m_RollingSpeed;
			
			LYNXBOOL							m_bPauseGameTime;
			LYNXLONGLONG						m_CurrentGameTime;
			LYNXLONGLONG						m_PauseTime;
			LYNXLONGLONG						m_LastFrameTime;
			DWORD								m_FrameCount;
			float								m_FrameElapsedTime;
			float								m_TotalFrameElapsedTime;

			CTransactionRecord					m_TransactionRecord;
		private:
			void								InitMembers();				
		public:			
			CKernel();
			virtual ~CKernel();			

			virtual void						vCreateMemAllocator();
			//static LYNXFORCEINLINE CMemAllocator* GetMemAllocator() {return m_lpMemAllocator; };

			LYNXFORCEINLINE CSpriteContainer*	GetScreenFilterSprite() { return m_ScreenFilterSprite; }

			virtual void						vLoadBorder();
			virtual LYNXBOOL					vInit(const int argc, const char **argv, INIT_DESC* desc = NULL, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);			
			virtual CEngine*					vCreateEngine();	
			virtual CGame*						vCreateGame();
			virtual LYNXBOOL					vLoop();
			virtual void						vPause();
			virtual void						vPauseRendering();
			virtual void						vResume();
			virtual void						vResumeRendering();
			virtual void						vTerminate();
			virtual void						vRender();
			virtual void						vRenderBorder();
            virtual void                        vSwapBackBuffer();
			virtual void						vSetMainContext() {};
			virtual void						vSetLoadingContext() {};
			virtual void						vRelease();
			virtual void						vExitApp(){};
			virtual void						vPostProcDeepLinkParmParam(){};

			static void							LogUnreleasedMemoryBlocks(void);

			LYNXFORCEINLINE CGame*				GetlpGame() {return m_lpGame; };
            LYNXFORCEINLINE DEVICETYPE          GetDeviceType() const {return m_DeviceType;};
			LYNXFORCEINLINE DEVICECLASS         GetCPUClass() const {return m_CPUClass;};
			LYNXFORCEINLINE DEVICECLASS         GetGPUClass() const {return m_GPUClass;};
			LYNXFORCEINLINE DEVICECLASS			GetMemoryClass() const {return m_MemoryClass;};
            LYNXFORCEINLINE DEVICEORIENTATION   GetDeviceOrientation() const {return m_DeviceOrientation;};
            virtual CString						vGetGameVersion();
			virtual void						vGenerateUDID(LYNXBOOL bforce_generate_new_udid = LYNX_FALSE);
			LYNXFORCEINLINE void				SetUDIDString(const CAnsiString& udid) {m_UDIDString = udid; };
			LYNXFORCEINLINE const CAnsiString&  GetUDIDString() const { return m_UDIDString; };
			LYNXFORCEINLINE void				SetAdvertisingIDString(const CAnsiString& id) { m_AdvertisingIDString = id; };
			LYNXFORCEINLINE const CAnsiString&  GetAdvertisingIDString() const { return m_AdvertisingIDString; };
			
            virtual void						vGenerateDeviceToken();
			LYNXFORCEINLINE void				SetDeviceToken(const CAnsiString& t) {m_DeviceToken = t; };
			LYNXFORCEINLINE const CAnsiString&  GetDeviceToken() const {return m_DeviceToken; };					    
            
            virtual void                        vRegisterRemoteNotification() {}
			virtual void						vScheduleLocalNotification(int id, const CString& title, const CString& context, const CString& ringtone, int invokeTimeInSeconds) {}
			virtual void						vCancelLocalNotification(int id) {}

			void								SetMemoryHackingDetectionFlag(LYNXBOOL b) { m_bMemoryHackingDetected = b; }
			LYNXBOOL							IsMemoryHackingDetected() { return m_bMemoryHackingDetected; }
			
			virtual CString						vGetSystemLanguageCode();
                
			float								GetCurrentFPS();
			static float						GetFreePhysicalMemorySize();
			static float						GetUsedPhysicalMemorySize();
			LYNXINLINE const DWORD				GetFrameCount() {return m_FrameCount; };
			LYNXINLINE const float				GetFrameElapsedTime() const {return m_FrameElapsedTime; };		
			LYNXINLINE const float				GetTotalFrameElapsedTime() const {return m_TotalFrameElapsedTime; };		

			LYNXINLINE const LYNXWINDOW&		GetWindow() const {return m_Window;};	
			LYNXINLINE void						SetWindow(const LYNXWINDOW& w) {m_Window = w;};
			
			LYNXINLINE void						Quit() {m_bRunning = LYNX_FALSE; };	
			LYNXINLINE void						SetRunning(LYNXBOOL b) {m_bRunning = b; };	
			LYNXINLINE LYNXBOOL					IsRunning() { return m_bRunning; };	
			LYNXFORCEINLINE	void				ResetPauseTime() {m_PauseTime = 0; };
			void								PauseGameTime(LYNXBOOL b);
			LYNXLONGLONG						GetKernelTime();
            static float                        ElapsedKernelTime(const LYNXLONGLONG t0, const LYNXLONGLONG t1);
			float				                ElapsedKernelTimeSince(const LYNXLONGLONG t0);
			LYNXLONGLONG						GetGameTime();            
			float				                ElapsedGameTimeSince(const LYNXLONGLONG t0);
			LYNXLONGLONG						ElapsedTimeStampSince(const LYNXLONGLONG t0);

			LYNXFORCEINLINE void				SetUITransitionMode(UITRASITIONMODE m) {m_UITransitionMode = m;};			
			LYNXFORCEINLINE	UITRASITIONMODE		GetUITransitionMode() const {return m_UITransitionMode; }
			LYNXFORCEINLINE CSpriteContainer*	GetOffscreenSprite() {return m_OffscreenSprite; };
			LYNXFORCEINLINE void				SetSwipeSpeed(float s) {m_OffscreenVelocity = s; };
			LYNXFORCEINLINE void				SetFadeOutSpeed(float s) {m_FadeOutSpeed = s; };
            LYNXFORCEINLINE void				SetRollingSpeed(float s) {m_RollingSpeed = s; };
			LYNXFORCEINLINE LYNXBOOL			IsUITransiting() const {return m_bUITransiting; };			
			LYNXFORCEINLINE void				Transit(LYNXBOOL b) {m_bUITransiting = b; };			

			void								RegisterUIPage(const CString& t, CUIPage* (*func)(CKernel*));
			LYNXFORCEINLINE CList<CUIPage*>&	GetUIPageList() {return m_lpUIPageList; };

			CUIPage*							CreateUIPage(const CString& name);
			CUIPage*							FindUIPage(const CString& name);
			LYNXFORCEINLINE void				AddUIPage(CUIPage* lpu) {m_lpUIPageList.push_back(lpu); };
			LYNXFORCEINLINE void				RemoveUIPage(CUIPage* lpu) {m_lpUIPageList.remove(lpu); };
			LYNXFORCEINLINE void				SetCurrentUIPage(CUIPage* lpu) {m_lpLastUIPage = m_lpCurrentUIPage; m_lpCurrentUIPage = lpu; if (m_lpLastUIPage) m_lpCurrentUIPage->SetLastUIPageName(m_lpLastUIPage->GetName()); };
			LYNXFORCEINLINE CUIPage*			GetLastUIPage() {return m_lpLastUIPage; };
			LYNXFORCEINLINE CUIPage*			GetCurrentUIPage() {return m_lpCurrentUIPage; };
			LYNXFORCEINLINE GUISystem::CWindow*	GetModalWindow() {return m_ModalWindow; }
			void								SetModalWindow(GUISystem::CWindow* w);
			
			LYNXINLINE void						SetMultiTask(LYNXBOOL b) {m_bMultiTask = b; };	
			LYNXINLINE LYNXBOOL					IsMultiTask() { return m_bMultiTask; };	

			LYNXINLINE CEngine*					GetlpEngine() {return m_lpEngine;};	

			void								SetTitle(LYNXCHAR *title);

			LYNXBOOL							GetEvent();
            
            LYNXFORCEINLINE GUISystem::CContext* GetCurrentFocusedContext() const {return m_CurrentInputFocusedContext; }
            LYNXFORCEINLINE void                SetCurrentFocusedContext(GUISystem::CContext* c) {m_CurrentInputFocusedContext = c; }
            void                                LoseInputFocus(GUISystem::CContext* c);
            
            virtual void                        vRenderingThreadBegin() {};
            virtual void                        vRenderingThreadEnd() {};
            virtual void                        vOnLowMemoryWarning();

			virtual void						vOpenTextInput(const CString& name, GUISystem::CText* text) {};
			virtual void						vOpenTextInput(const CString& name, const CString& displayText, GUISystem::CText* text) {}
			virtual void						vAddAnimationLayer(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect) {};
			virtual void						vCloseAnimationLayer() {};

			virtual void						vQueryIAPList(const CString& base64EncodedPublicKey, const CArray<CString>& list) {};
			virtual void						vOnFinishingQueryIAPList(const CArray<CJsonValue>& list) {};
			virtual void						vPurchaseIAP(const CString& productId, int requestCode, const CString& playerSN) {};
			virtual LYNXBOOL					vOnVerifyPurchase(const CJsonValue& purchase)  { return LYNX_TRUE; };
			virtual LYNXBOOL					vOnVerifyPromoCodePurchase(const CJsonValue& purchase)  { return LYNX_TRUE; };
			virtual void						vOnDeliverPurchase(int result, const CString& msg, CJsonValue& purchase) {};
			virtual void						vOnErrorPurchase(int result, const CString& msg) {};
			virtual void						vOnFinishingPurchase(int result, const CString& msg) {};
			
			virtual void						vFacebookLoginWithPermission(const CString& permissions){};//純登入要權限,不做事.
			virtual void						vOnFinishFacebookLogin(const CString& result){};
			virtual void						vFacebookLogout(){};
			virtual void						vQueryFacebookAccountData(const CString& permissions, const CString& fields) {};
			virtual	void						vOnFinishQueryFacebookAccountData(const CString& JSONResult) {};
			virtual void						vQueryFacebookFriendsData(const CString& permissions, const CString& fields){}; // ex: vQueryFacebookFriendsData("email,user_friends", "id");
			virtual void						vOnFinishQueryFacebookFriendsData(const CString& JSONResult){};
			virtual void						vQueryTwitterAccountData(const CString& permissions, const CString& fields) {};
			virtual	void						vOnFinishQueryTwitterAccountData(const CString& JSONResult) {};

			virtual	void						vFacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl) {};
			virtual	void						vOnFinishFacebookShareLink(const CString& JSONResult) {};

			virtual	void						vFacebookUploadPic(const CString& filepath){};
			virtual	CString						vFacebookGetAccessToken(){ return "";};
			virtual	void						vOnFinishFacebookUploadPic(const CString& JSONResult) {};

			virtual	CString						vFirebaseGetAccessToken(){ return ""; };
			virtual	void						vOnFirebaseUpdateAccessToken(){};
			virtual void						vCrashDump(LYNXBOOL succeeded, const CString& filename) {};

			virtual void                        vShareWithSelectApp(const CString& msg, const CString& imagePath, LYNXBOOL isInternalPath = LYNX_TRUE){};
            
			// Gesture related functions
			LYNXFORCEINLINE LYNXBOOL            IsGestureRecognization() {return m_bGestureRecognization; };
			virtual void						vSetGestureRecognization(LYNXBOOL b) {};
			virtual void						vSetTapGestureRecognization(LYNXBOOL b) {};
			virtual void						vSetSwipeGestureRecognization(LYNXBOOL b) {};
			virtual void						vSetPinchGestureRecognization(LYNXBOOL b) {};
			virtual void						vSetRotationGestureRecognization(LYNXBOOL b) {};
			virtual void						vSetLongPressGestureRecognization(LYNXBOOL b) {};
			virtual void                        vSetPanGestureRecognization(LYNXBOOL b) {};
			virtual void                        vSetGestureState(int s) {};
            LYNXFORCEINLINE int                 GetGestureState() {return m_GestureState; }
            LYNXFORCEINLINE LYNXBOOL            GetGestureState(int s) {return (LYNXBOOL)(m_GestureState&s); }
            
			virtual void						vHandleURLQueryParam(const CString& key, const CString& value) {};
			virtual void						vOpenWebView(const CString& url, const CString& title, LYNXRECT& rect) {};
            virtual void                        vOpenCustomWebview(const CString& name, LYNXRECT& rect, const CString& title, const CString& htmlOrUrl) {};
			virtual void						vOpenTextInputView(const CString& name, GUISystem::CText* text) {};

			// OS native UI
            virtual void                        vOSN_AddColorBackground(const CString& name, LYNXCOLORRGBA color) {};
			virtual void						vOSN_AddAnimationView(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect) {};
			virtual void						vOSN_AddTextView(const CString& name, const CString& filename, LYNXRECT& rect) {};
			virtual	void                        vOSN_AddWebview(const CString& name, LYNXRECT& rect, const CString& htmlOrUrl, const LYNXBOOL bNotFullScreen = LYNX_FALSE) {};
			virtual void                        vOSN_RemoveView(const CString& name) {};
			virtual void                        vOSN_ShowView(const CString& name, LYNXBOOL bshow) {};
			virtual void                        vOSN_MoveView(const CString& name, LYNXRECT& rect, const LYNXBOOL bNotFullScreen = LYNX_FALSE) {};
			virtual void                        vOSN_ReleaseView(const CString& name) {};
			virtual void                        vOSN_HideAllViews() {};

			virtual void                        vOSN_OpenMessageBox(const CString& title, const CString& text) {};
			virtual void						vOSN_OpenURL(const CString& url) {};
						
			// Event handling functions
			virtual void						vOnSysKeyDown(int key);
			virtual void						vOnSysKeyUp(int key);		
			virtual void						vOnSysBackPressed() {};

			// OS sleep mode
			virtual void						vSetDeviceSleepMode(LYNXBOOL b) {};
            virtual void                        vSetCurrentContext() {};
			
			// rating dialog
			virtual void						vOpenRatingDialog(const CString& title, const CString& text, const CString& positive_text, const CString& negative_text, CString neutral_text = CString(_T(""))) {}
			virtual void						vOnClickRatingDialog(int buttonIndex) {};

		protected:
			void                                SetupDeviceType();
			void                                SetupCPUClass();
			void                                SetupGPUClass();
			void                                SetupMemoryClass();
			void								RenderUITransition();
			void								SwipeOffscreen();
			void								FadeOut();
			void								AlphaFadeOut();
			void								ScaleFadeOut();
            void                                ScaleRollFadeOut();
		};
	}
}
#endif
