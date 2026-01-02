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
#include <InputSystem/LynxTeslameter.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
        IMPLEMENT_CLASSSCRIPT(CTeslameter, CInputDevice)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTeslameter::CTeslameter(CInputSystem* lpsys)
		: CInputDevice(lpsys)
		{
			m_InputDeviceType = GYROMETER;
			SetName(_T("System Teslameter"));
            
            INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTeslameter::~CTeslameter(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CTeslameter::vPoll(void)
		{									
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTeslameter::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			//REGISTER_SCRIPT_VAR(lps, "m_Gesture",		LynxEngine::InputSystem::CTeslameter, m_Gesture)						
			//REGISTER_SCRIPT_VAR(lps, "m_TouchData",		LynxEngine::InputSystem::CTeslameter, m_TouchData)			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTeslameter::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

            //REGISTER_SCRIPT_METHOD(lps, "GetNumTouches",    LynxEngine::InputSystem::CTeslameter, int,		GetNumTouches,			())
			//REGISTER_SCRIPT_METHOD(lps, "PollTouchData",	LynxEngine::InputSystem::CTeslameter, void,		PollTouchData,			(int))
			
			return LYNX_TRUE;
		}
	}
}
