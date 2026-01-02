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

#include <Platform/Web/LynxWebHelper.h>

#ifdef __WEB__
	#ifdef __WIN32__
		#include <win32/jawt_md.h>

		LynxEngine::GameFramework::CKernel*	glpKernel = NULL;
		extern LynxEngine::CLogger*			glpLogger;		

		static JAWT								gAWT;			
		static JAWT_DrawingSurface*				gDrawingSurface			 = NULL;
		static JAWT_DrawingSurfaceInfo*			gDrawingSurfaceInfo		 = NULL;
		static JAWT_Win32DrawingSurfaceInfo*	gWin32DrawingSurfaceInfo = NULL;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL
		Java_LynxEngine_Canvas_paint(JNIEnv* env, jobject canvas, jobject graphics)
		{			
			jboolean Result;
			jint Lock;

			if (!glpKernel)
			{
				glpLogger = ::new LynxEngine::CLogger();
				LynxEngine::CFileOutputDevice* lpOutputDevice = NULL;
				lpOutputDevice = ::new LynxEngine::CFileOutputDevice();
				glpLogger->vCreate(lpOutputDevice, LynxEngine::CString("LynxEngine.log"));			
				glpLogger->vClearLog();
				LYNX_LOG(glpLogger, _T("    __                    ____            __\n"));	
				LYNX_LOG(glpLogger, _T("   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____\n"));	
				LYNX_LOG(glpLogger, _T("  / /__/ // /  _ \\ \\/ / / __/  _ \\/ __ \\/ /  _ \\/  _/\n"));	
				LYNX_LOG(glpLogger, _T("  \\___/__, /__//_/_/\\_\\ \\__/__//_/\\__, /_/__//_/\\__/\n"));	
				LYNX_LOG(glpLogger, _T("      /___/                       /___/ \n"));	
				LYNX_LOG(glpLogger, _T(" \n"));	
				LYNX_LOG(glpLogger, _T("************************LynxEngine Log******************************\n\n"));									
			
				// Get the AWT
				gAWT.version = JAWT_VERSION_1_4;
				Result = JAWT_GetAWT(env, &gAWT);			
				if (Result == JNI_FALSE)
					return;

				LYNX_LOG(glpLogger, _T("Java::JAWT_GetAWT() OK\n"));							
				
				// Get the drawing surface
				gDrawingSurface = gAWT.GetDrawingSurface(env, canvas);
				if (gDrawingSurface == NULL)
					return;

				LYNX_LOG(glpLogger, _T("Java::GetDrawingSurface() OK\n"));							

				// Lock the drawing surface
				Lock = gDrawingSurface->Lock(gDrawingSurface);
				LYNX_ASSERT((Lock & JAWT_LOCK_ERROR) == 0);

				// Get the drawing surface info
				gDrawingSurfaceInfo = gDrawingSurface->GetDrawingSurfaceInfo(gDrawingSurface);

				// Get the platform-specific drawing info
				gWin32DrawingSurfaceInfo = (JAWT_Win32DrawingSurfaceInfo*)gDrawingSurfaceInfo->platformInfo;					
			
				glpKernel = ::new LynxEngine::GameFramework::CKernel();			

				/*
				LynxEngine::Ga:GameFramework::CKernel::INIT_DESC InitDesc;
				InitDesc.GraphicParam.Width = 1280;
				InitDesc.GraphicParam.Height = 720; 
				InitDesc.GraphicParam.FrameBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_X8R8G8B8;
				InitDesc.GraphicParam.DepthBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_D24S8;
				InitDesc.GraphicParam.bHardware = TRUE;
				InitDesc.GraphicParam.bFullScreen = FALSE;
				InitDesc.GraphicParam.NumMultiSamples = 0;			
				InitDesc.GraphicParam.RenderDeviceType = LynxEngine::GraphicsSystem::CRenderer::RDT_HARDWARE;
				*/

				glpKernel->SetWindow(gWin32DrawingSurfaceInfo->hwnd);						

				LYNXCHAR Temp[256];
				lynxSprintf(Temp, _T("HWD : %d\n\n"), gWin32DrawingSurfaceInfo->hwnd);							
				LYNX_LOG(glpLogger, Temp);											

				glpKernel->vInit();
				
				LynxEngine::GameFramework::CUIPage* lpUIPage = glpKernel->GetlpEngine()->CreateUIPage(L"MainMenuPage", L"../script/CMainMenuPage.lua", L"CMainMenuPage");						
				glpKernel->SetCurrentUIPage(lpUIPage);			
			}

			glpKernel->vLoop();

			glpKernel->GetlpEngine()->GetlpSystemMouse()->ClearPollData();			


			//gDrawingSurface->FreeDrawingSurfaceInfo(gDrawingSurfaceInfo);
			// Unlock the drawing surface
			//gDrawingSurface->Unlock(gDrawingSurface);	
			// Free the drawing surface
			//gAWT.FreeDrawingSurface(gDrawingSurface);

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
		LynxEngine_Canvas_Release(JNIEnv* env, jobject canvas)
		{
			// Free the drawing surface info
			/*
			gDrawingSurface->FreeDrawingSurfaceInfo(gDrawingSurfaceInfo);

			// Unlock the drawing surface
			gDrawingSurface->Unlock(gDrawingSurface);	

			// Free the drawing surface
			gAWT.FreeDrawingSurface(gDrawingSurface);
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		JNIEXPORT void JNICALL
		Java_LynxEngine_Kernel_SetMousePollData(JNIEnv* env, jobject kernel, jint x, jint y, jint mask)
		{
			//glpKernel->GetlpEngine()->GetlpSystemMouse()->Poll();
			LYNXPOINT2D Pos;
			Pos.x = x;
			Pos.y = y;
			glpKernel->GetlpEngine()->GetlpSystemMouse()->SetPollData(Pos, mask);									
		}
	#endif
#endif