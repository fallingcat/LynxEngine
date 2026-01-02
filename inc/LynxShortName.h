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

#ifndef __LYNXSHORTNAME_H__
#define __LYNXSHORTNAME_H__

#include <LynxString.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CShortName : public CString
	{	
		LYNX_DECLARE_CLASS(CShortName);
	protected:						
	public:		
		CShortName():CString(64) {};
		CShortName(const LYNXCHAR *str):CString(64) {Set(str); };
		virtual ~CShortName() {m_Str.clear(); };		

		LYNXINLINE void						Set(const LYNXCHAR *str) { m_Str.setData(str, 64); };						
	};				
}

#endif