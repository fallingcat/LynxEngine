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

#ifndef __LYNXARRAYPARAM_H__
#define __LYNXARRAYPARAM_H__

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
	class LYNXENGCLASS CBaseArrayParam : public CParam
	{	
	public:
	protected:			
		TYPE							m_ElementType;
	public:	
		CBaseArrayParam() {m_Type = CParam::ARRAY; };
		CBaseArrayParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::ARRAY; m_ElementType = et; };
		virtual ~CBaseArrayParam() {};			

		LYNXINLINE void					SetElementType(TYPE t) {m_ElementType = t; };
		LYNXINLINE TYPE					GetElementType() const {return m_ElementType; };
		virtual void					vSetNumElements(int s) = 0;		
		virtual int						vGetNumElements() = 0;		
		virtual void*					vGetElementPointer(int i)  = 0;
		virtual void					vClearElement()  = 0;
		virtual void					vRemoveElement(int i)  = 0;
		//virtual CParam*					vGetElementParam(int i)  = 0;
		//virtual CParam*					vCreateObjPopinterParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv) = 0;
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	template <class Type> class LYNXENGTEMPLATE CArrayParam : public CBaseArrayParam
	{	
	public:
	protected:	
	public:	
		CArrayParam() {};
		CArrayParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, void* const lpv): CBaseArrayParam(groupname, parent, name, et, lpv) 
		{
		};
		virtual ~CArrayParam() {};			

		virtual LYNXINLINE void			vSetNumElements(int s) 
		{
			int OldSize = ((CArray<Type>*)m_lpValue)->size();

			if (s != ((CArray<Type>*)m_lpValue)->size() ) 
			{
				((CArray<Type>*)m_lpValue)->resize(s); 
			}

			for (int i=OldSize; i<s; i++)			
			{
				((*((CArray<Type>*)m_lpValue))[i]) = Type(0); 
			}			
		}		
		virtual LYNXINLINE int			vGetNumElements() {return (int)((CArray<Type>*)m_lpValue)->size(); };		
		virtual LYNXINLINE void*		vGetElementPointer(int i) {return &((*((CArray<Type>*)m_lpValue))[i]); };
		virtual LYNXINLINE void			vClearElement() {((CArray<Type>*)m_lpValue)->clear(); };		
		virtual LYNXINLINE void			vRemoveElement(int i) {((CArray<Type>*)m_lpValue)->erase(((CArray<Type>*)m_lpValue)->begin()+i); };
		//virtual LYNXINLINE LYNXBOOL		vIsTheSameSubTypeObjPointer(CObj* lpo) {return dynamic_cast<Type*>(lpo)?LYNX_TRUE:LYNX_FALSE; };		
		//virtual LYNXINLINE CParam*		vCreateObjPopinterParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv) {return new CObjPointerParam<Type>(groupname, parent, name, lpv); };
	};		

	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	template <class Type> class LYNXENGTEMPLATE CNativeArrayParam : public CBaseArrayParam
	{	
	public:
	protected:	
		int													m_NumElements;
	public:	
		CNativeArrayParam() {m_Type = CParam::NATIVE_ARRAY; };
		CNativeArrayParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, int size, void* const lpv): CBaseArrayParam(groupname, parent, name, et, lpv) 
		{
			m_Type = CParam::NATIVE_ARRAY; 
			m_NumElements = size;			
		};
		virtual ~CNativeArrayParam() {};			

		LYNXINLINE void					vSetNumElements(int s) {m_NumElements = s; };
		LYNXINLINE void					vClearElement() {m_NumElements = 0; };
		LYNXINLINE int					vGetNumElements() {return m_NumElements; };		
		virtual LYNXINLINE void*		vGetElementPointer(int i) {return &((((Type*)m_lpValue))[i]); };		
		virtual void					vRemoveElement(int i) {}; 
		//virtual LYNXINLINE CParam*		vCreateObjPopinterParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv) {return new CObjPointerParam<Type>(groupname, parent, name, lpv); };
	};	
}

#endif