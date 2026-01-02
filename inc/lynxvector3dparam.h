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

#ifndef __LYNXVECTOR3DPARAM_H__
#define __LYNXVECTOR3DPARAM_H__

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
	class LYNXENGCLASS CVector3DParam : public CParam
	{	
	public:
	protected:					
	public:	
		CVector3DParam() {m_Type = CParam::VECTOR3D; };
		CVector3DParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::VECTOR3D; };
		virtual ~CVector3DParam() {};			

		LYNXINLINE					operator Math::CVector3*() { return ((Math::CVector3*)m_lpValue); };					
	};		
}

#endif