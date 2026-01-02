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
#include <StateMachine/LynxRandTranslation.h>

namespace LynxEngine 
{	
	namespace StateMachine
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRandTranslation::CRandTranslation(CStateMachine* sm)
		: CStateTranslation(sm)
		{
			m_TranslationType = RANDOM_TRANSLATION;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRandTranslation::~CRandTranslation(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState* CRandTranslation::vTranslate(const CString& input)
		{
			if (input == GetName())
			{
				int Size = (int)m_OutputList.size();
				int Result = (int)(rand()%m_OutputList.size());
				return (m_OutputList[Result]);
			}
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState* CRandTranslation::vTranslate(const CEvent& input)
		{
			if (input.GetName() == GetName())
			{
				int Size = (int)m_OutputList.size();
				int Result = (int)(rand()%m_OutputList.size());
				return (m_OutputList[Result]);
			}
			return NULL;
		}
	}
}	