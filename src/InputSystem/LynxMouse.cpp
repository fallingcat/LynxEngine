//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <InputSystem/LynxMouse.h>

namespace LynxEngine
{
	namespace InputSystem
	{
		IMPLEMENT_CLASSSCRIPT(CMouse, CInputDevice)
			//-------------------------------------------------------------------------------------------------------
			//
			//  :   
			//-------------------------------------------------------------------------------------------------------
			CMouse::CMouse(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMouse::CMouse(CInputSystem* lpsys)
			: CInputDevice(lpsys)
		{
			m_InputDeviceType = MOUSE;
			SetName(_T("System Mouse"));
			m_ButtonStatus = 0;
			
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMouse::~CMouse(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMouse::SetPollData(LYNXPOINT2D& pos, int mask)
		{
			m_ButtonStatus = mask;
			m_CursorPos = pos;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMouse::vPoll(void)
		{
#ifdef __WEB__
#else
			if (m_lpInputSystem->IsAllInputBlocked())
			{
				m_ButtonStatus = 0;
			}
			else
			{
#if defined(__WXWIDGET__)
				lynxGetMouseCursorState(&m_CursorPos, m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow());
#else					
				lynxGetClientCursorPos(&m_CursorPos, m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow());
#endif
				m_ButtonStatus = 0;
				/*
				if (m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_LBUTTON] || lynxGetKeyState(VK_LBUTTON))
				m_ButtonStatus |= LBUTTON;
				if (m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_MBUTTON] || lynxGetKeyState(VK_MBUTTON))
				m_ButtonStatus |= MBUTTON;
				if (m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_RBUTTON] || lynxGetKeyState(VK_RBUTTON))
				m_ButtonStatus |= RBUTTON;
				*/

				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_LBUTTON))
				{
					m_ButtonStatus |= LBUTTON;
				}
				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_MBUTTON))
				{
					m_ButtonStatus |= MBUTTON;
				}
				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_RBUTTON))
				{
					m_ButtonStatus |= RBUTTON;
				}
			}
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMouse::Poll(void)
		{
			vPoll();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CMouse::ClearPollData(LYNXBOOL bclearqueued)
		{
			m_ButtonStatus = 0;
			if (bclearqueued)
			{
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_LBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_MBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_RBUTTON] = LYNX_FALSE;			

				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBDUP[VK_LBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBDUP[VK_MBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBDUP[VK_RBUTTON] = LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMouse::ButtonUpStatus(int mask)
		{
			/*
			if (m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBDUP[mask])
			{
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBDUP[mask] = LYNX_FALSE;
				return LYNX_TRUE;
			}
			return LYNX_FALSE;
			*/

			return (m_ButtonStatus & mask) ? LYNX_TRUE : LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMouse::ButtonStatus(int mask)
		{
			return (m_ButtonStatus & mask) ? LYNX_TRUE : LYNX_FALSE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMouse::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			REGISTER_SCRIPT_VAR(lps, "m_CursorPos", LynxEngine::InputSystem::CMouse, m_CursorPos)
			REGISTER_SCRIPT_VAR(lps, "m_ButtonStatus", LynxEngine::InputSystem::CMouse, m_ButtonStatus)

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMouse::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "Poll", LynxEngine::InputSystem::CMouse, void, Poll, (void))
			REGISTER_SCRIPT_METHOD(lps, "ButtonStatus", LynxEngine::InputSystem::CMouse, LYNXBOOL, ButtonStatus, (int))

			return LYNX_TRUE;
		}
	}
}
