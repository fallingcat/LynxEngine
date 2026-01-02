#include <LynxEngine_PCH.h>

#include <GUISystem/LynxGUIContext.h>
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
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		bool CmpRenderOrder(const GUISystem::CControl* lhs, const GUISystem::CControl* rhs) 
		{
			return (lhs->m_RenderOrder < rhs->m_RenderOrder);													
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		bool CmpRenderOrder(const GUISystem::CScrollGroup* lhs, const GUISystem::CScrollGroup* rhs) 
		{
			return (lhs->m_RenderOrder < rhs->m_RenderOrder);													
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		CContext::CContext(LynxEngine::CEngine* lpe, GameFramework::CUIPage* page, CControl* parent, LYNXRECT& rect)
		: CObj(lpe)
		{
			m_bApplyLayoutFactor = LYNX_TRUE;
            m_ActiveControl = NULL;
			m_ParentPage = page;
            m_Parent = parent;
            m_Rect = rect;
			m_DefaultFontScale = 1.0f;
			m_DummyControl = NULL;
			m_DummyControl = LYNXNEW CDummyControl(lpe->GetlpGUISystem());
			m_AnimationSet = Animation::CAnimationSetPtr(LYNXNEW Animation::CAnimationSet(lpe));

			m_FocusControlID = -1;
			m_FocusControl = NULL;

			#ifdef _DEBUG
				m_bDebugRender = LYNX_TRUE;
			#else
				m_bDebugRender = LYNX_FALSE;
			#endif

			m_State = STATE_NORMAL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		CContext::~CContext()
		{
			if (m_DummyControl)
			{
				LYNXDEL m_DummyControl;
				m_DummyControl = NULL;
			}

			for (CList<GUISystem::CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				LYNXDEL (*Control);
				(*Control) = NULL;
			}	
			m_ControlList.clear();
			            
            for (CList<CScrollGroup*>::CIterator ScrollGroup = m_ScrollGroupList.begin(); ScrollGroup != m_ScrollGroupList.end(); ++ScrollGroup)
                LYNXDEL (*ScrollGroup);
            m_ScrollGroupList.clear();			

			m_OutControlList.clear();
			m_InControlList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void CContext::SendEvent(const int id)
		{
			m_DummyControl->SetID(id);
			SetActiveControl(m_DummyControl);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void CContext::ApplyLayoutFactor(LYNXBOOL b)
		{
			m_bApplyLayoutFactor = b;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void CContext::GroupButtonPressed(CControl* b)
		{
			CList<CControl*>::CIterator Pos;
            
			int Group = b->GetGroupID();
            
			if (Group >= 0)
			{
				for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
				{
					if ((*Control) != b && (*Control)->GetControlType() == CT_STATEBUTTON && (*Control)->GetGroupID() == Group)
					{
						static_cast<CStateButton*>(*Control)->Pressed(LYNX_FALSE);
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		CControl* CContext::FindControl(const CString& name)
		{
			CList<CControl*>::CIterator Pos;
            
			Pos = LynxEngine::find(m_ControlList.begin(), m_ControlList.end(), name);
			if (Pos != m_ControlList.end())
				return (*Pos);
			
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------		
		CControl* CContext::FindControl(const DWORD id)
        {
			CList<CControl*>::CIterator Pos;
			CObj Dummy;
         
			Dummy.SetID(id);
			Pos = LynxEngine::find(m_ControlList.begin(), m_ControlList.end(), Dummy);
			if (Pos != m_ControlList.end())
				return (*Pos);
         
			return NULL;
        }         
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::HitControl(const DWORD id)
		{
			CControl* Control = FindControl(id);
			if (Control)
			{
				Control->vOnHit();
				if (Control->GetControlType() == CT_STATEBUTTON)
					GroupButtonPressed(Control);
			}             
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  a°c˙:
        //-------------------------------------------------------------------------------------------------------
        void CContext::SetActiveControl(GUISystem::CControl* c)
        {
            m_ActiveControl = c;
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::ReleaseControl(const CString& name)
		{
			CControl* Con = FindControl(name);
			if (Con)
				ReleaseControl(Con->GetID());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::ReleaseControl(const DWORD id)
        {
			CControl* Con = FindControl(id);
			if (Con)
			{
				if (Con->GetScrollGroup())
				{
					Con->GetScrollGroup()->RemoveControl(Con);
					//Con->SetContext(NULL);
				}
				LYNXDEL Con;
				m_ControlList.remove(Con);
			}
		}        
        //-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::ReleaseControl(const CControl* c)
		{
			CList<CControl*>::CIterator Control = LynxEngine::find(m_ControlList.begin(), m_ControlList.end(), c);
			if (Control != m_ControlList.end())
			{
				if ((*Control)->GetScrollGroup())
				{
					(*Control)->GetScrollGroup()->RemoveControl((*Control));					
				}
				LYNXDEL (*Control);
				m_ControlList.remove((*Control));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::ReleaseAllControls(CScrollGroup* g)
		{			
			CList<CControl*>::CIterator Control = m_ControlList.begin();
			while (Control != m_ControlList.end())
			{
				CControl* C = *Control;
				Control++;
				if (g)
				{
					if (C->GetScrollGroup() == g)
						ReleaseControl(C);				
				}
				else
				{
					ReleaseControl(C);				
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CControl* CContext::GetHitControl(int x, int y)
		{
			LYNXPOINT2D Pos;
            
			Pos.x = x;
			Pos.y = y;
			for (CList<CControl*>::reverse_iterator Control = m_ControlList.rbegin(); Control != m_ControlList.rend(); ++Control)
			{
				if ((*Control)->m_InputGestureFilter == InputSystem::CTouch::TGF_NULL || (*Control)->m_InputGestureFilter == InputSystem::CTouch::TGF_ALL)
				{
					if (!(*Control)->GetScrollGroup() || ((*Control)->GetScrollGroup() && (*Control)->GetScrollGroup()->IsActive()))
					{
						if ((*Control)->vHitTest(&Pos) && !(*Control)->GetContext() && (*Control)->GetControlType() != CT_DIALOG && (*Control)->GetControlType() != CT_WINDOW)
						{
							(*Control)->m_InputGesture = InputSystem::CTouch::TG_NULL;
							(*Control)->vOnHit();
							SetActiveControl((*Control));
							if ((*Control)->GetControlType() == CT_STATEBUTTON)
								GroupButtonPressed((*Control));
							//ClearAllInputData();
							return (*Control);
						}
					}
				}
			}
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::SetFocusControl(const int id)
		{
			if (id < 0)
			{
				m_FocusControlID = id;
				m_FocusControl = NULL;
				glpKernel->GetScreenFilterSprite()->GetlpCurrentMaterial()->m_Diffuse.Alpha = 0;
			}
			else
			{
				m_FocusControlID = id;
				m_FocusControl = FindControl(id);
				if (m_FocusControl)
				{
					glpKernel->GetScreenFilterSprite()->GetlpCurrentMaterial()->m_Diffuse.Alpha = 150;
				}
				else
				{
					glpKernel->GetScreenFilterSprite()->GetlpCurrentMaterial()->m_Diffuse.Alpha = 0;
				}
			}
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CControl* CContext::GetHitControl(InputSystem::CTouch::TOUCHGESTURE gesture, int x, int y)
		{
			LYNXPOINT2D Pos;
            
			Pos.x = x;
			Pos.y = y;
            
			for (CList<CControl*>::reverse_iterator Control = m_ControlList.rbegin(); Control != m_ControlList.rend(); ++Control)
			{
				if (m_lpEngine->GetlpKernel()->GetGestureState(GameFramework::CKernel::TAP_GESTURE))
                {
                    if ((*Control)->GetControlType() == CT_STATEBUTTON || (*Control)->GetControlType() == CT_BUTTON)
                    {
                        if (gesture != InputSystem::CTouch::TG_TAP)
                            continue;
                    }
                }
                
				if (!(*Control)->GetScrollGroup() || ((*Control)->GetScrollGroup() && (*Control)->GetScrollGroup()->IsActive()))
				{
					if (!(*Control)->IsNoInput() && !(*Control)->GetContext() && (*Control)->GetControlType() != CT_DIALOG && (*Control)->GetControlType() != CT_WINDOW && (*Control)->vHitTest(&Pos))
					{
						if (m_FocusControl && (*Control) != m_FocusControl)
							continue;

						switch (gesture)
						{
							case InputSystem::CTouch::TG_TAP:
								if ((*Control)->m_InputGestureFilter & InputSystem::CTouch::TGF_TAP)
								{
									(*Control)->m_InputGesture = gesture;
									(*Control)->vOnHit();
									SetActiveControl((*Control));
									if ((*Control)->GetControlType() == CT_STATEBUTTON)
										GroupButtonPressed((*Control));
									//ClearAllInputData();
									return (*Control);
								}
								break;

							case InputSystem::CTouch::TG_LONGPRESS:
								if ((*Control)->m_InputGestureFilter & InputSystem::CTouch::TGF_LONGPRESS)
								{
									(*Control)->m_InputGesture = gesture;
									(*Control)->vOnLongPress();
									SetActiveControl((*Control));
									if ((*Control)->GetControlType() == CT_STATEBUTTON)
										GroupButtonPressed((*Control));
									//ClearAllInputData();
									return (*Control);
								}
								break;
						}
					}
				}

				if (!(*Control)->GetScrollGroup() || ((*Control)->GetScrollGroup() && (*Control)->GetScrollGroup()->IsActive()))
				{
					if (!(*Control)->IsNoInput() && ((*Control)->GetContext() || (*Control)->GetControlType() == CT_DIALOG || (*Control)->GetControlType() == CT_WINDOW) && (*Control)->vHitTest(&Pos))
					{
						if (m_FocusControl && (*Control) != m_FocusControl)
							continue;

						switch (gesture)
						{
							case InputSystem::CTouch::TG_TAP:
								if ((*Control)->m_InputGestureFilter & InputSystem::CTouch::TGF_TAP)
								{
									(*Control)->m_InputGesture = gesture;
									(*Control)->vOnHit();
									SetActiveControl((*Control));
									if ((*Control)->GetControlType() == CT_STATEBUTTON)
										GroupButtonPressed((*Control));
									//ClearAllInputData();
									return (*Control);
								}
								break;

							case InputSystem::CTouch::TG_LONGPRESS:
								if ((*Control)->m_InputGestureFilter & InputSystem::CTouch::TGF_LONGPRESS)
								{
									(*Control)->m_InputGesture = gesture;
									(*Control)->vOnLongPress();
									SetActiveControl((*Control));
									if ((*Control)->GetControlType() == CT_STATEBUTTON)
										GroupButtonPressed((*Control));
									//ClearAllInputData();
									return (*Control);
								}
								break;
						}
					}
				}
			}
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CContext::IsInputFocused()
		{
			if (m_lpEngine->GetlpKernel()->GetCurrentFocusedContext() == this)
			{
				return LYNX_TRUE;
			}
						
			if (m_Parent)
			{
				CContext* CurrentContext = this;
				while (CurrentContext && CurrentContext->m_Parent && CurrentContext->m_Parent->GetParentControl())
				{
					if (CurrentContext->m_Parent->GetParentControl()->GetContext() == m_lpEngine->GetlpKernel()->GetCurrentFocusedContext())
						return LYNX_TRUE;

					CurrentContext = CurrentContext->m_Parent->GetParentControl()->GetContext();
				}
			}

			if (m_Parent)
			{
				CContext* CurrentContext = this;
				while (CurrentContext)
				{
					if (CurrentContext == m_lpEngine->GetlpKernel()->GetCurrentFocusedContext())
						return LYNX_TRUE;

					if (CurrentContext->m_Parent)
						CurrentContext = CurrentContext->m_Parent->GetParentContext();
					else
						CurrentContext = NULL;
				}
			}

			if (m_lpEngine->GetlpKernel()->GetCurrentFocusedContext()->m_Parent)
			{
				CControl* ParentControl = m_lpEngine->GetlpKernel()->GetCurrentFocusedContext()->m_Parent;
				if (ParentControl->GetControlType() == GUISystem::CT_TAB)
				{
					CWindow* Window = dynamic_cast<CWindow*>(ParentControl->GetlpParent());
					if (Window && Window->GetContext() == this)
					{
						return LYNX_TRUE;
					}
				}		
			}			
			
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::GetInputFocus()
        {
			if (m_lpEngine->GetlpKernel()->GetModalWindow())
				m_lpEngine->GetlpKernel()->SetCurrentFocusedContext(m_lpEngine->GetlpKernel()->GetModalWindow()->GetContext());
			else
				m_lpEngine->GetlpKernel()->SetCurrentFocusedContext(this);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::GetInput()
		{
			if (!m_lpEngine->GetlpKernel()->GetModalWindow() || (m_lpEngine->GetlpKernel()->GetModalWindow() && m_lpEngine->GetlpKernel()->GetModalWindow()->GetContext() == this))
			{
				if (IsInputFocused())
				{					
					InputSystem::CMouse* lpMouse = (InputSystem::CMouse*)(m_lpEngine->GetlpSystemMouse());
					if (lpMouse)
					{
						if (m_lpEngine->GetlpInputSystem()->IsPlayingBack())
							m_lpEngine->GetlpSystemInputRecord()->Playback();
						else
							lpMouse->vPoll();
                    
						CControl* lpControl = NULL;
						if (lpMouse->ButtonUpStatus(InputSystem::CMouse::LBUTTON))
						{
							GetHitControl(InputSystem::CTouch::TG_TAP, lpMouse->GetPos().x, lpMouse->GetPos().y);
						}

						if (lpMouse->ButtonUpStatus(InputSystem::CMouse::RBUTTON))
						{
							GetHitControl(InputSystem::CTouch::TG_LONGPRESS, lpMouse->GetPos().x, lpMouse->GetPos().y);
						}
						//lpMouse->ClearPollData();
					}
                
					InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();
					if (lpTouch)
					{
						if (m_lpEngine->GetlpInputSystem()->IsPlayingBack())
							m_lpEngine->GetlpSystemInputRecord()->Playback();
						else
							lpTouch->vPoll();						
                    
						switch (lpTouch->GetTouchGesture())
						{
							case InputSystem::CTouch::TG_NULL:
								break;
                            
							default:
								//if (lpTouch->GetNumTouches())
									GetHitControl(lpTouch->GetTouchGesture(), lpTouch->GetTouch(0).Pos.x, lpTouch->GetTouch(0).Pos.y);
								/*if (GetActiveControl())
									ClearAllInputData();*/
								break;							
                        
							case InputSystem::CTouch::TG_PAN:
							{
								CVector2 Velocity = lpTouch->GetGestureValue2();
								Velocity.x *= -1.0f;										
								for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
								{
									if (lpTouch->GetNumTouches())
									{
										Math::CVector2 Pos = lpTouch->GetTouch(0).Pos;
										if ((*Group)->IsHit(Pos))
										{
											(*Group)->SetPanVelocity(Velocity);
										}
									}
									else
									{
										//(*Group)->SetPanVelocity(Velocity);
									}                                
								}
								break;
							}
						}							
                        
						if (m_ActiveControl && lpTouch)
						{
							if (lpTouch->GetTouchGesture() == InputSystem::CTouch::TG_TAP || lpTouch->GetTouchGesture() == InputSystem::CTouch::TG_LONGPRESS)
							{
								lpTouch->ClearTouchGesture();
							}
							//lpTouch->ClearTouches();
						}
						else
						{
							/*
							if (lpTouch->GetNumTouches())
							{
								GetHitControl(lpTouch->GetTouch(0).Pos.x, lpTouch->GetTouch(0).Pos.y);
							}
							*/
						}
					}

					InputSystem::CKeyboard* lpKeyboard = m_lpEngine->GetlpSystemKeyboard();
					if (lpKeyboard)
					{
						lpKeyboard->vPoll();
                    
						CVector2 PanVel(0.0f, 0.0f);

						if (lpKeyboard->Key(VK_UP))
						{
							PanVel = CVector2(0.0f, 200.0f);

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);                                
							}
						}
						else if (lpKeyboard->Key(VK_DOWN))
						{
							PanVel = CVector2(0.0f, -200.0f);

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);                                
							}
						}

						if (lpKeyboard->Key(VK_LEFT))
						{
							PanVel = CVector2(300.0f, 0.0f);

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);                                
							}
						}
						else if (lpKeyboard->Key(VK_RIGHT))
						{
							PanVel = CVector2(-300.0f, 0.0f);

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);                                
							}
						}
						else if (lpKeyboard->Key(VK_ESCAPE))
						{
							glpKernel->vOnSysBackPressed();
						}
					}					
					
					/*
					InputSystem::CGamePad* GamePad = m_lpEngine->GetSystemGamePad(0);
					if (GamePad && GamePad->IsConnected())
					{
						GamePad->vPoll();						

						CVector2 PanVel(0.0f, 0.0f);

						if (GamePad->m_LeftSticker.y < 0.0f || GamePad->GetButtonState(InputSystem::CGamePad::DPAD_DOWN))
						{
							PanVel.y =200.0f;

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);
							}
						}
						else if (GamePad->m_LeftSticker.y > 0.0f || GamePad->GetButtonState(InputSystem::CGamePad::DPAD_UP))
						{
							PanVel.y = -200.0f;

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);
							}
						}

						if (GamePad->m_LeftSticker.x < 0.0f || GamePad->GetButtonState(InputSystem::CGamePad::DPAD_LEFT))
						{
							PanVel.x = 300.0f;

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);
							}
						}
						else if (GamePad->m_LeftSticker.x > 0.0f || GamePad->GetButtonState(InputSystem::CGamePad::DPAD_RIGHT))
						{
							PanVel.x = -300.0f;

							for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
							{
								(*Group)->SetPanVelocity(PanVel);
							}							
						}		

						if (GamePad->GetButtonState(InputSystem::CGamePad::BACK))
						{							
							glpKernel->vOnSysBackPressed();
							GamePad->vSetVibrationSpeed(CVector2(0.2f, 0.0f), 1.5f);
						}						
					}
					*/

					if (m_FocusControl)
					{
						if (m_ActiveControl != m_FocusControl)
						{
							m_ActiveControl = NULL;
						}
					}
					
				}

				/*
				for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
				{
					if ((*Control)->GetContext())
					{
						(*Control)->GetContext()->GetInput();
					}
				}
				*/
			}
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		CVector2 CContext::GetAbsoluteCoordOrigin(GUISystem::CContext* c)
		{
			CVector2 Origin(0.0f, 0.0f);

			GUISystem::CContext* CurrentContext = c;
			while (CurrentContext)
			{
				LYNXRECT Rect = CurrentContext->GetRect();
				Origin.x += Rect.x1;
				Origin.y += Rect.y1;

				if (CurrentContext->GetParent() && CurrentContext->GetParent()->GetScrollGroup())
				{
					Origin.x += CurrentContext->GetParent()->GetScrollGroup()->GetOffset().x;
					Origin.y += CurrentContext->GetParent()->GetScrollGroup()->GetOffset().y;
				}

				if (CurrentContext->GetParent() && CurrentContext->GetParent()->GetParentControl())
					CurrentContext = CurrentContext->GetParent()->GetParentControl()->GetContext();
				else
					CurrentContext = NULL;
			}
			return Origin;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::vLoop(float step)
		{
			SortControls();
			SortGroups();

            if (!GetActiveControl())
            {
                GetInput();
            }
            
			ControlLoop(step);	
			UpdateControlRects();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CContext::IsControlArrayTransitionDone(CArray<CControl*>& list)
		{
			for (int i=0; i<list.size(); i++)
			{
				if (list[i]->IsActive() && list[i]->IsShown())
				{
					if (!list[i]->IsTransitionDone())
						return LYNX_FALSE;
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CContext::IsControlTransitionDone()
		{
			for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				if ((*Control)->IsActive() && (*Control)->IsShown())
				{
					if (!(*Control)->IsTransitionDone())
						return LYNX_FALSE;
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::ShowControlArray(CArray<CControl*>& list, LYNXBOOL b)
		{
			for (int i=0; i<list.size(); i++)
			{
				list[i]->Show(b);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::ActivateControlArray(CArray<CControl*>& list, LYNXBOOL b)
		{
			for (int i=0; i<list.size(); i++)
			{
				list[i]->Activate(b);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::ResetControlArrayActiveState(CArray<CControl*>& list)
		{
			for (int i=0; i<list.size(); i++)
			{
				list[i]->ResetActiveState();				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CContext::ControlLoop(float step)
		{
			for (CList<CScrollGroup*>::CIterator Group = GetScrollGroupList().begin(); Group != GetScrollGroupList().end(); ++Group)
			{		
				(*Group)->vLoop(step);
			}
			for (CList<CControl*>::CIterator Control = GetControlList().begin(); Control != GetControlList().end(); ++Control)
			{
				(*Control)->vLoop(step);
			}

			if (!m_OutControlList.empty() || !m_InControlList.empty())
			{
				switch (m_State)
				{
					case STATE_GROUP_STEP_OUT:
						//m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_TRUE);
						if (IsControlArrayTransitionDone(m_OutControlList))
						{
							ShowControlArray(m_OutControlList, LYNX_FALSE);
							ActivateControlArray(m_OutControlList, LYNX_FALSE);

							if (m_InControlList.size())
							{
								ShowControlArray(m_InControlList, LYNX_TRUE);
								//ActivateControlArray(m_InControlList, LYNX_TRUE);
								ResetControlArrayActiveState(m_InControlList);
								for (int i=0; i<m_InControlList.size(); i++)
								{
									m_InControlList[i]->Transit();
								}
								m_State = STATE_GROUP_STEP_IN;
							}
							else
							{
								m_State = STATE_GROUP_TRANSITION_OVER;
								m_OutControlList.clear();
								m_InControlList.clear();
								//m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
							}
						}
					break;

					case STATE_GROUP_STEP_IN:
						//m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_TRUE);
						if (IsControlArrayTransitionDone(m_InControlList))
						{
							m_State = STATE_GROUP_TRANSITION_OVER;
							m_OutControlList.clear();
							m_InControlList.clear();
							//m_lpEngine->GetlpInputSystem()->BlockAllInput(LYNX_FALSE);
						}
					break;
				}
			}
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		CScrollGroup* CContext::CreateScrollGroup(const CString& name, const LYNXRECT& rect)
		{
			CScrollGroup* ScrollGroup = LYNXNEW CScrollGroup(m_lpEngine);

			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect, GroupRect;
			
			if (!m_bApplyLayoutFactor)
            {				
                GroupRect = rect;
            }
            else 
            {
				GroupRect.x1 = (rect.x1 * PositionFactor.x);
				GroupRect.x2 = (GroupRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
				GroupRect.y1 = (rect.y1 * PositionFactor.y);
				GroupRect.y2 = (GroupRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }

			ScrollGroup->Create(GroupRect);
			ScrollGroup->SetName(name);
			ScrollGroup->SetContext(this);
			ScrollGroup->SetParentControl(m_Parent);
			m_ScrollGroupList.push_back(ScrollGroup);						
            
			return ScrollGroup;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		CScrollGroup* CContext::FindScrollGroup(const CString& name)
		{
			CList<CScrollGroup*>::CIterator Pos;
            
			Pos = LynxEngine::find(m_ScrollGroupList.begin(), m_ScrollGroupList.end(), name);
			if (Pos != m_ScrollGroupList.end())
				return (*Pos);
			
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::ResetControlTransitionOffset()
		{
			for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				(*Control)->ResetTransitionOffset();
			}
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::SortControls()
		{
			//LynxEngine::sort<CList<CControl*>::CIterator, CMPCONTROLFUNC>(m_ControlList.begin(), m_ControlList.end(), CmpRenderOrder);
			m_ControlList.sort<CMPCONTROLFUNC>(CmpRenderOrder);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::SortGroups()
		{
			//LynxEngine::sort<CList<CScrollGroup*>::CIterator, CMPGROUPFUNC>(m_ScrollGroupList.begin(), m_ScrollGroupList.end(), CmpRenderOrder);
			m_ScrollGroupList.sort<CMPGROUPFUNC>(CmpRenderOrder);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::UpdateControlRects()
		{
			RenderControls(LYNX_TRUE);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::RenderControls(LYNXBOOL bupdateonly)
		{
			LYNXRECT ClipRect;
			LYNXRECT OldRect, NewRect, OldClipRect;
			LYNXBOOL bScissor;
			CContext* CurrentContext;
            
			CVector2 Origin = GetAbsoluteCoordOrigin(this);

			if (!bupdateonly)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetScissorRect(&OldClipRect);
				bScissor = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->IsScissorRectEnabled();
            
				//SortControls();
				//SortGroups();
			}

			for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				if (!(*Control)->GetScrollGroup() && (*Control) != m_ActiveControl && !(*Control)->IsHintScale())
				{
					// Reposition controls according to the position of CWindow
					(*Control)->vGetAlignmentRect(OldRect);
					NewRect = OldRect;
					NewRect.x1 += Origin.x;
					NewRect.x2 += Origin.x;
					NewRect.y1 += Origin.y;
					NewRect.y2 += Origin.y;

					(*Control)->m_bClipped = LYNX_FALSE;								
					(*Control)->SetContainerAlignmentRect(NewRect);
					if (bupdateonly)
					{
						(*Control)->ComputeRenderRect();
					}
					else
					{
						if (!m_FocusControl || (m_FocusControl && m_FocusControl != (*Control)))
							(*Control)->vRender();
					}
					(*Control)->SetContainerAlignmentRect(OldRect);
				}
			}        
            
			// TODO : Render controls by scroll group since we need to set up scissor rect before rendering
			for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
			{				
				if ((*Group)->IsShown())
				{
					ClipRect = (*Group)->GetRect();
					
					ClipRect.x1 += Origin.x;
					ClipRect.x2 += Origin.x;
					ClipRect.y1 += Origin.y;
					ClipRect.y2 += Origin.y;

					if (!bupdateonly)
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect((*Group)->m_bClip);
						m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);
					}
                
					for (CList<CControl*>::CIterator Control = (*Group)->GetControlList().begin(); Control != (*Group)->GetControlList().end(); ++Control)
					{
						if ((*Control) != m_ActiveControl && !(*Control)->IsHintScale())
						{
							// Reposition controls according to the position of CWindow
							(*Control)->vGetAlignmentRect(OldRect);
							NewRect = OldRect;

							NewRect.x1 += Origin.x;
							NewRect.x2 += Origin.x;
							NewRect.y1 += Origin.y;
							NewRect.y2 += Origin.y;

							if ((*Control)->GetScrollGroup())
							{
								NewRect.x1 += (*Control)->GetScrollGroup()->GetOffset().x;
								NewRect.x2 += (*Control)->GetScrollGroup()->GetOffset().x;
								NewRect.y1 += (*Control)->GetScrollGroup()->GetOffset().y;
								NewRect.y2 += (*Control)->GetScrollGroup()->GetOffset().y;
							}

							if (!(NewRect.x2 < ClipRect.x1 || NewRect.y2 < ClipRect.y1 || NewRect.x1 > ClipRect.x2 || NewRect.y1 > ClipRect.y2))
							{
								(*Control)->SetContainerAlignmentRect(NewRect);
								if (bupdateonly)
									(*Control)->ComputeRenderRect();
								else
								{
									if (!m_FocusControl || (m_FocusControl && m_FocusControl != (*Control)))
										(*Control)->vRender();
								}
								(*Control)->m_bClipped = LYNX_FALSE;
								(*Control)->SetContainerAlignmentRect(OldRect);					
							}
							else
							{
								(*Control)->SetContainerAlignmentRect(NewRect);
								(*Control)->ComputeRenderRect();
								(*Control)->m_bClipped = LYNX_TRUE;
								(*Control)->SetContainerAlignmentRect(OldRect);													
							}
						}						
					}
				}
			}

			// for control is using HintScale
			for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				if (!(*Control)->GetScrollGroup() && (*Control) != m_ActiveControl && (*Control)->IsHintScale())
				{
					// Reposition controls according to the position of CWindow
					(*Control)->vGetAlignmentRect(OldRect);
					NewRect = OldRect;
					NewRect.x1 += Origin.x;
					NewRect.x2 += Origin.x;
					NewRect.y1 += Origin.y;
					NewRect.y2 += Origin.y;

					(*Control)->m_bClipped = LYNX_FALSE;
					(*Control)->SetContainerAlignmentRect(NewRect);
					if (bupdateonly)
					{
						(*Control)->ComputeRenderRect();
					}
					else
					{
						if (!m_FocusControl || (m_FocusControl && m_FocusControl != (*Control)))
							(*Control)->vRender();
					}
					(*Control)->SetContainerAlignmentRect(OldRect);
				}
			}

			// TODO : Render controls by scroll group since we need to set up scissor rect before rendering
			for (CList<CScrollGroup*>::CIterator Group = m_ScrollGroupList.begin(); Group != m_ScrollGroupList.end(); ++Group)
			{
				if ((*Group)->IsShown())
				{
					ClipRect = (*Group)->GetRect();

					ClipRect.x1 += Origin.x;
					ClipRect.x2 += Origin.x;
					ClipRect.y1 += Origin.y;
					ClipRect.y2 += Origin.y;

					if (!bupdateonly)
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect((*Group)->m_bClip);
						m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);
					}

					for (CList<CControl*>::CIterator Control = (*Group)->GetControlList().begin(); Control != (*Group)->GetControlList().end(); ++Control)
					{
						if ((*Control) != m_ActiveControl && (*Control)->IsHintScale())
						{
							// Reposition controls according to the position of CWindow
							(*Control)->vGetAlignmentRect(OldRect);
							NewRect = OldRect;

							NewRect.x1 += Origin.x;
							NewRect.x2 += Origin.x;
							NewRect.y1 += Origin.y;
							NewRect.y2 += Origin.y;

							if ((*Control)->GetScrollGroup())
							{
								NewRect.x1 += (*Control)->GetScrollGroup()->GetOffset().x;
								NewRect.x2 += (*Control)->GetScrollGroup()->GetOffset().x;
								NewRect.y1 += (*Control)->GetScrollGroup()->GetOffset().y;
								NewRect.y2 += (*Control)->GetScrollGroup()->GetOffset().y;
							}

							if (!(NewRect.x2 < ClipRect.x1 || NewRect.y2 < ClipRect.y1 || NewRect.x1 > ClipRect.x2 || NewRect.y1 > ClipRect.y2))
							{
								(*Control)->SetContainerAlignmentRect(NewRect);
								if (bupdateonly)
									(*Control)->ComputeRenderRect();
								else
								{
									if (!m_FocusControl || (m_FocusControl && m_FocusControl != (*Control)))
										(*Control)->vRender();
								}
								(*Control)->m_bClipped = LYNX_FALSE;
								(*Control)->SetContainerAlignmentRect(OldRect);
							}
							else
							{
								(*Control)->SetContainerAlignmentRect(NewRect);
								(*Control)->ComputeRenderRect();
								(*Control)->m_bClipped = LYNX_TRUE;
								(*Control)->SetContainerAlignmentRect(OldRect);
							}
						}
					}
				}
			}

			if (m_ActiveControl)
            {				
				if (!m_ActiveControl->GetScrollGroup())
				{
					if (!bupdateonly)
					{
						m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&OldClipRect);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(bScissor);			
					}
					// Reposition controls according to the position of CWindow
					m_ActiveControl->vGetAlignmentRect(OldRect);
					NewRect = OldRect;

					NewRect.x1 += Origin.x;
					NewRect.x2 += Origin.x;
					NewRect.y1 += Origin.y;
					NewRect.y2 += Origin.y;

					m_ActiveControl->m_bClipped = LYNX_FALSE;								
					m_ActiveControl->SetContainerAlignmentRect(NewRect);
					if (bupdateonly)
						m_ActiveControl->ComputeRenderRect();
					else
					{
						if (!m_FocusControl || (m_FocusControl && m_FocusControl != m_ActiveControl))
							m_ActiveControl->vRender();
					}
					m_ActiveControl->SetContainerAlignmentRect(OldRect);
				}
				else
				{
					if (m_ActiveControl->GetScrollGroup()->IsShown())
					{
						if (!bupdateonly)
						{
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(m_ActiveControl->GetScrollGroup()->m_bClip);
							ClipRect = m_ActiveControl->GetScrollGroup()->GetRect();						
							ClipRect.x1 += m_Rect.x1;
							ClipRect.x2 += m_Rect.x1;
							ClipRect.y1 += m_Rect.y1;
							ClipRect.y2 += m_Rect.y1;
							m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);
						}

						// Reposition controls according to the position of CWindow
						m_ActiveControl->vGetAlignmentRect(OldRect);
						NewRect = OldRect;

						NewRect.x1 += Origin.x;
						NewRect.x2 += Origin.x;
						NewRect.y1 += Origin.y;
						NewRect.y2 += Origin.y;

						NewRect.x1 += m_ActiveControl->GetScrollGroup()->GetOffset().x;
						NewRect.x2 += m_ActiveControl->GetScrollGroup()->GetOffset().x;
						NewRect.y1 += m_ActiveControl->GetScrollGroup()->GetOffset().y;
						NewRect.y2 += m_ActiveControl->GetScrollGroup()->GetOffset().y;						

						m_ActiveControl->m_bClipped = LYNX_FALSE;								
						m_ActiveControl->SetContainerAlignmentRect(NewRect);
						if (bupdateonly)
							m_ActiveControl->ComputeRenderRect();
						else
						{
							if (!m_FocusControl || (m_FocusControl && m_FocusControl != m_ActiveControl))
								m_ActiveControl->vRender();
						}
						m_ActiveControl->SetContainerAlignmentRect(OldRect);					
					}
				}
            }

			/*
			if (m_FocusControl)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetFullScreenViewport();	
				glpKernel->GetScreenFilterSprite()->vRender();
				m_lpEngine->GetlpGraphicsSystem()->SetViewportRect();
				m_FocusControl->vRender();
			}			
			*/

			if (!bupdateonly)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&OldClipRect);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(bScissor);			
			}

			if (m_bDebugRender)
			{
				for (CList<CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
				{
					(*Control)->vDebugRender();						
				}
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CButton* CContext::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask, const int text_w, const int text_h, const int textalignment)
		{
			CAnsiString ClassName = "CButton";
			if (classname)
				ClassName = classname;

			CButton* lpButton = (CButton*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(lpButton);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
			
            lpButton->SetFontScale(m_DefaultFontScale);
            
			CButtonCreationDesc Desc;
			Desc.Default();
            Desc.m_Name = text;
			Desc.m_bLoadHitMask = buse_mask;
            Desc.m_bUseSpriteTextString = buse_sprite_text;
			Desc.m_bShowText = LYNX_TRUE;
			Desc.m_TextSize.x = text_w * ScaleFactor.x;
			Desc.m_TextSize.y = text_h * ScaleFactor.y;
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_FileName = filename;
			Desc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_SpriteDesc.m_Number = 0;
			Desc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_SpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Material"));
			Desc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
			Desc.m_TextAlignment = textalignment;
            if (!m_bApplyLayoutFactor)
            {				
                Desc.m_SpriteDesc.m_Width = size.x;
                Desc.m_SpriteDesc.m_Height = size.y;
                Desc.m_SpriteDesc.m_AlignmentRect = rect;
            }
            else 
            {
				Desc.m_SpriteDesc.m_Width = size.x * ScaleFactor.x;
				Desc.m_SpriteDesc.m_Height = size.y * ScaleFactor.y;
				Desc.m_SpriteDesc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
				Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
				Desc.m_SpriteDesc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
				Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
			/*
             float Width = rect.x2 - rect.x1;
             float Height = rect.y2 - rect.y1;
             //Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             //Desc.m_SpriteDesc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);
             
             Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + Width*ScaleFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + Height*ScaleFactor.y);
             */
            
			Desc.m_SpriteDesc.m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER;
			Desc.m_SpriteDesc.m_OriginType = CSpriteContainer::OT_CENTER;
			lpButton->vCreate(&Desc);
			lpButton->SetName(text);
			lpButton->SetID(id);
			lpButton->vInit();			
			if (m_Parent)
				lpButton->SetParent(m_Parent, NULL);
            
			return lpButton;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CContext::RearrangeControl(GUISystem::CControl* control,  const LYNXRECT& rect)
		{
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect, AlignmentRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
			
			if (!m_bApplyLayoutFactor)
            {				
                AlignmentRect = rect;
            }
            else 
            {
				AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
				AlignmentRect.x2 = (AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
				AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
				AlignmentRect.y2 = (AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }			
			control->SetAlignmentRect(AlignmentRect);
			control->SetTargetAlignmentRect(AlignmentRect);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CButton* CContext::AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask, const int text_w, const int text_h, const int textalignment)
		{
			CAnsiString ClassName = "CButton";
			if (classname)
				ClassName = classname;

			CButton* lpButton = (CButton*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(lpButton);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
            
            lpButton->SetFontScale(m_DefaultFontScale);
            
			CButtonCreationDesc Desc;
			Desc.Default();
            Desc.m_Name = text;
			Desc.m_bLoadHitMask = buse_mask;
            Desc.m_bUseSpriteTextString = buse_sprite_text;
			Desc.m_bShowText = LYNX_TRUE;
			Desc.m_TextSize.x = text_w * ScaleFactor.x;
			Desc.m_TextSize.y = text_h * ScaleFactor.y;
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_FileName = filename;
			Desc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_SpriteDesc.m_Number = 0;
			Desc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_TextAlignment = textalignment;
			if (mat_name)
				Desc.m_SpriteDesc.m_MaterialName = mat_name;
			else
				Desc.m_SpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Material"));
			Desc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
            if (!m_bApplyLayoutFactor)
            {
                Desc.m_SpriteDesc.m_Width = size.x;
                Desc.m_SpriteDesc.m_Height = size.y;
                Desc.m_SpriteDesc.m_AlignmentRect = rect;
            }
            else
            {
                Desc.m_SpriteDesc.m_Width = size.x * ScaleFactor.x;
                Desc.m_SpriteDesc.m_Height = size.y * ScaleFactor.y;
                Desc.m_SpriteDesc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
                Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
            
			/*
             Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + size.x*ScaleFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + size.y*ScaleFactor.y);
             
             //Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             //Desc.m_SpriteDesc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);
             */
            
			Desc.m_SpriteDesc.m_UVOffset[0].x = uvoffset[0];
			Desc.m_SpriteDesc.m_UVOffset[0].y = uvoffset[1];
			Desc.m_SpriteDesc.m_UVOffset[1].x = uvoffset[2];
			Desc.m_SpriteDesc.m_UVOffset[1].y = uvoffset[3];
			lpButton->vCreate(&Desc);
			lpButton->SetName(text);
			lpButton->SetID(id);			
			lpButton->SetAlignmentType(CSpriteContainer::V_ALIGN_CENTER|CSpriteContainer::H_ALIGN_CENTER);
			lpButton->SetOriginType(CSpriteContainer::OT_CENTER);
			if (m_Parent)
				lpButton->SetParent(m_Parent, NULL);
            
			return lpButton;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CStateButton* CContext::AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name, const LYNXBOOL buse_mask)
		{
			CAnsiString ClassName = "CStateButton";
			if (classname)
				ClassName = classname;

			CStateButton* lpButton = (CStateButton*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(lpButton);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
            
            lpButton->SetFontScale(m_DefaultFontScale);
            
			CStateButtonCreationDesc Desc;
			Desc.Default();
			Desc.m_bShowText = LYNX_TRUE;
            Desc.m_Name = text;
			Desc.m_bLoadHitMask = buse_mask;
            Desc.m_bUseSpriteTextString = buse_sprite_text;
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_PressedStateFileName = pressedfilename;
			Desc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_FileName = filename;
			Desc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_SpriteDesc.m_Number = 0;
			Desc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			if (mat_name)
				Desc.m_SpriteDesc.m_MaterialName = mat_name;
			else
				Desc.m_SpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Material"));
			Desc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
            if (!m_bApplyLayoutFactor)
            {
                Desc.m_SpriteDesc.m_Width = size.x;
                Desc.m_SpriteDesc.m_Height = size.y;
                Desc.m_SpriteDesc.m_AlignmentRect = rect;
            }
            else
            {
				Desc.m_SpriteDesc.m_Width = size.x * ScaleFactor.x;
				Desc.m_SpriteDesc.m_Height = size.y * ScaleFactor.y;	
                Desc.m_SpriteDesc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
                Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
			/*
             //Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + size.x*ScaleFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             //Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + size.y*ScaleFactor.y);
             
             Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             Desc.m_SpriteDesc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);
             */
            
			lpButton->vCreate(&Desc);
			lpButton->SetName(text);
			lpButton->SetID(id);			
			lpButton->vInit();
			lpButton->SetAlignmentType(CSpriteContainer::V_ALIGN_CENTER|CSpriteContainer::H_ALIGN_CENTER);
			lpButton->SetOriginType(CSpriteContainer::OT_CENTER);
			if (m_Parent)
				lpButton->SetParent(m_Parent, NULL);
            
			return lpButton;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CSlider* CContext::AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text)
		{
			CAnsiString ClassName = "CSlider";
			if (classname)
				ClassName = classname;

			CSlider* lpSlider = (CSlider*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(lpSlider);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
            
			CSliderCreationDesc Desc;
			Desc.Default();
            Desc.m_Name = text;
			Desc.m_Pos = 0;
			Desc.m_RangeMin = min;
			Desc.m_RangeMax = max;
			Desc.m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER;
			if (!m_bApplyLayoutFactor)
            {
				Desc.m_Rect = rect;
			}
			else
			{
				Desc.m_Rect.x1 = rect.x1 * PositionFactor.x;
				Desc.m_Rect.x2 = rect.x2 * PositionFactor.x;
				Desc.m_Rect.y1 = rect.y1 * PositionFactor.y;
				Desc.m_Rect.y2 = rect.y2 * PositionFactor.y;
			}
            
			//Desc.m_Rect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
			//Desc.m_Rect.x2 = ClipRect.x1 + (rect.x2 * PositionFactor.x);
			//Desc.m_Rect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
			//Desc.m_Rect.y2 = ClipRect.y1 + (rect.y2 * PositionFactor.y);
            
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_FileName = filename;
			Desc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_SpriteDesc.m_Number = 0;
			Desc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_SpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Material"));
			Desc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
            if (!m_bApplyLayoutFactor)
            {
                Desc.m_SpriteDesc.m_Width = size.x;
                Desc.m_SpriteDesc.m_Height = size.y;
                Desc.m_SpriteDesc.m_AlignmentRect = rect;
            }
            else
            {
                Desc.m_SpriteDesc.m_Width = size.x * ScaleFactor.x;
                Desc.m_SpriteDesc.m_Height = size.y * ScaleFactor.y;
                Desc.m_SpriteDesc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
                Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
			/*
             //Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + size.x*ScaleFactor.x);
             //Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             //Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + size.y*ScaleFactor.y);
             
             Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x);
             Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             Desc.m_SpriteDesc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);
             */
            
			Desc.m_BarSpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_BarSpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_BarSpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_BarSpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_BarSpriteDesc.m_FileName = bar_filename;
			Desc.m_BarSpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_BarSpriteDesc.m_Number = 0;
			Desc.m_BarSpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_BarSpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Bar.Material"));
			Desc.m_BarSpriteDesc.m_bCreateMaterial = LYNX_TRUE;
			/*Desc.m_BarSpriteDesc.m_Width = (rect.x2 - rect.x1) * ScaleFactor.x + Desc.m_SpriteDesc.m_Width;
			Desc.m_BarSpriteDesc.m_Height = (rect.y2 - rect.y1) * ScaleFactor.y;
			Desc.m_BarSpriteDesc.m_AlignmentRect = Desc.m_Rect;*/
			//Desc.m_BarSpriteDesc.m_AlignmentRect.x1 -= size.x*0.5f;//Desc.m_SpriteDesc.m_Width * 0.5f;
			//Desc.m_BarSpriteDesc.m_AlignmentRect.x2 += size.x*0.5f;//Desc.m_SpriteDesc.m_Width * 0.5f;
			if (!m_bApplyLayoutFactor)
            {
				Desc.m_BarSpriteDesc.m_Width = (rect.x2 - rect.x1) + Desc.m_SpriteDesc.m_Width;
				Desc.m_BarSpriteDesc.m_Height = (rect.y2 - rect.y1);
				Desc.m_BarSpriteDesc.m_AlignmentRect = Desc.m_Rect;
            }
            else
            {
                Desc.m_BarSpriteDesc.m_Width = (rect.x2 - rect.x1) * ScaleFactor.x + Desc.m_SpriteDesc.m_Width;
				Desc.m_BarSpriteDesc.m_Height = (rect.y2 - rect.y1) * ScaleFactor.y;
				Desc.m_BarSpriteDesc.m_AlignmentRect = Desc.m_Rect;
            }
            
			lpSlider->vCreate(&Desc);
			lpSlider->SetName(text);
			lpSlider->SetID(id);			
			if (m_Parent)
				lpSlider->SetParent(m_Parent, NULL);
            
			return lpSlider;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CText* CContext::AddText(const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text, const int alignment)
		{
			CAnsiString ClassName = "CText";
			if (classname)
				ClassName = classname;

			CText* lpText = (CText*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(lpText);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
			
			CTextCreationDesc Desc;
			Desc.Default();
            Desc.m_bUseSpriteTextString = buse_sprite_text;
			Desc.m_Font = font;
			Desc.m_FontScale = scale;
            Desc.m_AlignmentType = alignment;
			Desc.m_OriginType = CSpriteContainer::OT_CENTER;
			if (!m_bApplyLayoutFactor)
            {
                Desc.m_AlignmentRect = rect;
            }
            else
            {
                Desc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
                Desc.m_AlignmentRect.x2 = (Desc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
                Desc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
                Desc.m_AlignmentRect.y2 = (Desc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
			/*if (color)
			{
				Desc.m_FontColor = color;
			}
			else
			{
				Desc.m_FontColor.PackedColor = 0xffffffff;
			}
			if (shadow_color)
			{
				Desc.m_FontShadowColor = shadow_color;
			}
			else
			{
				Desc.m_FontShadowColor.Red = 0;
				Desc.m_FontShadowColor.Green = 0;
				Desc.m_FontShadowColor.Blue = 0;
				Desc.m_FontShadowColor.Alpha = 156;
			}
			*/
			Desc.m_FontColor = color;
			Desc.m_FontShadowColor = shadow_color;
			Desc.m_bFontShadow = bshadow;
			Desc.m_Text = text;
			             
			/*
             float Width = rect.x2 - rect.x1;
             float Height = rect.y2 - rect.y1;
             //Desc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             //Desc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x);
             //Desc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             //Desc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);
             
             Desc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x);
             Desc.m_AlignmentRect.x2 = (Desc.m_AlignmentRect.x1 + Width*ScaleFactor.x);
             Desc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y);
             Desc.m_AlignmentRect.y2 = (Desc.m_AlignmentRect.y1 + Height*ScaleFactor.y);
             */
			
			lpText->vCreate(&Desc);
			lpText->SetID(id);			
			if (m_Parent)
				lpText->SetParent(m_Parent, NULL);
			
			return lpText;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		CImage* CContext::AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXBOOL buse_mask, const int text_w, const int text_h, const LYNXBOOL bshow_text)
		{
			CAnsiString ClassName = "CImage";
			if (classname)
				ClassName = classname;

			CImage* Image = (CImage*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(Image);
            
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
			LYNXRECT ClipRect;
			
			if (m_Parent)
			{
				ClipRect = m_Rect;
				ClipRect.x2 -= ClipRect.x1;
				ClipRect.x1 = 0;
				ClipRect.y2 -= ClipRect.y1;
				ClipRect.y1 = 0;
			}
			else // it's UIPage
			{
				ClipRect = m_Rect;
			}
            
            Image->SetFontScale(m_DefaultFontScale);
			
			CButtonCreationDesc Desc;
			Desc.Default();
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			if (text)
   				Desc.m_Name = text;
			Desc.m_bLoadHitMask = buse_mask;
            Desc.m_bUseSpriteTextString = buse_sprite_text;
			Desc.m_bShowText = bshow_text;
            Desc.m_TextSize.x = text_w;
            Desc.m_TextSize.y = text_h;
			Desc.m_SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			Desc.m_SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			Desc.m_SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
			Desc.m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			Desc.m_SpriteDesc.m_FileName = filename;
			Desc.m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			Desc.m_SpriteDesc.m_Number = 0;
			Desc.m_SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
			Desc.m_SpriteDesc.m_MaterialName = CString(filename) + CString(_T(".Material"));
			Desc.m_SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
            if (!m_bApplyLayoutFactor)
            {
                Desc.m_SpriteDesc.m_Width = size.x;
                Desc.m_SpriteDesc.m_Height = size.y;
                Desc.m_SpriteDesc.m_AlignmentRect = rect;
            }
            else
            {
                Desc.m_SpriteDesc.m_Width = size.x * ScaleFactor.x;
                Desc.m_SpriteDesc.m_Height = size.y * ScaleFactor.y;
                Desc.m_SpriteDesc.m_AlignmentRect.x1 = ClipRect.x1 + (rect.x1 * PositionFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + (rect.x2-rect.x1)*ScaleFactor.x);
                Desc.m_SpriteDesc.m_AlignmentRect.y1 = ClipRect.y1 + (rect.y1 * PositionFactor.y);
                Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + (rect.y2-rect.y1)*ScaleFactor.y);
            }
			/*
             
             float Width = rect.x2 - rect.x1;
             float Height = rect.y2 - rect.y1;
             
             
             Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x); 
             Desc.m_SpriteDesc.m_AlignmentRect.x2 = (Desc.m_SpriteDesc.m_AlignmentRect.x1 + Width*ScaleFactor.x); 
             Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y); 
             Desc.m_SpriteDesc.m_AlignmentRect.y2 = (Desc.m_SpriteDesc.m_AlignmentRect.y1 + Height*ScaleFactor.y);		
             
             //Desc.m_SpriteDesc.m_AlignmentRect.x1 = (rect.x1 * PositionFactor.x); 
             //Desc.m_SpriteDesc.m_AlignmentRect.x2 = (rect.x2 * PositionFactor.x); 
             //Desc.m_SpriteDesc.m_AlignmentRect.y1 = (rect.y1 * PositionFactor.y); 
             //Desc.m_SpriteDesc.m_AlignmentRect.y2 = (rect.y2 * PositionFactor.y);		
             */
            
			Desc.m_SpriteDesc.m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER;
			Desc.m_SpriteDesc.m_OriginType = CSpriteContainer::OT_CENTER;
			Image->vCreate(&Desc);		
			Image->SetID(id);		
			if (text)
				Image->ShowText(LYNX_TRUE);
			Image->vInit();
			if (m_Parent)
				Image->SetParent(m_Parent, NULL);
            
			return Image;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		CPEmitter* CContext::AddPEmitter(const LYNXANSICHAR* classname, const LYNXCHAR* filename)
		{
			CAnsiString ClassName = "CPEmitter";
			if (classname)
				ClassName = classname;

			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
			Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);

			CPEmitter* PEmitter = (CPEmitter*)m_lpEngine->GetlpGUISystem()->CreateControl(ClassName);
			AddControl(PEmitter);

			CUserProfile PEFile(filename);
			PEFile.Load();

			PEmitter->vCreateFromLayout(&PEFile, PEFile.GetRootElement(), this);
			CVector2 PScaleFactor = static_cast<CPEmitterContainer*>(PEmitter->GetlpContainer())->GetScaleFactor();
			PScaleFactor *= ScaleFactor;
			static_cast<CPEmitterContainer*>(PEmitter->GetlpContainer())->SetScaleFactor(PScaleFactor);
								
			return PEmitter;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::AddControl(CControl* c)
		{
			m_ControlList.push_back(c);
			c->SetParentControl(m_Parent);
			c->SetParentContext(this);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::ClearMouseInputData()
		{
			InputSystem::CMouse* lpMouse = m_lpEngine->GetlpSystemMouse();
			if (lpMouse)
			{
				lpMouse->ClearPollData(LYNX_TRUE);
			}
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  a︷
        //-------------------------------------------------------------------------------------------------------
        void CContext::ClearTouchGesture()
        {
            InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();
            if (lpTouch)
            {
                lpTouch->ClearTouchGesture();
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::ClearTouchInputData()
		{    
			InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();
			if (lpTouch)
			{
				lpTouch->ClearTouches();
				lpTouch->ClearTouchGesture();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::ClearAllInputData()
		{    
			ClearMouseInputData();
			ClearTouchInputData();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		int	CContext::GetDefinedControlID(const CString & key)
		{
			int ControlID = -1;
			if (m_ParentPage)
			{
				ControlID = m_ParentPage->GetDefinedControlID(key);
			}
			else if (m_Parent)
			{
				ControlID = ((CWindow*)m_Parent)->GetDefinedControlID(key);
			}
			return ControlID;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadGroup(CUserProfile* file, CXMLElement* group)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			CString Type, Name, Value;
			float VelScale = 1.0f;
			float BouncingSpeed = 1.0f;
			int Flags = 0;
			LYNXBOOL bBounceX, bBounceY, bShow, bActive;
			CVector2 SnapSize(-1.0f, -1.0f);
			LYNXBOOL bClip = LYNX_TRUE;
			int RenderOrder = 0;
			
            Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);
            Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
            
			Type = file->Read(_T("Type"), Type , group);						
			Name = file->Read(_T("Name"), Name , group);						
			ClipRect = file->Read(_T("Rect"), ClipRect , group);						
			VelScale = file->Read(_T("PanVelocityScale"), VelScale , group);	
			SnapSize = file->Read(_T("SnapSize"), SnapSize , group);	
			bShow = file->Read(_T("Active"), LYNX_TRUE, group);					
			bActive = file->Read(_T("Active"), LYNX_TRUE, group);					
			bClip = file->Read(_T("Clip"), LYNX_TRUE, group);
			BouncingSpeed = file->Read(_T("BouncingSpeed"), 1.0f, group);	
			RenderOrder = file->Read(_T("RenderOrder"), 0, group);	

			Flags = 0;
			CXMLElement* FlagsNode = group->FirstChildElement("Flags");	
			if (FlagsNode)
			{				
				CXMLElement* FlagNode = FlagsNode->FirstChildElement("Flag");	
				while (FlagNode)
				{
					Value = file->GetValue(FlagNode);						
					if (Value == CString("X_SCROLL"))
					{
						Flags |= GUISystem::CScrollGroup::X_SCROLL;
					}
					else if (Value == CString("Y_SCROLL"))
					{
						Flags |= GUISystem::CScrollGroup::Y_SCROLL;
					}
					FlagNode = FlagNode->NextSiblingElement("Flag");													
				}
			}
			Value = _T("");
			Value = file->Read(_T("Bounce"), Value , group);	

			lynxStrCpy(Buffer, Value.c_str());
			LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
			if (CString(Tok).find(CString(_T("TRUE"))) != CString::npos)
			{
				bBounceX = LYNX_TRUE;
			}
			else
			{
				bBounceX = LYNX_FALSE;
			}
			Tok = lynxStrTok(NULL, _T(","));					
			if (CString(Tok).find(CString(_T("TRUE"))) != CString::npos)
			{
				bBounceY = LYNX_TRUE;
			}
			else
			{
				bBounceY = LYNX_FALSE;
			}
			GUISystem::CScrollGroup* ScrollGroup = CreateScrollGroup(Name, ClipRect);
			ScrollGroup->SetScrollFlag(Flags);
			ScrollGroup->SetBouncing(bBounceX, bBounceY);
			ScrollGroup->SetPanVelocityScale(VelScale);	
			ScrollGroup->SetBouncingSpeed(BouncingSpeed);
			ScrollGroup->m_SnapSize = SnapSize * ScaleFactor.x;
			ScrollGroup->SetInitShow(bShow);
			ScrollGroup->Show(bShow);
			ScrollGroup->SetInitActiveState(bActive);
			ScrollGroup->Activate(bActive);
			ScrollGroup->m_bClip = bClip;
			ScrollGroup->SetRenderOrder(RenderOrder);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadButton(CUserProfile* file, CXMLElement* c)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			LYNXBOOL bActive, bMask, bUseSpriteText, bShowText, bShow;
			CString Class, Name, Lookup, Value, Group, File, HitSound, StepInSound, ID, InactiveFile, Font, Text, ApplyAnim;
			CVector2 Size(0.0f, 0.0f), Offset(0.0f, 0.0f);
			int GroupID = -1;
			float FontScale = m_DefaultFontScale;
			int RenderOrder = 0;
			LYNXCOLORRGBA FontOutlineColor = {255, 0, 0, 255 };
			int FontOutlineWidth = 3;
			LYNXBOOL bUseFonrOutline;

			Class = file->Read(_T("Class"), Class , c);						
			ID = file->Read(_T("ID"), ID , c);						
			Lookup = file->Read(_T("LookupID"), Lookup , c);						
			if (Lookup.empty())
			{
				Name = file->Read(_T("Name"), Name , c);						
			}
			else
			{
				Name = glpKernel->GetlpGame()->GetText(_T("LT_SYSTEM_MSG"), Lookup, _T("desc"));
			}			
			Group = file->Read(_T("Group"), Group , c);						
			GroupID = file->Read(_T("GroupID"), GroupID , c);						
			File = file->Read(_T("File"), File , c);			
			InactiveFile = file->Read(_T("InactiveFile"), InactiveFile , c);
			ApplyAnim = file->Read(_T("ApplyAnim"), ApplyAnim, c);
			Size = file->Read(_T("Size"), Size , c);						
			ClipRect = file->Read(_T("Rect"), ClipRect , c);						
			if (ClipRect.x2 == -1)
				ClipRect.x2 = ClipRect.x1 + Size.x;
			if (ClipRect.y2 == -1)
				ClipRect.y2 = ClipRect.y1 + Size.y;

			if (Size.x <= 0)
				Size.x = ClipRect.x2 - ClipRect.x1;
			if (Size.y <= 0)
				Size.y = ClipRect.y2 - ClipRect.y1;

			Offset = file->Read(_T("Offset"), Offset , c);						
			HitSound = file->Read(_T("Sound"), HitSound, c);					
			StepInSound = file->Read(_T("StepInSound"), StepInSound, c);					
			bActive = file->Read(_T("Active"), LYNX_TRUE, c);					
			bMask = file->Read(_T("Mask"), LYNX_FALSE, c);						
			bShowText = file->Read(_T("ShowText"), LYNX_TRUE, c);			
			Font = file->Read(_T("Font"), _T(""), c);
			FontScale = file->Read(_T("FontScale"), FontScale, c);
			bUseSpriteText = file->Read(_T("UseSpriteText"), LYNX_TRUE, c);
			RenderOrder = file->Read(_T("RenderOrder"), 0, c);	
			bShow = file->Read(_T("Show"), LYNX_TRUE, c);
			FontOutlineColor = file->Read(_T("FontOutlineColor"), FontOutlineColor, c);
			FontOutlineWidth = file->Read(_T("FontOutlineWidth"), 3, c);
			bUseFonrOutline = file->Read(_T("UseFontOutline"), LYNX_FALSE, c);

			int ControlID = GetDefinedControlID(ID);
			GUISystem::CButton* Button = AddButton(Class, File, ClipRect, Size, ControlID, Name, bUseSpriteText, bMask);
			if (!InactiveFile.empty())
			{
				Button->SetInactiveTexture(*(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), InactiveFile);  
			}
			Button->SetFont(m_lpEngine->GetFont(Font));
			Button->SetFontScale(FontScale);
			Button->Activate(bActive);
			Button->SetInitActiveState(bActive);
			Button->Show(bShow);
			Button->SetInitShowState(bShow);			
			Button->SetHitSound(glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(HitSound));			
			Button->SetRenderOrder(RenderOrder);			
			Button->ShowText(bShowText);
			Button->UseFontOutline(bUseFonrOutline);
			Button->SetFontOutlineColor(FontOutlineColor);
			Button->SetFontOutlineWidth(FontOutlineWidth);			
			Button->UpdateText(Name);

			GUISystem::CScrollGroup* ScrollGroup = FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(Button);
			if (GroupID >= 0)
			{
				Button->SetGroupID(GroupID);
			}
			Button->SetTransitionOffset(Offset);
			Button->SetStepInSound(StepInSound);
			//Button->SetStepOutSound(_T("UI_STEP_OUT"));

			if (!ApplyAnim.empty())
			{
				Button->ApplyAnimation(ApplyAnim);
				Button->PlayAnimation();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadStateButton(CUserProfile* file, CXMLElement* c)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			LYNXBOOL bActive, bMask, bShowText, bUseSpriteText, bShow;
			CString Class, Name, Lookup, Value, Group, File, PressedFile, HitSound, StepInSound, ID, InactiveFile, Font, ApplyAnim;
			CVector2 Size(0.0f, 0.0f), Offset(0.0f, 0.0f);
			LYNXCOLORRGBA FontColor = { 185, 185, 185, 255 }, PressedFontColor = { 228, 228, 228, 255 };
			int GroupID = -1;
			float FontScale = m_DefaultFontScale;
			int RenderOrder = 0;
			LYNXCOLORRGBA FontOutlineColor = { 255, 0, 0, 255 };
			int FontOutlineWidth = 3;
			LYNXBOOL bUseFonrOutline = LYNX_FALSE;

			Class = file->Read(_T("Class"), Class , c);						
			ID = file->Read(_T("ID"), ID , c);						
			Lookup = file->Read(_T("LookupID"), Lookup , c);						
			if (Lookup.empty())
			{
				Name = file->Read(_T("Name"), Name , c);						
			}
			else
			{
				Name = glpKernel->GetlpGame()->GetText(_T("LT_SYSTEM_MSG"), Lookup, _T("desc"));
			}			
			Group = file->Read(_T("Group"), Group , c);						
			GroupID = file->Read(_T("GroupID"), GroupID , c);						
			File = file->Read(_T("File"), File , c);									
			PressedFile = file->Read(_T("PressedFile"), File , c);			
			InactiveFile = file->Read(_T("InactiveFile"), InactiveFile , c);
			ApplyAnim = file->Read(_T("ApplyAnim"), ApplyAnim, c);
			Size = file->Read(_T("Size"), Size , c);						
			ClipRect = file->Read(_T("Rect"), ClipRect , c);						
			if (ClipRect.x2 == -1)
				ClipRect.x2 = ClipRect.x1 + Size.x;
			if (ClipRect.y2 == -1)
				ClipRect.y2 = ClipRect.y1 + Size.y;

			if (Size.x <= 0)
				Size.x = ClipRect.x2 - ClipRect.x1;
			if (Size.y <= 0)
				Size.y = ClipRect.y2 - ClipRect.y1;

			Offset = file->Read(_T("Offset"), Offset , c);						
			HitSound = file->Read(_T("Sound"), HitSound, c);					
			StepInSound = file->Read(_T("StepInSound"), StepInSound, c);					
			bActive = file->Read(_T("Active"), LYNX_TRUE, c);								
			bMask = file->Read(_T("Mask"), LYNX_FALSE, c);					
			bShowText = file->Read(_T("ShowText"), LYNX_TRUE, c);
			Font = file->Read(_T("Font"), _T(""), c);			
			FontScale = file->Read(_T("FontScale"), FontScale, c);
			bUseSpriteText = file->Read(_T("UseSpriteText"), LYNX_TRUE, c);
			RenderOrder = file->Read(_T("RenderOrder"), 0, c);	
			bShow = file->Read(_T("Show"), LYNX_TRUE, c);
			FontColor = file->Read(_T("FontColor"), FontColor, c);
			PressedFontColor = file->Read(_T("PressedFontColor"), PressedFontColor, c);

			int ControlID = GetDefinedControlID(ID);
			CStateButton* Button = AddStateButton(Class, File, PressedFile, ClipRect, Size, ControlID, Name, bUseSpriteText, NULL, bMask);
			Button->SetFontScale(FontScale);
			if (!InactiveFile.empty())
			{
				Button->SetInactiveTexture(*(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), InactiveFile);  
			}
			Button->Activate(bActive);
			Button->SetInitActiveState(bActive);
			Button->Show(bShow);
			Button->SetInitShowState(bShow);
			Button->SetFont(m_lpEngine->GetFont(Font));
			Button->ShowText(bShowText);			
			Button->SetHitSound(glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(HitSound));
			Button->SetRenderOrder(RenderOrder);

			GUISystem::CScrollGroup* ScrollGroup = FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(Button);
			if (GroupID >= 0)
			{
				Button->SetGroupID(GroupID);
			}
			Button->SetTransitionOffset(Offset);
			Button->SetStepInSound(StepInSound);
			Button->SetFontColor(FontColor);
			Button->SetPressedFontColor(PressedFontColor);
			Button->UseFontOutline(bUseFonrOutline);
			Button->SetFontOutlineColor(FontOutlineColor);
			Button->SetFontOutlineWidth(FontOutlineWidth);
			Button->UpdateText(Name);

			//Button->SetStepOutSound(_T("UI_STEP_OUT"));

			if (!ApplyAnim.empty())
			{
				Button->ApplyAnimation(ApplyAnim);
				Button->PlayAnimation();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadImage(CUserProfile* file, CXMLElement* c)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			CString Class, Name, Lookup, Value, Group, File, HitSound, StepInSound, ID, InactiveFile, ApplyAnim;
			CVector2 Size(0.0f, 0.0f), Offset(0.0f, 0.0f);
			LYNXBOOL bActive, bMask, bShowText, bShow;
			int RenderOrder = 0;
			float FontScale = m_DefaultFontScale;
			
			Class = file->Read(_T("Class"), Class , c);						
			ID = file->Read(_T("ID"), ID , c);						
			Lookup = file->Read(_T("LookupID"), Lookup , c);						
			if (Lookup.empty())
			{
				Name = file->Read(_T("Name"), Name , c);						
			}
			else
			{
				Name = glpKernel->GetlpGame()->GetText(_T("LT_SYSTEM_MSG"), Lookup, _T("desc"));
			}			
			Group = file->Read(_T("Group"), Group , c);						
			File = file->Read(_T("File"), File , c);						
			InactiveFile = file->Read(_T("InactiveFile"), InactiveFile , c);
			ApplyAnim = file->Read(_T("ApplyAnim"), ApplyAnim, c);
			Size = file->Read(_T("Size"), Size , c);						
			ClipRect = file->Read(_T("Rect"), ClipRect , c);						
			if (ClipRect.x2 == -1)
				ClipRect.x2 = ClipRect.x1 + Size.x;
			if (ClipRect.y2 == -1)
				ClipRect.y2 = ClipRect.y1 + Size.y;

			if (Size.x <= 0)
				Size.x = ClipRect.x2 - ClipRect.x1;
			if (Size.y <= 0)
				Size.y = ClipRect.y2 - ClipRect.y1;

			FontScale = file->Read(_T("FontScale"), FontScale, c);
			Offset = file->Read(_T("Offset"), Offset , c);						
			HitSound = file->Read(_T("Sound"), HitSound, c);
			StepInSound = file->Read(_T("StepInSound"), StepInSound, c);
			bActive = file->Read(_T("Active"), LYNX_TRUE, c);			
			bMask = file->Read(_T("Mask"), LYNX_FALSE, c);	
			bShowText = file->Read(_T("ShowText"), LYNX_FALSE, c);	
			RenderOrder = file->Read(_T("RenderOrder"), 0, c);	
			bShow = file->Read(_T("Show"), LYNX_TRUE, c);

			int ControlID = GetDefinedControlID(ID);
			GUISystem::CImage* Image = AddImage(Class, File, ClipRect, Size, ControlID, Name, LYNX_TRUE, bMask, -1, -1, bShowText);
			if (!InactiveFile.empty())
			{
				Image->SetInactiveTexture(*(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), InactiveFile);  
			}
			Image->SetName(Name);
			Image->SetFontScale(FontScale);
			Image->Activate(bActive);
			Image->ShowText(bShowText);
			Image->SetInitActiveState(bActive);
			Image->Show(bShow);
			Image->SetInitShowState(bShow);
			Image->SetHitSound(glpKernel->GetlpEngine()->GetlpSoundSystem()->FindSound(HitSound));
			Image->SetRenderOrder(RenderOrder);			
			
			GUISystem::CScrollGroup* ScrollGroup = FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(Image);
			Image->SetTransitionOffset(Offset);
			Image->SetStepInSound(StepInSound);
			//Image->SetStepOutSound(_T("UI_STEP_OUT"));

			if (!ApplyAnim.empty())
			{
				Image->ApplyAnimation(ApplyAnim);
				Image->PlayAnimation();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadText(CUserProfile* file, CXMLElement* c)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			float FontScale = m_DefaultFontScale;
			CString Class, Name, Text, Lookup, Value, Group, File, HitSound, ID, Font, ApplyAnim;
			CVector2 Size(0.0f, 0.0f), Offset(0.0f, 0.0f);
			LYNXBOOL bActive, bMask, bShowText, bUseFontShadow, bUseFonrOutline;
			LYNXCOLORRGBA FontColor = { 255, 255, 255, 255 }, FontShadowColor = { 0, 0, 0, 156 }, FontOutlineColor = { 255, 0, 0, 255 };
			int FontOutlineWidth = 3;
			int RenderOrder = 0;
			int VAlign, HAlign;
			CSpriteContainer::ORIGINTYPE Origin;
			
			Class = file->Read(_T("Class"), Class , c);						
			ID = file->Read(_T("ID"), ID , c);
            Name = file->Read(_T("Name"), Name , c);
			Lookup = file->Read(_T("LookupID"), Lookup , c);						
			if (Lookup.empty())
			{
				Text = file->Read(_T("Text"), Text, c);
			}
			else
			{
				Text = glpKernel->GetlpGame()->GetText(_T("LT_SYSTEM_MSG"), Lookup, _T("desc"));
			}
            Group = file->Read(_T("Group"), Group , c);
			Font = file->Read(_T("Font"), "" , c);			
			FontScale = file->Read(_T("FontScale"), m_DefaultFontScale , c);						
			FontColor = file->Read(_T("FontColor"), FontColor , c);
			FontShadowColor = file->Read(_T("FontShadowColor"), FontShadowColor , c);
			bUseFontShadow = file->Read(_T("UseFontShadow"), LYNX_TRUE , c);
			ClipRect = file->Read(_T("Rect"), ClipRect , c);									
			bActive = file->Read(_T("Active"), LYNX_TRUE, c);
			RenderOrder = file->Read(_T("RenderOrder"), 0, c);	
			Offset = file->Read(_T("Offset"), Offset , c);		
			Value = file->Read(_T("V-Align"), _T("V-Align-Center") , c);						
			VAlign = GetAligmentType(Value);
			Value = file->Read(_T("H-Align"), _T("H-Align-Center") , c);						
			HAlign = GetAligmentType(Value);
			Value = file->Read(_T("Origin"), _T("Center") , c);						
			Origin = GetOriginType(Value);
			ApplyAnim = file->Read(_T("ApplyAnim"), ApplyAnim, c);
			FontOutlineColor = file->Read(_T("FontOutlineColor"), FontOutlineColor, c);
			FontOutlineWidth = file->Read(_T("FontOutlineWidth"), FontOutlineWidth, c);
			bUseFonrOutline = file->Read(_T("UseFontOutline"), LYNX_FALSE, c);			

			int ControlID = -1;
			if (m_ParentPage)
			{
				ControlID = m_ParentPage->GetDefinedControlID(ID);
			}
			else if (m_Parent)
			{
				ControlID = ((CWindow*)m_Parent)->GetDefinedControlID(ID);
			}
			GUISystem::CText* cText = AddText(Class, Text, m_lpEngine->GetFont(Font), FontScale, FontColor, FontShadowColor, bUseFontShadow, ClipRect, ControlID, LYNX_TRUE, VAlign|HAlign);
			cText->SetName(Name);
			cText->SetFont(m_lpEngine->GetFont(Font));
            cText->SetFontScale(FontScale);
			cText->UseFontOutline(bUseFonrOutline);
			cText->SetFontOutlineColor(FontOutlineColor);
			cText->SetFontOutlineWidth(FontOutlineWidth);
			cText->Activate(bActive);
			cText->SetInitActiveState(bActive);
			cText->SetRenderOrder(RenderOrder);			
			cText->vSetAlignmentType(VAlign|HAlign);
			cText->vSetOriginType(Origin);
			cText->SetText(Text);

			GUISystem::CScrollGroup* ScrollGroup = FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(cText);
			cText->SetTransitionOffset(Offset);

			cText->ApplyAnimation(ApplyAnim);
			cText->PlayAnimation();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadWindow(CUserProfile* file, CXMLElement* c)
		{
			LYNXRECT ClipRect;			
			LYNXCHAR Buffer[256];
			CString Class, Name, Layout, Group;
			CVector2 Size, Offset(0.0f, 0.0f);
			LYNXBOOL bActive;
			
			Class = file->Read(_T("Class"), Class , c);						
			Layout = file->Read(_T("Layout"), Layout , c);						
			Group = file->Read(_T("Group"), Group , c);		
			Offset = file->Read(_T("Offset"), Offset , c);				
			
			if (Class == CString("CDialog"))
			{
				GUISystem::CDialog* Dialog = LYNXNEW CDialog(m_lpEngine->GetlpGUISystem());
				Dialog->vLoadLayout(CString("../data/layout/") + Layout);
				GUISystem::CScrollGroup* ScrollGroup = FindScrollGroup(Group);
				if (ScrollGroup)
					ScrollGroup->AddControl(Dialog);

				AddControl(Dialog);
				Dialog->SetTransitionOffset(Offset);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CContext::Activate(LYNXBOOL b)
		{
			for (CList<GUISystem::CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				if (!(*Control)->GetContext())
					(*Control)->Activate(b);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CContext::Show(LYNXBOOL b)
		{
			for (CList<GUISystem::CControl*>::CIterator Control = m_ControlList.begin(); Control != m_ControlList.end(); ++Control)
			{
				if (!(*Control)->GetContext())
					(*Control)->Show(b);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CContext::vInit()
		{
			CList<CScrollGroup*>::CIterator Pos;
            
			for (Pos = m_ScrollGroupList.begin(); Pos != m_ScrollGroupList.end(); Pos++)
			{
				(*Pos)->ResetShow();
				(*Pos)->ResetActiveState();
			}
			return LYNX_TRUE;
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadControl(CUserProfile* file, CXMLElement* c)
		{
			CString Type;

			Type = file->Read(_T("Type"), Type , c);						

			if (Type == CString(_T("Button")))
			{
				LoadButton(file, c);
			}			
			else if (Type == CString(_T("StateButton")))
			{
				LoadStateButton(file, c);
			}
			else if (Type == CString(_T("Image")))
			{
				LoadImage(file, c);
			}
			else if (Type == CString(_T("Text")))
			{
				LoadText(file, c);
			}
			else if (Type == CString(_T("Window")))
			{
				LoadWindow(file, c);
			}
			else
			{				
				CString Class;
				
				Class = file->Read(_T("Class"), Class , c);						
				CControl* Control = m_lpEngine->GetlpGUISystem()->CreateControl(Class);
				if (Control)
				{
					AddControl(Control);
					Control->vCreateFromLayout(file, c, this);									
					Control->vInit();
					//Control->SetStepInSound(_T("UI_STEP_IN"));
					//Control->SetStepOutSound(_T("UI_STEP_OUT"));
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CContext::LoadAnimation(CUserProfile* file, CXMLElement* c)
		{
			m_AnimationSet->ReleaseAllAnimations();
			m_AnimationSet->vLoad(file, c);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::AddStepOutControl(CControl* c)
		{
			m_OutControlList.push_back(c);
			c->TransitBack();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::AddStepOutGroup(CScrollGroup* g)
		{
			g->SetupContextStepOutList(NULL);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::AddStepInControl(CControl* c)
		{
			m_InControlList.push_back(c);
			c->TransitStop();
			c->SetState(GUISystem::CControl::TS_STEP_IN);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::AddStepInGroup(CScrollGroup* g)
		{
			g->SetupContextStepIntList(NULL);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::InitStepOut()
		{
			m_OutControlList.clear();
			m_InControlList.clear();
			m_State = STATE_GROUP_STEP_OUT;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  a︷
		//-------------------------------------------------------------------------------------------------------
		void CContext::StepOutGroup(CScrollGroup* out, CArray<WORD>* out_ex_list, CScrollGroup* in, CArray<WORD>* in_ex_list)
		{
			if (out)
			{
				out->SetupContextStepOutList(out_ex_list);		
				//out->Activate(LYNX_FALSE);
			}
			
			if (in)
			{
				in->SetupContextStepIntList(in_ex_list);
				//in->Activate(LYNX_TRUE);
			}

			if (!m_OutControlList.empty())
			{
				if (!m_InControlList.empty())
				{
					ShowControlArray(m_InControlList, LYNX_FALSE);
					ActivateControlArray(m_InControlList, LYNX_FALSE);				
				}
				m_State = STATE_GROUP_STEP_OUT;
			}
			else
			{
				if (!m_InControlList.empty())
				{
					ShowControlArray(m_InControlList, LYNX_TRUE);
					ResetControlArrayActiveState(m_InControlList);
					for (int i=0; i<m_InControlList.size(); i++)
					{
						m_InControlList[i]->Transit();
					}					
					m_State = STATE_GROUP_STEP_IN;
				}				
			}
		}
	}
}
