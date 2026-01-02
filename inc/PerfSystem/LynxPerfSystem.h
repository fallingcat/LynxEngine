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

#ifndef __LYNXPERFSYSTEM_H__
#define __LYNXPERFSYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace PerfSystem 
	{
		typedef CMap<CString, CCounter*>	COUNTERMAP;
		typedef CMap<CString, CTimer*>		TIMERMAP;

		class LYNXENGCLASS CPerfSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CPerfSystem);
		private:			
		protected:		
			// Todo : add hierarchy info into counter and timer 
			CMap<CString, COUNTERMAP>		m_CounterMap;
			CMap<CString, TIMERMAP>			m_TimerMap;
		public:			
			CPerfSystem(CEngine* const lpengine);
			~CPerfSystem();			

			LYNXBOOL						vCreate(void);			
			LYNXBOOL						vInit();	

			CCounter*						CreateCounter(const CString& group, const CString& name, const DWORD startcount = 0);
			CCounter*						FindCounter(const CString& group, const CString& name);
			CCounter*						Counter(const CString& group, const CString& name) {return m_CounterMap[group][name]; };

			CTimer*							CreateTimer(const CString& group, const CString& name);
			CTimer*							FindTimer(const CString& group, const CString& name);
			CTimer*							Timer(const CString& group, const CString& name) {return m_TimerMap[group][name]; };
		};	
	}
	
	#ifdef __PROFILE__
		#define PERF_TIMER(g, n)				m_lpEngine->GetlpPerfSystem()->Timer(g, n) 
		#define PERF_TIMER_CREATE(g, n)			m_lpEngine->GetlpPerfSystem()->CreateTimer(g, n);
		#define PERF_TIMER_RESET(g, n)			PERF_TIMER_CREATE(g, n); PERF_TIMER(g, n)->Reset(); 
		#define PERF_TIMER_START(g, n)			PERF_TIMER_CREATE(g, n); PERF_TIMER(g, n)->Start(); 
		#define PERF_TIMER_RESET_START(g, n)	PERF_TIMER_CREATE(g, n); PERF_TIMER(g, n)->Reset();  PERF_TIMER(g, n)->Start(); 
		#define PERF_TIMER_STOP(g, n)			PERF_TIMER(g, n)->Stop(); 
		#define PERF_TIMER_GET(g, n)			PERF_TIMER(g, n)->GetTime() 

		#define PERF_COUNTER(g, n)				m_lpEngine->GetlpPerfSystem()->Counter(g, n) 
		#define PERF_COUNTER_CREATE(g, n, s)	m_lpEngine->GetlpPerfSystem()->CreateCounter(g, n, s);
		#define PERF_COUNTER_RESET(g, n, s)		PERF_COUNTER_CREATE(g, n, s); PERF_COUNTER(g, n)->Reset(s); 
		#define PERF_COUNTER_INC(g, n, s)		PERF_COUNTER(g, n)->Increase(s); 
		#define PERF_COUNTER_GET(g, n)			PERF_COUNTER(g, n)->GetCount() 
	#else
		#define PERF_TIMER(g, n)				
		#define PERF_TIMER_CREATE(g, n)			
		#define PERF_TIMER_RESET(g, n)			
		#define PERF_TIMER_START(g, n)			
		#define PERF_TIMER_RESET_START(g, n)	 
		#define PERF_TIMER_STOP(g, n)			
		#define PERF_TIMER_GET(g, n)			

		#define PERF_COUNTER(g, n)				
		#define PERF_COUNTER_CREATE(g, n, s)	
		#define PERF_COUNTER_RESET(g, n, s)		
		#define PERF_COUNTER_INC(g, n, s)		
		#define PERF_COUNTER_GET(g, n)			
	#endif		
}

#endif