#include <LynxEngine_PCH.h>

#include <GUISystem/LynxSlider.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>	
#include <LynxZipStream.h>	
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxEngine.h>
#include <LynxResource.h>
#include <Container/LynxSpriteContainer.h>
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

namespace LynxEngine
{
	namespace GUISystem
	{
		IMPLEMENT_CLASSSCRIPT(CSlider, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSlider::CSlider(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_SLIDER;
			m_lpContainer = NULL;
            m_lpBarContainer = NULL;
            
            m_CurrentPos = 0;
            m_Min = 0;
            m_Max = 100;
			
			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;
			m_bAcceptInput = LYNX_TRUE;
			m_bShowCustomText = LYNX_FALSE;

			m_InputGestureFilter = InputSystem::CTouch::TGF_ALL;
            
            ShowText(LYNX_FALSE);
            
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSlider::~CSlider()
		{
			if (m_lpContainer)
			{
				m_lpEngine->DestroyContainer(m_lpContainer);
				m_lpContainer = NULL;
			}
            if (m_lpBarContainer)
			{
				m_lpEngine->DestroyContainer(m_lpBarContainer);
				m_lpBarContainer = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSlider::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);
            
			//REGISTER_SCRIPT_METHOD(lpbs, "SetTargetScale",	LynxEngine::GUISystem::CSlider, void, _SetTargetScale, (const float, const float))		
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSlider::vCreate(CCreationDesc* desc)
		{	
			CSliderCreationDesc* lpSliderDesc = (CSliderCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpSliderDesc->m_SpriteDesc);
            CSpriteCreationDesc* lpBarSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpSliderDesc->m_BarSpriteDesc);
            
			m_AlignmentType = lpSliderDesc->m_AlignmentType;

			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name);
			m_lpContainer->vCreate(lpSpriteDesc);
			m_lpContainer->vSetSize(lpSpriteDesc->m_Width, lpSpriteDesc->m_Height);

			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);				
			((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
            
            m_lpBarContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpBarContainer->SetName(desc->m_Name + CString(".Bar"));	
			m_lpBarContainer->vCreate(lpBarSpriteDesc);
            ((CSpriteContainer*)m_lpBarContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
            ((CSpriteContainer*)m_lpBarContainer)->SetOriginType(CSpriteContainer::OT_CENTER);                                      
			
            m_Min = lpSliderDesc->m_RangeMin;
            m_Max = lpSliderDesc->m_RangeMax;
            m_Rect = lpSliderDesc->m_Rect;                                      
			if (m_AlignmentType & CSpriteContainer::H_ALIGN_CENTER)
			{
				m_Step = float(m_Rect.x2 - m_Rect.x1) / float(m_Max - m_Min);
			}
			else if (m_AlignmentType & CSpriteContainer::H_ALIGN_LEFT)
			{
				m_Rect.x2 -=  lpSpriteDesc->m_Width;
				m_Step = (float(m_Rect.x2 - m_Rect.x1) - lpSpriteDesc->m_Width) / float(m_Max - m_Min);
			}
                                      
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSlider::vInit()
		{	
			CObj::vInit();
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSlider::vRender()
		{	
			if (m_bShow && m_lpContainer)
			{
				if (m_bActive)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
					m_lpBarContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
				}
				else
				{
					if (m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
						m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
					if (m_lpBarContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
						m_lpBarContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}
			}
            
			//if (!m_bAcceptInput)
			{
				if (m_bShow && m_lpContainer)
				{
					LYNXRECT Rect;
					Math::CReal Width, Height;
					m_lpContainer->vGetSize(Width, Height);
					if (m_AlignmentType & CSpriteContainer::H_ALIGN_CENTER)
					{
						Rect.x1 = m_Rect.x1 + (m_CurrentPos * m_Step) - (Width * 0.5f);
						Rect.x2 = Rect.x1 + Width;
						Rect.y1 = m_Rect.y1;
						Rect.y2 = m_Rect.y2;
					}
					else if (m_AlignmentType & CSpriteContainer::H_ALIGN_LEFT)
					{
						Rect.x1 = m_Rect.x1 + Width + (m_CurrentPos * m_Step);// - (Width * 0.5f);
						Rect.x2 = Rect.x1 + Width;
						Rect.y1 = m_Rect.y1;
						Rect.y2 = m_Rect.y2;
					}					
					((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);					
				}
			}
			if (m_bShow)
			{
				if (m_lpBarContainer)		
				{
					m_lpBarContainer->vRender();
					m_RenderRect = ((CSpriteContainer*)m_lpBarContainer)->m_RenderRect;
				}

				if (m_lpContainer)								
				{
					m_lpContainer->vRender();				

					if (m_Font && m_Font->GetFontType() == CFont::FT_TEXTURE_FONT)
					{
						m_lpEngine->GetSystemFontString()->SetFont(m_Font);
					}
                    else
                        m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					

					CVector2 OldScale = m_lpEngine->GetSystemFontString()->GetScale();
					CVector2 NewScale(m_FontScale, m_FontScale);
					m_lpEngine->GetSystemFontString()->SetScale(NewScale);

                    if (m_bShowText)
                    {
						LYNXRECT Rect;
						static_cast<CSpriteContainer*>(m_lpContainer)->GetAlignmentRect(&Rect);
						m_lpEngine->GetSystemFontString()->SetAlignmentRect(&Rect);
						if (!m_bShowCustomText)
						{
							m_lpEngine->GetSystemFontString()->Printf(&m_FontColor, _T("%d/%d"), m_CurrentPos+1, (m_Max - m_Min + 1));
						}
						else
						{
							m_lpEngine->GetSystemFontString()->Printf(&m_FontColor, _T("%s"), m_CustomText.c_str());
						}
						m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemFont());
						m_lpEngine->GetSystemFontString()->SetScale(OldScale);		
                    }
				}
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSlider::vSetPosition(const Math::CVector3& pos) 
		{
			m_lpContainer->vSetPosition(pos); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSlider::vGetPosition(Math::CVector3& pos) 
		{
			m_lpContainer->vGetPosition(pos); 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSlider::vSetSize(float w, float h)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSlider::vHitTest(const LYNXPOINT2D* p)
		{
			LYNXPOINT2D P;
			LYNXRECT ViewportRect = GetlpEngine()->GetlpGraphicsSystem()->GetViewportRect();				
			P.x = p->x - ViewportRect.x1;
			P.y = p->y - ViewportRect.y1;

			if (m_bActive && m_bAcceptInput)
			{
				if (P.x >= m_RenderRect.x1 && P.x <= m_RenderRect.x2 &&
					P.y >= m_RenderRect.y1 && P.y <= m_RenderRect.y2)
				{
					Math::CReal Width, Height;
					m_lpContainer->vGetSize(Width, Height);
					if (m_AlignmentType & CSpriteContainer::H_ALIGN_CENTER)
					{
						m_CurrentPos = LYNX_MIN(((m_Min + LYNX_MAX(P.x - m_RenderRect.x1 - Width/2, 0)) / m_Step), m_Max);
					}
					else if (m_AlignmentType & CSpriteContainer::H_ALIGN_LEFT)
					{
						m_CurrentPos = LYNX_MIN((m_Min + (P.x - m_RenderRect.x1 - Width) / m_Step), m_Max);
					}					
					return LYNX_TRUE;
				}
			}
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CSlider::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			return ((CSpriteContainer*)m_lpBarContainer)->SetOriginType(ot);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD CSlider::vSetAlignmentType(const DWORD type)
		{
			return ((CSpriteContainer*)m_lpBarContainer)->SetAlignmentType(type);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CSlider::vSetAlignmentRect(const LYNXRECT& r)
		{
			((CSpriteContainer*)m_lpBarContainer)->SetAlignmentRect(&r);
			m_Rect = r;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CSlider::vGetAlignmentRect(LYNXRECT& r)
		{
			//return ((CSpriteContainer*)m_lpBarContainer)->GetAlignmentRect(&r);
			r = m_Rect;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
        void CSlider::SetPos(int pos)
        {
			m_CurrentPos = pos;
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
        void CSlider::SetRange(int min, int max)
        {
			m_Min = min;
			m_Max = max;

			if (m_AlignmentType & CSpriteContainer::H_ALIGN_CENTER)
			{
				m_Step = float(m_Rect.x2 - m_Rect.x1) / float(m_Max - m_Min);
			}
			else if (m_AlignmentType & CSpriteContainer::H_ALIGN_LEFT)
			{
				Math::CReal Width, Height;
				m_lpContainer->vGetSize(Width, Height);
				m_Step = (float(m_Rect.x2 - m_Rect.x1) - Width) / float(m_Max - m_Min);
			}        
		}
    }
}
