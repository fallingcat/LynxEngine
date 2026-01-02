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
#include <LynxAlgorithm.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxStateTranslation.h>

namespace LynxEngine 
{	
	namespace StateMachine
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState::CState(CStateMachine* sm)
		{			
			m_lpStateMachine = sm;
			m_TranslationList.clear();
			m_lpEventForAnimationStop = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState::~CState(void)
		{
			CList<CStateTranslation*>::CIterator Temp;
			CList<CStateTranslation*>::CIterator Trans = m_TranslationList.begin();

			while (Trans != m_TranslationList.end())
			{				
				Temp = Trans;
				++Temp;
				m_lpStateMachine->ReleaseStateTranslation((*Trans));				
				Trans = Temp;			
			}
			m_TranslationList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CState::AddTranslation(CStateTranslation* t)
		{
			m_TranslationList.push_back(t);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateTranslation* CState::FindTranslation(const CString& name)
		{
			CList<CStateTranslation*>::CIterator Pos;

			Pos = LynxEngine::find(m_TranslationList.begin(), m_TranslationList.end(), name);
			if (Pos != m_TranslationList.end())
				return (*Pos);

			return NULL;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CState::Translate(const CEvent& input)
		{
			CList<CStateTranslation*>::CIterator Pos;
			CState* lpRes;			

			for (Pos = m_TranslationList.begin(); Pos != m_TranslationList.end(); ++Pos)
			{	
				lpRes = (*Pos)->vTranslate(input);
                if (lpRes)
				{					
					switch ((*Pos)->m_Method)
					{
						case CStateTranslation::INSTANT:							
							return m_lpStateMachine->SetCurrentState(lpRes);
							break;

						case CStateTranslation::QUEUE:
							m_lpStateMachine->QueueState(lpRes);
							return LYNX_FALSE;
							break;
					}
					break;
				}
			}			
			return LYNX_FALSE;
		}
	}
}	