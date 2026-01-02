//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxCoreEngine.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <SoundSystem/LynxSoundSystem.h>
#include <InputSystem/LynxInputSystem.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CCoreEngine::InitMembers(void)
	{			
		m_lpInputSystem = NULL;
		m_lpGameConsoleSystem = NULL;
		m_lpGUISystem = NULL;
		
		m_lpPhysicsSystem = NULL;
		m_lpScriptSystem = NULL;
		m_Step = 1.0f;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CCoreEngine::CCoreEngine(void)
	{		
		InitMembers();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CCoreEngine::~CCoreEngine(void)
	{		
		LYNXDEL m_lpInputSystem;
		LYNXDEL m_lpGameConsoleSystem;					

		// Unload dynamic subsystems
		UnloadSubSystem(m_lpPhysicsSystem);
		UnloadSubSystem(m_lpScriptSystem);		

		LYNX_SET_LOG(NULL, _T("\n\n"));			
	
		if (lynxGetAllocMemSize() > 0)
		{
			lynxClearBadMemBlock();
			//MessageBeep(MB_ICONEXCLAMATION);    
			//MessageBox(lpsdk->Renderer.SystemData.MainWindow, "記憶體未完全釋放", "警告", MB_OK | MB_ICONERROR);
		}		
		lynxClearBadFileBlock();					
	}				
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CCoreEngine::vCreate()
	{
		LynxEngine::PhysicsSystem::CPhysicsWorld* lpW;

		//if (!m_lpGraphicsSystem->vCreate())
		//	return LYNX_FALSE;

		if (m_lpGameConsoleSystem)
		{
			if (!m_lpGameConsoleSystem->vCreate())
				return LYNX_FALSE;
		}

		if (m_lpPhysicsSystem)
		{
			lpW = m_lpPhysicsSystem->vCreateWorld();
			if (!lpW)
				return LYNX_FALSE;
			lpW->SetName(_T("RootWorld"));
		}

		if (m_lpScriptSystem)
		{
		}
		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSystem* CCoreEngine::LoadSubSystem(const LYNXCHAR *name)
	{		
		CSystem *lpSubSystem = NULL;

		#ifdef __LYNXENGINE_DLL__		
			HMODULE hSubSystem = lynxOSLoadLibrary(name);
			if (hSubSystem)
			{
				LPCREATESYSTEMPROC lpCreateSystem = (LPCREATESYSTEMPROC)lynxOSGetProc(hSubSystem, "CreateSystem");
				if (lpCreateSystem)
				{
					lpSubSystem = lpCreateSystem();
				}
			}		
		#else		
			if (lynxStrStr(name, "LynxPhysics"))
			{
				lpSubSystem = CreatePhysicsSystem();	
			}
			/*
			else if (lynxStrStr(name, "LynxScript"))
			{
				lpSubSystem = CreateScriptSystem();		
			}
			*/
		#endif		

		return lpSubSystem;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CCoreEngine::UnloadSubSystem(CSystem* lpss)
	{		
		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CCoreEngine::RegisterEvent(CEvent::TYPE t, CGameObj* obj)
	{
		m_EventRegisteredObjList[t].push_back(obj);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CCoreEngine::BroadcastEvent(CEvent& e)
	{
		CList<CGameObj*>::CIterator Obj;

		switch (e.GetType())
		{
			case CEvent::SYS_KEY_DOWN:
				for (Obj = m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].end(); ++Obj)
				{
					(*Obj)->vOnSysKeyDown(e.m_Param);
				}				
				break;

			case CEvent::SYS_KEY_UP:
				for (Obj = m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].end(); ++Obj)
				{
					(*Obj)->vOnSysKeyUp(e.m_Param);
				}				
				break;

			default:
				for (Obj = m_EventRegisteredObjList[e.GetType()].begin(); Obj != m_EventRegisteredObjList[e.GetType()].end(); ++Obj)
				{
					(*Obj)->vOnEvent(e);
				}
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CCoreEngine::Loop(void)
	{
		if (m_lpGameConsoleSystem->IsRunning())
		{
			m_lpGameConsoleSystem->vLoop();			
		}
		else
		{
			if (m_lpPhysicsSystem)
				m_lpPhysicsSystem->Loop(m_Step*100);//*0.04f);						
		}
	}	
}