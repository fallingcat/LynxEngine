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
#include <LynxCore.h>
#include <LynxAlgorithm.h>
#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateTranslation.h>
#include <StateMachine/LynxRandTranslation.h>

namespace LynxEngine 
{	
	namespace StateMachine
	{
		CEvent CStateMachine::m_AnimationStopEvent;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateMachine::CStateMachine(void)
		{	
			m_StateList.clear();			
			m_lpLastState = m_lpCurrentState = NULL;	
			m_lpQueuedTranslation = NULL;		

			m_AnimationStopEvent.SetType(CEvent::ANIMATIONSTOP);
			m_AnimationStopEvent.SetName(_T("AnimationStop"));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateMachine::~CStateMachine(void)
		{
			CList<CState*>::CIterator Temp;
			CList<CState*>::CIterator State = m_StateList.begin();

			while (State != m_StateList.end())
			{				
				Temp = State;
				++Temp;
				ReleaseState((*State));				
				State = Temp;			
			}
			m_StateList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		const CEvent& CStateMachine::GetAnimationStopEvent() 
		{
			return m_AnimationStopEvent; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CStateMachine::GetCurrentStateID(void) 
		{
			return m_lpCurrentState->GetID();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		const CString& CStateMachine::GetCurrentStateName(void) 
		{
			return m_lpCurrentState->GetName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState*	CStateMachine::CreateState()
		{
			return LYNXNEW CState(this);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStateMachine::ReleaseState(CState* s)
		{
			LYNXDEL s;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateTranslation* CStateMachine::CreateStateTranslation(CStateTranslation::TRANSLATIONTYPE t)
		{
			switch (t)
			{
				case CStateTranslation::RANDOM_TRANSLATION:
					return LYNXNEW CRandTranslation(this);
					break;

				default:
					return LYNXNEW CStateTranslation(this);
					break;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStateMachine::ReleaseStateTranslation(CStateTranslation* t)
		{
			LYNXDEL t;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStateMachine::AddState(CState* s)
		{
			s->SetlpStateMachine(this);
			m_StateList.push_back(s);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState* const CStateMachine::FindState(CString& name)
		{
			LynxEngine::CList<CState*>::CIterator State;	
			State = LynxEngine::find(m_StateList.begin(), m_StateList.end(), name);
			if (State != m_StateList.end())
				return (*State);
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CState* const CStateMachine::FindState(const LYNXCHAR* name)
		{
			CString Name = name;
			return FindState(Name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateMachine::SetCurrentState(CState *lpstate) 
		{
			if (m_lpCurrentState != lpstate)
			{
				m_lpLastState = m_lpCurrentState;
				m_lpCurrentState = lpstate; 				
				LYNXDEL m_lpQueuedTranslation;
				m_lpQueuedTranslation = NULL;

				return LYNX_TRUE;				
			}
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateMachine::SetCurrentState(const CString& name)
		{
			CList<CState*>::CIterator Pos;

			if (m_lpCurrentState)
			{
				if (m_lpCurrentState->GetName() == name)				
					return LYNX_FALSE;
			}
			Pos = LynxEngine::find(m_StateList.begin(), m_StateList.end(), name);
			if (Pos != m_StateList.end())
			{
				m_lpLastState = m_lpCurrentState;
				SetCurrentState((*Pos));
				return LYNX_TRUE;
			}						
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateMachine::SetCurrentState(int id)
		{
			CList<CState*>::CIterator Pos;

			if (m_lpCurrentState)
			{
				if (m_lpCurrentState->GetID() == id)				
					return LYNX_FALSE;
			}
			for (Pos = m_StateList.begin(); Pos != m_StateList.end(); ++Pos)
			{
				if ((*Pos)->GetID() == id)
				{
					m_lpLastState = m_lpCurrentState;
					SetCurrentState((*Pos));
					return LYNX_TRUE;
				}
			}
			return LYNX_FALSE;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStateMachine::QueueState(CState *lpstate)
		{
			if (!m_lpQueuedTranslation)
			{						
				m_lpQueuedTranslation = LYNXNEW CStateTranslation(this);
				m_lpQueuedTranslation->SetName(CString(_T("AnimationStop")));
				m_lpQueuedTranslation->AddOutput(lpstate);
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStateMachine::Update(const CEvent& input)
		{		
			CState* lpRes;	

			if (m_lpQueuedTranslation)
			{
				lpRes = m_lpQueuedTranslation->vTranslate(input);
				return SetCurrentState(lpRes);
			}
			else
			{
				if (m_lpCurrentState)
					return m_lpCurrentState->Translate(input);			
			}
			return LYNX_FALSE;
		}
	}
}	