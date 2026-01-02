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
#include <Platform/JNI/LynxJNI.h>

using namespace LynxEngine;

#if (defined(__WEB__) || defined(__ANDROID__))	
	#ifdef __WIN32__		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::GameFramework::CKernel* lynxCreateKernel()
		{
			return LYNXNEW LynxEngine::GameFramework::CKernel();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk)
		{
			LYNXDEL lpk;
			lpk = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		#include <win32/jawt_md.h>		
		static JAWT									gAWT;			
		static JAWT_DrawingSurface*					gDrawingSurface			 = NULL;
		static JAWT_DrawingSurfaceInfo*				gDrawingSurfaceInfo		 = NULL;
		static JAWT_Win32DrawingSurfaceInfo*		gWin32DrawingSurfaceInfo = NULL;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL
		Java_LynxEngine_Kernel_NativeInit(JNIEnv* env, jobject kernel, jobject canvas, jint w, jint h, jint rendertype)
		{			
			jboolean Result;
			jint Lock;

			// Get the AWT
			gAWT.version = JAWT_VERSION_1_4;
			Result = JAWT_GetAWT(env, &gAWT);			
			if (Result == JNI_FALSE)
				return;
			
			//LYNX_LOG(glpLogger, _T("LynxJNI::JAWT_GetAWT() OK\n"));							
			
			// Get the drawing surface
			gDrawingSurface = gAWT.GetDrawingSurface(env, canvas);
			if (gDrawingSurface == NULL)
				return;

			//LYNX_LOG(glpLogger, _T("LynxJNI::GetDrawingSurface() OK\n"));							

			// Lock the drawing surface
			Lock = gDrawingSurface->Lock(gDrawingSurface);
			LYNX_ASSERT((Lock & JAWT_LOCK_ERROR) == 0);				

			// Get the drawing surface info
			gDrawingSurfaceInfo = gDrawingSurface->GetDrawingSurfaceInfo(gDrawingSurface);

			// Get the platform-specific drawing info
			gWin32DrawingSurfaceInfo = (JAWT_Win32DrawingSurfaceInfo*)gDrawingSurfaceInfo->platformInfo;							
			
			glpKernel->SetWindow(gWin32DrawingSurfaceInfo->hwnd);									

			LynxEngine::GameFramework::CKernel::INIT_DESC InitDesc;
			InitDesc.GraphicParam.Width = w;
			InitDesc.GraphicParam.Height = h; 
			InitDesc.GraphicParam.FrameBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_X8R8G8B8;
			InitDesc.GraphicParam.DepthBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_D24S8;
			InitDesc.GraphicParam.bHardware = LYNX_TRUE;
			InitDesc.GraphicParam.bFullScreen = LYNX_FALSE;
			InitDesc.GraphicParam.NumMultiSamples = 1;			
			InitDesc.GraphicParam.RenderDeviceType = LynxEngine::GraphicsSystem::CRenderer::RDT_DEFAULT;	
			InitDesc.GraphicParam.ShaderDebuggingFlag = LynxEngine::GraphicsSystem::CRenderer::SHADER_DEBUG_NONE;
			InitDesc.GraphicParam.RendererType = (LynxEngine::GraphicsSystem::CRenderer::RENDERERTYPE)rendertype;
			
			glpKernel->vInit(0, NULL, &InitDesc);
			//glpKernel->vInit();
			
			LynxEngine::GameFramework::CUIPage* lpUIPage = glpKernel->GetlpEngine()->CreateUIPage(L"MainMenuPage", L"../script/CMainMenuPage.lua", L"CMainMenuPage");						
			glpKernel->SetCurrentUIPage(lpUIPage);			

			gDrawingSurface->FreeDrawingSurfaceInfo(gDrawingSurfaceInfo);
			// Unlock the drawing surface
			gDrawingSurface->Unlock(gDrawingSurface);										
		}		
	#elif defined(__xxANDROID__)
		#include <Platform/Android/LynxAndroidGameFrameworkKernel.h>
		#include <android/asset_manager_jni.h>	
		#include <android/native_window_jni.h>	
		AAssetManager*							glpAssetManager = NULL;
		static jobject							gAssetManagerObj = NULL; 
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::GameFramework::CKernel* lynxCreateKernel()
		{
			return (LYNXNEW LynxEngine::GameFramework::Android::CKernel());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk)
		{
			LYNXDEL lpk;
			lpk = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL
		Java_LynxEngine_Android_LynxActivity_NativeInit(JNIEnv* env, jobject activity, jobject assetmanager)
		{
			lynxInit();
			LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;		
			
			gAssetManagerObj = env->NewGlobalRef(assetmanager);
			glpAssetManager = AAssetManager_fromJava(env, gAssetManagerObj);									
			lpKernel->SetAssetManager(glpAssetManager);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL
		Java_LynxEngine_Kernel_NativeInit(JNIEnv* env, jobject kernel, jobject view, jint w, jint h, jint rendertype)
		{
			LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;		

			lpKernel->SetlpView(view);			
			lpKernel->SetlpKernel(kernel);				
			lpKernel->SetWindow(ANativeWindow_fromSurface(env, view));				
			
			/*
			jclass ViewClass = env->GetObjectClass(view);			
			jfieldID FiledID = env->GetFieldID(ViewClass, "Kernel", "LLynxEngine/Kernel;");
			jobject KernelObj = env->GetObjectField(view, FiledID);
			lpKernel->SetlpKernel(KernelObj);
			*/
			/*
			if (KernelObj)
			{
				jclass KernelClass = env->GetObjectClass(KernelObj);
				if (KernelClass)
					LYNX_LOG(glpLogger, LynxEngine::CString(_T("Found LynxEngine.Kernel class \r\n")));
			}
			*/

			//jmethodID MethodID = env->GetMethodID(ViewClass, "Callback","()LLynxEngine/Kernel;");
			//lpKernel->SetlpKernel(env->CallObjectMethod(view, ));		

			//jclass KernelClass = env->FindClass("LynxEngine/Kernel");
			//if (KernelClass)
			//	LYNX_LOG(glpLogger, LynxEngine::CString(_T("Found LynxEngine.Kernel class \r\n")));

			LynxEngine::GameFramework::CKernel::INIT_DESC InitDesc;
			InitDesc.GraphicParam.Width = w;
			InitDesc.GraphicParam.Height = h; 
			InitDesc.GraphicParam.FrameBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_X8R8G8B8;
			InitDesc.GraphicParam.DepthBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_D24S8;
			InitDesc.GraphicParam.bHardware = LYNX_TRUE;
			InitDesc.GraphicParam.bFullScreen = LYNX_FALSE;
			InitDesc.GraphicParam.NumMultiSamples = 1;			
			InitDesc.GraphicParam.RenderDeviceType = LynxEngine::GraphicsSystem::CRenderer::RDT_DEFAULT;	
			InitDesc.GraphicParam.ShaderDebuggingFlag = LynxEngine::GraphicsSystem::CRenderer::SHADER_DEBUG_NONE;
			InitDesc.GraphicParam.RendererType = (LynxEngine::GraphicsSystem::CRenderer::RENDERERTYPE)rendertype;
			
			lpKernel->vInit(&InitDesc);

			LynxEngine::GameFramework::CUIPage* lpUIPage = lpKernel->GetlpEngine()->CreateUIPage(_T("MainMenuPage"), _T("../script/CMainMenuPage.lua"), _T("CMainMenuPage"));						
			lpKernel->SetCurrentUIPage(lpUIPage);				
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL 
		Java_LynxEngine_Android_Renderer_Clear(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b)
		{
			LynxEngine::Math::CColor Color(62.0f/255.0f, 60.0f/255.0f, 58.0f/255.0f, 255.0f/255.0f);
			glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->Clear(LynxEngine::GraphicsSystem::CRenderer::RENDER_BUFFER|LynxEngine::GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);
		}
	#endif
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	Java_LynxEngine_LynxApplet_NativeInit(JNIEnv* env, jobject applet)
	{
		lynxInit();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_Loop(JNIEnv* env, jobject kernel)
	{			
		#ifdef __WIN32__
			gDrawingSurface->Lock(gDrawingSurface);
				glpKernel->vLoop();
				if (glpKernel->GetlpEngine()->GetlpSystemMouse())
					glpKernel->GetlpEngine()->GetlpSystemMouse()->ClearPollData();		
			gDrawingSurface->Unlock(gDrawingSurface);	
		#elif defined __ANDROID__
			//glpKernel->vLoop();
			static bool bCreated = false;
			if (!bCreated)
			{
				glpKernel->GetCurrentUIPage()->vCreate();
				bCreated = true;
			}
			if (glpKernel->GetCurrentUIPage())
				glpKernel->GetCurrentUIPage()->vRender();
		#endif


		/*
		LYNXRECT Rect;
		
		glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->Start();							
	
			LynxEngine::Math::CColor Color(62.0f/255.0f, 60.0f/255.0f, 58.0f/255.0f, 255.0f/255.0f);
			glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->Clear(LynxEngine::GraphicsSystem::CRenderer::RENDER_BUFFER|LynxEngine::GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);


			LYNXCOLORRGBA FontColor;			

			FontColor.Red = 204;
			FontColor.Green = 152;
			FontColor.Blue = 49;
			FontColor.Alpha = 190;

			glpKernel->GetlpEngine()->GetSystemFontString()->SetFont(glpKernel->GetlpEngine()->GetSystemSmallFont());
			glpKernel->GetlpEngine()->GetSystemFontString()->SetAlignmentType(LynxEngine::CSpriteContainer::H_ALIGN_CENTER|LynxEngine::CSpriteContainer::V_ALIGN_CENTER);
			glpKernel->GetlpEngine()->GetSystemFontString()->SetOriginType(LynxEngine::CSpriteContainer::OT_CENTER);
			Rect.x1 = 300; Rect.x2 = 480; Rect.y1 = 0; Rect.y2 = 20;
			glpKernel->GetlpEngine()->GetSystemFontString()->SetAlignmentRect(&Rect);
			//glpKernel->GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("FPS:%03d, Step:%01.1f"), LYNX_ROUND(1000.0f/glpKernel->GetTotalFrameElapsedTime()), glpKernel->m_Step);
			//glpKernel->GetlpEngine()->GetSystemFontString()->Printf(&FontColor, _T("LynxEngine Web Player"));
			
			Rect.x1 = 0;	Rect.y1 = 0;
			Rect.x2 = 1280;	Rect.y2 = 720;
			static int Frame = 0;
			glpKernel->GetlpEngine()->GetlpGraphicsSystem()->Printf(&Rect, 0, 0, &FontColor, _T("%d, LynxEngine %d.%d.%d Web Player"), Frame++, glpKernel->GetlpEngine()->GetMajorVersion(), glpKernel->GetlpEngine()->GetMinorVersion(), glpKernel->GetlpEngine()->GetBuildVersion());
			glpKernel->GetlpEngine()->GetlpGraphicsSystem()->Printf(&Rect, 0, 20, &FontColor, _T("%d, %d"), gX, gY);

		glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->End();
		glpKernel->GetlpEngine()->SwapBackBuffer();			
		*/
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	LynxEngine_Kernel_NativeRelease(JNIEnv* env, jobject canvas)
	{			
		#ifdef __WIN32__
			// Free the drawing surface
			gAWT.FreeDrawingSurface(gDrawingSurface);			
		#elif defined __xxANDROID__
			env->DeleteGlobalRef(gAssetManagerObj);			
		#endif
		glpKernel->vRelease();
		lynxRelease();			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_SetMousePollData(JNIEnv* env, jobject kernel, jint x, jint y, jint mask)
	{
		//glpKernel->GetlpEngine()->GetlpSystemMouse()->Poll();
		if (glpKernel->GetlpEngine()->GetlpSystemMouse())
		{
			LYNXPOINT2D Pos;
			Pos.x = x;
			Pos.y = y;
			glpKernel->GetlpEngine()->GetlpSystemMouse()->SetPollData(Pos, mask);			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_SetKeyboardPollData(JNIEnv* env, jobject kernel, jint key, jboolean bpressed)
	{
		if (glpKernel->GetlpEngine()->GetlpSystemKeyboard())
		{
			glpKernel->GetlpEngine()->GetlpSystemKeyboard()->SetKey(key, bpressed);									
			glpKernel->m_KBD[key] = bpressed;			
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_SetCharPressed(JNIEnv* env, jobject kernel, jint c)
	{		
		glpKernel->m_CharPressed = c;			
	}
#endif