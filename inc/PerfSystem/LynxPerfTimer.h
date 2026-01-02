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

#ifndef __LYNXPERFTIMER_H__
#define __LYNXPERFTIMER_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace PerfSystem 
	{
		class LYNXENGCLASS CTimer : public CUnknown
		{
			LYNX_DECLARE_CLASS(CTimer);
		private:			
		protected:		
			LYNXLONGLONG					m_StartTime;			
			LYNXDOUBLE						m_ElapsedTime;							
		public:			
			CTimer() {Reset(); };
			~CTimer() {};			

			void							Reset() {m_ElapsedTime = 0.0; m_ElapsedTime = 0.0; };
			LYNXFORCEINLINE void			Start() {m_StartTime = lynxOSGetPerformanceCounter(); };
			LYNXFORCEINLINE void			Stop() { m_ElapsedTime += (lynxOSGetPerformanceCounter() - m_StartTime)*1000.0/((LYNXDOUBLE)lynxOSGetPerformanceFrequency()); };
			LYNXFORCEINLINE LYNXDOUBLE		GetTime() const {return m_ElapsedTime; };			
		};	
	}
}

#endif