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

#ifndef __LYNXPERFCOUNTER_H__
#define __LYNXPERFCOUNTER_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace PerfSystem 
	{
		class LYNXENGCLASS CCounter : public CUnknown
		{
			LYNX_DECLARE_CLASS(CCounter);
		private:			
		protected:		
			LYNXDWORD						m_Count;			
		public:			
			CCounter(const LYNXDWORD s = 0) {m_Count = s; };
			~CCounter() {};			

			void							Reset(const LYNXDWORD s = 0) {m_Count = s; };
			LYNXFORCEINLINE void			Increase(const LYNXDWORD step = 1) {m_Count += step; };
			LYNXFORCEINLINE LYNXDWORD		GetCount() const {return m_Count; };			
		};	
	}
}

#endif