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

#ifndef __LYNXGMSCRIPTSYSTEM_H__
#define __LYNXGMSCRIPTSYSTEM_H__

#include <LynxCore.h>
#include <ScriptSystem/LynxScriptSystem.h>

#ifdef __LYNXSCRIPT_LIB__
	#define	LYNXSCRIPTCLASS						
#elif __LYNXSCRIPT_DLL__
	#define	LYNXSCRIPTCLASS						LYNXEXPORT
#else
	#define LYNXSCRIPTCLASS						LYNXIMPORT
#endif	

#ifdef __LYNXSCRIPT_LIB__
	#define	LYNXSCRIPTAPI						
#elif __LYNXSCRIPT_DLL__
	#define	LYNXSCRIPTAPI						LYNXEXPORT
#else
	#define LYNXSCRIPTAPI						LYNXIMPORT
#endif	

namespace LynxScript 
{
	namespace GM 
	{		
		class /*LYNXSCRIPTCLASS*/ CScriptSystem : public LynxEngine::ScriptSystem::CScriptSystem		
		{	
			DECLARE_CLASS(CCScriptSystem);
		protected:						
		public:			
			CScriptSystem();
			virtual ~CScriptSystem();

			LYNXINLINE const char*							vGetAPIString() {return "GM"; };
			LYNXBOOL										vInit() {return LYNX_TRUE; };			
		};	
	}
}

#endif