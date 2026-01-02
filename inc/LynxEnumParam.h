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

#ifndef __LYNXENUMPARAM_H__
#define __LYNXENUMPARAM_H__

#include <LynxParam.h>

namespace LynxEngine 
{
	typedef struct {
		CString		Name;
		DWORD		Value;
	}ENUMDATA;

	class LYNXENGCLASS CBaseEnumParam : public CParam
	{	
	public:
	protected:	
		LYNXDWORD					m_NumEnums;
		ENUMDATA*					m_EnumArray;
	public:	
		CBaseEnumParam() {m_Type = CParam::ENUM; };
		CBaseEnumParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int num, ENUMDATA* enumdata): CParam(groupname, parent, name, lpv) {m_Type = CParam::ENUM; m_NumEnums = num; m_EnumArray = enumdata; };
		virtual ~CBaseEnumParam() {};			

		void								SetupEx(int num, ENUMDATA* enumdata) {m_NumEnums = num; m_EnumArray = enumdata; };
		virtual LYNXINLINE LYNXDWORD		vGetNumEnums() const { return m_NumEnums; };
		//virtual LYNXINLINE const ENUMDATA*	vGetEnumArray() const { return m_EnumArray; };
		//virtual LYNXINLINE const CString&	vGetEnumString(int i) const { return m_EnumArray[i].Name; };
		//virtual LYNXINLINE DWORD			vGetEnumValue(int i) const { return m_EnumArray[i].Value; };

		virtual const CString				vGetEnumString(int i) const = 0;
		virtual LYNXINLINE LYNXDWORD		vGetEnumValue(int i) const = 0;
	};	
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	template <class Type> class CEnumParam : public CBaseEnumParam
	{	
	public:
	protected:	
	public:	
		CEnumParam() {};
		CEnumParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int num, ENUMDATA* enumdata): CBaseEnumParam(groupname, parent, name, lpv, num, enumdata) {};
		virtual ~CEnumParam() {};			

		LYNXINLINE					operator Type*() { return ((Type*)m_lpValue); };			
		LYNXINLINE Type				GetValue() { return (*((Type*)m_lpValue)); };		
		virtual LYNXINLINE const CString	vGetEnumString(int i) const { return LynxEngine::GetEnumStr((Type)i); };
		virtual LYNXINLINE LYNXDWORD		vGetEnumValue(int i) const { return i; };
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	template <class Type> class CListEnumParam : public CBaseEnumParam
	{	
	public:
	protected:	
		CList<Type>*				m_lpList;
	public:	
		CListEnumParam() {m_Type = CParam::LIST_ENUM; m_lpList = NULL; };
		CListEnumParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv, CList<Type>* lplist): CBaseEnumParam(groupname, parent, name, lpv, 0, NULL) {m_Type = CParam::LIST_ENUM; m_lpList = lplist; };
		virtual ~CListEnumParam() {};			

		void								SetupEx(CList<Type>* lplist) {m_lpList = lplist; };		
		virtual LYNXINLINE LYNXDWORD		vGetNumEnums() const { if (!m_lpList) return 0; else return (LYNXDWORD)(*m_lpList).size(); };		
		virtual LYNXINLINE const CString	vGetEnumString(int i) const { LYNX_ASSERT(m_lpList != NULL); return (*m_lpList)[i]->GetName(); };
		virtual LYNXINLINE LYNXDWORD		vGetEnumValue(int i) const {return (LYNXDWORD)((void*)((*m_lpList)[i])); };

		LYNXINLINE							operator Type*() { return ((Type*)m_lpValue); };			
		LYNXINLINE Type						GetValue() { return (*((Type*)m_lpValue)); };		
	};		
}

#endif