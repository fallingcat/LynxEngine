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

#ifndef __LYNXGMSCRIPT_H__
#define __LYNXGMSCRIPT_H__

#include <ScriptSystem/LynxScript.h>

namespace LynxScript 
{
	namespace GM 
	{	
		class CScript : public LynxEngine::ScriptSystem::CScript
		{	
		protected:			
			gmUserObject*					m_UserObj;	//< The script object
			gmTableObject*					m_TableObj; //< Table functionality for script object members
			gmArraySimple<int>				m_Threads;  //< Threads associated with this game object
			
		public:
			IMPLEMENT_GETCLASSNAME(CVMachine);
			CScript();
			CScript(CScriptSystem* lpsys);
			virtual ~CScript();		
		};	
	}
}

#endif