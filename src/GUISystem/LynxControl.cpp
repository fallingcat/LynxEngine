#include <LynxEngine_PCH.h>

#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		IMPLEMENT_CLASSSCRIPT(CControl, CGameObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl::CControl(CGUISystem* lpsys)
		: CGameObj(lpsys->GetlpEngine())
		{
			m_bInitActive = m_bActive;
			m_bShow = LYNX_TRUE;
			m_bInitShow = LYNX_TRUE;
			m_ParentContext = NULL;
			m_lpContainer = NULL;
			m_ParentControl = NULL;
			m_Context = NULL;            
			m_bModeless = LYNX_FALSE;
			m_bAcceptInput = LYNX_TRUE;            
			m_bShow = LYNX_TRUE;
			m_bShowText = LYNX_TRUE;
			m_GroupID = -1;			
			m_HitMask = NULL;
			m_bReadyToTransit = LYNX_TRUE;
			m_bUseCustomHitRect = LYNX_FALSE;
			m_bHitTest = LYNX_TRUE;
			m_bHintScale = LYNX_FALSE;
			m_HintScaleSpeed = 1.0f;
			m_HintScaleMin = 0.9f;
			m_HintScaleMax = 1.2f;
			m_HitTime = 0;
			m_HitAgainTime = 0.5f;

			m_TransitionVelocity = 0.0f;

			//m_Font = NULL;
			m_FontScale = 1.0f;
            m_FontColor.Red = 255;
            m_FontColor.Green = 255;
            m_FontColor.Blue = 255;
            m_FontColor.Alpha = 255;

			m_bFontShadow = LYNX_TRUE;
			m_FontShadowColor.Red = 0;
            m_FontShadowColor.Green = 0;
            m_FontShadowColor.Blue = 0;
            m_FontShadowColor.Alpha = 153;

			m_bFontOutline = LYNX_FALSE;
			m_FontOutlineColor.Red = 255;
			m_FontOutlineColor.Green = 0;
			m_FontOutlineColor.Blue = 0;
			m_FontOutlineColor.Alpha = 255;
			m_FontOutlineWidth = 3;

			m_FontStrokeColor.Red = 0;
            m_FontStrokeColor.Green = 0;
            m_FontStrokeColor.Blue = 0;
            m_FontStrokeColor.Alpha = 255;
			m_FontStrokeWidth = -1.0f;

            m_FontAlignmentType = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER;
            m_FontOriginType = CSpriteContainer::OT_CENTER;
            m_bCustomFontAlignmentRect = LYNX_FALSE;

			m_ScrollGroup = NULL;

			m_InputGestureFilter = InputSystem::CTouch::TGF_TAP;
			m_InputGesture = InputSystem::CTouch::TG_NULL;
            
            m_SpriteTextString = NULL;            

            ShowText(LYNX_TRUE);

			m_bPauseTransition = LYNX_FALSE;
			m_OriginalTransitionOffset = m_TransitionOffset = CVector2(0.0f, 0.0f);
			m_TransitionSteps = 3.0f;
			m_TransitionState = TS_STOP;

			m_RenderOrder = 0;
			m_bClipped = LYNX_FALSE;
			m_bNoInput = LYNX_FALSE;				

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl::~CControl()
		{
			if (m_Context)
			{
				LYNXDEL m_Context;
				m_Context = NULL;
			}						

			if (m_lpContainer)
			{
				m_lpEngine->DestroyContainer(m_lpContainer);
				m_lpContainer = NULL;
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
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CControl::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "SetAlignmentType",	LynxEngine::GUISystem::CControl, void,		SetAlignmentType,	(const DWORD))		
			REGISTER_SCRIPT_METHOD(lps, "SetPosition",		LynxEngine::GUISystem::CControl, void,		SetPosition,		(const Math::CVector3&))		
			REGISTER_SCRIPT_METHOD(lps, "HitTest",			LynxEngine::GUISystem::CControl, LYNXBOOL,	HitTest,			(const LYNXPOINT2D&))		
			REGISTER_SCRIPT_METHOD(lps, "OnHit",			LynxEngine::GUISystem::CControl, void,		OnHit,				(void))		
			REGISTER_SCRIPT_METHOD(lps, "SetTargetName",	LynxEngine::GUISystem::CControl, void,		SetTargetName,		(const LYNXCHAR*))		
			REGISTER_SCRIPT_METHOD(lps, "GetTargetName",	LynxEngine::GUISystem::CControl, LYNXCHAR*, _GetTargetName,		(void))		
			REGISTER_SCRIPT_METHOD(lps, "DoesAcceptInput",	LynxEngine::GUISystem::CControl, LYNXBOOL,	DoesAcceptInput,	(void))		

			return LYNX_TRUE;
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  :
        //-------------------------------------------------------------------------------------------------------
        void CControl::ReleaseBitmapContextData()
        {
            if (m_SpriteTextString && m_SpriteTextString->GetBitmapContext())
            {				
                m_SpriteTextString->GetBitmapContext()->vReleaseContextData();
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::Activate(LYNXBOOL b)
		{
			CGameObj::Activate(b);
			if (m_Context)
				m_Context->Activate(b);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::Show(LYNXBOOL b)
		{
			m_bShow = b;
			if (m_Context)
				m_Context->Show(b);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CControl::SetContainerAlignmentRect(const LYNXRECT& r)
		{			
			if (m_lpContainer)
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&r);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CControl::GetContainerAlignmentRect(LYNXRECT& r)
		{			
			if (m_lpContainer)
				((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&r);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CControl::GetContainerRenderRect(LYNXRECT& r)
		{			
			if (m_lpContainer)
				r = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetTargetAlignmentRect(LYNXRECT& r)
		{
			if (m_lpContainer)
			{
				m_TargetAlignmentRect = r;	
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::ComputeRenderRect()
		{
			if (GetlpContainer() && GetlpContainer()->GetContainerType() == CContainer::SPRITE)
			{
				static_cast<CSpriteContainer*>(GetlpContainer())->ComputeRenderRect();
				m_Rect = static_cast<CSpriteContainer*>(GetlpContainer())->m_RenderRect;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::Rerender()
		{
			LYNXRECT OldRect, NewRect;
			vGetAlignmentRect(OldRect);			
			GetContainerRenderRect(NewRect);
			vSetAlignmentRect(NewRect);			
			vRender();		
			vSetAlignmentRect(OldRect);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::_SetScrollGroup(CScrollGroup* c) 
		{			
			m_ScrollGroup = c; 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetScrollGroup(CScrollGroup* c) 
		{
			if (c && m_ScrollGroup)
			{
				m_ScrollGroup->RemoveControl(this);
			}
			
			m_ScrollGroup = c; 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::StopStepInAndOutSound()
		{
			if (m_StepInSound)
				m_StepInSound->vStop();
			if (m_StepOutSound)
				m_StepOutSound->vStop();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::ResetTransitionOffset() 
		{
			m_OriginalTargetAlignmentRect = m_TargetAlignmentRect; 
			SetTransitionOffsetWithoutFactor(m_OriginalTransitionOffset); 
			if (m_Context)
			{
				m_Context->ResetControlTransitionOffset();
			}
			StopStepInAndOutSound();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CControl::vHitTest(const LYNXPOINT2D* p)
		{
			LYNXRECT HitRect;	
			LYNXBOOL bHit = LYNX_FALSE;		

			if (glpKernel->ElapsedKernelTimeSince(m_HitTime) > m_HitAgainTime*1000)
			{
				if (m_bHitTest && m_bActive && !m_bClipped && m_bAcceptInput && (m_TransitionState == TS_STOP || m_TransitionState == TS_STEP_IN_DONE || m_TransitionState == TS_STEP_OUT_DONE))
				{
					/*
					if (m_Context)
					{
					CVector2 Origin(0.0f, 0.0f);

					HitRect = m_Context->GetRect();
					if (m_ParentContext)
					Origin = GUISystem::CContext::GetAbsoluteCoordOrigin(m_ParentContext);

					HitRect.x1 += Origin.x;
					HitRect.x2 += Origin.x;
					HitRect.y1 += Origin.y;
					HitRect.y2 += Origin.y;
					}
					else
					{
					HitRect = m_Rect;
					}
					*/
					if (m_bUseCustomHitRect)
					{
						int Width = (m_CustomHitRect.x2 - m_CustomHitRect.x1) * 2.0f;
						int Height = (m_CustomHitRect.y2 - m_CustomHitRect.y1) * 2.0f;
						CVector2 Center = CVector2((m_Rect.x2 + m_Rect.x1) * 0.5f, (m_Rect.y2 + m_Rect.y1) * 0.5f);
						HitRect.x1 = Center.x - Width * 0.5f;
						HitRect.x2 = HitRect.x1 + Width;
						HitRect.y1 = Center.y - Height * 0.5f;
						HitRect.y2 = HitRect.y1 + Height;
					}
					else
						HitRect = m_Rect;

					LYNXRECT ViewportRect = GetlpEngine()->GetlpGraphicsSystem()->GetViewportRect();
					HitRect.x1 += ViewportRect.x1;
					HitRect.x2 += ViewportRect.x1;
					HitRect.y1 += ViewportRect.y1;
					HitRect.y2 += ViewportRect.y1;

					CScrollGroup* ParentScrollGroup = m_ScrollGroup;
					CControl* ParentControl = m_ParentControl;

					if (!ParentScrollGroup)
					{
						while (ParentControl)
						{
							ParentScrollGroup = m_ParentControl->GetScrollGroup();
							if (ParentScrollGroup)
								break;
							ParentControl = ParentControl->m_ParentControl;
						}
					}

					if (ParentScrollGroup)
					{
						LYNXRECT ClipRect = ParentScrollGroup->GetRect();
						GUISystem::CContext* CurrentContext = ParentScrollGroup->GetContext();
						while (CurrentContext)
						{
							LYNXRECT ParentRrect = CurrentContext->GetRect();
							ClipRect.x1 += ParentRrect.x1;
							ClipRect.x2 += ParentRrect.x1;
							ClipRect.y1 += ParentRrect.y1;
							ClipRect.y2 += ParentRrect.y1;

							if (CurrentContext->GetParent() && CurrentContext->GetParent()->GetParentControl())
								CurrentContext = CurrentContext->GetParent()->GetParentControl()->GetContext();
							else
								CurrentContext = NULL;
						}

						ClipRect.x1 += ViewportRect.x1;
						ClipRect.x2 += ViewportRect.x1;
						ClipRect.y1 += ViewportRect.y1;
						ClipRect.y2 += ViewportRect.y1;

						if (p->x > ClipRect.x1 && p->x < ClipRect.x2 && p->y > ClipRect.y1 && p->y < ClipRect.y2)
						{
							if (p->x >= HitRect.x1 && p->x <= HitRect.x2 &&	p->y >= HitRect.y1 && p->y <= HitRect.y2)
							{
								if (m_HitMask)
								{
									bHit = m_HitMask->HitTest(p->x - HitRect.x1, p->y - HitRect.y1, HitRect.x2 - HitRect.x1, HitRect.y2 - HitRect.y1);
								}
								else
								{
									bHit = LYNX_TRUE;
								}
							}
						}
					}
					else
					{
						if (p->x >= HitRect.x1 && p->x <= HitRect.x2 && p->y >= HitRect.y1 && p->y <= HitRect.y2)
						{
							if (m_HitMask)
							{
								bHit = m_HitMask->HitTest(p->x - HitRect.x1, p->y - HitRect.y1, HitRect.x2 - HitRect.x1, HitRect.y2 - HitRect.y1);
							}
							else
							{
								bHit = LYNX_TRUE;
							}
						}
					}
				}

				if (bHit)
				{
					m_HitTime = glpKernel->GetKernelTime();
				}
			}

			return bHit;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetTransitionOffset(const Math::CVector2& offset)
		{
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);			
			CVector2 Offset = offset;		
			if (m_Context)
			{
				if (m_Context->DoesApplyLayoutFactor())
				{
					Offset.x *= ScaleFactor.x;
					Offset.y *= ScaleFactor.y;
				}
			}
			else if (m_ParentContext)
			{
				if (m_ParentContext->DoesApplyLayoutFactor())
				{
					Offset.x *= ScaleFactor.x;
					Offset.y *= ScaleFactor.y;
				}
			}			
			m_OriginalTransitionOffset = m_TransitionOffset = Offset;
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
			m_TransitionVelocity = 0.0f;
			if (offset.x == 0.0f && offset.y == 0.0f)
			{
				m_TransitionState = TS_STOP;
			}
			else
			{
				m_TransitionState = TS_STEP_IN;							
			}
			StopStepInAndOutSound();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetTransitionOffsetWithoutFactor(const Math::CVector2& offset)
		{
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);			
			CVector2 Offset = offset;		
						
			m_OriginalTransitionOffset = m_TransitionOffset = Offset;
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
			m_TransitionVelocity = 0.0f;
			if (offset.x == 0.0f && offset.y == 0.0f)
			{
				m_TransitionState = TS_STOP;
			}
			else
			{
				m_TransitionState = TS_STEP_IN;							
			}
			//StopStepInAndOutSound();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::Transit()
		{
			SetTransitionOffsetWithoutFactor(m_TransitionOffset);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::TransitStop()
		{
			m_TransitionState = TS_STOP;
			StopStepInAndOutSound();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::TransitDone()
		{
			m_TotalOffset = m_TransitionOffset * -1.0f;
			SetAlignmentRect(m_TargetAlignmentRect);
			((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&m_TargetAlignmentRect);
			ResetActiveState();
			m_TransitionState = TS_STEP_IN_DONE;	
			StopStepInAndOutSound();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::TransitBack()
		{
			CVector2 Target;

			m_OriginalTargetAlignmentRect = m_TargetAlignmentRect;

			Target.x = m_TargetAlignmentRect.x1 + m_TransitionOffset.x;
			Target.y = m_TargetAlignmentRect.y1 + m_TransitionOffset.y;
			
			MoveTo(Target);
			m_TransitionState = TS_STEP_OUT;			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::MoveTo(const Math::CVector2& pos)
		{
			CVector2 CPos, Offset;

			//if (m_TransitionState != TS_MOVING)
			{
				CPos.x = m_TargetAlignmentRect.x1;
				CPos.y = m_TargetAlignmentRect.y1;

				Offset = CPos - pos;

				m_TargetAlignmentRect.x1 -= Offset.x;
				m_TargetAlignmentRect.x2 -= Offset.x;
				m_TargetAlignmentRect.y1 -= Offset.y;
				m_TargetAlignmentRect.y2 -= Offset.y;
			
				SetTransitionOffsetWithoutFactor(Offset);

				m_TransitionVelocity = 0.0f;

				m_TransitionState = TS_MOVING;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::TransitionLoop()
		{
			if (m_lpContainer && m_bReadyToTransit && !m_bPauseTransition && (m_TransitionOffset.x != 0.0f || m_TransitionOffset.y != 0.0f) && (m_TransitionState == TS_STEP_OUT || m_TransitionState == TS_STEP_IN || m_TransitionState == TS_MOVING))
			{
				LYNXRECT Rect;
				((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&Rect);
				CVector2 Dir;
				Dir.x = m_TargetAlignmentRect.x1 - Rect.x1;
				Dir.y = m_TargetAlignmentRect.y1 - Rect.y1;
                float Len = Dir.Length();

				if (m_TransitionState == TS_STEP_OUT)
					PlayStepOutSound();
				else if (m_TransitionState == TS_STEP_IN)
					PlayStepInSound();

				if (Len <= 3.0f ||
					(m_TransitionState == TS_STEP_OUT && (Rect.x2 < 0 || Rect.y2 < 0 || Rect.x1 > m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2 || Rect.y1 > m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y2)))
				{	
					Rect = m_TargetAlignmentRect;
					if (m_TransitionState == TS_STEP_OUT)// || m_TransitionState == TS_STEP_OUT_DONE)
					{						
						m_TargetAlignmentRect = m_OriginalTargetAlignmentRect;
						//Rect = m_OriginalTargetAlignmentRect;
						SetTransitionOffsetWithoutFactor(m_TransitionOffset * -1.0f);
						Show(LYNX_FALSE);
						Activate(LYNX_FALSE);
						m_TransitionState = TS_STEP_OUT_DONE;
					}					
					else// if (m_TransitionState == TS_STEP_IN || m_TransitionState == TS_STEP_IN_DONE)
					{
						ResetActiveState();
						m_TransitionState = TS_STEP_IN_DONE;
					}
				}
				else //if (m_TransitionState == TS_STEP_OUT || m_TransitionState == TS_STEP_IN || m_TransitionState == TS_MOVING)
				{					
					/*Dir.Normalise();
					if (Len < 120.0f)
					{
						m_TransitionVelocity -= 32.0f;
						if (m_TransitionVelocity < 3.0f)
							m_TransitionVelocity = 3.0f;
					}
					else
					{
						m_TransitionVelocity += 32.0f;
						if (m_TransitionVelocity > 80.0f)
							m_TransitionVelocity = 80.0f;
					}
					Dir *= m_TransitionVelocity;
					*/

					Dir.Normalise();
					Len /= m_TransitionSteps;
					//Len = LYNX_MIN(Len, 150.0f);
					Dir *= Len;
					//if (m_TransitionState == TS_STEP_OUT)
						//Dir *= 0.1f;
					m_TotalOffset += Dir;
                    Rect.x1 = m_TargetAlignmentRect.x1 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.x2 = m_TargetAlignmentRect.x2 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.y1 = m_TargetAlignmentRect.y1 + m_TransitionOffset.y + m_TotalOffset.y;
					Rect.y2 = m_TargetAlignmentRect.y2 + m_TransitionOffset.y + m_TotalOffset.y;
				}
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);
				if (m_Context)
				{
					m_Context->SetRect(Rect);
				}
                
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
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetHitSound(const CString& name, const CString& filename)
        {
            if (m_lpEngine->GetlpSoundSystem()->LoadSound(m_HitSound, name, filename))
			{
				m_HitSound->SetLoops(0);
				m_HitSound->SetVolume(1.0f);
			}
        }		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetHitSound(const SoundSystem::CSoundPtr& s)
        {
			m_HitSound = s;
			if (m_HitSound)
			{
				m_HitSound->SetLoops(0);
				m_HitSound->SetVolume(1.0f);
			}
        }		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetHitSound(const CString& name)
        {
			m_HitSound = glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(name);			

			if (m_HitSound)
				m_HitSound->Stop();
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetStepInSound(const CString& sound)
		{
			if (sound.empty())
				m_StepInSound.Reset();
			else
				m_StepInSound = glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(sound);	

			if (m_StepInSound)
				m_StepInSound->Stop();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetStepOutSound(const CString& sound)
		{
			if (sound.empty())
				m_StepOutSound.Reset();
			else
				m_StepOutSound = glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(sound);

			if (m_StepOutSound)
				m_StepOutSound->Stop();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::PlayStepInSound()
		{
			if (m_StepInSound && !m_StepInSound->IsPlaying() && !m_StepInSound->IsPlayed())
			{
				m_StepInSound->AddToPlayingList();
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::PlayStepOutSound()
		{
			if (m_StepOutSound && !m_StepOutSound->IsPlaying() && !m_StepOutSound->IsPlayed())
			{
				m_StepOutSound->AddToPlayingList();
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::SetHintScale(LYNXBOOL b)
		{
			Math::CReal W, H;

			if (b == m_bHintScale)
				return;

			m_bHintScale = b;			

			if (m_bHintScale)
			{
				m_lpContainer->vGetSize(W, H);
				m_OriginalSize.x = W;
				m_OriginalSize.y = H;
			}
			else
			{
				m_lpContainer->vSetSize(m_OriginalSize.x, m_OriginalSize.y);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::HintScaleLoop(float step)
		{
			float Scale = m_HintScaleMin + lynxSin((glpKernel->GetFrameCount() % int(60 / m_HintScaleSpeed))*3.0f*m_HintScaleSpeed) * (m_HintScaleMax - m_HintScaleMin);
			CVector2 Size = m_OriginalSize;

			Size *= Scale;

			if (m_lpContainer)
			{
				m_lpContainer->vSetSize(Size.x, Size.y);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CControl::vDebugRender()
		{
			if (m_bActive)
			{
				LYNXCOLORRGBA WireColor = { 0, 255, 0, 255 };
				m_lpEngine->GetlpGraphicsSystem()->DrawRectWireframe(m_Rect, WireColor);
			}
		}		
	}
}