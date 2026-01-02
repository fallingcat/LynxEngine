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

#ifndef __LYNXINPUTDEVICE_H__
#define __LYNXINPUTDEVICE_H__

#include <LynxObj.h>
#include <InputSystem/LynxInputSystem.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CInputDevice : public CObj
		{
			LYNX_DECLARE_CLASS(CInputDevice);
		public:	
			typedef enum {
				PAD_UP,
				PAD_DOWN,
				PAD_RIGHT,
				PAD_LEFT,
				JOYSTICK_MOVEMENT,
			}INPUTMESSAGETYPE;
		protected:
			CInputSystem*						m_lpInputSystem;
			INPUTDEVICETYPE						m_InputDeviceType;
			LYNXBOOL							m_bEnable;

		protected:
			CInputDevice();
		public:						
			CInputDevice(CInputSystem* lpsys);
			~CInputDevice();

			DECLARE_CLASSSCRIPT

			const INPUTDEVICETYPE				GetInputDeviceType() const {return m_InputDeviceType;}; 
			LYNXINLINE virtual void				vEnableHitTest(LYNXBOOL b) {m_bEnable = b; };
			LYNXINLINE LYNXBOOL					IsEnabled() const {return m_bEnable; };
			virtual LYNXBOOL					vCreate() = 0;
			virtual	void						vPoll() = 0;	
		};	
	}
}

#endif