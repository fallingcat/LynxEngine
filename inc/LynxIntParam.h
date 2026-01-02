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

#ifndef __LYNXINTPARAM_H__
#define __LYNXINTPARAM_H__

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
	class LYNXENGCLASS CIntParam : public CParam
	{	
	public:
	protected:					
	public:	
		CIntParam() {m_Type = CParam::INT; };
		CIntParam(const CString& groupname, CParam* parent, const CString& name,  void* const lpv):CParam(groupname, parent, name, lpv) {m_Type = CParam::INT; };
		virtual ~CIntParam() {};			

		LYNXINLINE					operator int*() { return ((int*)m_lpValue); };			
		LYNXINLINE int				GetInt() { return (*((int*)m_lpValue)); };		
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CRangeIntParam : public CIntParam
	{	
	public:
	protected:					
		int							m_Min, m_Max;		
	public:	
		CRangeIntParam() {m_Type = CParam::RANGE_INT; m_Min = 0; m_Max = 100; };
		CRangeIntParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CIntParam(groupname, parent, name, lpv) {m_Type = CParam::RANGE_INT; m_Min = 0; m_Max = 100;};
		virtual ~CRangeIntParam() {};			

		LYNXINLINE void				SetupEx(int min, int max) {m_Min = min; m_Max = max; };			
		LYNXINLINE int				GetMin() const { return m_Min; };			
		LYNXINLINE int				GetMax() const { return m_Max; };			
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CDwordParam : public CParam
	{	
	public:
	protected:					
	public:	
		CDwordParam() {m_Type = CParam::DWORD; };
		CDwordParam(const CString& groupname, CParam* parent, const CString& name,  void* const lpv):CParam(groupname, parent, name, lpv) {m_Type = CParam::DWORD; };
		virtual ~CDwordParam() {};			

		LYNXINLINE					operator LYNXDWORD*() { return ((LYNXDWORD*)m_lpValue); };			
		LYNXINLINE LYNXDWORD		GetDword() { return (*((LYNXDWORD*)m_lpValue)); };		
	};		
}

#endif