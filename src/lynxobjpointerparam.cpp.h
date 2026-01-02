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

#ifndef __LYNXOBJPOINTERPARAM_H__
#define __LYNXOBJPOINTERPARAM_H__

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
	template <class Type> class LYNXENGTEMPLATE CObjPointerParam : public CParam 
	{	
	public:
	protected:					
	public:	
		CObjPointerParam() {m_Type = CParam::LPOBJ; };
		CObjPointerParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::LPOBJ; };
		virtual ~CObjPointerParam() {};			

		//LYNXINLINE					operator CObj*() { return (dynamic_cast<CObj*>*((Type**)m_lpValue))); };			
		//LYNXINLINE virtual CObj*	vGetlpObj() {return (dynamic_cast<CObj*>(*((Type**)m_lpValue)));	};		
		LYNXINLINE					operator Type*() { return (*((Type**)m_lpValue)); };			
		LYNXINLINE virtual CObj*	vGetlpObj() {return (CObj*)(*((Type**)m_lpValue)); };		
		LYNXINLINE virtual void		vSetlpObj(CObj* lpo) {(*((Type**)m_lpValue)) = dynamic_cast<Type*>(lpo); };
		LYNXINLINE virtual LYNXBOOL	vIsTheSameObjPointer(CObj* lpo) {return dynamic_cast<Type*>(lpo)?LYNX_TRUE:LYNX_FALSE; };		
	};		
}

#endif