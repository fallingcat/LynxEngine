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

//#ifdef __LUA__

#ifndef __LYNXLUAVMACHINE_H__
#define __LYNXLUAVMACHINE_H__

#include <LynxMathWrap.h>
#include <ScriptSystem/LynxVMachine.h>
#include <LynxList.h>
#include <LuaPlus.h>
#include <LynxLuaBuiltinTypes.h>

namespace LuaPlus
{
	class LuaState;
}

using namespace LuaPlus;

namespace LynxScript 
{
	namespace Lua 
	{	
		class CClassObj;
		class CVMachine : public LynxEngine::ScriptSystem::CVMachine
		{	
			LYNX_DECLARE_CLASS(CVMachine);
		public:
			struct LIBDATA {
				LynxEngine::CAnsiString		Name;
				int (*Func)(LuaState*);
			};			
			struct NATIVELIB {
				LuaObject					System;
				LuaObject					Math;				
			};			
		protected:					
			LuaState*						m_State;		
			NATIVELIB						m_NativeLib;
		public:			
			CVMachine(LynxEngine::ScriptSystem::CScriptSystem* lps);
			virtual ~CVMachine();		

			LYNXINLINE LuaState*			GetState() {return  m_State; };
			virtual	LYNXBOOL				vCreate(int size);		
			virtual	void					vDestory();		
			virtual LynxEngine::ScriptSystem::CScript* const  vCreateScript();			
			//virtual LynxEngine::ScriptSystem::CScriptClass* const vCreateClass();				
			virtual LYNXBOOL				vRegisterDefaultClasses();

			virtual void					vRegisterVarType() {};

			virtual void					vRegisterFunction() {};
			virtual void					vRegisterVar() {};

			virtual void					vRegisterClassFunction() {};
			virtual void					vRegisterClassVar() {};

			virtual void					vDumpAllScripts(const LYNXCHAR* filename);

			virtual LYNXBOOL				vIsValid() { return (m_State ? LYNX_TRUE : LYNX_FALSE); }

			void							RegisterFunction();//LynxEngine::CString& s, void* f);
		};	
	}
}

#endif

//#endif