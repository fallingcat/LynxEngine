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
#include <LynxEngine.h>
#include <LynxCore.h>
#include <LynxEvent.h>
#include <LynxGameObj.h>
#include <StateMachine/LynxState.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>

namespace LynxEngine 
{	
	IMPLEMENT_CLASSSCRIPT(CGameObj, CObj)
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:    
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::InitMembers(void)
	{	
		m_bActive = LYNX_TRUE;

		m_bInheritParentTransform = LYNX_TRUE;
		m_HierarchyData.Parent.lpObj = NULL;
		m_HierarchyData.Parent.lpSubObj = NULL;
		m_HierarchyData.ChildArray.clear();	

		m_StateMachine = StateMachine::CStateMachinePtr(LYNXNEW StateMachine::CStateMachine);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CGameObj::CGameObj(void)	
	{
		InitMembers();		
		m_lpEngine = NULL;

		m_ClassScriptClassName = ClassScriptClassName();
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CGameObj::CGameObj(CEngine* lpe)
	: CObj(lpe)
	{
		InitMembers();		
		m_lpEngine = lpe;		

		INIT_CLASSSCRIPT
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CGameObj::~CGameObj(void)
	{
		m_HierarchyData.ChildArray.clear();		
	}			
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::SetNumChildren(DWORD num) 
	{
		if (num <= 0)
			m_HierarchyData.ChildArray.clear();
		else
		{
			m_HierarchyData.ChildArray.resize(num); 		
			for (int i=0; i<m_HierarchyData.ChildArray.size(); ++i)		
				m_HierarchyData.ChildArray[i] = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::SetParent(CGameObj* const lpo, CObj* const lpso) 
	{
		if (m_HierarchyData.Parent.lpObj)
			m_HierarchyData.Parent.lpObj->RemoveChild(this);
		if (lpo)
			lpo->AddChild(this);
		SetlpParent(lpo, lpso); 
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::RemoveParent() 
	{ 
		m_HierarchyData.Parent.lpObj->RemoveChild(this);
		SetlpParent(NULL, NULL); 
	}		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::AddChild(CGameObj* const lpo) 
	{
		if (!lpo)
			return;

		m_HierarchyData.ChildArray.push_back(lpo); 
		lpo->SetlpParent(this, NULL); 
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::InsertChild(int i, CGameObj* const lpo) 
	{ 
		if (!lpo)
			return;

		CArray<CGameObj*>::CIterator Where = m_HierarchyData.ChildArray.begin() + i;
		m_HierarchyData.ChildArray.insert(Where, lpo);
		lpo->SetlpParent(this, NULL); 
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::RemoveChild(CGameObj* const lpo) 
	{
		//LYNX_ASSERT(LynxEngine::find(m_HierarchyData.ChildArray.begin(), m_HierarchyData.ChildArray.end(), lpo) != m_HierarchyData.ChildArray.end());

		CArray<CGameObj*>::CIterator Where = LynxEngine::find(m_HierarchyData.ChildArray.begin(), m_HierarchyData.ChildArray.end(), lpo);
		if (Where != m_HierarchyData.ChildArray.end())
		{
			lpo->SetlpParent(NULL, NULL);
			m_HierarchyData.ChildArray.erase(Where); 			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::SetNumGameLogicChildren(DWORD num)
	{
		if (num <= 0)
			m_GameLogicHierarchyData.ChildArray.clear();
		else
		{
			m_GameLogicHierarchyData.ChildArray.resize(num);
			for (int i = 0; i<m_GameLogicHierarchyData.ChildArray.size(); ++i)
				m_GameLogicHierarchyData.ChildArray[i] = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::SetGameLogicParent(CGameObj* const lpo, CObj* const lpso)
	{
		if (m_GameLogicHierarchyData.Parent.lpObj)
			m_GameLogicHierarchyData.Parent.lpObj->RemoveGameLogicChild(this);
		if (lpo)
			lpo->AddGameLogicChild(this);
		SetlpGameLogicParent(lpo, lpso);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::RemoveGameLogicParent()
	{
		m_GameLogicHierarchyData.Parent.lpObj->RemoveGameLogicChild(this);
		SetlpGameLogicParent(NULL, NULL);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::AddGameLogicChild(CGameObj* const lpo)
	{
		if (!lpo)
			return;

		m_GameLogicHierarchyData.ChildArray.push_back(lpo);
		lpo->SetlpGameLogicParent(this, NULL);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::InsertGameLogicChild(int i, CGameObj* const lpo)
	{
		if (!lpo)
			return;

		CArray<CGameObj*>::CIterator Where = m_GameLogicHierarchyData.ChildArray.begin() + i;
		m_GameLogicHierarchyData.ChildArray.insert(Where, lpo);
		lpo->SetlpGameLogicParent(this, NULL);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::RemoveGameLogicChild(CGameObj* const lpo)
	{
		//LYNX_ASSERT(LynxEngine::find(m_GameLogicHierarchyData.ChildArray.begin(), m_GameLogicHierarchyData.ChildArray.end(), lpo) != m_GameLogicHierarchyData.ChildArray.end());

		CArray<CGameObj*>::CIterator Where = LynxEngine::find(m_GameLogicHierarchyData.ChildArray.begin(), m_GameLogicHierarchyData.ChildArray.end(), lpo);
		if (Where != m_GameLogicHierarchyData.ChildArray.end())
		{
			m_GameLogicHierarchyData.ChildArray.erase(Where);
			lpo->SetlpGameLogicParent(NULL, NULL);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CGameObj::vSetCurrentState(int s) 
	{
		return m_StateMachine->SetCurrentState(s); 		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CGameObj::vSetCurrentState(const CString& s) 
	{
		return m_StateMachine->SetCurrentState(s); 
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CGameObj::vUpdateStateMachine(const CEvent& s)
	{
		m_StateMachine->Update(s);		
	}		
}