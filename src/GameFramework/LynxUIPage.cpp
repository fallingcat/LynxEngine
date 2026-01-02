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
//  a︷
//
//###########################################################################

#include <LynxEngine_PCH.h>

#include <GUISystem/LynxButton.h>
#include <GUISystem/LynxSlider.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <GameFramework/LynxUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <InputSystem/LynxInputSystem.h>

namespace LynxEngine
{
	namespace GameFramework
	{
		IMPLEMENT_CLASSSCRIPT(CUIPage, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		CUIPage::CUIPage(CKernel* const lpk)
		: CObj(lpk->GetlpEngine())
		{
			m_lpKernel = lpk;
			m_bNeedInit = LYNX_TRUE;
            m_bRenderToOffscreenForTransition = LYNX_TRUE;
			m_bTransitionRendering = LYNX_FALSE;
			m_bQuit = LYNX_FALSE;
            
			//m_Context->GetControlList().clear();
			//m_ActiveControl = NULL;
            
			m_BackgroundSprite = NULL;
            
            m_Context = NULL;

			m_UIFile = NULL;
			m_bOnBack = LYNX_FALSE;
            
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		CUIPage::~CUIPage(void)
		{	 
			ReleaseLayoutData();

			if (m_lpClassScript)
			{
				ScriptBindObject();
				m_lpClassScript->vOnDestroy();
			}
            
			if (m_BackgroundSprite)
            {
				m_lpEngine->DestroyContainer(m_BackgroundSprite);
                m_BackgroundSprite = NULL;
            }
			
            if (m_Context)
			{
				LYNXDEL m_Context;
				m_Context = NULL;
			}			
            
			m_lpKernel->GetlpEngine()->ClearInvalidResource();            
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUIPage::vCreate(void)
		{
			if (m_lpClassScript)
			{
				ScriptBindObject();
				m_lpClassScript->vOnCreate();
			}           
            
            LYNXRECT Viewport, Rect;            
            /*
			Rect.x1  = 0;
            Rect.y1  = 0;
            Rect.x2  = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth();
            Rect.y2  = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight();
			*/
			Viewport = m_lpEngine->GetlpGraphicsSystem()->GetViewportRect();
			//Rect = Viewport;
			
			Rect.x1 = 0;
			Rect.y1 = 0;
			Rect.x2 = Viewport.x2 - Viewport.x1;
			Rect.y2 = Viewport.y2 - Viewport.y1;
			
			m_Context = LYNXNEW GUISystem::CContext(m_lpEngine, this, NULL, Rect);
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUIPage::vInit(void)
		{
			m_bNeedInit = LYNX_FALSE;
			m_bQuit = LYNX_FALSE;
			m_Context->SetActiveControl(NULL);
            
			for (CList<GUISystem::CControl*>::CIterator Control = m_Context->GetControlList().begin(); Control != m_Context->GetControlList().end(); ++Control)
			{
				(*Control)->vInit();
			}
            
			#ifdef __MOBILE__
				InputSystem::CTouch* lpTouch = m_lpKernel->GetlpEngine()->GetlpSystemTouch();
				if (lpTouch)
				{
					lpTouch->ClearTouches();
					lpTouch->ClearTouchGesture();
				}
			#endif
            
			CObj::vInit();
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::ControlsLoop(float step)
		{
			m_Context->ControlLoop(step);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::vLoop(float step)
		{
			CObj::vLoop(step);
			m_Context->vLoop(step);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::RenderControls()
		{
			m_Context->RenderControls();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::vRender()
		{
			if (m_lpClassScript)
			{
				ScriptBindObject();
				m_lpClassScript->vOnRender();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::SetQuit(LYNXBOOL b)
		{
			if (b)
			{
				if (!m_bQuit)
				{
					if (m_lpKernel->GetUITransitionMode() != UTM_NONE)
					{
						RenderToOffscreen();
						m_lpKernel->Transit(LYNX_TRUE);
					}
					m_bQuit = LYNX_TRUE;
				}
			}
			else
			{
				m_bQuit = LYNX_FALSE;
				m_bNeedInit = LYNX_TRUE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUIPage::vQuit()
		{
			SetQuit(LYNX_FALSE);
            
			if (m_lpClassScript)
			{
				ScriptBindObject();
				m_lpClassScript->vOnQuit();
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::QuitTo(const LYNXCHAR* name)
		{
			CUIPage* NextUIPage = m_lpEngine->FindUIPage(name);
			if (NextUIPage)
			{
    			m_lpEngine->SetCurrentUIPage(NextUIPage);
			}
			else
			{
				CString UIPageClassName = CString(_T("C")) + CString(name);
				NextUIPage = m_lpEngine->CreateUIPage(name, UIPageClassName);
				if (!NextUIPage)
				{
					CString ScriptFileName = CString(_T("../script/C")) + CString(name) + CString(_T(".lua"));
					NextUIPage = m_lpEngine->CreateUIPage(CString(name), ScriptFileName, UIPageClassName);
				}
				m_lpEngine->SetCurrentUIPage(NextUIPage);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer* CUIPage::AddBackground(const LYNXCHAR* filename, const LYNXCOLORRGBA& color, float tex_ratio)
		{
            LYNXRECT ViewportRect = m_lpEngine->GetlpGraphicsSystem()->GetViewportRect();
			int ViewportWidth = ViewportRect.x2 - ViewportRect.x1;
			int ViewportHeight = ViewportRect.y2 - ViewportRect.y1;
            
			m_BackgroundSprite = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			LynxEngine::CSpriteCreationDesc Desc;
			Desc.Default();
			Desc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_FileName = filename;
			Desc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_Number = 0;
			Desc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_Name = filename;
			Desc.m_MaterialName = Desc.m_Name + CString(_T(".Material"));
			Desc.m_Width = ViewportWidth;
			Desc.m_Height = ViewportHeight;
			Desc.ComputeUVOffsetByXYRatio(tex_ratio, (float)(ViewportWidth)/(float)(ViewportHeight));
			m_BackgroundSprite->vCreate(&Desc);
			m_BackgroundSprite->GetlpCurrentMaterial()->m_Diffuse.PackedColor = color.PackedColor;
			m_BackgroundSprite->SetAlignmentType(0);
			m_BackgroundSprite->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
            
			return m_BackgroundSprite;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CButton* CUIPage::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask)
		{
            return m_Context->AddButton(classname, filename, rect, size, id, text, buse_sprite_text, buse_mask);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CButton* CUIPage::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask)
		{
            return m_Context->AddButton(classname, filename, uvoffset, rect, size, id, text, buse_sprite_text, mat_name, buse_mask);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CStateButton* CUIPage::AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask)
		{
            return m_Context->AddStateButton(classname, filename, pressedfilename, rect, size, id, text, buse_sprite_text, mat_name, buse_mask);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CSlider* CUIPage::AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text)
		{
            return m_Context->AddSlider(classname, filename, bar_filename, rect, size, min, max, id, text);            
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CImage* CUIPage::AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask)
		{
            return m_Context->AddImage(classname, filename, rect, size, id, text, buse_sprite_text, buse_mask);
		}
		//-------------------------------------------------------------------------------------------------------
		/**
         *	@brief 。
         *
         *	@param 。
         *	@param 。
         *	@return 。
         */
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CMessageBox* CUIPage::AddMessageBox(int w, int h, DWORD id)
		{
			m_lpKernel->GetlpEngine()->GetlpGUISystem()->ComputeLayoutFactor(LYNX_FALSE);
			GUISystem::CWindowCreationDesc WindowDesc;
			WindowDesc.Default();
			WindowDesc.m_lpStream = m_lpKernel->GetlpEngine()->GetlpFileSystem()->GetPackage()->GetStream();
			WindowDesc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			WindowDesc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			WindowDesc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			WindowDesc.m_SpriteDesc.m_lpStream = m_lpKernel->GetlpEngine()->GetlpFileSystem()->GetPackage()->GetStream();
			WindowDesc.m_SpriteDesc.m_FileName = _T("../texture/ui/message_box.png");
			WindowDesc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			WindowDesc.m_SpriteDesc.m_Number = 0;
			WindowDesc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			WindowDesc.m_SpriteDesc.m_MaterialName = WindowDesc.m_SpriteDesc.m_FileName + CString(_T(".Material"));
			WindowDesc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
			WindowDesc.m_SpriteDesc.m_Width = w * m_lpKernel->GetlpEngine()->GetlpGUISystem()->GetScaleFactor().x;
			WindowDesc.m_SpriteDesc.m_Height = h * m_lpKernel->GetlpEngine()->GetlpGUISystem()->GetScaleFactor().y;
			WindowDesc.m_SpriteDesc.m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER;
			WindowDesc.m_SpriteDesc.m_OriginType = CSpriteContainer::OT_CENTER;
			WindowDesc.m_SpriteDesc.m_AlignmentRect.x1 = 0;
			WindowDesc.m_SpriteDesc.m_AlignmentRect.x2 = m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetBackbufferWidth();
			WindowDesc.m_SpriteDesc.m_AlignmentRect.y1 = 0;
			WindowDesc.m_SpriteDesc.m_AlignmentRect.y2 = m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetBackbufferHeight();
            
			GUISystem::CMessageBox* MessageBox = LYNXNEW GUISystem::CMessageBox(m_lpKernel->GetlpEngine()->GetlpGUISystem());
			AddControl(MessageBox);
			MessageBox->vCreate(&WindowDesc);
			MessageBox->vInit();
			MessageBox->Modeless(LYNX_TRUE);
			MessageBox->SetID(id);
			MessageBox->Activate(LYNX_FALSE);
			MessageBox->Show(LYNX_FALSE);
            
			return MessageBox;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CControl* CUIPage::GetHitControl(int x, int y)
		{
			return m_Context->GetHitControl(x, y);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CControl* CUIPage::GetHitControl(InputSystem::CTouch::TOUCHGESTURE gesture, int x, int y)
		{
			return m_Context->GetHitControl(gesture, x, y);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		int CUIPage::GetHitControlID(int x, int y)
		{
			LYNXPOINT2D Pos;
            
			Pos.x = x;
			Pos.y = y;
			for (CList<GUISystem::CControl*>::CIterator Control = m_Context->GetControlList().begin(); Control != m_Context->GetControlList().end(); ++Control)
			{
				if ((*Control)->vHitTest(&Pos) && !(*Control)->GetContext() && (*Control)->GetControlType() != GUISystem::CT_DIALOG && (*Control)->GetControlType() != GUISystem::CT_WINDOW)
				{
					(*Control)->vOnHit();
					SetActiveControl((*Control));
					return (*Control)->GetID();
				}
			}
			return -1;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::RenderToOffscreen()
		{
            if (m_bRenderToOffscreenForTransition)
            {
                LynxEngine::GraphicsSystem::CBuffer* LastRenderBuffers[3] = {NULL, NULL, NULL};
                
				m_bTransitionRendering = LYNX_TRUE;
                m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition()->ResetCurrent();
                m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->SetMapAsRenderBuffer(LastRenderBuffers, 0, m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpUITransition());
                vRender();
				#ifdef __iOS__
					m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->DiscardRenderBuffer(0);
					m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->DiscardDepthBuffer();
				#endif
                //m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpOffscreen()->DownSampled();
                m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->SetRenderBuffer(0, LastRenderBuffers[0]);
                m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->SetViewportRect();
				m_bTransitionRendering = LYNX_FALSE;
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUIPage::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassMethod(lps);
            
			REGISTER_SCRIPT_METHOD(lps, "SetQuit",				LynxEngine::GameFramework::CUIPage,	void,					SetQuit,			(LYNXBOOL))
			REGISTER_SCRIPT_METHOD(lps, "QuitTo",				LynxEngine::GameFramework::CUIPage,	void,					QuitTo,				(const LYNXCHAR*))
			//REGISTER_SCRIPT_METHOD(lps, "AddButton",			LynxEngine::GameFramework::CUIPage,	GUISystem::CButton*,	AddButton,			(const LYNXANSICHAR*, const LYNXCHAR*, const LYNXRECT&, const Math::CVector2&, DWORD, const LYNXCHAR*, const LYNXBOOL, const LYNXBOOL))
			//REGISTER_SCRIPT_METHOD(lps, "AddStateButton",		LynxEngine::GameFramework::CUIPage,	GUISystem::CButton*,	AddStateButton,		(const LYNXANSICHAR*, const LYNXCHAR*, const LYNXCHAR*, const LYNXRECT&, const Math::CVector2&, DWORD, const LYNXCHAR*, LYNXBOOL))
			REGISTER_SCRIPT_METHOD(lps, "AddBackground",		LynxEngine::GameFramework::CUIPage,	CSpriteContainer*,		AddBackground,		(const LYNXCHAR*, const LYNXCOLORRGBA&))
			REGISTER_SCRIPT_METHOD(lps, "GetNumControls",		LynxEngine::GameFramework::CUIPage,	int,					GetNumControls,		(void))
			REGISTER_SCRIPT_METHOD(lps, "FindControl",			LynxEngine::GameFramework::CUIPage,	GUISystem::CButton*,	FindControl,		(const LYNXCHAR*))
			REGISTER_SCRIPT_METHOD(lps, "FindControlByID",		LynxEngine::GameFramework::CUIPage,	GUISystem::CButton*,	FindControlByID,	(const DWORD))
			REGISTER_SCRIPT_METHOD(lps, "GetControlByIndex",	LynxEngine::GameFramework::CUIPage,	GUISystem::CButton*,	GetControlByIndex,	(const int))
			REGISTER_SCRIPT_METHOD(lps, "SetActiveControl",		LynxEngine::GameFramework::CUIPage,	void,					SetActiveControl,	(void*))
			REGISTER_SCRIPT_METHOD(lps, "GetActiveControl",		LynxEngine::GameFramework::CUIPage,	void*,					GetActiveControl,	(void))
			REGISTER_SCRIPT_METHOD(lps, "RenderToOffscreen",	LynxEngine::GameFramework::CUIPage,	void,					RenderToOffscreen,	(void))
			REGISTER_SCRIPT_METHOD(lps, "RenderControls",		LynxEngine::GameFramework::CUIPage,	void,					RenderControls,		(void))
			REGISTER_SCRIPT_METHOD(lps, "GetHitControl",		LynxEngine::GameFramework::CUIPage,	void*,					_GetHitControl,		(int, int))
			REGISTER_SCRIPT_METHOD(lps, "GetHitControlID",		LynxEngine::GameFramework::CUIPage,	int,					GetHitControlID,	(int, int))
			REGISTER_SCRIPT_METHOD(lps, "DoesAcceptInput",		LynxEngine::GameFramework::CUIPage,	LYNXBOOL,				DoesAcceptInput,	(void))
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		int CUIPage::GetNumControls()
		{
			return m_Context->GetControlList().size();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CControl* CUIPage::GetControlByIndex(const int i)
		{
			return (m_Context->GetControlList()[i]);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::AddControl(GUISystem::CControl* c)
		{
			m_Context->AddControl(c); 			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CControl* CUIPage::FindControl(const LYNXCHAR* name)
		{
			return (m_Context->FindControl(name));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CControl* CUIPage::FindControlByID(const DWORD id)
		{
			return (m_Context->FindControl(id));			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::SetActiveControl(GUISystem::CControl* c)
		{
			m_Context->SetActiveControl(c);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUIPage::DoesAcceptInput()
		{
			if (m_Context->GetActiveControl())
			{
				return m_Context->GetActiveControl()->DoesAcceptInput();					
			}
			else
			{
				return (!(m_lpKernel->IsUITransiting()));
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::GroupButtonPressed(GUISystem::CControl* b)
		{
            return m_Context->GroupButtonPressed(b);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		GUISystem::CScrollGroup* CUIPage::CreateScrollGroup(const CString& name, const LYNXRECT& rect)
		{
            return m_Context->CreateScrollGroup(name, rect);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::vLoadLayout(const CString& filename)
		{
			ReleaseLayoutData();

			#ifdef __DESKTOP__
				m_UIFile = LYNXNEW CUserProfile(filename);
			#else
				m_UIFile = LYNXNEW  CReadOnlyProfile(filename);
			#endif

			m_UIFile->Load();	
							
			CString Value, Name, Filename;
			LYNXRECT ClipRect;
			CVector2 Size;

			CXMLElement* DefinesNode = m_UIFile->GetRootElement()->FirstChildElement("Defines");	
			if (DefinesNode)
			{
				CXMLElement* DefineNode = DefinesNode->FirstChildElement();	
				while (DefineNode)
				{
					m_Defines[DefineNode->Name()] = lynxAtoi(m_UIFile->GetValue(DefineNode).c_str());
					DefineNode = DefineNode->NextSiblingElement();
				}
			}			
			
			CXMLElement* Window = m_UIFile->GetRootElement()->FirstChildElement("Window");	
			if (Window)
			{
				CXMLElement* Groups = Window->FirstChildElement("Groups");	
				if (Groups)
				{
					CXMLElement* Group = Groups->FirstChildElement("Group");	
					while  (Group)
					{
						m_Context->LoadGroup(m_UIFile, Group);
						Group = Group->NextSiblingElement("Group");													
					}
				}

				CXMLElement* AnimationSet = Window->FirstChildElement("AnimationSet");
				if (AnimationSet)
				{
					m_Context->LoadAnimation(m_UIFile, AnimationSet);
				}

				CXMLElement* Controls = Window->FirstChildElement("Controls");	
				if (Controls)
				{
					CXMLElement* Control = Controls->FirstChildElement("Control");	
					while  (Control)
					{
						m_Context->LoadControl(m_UIFile, Control);
						Control = Control->NextSiblingElement("Control");													
					}
				}
			}	
			m_Context->vInit();	
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CUIPage::ReleaseLayoutData()
		{
			if (m_UIFile)
			{
				LYNXDEL m_UIFile;
				m_UIFile = NULL;
			}
			m_Defines.clear();
		}
	}	
}
