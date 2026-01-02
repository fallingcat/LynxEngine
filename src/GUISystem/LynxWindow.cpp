#include <LynxEngine_PCH.h>

#include <GUISystem/LynxWindow.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>	
#include <LynxZipStream.h>	
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxEngine.h>
#include <LynxResource.h>
#include <Font/LynxTextureFont.h>
#include <Font/LynxTextureFontString.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxBuffer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <LynxScreenOutputDevice.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#include <ScriptSystem/LynxVMachine.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Shader/LynxGaussinBlurPS.h>
#include <GUISystem/LynxButton.h>
#include <GUISystem/LynxSlider.h>
#include <GUISystem/LynxText.h>
#include <GUISystem/LynxImage.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		IMPLEMENT_CLASSSCRIPT(CWindow, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CWindow::CWindow(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_WINDOW;			
			m_lpContainer = NULL;
			
			m_Scale = 1.0f;

			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;        
			m_bShowText = LYNX_FALSE;

			m_bUseBackground = LYNX_TRUE;
			m_BackgroundType = BT_FILE;
			m_bNoInput = LYNX_TRUE;
			m_UIFile = NULL;
			m_bCancelCanBack = LYNX_TRUE;
			m_bOnBackInput = LYNX_FALSE;
			m_bOnBack = LYNX_FALSE;
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CWindow::~CWindow()
		{				
			ReleaseLayoutData();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CWindow::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			//REGISTER_SCRIPT_METHOD(lps, "SetTargetScale",	LynxEngine::CWindow, void, _SetTargetScale, (const float, const float))		

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CWindow::vCreate(CCreationDesc* desc)
		{	
			CWindowCreationDesc* lpWindowDesc = (CWindowCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpWindowDesc->m_SpriteDesc);				

			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);

			lpSpriteDesc->m_Width *= ScaleFactor.x;
			lpSpriteDesc->m_Height *= ScaleFactor.y;
			float Len = lpSpriteDesc->m_AlignmentRect.x2 - lpSpriteDesc->m_AlignmentRect.x1;
			lpSpriteDesc->m_AlignmentRect.x1 *= PositionFactor.x;
			lpSpriteDesc->m_AlignmentRect.x2 = (lpSpriteDesc->m_AlignmentRect.x1 + Len*ScaleFactor.x);
			Len = lpSpriteDesc->m_AlignmentRect.y2 - lpSpriteDesc->m_AlignmentRect.y1;
			lpSpriteDesc->m_AlignmentRect.y1 *= PositionFactor.y;
			lpSpriteDesc->m_AlignmentRect.y2 = (lpSpriteDesc->m_AlignmentRect.y1 + Len*ScaleFactor.y);

			SetName(desc->m_Name);
			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name + CString(_T(".Container")));	
			if (!m_bUseBackground || lpSpriteDesc->m_FileName.empty())
			{
				lpSpriteDesc->m_FileName = _T("white.png");
			}
			m_lpContainer->vCreate(lpSpriteDesc);
			m_Size.x = ((CSpriteContainer*)m_lpContainer)->GetWidth();
			m_Size.y = ((CSpriteContainer*)m_lpContainer)->GetHeight();		

			((CSpriteContainer*)m_lpContainer)->Align(m_Size.x, m_Size.y);
			m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;
			((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&m_TargetAlignmentRect);

			m_Context = LYNXNEW CContext(m_lpEngine, NULL, this, m_Rect);
			m_Context->ApplyLayoutFactor(lpWindowDesc->m_bApplyLayoutFactory);
			//((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
			//((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
			//((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&m_AlignmentRect);			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CWindow::vInit()
		{	
			CObj::vInit();
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CWindow::GroupButtonPressed(CControl* b)
		{
			return m_Context->GroupButtonPressed(b);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CWindow::FindControl(const CString& name)
		{
			return m_Context->FindControl(name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CWindow::FindControl(const DWORD id)
		{
			return m_Context->FindControl(id);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::ReleaseControl(const CString& name)
		{
			CControl* Con = FindControl(name);
			if (Con)
			{
				LYNXDEL Con;
				m_Context->GetControlList().remove(Con);			
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		/*
		void CWindow::ReleaseControl(const DWORD id)
		{
			CControl* Con = FindControl(id);
			if (Con)
			{
				LYNXDEL Con;
				m_Context->GetControlList().remove(Con);			
			}
		}
		*/
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CWindow::GetHitControl(int x, int y)
		{
			return m_Context->GetHitControl(x, y);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CWindow::GetInputFocus()
        {
            m_lpEngine->GetlpKernel()->SetCurrentFocusedContext(m_Context);
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CWindow::Activate(LYNXBOOL b)
        {
            CGameObj::Activate(b);
            
            if (b)
            {
                GetInputFocus();
            }
            else
            {
                m_lpEngine->GetlpKernel()->LoseInputFocus(m_Context);
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::GetInput()
		{
			InputSystem::CMouse* lpMouse = (InputSystem::CMouse*)(m_lpEngine->GetlpSystemMouse());
			if (lpMouse)
			{
				lpMouse->vPoll();		

				CControl* lpControl = NULL;				
				if (lpMouse->ButtonStatus(InputSystem::CMouse::LBUTTON))
				{		
					GetHitControl(lpMouse->GetPos().x, lpMouse->GetPos().y);                    						
				}                							
				//lpMouse->ClearPollData();
			}

			InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();
			if (lpTouch)
			{
                lpTouch->vPoll();
                
				switch (lpTouch->GetTouchGesture())
				{
					case InputSystem::CTouch::TG_TAP:
						GetHitControl(lpTouch->GetTouch(0).Pos.x, lpTouch->GetTouch(0).Pos.y);						
						break;                         
				}
                
                if (!m_lpEngine->GetlpKernel()->GetGestureState(GameFramework::CKernel::TAP_GESTURE))
                {
                    if (lpTouch->GetNumTouches())
                    {
                        GetHitControl(lpTouch->GetTouch(0).Pos.x, lpTouch->GetTouch(0).Pos.y);
                    }
                }
			}
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vLoop(float step)
		{
			m_Context->SortControls();
			m_Context->SortGroups();

			if (!m_Context->GetActiveControl() && m_bActive)
                m_Context->GetInput();

			m_Context->ControlLoop(step);       

			if (m_lpContainer)
			{
				TransitionLoop();
			}

			m_Context->UpdateControlRects();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vRender()
		{	
			if (m_bShow && m_lpContainer)
			{
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
				
				if (!m_bActive && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}				

				// Set size everytime will hurt the performance.....
				//Math::CVector2 Size = m_Size * m_Scale;
				//((CSpriteContainer*)m_lpContainer)->vSetSize(Size.x, Size.y);

				if (m_bUseBackground)
				{
					switch (m_BackgroundType)
					{
						default:
						case BT_FILE:
							m_lpContainer->vRender();        
							break;

						case BT_UI_OFFSCREEN:
							break;
					}
				}
				else
				{
					static_cast<CSpriteContainer*>(m_lpContainer)->ComputeRenderRect();
				}
				m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;

                if (m_Name.size() && m_bShowText)
                {
                    if (m_Font)
                        m_lpEngine->GetSystemFontString()->SetFont(m_Font);
                    else
                        m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					
					
                    if (m_bCustomFontAlignmentRect)
                    {
                        m_lpEngine->GetSystemFontString()->SetAlignmentType(m_FontAlignmentType);
                        m_lpEngine->GetSystemFontString()->SetOriginType(m_FontOriginType);
                        m_lpEngine->GetSystemFontString()->SetAlignmentRect(&m_TargetFontAlignmentRect);
                    }
                    else
                    {
                        m_lpEngine->GetSystemFontString()->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                        m_lpEngine->GetSystemFontString()->SetOriginType(CSpriteContainer::OT_CENTER);
                        m_lpEngine->GetSystemFontString()->SetAlignmentRect(&m_Rect);
                    }
                    m_lpEngine->GetSystemFontString()->Printf(&m_FontColor, m_Name.c_str());
                    m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemFont());					
                }
				m_Context->RenderControls();
			}
		}        		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vOnHit()
		{
            if (m_lpClassScript)
			{
				ScriptBindObject();			
				m_lpClassScript->vOnHit();
			}
			else
			{
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vSetSize(float w, float h)
		{
			((CSpriteContainer*)m_lpContainer)->vSetSize(w, h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vGetSize(float& w, float& h)
		{
			((CSpriteContainer*)m_lpContainer)->vGetSize((Math::CReal&)w, (Math::CReal&)h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vSetPosition(const Math::CVector3& pos) 
		{
			CSpriteContainer* Sprite = ((CSpriteContainer*)m_lpContainer);

			if (Sprite->GetAlignmentType() == (CSpriteContainer::H_ALIGN_LEFT|CSpriteContainer::V_ALIGN_TOP) &&
				Sprite->GetOriginType() == CSpriteContainer::OT_UPPER_LEFT)
			{
				LYNXRECT Rect;

				Rect.x1 = pos.x;
				Rect.y1 = pos.y;
				Rect.x2 = Rect.x1 + m_Size.x;
				Rect.y2 = Rect.y1 + m_Size.y;
				vSetAlignmentRect(Rect);
			}
			else
			{			
				m_lpContainer->vSetPosition(pos); 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vGetPosition(Math::CVector3& pos) 
		{
			CSpriteContainer* Sprite = ((CSpriteContainer*)m_lpContainer);

			if (Sprite->GetAlignmentType() == (CSpriteContainer::H_ALIGN_LEFT|CSpriteContainer::V_ALIGN_TOP) &&
				Sprite->GetOriginType() == CSpriteContainer::OT_UPPER_LEFT)
			{
				LYNXRECT Rect;

				vGetAlignmentRect(Rect);
				pos.x = Rect.x1;
				pos.y = Rect.y1;				
			}
			else
			{			
				m_lpContainer->vGetPosition(pos); 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CWindow::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			return ((CSpriteContainer*)m_lpContainer)->SetOriginType(ot);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD  CWindow::vSetAlignmentType(const DWORD type)
		{			
			return ((CSpriteContainer*)m_lpContainer)->SetAlignmentType(type);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CWindow::vSetAlignmentRect(const LYNXRECT& r)
		{			
			((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&r);						
			m_Context->SetRect(r);
			m_Rect = r;

			if (m_TransitionOffset.x == 0 && m_TransitionOffset.y == 0)
				m_TargetAlignmentRect = r;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CWindow::vGetAlignmentRect(LYNXRECT& r)
		{
			return ((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&r);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CScrollGroup* CWindow::CreateScrollGroup(const CString& name, const LYNXRECT& rect)
		{
			return m_Context->CreateScrollGroup(name, rect);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CButton* CWindow::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask, const int text_w, const int text_h, const int textalignment)
		{
            return m_Context->AddButton(classname, filename, rect, size, id, text, buse_sprite_text, buse_mask, text_w, text_h, textalignment);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CButton* CWindow::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask, const int text_w, const int text_h, const int textalignment)
		{
			return m_Context->AddButton(classname, filename, uvoffset, rect, size, id, text, buse_sprite_text, mat_name, buse_mask, text_w, text_h, textalignment);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateButton* CWindow::AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask)
		{
            return m_Context->AddStateButton(classname, filename, pressedfilename, rect, size, id, text, buse_sprite_text, mat_name, buse_mask);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSlider* CWindow::AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text)
		{
            return m_Context->AddSlider(classname, filename, bar_filename, rect, size, min, max, id, text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CText* CWindow::AddText(const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text, const int alignment)
		{
            return m_Context->AddText(classname, text, font, scale, color, shadow_color, bshadow, rect, id, buse_sprite_text, alignment);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CImage* CWindow::AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask, const int text_w, const int text_h, const LYNXBOOL bshow_text)
		{
            return m_Context->AddImage(classname, filename, rect, size, id, text, buse_sprite_text, buse_mask, text_w, text_h, bshow_text);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void CWindow::SendEvent(const int id)
		{
			m_Context->SendEvent(id);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vCreateFromLayout(CUserProfile* file, CXMLElement* w, CContext* context)
		{
			GUISystem::CWindowCreationDesc WindowDesc;
			CString Value, Name, Filename, Group, ID;
			LYNXRECT ClipRect;
			CVector2 Size(0.0f, 0.0f);
			LYNXBOOL bApplyLayoutFactor = LYNX_TRUE;

			CXMLElement* DefinesNode = w->FirstChildElement("Defines");	
			if (DefinesNode)
			{
				CXMLElement* DefineNode = DefinesNode->FirstChildElement();	
				while (DefineNode)
				{
					m_Defines[DefineNode->Name()] = lynxAtoi(m_UIFile->GetValue(DefineNode).c_str());
					DefineNode = DefineNode->NextSiblingElement();
				}
			}	

			ID = file->Read(_T("ID"), ID , w);						
			Name = m_UIFile->Read(_T("Name"), Name, w);						
			ClipRect = m_UIFile->Read(_T("Rect"), ClipRect, w);
			Size = m_UIFile->Read(_T("Size"), Size , w);						
			Filename = m_UIFile->Read(_T("File"), Filename, w);
			if (ClipRect.x2 == -1)
				ClipRect.x2 = ClipRect.x1 + Size.x;
			if (ClipRect.y2 == -1)
				ClipRect.y2 = ClipRect.y1 + Size.y;

			if (Size.x <= 0)
				Size.x = ClipRect.x2 - ClipRect.x1;
			if (Size.y <= 0)
				Size.y = ClipRect.y2 - ClipRect.y1;

			m_bShowText = m_UIFile->Read(_T("ShowText"), LYNX_FALSE, w);
			bApplyLayoutFactor = m_UIFile->Read(_T("ApplyLayoutFactor"), LYNX_TRUE, w);
			Group = m_UIFile->Read(_T("Group"), Group , w);	

			WindowDesc.Default();
			WindowDesc.m_Name = Name;
			WindowDesc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			WindowDesc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			WindowDesc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;			
			WindowDesc.m_SpriteDesc.m_lpStream = glpKernel->GetlpEngine()->GetlpFileSystem()->GetPackage()->GetStream();			
			WindowDesc.m_SpriteDesc.m_FileName = Filename; 			
			WindowDesc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			WindowDesc.m_SpriteDesc.m_Number = 0;
			WindowDesc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			WindowDesc.m_SpriteDesc.m_MaterialName = WindowDesc.m_SpriteDesc.m_FileName + CString(_T(".Material"));
			WindowDesc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
			WindowDesc.m_SpriteDesc.m_Width = Size.x;
			WindowDesc.m_SpriteDesc.m_Height = Size.y;	
			WindowDesc.m_SpriteDesc.m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER;
			WindowDesc.m_SpriteDesc.m_OriginType = CSpriteContainer::OT_CENTER;
			WindowDesc.m_SpriteDesc.m_AlignmentRect = ClipRect;
			WindowDesc.m_bApplyLayoutFactory = bApplyLayoutFactor;
				
			if (WindowDesc.m_SpriteDesc.m_FileName.empty())
				m_bUseBackground = LYNX_FALSE;
			else
				m_bUseBackground = LYNX_TRUE;

			vCreate(&WindowDesc);			
			SetName(Name);
			int ControlID = GetDefinedControlID(ID);
			SetID(ControlID);

			if (m_ParentContext)
			{
				GUISystem::CScrollGroup* ScrollGroup = m_ParentContext->FindScrollGroup(Group);
				if (ScrollGroup)
					ScrollGroup->AddControl(this);
			}

			CXMLElement* AnimationSet = w->FirstChildElement("AnimationSet");
			if (AnimationSet)
			{
				m_Context->LoadAnimation(m_UIFile, AnimationSet);
			}

			CXMLElement* Groups = w->FirstChildElement("Groups");	
			if (Groups)
			{
				CXMLElement* Group = Groups->FirstChildElement("Group");	
				while  (Group)
				{
					m_Context->LoadGroup(m_UIFile, Group);
					Group = Group->NextSiblingElement("Group");													
				}
			}

			CXMLElement* Controls = w->FirstChildElement("Controls");	
			if (Controls)
			{
				CXMLElement* Control = Controls->FirstChildElement("Control");	
				while  (Control)
				{
					m_Context->LoadControl(m_UIFile, Control);
					Control = Control->NextSiblingElement("Control");													
				}
			}

			vPostCreate();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CWindow::vLoadLayout(const CString& filename)
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
				vCreateFromLayout(m_UIFile, Window, m_Context);

				vInit();				
				SetID(0);			
				Modeless(LYNX_TRUE);	
				Activate(LYNX_TRUE);
				Show(LYNX_TRUE);				
			}			
			m_Context->vInit();	
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CWindow::ReleaseLayoutData()
		{
			if (m_UIFile)
			{
				LYNXDEL m_UIFile;
				m_UIFile = NULL;
			}
			m_Defines.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CWindow::AnyInput()
		{
			InputSystem::CMouse* lpMouse = (InputSystem::CMouse*)(m_lpEngine->GetlpSystemMouse());
			if (lpMouse)
			{
				if (m_lpEngine->GetlpInputSystem()->IsPlayingBack())
					m_lpEngine->GetlpSystemInputRecord()->Playback();
				else
					lpMouse->vPoll();				
                    
				if (lpMouse->ButtonStatus(InputSystem::CMouse::LBUTTON))
				{
					return LYNX_TRUE;
				}
			}
                
			InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();
			if (lpTouch)
			{
				if (m_lpEngine->GetlpInputSystem()->IsPlayingBack())
					m_lpEngine->GetlpSystemInputRecord()->Playback();
				else
					lpTouch->vPoll();                 
		
				if (lpTouch->GetNumTouches())
				{
					return LYNX_TRUE;
				}							
			}

			return LYNX_FALSE;
		}		
	}
}
