#include <LynxEngine_PCH.h>

#include <GUISystem/LynxText.h>
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

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CText::CText(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_TEXT;		
			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;
            m_bAcceptInput = LYNX_TRUE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CText::~CText()
		{
			if (m_Context)
			{
				LYNXDEL m_Context;
				m_Context = NULL;
			}									

			if (m_HitMask)
			{
				LYNXDEL m_HitMask;
				m_HitMask = NULL;
			}

			if (m_SpriteTextString)
			{
				LYNXDEL m_SpriteTextString;
				m_SpriteTextString = NULL;
				m_lpContainer = NULL;
			}			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CText::vCreate(CCreationDesc* desc)
		{	
			CTextCreationDesc* TextDesc = (CTextCreationDesc*)desc;

			SetFont(TextDesc->m_Font);
			SetFontColor(TextDesc->m_FontColor);
			SetFontScale(TextDesc->m_FontScale);
			SetFontShadowColor(TextDesc->m_FontShadowColor);
			UseFontShadow(TextDesc->m_bFontShadow);

			SetFontOriginType(TextDesc->m_OriginType);
            SetFontAlignmentType(TextDesc->m_AlignmentType);
            SetFontAlignmentRect(TextDesc->m_AlignmentRect);			
			
			SetFontOutlineColor(TextDesc->m_FontOutlineColor);
			SetFontOutlineWidth(TextDesc->m_FontOutlineWidth);
			UseFontOutline(TextDesc->m_bFontOutline);

			m_Text = TextDesc->m_Text;
            
            if (TextDesc->m_bUseSpriteTextString)
            {
                m_SpriteTextString = LYNXNEW CSpriteTextString(m_lpEngine);
				m_lpContainer = static_cast<CContainer*>(m_SpriteTextString);				
                
                //m_SpriteTextString->Create((TextDesc->m_AlignmentRect.x2 - TextDesc->m_AlignmentRect.x1), (TextDesc->m_AlignmentRect.y2 - TextDesc->m_AlignmentRect.y1), (TextDesc->m_AlignmentRect.x2 - TextDesc->m_AlignmentRect.x1), (TextDesc->m_AlignmentRect.y2 - TextDesc->m_AlignmentRect.y1));
                
                m_SpriteTextString->Create(-1, -1, (TextDesc->m_AlignmentRect.x2 - TextDesc->m_AlignmentRect.x1), (TextDesc->m_AlignmentRect.y2 - TextDesc->m_AlignmentRect.y1), TextDesc->m_AlignmentType);
                
				m_SpriteTextString->SetAlignmentType(TextDesc->m_AlignmentType);	
				m_SpriteTextString->SetOriginType(TextDesc->m_OriginType);				
				m_SpriteTextString->SetAlignmentRect(&TextDesc->m_AlignmentRect);
				m_TargetAlignmentRect = TextDesc->m_AlignmentRect;

				//if (m_lpEngine->GetlpKernel()->GetDeviceType() != DT_PC)
				{
					if (TextDesc->m_Font)
						m_SpriteTextString->SetFont(TextDesc->m_Font);
					else
						m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());

					m_SpriteTextString->m_bShadow = m_bFontShadow;
					m_SpriteTextString->m_bOutline = m_bFontOutline;
					m_SpriteTextString->m_OutlineColor = m_FontOutlineColor;
					m_SpriteTextString->m_OutlineWidth = m_FontOutlineWidth;
					m_SpriteTextString->Printf(&m_FontColor, m_Text.c_str());
				}
            }
			
            return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CText::vInit()
		{	
			CControl::vInit();
			
			return LYNX_TRUE;
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::SetScale(const float s)
		{
			m_FontScale = s;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vSetPosition(const Math::CVector3& pos) 
		{
			if (m_SpriteTextString)
			{
				m_SpriteTextString->vSetPosition(pos); 
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vGetPosition(Math::CVector3& pos) 
		{
			if (m_SpriteTextString)
			{
				m_SpriteTextString->vGetPosition(pos); 
			}			
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vLoop(float step)
		{
			CControl::vLoop(step);						

			if (m_lpContainer)
			{
				TransitionLoop();

				AnimationLoop(step);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::AnimationLoop(float step)
		{
			if (LYNX_FALSE == m_bAnimationLoop)
				return;

			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			if (!lpAni)
				return;

			if (m_lpContainer->vPlay(step)) // bStop
			{
				if (PM_LOOP == lpAni->m_PlayMode || PM_BOUNCE == lpAni->m_PlayMode)	{
					InvokeAnimationEvent(lpAni->GetName(), LYNX_FALSE);
				}
				else {
					StopAnimation(LYNX_FALSE);
				}
			}

			static Math::CReal Values[4];
			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE])
			{
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->Lerp(Values, lpAni->m_Time);
				SetFontScale(Values[0]);
			}

			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION])
			{
				const Math::CVector2& ScaleFactor = m_lpEngine->GetlpGUISystem()->GetScaleFactor();
				const Math::CMatrix4& M = m_lpContainer->GetTransformMatrix();
				LYNXRECT RenderRect = m_TargetAlignmentRect;
				RenderRect.x1 += M.m41 * ScaleFactor.x;
				RenderRect.x2 += M.m41 * ScaleFactor.x;
				RenderRect.y1 += M.m42 * ScaleFactor.y;
				RenderRect.y2 += M.m42 * ScaleFactor.y;
				vSetAlignmentRect(RenderRect);
			}
			
			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR])
			{
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Values, lpAni->m_Time);

				LYNXCOLORRGBA FontColor;
				FontColor.Red = Values[0] * 255.0f;
				FontColor.Green = Values[1] * 255.0f;
				FontColor.Blue = Values[2] * 255.0f;
				FontColor.Alpha = Values[3] * 255.0f;
				SetFontColor(FontColor);
				
				m_lpContainer->m_Diffuse.Red = Values[0] * 255.0f;
				m_lpContainer->m_Diffuse.Green = Values[1] * 255.0f;
				m_lpContainer->m_Diffuse.Blue = Values[2] * 255.0f;
				m_lpContainer->m_Diffuse.Alpha = Values[3] * 255.0f;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vRender()
		{	
			if (m_bShow)
			{
				LYNXRECT Rect;
                if (m_SpriteTextString)
                {					
                    m_SpriteTextString->SetScale(m_FontScale);                    
                    m_SpriteTextString->SetAlignmentType(m_FontAlignmentType);
                    m_SpriteTextString->SetOriginType(m_FontOriginType);
					m_Rect = m_FontAlignmentRect;
					//m_Rect.x1 += m_TransitionOffset.x;
					//m_Rect.x2 += m_TransitionOffset.x;
					//m_Rect.y1 += m_TransitionOffset.y;
					//m_Rect.y2 += m_TransitionOffset.y;                    
                    //m_SpriteTextString->SetAlignmentRect(&Rect);
                    
                    //m_SpriteTextString->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                    //m_SpriteTextString->SetOriginType(CSpriteContainer::OT_CENTER);
                    //m_SpriteTextString->SetAlignmentRect(&m_FontAlignmentRect);
                    
                    //m_SpriteTextString->GetlpCurrentMaterial()->m_Diffuse = m_FontColor;
                    m_SpriteTextString->vRender();
                }
                else
                {
					if (m_Font && m_Font->GetFontType() == CFont::FT_TEXTURE_FONT)
					{
						m_lpEngine->GetSystemFontString()->SetFont(m_Font);
					}
                    else
						m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					

                    CVector2 OldScale = m_lpEngine->GetSystemFontString()->SetScale(m_FontScale);
                    m_lpEngine->GetSystemFontString()->SetAlignmentType(m_FontAlignmentType);
                    m_lpEngine->GetSystemFontString()->SetOriginType(m_FontOriginType);
					Rect = m_FontAlignmentRect;
					Rect.x1 += m_TransitionOffset.x;
					Rect.x2 += m_TransitionOffset.x;
					Rect.y1 += m_TransitionOffset.y;
					Rect.y2 += m_TransitionOffset.y;                    
                    m_lpEngine->GetSystemFontString()->SetAlignmentRect(&Rect);
                    m_Rect = m_FontAlignmentRect;
                    m_Rect.y1 -= m_lpEngine->GetSystemFontString()->GetCurrentFontHeight();                    
					LYNXBOOL bOldAutoReturn;
					if (m_FontAlignmentType == 0)
					{
						bOldAutoReturn = m_lpEngine->GetSystemFontString()->AutoReturn(LYNX_FALSE);
						m_lpEngine->GetSystemFontString()->Printf(m_Rect.x1, m_Rect.y1, &m_FontColor, m_Text.c_str());           
					}
					else
					{
						bOldAutoReturn = m_lpEngine->GetSystemFontString()->AutoReturn(LYNX_TRUE);
						m_lpEngine->GetSystemFontString()->Printf(&m_FontColor, m_Text.c_str());           
					}

                    m_lpEngine->GetSystemFontString()->SetScale(OldScale);
                    m_lpEngine->GetSystemFontString()->AutoReturn(bOldAutoReturn);
                    
                    /*
                    m_lpEngine->GetSystemTTFontString()->SetAlignmentType(m_FontAlignmentType);
                    m_lpEngine->GetSystemTTFontString()->SetOriginType(m_FontOriginType);
                    m_lpEngine->GetSystemTTFontString()->SetAlignmentRect(&m_FontAlignmentRect);
                    m_lpEngine->GetSystemTTFontString()->AutoReturn(LYNX_TRUE);
                    m_lpEngine->GetSystemTTFontString()->Printf(&m_FontColor, m_Text.c_str());
                    */
                }
			}
			//CControl::vRender();
		}        		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vDebugRender()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CText::vOnHit()
		{
			if (m_HitSound)
				m_HitSound->AddToPlayingList();
            
            if (m_lpClassScript)
			{
				ScriptBindObject();			
				m_lpClassScript->vOnHit();
			}
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CText::vHitTest(const LYNXPOINT2D* p)
		{
			return CControl::vHitTest(p);

			/*
			if (m_bActive)
			{
				if (p->x >= m_Rect.x1 && p->x <= m_Rect.x2 &&
					p->y >= m_Rect.y1 && p->y <= m_Rect.y2)
				{
					return LYNX_TRUE;
				}
			}
			return LYNX_FALSE;
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CText::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			CSpriteContainer::ORIGINTYPE Old = m_FontOriginType;
			SetFontOriginType(ot);
			return Old;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD  CText::vSetAlignmentType(const DWORD type)
		{
			DWORD Old = m_FontAlignmentType;
			SetFontAlignmentType(type);

			return Old;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CText::vSetAlignmentRect(const LYNXRECT& r)
		{
			if (m_SpriteTextString)
			{
				m_SpriteTextString->SetAlignmentRect(&r);
				SetFontAlignmentRect(r);
			}
			else
			{
				SetFontAlignmentRect(r);
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CText::vGetAlignmentRect(LYNXRECT& r)
		{
			LYNXRECT Rect;

			if (m_SpriteTextString)
			{
				m_SpriteTextString->GetAlignmentRect(&Rect);
				m_FontAlignmentRect = Rect;
				GetFontAlignmentRect(r);
			}
			else
			{
				GetFontAlignmentRect(r);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::SetText(const CString& text) 	
		{
			m_Text = text;
			if (m_SpriteTextString)
			{
				m_SpriteTextString->m_bShadow = m_bFontShadow;				
				m_SpriteTextString->m_bOutline = m_bFontOutline;
				m_SpriteTextString->m_OutlineColor = m_FontOutlineColor;
				m_SpriteTextString->m_OutlineWidth = m_FontOutlineWidth;				
				m_SpriteTextString->Printf(&m_FontColor, m_Text.c_str());
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::SetText(const CString& text, const float& x, const float& y)
		{
			m_Text = text;
			if (m_SpriteTextString)
			{
				m_SpriteTextString->m_bShadow = m_bFontShadow;
				m_SpriteTextString->m_bOutline = m_bFontOutline;
				m_SpriteTextString->m_OutlineColor = m_FontOutlineColor;
				m_SpriteTextString->m_OutlineWidth = m_FontOutlineWidth;				
				m_SpriteTextString->Printf(x, y, &m_FontColor, m_Text.c_str());
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::ApplyAnimation(const CString& anim, const EVENTDELEGATE& event_end)
		{
			if (m_ParentContext && m_lpContainer)
			{
				Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetAnimation(anim);
				if (!lpAni) {
					lpAni = m_ParentContext->m_AnimationSet->GetAnimation(anim);
				}
				if (lpAni) {
					m_lpContainer->m_AnimationSet->vInstance(*(m_ParentContext->m_AnimationSet));
				}

				if (lpAni)
				{
					m_lpContainer->m_AnimationSet->SetCurrentAnimation(anim);
					m_lpContainer->vPlay(0);
				}

				if (lpAni && event_end)
				{
					CMap<CString, EVENTDELEGATE>::iterator found = m_AnimationEvent.find(anim);
					if (found != m_AnimationEvent.end())
					{
						found->second = event_end;
					}
					else
						m_AnimationEvent[anim] = event_end;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::PlayAnimation(LYNXBOOL bReset)
		{
			if (!m_lpContainer)
				return;

			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			if (!lpAni)
				return;

			if (bReset)
			{
				SeekAnimation(0);
			}

			m_bAnimationLoop = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::StopAnimation(LYNXBOOL bReset)
		{
			if (!m_lpContainer)
				return;

			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			if (!lpAni)
				return;

			if (!bReset)
			{
				LYNXREAL Start, End;
				lpAni->GetRange(&Start, &End);

				SeekAnimation(End);
			}
			else
				SeekAnimation(0);

			m_bAnimationLoop = LYNX_FALSE;
			InvokeAnimationEvent(lpAni->GetName(), LYNX_TRUE);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::SeekAnimation(float seek_time)
		{
			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			if (!lpAni)
				return;

			lpAni->ResetTime();
			m_lpContainer->vPlay(seek_time);

			static Math::CReal Values[4];
			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE])
			{
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->Lerp(Values, lpAni->m_Time);
				SetFontScale(Values[0]);
			}

			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR])
			{
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Values, lpAni->m_Time);

				LYNXCOLORRGBA FontColor;
				FontColor.Red = Values[0] * 255.0f;
				FontColor.Green = Values[1] * 255.0f;
				FontColor.Blue = Values[2] * 255.0f;
				FontColor.Alpha = Values[3] * 255.0f;
				SetFontColor(FontColor);

				MaterialSystem::CMaterialPtr& MaterialPtr = m_lpContainer->GetlpCurrentMaterial();
				m_lpContainer->m_Diffuse.Red = Values[0] * 255.0f;
				m_lpContainer->m_Diffuse.Green = Values[1] * 255.0f;
				m_lpContainer->m_Diffuse.Blue = Values[2] * 255.0f;
				m_lpContainer->m_Diffuse.Alpha = Values[3] * 255.0f;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CText::IsAnimationStop()
		{
			if (!m_lpContainer)
				return LYNX_FALSE;

			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			return (lpAni) ? lpAni->m_bPlayStop : LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CText::InvokeAnimationEvent(const CString& anim, LYNXBOOL remove)		
		{
			CMap<CString, EVENTDELEGATE>::iterator found = m_AnimationEvent.find(anim);
			if (found != m_AnimationEvent.end())
			{
				EVENTDELEGATE InvokeEvent = found->second;
				if (InvokeEvent)
					InvokeEvent(anim, this);

				if (remove)
					m_AnimationEvent.erase(found);
			}
		}
	}
}
