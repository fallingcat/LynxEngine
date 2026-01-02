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
#include <LynxEvent.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateTranslation.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(StateMachine::CStateTranslation::TRANSLATIONMETHOD e)
	{
		CString				Translation;

		switch (e)
		{
			case StateMachine::CStateTranslation::INSTANT:
				Translation = _T("Instant");
				break;
			case StateMachine::CStateTranslation::QUEUE:
				Translation = _T("Queue");
				break;
		}
		return Translation;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(StateMachine::CStateTranslation::TRANSLATIONTYPE t)
	{
		CString				Translation;

		switch (t)
		{
			case StateMachine::CStateTranslation::USERINPUT_TRANSLATION:
				Translation = _T("User Input");
				break;
			case StateMachine::CStateTranslation::EVENT_TRANSLATION:
				Translation = _T("Event");
				break;
			case StateMachine::CStateTranslation::RANDOM_TRANSLATION:
				Translation = _T("Random");
				break;
		}
		return Translation;
	}
	namespace StateMachine
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateTranslation::CStateTranslation(CStateMachine* sm)
		{
			m_lpStateMachine = sm;
			m_Method = INSTANT;
			m_lpInput = NULL;
			m_OutputList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateTranslation::~CStateTranslation(void)
		{
			m_OutputList.clear();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateTranslation::vMatch(const CEvent& input)
		{
			if (input.GetName() == GetName())
				return LYNX_TRUE;
			
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState* CStateTranslation::vTranslate(const CEvent& input)
		{
			if (vMatch(input))
			{
				if (m_OutputList.size())
					return *(m_OutputList.begin());
			}
			return NULL;
		}
	}
}	