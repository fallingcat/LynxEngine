#include <LynxEngine_PCH.h>

#include <GUISystem/LynxStateButton.h>
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

namespace LynxEngine
{
	namespace GUISystem
	{
		IMPLEMENT_CLASSSCRIPT(CStateButton, CButton)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CStateButton::CStateButton(CGUISystem* lpsys)
		: CButton(lpsys)
		{
			m_ControlType = CT_STATEBUTTON;		
			m_bPressed = LYNX_FALSE;			
			m_PressedFontColor.PackedColor = 0xE4E4E4FF; // {228, 228, 228, 255}
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CStateButton::~CStateButton()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CStateButton::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);
			
			REGISTER_SCRIPT_METHOD(lps, "Pressed",		LynxEngine::GUISystem::CStateButton, void, Pressed, (LYNXBOOL))					
			REGISTER_SCRIPT_METHOD(lps, "IsPressed",	LynxEngine::GUISystem::CStateButton, LYNXBOOL, IsPressed, (void))					

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateButton::vCreate(CCreationDesc* desc)
		{	
			CStateButtonCreationDesc* lpButtonDesc = (CStateButtonCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpButtonDesc->m_SpriteDesc);	

			SetName(desc->m_Name);
            SetFont(lpButtonDesc->m_Font);
			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name + CString(_T(".Container")));
			m_lpContainer->vCreate(lpSpriteDesc);
			m_Size.x = ((CSpriteContainer*)m_lpContainer)->GetWidth();
			m_Size.y = ((CSpriteContainer*)m_lpContainer)->GetHeight();

			m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->Create(2);
			m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->LoadTexture(PRESSED, *(lpButtonDesc->m_lpStream), lpButtonDesc->m_PressedStateFileName.c_str(), &lpButtonDesc->m_SpriteDesc.m_TextureDesc);					

			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
			((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
			((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&m_TargetAlignmentRect);
            
            //if (lpButtonDesc->m_bUseSpriteTextString && lpButtonDesc->m_bShowText && m_lpEngine->GetlpKernel()->GetDeviceType() != DT_PC)
			if (lpButtonDesc->m_bUseSpriteTextString && lpButtonDesc->m_bShowText)
            {
                m_SpriteTextString = LYNXNEW CSpriteTextString(m_lpEngine);
                m_SpriteTextString->Create(-1, -1, m_Size.x, m_Size.y);
                //m_SpriteTextString->Create(64, 32, 64, 32);
                if (lpButtonDesc->m_Font)
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());
                else
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());
                m_SpriteTextString->Printf(NULL, desc->m_Name.c_str());
                //m_SpriteTextString->SetOriginalSize(128, 64);
            }
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateButton::vInit()
		{	
			CButton::vInit();

			m_bPressed = LYNX_FALSE;			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CStateButton::vLoop(float step)
		{
			CControl::vLoop(step);						

			if (m_bShow && m_lpContainer)
			{
				if (m_bActive)
				{
					TransitionLoop();
					AnimationLoop(step);
				}				
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CStateButton::vRender()
		{	
			if (m_bHintScale)
			{
				HintScaleLoop(1.0f);
			}
			else
			{
				if (m_bShow && m_lpContainer)
				{
					if ((LYNX_FALSE == m_bActive) || (LYNX_FALSE == m_bHitTest))
					{
						if (m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
							m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
						else
						{
							if (m_bPressed)
								m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = PRESSED;
							else
								m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
						}
					}
					else
					{
						if (m_bPressed)
							m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = PRESSED;
						else
							m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
					}
				}

				if (!m_bAcceptInput)
				{
					if (m_bScale)
					{
						Math::CReal ScaleStep;
						if (m_TargetScale > m_Scale)
						{
							ScaleStep = (m_TargetScale*1.05 - m_Scale) / (m_NumSteps / m_lpEngine->GetGUIStep()) * (60.0f / m_lpEngine->m_UpdateRate);
							m_Scale += ScaleStep;
							if (m_Scale >= m_TargetScale)
							{
								if (m_Count == 0)
								{
									m_TargetScale = 0.7f;
									//m_TargetScale = 1.3f;
								}
								else if (m_Count == 1)
								{
									m_TargetScale = 1.0f;
								}
								else
								{
									m_Scale = m_TargetScale;
									m_bAcceptInput = LYNX_TRUE;
								}
								m_Count++;
							}
						}
						else
						{
							ScaleStep = (m_TargetScale*0.95 - m_Scale) / (m_NumSteps / m_lpEngine->GetGUIStep()) * (60.0f / m_lpEngine->m_UpdateRate);
							m_Scale += ScaleStep;
							if (m_Scale <= m_TargetScale)
							{
								if (m_Count == 0)
								{
									//m_TargetScale = 0.7f;
									m_TargetScale = 1.3f;
								}
								else if (m_Count == 1)
								{
									m_TargetScale = 1.0f;
								}
								else
								{
									m_Scale = m_TargetScale;
									m_bAcceptInput = LYNX_TRUE;
								}
								m_Count++;
							}
						}
					}
					else
					{
						m_bAcceptInput = LYNX_TRUE;
					}

					if (m_bShow && m_lpContainer)// && m_bUpdateSize)
					{
						Math::CVector2 Size = m_Size * m_Scale;
						((CSpriteContainer*)m_lpContainer)->vSetSize(Size.x, Size.y);
					}
				}
			}

			if (m_bShow && m_lpContainer)
			{		
				m_lpContainer->vRender();
				if (!m_bUseCustomHitRect)
					m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;

                if (m_Name.size() && m_bShowText)
                {
                    if (m_SpriteTextString)
                    {
                        float OldScale = m_SpriteTextString->GetScale();
                        float NewScale = m_FontScale * m_Scale;
                        m_SpriteTextString->SetScale(NewScale);
                        
                        if (m_bCustomFontAlignmentRect)
                        {
                            m_SpriteTextString->SetAlignmentType(m_FontAlignmentType);
                            m_SpriteTextString->SetOriginType(m_FontOriginType);
                            m_SpriteTextString->SetAlignmentRect(&m_TargetFontAlignmentRect);
                        }
                        else
                        {
                            m_SpriteTextString->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                            m_SpriteTextString->SetOriginType(CSpriteContainer::OT_CENTER);
                            m_SpriteTextString->SetAlignmentRect(&m_Rect);
                        }
                        LYNXCOLORRGBA Color = m_FontColor;
                        if (!m_bActive)
                            Color.Alpha = 100;
						else if (!m_bHitTest)
							Color.Alpha = 100;
						else if (m_bPressed)
							Color = m_PressedFontColor;

                        m_SpriteTextString->GetlpCurrentMaterial()->m_Diffuse = Color;
                        m_SpriteTextString->vRender();
                        m_SpriteTextString->SetScale(OldScale);
                    }
                    else
                    {
						if (m_Font && m_Font->GetFontType() == CFont::FT_TEXTURE_FONT)
						{
							m_lpEngine->GetSystemFontString()->SetFont(m_Font);
						}
                        else
                            m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					                        

                        CVector2 OldScale = m_lpEngine->GetSystemFontString()->GetScale();
                        CVector2 NewScale = CVector2(m_FontScale * m_Scale, m_FontScale * m_Scale);
                        m_lpEngine->GetSystemFontString()->SetScale(NewScale);
                        
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
                        LYNXCOLORRGBA Color = m_FontColor;
                        if (!m_bActive)
                            Color.Alpha = 100;
						else if (!m_bHitTest)
							Color.Alpha = 100;
						else if (m_bPressed)
							Color = m_PressedFontColor;

                        m_lpEngine->GetSystemFontString()->Printf(&Color, m_Name.c_str());
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
		void CStateButton::vOnHit()
		{
			if (m_GroupID == -1 || (m_GroupID != -1 && !m_bPressed))
			{					
				SetHintScale(LYNX_FALSE);

				m_bPressed = !m_bPressed;
				if (m_bPressed)
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = 1;
				else
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = 0;

				if (m_HitSound)
					m_HitSound->AddToPlayingList();
            
				if (m_lpClassScript)
				{
					ScriptBindObject();			
					m_lpClassScript->vOnHit();
				}
				else
				{
					SetTargetScale(0.7f, 4.0f);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CStateButton::Pressed(LYNXBOOL b)
		{
			m_bPressed = b;

			if (m_bPressed)
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = PRESSED;
			else
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;			
		}
	}
}
