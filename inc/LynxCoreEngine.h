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

#ifndef __LYNXENGINE_H__
#define __LYNXENGINE_H__

#include <LynxObj.h>
#include <LynxException.h>
#include <LynxList.h>
#include <LynxArray.h>
#include <LynxEvent.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CCoreEngine : public CObj
	{	
		LYNX_DECLARE_CLASS(CCoreEngine);
	public:				
		typedef CSystem* (*LPCREATESYSTEMPROC)(void);			
	protected:						
		static const int								m_Version = 100;
		
		//Subsystems		
		InputSystem::CInputSystem*						m_lpInputSystem;
		GameConsoleSystem::CGameConsoleSystem*			m_lpGameConsoleSystem;
		GUISystem::CGUISystem*							m_lpGUISystem;					
		NetworkSystem::CNetworkSystem*					m_lpNetworkSystem;
		SoundSystem::CSoundSystem*						m_lpSoundSystem;
		ScriptSystem::CScriptSystem*					m_lpScriptSystem;		
		PhysicsSystem::CPhysicsSystem*					m_lpPhysicsSystem;	
	
		CList<CGameObj*>								m_EventRegisteredObjList[CEvent::NUM_TYPES];							
	private:
		void											InitMembers();		
	public:
		//CArray<CViewport*>								m_ViewportArray;
		float											m_Step;

	public:		
		CCoreEngine();
		virtual ~CCoreEngine();

		virtual LYNXBOOL								vCreate(void);		
		LYNXINLINE int									GetVersion() {return m_Version; };

		CSystem*										LoadSubSystem(const LYNXCHAR *name);
		LYNXBOOL										UnloadSubSystem(CSystem* lpss);

		virtual void									vLoop(void);
		
		LYNXINLINE ScriptSystem::CScriptSystem*			GetlpScriptSystem() {return (m_lpScriptSystem);};
		LYNXINLINE PhysicsSystem::CPhysicsSystem*		GetlpPhysicsSystem() {return (m_lpPhysicsSystem);};
		LYNXINLINE GameConsoleSystem::CGameConsoleSystem* GetlpGameConsoleSystem() {return m_lpGameConsoleSystem;};						

		void											RegisterEvent(CEvent::TYPE t, CGameObj* obj);
		void											BroadcastEvent(CEvent& e);
	};	
}
#endif