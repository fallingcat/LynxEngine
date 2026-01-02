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

#ifndef __LYNXINPUTRECORD_H__
#define __LYNXINPUTRECORD_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CInputRecord : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CInputRecord);
		public:
			typedef struct {
				int							ButtonStatus;
				LYNXPOINT2D					CursorPos;		
			}MOUSEINPUTDATA;
		protected:
			int								m_FrameOffset;
			CMap<int, MOUSEINPUTDATA>		m_InputMap;			
		protected:
			CInputRecord();
		public:						
			CInputRecord(CInputSystem* lpsys);
			~CInputRecord();			

			DECLARE_CLASSSCRIPT
			//LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			//LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
			//void							Poll();
			//void							SetPollData(LYNXPOINT2D& pos, int mask);
			//void							ClearPollData(LYNXBOOL bclearqueued = LYNX_FALSE);
			//LYNXBOOL						ButtonStatus(int mask);			

			//LYNXINLINE LYNXPOINT2D			GetPos() const { return m_CursorPos; };
			//LYNXINLINE void					GetPos(LYNXPOINT2D* pos) const { pos->x = m_CursorPos.x; pos->y = m_CursorPos.y; };
			//LYNXINLINE void					SetPos(const int x, const int y) { lynxSetCursorPos(x, y); };
			virtual LYNXBOOL				vCreate();
			virtual void					vPoll();

			void							StartRecord();
			void							EndRecord(const CString& filename);
			void							Record();
			void							StartPlayback(const CString& filename);
			void							Playback();
		};	
	}
}

#endif