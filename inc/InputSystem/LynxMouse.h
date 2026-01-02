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

#ifndef __LYNXMOUSE_H__
#define __LYNXMOUSE_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine
{
	namespace InputSystem
	{
		class LYNXENGCLASS CMouse : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CMouse);
		public:
			static const int				LBUTTON = 1;
			static const int				MBUTTON = 2;
			static const int				RBUTTON = 4;

		protected:
			int								m_ButtonStatus;
			LYNXPOINT2D						m_CursorPos;			
		protected:
			CMouse();
		public:
			CMouse(CInputSystem* lpsys);
			~CMouse();

			DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void							Poll();
			void							SetPollData(LYNXPOINT2D& pos, int mask);
			void							ClearPollData(LYNXBOOL bclearqueued = LYNX_FALSE);
			LYNXBOOL						ButtonStatus(int mask);
			LYNXBOOL						ButtonUpStatus(int mask);

			LYNXINLINE int					GetButtonStatus() const { return m_ButtonStatus; };
			LYNXINLINE LYNXPOINT2D			GetPos() const { return m_CursorPos; };
			LYNXINLINE void					GetPos(LYNXPOINT2D* pos) const { pos->x = m_CursorPos.x; pos->y = m_CursorPos.y; };
			LYNXINLINE void					SetPos(const int x, const int y) { lynxSetCursorPos(x, y); };
			virtual LYNXBOOL				vCreate() { return LYNX_TRUE; };
			virtual void					vPoll();
		};
	}
}

#endif