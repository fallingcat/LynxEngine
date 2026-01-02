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
//  :
//
//###########################################################################
#ifndef __LYNXIOSGYROMETER_H__
#define __LYNXIOSGYROMETER_H__

#include <InputSystem/LynxGyrometer.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CGyrometer : public LynxEngine::InputSystem::CGyrometer
			{
				LYNX_DECLARE_CLASS(CGyrometer);
			protected:
                //CMMotionManager*                m_MotionManager; 
			public:						
				CGyrometer(CInputSystem* lpsys);
				~CGyrometer();			
			
				virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
				virtual void					vPoll();
			};
		}
	}
}

#endif
