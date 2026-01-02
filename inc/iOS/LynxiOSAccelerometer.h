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

#ifndef __LYNXACCELEROMETER_H__
#define __LYNXACCELEROMETER_H__

#include <InputSystem/LynxInputDevice.h>

class NSSet;

namespace LynxEngine 
{
	namespace InputSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CAccelerometer : public CInputDevice
			{
				LYNX_DECLARE_CLASS(CAccelerometer);
			protected:
				Math::CReal						m_HighPassFactor;	
				Math::CVector3					m_Value;
				Math::CVector3					m_HighPassValue;
			public:						
				CAccelerometer(CInputSystem* lpsys);
				~CAccelerometer();			
			
				LYNXFORCEINLINE void			SetHighPassFactor(Math::CReal& f) {m_HighPassFactor = f; };
				void							SetValue(const Math::CVector3& v);
				LYNXFORCEINLINE Math::CVector3	GetValue() {return m_Value; };
				LYNXFORCEINLINE Math::CVector3	GetHighPassValue() {return m_HighPassValue; };
				LYNXFORCEINLINE void			Clear() {m_Value.x = m_Value.y = m_Value.z = 0.0f; m_HighPassValue.x = m_HighPassValue.y = m_HighPassValue.z = 0.0f; };
				virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
				virtual void					vPoll();
			};
		}
	}
}

#endif