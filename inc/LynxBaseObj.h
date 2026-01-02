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

#ifndef __LYNXBASEOBJ_H__
#define __LYNXBASEOBJ_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CBaseObj
	{	
		LYNX_DECLARE_CLASS(CBaseObj);
	public:
		DWORD								m_ID;
	protected:			
		int									m_Count;		
		#ifdef __TOOL__
			CList<CParam*>					m_ParamList;
		#endif
	public:	
		CBaseObj();
		virtual ~CBaseObj();								

		LYNXINLINE void						SetID(int id) {m_ID = id; };
		LYNXINLINE DWORD					GetID() { return m_ID; };
		LYNXINLINE void						IncreaseReference() {m_Count++;};
		LYNXINLINE void						DecreaseReference() {m_Count--;};
		LYNXINLINE const int				GetReferenceCount() const {return m_Count; };

		#ifdef __TOOL__
			LYNXINLINE CList<CParam*>&			GetParamList() {return m_ParamList; };
		#endif
	};		
}

#endif