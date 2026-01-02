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
//  Created by Owen Wu : 2008/01/31
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXLISTPARAM_H__
#define __LYNXLISTPARAM_H__

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
	class LYNXENGCLASS CBaseListParam : public CParam
	{	
	public:
	protected:			
		TYPE							m_ElementType;
	public:	
		CBaseListParam() {m_Type = CParam::LIST; };
		CBaseListParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::LIST; m_ElementType = et; };
		virtual ~CBaseListParam() {};			

		LYNXINLINE void					SetElementType(TYPE t) {m_ElementType = t; };
		LYNXINLINE TYPE					GetElementType() const {return m_ElementType; };
		virtual void					vSetNumElements(int s) = 0;
		virtual int						vGetNumElements() = 0;		
		virtual void*					vGetElementPointer(int i) = 0;
		virtual void					vAddElement(void* d) = 0;
		virtual void					vClearElement() = 0;
		virtual void					vRemoveElement(int i)  = 0;
		//virtual CParam*					vCreateObjPopinterParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv) = 0;
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	template <class Type> class CListParam : public CBaseListParam
	{	
	public:
	protected:	
	public:	
		CListParam() {};
		CListParam(const CString& groupname, CParam* parent, const CString& name, TYPE et, void* const lpv): CBaseListParam(groupname, parent, name, et, lpv) {};
		virtual ~CListParam() {};			

		virtual LYNXINLINE void			vSetNumElements(int s)
		{
			int OldSize = ((CList<Type>*)m_lpValue)->size();

			if (s != ((CList<Type>*)m_lpValue)->size() ) 
				((CList<Type>*)m_lpValue)->resize(s); 

			for (int i=OldSize; i<s; i++)			
				((*((CList<Type>*)m_lpValue))[i]) = Type(0); 
		}
		virtual LYNXINLINE int			vGetNumElements() {return (int)((CList<Type>*)m_lpValue)->size(); };		
		virtual LYNXINLINE void*		vGetElementPointer(int i) {return (&((*((CList<Type>*)m_lpValue))[i])); };
		virtual LYNXINLINE void			vAddElement(void* d) { (*((CList<Type>*)m_lpValue)).push_back((*((Type*)d))); };
		virtual LYNXINLINE void			vClearElement() { (*((CList<Type>*)m_lpValue)).clear(); };
		virtual LYNXINLINE void			vRemoveElement(int i) 
		{
			typename CList<Type>::CIterator Pos;
			int Index = 0;
			for (Pos=((CList<Type>*)m_lpValue)->begin(); Pos!=((CList<Type>*)m_lpValue)->end(); Pos++)
			{
				if (Index == i)
				{
					((CList<Type>*)m_lpValue)->erase(Pos); 
					return;
				}
				Index++;
			}
		}
		//virtual LYNXINLINE CParam*		vCreateObjPopinterParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv) {return new CObjPointerParam<Type>(groupname, parent, name, lpv); };
	};
}

#endif