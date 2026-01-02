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

//#ifndef __LYNXNATIVEARRAYPARAM_H__
//#define __LYNXNATIVEARRAYPARAM_H__
//
//#include <LynxParam.h>
//
//namespace LynxEngine 
//{
//	/**	
//	*  @brief Base object class of the entire engine。
//	* 
//	*  The basic object class of the entire engine, every class of the engine is drived from this class.
//	*  It records a unique ID and a reference counter. 
//	*	
//	*/
//	class LYNXENGCLASS CBaseNativeArrayParam : public CParam
//	{	
//	public:
//	protected:			
//		int								m_NumElements;
//		TYPE							m_ElementType;
//	public:	
//		CBaseNativeArrayParam() {m_Type = CParam::NATIVE_ARRAY; m_NumElements = 0; };
//		CBaseNativeArrayParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, int size, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::NATIVE_ARRAY; m_ElementType = et; m_NumElements = size; };
//		virtual ~CBaseNativeArrayParam() {};			
//
//		LYNXINLINE void					SetElementType(TYPE t) {m_ElementType = t; };
//		LYNXINLINE TYPE					GetElementType() const {return m_ElementType; };
//		LYNXINLINE void					SetNumElements(int s) {m_NumElements = s; };
//		LYNXINLINE int					GetNumElements() const {return m_NumElements; };		
//		virtual void*					vGetElementPointer(int i)  = 0;
//	};		
//	/**	
//	*  @brief Base object class of the entire engine。
//	* 
//	*  The basic object class of the entire engine, every class of the engine is drived from this class.
//	*  It records a unique ID and a reference counter. 
//	*	
//	*/
//	template <class Type> class CNativeArrayParam : public CBaseNativeArrayParam
//	{	
//	public:
//	protected:	
//	public:	
//		CNativeArrayParam() {};
//		CNativeArrayParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, int size, void* const lpv): CBaseNativeArrayParam(groupname, parent, name, lpv) {};
//		virtual ~CNativeArrayParam() {};			
//
//
//		virtual LYNXINLINE void*		vGetElementPointer(int i) {return &((((Type*)m_lpValue))[i]); };
//	};		
//}
//
//#endif