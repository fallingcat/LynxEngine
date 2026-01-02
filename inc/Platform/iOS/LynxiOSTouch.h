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

#ifndef __LYNXIOSTOUCH_H__
#define __LYNXIOSTOUCH_H__

#include <InputSystem/LynxTouch.h>

LYNX_FORWARD_DECLARATION(NSSet)

namespace LynxEngine 
{
	namespace InputSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CTouch : public LynxEngine::InputSystem::CTouch
			{
				LYNX_DECLARE_CLASS(CTouch);
				
            protected:
				NSSet*                          m_Touches;
            public:						
				CTouch(CInputSystem* lpsys);
				~CTouch();		
                
                LYNXFORCEINLINE void			SetTouches(NSSet* t) {m_Touches = t; };
				virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
				virtual void					vPoll();
			};
		}
	}
}

#endif
