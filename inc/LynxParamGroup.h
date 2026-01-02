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

#ifndef __LYNXPARAMGROUP_H__
#define __LYNXPARAMGROUP_H__

#include <LynxEngineClasses.h>
#include <LynxString.h>
#include <LynxList.h>
#include <LynxParam.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CParamGroup
	{	
	public:		
	protected:		
		CString						m_Name;
		CList<CParam*>				m_ParamList;		
	public:	
		CParamGroup();
		CParamGroup(const CString& name);
		virtual ~CParamGroup();			

		bool						operator ==(const CString& name) {return (m_Name == name); };			
		LYNXINLINE void				SetName(const CString name) {m_Name = name; };
		LYNXINLINE const CString&	GetName() const {return m_Name; };
		LYNXINLINE CList<CParam*>&  GetParamList() {return m_ParamList; };		
		LYNXINLINE void				Add(CParam* const p) {m_ParamList.push_back(p); };		
	};
	LYNXINLINE bool operator ==(const CParamGroup* pg, const CString& name) {return (pg->GetName() == name); };			
}

#endif