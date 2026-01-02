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

#ifndef __LYNXEXCEPTION_H__
#define __LYNXEXCEPTION_H__

#include <LynxCore.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CException
	{			
		LYNX_DECLARE_CLASS(CException);							
	protected:
		CString						m_Message;		
	public:			
		CException(const CString& str) {m_Message = str; };
		virtual ~CException() {};						

		LYNXINLINE CString&			GetMessage() {return m_Message; };
	};				
}

#define LYNX_JUST_THROW			throw	
#define LYNX_THROW(e)			throw e

#ifdef __NO_GUARD__
	#define LYNX_GUARD(func)
	#define LYNX_UNGUARD
	#define LYNX_DEFAULTCATCH
	#define LYNX_DEFAULTCATCH1(e)		
#else	
	#ifdef __CALL_STACK__
		#define LYNX_GUARD(func)		{ FUNC_NAME(func) try {		
		#define LYNX_UNGUARD			}		
		#define LYNX_DEFAULTCATCH		catch (...) { LynxEngine::AddToCallStack(__FUNC_NAME__); throw; } }		
		#define LYNX_DEFAULTCATCH1(e)	catch (...) { e; LynxEngine::AddToCallStack(__FUNC_NAME__); throw; } }		
	#else
		#define LYNX_GUARD(func)		{ try {
		#define LYNX_UNGUARD			}		
		#define LYNX_DEFAULTCATCH		catch (...) { throw; } }		
		#define LYNX_DEFAULTCATCH1(e)	catch (...) { e; throw; } }		
	#endif				
#endif

#endif