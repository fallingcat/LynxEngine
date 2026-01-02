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

#ifndef __LYNXGAMEOBJ_H__
#define __LYNXGAMEOBJ_H__

#include <LynxObj.h>
#include <LynxArray.h>
#include <LynxList.h>
#include <LynxParam.h>
#include <LynxEngineClasses.h>
#include <StateMachine/LynxStateMachine.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CGameObj : public CObj
	{			
	public:				
		typedef struct{
			CGameObj*					lpObj;					
			CObj*						lpSubObj;			
		}PARENTDATA, *LPPARENTDATA;	
		typedef struct{
			PARENTDATA					Parent;		
			CArray<CGameObj*>			ChildArray;	
		}HIERARCHYDATA, *LPHIERARCHYDATA;	
	protected:				
		LYNXBOOL						m_bInheritParentTransform;
		HIERARCHYDATA					m_HierarchyData;
		HIERARCHYDATA					m_GameLogicHierarchyData;
		StateMachine::CStateMachinePtr	m_StateMachine;		
		LYNX_BITFIELD_BOOL(m_bActive)
	private:
		void							InitMembers();			
		LYNXINLINE void					SetlpParent(CGameObj* const lpo, CObj* const lpso = NULL) { m_HierarchyData.Parent.lpObj = lpo; m_HierarchyData.Parent.lpSubObj = lpso; };
		LYNXINLINE void					SetlpChild(int i, CGameObj* const lpo) { LYNX_ASSERT(i < (int)m_HierarchyData.ChildArray.size()); m_HierarchyData.ChildArray[i] = lpo; };

		LYNXINLINE void					SetlpGameLogicParent(CGameObj* const lpo, CObj* const lpso = NULL) { m_GameLogicHierarchyData.Parent.lpObj = lpo; m_GameLogicHierarchyData.Parent.lpSubObj = lpso; };
		LYNXINLINE void					SetlpGameLogicChild(int i, CGameObj* const lpo) { LYNX_ASSERT(i < (int)m_GameLogicHierarchyData.ChildArray.size()); m_GameLogicHierarchyData.ChildArray[i] = lpo; };
	public:
		LYNX_DECLARE_CLASS(CGameObj);
		CGameObj();
		CGameObj(CEngine* lpe);
		virtual ~CGameObj();			

		DECLARE_CLASSSCRIPT
		
		bool							operator ==(const CString& name) {return (m_Name == name); };					
		
		LYNXFORCEINLINE void			SetInheritParentTransform(LYNXBOOL b) { m_bInheritParentTransform = b; };
		LYNXFORCEINLINE LYNXBOOL		GetInheritParentTransform() { return m_bInheritParentTransform; };

		LYNXINLINE CGameObj* const		GetlpParent() {return m_HierarchyData.Parent.lpObj; };		
		LYNXINLINE CObj* const			GetlpParentSubObj() {return m_HierarchyData.Parent.lpSubObj; };

		void							SetNumChildren(DWORD num);
		LYNXINLINE DWORD				GetNumChildren() {return (DWORD)m_HierarchyData.ChildArray.size(); };			
		LYNXINLINE CGameObj* const		GetlpChild(int i) {LYNX_ASSERT(i < (int)m_HierarchyData.ChildArray.size()); return m_HierarchyData.ChildArray[i]; };


		LYNXFORCEINLINE void			Activate(LYNXBOOL b) {m_bActive = b; };
		LYNXFORCEINLINE LYNXBOOL		IsActive() const {return m_bActive; };
		//virtual LYNXFORCEINLINE void	vActivate(LYNXBOOL b) {Activate(b); };

		void							SetParent(CGameObj* const lpo, CObj* const lpso = NULL);	
		void							RemoveParent();
		void							AddChild(CGameObj* const lpo);		
		void							InsertChild(int i, CGameObj* const lpo);
		void							RemoveChild(CGameObj* const lpo);
				

		LYNXINLINE CGameObj* const		GetGameLogicParent() { return m_GameLogicHierarchyData.Parent.lpObj; };
		LYNXINLINE CObj* const			GetGameLogicParentSubObj() { return m_GameLogicHierarchyData.Parent.lpSubObj; };

		void							SetNumGameLogicChildren(DWORD num);
		LYNXINLINE DWORD				GetNumGameLogicChildren() { return (DWORD)m_GameLogicHierarchyData.ChildArray.size(); };
		LYNXINLINE CGameObj* const		GetGameLogicChild(int i) { LYNX_ASSERT(i < (int)m_GameLogicHierarchyData.ChildArray.size()); return m_GameLogicHierarchyData.ChildArray[i]; };

		void							SetGameLogicParent(CGameObj* const lpo, CObj* const lpso = NULL);
		void							RemoveGameLogicParent();
		void							AddGameLogicChild(CGameObj* const lpo);
		void							InsertGameLogicChild(int i, CGameObj* const lpo);
		void							RemoveGameLogicChild(CGameObj* const lpo);


		LYNXINLINE StateMachine::CStateMachine& GetStateMachine() {return (*m_StateMachine); };
		virtual LYNXBOOL				vSetCurrentState(int s);
		virtual LYNXBOOL				vSetCurrentState(const CString& s);
		virtual void					vUpdateStateMachine(const CEvent& s);			

		LYNXINLINE virtual void			vOnSysKeyDown(int key) {};
		LYNXINLINE virtual void			vOnSysKeyUp(int key) {};
		LYNXINLINE virtual void			vOnSysLButtonDown() {};
		LYNXINLINE virtual void			vOnSysLButtonUp() {};
		LYNXINLINE virtual void			vOnSysRButtonDown() {};
		LYNXINLINE virtual void			vOnSysRButtonUp() {};
		LYNXINLINE virtual void			vOnEvent(const CEvent& e) {m_StateMachine->Update(e); };		
	};		
}
#endif