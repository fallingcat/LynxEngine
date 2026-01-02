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

#ifndef __LYNXLUACLASS_H__
#define __LYNXLUACLASS_H__

#include <LuaPlus.h>
#include <LynxEngineClasses.h>
#include <LynxType.h>
#include <ScriptSystem/LynxScriptClass.h>

using namespace LuaPlus;

namespace LynxScript 
{
	namespace Lua 
	{			
		class CScriptClass : public LynxEngine::ScriptSystem::CScriptClass
		{	
			LYNX_DECLARE_CLASS(CScriptClass);
		public:
		//protected:						
			LuaObject						m_Class;
			LuaObject						m_FuncTable; 			
		public:			
			CScriptClass(LynxEngine::ScriptSystem::CVMachine* vm);
			virtual ~CScriptClass();			

			void							vRegisterClass(const LynxEngine::CAnsiString& name);
			
			LYNXINLINE void vSetObject(void* t)
			{
				if (m_lpBaseClass)
					m_lpBaseClass->vSetObject(t);
				m_Class.SetLightUserData("__object", t);
				m_Class.SetMetaTable(m_FuncTable);
			}

			//virtual void					vSetup() {m_FuncTable.RegisterObjectDirect(m_MethodList[0]->m_Name.c_str(), m_MethodList[0]->m_Callee, m_MethodList[0]->m_Func, 0);	};

			template <typename Callee, typename Func>
			LYNXINLINE void RegisterMethod(const char* funcName, const Callee* callee, Func func, unsigned int nupvalues = 0)
			{
				m_FuncTable.RegisterObjectDirect(funcName, callee, func, nupvalues);							
			}			

			template <typename Object, typename VarType>
			LYNXINLINE void RegisterVar(const char* varName, VarType Object::* var)
			{
				LPCD::PropertyCreate(m_FuncTable, varName, var);				
			}			
			
		};	

		/*template <typename Callee, typename Func>  
		class LYNXENGTEMPLATE CScriptFunc : public LynxEngine::ScriptSystem::CScriptFunc<Callee, Func>
		{
		public:
			CScriptFunc(const char* n, Callee* callee, Func func):LynxEngine::ScriptSystem::CScriptFunc<Callee, Func>(n, callee, func) {};
			virtual	void		vRegister(LynxEngine::ScriptSystem::CScriptClass* lps) {((CScriptClass*)lps)->m_FuncTable.RegisterObjectDirect(m_Name.c_str(), m_Callee, m_Func); };			
		};*/
	}
}

//template <typename Callee, typename Func>  
//LynxEngine::ScriptSystem::CScriptFunc<Callee, Func>* CreatScriptFunc(const char *name, Callee* callee, Func func) {	return LYNXNEW LynxScript::Lua::CScriptFunc<Callee, Func>(name, callee, func); };

#endif