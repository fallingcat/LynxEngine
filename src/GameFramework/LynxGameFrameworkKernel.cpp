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
//  :
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <GameFramework/LynxUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <InputSystem/LynxInputSystem.h>
#include <ControlPanel/LynxControlPanel.h>

#define BORDER_BUFFER							2

//-------------------------------------------------------------------------------------------------------
//
// 
//-------------------------------------------------------------------------------------------------------
namespace LynxEngine 
{
	namespace GameFramework 
	{	
		//CMemAllocator* CKernel::m_lpMemAllocator = NULL;
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::InitMembers(void)
		{
            m_DeviceType = DT_UNKNOWN;
            m_DeviceOrientation = DO_UNKNOWN;
            
			m_lpControlPanel = NULL;
			m_lpEngine = NULL;
			m_lpGame = NULL;
			m_bRunning = LYNX_FALSE;
			m_bPauseRendering = LYNX_FALSE;
			m_bWaitMessage = LYNX_FALSE;
			m_bMultiTask = LYNX_TRUE;
			m_bActive = LYNX_FALSE;
			for (int i = 0; i < 256; i++)
			{
				m_KBD[i] = LYNX_FALSE;
				m_KBDUP[i] = LYNX_FALSE;
			}

			m_WheelDist = 0.0f;

			m_UIPageFactory.SetParam(this);
			m_lpUIPageList.clear();
			m_lpLastUIPage = NULL;
			m_lpCurrentUIPage = NULL;			
			m_LastModalWindow = m_ModalWindow = NULL;

			m_OffscreenPos.x = m_OffscreenPos.y = 0.0f;
			m_OffscreenVelocity = 60.0f;
			m_OffscreenSwipeDir.x = 1.0f;
			m_OffscreenSwipeDir.y = 0.0f;

			m_UITransitionMode = UTM_NONE;
			m_bUITransiting = LYNX_FALSE;
			m_OffscreenSprite = NULL;
			m_ScreenFilterSprite = NULL;
			m_Border[0] = NULL;
			m_Border[1] = NULL;
			m_FadeOutSpeed = 0.01f;
            m_RollingSpeed = 3.0f;

			m_FrameCount = 0;
			m_FrameElapsedTime = 33.0f;
			m_TotalFrameElapsedTime = 0.0f;
			m_Step = 1.0f;			
			m_LastFrameTime = lynxOSGetPerformanceCounter();
			m_bPauseGameTime = LYNX_FALSE;
			m_PauseTime = 0;
			m_bPause = LYNX_FALSE;
			m_InputScaleFactor = 1.0f;
			m_bRenderBorder = LYNX_TRUE;
			m_bMemoryHackingDetected = LYNX_FALSE;
            
            m_CurrentInputFocusedContext = NULL;
        
			m_InitFreeMemSize = lynxGetAvailablePhysicalMem();

			m_DeviceOrientationMask = DOM_LANDSCAPE_RIGHT;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CKernel::CKernel(void)
		{
			InitMembers();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CEngine* CKernel::vCreateEngine() 
		{
			CEngine* lpEngine = LYNXNEW CEngine(this);

			return  lpEngine; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CGame* CKernel::vCreateGame()
		{
			m_lpGame = LYNXNEW CGame(this);
			m_lpGame->vCreate();
			m_lpGame->vInit();

			return  m_lpGame;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetupDeviceType()
        {
			#if defined(__WIN32__) && defined(__DESKTOP__) 
				m_DeviceType = DT_PC;
			#endif            
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetupCPUClass()
		{
			m_CPUClass = DC_HIGH;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetupGPUClass()
		{
			m_GPUClass = DC_ULTRA;			
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetupMemoryClass()
		{
			m_MemoryClass = DC_HIGH;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::LogUnreleasedMemoryBlocks(void)
		{	
			LYNXCHAR Number[32], Line[32];
			CList<CMemAllocator::MemoryChunk>::CIterator Mem;

			/*
			if (m_lpMemAllocator && m_lpMemAllocator->vGetMemoryChunkList())
			{
				LYNX_LOG(glpLogger, CString(_T("\r\n")));
				LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased memory blocks -------------------------------\r\n")));
				lynxItoA(m_lpMemAllocator->vGetMemoryChunkList()->size(), Number, 10);
				LYNX_LOG(glpLogger, CString(Number)+CString(_T(" unreleased memory blocks!!\r\n")));
				for (Mem = m_lpMemAllocator->vGetMemoryChunkList()->begin(); Mem != m_lpMemAllocator->vGetMemoryChunkList()->end(); ++Mem)
				{
					CObj* Obj = NULL;
					//Obj = static_cast<CObj*>((*Mem).Address);
					
					lynxItoA((*Mem).Size, Number, 10);
					lynxItoA((*Mem).Line, Line, 10);
					if (Obj)
					{
						LYNX_LOG(glpLogger, CString(_T("Memory allocated by ")) + CString((*Mem).File) + CString(_T("<Func = ")) + 
																				  CString((*Mem).Function) + CString(_T(", Line = ")) + 
																				  CString(Line) + CString(_T(", Size = ")) + 
																				  CString(Number) + CString(_T(", Name = ")) + 
																				  Obj->GetName() + CString(_T("> is not correctly released !!!\r\n")));					
					}
					else
					{
						LYNX_LOG(glpLogger, CString(_T("Memory allocated by ")) + CString((*Mem).File) + CString(_T("<Func = ")) + 
																				  CString((*Mem).Function) + CString(_T(", Line = ")) + 
																				  CString(Line) + CString(_T(", Size = ")) + 
																				  CString(Number) + CString(_T("> is not correctly released !!!\r\n")));					
					}
				}		
				LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased memory blocks -------------------------------\r\n")));
				LYNX_LOG(glpLogger, CString(_T("\r\n")));
			}
			*/

		#ifdef LYNXMEMALLOCATOR
			if (LYNXMEMALLOCATOR::vGetMemoryChunkList())
			{
				LYNX_LOG(glpLogger, CString(_T("\r\n")));
				LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased memory blocks -------------------------------\r\n")));
				lynxItoA(LYNXMEMALLOCATOR::vGetMemoryChunkList()->size(), Number, 10);
				LYNX_LOG(glpLogger, CString(Number) + CString(_T(" unreleased memory blocks!!\r\n")));
				for (Mem = LYNXMEMALLOCATOR::vGetMemoryChunkList()->begin(); Mem != LYNXMEMALLOCATOR::vGetMemoryChunkList()->end(); ++Mem)
				{
					CObj* Obj = NULL;
					//Obj = static_cast<CObj*>((*Mem).Address);

					lynxItoA((*Mem).Size, Number, 10);
					lynxItoA((*Mem).Line, Line, 10);
					if (Obj)
					{
						LYNX_LOG(glpLogger, CString(_T("Memory allocated by ")) + CString((*Mem).File) + CString(_T("<Func = ")) +
							CString((*Mem).Function) + CString(_T(", Line = ")) +
							CString(Line) + CString(_T(", Size = ")) +
							CString(Number) + CString(_T(", Name = ")) +
							Obj->GetName() + CString(_T("> is not correctly released !!!\r\n")));
					}
					else
					{
						LYNX_LOG(glpLogger, CString(_T("Memory allocated by ")) + CString((*Mem).File) + CString(_T("<Func = ")) +
							CString((*Mem).Function) + CString(_T(", Line = ")) +
							CString(Line) + CString(_T(", Size = ")) +
							CString(Number) + CString(_T("> is not correctly released !!!\r\n")));
					}
				}
				LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased memory blocks -------------------------------\r\n")));
				LYNX_LOG(glpLogger, CString(_T("\r\n")));
			}
		#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CKernel::~CKernel(void)
		{
			//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			
			vRelease();

			SetMultiTask(LYNX_TRUE);
			#ifdef __WIN32__	
				#ifdef __WINAPP__
				#elif _WINDOWS || (_LIB && !_XBOX)				
					PostMessage(m_Window, WM_CLOSE, 0L, 0L);
					DestroyWindow(m_Window);						
				#endif		
			#endif				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vRelease()
		{
			CList<CUIPage*>::CIterator Page;			
			CList<GUISystem::CControl*>::CIterator Control;			
			
			for (Page = m_lpUIPageList.begin(); Page != m_lpUIPageList.end(); ++Page)
				LYNXDEL (*Page);
			m_lpUIPageList.clear();
			
			if (m_OffscreenSprite)
				m_lpEngine->DestroyContainer(m_OffscreenSprite);
			m_OffscreenSprite = NULL;			

			if (m_ScreenFilterSprite)
				m_lpEngine->DestroyContainer(m_ScreenFilterSprite);
			m_ScreenFilterSprite = NULL;						

			for (int i = 0; i < 2; i++)
			{
				if (m_Border[i])
				{
					m_lpEngine->DestroyContainer(m_Border[i]);
					m_Border[i] = NULL;
				}
			}

			if (m_lpGame)
			{
				LYNXDEL m_lpGame;
				m_lpGame = NULL;
			}

			if (m_lpEngine)
			{
				m_lpEngine->ClearInvalidResource();
				LYNXDEL m_lpEngine;		
			}
			m_lpEngine = NULL;

			LogUnreleasedMemoryBlocks();
						
			/*
			CMemAllocator* lpMemAlloc = m_lpMemAllocator;
			m_lpMemAllocator = NULL;
			if (lpMemAlloc )
				delete lpMemAlloc;
			m_lpMemAllocator = NULL;			
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vCreateMemAllocator()
		{	
			/*
			#ifdef __WEB__
				m_lpMemAllocator = NULL;
			#else
				#if defined(DEBUG) || defined(_DEBUG)
					//m_lpMemAllocator = new LynxEngine::CDebugMemAllocator();
				#else
					//m_lpMemAllocator = new LynxEngine::CSmallObjAllocator(4096, 256);					
				#endif					
			#endif			
			*/
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		CString CKernel::vGetGameVersion()
		{
			CString Version = _T("0.0");            
            return Version;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vGenerateUDID(LYNXBOOL bforce_generate_new_udid)
		{
			LYNXGUID GUID;

			lynxOSMakeGUID(&GUID);
			m_UDIDString = CString(lynxOSGUIDToString(&GUID));
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vGenerateDeviceToken()
        {            
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		CString CKernel::vGetSystemLanguageCode()
        {       
			return CString(_T("eng"));
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vLoadBorder()
		{
			LynxEngine::CSpriteCreationDesc Desc;

			for (int i = 0; i < 2; i++)
			{
				if (m_Border[i])
				{
					m_lpEngine->DestroyContainer(m_Border[i]);
					m_Border[i] = NULL;
				}

				if (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1 > 0)
				{
					float ULen;
					float UOffset;

					m_Border[i] = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
					GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
					Desc.Default();
					Desc.m_TextureDesc.MipMapLevel = 1;
					Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
					Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
					Desc.m_lpStream = NULL;
					Desc.m_FileName = _T("ui/border.png");
					Desc.m_bUseBufferObject = LYNX_TRUE;
					Desc.m_Number = 0;
					Desc.m_bInvertTexCoordV = LYNX_FALSE;
					Desc.m_Name = CString(_T("Border"));
					Desc.m_MaterialName = CString(_T("Border.Material"));

					switch (i)
					{
					case 0:
						Desc.m_Width = (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1 + BORDER_BUFFER);
						Desc.m_Height = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());						
						Desc.m_UVOffset[0].y = 0.0f;
						Desc.m_UVOffset[1].x = 0.0f;
						//Desc.m_UVOffset[0].x = 1.0f;
						//Desc.m_UVOffset[1].y = (Desc.m_Height / (Desc.m_Width / 203.0f*512.0f));
						Desc.m_UVOffset[0].x = LYNX_MIN((Desc.m_Width / 256.0f), 1.0f);
						Desc.m_UVOffset[1].y = (Desc.m_UVOffset[0].x == 1.0f) ? (Desc.m_Height / (Desc.m_Width / 256.0f*512.0f)) : ((float)Desc.m_Height / 512.0f);
						ULen = Desc.m_UVOffset[1].y;
						UOffset = ULen * 0.5f;
						Desc.m_UVOffset[0].y = -UOffset;
						while (Desc.m_UVOffset[0].y < 0)
						{
							Desc.m_UVOffset[0].y += 1.0f;
						}
						Desc.m_UVOffset[1].y += Desc.m_UVOffset[0].y;
						break;

					case 1:
						Desc.m_Width = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth() - m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2 + BORDER_BUFFER);
						Desc.m_Height = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
						Desc.m_UVOffset[0].x = 0.0f;
						Desc.m_UVOffset[0].y = 0.0f;						
						//Desc.m_UVOffset[1].x = 1.0f;
						//Desc.m_UVOffset[1].y = (Desc.m_Height / (Desc.m_Width / 203.0f*512.0f));
						Desc.m_UVOffset[1].x = LYNX_MIN((Desc.m_Width / 256.0f), 1.0f);
						Desc.m_UVOffset[1].y = (Desc.m_UVOffset[1].x == 1.0f) ? (Desc.m_Height / (Desc.m_Width / 256.0f*512.0f)) : ((float)Desc.m_Height / 512.0f);
						ULen = Desc.m_UVOffset[1].y;
						UOffset = ULen * 0.5f;
						Desc.m_UVOffset[0].y = -UOffset;
						while (Desc.m_UVOffset[0].y < 0)
						{
							Desc.m_UVOffset[0].y += 1.0f;
						}
						Desc.m_UVOffset[1].y += Desc.m_UVOffset[0].y;
					}										
					Desc.m_TextureDesc.Default();
					Desc.m_TextureDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_WRAP;
					Desc.m_TextureDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_WRAP;
					m_Border[i]->vCreate(&Desc);
					m_Border[i]->SetAlignmentType(0);
					m_Border[i]->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
					m_Border[i]->GetlpCurrentMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Red = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Green = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Blue = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Alpha = 255;
				}
				else if (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1 > 0)
				{
					float ULen;
					float UOffset;

					m_Border[i] = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
					GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
					Desc.Default();
					Desc.m_TextureDesc.MipMapLevel = 1;
					Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
					Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
					Desc.m_lpStream = NULL;
					Desc.m_FileName = _T("ui/border2.png");
					Desc.m_bUseBufferObject = LYNX_TRUE;
					Desc.m_Number = 0;
					Desc.m_bInvertTexCoordV = LYNX_FALSE;
					Desc.m_Name = CString(_T("Border2"));
					Desc.m_MaterialName = CString(_T("Border2.Material"));

					switch (i)
					{
					case 0:
						Desc.m_Width = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth());
						Desc.m_Height = (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1 + BORDER_BUFFER);
						Desc.m_UVOffset[0].x = 0.0f;
						Desc.m_UVOffset[1].y = 0.0f;
						//Desc.m_UVOffset[1].x = 1.0f;
						//Desc.m_UVOffset[1].y = (Desc.m_Height / (Desc.m_Width / 203.0f*512.0f));
						Desc.m_UVOffset[0].y = LYNX_MIN((Desc.m_Height / 128.0f), 1.0f);
						Desc.m_UVOffset[1].x = (Desc.m_UVOffset[0].y == 1.0f) ? (Desc.m_Width / (Desc.m_Height / 128.0f*256.0f)) : ((float)Desc.m_Width / 256.0f);
						ULen = Desc.m_UVOffset[1].x;
						UOffset = ULen * 0.5f;
						Desc.m_UVOffset[0].x = -UOffset;
						while (Desc.m_UVOffset[0].x < 0)
						{
							Desc.m_UVOffset[0].x += 1.0f;
						}
						Desc.m_UVOffset[1].x += Desc.m_UVOffset[0].x;
						break;

					case 1:
						Desc.m_Width = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth()); 
						Desc.m_Height = (GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight() - m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y2 + BORDER_BUFFER);						
						Desc.m_UVOffset[0].y = 0.0f;
						Desc.m_UVOffset[0].x = 0.0f;
						//Desc.m_UVOffset[0].x = 1.0f;
						//Desc.m_UVOffset[1].y = (Desc.m_Height / (Desc.m_Width / 203.0f*512.0f));
						Desc.m_UVOffset[1].y = LYNX_MIN((Desc.m_Height / 128.0f), 1.0f);
						Desc.m_UVOffset[1].x = (Desc.m_UVOffset[1].y == 1.0f) ? (Desc.m_Width / (Desc.m_Height / 128.0f*256.0f)) : ((float)Desc.m_Width / 256.0f);
						ULen = Desc.m_UVOffset[1].x;
						UOffset = ULen * 0.5f;
						Desc.m_UVOffset[0].x = -UOffset;
						while (Desc.m_UVOffset[0].x < 0)
						{
							Desc.m_UVOffset[0].x += 1.0f;
						}
						Desc.m_UVOffset[1].x += Desc.m_UVOffset[0].x;					
						break;
					}
					Desc.m_TextureDesc.Default();
					Desc.m_TextureDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_WRAP;
					Desc.m_TextureDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_WRAP;
					m_Border[i]->vCreate(&Desc);
					m_Border[i]->SetAlignmentType(0);
					m_Border[i]->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
					m_Border[i]->GetlpCurrentMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Red = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Green = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Blue = 255;
					m_Border[i]->GetlpCurrentMaterial()->m_Diffuse.Alpha = 255;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CKernel::vInit(const int argc, const char **argv, INIT_DESC* desc, const int w, const int h, const LYNXBOOL bfull)
		{
			m_CurrentGameTime = 0;

			SetupDeviceType();
			SetupCPUClass();
			SetupGPUClass();
			SetupMemoryClass();

			m_lpEngine = vCreateEngine();
			m_lpEngine->vCreate(desc);

			for (int i=0; i<argc; i++)
			{
				if (argv[i])
				{
					LynxEngine::CString sArg = argv[i];
					if (sArg == _T("-notouch"))
					{
						m_lpEngine->m_bUseTouchDevice = LYNX_FALSE;
					}					
				}
			}
			
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

            //this->vSetGestureRecognization(LYNX_TRUE);
            
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
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetTitle(LYNXCHAR *title)
		{
			LYNXCHAR		APIName[64];
			LYNXCHAR		OSName[64];	
			LYNXCHAR		Title[128];
			
			if (title)
				lynxStrCpy(Title, title);
			else
			{
				lynxStrCpy(APIName, ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetAPIString());			
				#ifdef __WIN32__
					lynxStrCpy(OSName, _T("Windows"));
				#elif __LINUX__
					lynxStrCpy(OSName, _T("Linux"));
				#elif __OS_X__
					lynxStrCpy(OSName, _T("OS X"));
				#endif	
				lynxSprintf(Title, _T("API::%s, OS::%s(LynxEngine Ver.%d.%d.%d). Copyright (C) 2003.fallingCAT Studios."), APIName, OSName, m_lpEngine->GetMajorVersion(), m_lpEngine->GetMinorVersion(), m_lpEngine->GetBuildVersion());
			}
			lynxOSSetTitle(m_Window, Title);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::RegisterUIPage(const CString& t, CUIPage* (*func)(CKernel*))
		{
			m_UIPageFactory.Register(t, func);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CUIPage* CKernel::CreateUIPage(const CString& name)
		{
			CUIPage* lpPage = m_UIPageFactory.CreateObject(name);
			
			return lpPage;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CUIPage* CKernel::FindUIPage(const CString& name)
		{
			CList<CUIPage*>::CIterator Page;
			
			Page = LynxEngine::find(m_lpUIPageList.begin(), m_lpUIPageList.end(), name);
			if (Page == m_lpUIPageList.end())
				return NULL;
			
			return (*Page);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::PauseGameTime(LYNXBOOL b) 
		{
			static LYNXLONGLONG T0 = 0;

			if (b == m_bPauseGameTime)
				return;

			m_bPauseGameTime = b; 

			if (b)
			{
				T0 = lynxOSGetPerformanceCounter();
			}
			else
			{
				m_PauseTime += (lynxOSGetPerformanceCounter() - T0); 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXLONGLONG CKernel::GetKernelTime()
		{
			LYNXLONGLONG T = lynxOSGetPerformanceCounter();
			return (T - m_PauseTime);
		
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		float CKernel::ElapsedKernelTime(const LYNXLONGLONG t0, const LYNXLONGLONG t1)
		{
			return ((t1-t0) * 1000.0f) / ((float)lynxOSGetPerformanceFrequency());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		float CKernel::ElapsedKernelTimeSince(const LYNXLONGLONG t0)
		{
			LYNXLONGLONG T = GetKernelTime();
			return ((T-t0) * 1000.0f) / ((float)lynxOSGetPerformanceFrequency());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXLONGLONG CKernel::GetGameTime()
		{
			return m_CurrentGameTime;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		float CKernel::ElapsedGameTimeSince(const LYNXLONGLONG t0)
		{
			LYNXLONGLONG T = GetGameTime();
			return ((T - t0) * 1000.0f *  GetlpEngine()->GetStepMultiplier()) / ((float)lynxOSGetPerformanceFrequency());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXLONGLONG CKernel::ElapsedTimeStampSince(const LYNXLONGLONG t0)
		{
			LYNXLONGLONG T = lynxOSGetTimeStamp();
			return (T-t0);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		float CKernel::GetCurrentFPS()
		{
			float FPS = 1000.0f / (float)m_TotalFrameElapsedTime;
			return FPS;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		float CKernel::GetFreePhysicalMemorySize()
		{
			LYNXLONGLONG Size = lynxGetAvailablePhysicalMem();
			float SizeInMega = Size / (1048576);

			return SizeInMega;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		float CKernel::GetUsedPhysicalMemorySize()
		{
			LYNXLONGLONG Size = lynxGetUsedPhysicalMem();
			float SizeInMega = Size / (1048576);

			return SizeInMega;
		}		
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::LoseInputFocus(GUISystem::CContext* c)
        {
            if (m_CurrentInputFocusedContext == c)
            {
                m_CurrentInputFocusedContext = m_lpCurrentUIPage->GetContext();
            }
        }		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CKernel::vLoop()
		{			
			if (IsRunning()) 
			{					
				//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->ResetAllCounters();
                
				PERF_COUNTER_RESET(_T("Renderer"), _T("SetRS"), 0)
					
				GetEvent();					
				m_lpEngine->GetlpInputSystem()->Poll();
				
				LYNXLONGLONG T = lynxOSGetPerformanceCounter();
				m_FrameElapsedTime = ((T - m_LastFrameTime)*1000.0f)/((float)lynxOSGetPerformanceFrequency());
				if (!m_bPauseGameTime)
				{
					m_CurrentGameTime += (LYNXLONGLONG)((T - m_LastFrameTime)*m_lpEngine->GetStepMultiplier());
				}				
				m_LastFrameTime = T;				
				float Step = ((float)m_FrameElapsedTime)/(1000.0f/m_lpEngine->m_UpdateRate);				
				
				LYNX_ASSERT(m_lpCurrentUIPage);										
				
				// Initialize UI page if necessary.
				if (m_lpCurrentUIPage->DoesNeedInit())
				{					
					m_lpCurrentUIPage->vInit();							
					m_Step = 1.0f;
				}

				// UIPage loop
				LYNXBOOL bTimeToRender = LYNX_FALSE;

				//lynxOSLockMutex(&m_Mutex);

				if (m_lpEngine->GetlpSystemInputRecord() && (m_lpEngine->GetlpInputSystem()->IsRecording() || m_lpEngine->GetlpInputSystem()->IsPlayingBack()))
				{			
					m_lpEngine->m_UpdateRate = 30.0f;

					// Update scene based on update rate of engine
					m_TotalFrameElapsedTime += (float)m_FrameElapsedTime;
					if (m_TotalFrameElapsedTime >= (1000.0f/m_lpEngine->m_UpdateRate))
					{				
						m_Step = 1.0f;					

						if (m_lpEngine->GetlpInputSystem()->IsRecording())
							m_lpEngine->GetlpSystemInputRecord()->Record();														

						//if (!m_bUITransiting && !m_bPause)
						if (!m_bPause)
						{
							if (m_ModalWindow)
							{
								m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha += m_Step*16;
								if (m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha > 170)
									m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha = 170;
								m_ModalWindow->GetContext()->GetInputFocus();
								m_ModalWindow->vLoop(m_Step);
								m_ModalWindow->GetContext()->ClearAllInputData();
								if (!m_ModalWindow->IsActive())
								{
									SetModalWindow(NULL);									
									m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha = 0;
								}
							}
							m_lpCurrentUIPage->vLoop(m_Step);																				
						}															
						bTimeToRender = LYNX_TRUE;
					}								

					// Engine loop				
					if (!m_bPause)
					{					
						m_lpEngine->vLoop(1.0f);				
					}		

					if (bTimeToRender && !m_bPauseRendering)
					{				
						// start rendering
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Start();													

							vRender();						
							m_lpEngine->GetlpGameConsoleSystem()->vRender();
							if (m_ModalWindow)
							{							
								m_lpEngine->GetlpGraphicsSystem()->SetFullScreenViewport();														
								m_ScreenFilterSprite->vRender();
								m_lpEngine->GetlpGraphicsSystem()->SetViewportRect();														
								m_ModalWindow->vRender();
							}
							m_lpEngine->ShowData();
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->End();			
						#ifdef __iOS__
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DiscardRenderBuffer(0);
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DiscardDepthBuffer();
						#endif					
						((CEngine*)m_lpEngine)->SwapBackBuffer();

						m_TotalFrameElapsedTime = 0.0f;
						m_FrameCount++;
					}
				}
				else
				{
					if (m_lpEngine->m_bLockFrameRate)
					{
						// Update scene based on update rate of engine
						m_TotalFrameElapsedTime += (float)m_FrameElapsedTime;
						if (m_TotalFrameElapsedTime >= (1000.0f/m_lpEngine->m_UpdateRate))
						{				
							m_Step = ((float)m_TotalFrameElapsedTime)/(1000.0f/m_lpEngine->m_UpdateRate);// * m_lpEngine->GetStepMultiplier();
							if (m_Step > 5.0f)
								m_Step = 5.0f;									
						
							//if (!m_bUITransiting && !m_bPause)
							if (!m_bPause)
							{
								if (m_ModalWindow)
								{
									m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha += m_Step*16;
									if (m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha > 170)
										m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha = 170;
									m_ModalWindow->GetContext()->GetInputFocus();
									m_ModalWindow->vLoop(m_Step);
									m_ModalWindow->GetContext()->ClearAllInputData();
									if (!m_ModalWindow->IsActive())
									{
										SetModalWindow(NULL);
										m_ScreenFilterSprite->GetlpCurrentMaterial()->m_Diffuse.Alpha = 0;
									}
								}
								m_lpCurrentUIPage->vLoop(m_Step);																				
							}															
							bTimeToRender = LYNX_TRUE;
						}								
					}
					else
					{
						m_Step = Step;
						if (m_Step > 5.0f)
							m_Step = 5.0f;

						m_TotalFrameElapsedTime = m_FrameElapsedTime;					
									
						if (!m_bUITransiting && !m_bPause)
						{	
							if (!m_lpCurrentUIPage->IsQuitting())
								m_lpCurrentUIPage->vLoop(m_Step);		

							if (m_ModalWindow)
							{
								m_ModalWindow->GetContext()->GetInputFocus();
								m_ModalWindow->vLoop(m_Step);
								if (!m_ModalWindow->IsActive())
								{
									SetModalWindow(NULL);
								}
							}						
						}						
						bTimeToRender = LYNX_TRUE;
					}	

					// Engine loop				
					if (!m_bPause)
					{					
						m_lpEngine->vLoop((Step > 5.0f) ? 5.0f : Step);				
					}		

					if (bTimeToRender && !m_bPauseRendering)
					{				
						// start rendering
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Start();													

							vRender();									
							m_lpEngine->GetlpGameConsoleSystem()->vRender();
							if (m_ModalWindow)
							{							
								m_lpEngine->GetlpGraphicsSystem()->SetFullScreenViewport();														
								m_ScreenFilterSprite->vRender();
								m_lpEngine->GetlpGraphicsSystem()->SetViewportRect();														
								m_ModalWindow->vRender();
							}
							m_lpEngine->ShowData();
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->End();			
						#ifdef __iOS__
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DiscardRenderBuffer(0);
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DiscardDepthBuffer();
						#endif					
						((CEngine*)m_lpEngine)->SwapBackBuffer();

						m_TotalFrameElapsedTime = 0.0f;
						m_FrameCount++;
					}
				}
				// Quit the UIPage if necessary
				if (m_lpCurrentUIPage->IsQuitting())
				{					
					m_lpCurrentUIPage->vQuit();						
				}				

				if (m_KBD[VK_TAB])
				{
					m_KBD[VK_TAB] = LYNX_FALSE;					
					m_lpEngine->GetlpGameConsoleSystem()->Trigger();
				}				
				else if (m_lpEngine->GetlpGameConsoleSystem()->IsRunning())
				{			
					if (m_KBD[VK_RETURN])
					{
						m_KBD[VK_RETURN] = LYNX_FALSE;
						m_lpEngine->GetlpGameConsoleSystem()->ExecuteCommandLine();
					}
					else if (m_KBD[VK_BACK])
					{
						m_KBD[VK_BACK] = LYNX_FALSE;
						m_lpEngine->GetlpGameConsoleSystem()->CommandLineBack();
					}					
					else if (m_KBD[VK_UP])
					{
						m_KBD[VK_UP] = LYNX_FALSE;
						m_lpEngine->GetlpGameConsoleSystem()->LastCommandLineFromStack();
					}
					else if (m_KBD[VK_DOWN])
					{
						m_KBD[VK_DOWN] = LYNX_FALSE;
						m_lpEngine->GetlpGameConsoleSystem()->NextCommandLineFromStack();
					}
					
					if (m_CharPressed != VK_TAB && m_CharPressed != VK_BACK)
					{
						m_lpEngine->GetlpGameConsoleSystem()->CommandLineCat(m_CharPressed);
						m_CharPressed = 0;
					}				
				}

				if (0)//m_lpEngine->GetlpSystemInputRecord() && (m_lpEngine->GetlpInputSystem()->IsPlayingBack() || m_lpEngine->GetlpInputSystem()->IsRecording()))
				{
					#if defined(__WXWIDGET__)
						if (gControlPanelWnd)
						{
							wxString Title;
							LYNXCHAR Buffer[356];

							lynxSprintf(Buffer, _T("[%d] - Playback [%d]"), glpKernel->GetFrameCount(),  m_lpEngine->GetlpInputSystem()->m_CurrentPlaybackFrame);
							Title = wxString(Buffer);							
							gControlPanelWnd->SetTitle(Title);
						}
					#endif
				}


				//lynxOSUnlockMutex(&m_Mutex);
				
				return LYNX_TRUE;
			}	
			else
			{
				m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);				
				return LYNX_FALSE;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
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
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vPauseRendering()
		{
			m_bPauseRendering = LYNX_TRUE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
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
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vResumeRendering()
		{
			m_bPauseRendering = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
        void CKernel::vTerminate()
		{
			SetRunning(LYNX_FALSE);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
        void CKernel::vSwapBackBuffer()
        {
            ((CEngine*)m_lpEngine)->SwapBackBuffer();
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vRenderBorder()
		{			
			if (!m_bRenderBorder)
				return;

			m_lpEngine->GetlpGraphicsSystem()->SetFullScreenViewport();
			m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);

			if (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1 > 0)
			{
				for (int i = 0; i < 2; i++)
				{
					if (m_Border[i])
					{
						switch (i)
						{
						case 0:
							m_Border[i]->vSetPosition(CVector3(0.0f, 0.0f, 0.0f));
							break;
						case 1:
							m_Border[i]->vSetPosition(CVector3(m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2 - BORDER_BUFFER, 0.0f, 0.0f));
							break;
						}
						m_Border[i]->vRender();
					}
				}
			}
			else if (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1 > 0)
			{
				for (int i = 0; i < 2; i++)
				{
					if (m_Border[i])
					{
						switch (i)
						{
						case 0:
							m_Border[i]->vSetPosition(CVector3(0.0f, 0.0f, 0.0f));
							break;
						case 1:
							m_Border[i]->vSetPosition(CVector3(0.0f, m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y2 - BORDER_BUFFER, 0.0f));
							break;
						}
						m_Border[i]->vRender();
					}
				}
			}			
			m_lpEngine->GetlpGraphicsSystem()->SetViewportRect();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vRender()
		{
            if (m_lpEngine->GetSystemTTFontString())
                m_lpEngine->GetSystemTTFontString()->Reset();
            
			if (m_bUITransiting)
			{
				switch (m_UITransitionMode)
				{
					case UTM_SWIPE:
						m_lpCurrentUIPage->vRender();
						break;
					case UTM_FADEOUT:
						m_lpCurrentUIPage->vRender();
						break;
					case UTM_ALPHA_FADEOUT:
						m_lpCurrentUIPage->vRender();
						break;
                    case UTM_SCALE_ROLL_FADEOUT:
					case UTM_SCALE_FADEOUT:
						m_lpCurrentUIPage->vRender();
						break;
				}
				RenderUITransition();				
			}
			else
			{
				m_lpCurrentUIPage->vRender();					
			}			
			vRenderBorder();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CKernel::GetEvent()
		{
			#ifdef __WIN32__
				MSG							Msg;
				DWORD						Res;
				static int					i = 0;
				int							Type, SI;
			#elif __XWIN__
				XEvent						Event;	
			#elif __OS_X__
				EventRecord					Event;
				short						Part;
				Boolean						Hit;
				char						Key;
				Rect						TempRect;
				WindowRef					WhichWindow;
				long						MenuResult;
				short						MenuID;		/* the resource ID of the selected menu */
				short						MenuItem;	/* the item number of the selected menu */
			#endif			

			#ifdef __WIN32__		
				/*----temp-------
				if (!m_lpEngine->GetSDK().bSoundLoopThread)
					lynxsSoundSystemLoop();
				*/		
				#ifndef __WINAPP__
					if (m_bWaitMessage)
					{
						if (GetMessage(&Msg, NULL, 0, 0))
						{  			
							TranslateMessage(&Msg);						
							DispatchMessage(&Msg);
						}
						else
							return FALSE;
					}
					else
					{
						if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
						{  			
							TranslateMessage(&Msg);						
							DispatchMessage(&Msg);
						}
						else
							return FALSE;
					}				
				#endif
			#elif  __WINCE__		
				if (m_lpEngine->GetSDK().bWaitMessage)
				{
					if (GetMessage(&Msg, NULL, 0, 0))
					{  			
						TranslateMessage(&Msg);						
						DispatchMessage(&Msg);
					}
					else
						return FALSE;
				}
				else
				{
					if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
					{  			
						TranslateMessage(&Msg);						
						DispatchMessage(&Msg);
					}
					else
						return FALSE;
				}				
			#elif __XWIN__
				//do {
						if (XEventsQueued(m_lpEngine->GetSDK().Renderer.SystemData.Disp, QueuedAfterFlush))
						{
							XNextEvent(m_lpEngine->GetSDK().Renderer.SystemData.Disp,  &Event);
      						switch(Event.type)
							{
								case ButtonPress:
				       				break;

								case KeyPress:
									m_lpEngine->GetSDK().KBD[(XLookupKeysym(&Event.xkey, 0))&0xff] = TRUE; 
									break;

								case KeyRelease:
									m_lpEngine->GetSDK().KBD[(XLookupKeysym(&Event.xkey, 0))&0xff] = FALSE; 
									break;
							}
						}
				//}while (XPending(m_lpEngine->GetSDK().Renderer.SystemData.BackBuffer.Disp));  //loop to compress events/
			#elif __OS_X__
 				if (!WaitNextEvent(everyEvent, &Event, 0, nil))
					return FALSE;
		            
				switch (Event.what) 
				{
					case mouseDown:
						Part = FindWindow(Event.where, &WhichWindow);
						switch (Part)
						{
							case inMenuBar:  /* process a moused menu command */
								MenuResult = MenuSelect(Event.where);	
								MenuID = HiWord(MenuResult);    /* use macros to get item & menu number */
								MenuItem = LoWord(MenuResult);
			
								switch (MenuID) 
								{
									case 128:
										switch (MenuItem) 
										{
											case 11://iQuit:
												//ExitToShell();
												m_lpEngine->GetSDK().bRunning = FALSE;
												break;
						
											default:
												break;
										}
										break;
		        
								}
								HiliteMenu(0);	/* unhighlight what MenuSelect (or MenuKey) hilited */
								break;
		                    
							case inSysWindow:
								break;
		                
							case inContent:
								if (WhichWindow != FrontWindow()) 
									SelectWindow(WhichWindow);
								break;
		                
							case inDrag:	/* pass screenBits.bounds */
								GetRegionBounds(GetGrayRgn(), &TempRect);
								DragWindow(WhichWindow, Event.where, &TempRect);
								break;
		                    
							case inGrow:
								break;

		                    
							case inGoAway:
								if (TrackGoAway(WhichWindow, Event.where))
								{
									DisposeWindow(WhichWindow);
									m_lpEngine->GetSDK().bRunning = FALSE;
								}
								break;
		                    
							case inZoomIn:
							case inZoomOut:
								Hit = TrackBox(WhichWindow, Event.where, Part);
								if (Hit) 
								{
									SetPort(GetWindowPort(WhichWindow));   // window must be current port
									EraseRect(GetWindowPortBounds(WhichWindow, &TempRect));   // inval/erase because of ZoomWindow bug
									ZoomWindow(WhichWindow, Part, true);
									InvalWindowRect(WhichWindow, GetWindowPortBounds(WhichWindow, &TempRect));	
								}
								break;
						}
						break;
		                                        
					case kHighLevelEvent:
						AEProcessAppleEvent(&Event);
						break;
				
					case diskEvt:
						break;
				}

			#endif	

			//Yahoo!!
			return TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::RenderUITransition()
		{
			m_lpEngine->GetlpGraphicsSystem()->SetFullScreenViewport();
			switch (m_UITransitionMode)
			{
				case UTM_SWIPE:
					SwipeOffscreen();
					break;

				case UTM_FADEOUT:
					FadeOut();
					break;

				case UTM_ALPHA_FADEOUT:
					AlphaFadeOut();
					break;

				case UTM_SCALE_FADEOUT:
					ScaleFadeOut();
					break;
                    
                case UTM_SCALE_ROLL_FADEOUT:
                    ScaleRollFadeOut();
                    break;
			}			
			m_lpEngine->GetlpGraphicsSystem()->SetViewportRect();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::FadeOut()
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);
			float FadeOutStep = m_Step * m_FadeOutSpeed;
			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;

			if (m_bUITransiting)
			{
				if (!m_bFadeIn)
				{
					BDesc.Default();
					BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
					BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
					GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

					if (Color.r >= FadeOutStep)
					{
						GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
						GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);					
                        GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
						GetOffscreenSprite()->vRender(NULL);			
						
						Color.r -= FadeOutStep;
						Color.g -= FadeOutStep;
						Color.b -= FadeOutStep;					
					}
					else
					{
						m_bFadeIn = LYNX_TRUE;
						Color.r = Color.g = Color.b = 0.0f;
						Color.a = 1.0f;
                        
                        GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
						GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);			
                        GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
						GetOffscreenSprite()->vRender(NULL);
					}
				}
				else
				{
					BDesc.Default();
					BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
					BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
					BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
					BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
					GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

					if (Color.a >= FadeOutStep)
					{
						GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
						GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);	
						GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::BLACK_TEXTURE));
						GetOffscreenSprite()->vRender(NULL);			
						
						Color.a -= FadeOutStep;						
					}					
					else 
					{
						Color.r = Color.g = Color.b = Color.a = 1.0f;
						m_bUITransiting = LYNX_FALSE;
						m_bFadeIn = LYNX_FALSE;
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------		
		void CKernel::AlphaFadeOut()
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);
			float FadeOutStep = m_Step * m_FadeOutSpeed;
			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;

			if (m_bUITransiting && !m_lpCurrentUIPage->IsQuitting())
			{
				BDesc.Default();
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

				if (Color.a >= FadeOutStep)
				{
					GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
					GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);		
                    GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
					GetOffscreenSprite()->vRender(NULL);			
					
					Color.a -= FadeOutStep;
				}
				else 
				{
					Color.r = Color.g = Color.b = Color.a = 1.0f;
					m_bUITransiting = LYNX_FALSE;					
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------		
		void CKernel::ScaleFadeOut()
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);
			float FadeOutStep = m_Step * m_FadeOutSpeed;
			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;

			if (m_bUITransiting && !m_lpCurrentUIPage->IsQuitting())
			{
				BDesc.Default();
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

				if (Color.a >= FadeOutStep)
				{
					GetOffscreenSprite()->Scale(Math::CVector3(1.0f+FadeOutStep, 1.0f+FadeOutStep, 1.0f), LYNX_MATH_PRE);
                 	GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
					GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);		
                    GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
					GetOffscreenSprite()->vRender(NULL);			
					
					Color.a -= FadeOutStep;
				}
				else 
				{
					Color.r = Color.g = Color.b = Color.a = 1.0f;
					m_bUITransiting = LYNX_FALSE;
					GetOffscreenSprite()->ResetTransform();
				}
			}
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------		
		void CKernel::ScaleRollFadeOut()
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);
			float FadeOutStep = m_Step * m_FadeOutSpeed;
            float RollingSpeed = m_Step * m_RollingSpeed;
			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
            
			if (m_bUITransiting && !m_lpCurrentUIPage->IsQuitting())
			{
				BDesc.Default();
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

				if (Color.a >= FadeOutStep)
				{
					GetOffscreenSprite()->Scale(Math::CVector3(1.0f+FadeOutStep, 1.0f+FadeOutStep, 1.0f), LYNX_MATH_PRE);
                    GetOffscreenSprite()->Roll(RollingSpeed, LYNX_MATH_PRE);
					GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
					GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = (LYNXCOLORRGBA&)(Color);		
                    GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
					GetOffscreenSprite()->vRender(NULL);			
					
					Color.a -= FadeOutStep;
				}
				else 
				{
					Color.r = Color.g = Color.b = Color.a = 1.0f;
					m_bUITransiting = LYNX_FALSE;
					GetOffscreenSprite()->ResetTransform();
				}
			}
		}

		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SwipeOffscreen()
		{
			static LYNXCOLORRGBA Color = {255, 255, 255, 255};

			if (m_bUITransiting && !m_lpCurrentUIPage->IsQuitting())
			{
				if (m_OffscreenPos.x >= -GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth() && 
					m_OffscreenPos.x <=  GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth() &&
					m_OffscreenPos.y >= -GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight() &&
					m_OffscreenPos.y <=  GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight() )
				{
					GetOffscreenSprite()->GetlpCurrentMaterial()->m_Diffuse = Color;
					GetOffscreenSprite()->vSetPosition(Math::CVector3(m_OffscreenPos.x, m_OffscreenPos.y, 0.0f));
                    GetOffscreenSprite()->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->GetCurrent());
					GetOffscreenSprite()->vRender(NULL);
			
					m_OffscreenPos.x += m_OffscreenVelocity*m_OffscreenSwipeDir.x*m_Step;
					m_OffscreenPos.y += m_OffscreenVelocity*m_OffscreenSwipeDir.y*m_Step;
					m_OffscreenVelocity -= 3.0f;
					if (m_OffscreenVelocity < 15.0f)
						m_OffscreenVelocity = 15.0f;

					//if (Color.Alpha < 55)
					//	Color.Alpha = 55;
					//Color.Alpha -= 5;					
				}
				else 
				{
					m_OffscreenPos.x = 0.0f;
					m_OffscreenPos.y = 0.0f;
					m_OffscreenVelocity = 0.0f;
					m_OffscreenSwipeDir.x = 1.0f;
					m_OffscreenSwipeDir.y = 0.0f;
					m_bUITransiting = LYNX_FALSE;
					Color.PackedColor = 0xffffffff;
					GetOffscreenSprite()->vSetPosition(Math::CVector3(0.0f, 0.0f, 0.0f));
					GetOffscreenSprite()->UpdateTransform();
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief C
		*
		*	@param C
		*	@param C  
		*	@return C 
		*/
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vOnSysKeyDown(int key)
		{
			m_KeyPressed = (BYTE)key; 			
			m_KBD[(BYTE)key] = LYNX_TRUE;						
			CEvent KeyDown(CEvent::SYS_KEY_DOWN);		
			KeyDown.m_Param = key;
			m_lpEngine->BroadcastEvent(KeyDown);
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief C
		*
		*	@param C
		*	@param C  
		*	@return C 
		*/
		//-------------------------------------------------------------------------------------------------------
		void CKernel::vOnSysKeyUp(int key)
		{		
			m_KeyPressed = -1;
			m_KBD[(BYTE)key] = LYNX_FALSE;   
			m_KBDUP[(BYTE)key] = LYNX_TRUE;
			CEvent KeyUp(CEvent::SYS_KEY_UP);
			KeyUp.m_Param = key;
			m_lpEngine->BroadcastEvent(KeyUp);
		}
        //-------------------------------------------------------------------------------------------------------
        /**
         *	@brief C
         *
         *	@param C
         *	@param C
         *	@return C
         */
        //-------------------------------------------------------------------------------------------------------
        void CKernel::vOnLowMemoryWarning()
        {
            vOSN_OpenMessageBox(_T("Low Memory Warning!"), _T("Please release some memorries."));
        }
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief C
		*
		*	@param C
		*	@param C
		*	@return C
		*/
		//-------------------------------------------------------------------------------------------------------
		void CKernel::SetModalWindow(GUISystem::CWindow* w) 
		{ 			
			if (w)
			{
				m_LastModalWindow = m_ModalWindow;
				m_ModalWindow = w;
				vOSN_HideAllViews();
			}
			else
			{
				if (m_LastModalWindow && m_ModalWindow != m_LastModalWindow)
				{
					m_ModalWindow = m_LastModalWindow;
					m_LastModalWindow = NULL;
					vOSN_HideAllViews();					
				}
				else
				{
					m_LastModalWindow = NULL;
					m_ModalWindow = NULL;
					vOSN_HideAllViews();								
				}
			}			
		}		
	}	
}
