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

#ifndef __LYNXSTATE_H__
#define __LYNXSTATE_H__

#include <LynxObj.h>

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
		class LYNXENGCLASS CState : public CObj
		{	
			LYNX_DECLARE_CLASS(CState);
		protected:												
			CStateMachine*						m_lpStateMachine;
			Animation::CAnimation*				m_lpAnimation;
			CList<CStateTranslation*>			m_TranslationList;
			CStateTranslation*					m_lpEventForAnimationStop;
		public:			
			CState(CStateMachine* sm);			
			~CState();		
			
			LYNXINLINE void						SetlpStateMachine(CStateMachine* lps) {m_lpStateMachine = lps; };		
			LYNXINLINE CStateMachine*			GetlpStateMachine() {return m_lpStateMachine; };
			LYNXINLINE void						SetlpAnimation(LynxEngine::Animation::CAnimation* lpa) {m_lpAnimation = lpa; };		
			LYNXINLINE LynxEngine::Animation::CAnimation* GetlpAnimation() {return m_lpAnimation; };
			LYNXINLINE void						ClearTranslations() {m_TranslationList.clear(); };
			void								AddTranslation(CStateTranslation* t);
			LYNXINLINE DWORD					GetNumTranslations() {return (DWORD)m_TranslationList.size(); };
			LYNXINLINE CStateTranslation*		GetTranslation(int i) {return m_TranslationList[i]; };
			LYNXINLINE CList<CStateTranslation*>& GetTranslationList() {return m_TranslationList; };
			CStateTranslation*					FindTranslation(const CString& name);

			LYNXINLINE void						SetEventForAnimationStop(CStateTranslation* t) {m_lpEventForAnimationStop = t; };
			LYNXINLINE CStateTranslation*		GetEventForAnimationStop() {return m_lpEventForAnimationStop; };

			LYNXBOOL							Translate(const CEvent& input);
		};	
	}
}
#endif

