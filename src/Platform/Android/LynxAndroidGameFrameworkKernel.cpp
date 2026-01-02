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

#include <LynxEngine_PCH.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <GameFramework/LynxUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <InputSystem/LynxInputSystem.h>
#include <Platform/Android/LynxAndroidEngine.h>
#include <Platform/Android/LynxAndroidGraphicsSystem.h>
#include <Platform/Android/LynxAndroidGameFrameworkKernel.h>
#include <Platform/Android/LynxAndroidCrashdump.h>

namespace LynxEngine 
{
	namespace GameFramework 
	{			
		namespace Android
		{
			CMap<CString, DEVICECLASS>	CKernel::m_CPUClassTable;
			CMap<CString, DEVICECLASS>	CKernel::m_GPUClassTable;
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CKernel::CKernel(void)
			{
				m_AssetManager = NULL;
				m_JavaVM = NULL;
				m_JNIEnv = NULL;

				m_GPUClassTable[_T("Adreno (TM) 330")] = DC_HIGH;
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CKernel::~CKernel(void)
			{
				LYNXDEL_ARRAY(gAndroidInternalPath);
				m_OSNViewList.clear();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------			
			CEngine* CKernel::vCreateEngine() 
			{
				CEngine* lpEngine = LYNXNEW LynxEngine::Android::CEngine(this);
				
				return  lpEngine; 
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vCreateMemAllocator()
			{	
				//m_lpMemAllocator = NULL;				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::SetApp(android_app* app) 
			{
				m_App = app; 
				SetAssetManager(app->activity->assetManager);
				SetWindow(app->window);
				m_JavaVM = app->activity->vm;
				m_JNIEnv = app->activity->env;

				gAndroidInternalPath = LYNXNEW LYNXCHAR[256];
				strcpy(gAndroidInternalPath, app->activity->internalDataPath);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CKernel::vInit(const int argc, const char **argv, INIT_DESC* desc, const int w, const int h, const LYNXBOOL bfull)
			{				
				// Inject goorgle-breakpad here.
				LynxEngine::Android::CCrashDumpSystem::vInit(gAndroidInternalPath);				
				
				m_CurrentGameTime = 0;

				m_lpEngine = vCreateEngine();
				m_lpEngine->vCreate(desc);

				if (desc)
				{
					if (!((CEngine*)m_lpEngine)->vInit(m_Window, &(desc->GraphicParam)))
						return LYNX_FALSE;
				}
				else 
				{
					if (!((CEngine*)m_lpEngine)->vInit(m_Window, w, h, bfull))
						return LYNX_FALSE;
				}			
				SetRunning(LYNX_TRUE);
							
				SetupDeviceType();
				SetupCPUClass();
				SetupGPUClass();

				if (m_OffscreenSprite)
					m_lpEngine->DestroyContainer(m_OffscreenSprite);
				if (m_ScreenFilterSprite)
					m_lpEngine->DestroyContainer(m_ScreenFilterSprite);

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
				//this->vSetGestureRecognization(LYNX_TRUE);
            
				vLoadBorder();

				m_bFadeIn = LYNX_FALSE;
				m_GestureState = 0;
				vSetTapGestureRecognization(LYNX_TRUE);
				vSetSwipeGestureRecognization(LYNX_TRUE);
				vSetPinchGestureRecognization(LYNX_FALSE);
				vSetRotationGestureRecognization(LYNX_FALSE);
				vSetLongPressGestureRecognization(LYNX_TRUE);
				vSetPanGestureRecognization(LYNX_FALSE);

				//lynxOSCreateMutex(&m_Mutex);
            
				//LYNXRECT Rect;
				//Rect.x1 = 0; Rect.x2 = GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(); Rect.y1 = 0; Rect.y2 = GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight();
				//m_OffscreenSprite->SetAlignmentRect(&Rect);
				//m_OffscreenSprite->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
						
				return LYNX_TRUE;
			}


			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::SetupDeviceType()
			{
				JNI_ATTACH_CURRENT_THREAD

				jclass build_class = env->FindClass("android/os/Build");
				jclass versionClass = env->FindClass("android/os/Build$VERSION");
				jfieldID brand_id = env->GetStaticFieldID(build_class, "BRAND", "Ljava/lang/String;");
				jfieldID model_id = env->GetStaticFieldID(build_class, "MODEL", "Ljava/lang/String;");
				jfieldID cpuabi_id = env->GetStaticFieldID(build_class, "CPU_ABI", "Ljava/lang/String;");
				jfieldID manufacturer_id = env->GetStaticFieldID(build_class, "MANUFACTURER", "Ljava/lang/String;");
				jfieldID product_id = env->GetStaticFieldID(build_class, "PRODUCT", "Ljava/lang/String;");				
				jfieldID versionrelease_id = env->GetStaticFieldID(versionClass, "RELEASE", "Ljava/lang/String;");

				jstring brand_strobj = (jstring)env->GetStaticObjectField(build_class, brand_id);
				jstring model_strobj = (jstring)env->GetStaticObjectField(build_class, model_id);
				jstring cpuabi_strobj = (jstring)env->GetStaticObjectField(build_class, cpuabi_id);
				jstring manufacturer_strobj = (jstring)env->GetStaticObjectField(build_class, manufacturer_id);
				jstring product_strobj = (jstring)env->GetStaticObjectField(build_class, product_id);
				jstring versionrelease_strobj = (jstring)env->GetStaticObjectField(versionClass, versionrelease_id);


				const char *brandstr = env->GetStringUTFChars(brand_strobj, 0);
				const char *modelstr = env->GetStringUTFChars(model_strobj, 0);
				const char *cpuabistr = env->GetStringUTFChars(cpuabi_strobj, 0);
				const char *manufacturerstr = env->GetStringUTFChars(manufacturer_strobj, 0);
				const char *productstr = env->GetStringUTFChars(product_strobj, 0);
				const char *versionreleasestr = env->GetStringUTFChars(versionrelease_strobj, 0);

				{
					android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
					jclass cls = env->GetObjectClass(app->activity->clazz);
					jmethodID mtehod = env->GetMethodID(cls, "GetNetworkType", "()Ljava/lang/String;");
					jstring str = (jstring)env->CallObjectMethod(app->activity->clazz, mtehod);
					const char* c = env->GetStringUTFChars(str, 0);
					CString NewtorkType = _T(c);
					LYNX_LOG(glpLogger, CString(_T("networktest GetNetworkType:") + NewtorkType));
					env->ReleaseStringUTFChars(str, c);
					env->DeleteLocalRef(str);
					m_NetworkType = NewtorkType;
				}

				{
					android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
					jclass cls = env->GetObjectClass(app->activity->clazz);
					jmethodID mtehod = env->GetMethodID(cls, "GetNetworkOperatorName", "()Ljava/lang/String;");
					jstring str = (jstring)env->CallObjectMethod(app->activity->clazz, mtehod);
					const char* c = env->GetStringUTFChars(str, 0);
					CString OperatorName = _T(c);
					LYNX_LOG(glpLogger, CString(_T("networktest GetNetworkOperatorName:") + OperatorName));
					env->ReleaseStringUTFChars(str, c);
					env->DeleteLocalRef(str);
					m_TelCompany = OperatorName;
				}

				m_DeviceBrand = brandstr;
				m_DeviceModel = modelstr;
				m_DeviceOSVersion = versionreleasestr;
				m_DeviceCPU_ABI = cpuabistr;
				m_DeviceManufacturer = manufacturerstr;
				m_DeviceProduct = productstr;

				env->DeleteLocalRef(brand_strobj);
				env->DeleteLocalRef(model_strobj);
				env->DeleteLocalRef(cpuabi_strobj);
				env->DeleteLocalRef(manufacturer_strobj);
				env->DeleteLocalRef(product_strobj);
				env->DeleteLocalRef(versionrelease_strobj);

				
				JNI_DETACH_CURRENT_THREAD
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetMainContext()
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetMainContext();                
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetLoadingContext()
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetLoadingContext();                
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CKernel::vLoop()
			{
				LYNXBOOL Ret = LynxEngine::GameFramework::CKernel::vLoop();
                return Ret;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vPause()
			{
				m_bPause = LYNX_TRUE;
				m_bPauseGameTime = LYNX_TRUE;
				if (!GetlpEngine()->GetlpSoundSystem()->IsPaused())
				{
					if (GetlpEngine()->GetlpSoundSystem())
						GetlpEngine()->GetlpSoundSystem()->vPause();
				}

				SetRunning(LYNX_FALSE);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vResume()
			{
				m_bPause = LYNX_FALSE;
				m_bPauseGameTime = LYNX_FALSE;
				if (GetlpEngine()->GetlpSoundSystem()->IsPaused())
				{
					if (GetlpEngine()->GetlpSoundSystem())
						GetlpEngine()->GetlpSoundSystem()->vResume();
				}

				SetRunning(LYNX_TRUE);
			}


			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   遊戲主動結束.
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vExitApp()
			{
				JNI_ATTACH_CURRENT_THREAD

				//jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "finish", "()V");
				//env->CallVoidMethod(m_App->activity->clazz, mid);

				//jclass clssystem = env->FindClass("System");
				//jmethodID midexit = GetJavaMethodID(env, m_App->activity->clazz, "exit", "(I)V");
				//env->CallVoidMethod(clssystem, midexit, 0);


				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "MoveToBackGround", "()V");
				env->CallVoidMethod(m_App->activity->clazz, mid);


				JNI_DETACH_CURRENT_THREAD
			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
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
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetTapGestureRecognization(LYNXBOOL b)
			{				
			    if (b)
                    m_GestureState |= TAP_GESTURE;
                else
                    m_GestureState &= ~TAP_GESTURE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetSwipeGestureRecognization(LYNXBOOL b)
			{
			    if (b)
                    m_GestureState |= SWIPE_GESTURE;
                else
                    m_GestureState &= ~SWIPE_GESTURE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetPinchGestureRecognization(LYNXBOOL b)
			{
			    if (b)
                    m_GestureState |= PINCH_GESTURE;
                else
                    m_GestureState &= ~PINCH_GESTURE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetRotationGestureRecognization(LYNXBOOL b)
			{
			    if (b)
                    m_GestureState |= ROTATION_GESTURE;
                else
                    m_GestureState &= ~ROTATION_GESTURE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetLongPressGestureRecognization(LYNXBOOL b)
			{
			    if (b)
                    m_GestureState |= LONGPRESS_GESTURE;
                else
                    m_GestureState &= ~LONGPRESS_GESTURE;
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetPanGestureRecognization(LYNXBOOL b)
			{
			    if (b)
                    m_GestureState |= PAN_GESTURE;
                else
                    m_GestureState &= ~PAN_GESTURE;
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
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
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CKernel::vGetSystemLanguageCode()
			{
				/////Get Language/////
				JNI_ATTACH_CURRENT_THREAD

				jclass cls = env->FindClass("java/util/Locale");
				jmethodID getDefault = env->GetStaticMethodID(cls, "getDefault", "()Ljava/util/Locale;");				
				jmethodID toString = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
				jobject locale = env->CallStaticObjectMethod(cls, getDefault);
				jstring str = (jstring) env->CallObjectMethod(locale, toString);
				const char* c = env->GetStringUTFChars(str, 0);
				CString lang = _T(c);
				lynxDebugOutputPrintf("System Language: %s", lang.c_str());
				
				env->ReleaseStringUTFChars(str, c);
				env->DeleteLocalRef(cls);
				env->DeleteLocalRef(locale);
				env->DeleteLocalRef(str);

				JNI_DETACH_CURRENT_THREAD

				return CGame::GetLanguageCodeFromISO_639_1(lang);				
			}
			//-------------------------------------------------------------------------------------------------------
			// Utility for get class name
			//-------------------------------------------------------------------------------------------------------
			void CKernel::GetJavaClassName(JavaVM* vm, jobject jobj)
			{
				JNI_ATTACH_CURRENT_THREAD

				// find java class
				jclass clazz = env->GetObjectClass(jobj); /* app->activity->clazz */
				jmethodID mid_getClass = env->GetMethodID(clazz, "getClass", "()Ljava/lang/Class;");
				jobject clazzObj = env->CallObjectMethod(jobj, mid_getClass);
				
				// get the class object's class descriptor
				jclass clazzdesc = env->GetObjectClass(clazzObj);
				
				// Find the getName() method on the class object
				jmethodID mid_getName = env->GetMethodID(clazzdesc, "getName", "()Ljava/lang/String;");
				jstring name = (jstring)env->CallObjectMethod(clazz, mid_getName);
				
				const char *str= env->GetStringUTFChars(name, 0);
				LYNX_LOG(glpLogger, CString("getClassName() = ") + CString(str));
				env->ReleaseStringUTFChars(name, str);
				
				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			// Utility for get methodID
			//-------------------------------------------------------------------------------------------------------
			jmethodID CKernel::GetJavaMethodID(JNIEnv *env, jobject jobj, const CString& functionName, const CString& signature)
			{
				LYNX_LOG(glpLogger, CString("GetMethodID():: method = ") + functionName + CString(", signature = ") + signature);

				jclass clazz = env->GetObjectClass(jobj);
				if (NULL == clazz)
				{
					LYNX_LOG(glpLogger, CString("Could not find clazz"));
				}

				jmethodID mid = env->GetMethodID(clazz, functionName, signature);
				if (0 == mid)
				{
					LYNX_LOG(glpLogger, CString("Could not find method id"));
				}

				return mid;
			}
            //-------------------------------------------------------------------------------------------------------
			// OpenWebView
			//
			// @description 
			//  open webView and show html by use different protocol 
			// 
			// @param
			//	_URL : url string
			//  _Rect: geometry of webView
			//  _Title: titlebar text of webView, "Empty" for hide titiebar
			//  _ButtonTitel: text of close button, "Empty" for hide close button
			//  _Button_Width: button width
			//  _Button_Height: button height
			//  _Bg_Color: background color of webView
			//
			// 1. Load html from file, notice the asset folder path
			//  example: CKernel::OpenWebView("file:///android_asset/html/qubot.html", ...);
			// 2. Load html from web
			//  example: CKernel::OpenWebView("http://www.google.com.tw", ...);
			//-------------------------------------------------------------------------------------------------------
			void CKernel::OpenWebView(const CString&	url, 
									  LYNXRECT			rect, 
									  const CString&	title,
									  const CString&	button_title,
									  int				button_width,
									  int				button_height,
									  LYNXCOLORRGBA		bg_color)
			{
				if (!m_JavaVM)
					return;
				
				/**
				  DON'T use m_JNIEnv, it needs to attach a new thread and retrieve JNIEnv
				 */
				JNI_ATTACH_CURRENT_THREAD
				
				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "OpenURL", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIIIIII)V");
				
				// Call Java
				jstring jurl = NewModifiedUTF8String(env, url.c_str());
				jstring jtitle = NewModifiedUTF8String(env, title.c_str());
				jstring jbutton_title = NewModifiedUTF8String(env, button_title.c_str());

				// RGBA -> ARGB
				int cRGB = ((bg_color.Red << 16) | (bg_color.Green << 8) | (bg_color.Blue));
				int cAlpha = bg_color.Alpha;

				env->CallVoidMethod(m_App->activity->clazz, mid, 
					jurl, jtitle, jbutton_title, rect.x1, rect.y1, rect.x2, rect.y2, button_width, button_height, cRGB, cAlpha);
				
				env->DeleteLocalRef(jurl);
				env->DeleteLocalRef(jtitle);
				env->DeleteLocalRef(jbutton_title);

				JNI_DETACH_CURRENT_THREAD

			    /**
				 DON'T asscess FindClass of current activity. 
				 like this : => jclass clazz = env->FindClass("com/qubitent/qubot/NativeActivity");
				 use ClassLoader if necessary.

				 jmethodID mid_getClassLoader = env->GetMethodID(clazz, "getClassLoader", "()Ljava/lang/ClassLoader;");
				 jobject sub_clazz = env->CallObjectMethod(m_App->activity, mid_getClassLoader);
				 jclass classLoader = env->FindClass("java/lang/ClassLoader");
				 jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
				 jstring strClassName = env->NewStringUTF("com/qubitent/qubot/NativeActivity");
				 jclass clazz = (jclass)(jni_env->CallObjectMethod(cls, findClass, strClassName));
				 jni_env->DeleteLocalRef(strClassName);
				 */
			}
			//-------------------------------------------------------------------------------------------------------
			// OpenAlertDialog
			//
			// @description 
			//  open android alert dialog  
			//
			// @param
			//-------------------------------------------------------------------------------------------------------
			void CKernel::OpenAlertDialog(const CString& title, const CString& message, const CString& ok, const CString& middle, const CString& cancel, const CArray<CString>& vParamStr)
			{
				if (!m_JavaVM)
					return;
				
				const CString* arrayParamStr = &vParamStr[0];				

				JNI_ATTACH_CURRENT_THREAD
				
				// public void OpenAlertDialog(String, String, String, String, String, String[]) 
				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, 
					"OpenAlertDialog", 
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V");
				
				jstring jtitle = NewModifiedUTF8String(env, title.c_str());
				jstring jmessage = NewModifiedUTF8String(env, message.c_str());
				jstring jok = NewModifiedUTF8String(env, ok.c_str());
				jstring jmiddle = NewModifiedUTF8String(env, middle.c_str());
				jstring jcancel = NewModifiedUTF8String(env, cancel.c_str());
				jstring empty = env->NewStringUTF("");

				jobjectArray jskusArray;
				jint size = (int)vParamStr.size();
				jclass stringClazz = env->FindClass("java/lang/String");
				jskusArray = env->NewObjectArray(size, stringClazz, empty);

				jstring tempString;
				for (int i=0; i<size ;++i) 
				{
					tempString = NewModifiedUTF8String(env, arrayParamStr[i].c_str());        
					env->SetObjectArrayElement(jskusArray, i, tempString);    
					env->DeleteLocalRef(tempString);
				}

				env->CallVoidMethod(m_App->activity->clazz, mid, jtitle, jmessage, jok, jmiddle, jcancel, jskusArray);
				
				env->DeleteLocalRef(jtitle);
				env->DeleteLocalRef(jmessage);
				env->DeleteLocalRef(jok);
				env->DeleteLocalRef(jmiddle);
				env->DeleteLocalRef(jcancel);
				env->DeleteLocalRef(empty);
				env->DeleteLocalRef(jskusArray);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			// vQueryIAPList
			//
			// @description 
			//  Open Google In App Purchase and query Inventory.
			//
			//  To test iap, it should build release-signed-aligned apk.
			//
			//		- Sign : jarsigner - verbose - sigalg SHA1withRSA - digestalg SHA1 - keystore qubot.keystore QubotWar.apk qubot
			//		- Zipalign : C:\NVPACK\android-sdk-windows\build-tools\21.1.2\zipalign -v 4 QubotWar.apk QubotWar-release.apk
			//      - Install : adb install - r QubotWar - release.apk
			//						 
			// @param
			//	base64EncodedPublicKey : public key string from Google Develope Console
			//  list : string list of productId from game server
			//
			// @callback
			//  vOnFinishingQueryIAPList
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vQueryIAPList(const CString& base64EncodedPublicKey, const CArray<CString>& list)
			{				
				LYNX_LOG(glpLogger, "vQueryIAPList::QueryInventoryAsync");

				JNI_ATTACH_CURRENT_THREAD
				
				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "QueryInventoryAsync", "(Ljava/lang/String;[Ljava/lang/String;)V");
				
				
				jobjectArray jskusArray;
				jint size = list.size();
				jclass stringClazz = env->FindClass("java/lang/String");
				jstring empty = env->NewStringUTF("");
				jskusArray = env->NewObjectArray(size, stringClazz, empty);

				jstring tempString;
				for (int i=0; i<size ;++i) 
				{
					tempString = NewModifiedUTF8String(env, list[i].c_str());        
					env->SetObjectArrayElement(jskusArray, i, tempString);    
					env->DeleteLocalRef(tempString);
				}

				// Call Java
				jstring jstr = NewModifiedUTF8String(env, base64EncodedPublicKey.c_str());
				env->CallVoidMethod(m_App->activity->clazz, mid, jstr, jskusArray);
				env->DeleteLocalRef(jstr);
				env->DeleteLocalRef(empty);
				env->DeleteLocalRef(jskusArray);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			// vPurchaseIAP
			//
			// @description 
			//  after user click item, run GooglePlayStore purchase flow  
			//
			// @param
			//	productId : sku id in query Inventory
			//  requestCode : developer defined for verify
			//  payload : developer defined for verify
			// 
			// @callback
			//	vOnVerifyPurchase
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vPurchaseIAP(const CString& productId, int requestCode, const CString& playerSN)
			{				
				CString Payload;
				LynxEngine::ComputePurchasePayload(playerSN, productId, Payload);
				
				JNI_ATTACH_CURRENT_THREAD
				
				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "LaunchPurchaseFlow", "(Ljava/lang/String;ILjava/lang/String;)V");
				
				jstring jsku = NewModifiedUTF8String(env, productId.c_str());
				jstring jpayload = NewModifiedUTF8String(env, Payload.c_str());
				env->CallVoidMethod(m_App->activity->clazz, mid, jsku, (jint)requestCode, jpayload);
				env->DeleteLocalRef(jsku);
				env->DeleteLocalRef(jpayload);

				JNI_DETACH_CURRENT_THREAD
			}
			
			//-------------------------------------------------------------------------------------------------------
			// vConsumePurchase
			//
			// @description 
			//  helper caller for android platform to consume purchase
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vConsumePurchase()
			{
				JNI_ATTACH_CURRENT_THREAD

				jmethodID mid = GetJavaMethodID(env, m_App->activity->clazz, "ConsumePurchase", "()V");
				env->CallVoidMethod(m_App->activity->clazz, mid);
								
				JNI_DETACH_CURRENT_THREAD
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenWebView(const CString& url, const CString& title, LYNXRECT& rect)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID openWebView = env->GetMethodID(cls, "OpenWebView", "(Ljava/lang/String;IIII)V");
				jstring u = NewModifiedUTF8String(env, url.c_str());
				env->CallVoidMethod(app->activity->clazz, openWebView, u, int(rect.x1/m_InputScaleFactor), int(rect.y1/m_InputScaleFactor), int(rect.x2/m_InputScaleFactor), int(rect.y2/m_InputScaleFactor));

				env->DeleteLocalRef(u);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenCustomWebview(const CString name, LYNXRECT& rect, const CString& title, const CString& htmlOrUrl)
			{
			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenTextInput(const CString& name, GUISystem::CText* text)
			{
				vOpenTextInput(name, text->GetText(), text);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenTextInput(const CString& name, const CString& displayText, GUISystem::CText* text)
			{
				text->Activate(LYNX_FALSE);

				JNI_ATTACH_CURRENT_THREAD

					android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID openEditText = env->GetMethodID(cls, "OpenEditText", "(Ljava/lang/String;Ljava/lang/String;J)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				jstring str = NewModifiedUTF8String(env, displayText.c_str());
				jlong ptr = (jlong)text;
				env->CallVoidMethod(app->activity->clazz, openEditText, n, str, ptr);

				env->DeleteLocalRef(n);
				env->DeleteLocalRef(str);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vAddAnimationLayer(const CString& name, const CString& filename, const CString& ext, int num, int dur, LYNXRECT& rect)
			{
				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vCloseAnimationLayer()
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID closePopupWindow = env->GetMethodID(cls, "ClosePopupWindow", "()V");
				env->CallVoidMethod(app->activity->clazz, closePopupWindow);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vScheduleLocalNotification(int id, const CString& title, const CString& context, const CString& ringtone, int invokeTimeInSeconds)
			{
				JNI_ATTACH_CURRENT_THREAD

				jint jid = (jint)id;
				jstring jtitle = NewModifiedUTF8String(env, title.c_str());
				jstring jcontext = NewModifiedUTF8String(env, context.c_str());
				jstring jringtone = NewModifiedUTF8String(env, ringtone.c_str());
				jint jinvokeTime = (jint)invokeTimeInSeconds;
				
				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID scheduleLocalNotification = env->GetMethodID(cls, "ScheduleLocalNotification", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
				env->CallVoidMethod(app->activity->clazz, scheduleLocalNotification, jid, jtitle, jcontext, jringtone, jinvokeTime);

				env->DeleteLocalRef(jtitle);
				env->DeleteLocalRef(jcontext);
				env->DeleteLocalRef(jringtone);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vCancelLocalNotification(int id)
			{
				JNI_ATTACH_CURRENT_THREAD

				jint jid = (jint)id;				

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID cancelLocalNotification = env->GetMethodID(cls, "CancelLocalNotification", "(I)V");
				env->CallVoidMethod(app->activity->clazz, cancelLocalNotification, jid);
				
				JNI_DETACH_CURRENT_THREAD
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vFacebookLoginWithPermission(const CString& permissions)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID queryFacebookAccountData = env->GetMethodID(cls, "FacebookLoginWithPermission", "(Ljava/lang/String;)V");
				jstring p = NewModifiedUTF8String(env, permissions.c_str());
				env->CallVoidMethod(app->activity->clazz, queryFacebookAccountData, p);

				env->DeleteLocalRef(p);

				JNI_DETACH_CURRENT_THREAD
			}


			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vFacebookLogout()
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID facebooklogout = env->GetMethodID(cls, "FacebookLogout", "()V");
				env->CallVoidMethod(app->activity->clazz, facebooklogout);
				
				JNI_DETACH_CURRENT_THREAD
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vQueryFacebookAccountData(const CString& permissions, const CString& fields)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID queryFacebookAccountData = env->GetMethodID(cls, "QueryFacebookAccountData", "(Ljava/lang/String;Ljava/lang/String;)V");
				jstring p = NewModifiedUTF8String(env, permissions.c_str());
				jstring f = NewModifiedUTF8String(env, fields.c_str());
				env->CallVoidMethod(app->activity->clazz, queryFacebookAccountData, p, f);

				env->DeleteLocalRef(p);
				env->DeleteLocalRef(f);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOnFinishQueryFacebookAccountData(const CString& JSONResult)
			{
				lynxDebugOutputPrintf("JSON result: %s", JSONResult.c_str());
				//SetUITransitionMode(UTM_FADEOUT);
				//SetFadeOutSpeed(0.1f);
				//GetCurrentUIPage()->SetQuit(LYNX_TRUE);
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOnFinishQueryFacebookFriendsData(const CString& JSONResult)
			{
			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vFacebookShareLink(const CString& title, const CString& Desc, const CString& contenturl, const CString& imageurl)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID queryFacebookAccountData = env->GetMethodID(cls, "tryFacebookShareLink", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
				jstring ti = NewModifiedUTF8String(env, title.c_str());
				jstring de = NewModifiedUTF8String(env, Desc.c_str());
				jstring cu = NewModifiedUTF8String(env, contenturl.c_str());
				jstring iu = NewModifiedUTF8String(env, imageurl.c_str());

				env->CallVoidMethod(app->activity->clazz, queryFacebookAccountData, ti, de, cu, iu);

				env->DeleteLocalRef(ti);
				env->DeleteLocalRef(de);
				env->DeleteLocalRef(cu);
				env->DeleteLocalRef(iu);

				JNI_DETACH_CURRENT_THREAD

			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOnFinishFacebookShareLink(const CString& JSONResult)
			{

			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vFacebookUploadPic(const CString& filepath)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID queryFacebookAccountData = env->GetMethodID(cls, "TryFacebookUploadPic", "(Ljava/lang/String;)V");
				jstring ft = NewModifiedUTF8String(env, filepath.c_str());

				env->CallVoidMethod(app->activity->clazz, queryFacebookAccountData, ft);

				env->DeleteLocalRef(ft);

				JNI_DETACH_CURRENT_THREAD

			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString	CKernel::vFacebookGetAccessToken()
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID mtehod = env->GetMethodID(cls, "GetFacebookAccessToken", "()Ljava/lang/String;");

				jstring str = (jstring)env->CallObjectMethod(app->activity->clazz, mtehod);
				LYNX_LOG(glpLogger, CString(_T("joybomb vFacebookGetAccessToken")));
				const char* c = env->GetStringUTFChars(str, 0);
				CString Token = _T(c);

				env->ReleaseStringUTFChars(str, c);
				env->DeleteLocalRef(str);

				JNI_DETACH_CURRENT_THREAD

				return Token;
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOnFinishFacebookUploadPic(const CString& JSONResult)
			{
			
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明: 挑選應用程式做分享.
			//	msg : 替玩家寫入的訊息.
			//	imagePath : 例: texture/ui/infobox24.png.
			//	isInternalPath : 內部或外部路徑.
			//			true = data/data/my.package.name/files/
			//			false = Andorid/data/my.package.name/files/
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vShareWithSelectApp(const CString& msg, const CString& imagePath, LYNXBOOL isInternalPath)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID shareWithIntent = env->GetMethodID(cls, "ShareWithIntent", "(Ljava/lang/String;Ljava/lang/String;Z)V");
				jstring jmsg = NewModifiedUTF8String(env, msg.c_str());
				jstring jimgpath = NewModifiedUTF8String(env, imagePath.c_str());
				env->CallVoidMethod(app->activity->clazz, shareWithIntent, jmsg, jimgpath, isInternalPath);

				env->DeleteLocalRef(jmsg);
				env->DeleteLocalRef(jimgpath);

				JNI_DETACH_CURRENT_THREAD
			
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString	CKernel::vFirebaseGetAccessToken()
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID mtehod = env->GetMethodID(cls, "GetFirebaseAccessToken", "()Ljava/lang/String;");

				jstring str = (jstring)env->CallObjectMethod(app->activity->clazz, mtehod);
				LYNX_LOG(glpLogger, CString(_T("joybomb GetFirebaseAccessToken")));
				const char* c = env->GetStringUTFChars(str, 0);
				CString Token = _T(c);

				env->ReleaseStringUTFChars(str, c);
				env->DeleteLocalRef(str);

				JNI_DETACH_CURRENT_THREAD

					return Token;
			}

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vQueryFacebookFriendsData(const CString& permissions, const CString& fields)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID queryFacebookFriendsData = env->GetMethodID(cls, "QueryFacebookFriendsData", "(Ljava/lang/String;Ljava/lang/String;)V");
				jstring p = NewModifiedUTF8String(env, permissions.c_str());
				jstring f = NewModifiedUTF8String(env, fields.c_str());
				env->CallVoidMethod(app->activity->clazz, queryFacebookFriendsData, p, f);

				env->DeleteLocalRef(p);
				env->DeleteLocalRef(f);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_AddColorBackground(const CString& name, LYNXCOLORRGBA color)
			{
				vOSN_ReleaseView(name);

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID addColorBackground = env->GetMethodID(cls, "AddColorBackground", "(Ljava/lang/String;I)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				jint c = color.PackedColor;
				env->CallVoidMethod(app->activity->clazz, addColorBackground, n, c);

				env->DeleteLocalRef(n);

				m_OSNViewList.push_back(name);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_AddAnimationView(const CString& name, const CString& animfile, const CString& ext, int num, int dur, LYNXRECT& rect)
			{
				vOSN_ReleaseView(name);

				CString filename(animfile);
				if (filename.c_str()[0] == _T('.') && filename.c_str()[1] == _T('.') && filename.c_str()[2] == _T('/'))
					filename = &(filename.c_str()[3]);
				else if (filename.c_str()[0] == _T('.') && filename.c_str()[1] == _T('/'))
					filename = &(filename.c_str()[2]);

				JNI_ATTACH_CURRENT_THREAD

				LYNXRECT Rect = rect;
				Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
				Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID addAnimationView = env->GetMethodID(cls, "AddAnimationView", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIIII)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				jstring f = NewModifiedUTF8String(env, filename.c_str());
				jstring e = NewModifiedUTF8String(env, ext.c_str());
				env->CallVoidMethod(app->activity->clazz, addAnimationView, n, f, e, num, dur, int(Rect.x1/m_InputScaleFactor), int(Rect.y1/m_InputScaleFactor), int(Rect.x2/m_InputScaleFactor), int(Rect.y2/m_InputScaleFactor));

				env->DeleteLocalRef(n);
				env->DeleteLocalRef(f);
				env->DeleteLocalRef(e);

				m_OSNViewList.push_back(name);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_AddTextView(const CString& name, const CString& filename, LYNXRECT& rect)
			{
				vOSN_ReleaseView(name);			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_AddWebview(const CString& name, LYNXRECT& rect, const CString& htmlOrUrl, const LYNXBOOL bNotFullScreen)
			{
				vOSN_ReleaseView(name);

				JNI_ATTACH_CURRENT_THREAD

				LYNXRECT Rect = rect;
				Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
				Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID addWebView = env->GetMethodID(cls, "AddWebView", "(Ljava/lang/String;Ljava/lang/String;IIIIZ)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				jstring h = NewModifiedUTF8String(env, htmlOrUrl.c_str());
				env->CallVoidMethod(app->activity->clazz, addWebView, n, h, int(Rect.x1 / m_InputScaleFactor), int(Rect.y1 / m_InputScaleFactor), int(Rect.x2 / m_InputScaleFactor), int(Rect.y2 / m_InputScaleFactor), bNotFullScreen);

				env->DeleteLocalRef(n);
				env->DeleteLocalRef(h);

				m_OSNViewList.push_back(name);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_RemoveView(const CString& name)
			{
				CList<CString>::iterator found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found == m_OSNViewList.end())
				{
					return;
				}

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID removeView = env->GetMethodID(cls, "RemoveView", "(Ljava/lang/String;)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				env->CallVoidMethod(app->activity->clazz, removeView, n);

				env->DeleteLocalRef(n);

				found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found != m_OSNViewList.end())
				{
					m_OSNViewList.erase(found);
				}

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
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

				CList<CString>::iterator found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found == m_OSNViewList.end())
				{
					return;
				}

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID showView = env->GetMethodID(cls, "ShowView", "(Ljava/lang/String;Z)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				env->CallVoidMethod(app->activity->clazz, showView, n, bshow);

				env->DeleteLocalRef(n);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_HideAllViews()
			{
				m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);	

				for (CList<CString>::CIterator View = m_OSNViewList.begin(); View != m_OSNViewList.end(); View++)
					vOSN_ShowView((*View), LYNX_FALSE);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_MoveView(const CString& name, LYNXRECT& rect, const LYNXBOOL bNotFullScreen)
			{
				CList<CString>::iterator found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found == m_OSNViewList.end())
				{
					return;
				}

				JNI_ATTACH_CURRENT_THREAD
				
				LYNXRECT Rect = rect;
				Rect.x1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.x2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
				Rect.y1 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
				Rect.y2 += m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID moveView = env->GetMethodID(cls, "MoveView", "(Ljava/lang/String;IIIIZ)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				env->CallVoidMethod(app->activity->clazz, moveView, n, int(Rect.x1 / m_InputScaleFactor), int(Rect.y1 / m_InputScaleFactor), int(Rect.x2 / m_InputScaleFactor), int(Rect.y2 / m_InputScaleFactor), bNotFullScreen);

				env->DeleteLocalRef(n);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_ReleaseView(const CString& name)
			{
				if (name == CString("LoadingAnimation") || name == CString("ConnectingAnimation") || name == CString("LoadingAnimationRun"))
				{
					m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
				}
				CList<CString>::iterator found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found == m_OSNViewList.end())
				{
					return;
				}

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID releaseView = env->GetMethodID(cls, "ReleaseView", "(Ljava/lang/String;)V");
				jstring n = NewModifiedUTF8String(env, name.c_str());
				env->CallVoidMethod(app->activity->clazz, releaseView, n);

				env->DeleteLocalRef(n);

				found = LynxEngine::find(m_OSNViewList.begin(), m_OSNViewList.end(), name);
				if (found != m_OSNViewList.end())
				{
					m_OSNViewList.erase(found);
				}

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOSN_OpenURL(const CString& url)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID openURL = env->GetMethodID(cls, "OpenURL", "(Ljava/lang/String;)V");
				jstring u = NewModifiedUTF8String(env, url.c_str());
				env->CallVoidMethod(app->activity->clazz, openURL, u);

				env->DeleteLocalRef(u);

				JNI_DETACH_CURRENT_THREAD			
			}	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::SetupCPUClass()
			{
				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CKernel::SetupGPUClass()
			{
				m_GPUClass = m_GPUClassTable[m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetRenderer()];
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CKernel::vGetGameVersion()
            {
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID getGameVersion = env->GetMethodID(cls, "GetGameVersion", "()Ljava/lang/String;");
				jstring str = (jstring) env->CallObjectMethod(app->activity->clazz, getGameVersion);

				const char* c = env->GetStringUTFChars(str, 0);
				CString version = _T(c);

				env->ReleaseStringUTFChars(str, c);
				env->DeleteLocalRef(str);

				JNI_DETACH_CURRENT_THREAD

				return version;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vSetDeviceSleepMode(LYNXBOOL b)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);				
				jmethodID setDeviceSleepMode = env->GetMethodID(cls, "SetDeviceSleepMode", "(Z)V");				
				env->CallVoidMethod(app->activity->clazz, setDeviceSleepMode, b);

				JNI_DETACH_CURRENT_THREAD
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CKernel::vOpenRatingDialog(const CString& title, const CString& text, const CString& positive_text, const CString& negative_text, CString neutral_text)
			{
				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID openRatingDialog = env->GetMethodID(cls, "OpenRatingDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
								
				jstring jtitle = NewModifiedUTF8String(env, title.c_str());
				jstring jtext = NewModifiedUTF8String(env, text.c_str());
				jstring jpositive_text = NewModifiedUTF8String(env, positive_text.c_str());
				jstring jnegative_text = NewModifiedUTF8String(env, negative_text.c_str());
				jstring jneutral_text = NewModifiedUTF8String(env, neutral_text.c_str());
				env->CallVoidMethod(app->activity->clazz, openRatingDialog, jtitle, jtext, jpositive_text, jnegative_text, jneutral_text);
				
				env->DeleteLocalRef(jtitle);
				env->DeleteLocalRef(jtext);
				env->DeleteLocalRef(jpositive_text);
				env->DeleteLocalRef(jnegative_text);
				env->DeleteLocalRef(jneutral_text);

				JNI_DETACH_CURRENT_THREAD
			}
		}
	}	
}