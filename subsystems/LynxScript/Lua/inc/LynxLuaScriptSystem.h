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

//#ifdef __LUA__

#ifndef __LYNXLUASCRIPTSYSTEM_H__
#define __LYNXLUASCRIPTSYSTEM_H__

#include <LynxCore.h>
#include <luapluscd.h>
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

//namespace LPCD
//{
//	inline bool	Match(TypeWrapper<const wchar_t*>, lua_State* L, int idx)
//		{  return lua_type(L, idx) == LUA_TWSTRING;  }
//}

namespace LynxScript 
{
	namespace Lua 
	{		
		class CVMachine;
		class CScriptSystem : public LynxEngine::ScriptSystem::CScriptSystem		
		{	
			LYNX_DECLARE_CLASS(CScriptSystem);
		protected:					
		public:			
			CScriptSystem(LynxEngine::CEngine* const lpengine);
			virtual ~CScriptSystem();

			LYNXINLINE const LYNXCHAR*						vGetAPIString() {return _T("Lua"); };
			LYNXBOOL										vCreate();			
			LYNXBOOL										vOnInit();		
			static LYNXBOOL									EncryptScript(const LYNXCHAR *filename, const char* key = NULL);
		};	
	}
}

#endif

//#endif