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

#ifndef __LYNXSTATETRNSLATION_H__
#define __LYNXSTATETRNSLATION_H__

#include <LynxObj.h>

namespace LynxEngine 
{	
	namespace StateMachine
	{
		/**	
		*  @brief State Machine 機制中的State 轉換條件。
		* 
		*  State Machine 機制中的State 轉換條件。
		*	
		*/
		class LYNXENGCLASS CStateTranslation : public CObj
		{
			LYNX_DECLARE_CLASS(CStateTranslation);
		public:
			typedef enum {
				INSTANT = 0,
				QUEUE,			
				NUM_TRANSLATIONMETHOD,
			}TRANSLATIONMETHOD;

			typedef enum {
				USERINPUT_TRANSLATION = 0,
				EVENT_TRANSLATION,
				RANDOM_TRANSLATION,
				NUM_TRANSLATIONTYPE,
			}TRANSLATIONTYPE;
		
			TRANSLATIONMETHOD				m_Method;
		protected:						
			CStateMachine*					m_lpStateMachine;
			TRANSLATIONTYPE					m_TranslationType;
			CState*							m_lpInput;
			CList<CState*>					m_OutputList;							

		public:			
			CStateTranslation(CStateMachine* sm);			
			~CStateTranslation();		

			LYNXINLINE TRANSLATIONTYPE		GetTranslationType() {return m_TranslationType; };
			LYNXINLINE void					SetlpInput(CState* s) {m_lpInput = s;};
			LYNXINLINE CState*				GetlpInput() {return m_lpInput;};
			LYNXINLINE void					AddOutput(CState* s) {m_OutputList.push_back(s);};
			LYNXINLINE DWORD				GetNumOutputs() {return (DWORD)m_OutputList.size();};
			LYNXINLINE CState*				GetOutput(int i) {return m_OutputList[i];};
			LYNXINLINE CList<CState*>&		GetOutputList() {return m_OutputList;};
			virtual LYNXBOOL				vMatch(const CEvent& input);
			virtual CState*					vTranslate(const CEvent& input);
		};	
	}
	LYNXENGCLASS CString		GetEnumStr(StateMachine::CStateTranslation::TRANSLATIONMETHOD e);
	LYNXENGCLASS CString		GetEnumStr(StateMachine::CStateTranslation::TRANSLATIONTYPE t);
}
#endif

