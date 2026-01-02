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

#include <LynxEngine_PCH.h>
#include <PerfSystem/LynxPerfSystem.h>
#include <PerfSystem/LynxPerfCounter.h>
#include <PerfSystem/LynxPerfTimer.h>

namespace LynxEngine 
{	
	namespace PerfSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPerfSystem::CPerfSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{		
			m_CounterMap.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPerfSystem::~CPerfSystem(void)
		{
			CMap<CString, COUNTERMAP>::CIterator CounterGroup;
			COUNTERMAP::CIterator Counter;
			for (CounterGroup = m_CounterMap.begin(); CounterGroup != m_CounterMap.end(); ++CounterGroup)
			{
				for (Counter = (*CounterGroup).second.begin(); Counter != (*CounterGroup).second.end(); ++Counter)
				{
					LYNXDEL(Counter->second);
				}
				CounterGroup->second.clear();
			}
			m_CounterMap.clear();

			CMap<CString, TIMERMAP>::CIterator TimerGroup;
			TIMERMAP::CIterator Timer;
			for (TimerGroup = m_TimerMap.begin(); TimerGroup != m_TimerMap.end(); ++TimerGroup)
			{
				for (Timer = (*TimerGroup).second.begin(); Timer != (*TimerGroup).second.end(); ++Timer)
				{
					LYNXDEL(Timer->second);
				}
				TimerGroup->second.clear();
			}
			m_TimerMap.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPerfSystem::vCreate(void)
		{			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPerfSystem::vInit(void)
		{			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCounter* CPerfSystem::CreateCounter(const CString& group, const CString& name, const DWORD startcount)
		{
			CCounter* Counter = FindCounter(group, name);
			if (!Counter)
			{			
				Counter = LYNXNEW CCounter(startcount);
				m_CounterMap[group].insert(CMap<CString, CCounter*>::value_type(name, Counter));
			}
			return Counter;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCounter* CPerfSystem::FindCounter(const CString& group, const CString& name)
		{
			CMap<CString, CCounter*>::CIterator i = m_CounterMap[group].find(name);
			if (i == m_CounterMap[group].end())
			{
				return NULL;
			}
			return (i->second);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTimer*	CPerfSystem::CreateTimer(const CString& group, const CString& name)
		{
			CTimer* Timer = FindTimer(group, name);			
			if (!Timer)
			{
				Timer = LYNXNEW CTimer();
				m_TimerMap[group].insert(CMap<CString, CTimer*>::value_type(name, Timer));
			}
			return Timer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTimer*	CPerfSystem::FindTimer(const CString& group, const CString& name)
		{
			CMap<CString, CTimer*>::CIterator i = m_TimerMap[group].find(name);
			if (i == m_TimerMap[group].end())
			{
				return NULL;
			}
			return (i->second);
		}
	}
}