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

#ifndef __LYNXLUASCRIPT_H__
#define __LYNXLUASCRIPT_H__

#include <LuaPlus.h>
#include <ScriptSystem/LynxScript.h>
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
		class CScript : public LynxEngine::ScriptSystem::CScript
		{	
			LYNX_DECLARE_CLASS(CScript);
		public:
			LuaObject							m_ClassObjectTable;
			LuaObject							m_ClassMetaTable; 			
		protected:
            LuaFunction<int>*                   m_OnConstruct;
			LuaFunction<int>*					m_OnCreate;
			LuaFunction<int>*					m_OnInit;
			LuaFunction<int>*					m_OnLoop;
			LuaFunction<int>*					m_OnRender;
			LuaFunction<int>*					m_OnQuit;
			LuaFunction<int>*					m_OnDestroy;
			LuaFunction<int>*					m_OnHit;
			LynxEngine::CMap<LynxEngine::CAnsiString, LuaFunction<int>*> m_UserFuncMap;
		public:			
			CScript(LynxEngine::ScriptSystem::CScriptSystem* lps);
			virtual ~CScript();		

			void								vRegisterClass(const LynxEngine::CAnsiString& name);

			LYNXINLINE void vBindObject(void* t)
			{
				if (m_lpSuperClass)
					m_lpSuperClass->vBindObject(t);
				m_ClassObjectTable.SetLightUserData("__object", t);
				m_ClassObjectTable.SetMetaTable(m_ClassMetaTable);
			}

			template <typename Callee, typename Func>
			LYNXINLINE void RegisterMethod(const char* funcName, const Callee* callee, Func func, unsigned int nupvalues = 0)
			{
				m_ClassMetaTable.RegisterObjectDirect(funcName, callee, func, nupvalues);							
			}			

			template <typename Object, typename VarType>
			LYNXINLINE void RegisterVar(const char* varName, VarType Object::* var)
			{
				LPCD::PropertyCreate(m_ClassMetaTable, varName, var);				
			}

			virtual void						vSetlpSuperClass(LynxEngine::ScriptSystem::CScriptPtr& base);
			virtual	void						vSetupHandler();
			virtual	void						vAddUserHandler(const LynxEngine::CAnsiString& name);
			virtual LYNXBOOL					vLoad(const LYNXCHAR *filename, LynxEngine::CStream& stream) {return 1; }; 
			virtual LYNXBOOL					vCompile(const LYNXCHAR *filename, LynxEngine::CStream& stream);			
			virtual LYNXBOOL					vExecute(const LYNXANSICHAR *func) {return 1; }; 

            virtual void                        vOnConstruct();
			virtual void						vOnCreate();
			virtual void						vOnInit();
			virtual void						vOnLoop(float step);
			virtual void						vOnRender();
			virtual void						vOnQuit();
			virtual void						vOnDestroy();
			virtual void						vOnHit();
			virtual void						vOnUserFunc(const LynxEngine::CAnsiString& name);
			virtual LYNXBOOL					vHasUserFunc(const LynxEngine::CAnsiString& name);
		};	
	}
}

#endif

//#endif