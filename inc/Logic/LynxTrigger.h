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

#ifndef __LYNXTRIGGER_H__
#define __LYNXTRIGGER_H__

#include <LynxObj.h>

namespace LynxEngine 
{	
	namespace Logic
	{		
		/**	
		*  @brief Logic系統中的Trigger物件。
		* 
		*	Logic系統中的Trigger物件。
		*	
		*/
		class LYNXENGCLASS CTrigger : public CObj
		{	
			LYNX_DECLARE_CLASS(CTrigger);
		public:			
			typedef enum {
				ENTER = 0,
				USE,
				NUM_TRIGGERMETHOD,
			}TRIGGERMETHOD;
		protected:		
			CEvent								m_TriggeringEvent;
			TRIGGERMETHOD						m_TriggeringMethod;
			CList<CGameObj*>					m_TargetList;						
		public:			
			CTrigger();			
			~CTrigger();					
			
			LYNXBOOL							Trigger(const CEvent& input);
		};	
	}
}
#endif

