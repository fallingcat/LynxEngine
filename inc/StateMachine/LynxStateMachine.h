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

#ifndef __LYNXSTATEMACHINE_H__
#define __LYNXSTATEMACHINE_H__

#include <LynxObj.h>
#include <LynxEvent.h>
#include <StateMachine/LynxStateTranslation.h>

namespace LynxEngine 
{	
	namespace StateMachine
	{
		/**	
		*  @brief State Machine 機制中的State表現。
		* 
		*	State Machine 機制中的State表現。
		*	
		*/
		class LYNXENGCLASS CStateMachine : public CObj
		{
			LYNX_DECLARE_CLASS(CStateMachine);
		public:
		protected:								
			CList<CState*>				m_StateList;
			CState*						m_lpCurrentState;	
			CState*						m_lpLastState;				
			CStateTranslation*			m_lpQueuedTranslation;				
			static CEvent				m_AnimationStopEvent;
		public:
			CStateMachine();			
			~CStateMachine();		

			CState*						CreateState();
			static void					ReleaseState(CState* s);
			CStateTranslation*			CreateStateTranslation(CStateTranslation::TRANSLATIONTYPE t);
			static void					ReleaseStateTranslation(CStateTranslation* t);
			void						AddState(CState* s);
			CState* const				FindState(CString& name);
			CState* const				FindState(const LYNXCHAR* name);
			LYNXINLINE DWORD			GetNumStates() {return (DWORD)m_StateList.size(); };	
			LYNXINLINE CState*			GetState(int i) {return m_StateList[i]; };	
			LYNXINLINE CState* const	GetlpLastState(void) {return m_lpLastState;};
			LYNXINLINE CState* const	GetlpCurrentState(void) {return m_lpCurrentState;};
			LYNXINLINE int				GetCurrentStateID(void);
			LYNXINLINE const CString&	GetCurrentStateName(void);
			LYNXINLINE LYNXBOOL			StateChange(void) {return (m_lpCurrentState == m_lpLastState) ? LYNX_FALSE : LYNX_TRUE;};
			LYNXBOOL					SetCurrentState(CState *lpstate);
			LYNXBOOL					SetCurrentState(const CString& name);
			LYNXBOOL					SetCurrentState(int id);
			static const CEvent&		GetAnimationStopEvent();
			void						QueueState(CState *lpstate);
			LYNXBOOL					Update(const CEvent& input);
		};			
	}	
}
#endif

