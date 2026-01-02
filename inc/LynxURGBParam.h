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

#ifndef __LYNXURGBPARAM_H__
#define __LYNXURGBPARAM_H__

#include <LynxParam.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CURGBParam : public CParam
	{	
	public:
	protected:					
	public:	
		CURGBParam() {m_Type = CParam::UB_RGB; };
		CURGBParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::UB_RGB; };
		virtual ~CURGBParam() {};			

		LYNXINLINE					operator LYNXCOLORRGB*() { return ((LYNXCOLORRGB*)m_lpValue); };			
		LYNXINLINE LYNXCOLORRGB		GetRGB() { return (*((LYNXCOLORRGB*)m_lpValue)); };		
	};		
}

#endif