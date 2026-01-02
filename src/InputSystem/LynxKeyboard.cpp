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
#include <InputSystem/LynxKeyboard.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CKeyboard, CInputDevice)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CKeyboard::CKeyboard(void)
		{
			m_bCheckingFocus = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CKeyboard::CKeyboard(CInputSystem* lpsys)
		: CInputDevice(lpsys)
		{
			m_InputDeviceType = KEYBOARD;
			SetName(_T("System Keyboard"));

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CKeyboard::~CKeyboard(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CKeyboard::vPoll(void)
		{
			if (!m_lpInputSystem->IsAllInputBlocked())
			{
				Poll(m_bCheckingFocus);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CKeyboard::Poll(LYNXBOOL bfocus_checking)
		{
			#ifdef __WEB__
			#else
				LYNXWINDOW Wnd;
				CMap<int, LYNXBOOL>::CIterator Key;

				if (bfocus_checking)
					Wnd = m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow();
				else
					Wnd = NULL;

				for (Key = m_KeyMap.begin(); Key != m_KeyMap.end(); ++Key)
				{
					Key->second = lynxGetKeyStatus(Wnd, Key->first);
				}
			#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CKeyboard::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "Poll",			LynxEngine::InputSystem::CKeyboard, void,		Poll,			(void))
			REGISTER_SCRIPT_METHOD(lps, "Key",			LynxEngine::InputSystem::CKeyboard, LYNXBOOL,	Key,			(int))

			return LYNX_TRUE;
		}
	}
}
