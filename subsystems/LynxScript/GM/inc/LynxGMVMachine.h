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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXGMVMACHINE_H__
#define __LYNXGMVMACHINE_H__

#include <ScriptSystem/LynxVMachine.h>

namespace LynxScript 
{
	namespace GM 
	{	
		class CVMachine : public LynxEngine::ScriptSystem::CVMachine
		{	
			DECLARE_CLASS(CVMachine);
		protected:			
			gmMachine*						m_VMachine; 
			
		public:			
			CVMachine();
			virtual ~CVMachine();		
		};	
	}
}

#endif