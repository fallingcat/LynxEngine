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

#ifndef __LYNXINPUTCOMMAND_H__
#define __LYNXINPUTCOMMAND_H__

#include <LynxSystem.h>
#include <LynxFactory.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CInputCommand : public CUnknown
		{
			LYNX_DECLARE_CLASS(CInputCommand);
		public:						
			typedef enum {
				ICT_NONE = -1,
				ICT_LEFT,
				ICT_RIGHT,
				ICT_UP,
				ICT_DOWN,
				ICT_JOYSTICK_1,
				ICT_JOYSTICK_2,
				ICT_START,
				ICT_OPTION,
				ICT_BUTTON_A,
				ICT_BUTTON_B,
				ICT_BUTTON_C,
				ICT_BUTTON_D,
				ICT_BUTTON_E,
				ICT_BUTTON_F,
				ICT_TRIGGER_A,
				ICT_TRIGGER_B,
				ICT_TRIGGER_C,
				ICT_TRIGGER_D,
				ICT_TRIGGER_E,
				ICT_TRIGGER_F,
				NUM_INPUTCOMMSNDTYPE,
			}INPUTCOMMANDTYPE;
		protected:
			INPUTCOMMANDTYPE				m_CommandType;			
			Math::CVector3					m_Position;
		public:			
			CInputCommand();
			~CInputCommand();
		};	
	}
}

#endif