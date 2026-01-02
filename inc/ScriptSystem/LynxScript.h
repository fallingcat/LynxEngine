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

#ifndef __LYNXSCRIPT_H__
#define __LYNXSCRIPT_H__

#include <LynxEngineClasses.h>
#include <LynxResource.h>

namespace LynxEngine 
{
	namespace ScriptSystem 
	{
		/*
		template <typename Callee, typename Func>  
		class LYNXENGTEMPLATE CScriptFunc 
		{
		public:
			CString				m_Name;
			Callee*				m_Callee;
			Func				m_Func;			
		public:
			CScriptFunc(const char* n, Callee* callee, Func func) {m_Name = n; m_Callee = callee; m_Func = func; };

			virtual	void		vRegister(CScript* lps) {};			
		};
		*/		

		class LYNXENGCLASS CScript : public CResource
		{	
			LYNX_DECLARE_CLASS(CScript);
		public:
            typedef void (*ONCONSTRUCT)(void);
			typedef void (*ONCREATE)(void);
			typedef void (*ONINIT)(void);
			typedef void (*ONLOOP)(float);
			typedef void (*ONRENDER)(void);
			typedef void (*ONQUIT)(void);
			typedef void (*ONDESTORY)(void);
			typedef void (*ONHIT)(void);
		protected:		
			CScriptSystem*						m_lpScriptSystem;
			CScriptPtr							m_lpSuperClass;	
		public:			
			CScript(CScriptSystem* lps);
			virtual ~CScript();					
			
			virtual LYNXINLINE void				vSetlpSuperClass(CScriptPtr& base) { m_lpSuperClass = base; };
			LYNXINLINE CScriptPtr&				GetlpSuperClass() { return m_lpSuperClass; };
            virtual void						vRegisterClass(const LynxEngine::CAnsiString& name) = 0;
			//template <typename Callee, typename Func> 
			//void								RegisterMethod(const char* funcName, const Callee* callee, Func func, unsigned int nupvalues) {m_MethodList.push_back(LYNXNEW CScriptFunc<Callee, Func>(funcName, callee, func)); };		
			//template <typename Object, typename VarType>
			//void								RegisterVar(const char* varName, VarType Object::* var) = 0;
			virtual void						vBindObject(void* t) = 0;			

			virtual	void						vSetupHandler() = 0;
			virtual	void						vAddUserHandler(const CAnsiString& name) = 0;
			virtual LYNXBOOL					vLoad(const LYNXCHAR *filename, CStream& stream) = 0; 
			virtual LYNXBOOL					vCompile(const LYNXCHAR *filename, CStream& stream) = 0;			
			virtual LYNXBOOL					vExecute(const LYNXANSICHAR *func) = 0;

            virtual void                        vOnConstruct() = 0;
			virtual void						vOnCreate() = 0;
			virtual void						vOnInit() = 0;
			virtual void						vOnLoop(float step) = 0;
			virtual void						vOnRender() = 0;
			virtual void						vOnQuit() = 0;
			virtual void						vOnDestroy() = 0;
			virtual void						vOnHit() = 0;
			virtual void						vOnUserFunc(const CAnsiString& name) = 0;
			virtual LYNXBOOL					vHasUserFunc(const CAnsiString& name) = 0;
		};	
		//template <typename Callee, typename Func>  CScriptFunc<Callee, Func>* CreatScriptFunc(const char *name, Callee* callee, Func func);
	}	
}

#endif