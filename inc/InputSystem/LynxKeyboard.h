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
//  說明:
//
//###########################################################################

#ifndef __LYNXKEYBOARD_H__
#define __LYNXKEYBOARD_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CKeyboard : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CKeyboard);
		public:
			LYNXBOOL						m_bCheckingFocus;
		protected:
			CMap<int, LYNXBOOL>				m_KeyMap;
		protected:
			CKeyboard();
		public:						
			CKeyboard(CInputSystem* lpsys);
			~CKeyboard();			

			DECLARE_CLASSSCRIPT
			//LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
			void							Poll(LYNXBOOL bfocus_checking = LYNX_TRUE);

			void							AddKey(int k) { m_KeyMap.insert(CMap<int, LYNXBOOL>::value_type(k, LYNX_FALSE)); };
			LYNXINLINE LYNXBOOL				Key(int k) { return (m_KeyMap.find(k) == m_KeyMap.end()) ? LYNX_FALSE : m_KeyMap.find(k)->second; };
			LYNXINLINE void					SetKey(int k, LYNXBOOL b) { if (m_KeyMap.find(k) != m_KeyMap.end()) m_KeyMap.find(k)->second = b; };
			virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
			virtual void					vPoll();
		protected:			
		};	
	}
}

#endif