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

#ifndef __LYNXSTRINGPARAM_H__
#define __LYNXSTRINGPARAM_H__

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
	class LYNXENGCLASS CStringParam : public CParam
	{	
	public:
	protected:					
	public:	
		CStringParam() {m_Type = CParam::STRING; };
		CStringParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::STRING; };
		virtual ~CStringParam() {};			

		LYNXINLINE					operator CString*() { return ((CString*)m_lpValue); };			
		LYNXINLINE CString&			GetStr() { return (*((CString*)m_lpValue)); };		
	};
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CAnsiStringParam : public CParam
	{	
	public:
	protected:					
	public:	
		CAnsiStringParam() {m_Type = CParam::ANSI_STRING; };
		CAnsiStringParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::ANSI_STRING; };
		virtual ~CAnsiStringParam() {};			

		LYNXINLINE					operator CAnsiString*() { return ((CAnsiString*)m_lpValue); };			
		LYNXINLINE CAnsiString&		GetStr() { return (*((CAnsiString*)m_lpValue)); };		
	};
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CBrowserParam : public CStringParam
	{	
	public:
	protected:					
	public:	
		CBrowserParam() {m_Type = CParam::BROWSER; };
		CBrowserParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CStringParam(groupname, parent, name, lpv) {m_Type = CParam::BROWSER; };
		virtual ~CBrowserParam() {};				
	};		
}

#endif