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
#include <LynxAlgorithm.h>
#include <LynxParam.h>
#include <LynxParamGroup.h>
#include <LynxParamManager.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParamManager::CParamManager(void)
	{
		m_ParamGroupList.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CParamManager::~CParamManager(void)
	{
		CList<CParamGroup*>::CIterator Group;
		/*for (Group = m_ParamGroupList.begin(); Group != m_ParamGroupList.end(); ++Group)
		{
			LYNXDEL (*Group);
		}*/		
		m_ParamGroupList.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CParamManager::Add(CParam* p)
	{
		CList<CParamGroup*>::CIterator Group;
		CParamGroup* lpGroup;

		Group = LynxEngine::find(m_ParamGroupList.begin(), m_ParamGroupList.end(), p->GetGroupName());
		if (Group == m_ParamGroupList.end())
		{			
			lpGroup = LYNXGLOBAL_NEW CParamGroup(p->GetGroupName());			
			m_ParamGroupList.push_back(lpGroup);
			lpGroup->Add(p);
		}		
		else
		{
			(*Group)->Add(p);
		}
	}	
}