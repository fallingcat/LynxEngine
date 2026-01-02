#include <LynxEngine_PCH.h>

#include <GUISystem/LynxButton.h>
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

#define SAFE_RELEASE(x) if(x) { LYNXDEL(x); x = NULL;}

		IMPLEMENT_CLASSSCRIPT(CButton, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CButton::CButton(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_BUTTON;
			m_lpContainer = NULL;
			m_Scale = 1.0f;
			SetTargetScale(1.0f, 4.0f);
            m_bUseCustomHitRect = LYNX_FALSE;
			m_bScale = LYNX_TRUE;

            m_bPauseTransition = LYNX_FALSE;
			m_OriginalTransitionOffset = m_TransitionOffset = CVector2(0.0f, 0.0f);			

			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;          
			m_bAcceptInput = LYNX_TRUE;
			m_bAnimationLoop = LYNX_FALSE;
			m_AnimationEvent.clear();			
			m_TransitionState = TS_STOP;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CButton::~CButton()
		{	
			/*
			for (CMap<CString, IEventDelegate*>::iterator iter = m_AnimationEvent.begin(); iter != m_AnimationEvent.end(); ++iter)
			{
				SAFE_RELEASE(iter->second);				
			}	
			*/			
			m_AnimationEvent.clear();			
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CButton::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "SetTargetScale",	LynxEngine::GUISystem::CButton, void, _SetTargetScale, (const float, const float))		

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::vCreateFromLayout(CUserProfile* file, CXMLElement* node, CContext* context)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			LYNXBOOL bActive;
			CString Class, Name, Lookup, Value, Group, File, HitSound, ID, StepInSound;
			CVector2 Size, Offset;
			
			Class = file->Read(_T("Class"), Class , node);						
			ID = file->Read(_T("ID"), ID , node);						
			Lookup = file->Read(_T("LookupID"), Lookup , node);						
			if (Lookup.empty())
			{
				Name = file->Read(_T("Name"), Name , node);						
			}
			else
			{
				Name = glpKernel->GetlpGame()->GetText(_T("LT_SYSTEM_MSG"), Lookup, _T("desc"));
			}			
			Group = file->Read(_T("Group"), Group , node);						
			File = file->Read(_T("File"), File , node);						
			Size = file->Read(_T("Size"), Size , node);						
			ClipRect = file->Read(_T("Rect"), ClipRect , node);						
			if (ClipRect.x2 == -1)
				ClipRect.x2 = ClipRect.x1 + Size.x;
			if (ClipRect.y2 == -1)
				ClipRect.y2 = ClipRect.y1 + Size.y;
			Offset = file->Read(_T("Offset"), Offset , node);						
			HitSound = file->Read(_T("Sound"), HitSound, node);	
			StepInSound = file->Read(_T("StepInSound"), StepInSound, node);
			bActive = file->Read(_T("Active"), LYNX_TRUE, node);
						
			int ControlID = context->GetDefinedControlID(ID);
			GUISystem::CButton* Button = context->AddButton(Class, File, ClipRect, Size, ControlID, Name, LYNX_TRUE);
			Button->Activate(bActive);
			Button->SetStepInSound(StepInSound);
			Button->SetHitSound(glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(HitSound));
			GUISystem::CScrollGroup* ScrollGroup = context->FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(Button);
			Button->SetTransitionOffset(Offset);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CButton::vCreate(CCreationDesc* desc)
		{	
			CButtonCreationDesc* lpButtonDesc = (CButtonCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpButtonDesc->m_SpriteDesc);	

			SetName(desc->m_Name);
            SetFont(lpButtonDesc->m_Font);
			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name + CString(_T(".Container")));
			m_lpContainer->vCreate(lpSpriteDesc);
			m_Size.x = ((CSpriteContainer*)m_lpContainer)->GetWidth();
			m_Size.y = ((CSpriteContainer*)m_lpContainer)->GetHeight();

			if (lpButtonDesc->m_bLoadHitMask)
			{
				m_HitMask = LYNXNEW CHitMask(m_lpEngine);
				m_HitMask->CreateFromFile(lpSpriteDesc->m_FileName);
			}

			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
			((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
			((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&m_TargetAlignmentRect);
            
            //if (lpButtonDesc->m_bUseSpriteTextString && lpButtonDesc->m_bShowText && m_lpEngine->GetlpKernel()->GetDeviceType() != DT_PC)
			if (lpButtonDesc->m_bUseSpriteTextString && lpButtonDesc->m_bShowText)
            {
                m_SpriteTextString = LYNXNEW CSpriteTextString(m_lpEngine);
                CVector2 Size = lpButtonDesc->m_TextSize;
                if (Size.x < 0)
                    Size.x = m_Size.x;
                if (Size.y < 0)
                    Size.y = m_Size.y;
				m_SpriteTextString->Create(-1, -1, Size.x*1.1f + 0.5f, Size.y*0.8f + 0.5f, lpButtonDesc->m_TextAlignment);
                if (lpButtonDesc->m_Font)
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());
                else
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());

				UseFontShadow(lpButtonDesc->m_bFontShadow);
				SetFontShadowColor(lpButtonDesc->m_FontShadowColor);
				UseFontOutline(lpButtonDesc->m_bFontOutline);
				SetFontOutlineColor(lpButtonDesc->m_FontOutlineColor);
				SetFontOutlineWidth(lpButtonDesc->m_FontOutlineWidth);

				m_SpriteTextString->m_bShadow = m_bFontShadow;
				m_SpriteTextString->m_bOutline = m_bFontOutline;
				m_SpriteTextString->m_OutlineColor = m_FontOutlineColor;
				m_SpriteTextString->m_OutlineWidth = m_FontOutlineWidth;
                m_SpriteTextString->Printf(NULL, desc->m_Name.c_str());
            }
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::SetInactiveTexture(CStream& fs, const CString& filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc)
		{
			m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->Create(3);
			m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->LoadTexture(INACTIVE, fs, filename.c_str(), desc);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::UpdateText(const CString& t)
		{
			SetName(t);

			if (m_SpriteTextString)
			{
				m_SpriteTextString->m_bShadow = m_bFontShadow;
				m_SpriteTextString->m_bOutline = m_bFontOutline;
				m_SpriteTextString->m_OutlineColor = m_FontOutlineColor;
				m_SpriteTextString->m_OutlineWidth = m_FontOutlineWidth;
				m_SpriteTextString->Printf(NULL, t.c_str());
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CButton::SetTargetScale(const Math::CReal& target, const Math::CReal& numsteps)
		{
			m_TargetScale = target;
			m_NumSteps = numsteps;
			m_bAcceptInput = LYNX_FALSE;
			m_Count = 0;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------		
		void CButton::ApplyAnimation(const CString& anim, const EVENTDELEGATE& event_stop)
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

				if (lpAni && event_stop)
				{
					CMap<CString, EVENTDELEGATE>::iterator found = m_AnimationEvent.find(anim);
					if (found != m_AnimationEvent.end())
					{
						found->second = event_stop;
					}
					else
						m_AnimationEvent[anim] = event_stop;
				}
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CButton::PlayAnimation(LYNXBOOL bReset)
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
		void CButton::StopAnimation(LYNXBOOL bReset)
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
		void CButton::SeekAnimation(float seek_time)
		{						
			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();
			if (!lpAni)
				return;

			lpAni->ResetTime();
			m_lpContainer->vPlay(seek_time);

			if (lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR])
			{
				static CReal Values[4];
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Values, lpAni->m_Time);

				// MaterialSystem::CMaterialPtr& MaterialPtr = m_lpContainer->GetlpCurrentMaterial();
				// MaterialPtr->m_Diffuse.Red = Values[0] * 255.0f;
				// MaterialPtr->m_Diffuse.Green = Values[1] * 255.0f;
				// MaterialPtr->m_Diffuse.Blue = Values[2] * 255.0f;
				// MaterialPtr->m_Diffuse.Alpha = Values[3] * 255.0f;
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
		LYNXBOOL CButton::IsAnimationStop()
		{
			Animation::CAnimation* lpAni = m_lpContainer->m_AnimationSet->GetCurrentAnimation();			
			return (lpAni) ? lpAni->m_bPlayStop : LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CButton::InvokeAnimationEvent(const CString& anim, LYNXBOOL remove)
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
		/*
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::SetTransitionOffset(const Math::CVector2& offset)
		{
			m_OriginalTransitionOffset = m_TransitionOffset = offset;
			m_TotalOffset = CVector2(0.0f, 0.0f);
			LYNXRECT Rect = m_TargetAlignmentRect;
			Rect.x1 += m_TransitionOffset.x;
			Rect.x2 += m_TransitionOffset.x;
			Rect.y1 += m_TransitionOffset.y;
			Rect.y2 += m_TransitionOffset.y;
			if (m_lpContainer)
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);
            
            if (m_bCustomFontAlignmentRect)
            {
                Rect = m_FontAlignmentRect;
                Rect.x1 += m_TransitionOffset.x;
                Rect.x2 += m_TransitionOffset.x;
                Rect.y1 += m_TransitionOffset.y;
                Rect.y2 += m_TransitionOffset.y;
                m_TargetFontAlignmentRect = Rect;
            }
			m_TransitionState = TS_STEP_IN;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::TransitBack()
		{
			CVector2 Target;

			Target.x = m_TargetAlignmentRect.x1 + m_TransitionOffset.x;
			Target.y = m_TargetAlignmentRect.y1 + m_TransitionOffset.y;

			MoveTo(Target);
			m_TransitionState = TS_STEP_OUT;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::MoveTo(const Math::CVector2& pos)
		{
			CVector2 CPos, Offset;

			CPos.x = m_TargetAlignmentRect.x1;
			CPos.y = m_TargetAlignmentRect.y1;

			Offset = CPos - pos;

			m_TargetAlignmentRect.x1 -= Offset.x;
			m_TargetAlignmentRect.x2 -= Offset.x;
			m_TargetAlignmentRect.y1 -= Offset.y;
			m_TargetAlignmentRect.y2 -= Offset.y;
			
			SetTransitionOffset(Offset);

			m_TransitionState = TS_MOVING;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::TransitionLoop()
		{
			if (!m_bPauseTransition && (m_TransitionOffset.x != 0.0f || m_TransitionOffset.y != 0.0f) && m_TotalOffset.x != -FLT_MAX)
			{
				LYNXRECT Rect;
				((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&Rect);
				CVector2 Dir;
				Dir.x = m_TargetAlignmentRect.x1 - Rect.x1;
				Dir.y = m_TargetAlignmentRect.y1 - Rect.y1;
                float Len = Dir.Length();

				if (Len < 1.5f ||
					(m_TransitionState == TS_STEP_OUT && (Rect.x2 < 0 || Rect.y2 < 0 || Rect.x1 > m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2 || Rect.y1 > m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y2)))
				{
					Rect = m_TargetAlignmentRect;
					m_TotalOffset = CVector2(-FLT_MAX, -FLT_MAX);
					if (m_TransitionState == TS_STEP_OUT)
					{
						m_TargetAlignmentRect.x1 = Rect.x1 + m_TransitionOffset.x;
						m_TargetAlignmentRect.x2 = Rect.x2 + m_TransitionOffset.x;
						m_TargetAlignmentRect.y1 = Rect.y1 + m_TransitionOffset.y;
						m_TargetAlignmentRect.y2 = Rect.y2 + m_TransitionOffset.y;

						m_TransitionOffset *= -1.0f;

						Show(LYNX_FALSE);
						Activate(LYNX_FALSE);
					}
					m_TransitionState = TS_DONE;
				}
				else
				{
					Dir /= m_TransitionSteps;
					if (m_TransitionState == TS_STEP_OUT)
						Dir *= 0.5f;
					m_TotalOffset += Dir;
                    Rect.x1 = m_TargetAlignmentRect.x1 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.x2 = m_TargetAlignmentRect.x2 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.y1 = m_TargetAlignmentRect.y1 + m_TransitionOffset.y + m_TotalOffset.y;
					Rect.y2 = m_TargetAlignmentRect.y2 + m_TransitionOffset.y + m_TotalOffset.y;
				}
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);
                
                if (m_bCustomFontAlignmentRect)
                {
                    if (Len < 1.5f)
                    {
                        Rect = m_FontAlignmentRect;
                    }
                    else
                    {
                        Rect.x1 = m_FontAlignmentRect.x1 + m_TransitionOffset.x + m_TotalOffset.x;
                        Rect.x2 = m_FontAlignmentRect.x2 + m_TransitionOffset.x + m_TotalOffset.x;
                        Rect.y1 = m_FontAlignmentRect.y1 + m_TransitionOffset.y + m_TotalOffset.y;
                        Rect.y2 = m_FontAlignmentRect.y2 + m_TransitionOffset.y + m_TotalOffset.y;
                    }
                    m_TargetFontAlignmentRect = Rect;
                }
			}
		}
		*/
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CButton::vInit()
		{	
			CObj::vInit();

			m_Scale = 1.0f;
			SetTargetScale(1.0f, 4.0f);
			m_Count = 2;
			m_bAcceptInput = LYNX_TRUE;
		
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::vLoop(float step)
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
		void CButton::AnimationLoop(float step)
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
				static CReal Values[4];
				lpAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Values, lpAni->m_Time);
								
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
		void CButton::vSetupTexture()
		{
			if (m_bShow && m_lpContainer)
			{
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
				if (!m_bActive && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::vRender()
		{	
			vSetupTexture();		

			if (m_bHintScale)
			{
				HintScaleLoop(1.0f);
			}
			else
			{
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
					if (m_bShow && m_lpContainer)
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
							m_SpriteTextString->SetTextAlignment(m_FontAlignmentType);
                        }
                        else
                        {
                            m_SpriteTextString->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                            m_SpriteTextString->SetOriginType(CSpriteContainer::OT_CENTER);
                            m_SpriteTextString->SetAlignmentRect(&m_Rect);
                        }
                        LYNXCOLORRGBA Color = m_FontColor;
						if ((LYNX_FALSE == m_bActive) || (LYNX_FALSE == m_bHitTest))
						{
							if (m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetTexture(INACTIVE))
							{
								m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
							}
							else
							{
								m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
								Color.Alpha = 100;
							}
						}

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
                        CVector2 NewScale(m_FontScale * m_Scale, m_FontScale * m_Scale);
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
		void CButton::vOnHit()
		{
			SetHintScale(LYNX_FALSE);

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
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::vSetSize(float w, float h)
		{
			((CSpriteContainer*)m_lpContainer)->vSetSize(w, h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CButton::vGetSize(float& w, float& h)
		{
			((CSpriteContainer*)m_lpContainer)->vGetSize((Math::CReal&)w, (Math::CReal&)h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CButton::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			return ((CSpriteContainer*)m_lpContainer)->SetOriginType(ot);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD  CButton::vSetAlignmentType(const DWORD type)
		{
			return ((CSpriteContainer*)m_lpContainer)->SetAlignmentType(type);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CButton::vSetAlignmentRect(const LYNXRECT& r)
		{
			((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&r);
			//m_TargetAlignmentRect = r;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CButton::vGetAlignmentRect(LYNXRECT& r)
		{
			return ((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&r);
		}		
	}
}
