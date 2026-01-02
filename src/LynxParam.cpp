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

#include <LynxEngine_PCH.h>
#include <LynxCore.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParam::CParam(void)
	{
		m_ParentParam = NULL;
		m_Flag = 0;
		m_lpValue = NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParam::~CParam(void)
	{
		m_ParentParam = NULL;
		m_Flag = 0;
		m_lpValue = NULL;
	}			
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParam::CParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int flag, const CString& comment)
	: m_GroupName(groupname), m_ParentParam(parent), m_Name(name), m_Comment(comment)
	{		
		m_lpValue = lpv;
		m_Flag = flag;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CParam::Setup(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int flag, const CString& comment)
	{
		m_GroupName = groupname;
		m_ParentParam = parent;
		m_Name = name;
		m_Comment = comment;		
		m_lpValue = lpv;
		m_Flag = flag;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParam& CParam::operator =(const CParam& rhs)
	{
		m_GroupName = rhs.m_GroupName;
		m_ParentParam = rhs.m_ParentParam;
		m_Name = rhs.m_Name;
		m_Type = rhs.m_Type;
		m_lpValue = rhs.m_lpValue;

		return (*this);
	}
}