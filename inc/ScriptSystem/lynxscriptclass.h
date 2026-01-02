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

#ifndef __LYNXSCRIPTCLASS_H__
#define __LYNXSCRIPTCLASS_H__

#include <LynxEngineClasses.h>
#include <LynxType.h>
#include <LynxString.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	namespace ScriptSystem 
	{	
		template <typename Callee, typename Func>  
		class LYNXENGTEMPLATE CScriptFunc 
		{
		public:
			CString				m_Name;
			Callee*				m_Callee;
			Func				m_Func;			
		public:
			CScriptFunc(const char* n, Callee* callee, Func func) {m_Name = n; m_Callee = callee; m_Func = func; };

			virtual	void		vRegister(CScriptClass* lps) {};			
		};

		/*template <typename Callee, typename Func>  
		class LYNXENGTEMPLATE CScriptFuncFactory 
		{
		public:
			CScriptFunc<Callee, Func>* (*m_lpCreator)(const char *name, Callee* callee, Func func);

			void						RegisterCreator(CScriptFunc<Callee, Func>* (*func)()) {m_lpCreator = func; };
			CScriptFunc<Callee, Func>*	Create(const char *name, Callee* callee, Func func) {return m_lpCreator(name, callee, func); };
		};*/

		class LYNXENGCLASS CScriptClass : public CObj
		{	
			LYNX_DECLARE_CLASS(CScriptClass);		
		protected:						
			CVMachine*						m_lpVMachine;						
			CScriptClass*					m_lpBaseClass;
			CScriptPtr						m_lpScript;
			//CList<CScriptFunc*>				m_MethodList;						
		public:						
			CScriptClass(CVMachine* vm);
			virtual ~CScriptClass();			

			LYNXINLINE CScriptPtr&			GetlpScript() { return m_lpScript; };
			LYNXINLINE void					SetlpScript(CScriptPtr& s) { m_lpScript = s; };
			LYNXINLINE void					SetlpBaseClass(CScriptClass* base) { m_lpBaseClass = base; };
			LYNXINLINE CScriptClass*		GetlpBaseClass() { return m_lpBaseClass; };
            virtual void					vRegisterClass(const LynxEngine::CAnsiString& name) = 0;

			//template <typename Callee, typename Func> 
			//void RegisterMethod(const char* funcName, const Callee* callee, Func func, unsigned int nupvalues) {m_MethodList.push_back(LYNXNEW CScriptFunc<Callee, Func>(funcName, callee, func)); };		
			//template <typename Object, typename VarType>
			//void RegisterVar(const char* varName, VarType Object::* var) = 0;
			//virtual void					vSetup() = 0;
			virtual void					vSetObject(void* t) = 0;			
		};	
	}
}

template <typename Callee, typename Func>  LynxEngine::ScriptSystem::CScriptFunc<Callee, Func>* CreatScriptFunc(const char *name, Callee* callee, Func func);

#endif