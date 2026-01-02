//###########################################################################
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

#ifndef __LYNXMONOSCRIPTSYSTEM_H__
#define __LYNXMONOSCRIPTSYSTEM_H__

namespace LynxScript 
{
	namespace Mono 
	{		
		class CVMachine;
		class CScriptSystem : public LynxEngine::ScriptSystem::CScriptSystem		
		{	
			LYNX_DECLARE_CLASS(CScriptSystem);
		protected:					
		public:			
			CScriptSystem(LynxEngine::CEngine* const lpengine);
			virtual ~CScriptSystem();

			LYNXINLINE const LYNXCHAR*						vGetAPIString() {return _T("Mono"); };
			LYNXBOOL										vCreate();			
			LYNXBOOL										vOnInit();	
			static  LYNXBOOL								EncryptScript(const LYNXCHAR *filename, const char* key = NULL) {return LYNX_TRUE; }
		};	
	}
}

#endif
