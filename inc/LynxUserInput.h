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
//  Created by Owen Wu : 2006/11/16
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXUSERINPUT_H__
#define __LYNXUSERINPUT_H__

#include <LynxEvent.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CUserInput : public CEvent
	{	
		LYNX_DECLARE_CLASS(CUserInput);
	public:				
	protected:			
	public:			
		CUserInput() {m_Type = CEvent::USER_INPUT; };			
		CUserInput(const CString& name):CEvent(CEvent::USER_INPUT, name) {};	
		virtual ~CUserInput() {};	
	};
}
#endif

