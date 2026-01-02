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

#ifndef __LYNXRANDTRANSLATION_H__
#define __LYNXRANDTRANSLATION_H__

#include <StateMachine/LynxStateTranslation.h>

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
		class LYNXENGCLASS CRandTranslation : public CStateTranslation
		{
			LYNX_DECLARE_CLASS(CRandTranslation);		
		public:			
			CRandTranslation(CStateMachine* sm);			
			~CRandTranslation();		

			virtual CState*			vTranslate(const CString& input);
			virtual CState*			vTranslate(const CEvent& input);
		};	
	}
}
#endif

