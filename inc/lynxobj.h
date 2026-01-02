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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXOBJ_H__
#define __LYNXOBJ_H__

#include <LynxUnknown.h>
#include <LynxEngineClasses.h>

#include <LynxList.h>
#include <LynxParamManager.h>
#include <LynxMemAllocator.h>

#define SCRIPTEXP_METHOD1(ret, func, t1, p1)				ret func(t1 p1); \
															ret Script_##func(t1 p1) {return func(p1); };	
//#define SCRIPTEXP_METHOD2(ret, func, p1, p2)				ret func(p1, p2);

//#if (defined(__LUA__) || defined(__MONO__))
//	#define INIT_CLASSSCRIPT				\
//		m_ClassScriptClassName = ClassScriptClassName();\
//		if (m_lpEngine->GetlpScriptSystem())\
//		{\
//			/* Load class script*/\
//			LoadClassScript();\
//		}
//
//	#define DECLARE_CLASSSCRIPT	\
//		virtual LYNXBOOL							LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass);\
//		virtual LYNXBOOL							LoadClassScript();\
//		virtual LYNXBOOL							OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname);\
//		virtual LYNXANSICHAR*						ClassScriptClassName();\
//		void										RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps);\
//		void										RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps);\
//
//	#define IMPLEMENT_CLASSSCRIPT(_class, _superclass)	\
//		LYNXBOOL _class::LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass)\
//		{\
//			LynxEngine::ScriptSystem::CScriptPtr lpSuperClass = m_lpEngine->GetlpScriptSystem()->FindScript(#_superclass);\
//			if (!lpSuperClass)\
//			{\
//				LynxEngine::CPlatformFileStream FileStream;\
//				m_lpEngine->GetlpScriptSystem()->CreateScript(lpSuperClass);\
//				if (!lpSuperClass)\
//					return LYNX_FALSE;\
//				lpSuperClass->SetName(#_superclass);\
//				RegisterScriptClass(lpSuperClass, #_superclass);\
//				if (!lpSuperClass->vCompile(LynxEngine::CString(LYNX_GAME_SCRIPT_CLASS_PATH#_superclass".lua"), FileStream))\
//					lpSuperClass->vCompile(LynxEngine::CString(LYNX_SCRIPT_CLASS_PATH#_superclass".lua"), FileStream);\
//			}\
//			lpclass->vSetlpSuperClass(lpSuperClass);\
//			_superclass::LoadSuperClassScript(lpSuperClass);\
//			return LYNX_TRUE;\
//		}\
//		LYNXBOOL _class::LoadClassScript()\
//		{\
//			LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());\
//			LynxEngine::CPlatformFileStream FileStream;\
//			if (m_lpEngine->GetlpScriptSystem()->AllocateScriptPerInstance())\
//			{\
//				if (m_lpClassScript)\
//					m_lpClassScript.Dispose();\
//				CreateClassScript();\
//				if (!m_lpClassScript)\
//					return LYNX_FALSE;\
//				m_lpClassScript->SetName(m_ClassScriptClassName);\
//				RegisterScriptClass(m_lpClassScript, m_ClassScriptClassName);\
//				if (!m_lpClassScript->vCompile(LynxEngine::CString(LYNX_GAME_SCRIPT_CLASS_PATH#_class".lua"), FileStream))\
//					m_lpClassScript->vCompile(LynxEngine::CString(LYNX_SCRIPT_CLASS_PATH#_class".lua"), FileStream);\
//				m_lpClassScript->vSetupHandler();\
//				m_lpClassScript->vBindObject((CObj*)this);\
//				return LYNX_TRUE;\
//			}\
//			else\
//			{\
//				m_lpClassScript = m_lpEngine->GetlpScriptSystem()->FindScript(m_ClassScriptClassName);\
//				if (!m_lpClassScript) /*if this script class is already loaded into the system <如果這個script類別已經被載入系統>*/\
//				{\
//					CreateClassScript();\
//					if (!m_lpClassScript)\
//						return LYNX_FALSE;\
//					m_lpClassScript->SetName(m_ClassScriptClassName);\
//					RegisterScriptClass(m_lpClassScript, m_ClassScriptClassName);\
//					if (!m_lpClassScript->vCompile(LynxEngine::CString(LYNX_GAME_SCRIPT_CLASS_PATH#_class".lua"), FileStream))\
//						m_lpClassScript->vCompile(LynxEngine::CString(LYNX_SCRIPT_CLASS_PATH#_class".lua"), FileStream);\
//					m_lpClassScript->vSetupHandler();\
//					m_lpClassScript->vBindObject(this);\
//				}\
//				return LoadSuperClassScript(m_lpClassScript);\
//			}\
//		}\
//		LYNXBOOL _class::OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname)\
//		{\
//			LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());\
//			LynxEngine::ScriptSystem::CScriptPtr lpOverloadedClassScript;/*m_lpEngine->GetlpScriptSystem()->FindScript(scriptname);*/\
//			if (!lpOverloadedClassScript) /*if this script class is already loaded into the system <如果這個script類別已經被載入系統>*/\
//			{\
//				LynxEngine::CPlatformFileStream FileStream;\
//				if (m_lpEngine->GetlpScriptSystem()->AllocateScriptPerInstance())\
//				{\
//					if (m_lpClassScript)\
//						m_lpClassScript.Dispose();\
//					m_lpEngine->GetlpScriptSystem()->CreateScript(lpOverloadedClassScript);\
//					if (!lpOverloadedClassScript)\
//						return LYNX_FALSE;\
//					m_ClassScriptClassName = scriptname;\
//					lpOverloadedClassScript->SetName(scriptname);\
//					RegisterScriptClass(lpOverloadedClassScript, scriptname);\
//					lpOverloadedClassScript->vCompile(scriptfilename, FileStream);\
//					lpOverloadedClassScript->vSetupHandler();\
//					lpOverloadedClassScript->vBindObject((CObj*)this);\
//				}\
//				lpOverloadedClassScript->vSetlpSuperClass(m_lpClassScript);\
//				m_lpClassScript = lpOverloadedClassScript;\
//				return LYNX_TRUE;/*LoadSuperClassScript(m_lpClassScript);*/\
//			}\
//			else\
//			{\
//				lpOverloadedClassScript = m_lpEngine->GetlpScriptSystem()->FindScript(scriptname);\
//				if (!lpOverloadedClassScript) /*if this script class is already loaded into the system <如果這個script類別已經被載入系統>*/\
//				{\
//					LynxEngine::CPlatformFileStream FileStream;\
//					m_lpEngine->GetlpScriptSystem()->CreateScript(lpOverloadedClassScript);\
//					if (!lpOverloadedClassScript)\
//						return LYNX_FALSE;\
//					m_ClassScriptClassName = scriptname;\
//					lpOverloadedClassScript->SetName(scriptname);\
//					RegisterScriptClass(lpOverloadedClassScript, scriptname);\
//					lpOverloadedClassScript->vCompile(scriptfilename, FileStream);\
//					lpOverloadedClassScript->vSetupHandler();\
//					lpOverloadedClassScript->vBindObject(this);\
//				}\
//				lpOverloadedClassScript->vSetlpSuperClass(m_lpClassScript);\
//				m_lpClassScript = lpOverloadedClassScript;\
//				return LoadSuperClassScript(m_lpClassScript);\
//			}\
//		}\
//		LYNXANSICHAR* _class::ClassScriptClassName()\
//		{\
//			return #_class;\
//		}\
//		void _class::RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps)\
//		{\
//			_superclass::vRegisterScriptClassMethod(lps);\
//		}\
//		void _class::RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps)\
//		{\
//			_superclass::vRegisterScriptClassVar(lps);\
//		}\
//
//	#define REGISTER_SCRIPT_VAR(s, name, classname, var)\
//		((LynxScript::Lua::CScript*)s)->RegisterVar(name, &classname::var);						
//	#define REGISTER_SCRIPT_METHOD(s, name, classname, rt, func, arg)\
//		((LynxScript::Lua::CScript*)s)->RegisterMethod(name, (classname*)0, (rt (classname::*)arg)&classname::func);									
#if defined(__LUA__)
	#define INIT_CLASSSCRIPT				\
		m_ClassScriptClassName = ClassScriptClassName();\
		if (m_lpEngine->GetlpScriptSystem())\
		{\
			/* Load class script*/\
			LoadClassScript();\
		}

	#define DECLARE_CLASSSCRIPT	\
		virtual LYNXBOOL							LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass);\
		virtual LYNXBOOL							LoadClassScript();\
		virtual LYNXBOOL							OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname);\
		virtual LYNXANSICHAR*						ClassScriptClassName();\
		void										RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps);\
		void										RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps);\

	#define IMPLEMENT_CLASSSCRIPT(_class, _superclass)	\
		LYNXBOOL _class::LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass)\
		{\
			if (!m_lpEngine->GetlpScriptSystem())\
				return LYNX_FALSE;\
			LynxEngine::ScriptSystem::CScriptPtr lpSuperClass = m_lpEngine->GetlpScriptSystem()->FindScript(#_superclass);\
			if (!lpSuperClass)\
			{\
				LynxEngine::CPlatformFileStream FileStream;\
				m_lpEngine->GetlpScriptSystem()->CreateScript(lpSuperClass);\
				if (!lpSuperClass)\
					return LYNX_FALSE;\
				lpSuperClass->SetName(#_superclass);\
				RegisterScriptClass(lpSuperClass, #_superclass);\
				for (LynxEngine::CList<LynxEngine::CString>::CIterator SrcPath = glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().begin(); SrcPath != glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().end(); SrcPath++)\
				{\
					LynxEngine::CString Path = (*SrcPath) + CString(#_superclass".lua");\
					if (lpSuperClass->vCompile(Path, FileStream))\
					{\
						break;\
					}\
				}\
			}\
			lpclass->vSetlpSuperClass(lpSuperClass);\
			_superclass::LoadSuperClassScript(lpSuperClass);\
			return LYNX_TRUE;\
		}\
		LYNXBOOL _class::LoadClassScript()\
		{\
			/*LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());*/\
			if (!m_lpEngine->GetlpScriptSystem())\
				return LYNX_FALSE;\
			m_lpClassScript = m_lpEngine->GetlpScriptSystem()->FindScript(m_ClassScriptClassName);\
			if (!m_lpClassScript) /*if this script class is already loaded into the system <如果這個script類別已經被載入系統>*/\
			{\
				LynxEngine::CPlatformFileStream FileStream;\
				CreateClassScript();\
				if (!m_lpClassScript)\
					return LYNX_FALSE;\
				m_lpClassScript->SetName(m_ClassScriptClassName);\
				RegisterScriptClass(m_lpClassScript, m_ClassScriptClassName);\
				for (LynxEngine::CList<LynxEngine::CString>::CIterator SrcPath = glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().begin(); SrcPath != glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().end(); SrcPath++)\
				{\
					LynxEngine::CString Path = (*SrcPath) + CString(#_class".lua");\
					if (m_lpClassScript->vCompile(Path, FileStream))\
					{\
						break;\
					}\
				}\
				m_lpClassScript->vSetupHandler();\
				m_lpClassScript->vBindObject(this);\
			}\
			return LoadSuperClassScript(m_lpClassScript);\
		}\
		LYNXBOOL _class::OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname)\
		{\
			/*LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());*/\
			if (!m_lpEngine->GetlpScriptSystem())\
				return LYNX_FALSE;\
			LynxEngine::ScriptSystem::CScriptPtr lpOverloadedClassScript = m_lpEngine->GetlpScriptSystem()->FindScript(scriptname);\
			if (!lpOverloadedClassScript) /*if this script class is already loaded into the system <如果這個script類別已經被載入系統>*/\
			{\
				LynxEngine::CPlatformFileStream FileStream;\
				m_lpEngine->GetlpScriptSystem()->CreateScript(lpOverloadedClassScript);\
				if (!lpOverloadedClassScript)\
					return LYNX_FALSE;\
				m_ClassScriptClassName = scriptname;\
				lpOverloadedClassScript->SetName(scriptname);\
				RegisterScriptClass(lpOverloadedClassScript, scriptname);\
				lpOverloadedClassScript->vCompile(scriptfilename, FileStream);\
				lpOverloadedClassScript->vSetupHandler();\
				lpOverloadedClassScript->vBindObject(this);\
			}\
			lpOverloadedClassScript->vSetlpSuperClass(m_lpClassScript);\
			m_lpClassScript = lpOverloadedClassScript;\
			return LoadSuperClassScript(m_lpClassScript);\
		}\
		LYNXANSICHAR* _class::ClassScriptClassName()\
		{\
			return (LYNXANSICHAR*)(#_class);\
		}\
		void _class::RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps)\
		{\
			_superclass::vRegisterScriptClassMethod(lps);\
		}\
		void _class::RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps)\
		{\
			_superclass::vRegisterScriptClassVar(lps);\
		}\

	#define REGISTER_SCRIPT_VAR(s, name, classname, var)\
		if (s) ((LynxScript::Lua::CScript*)s)->RegisterVar(name, &classname::var);						
	#define REGISTER_SCRIPT_VAR_S(s, classname, var)\
	if (s) ((LynxScript::Lua::CScript*)s)->RegisterVar(#var, &classname::var);						
	#define REGISTER_SCRIPT_METHOD(s, name, classname, rt, func, arg)\
		if (s) ((LynxScript::Lua::CScript*)s)->RegisterMethod(name, (classname*)0, (rt (classname::*)arg)&classname::func);									
#elif defined(__MONO__)
	#define INIT_CLASSSCRIPT				\
		m_ClassScriptClassName = ClassScriptClassName();\
		if (m_lpEngine->GetlpScriptSystem())\
		{\
			/* Load class script*/\
			LoadClassScript();\
		}

	#define DECLARE_CLASSSCRIPT	\
		virtual LYNXBOOL							LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass);\
		virtual LYNXBOOL							LoadClassScript();\
		virtual LYNXBOOL							OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname);\
		virtual LYNXANSICHAR*						ClassScriptClassName();\
		void										RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps);\
		void										RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps);\

	#define IMPLEMENT_CLASSSCRIPT(_class, _superclass)	\
		LYNXBOOL _class::LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass)\
		{\
			return LYNX_TRUE;\
		}\
		LYNXBOOL _class::LoadClassScript()\
		{\
			LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());\
			LynxEngine::CPlatformFileStream FileStream;\
			if (m_lpClassScript)\
				m_lpClassScript.Dispose();\
			CreateClassScript();\
			if (!m_lpClassScript)\
				return LYNX_FALSE;\
			m_lpClassScript->SetName(m_ClassScriptClassName);\
			RegisterScriptClass(m_lpClassScript, m_ClassScriptClassName);\
			for (LynxEngine::CList<LynxEngine::CString>::CIterator SrcPath = glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().begin(); SrcPath != glpKernel->GetlpEngine()->GetlpScriptSystem()->GetScriptPathList().end(); SrcPath++)\
			{\
				LynxEngine::CString Path = (*SrcPath) + CString(#_class".cs");\
				if (!m_lpClassScript->vCompile(Path, FileStream))\
				{\
					break;\
				}\
			}\
			m_lpClassScript->vSetupHandler();\
			m_lpClassScript->vBindObject((CObj*)this);\
			return LYNX_TRUE;\
		}\
		LYNXBOOL _class::OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname)\
		{\
			LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());\
			LynxEngine::ScriptSystem::CScriptPtr lpOverloadedClassScript;\
			LynxEngine::CPlatformFileStream FileStream;\
			if (m_lpClassScript)\
				m_lpClassScript.Dispose();\
			m_lpEngine->GetlpScriptSystem()->CreateScript(lpOverloadedClassScript);\
			if (!lpOverloadedClassScript)\
				return LYNX_FALSE;\
			m_ClassScriptClassName = scriptname;\
			lpOverloadedClassScript->SetName(scriptname);\
			RegisterScriptClass(lpOverloadedClassScript, scriptname);\
			lpOverloadedClassScript->vCompile(scriptfilename, FileStream);\
			lpOverloadedClassScript->vSetupHandler();\
			lpOverloadedClassScript->vBindObject((CObj*)this);\
			lpOverloadedClassScript->vSetlpSuperClass(m_lpClassScript);\
			m_lpClassScript = lpOverloadedClassScript;\
			return LYNX_TRUE;\
		}\
		LYNXANSICHAR* _class::ClassScriptClassName()\
		{\
			return #_class;\
		}\
		void _class::RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps)\
		{\
			_superclass::vRegisterScriptClassMethod(lps);\
		}\
		void _class::RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps)\
		{\
			_superclass::vRegisterScriptClassVar(lps);\
		}\

	#define REGISTER_SCRIPT_VAR(s, name, classname, var)
	#define REGISTER_SCRIPT_METHOD(s, name, classname, rt, func, arg)
#else
	#define INIT_CLASSSCRIPT
	#define DECLARE_CLASSSCRIPT \
		virtual LYNXBOOL							LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass) {return LYNX_TRUE; }\
		virtual LYNXBOOL							LoadClassScript() {return LYNX_TRUE; }\
		virtual LYNXBOOL							OverloadClassScript(const LynxEngine::CString& scriptfilename, const LynxEngine::CAnsiString& scriptname) {return LYNX_TRUE; }\
		virtual LYNXANSICHAR*						ClassScriptClassName() {return (LYNXANSICHAR*)(""); }\
		void										RegisterSuperClassMethod(LynxEngine::ScriptSystem::CScript* const lps) {}\
		void										RegisterSuperClassVar(LynxEngine::ScriptSystem::CScript* const lps) {}\

	#define IMPLEMENT_CLASSSCRIPT(_class, _superclass)
	#define REGISTER_SCRIPT_VAR(s, name, classname, var)
	#define REGISTER_SCRIPT_METHOD(s, name, classname, func, rt, arg)
#endif

namespace LynxEngine 
{
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CObj : public CUnknown
	{	
		LYNX_DECLARE_CLASS(CObj);
	public:		
		static const int					MAX_STRING_LEN = 128;
	protected:			
		DECLAR_PARAM_MANAGER

		CEngine*							m_lpEngine;
		OBJID								m_ID;	
		LYNXCRC								m_CRCCode;		

		DECLAR_PARAM_STRING(m_Name)		

		// the default script of this class
		LynxEngine::CAnsiString							m_ClassScriptClassName;
		LynxEngine::ScriptSystem::CScriptPtr			m_lpClassScript;
		
		// overloading script of this class
		//LynxEngine::CAnsiString						m_ScriptClassName;
		//LynxEngine::ScriptSystem::CScript*			m_lpScript;				
	public:			
		CObj();
		CObj(CEngine* lpe);
		virtual ~CObj();				
		
		bool								operator ==(const LynxEngine::CString& name) const {return (m_Name == name); };					
		bool								operator ==(const OBJID id) const {return (m_ID == id); };					

		virtual void						vInstance(const CObj& rhs);
				
		LYNXINLINE void						SetlpEngine(CEngine* const lpengine) {m_lpEngine = lpengine;};		
		LYNXINLINE CEngine*					GetlpEngine() const {return m_lpEngine;};
		
		LYNXINLINE void						SetID(OBJID id) {m_ID = id; };
		LYNXINLINE const OBJID				GetID() const { return m_ID; };		

		virtual LYNXBOOL					vSave(CStream& s);
		virtual LYNXBOOL					vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE);

		virtual LYNXBOOL					vLoad(CUserProfile* file, CXMLElement* c);
		virtual LYNXBOOL					vSave(CUserProfile* file, CXMLElement* c);

		LYNXINLINE const LYNXCRC			GetCRCCode() const { return m_CRCCode; };

		LYNXINLINE const LYNXCRC			ComputeCRCCode() { m_CRCCode = ComputeCRCCode(m_Name); return m_CRCCode; };
		static const LYNXCRC				ComputeCRCCode(const LynxEngine::CString& str);

		LYNXINLINE const LynxEngine::CString&			GetName() const {return m_Name; };
		LYNXINLINE const LYNXCHAR*			_GetName() const {return m_Name.c_str(); };
		LYNXINLINE void						SetName(const LynxEngine::CString& name) {m_Name = name; ComputeCRCCode(); };				
		LYNXINLINE void						SetName(const LYNXCHAR* name) {m_Name = name; ComputeCRCCode(); };						
		CObj&								operator =(const CObj& rhs) {m_ID = rhs.m_ID; m_Name = rhs.m_Name; return *this; };		
		void								vDispose(DWORD count);
		#ifdef __TOOL__
			LYNXINLINE CParamManager&		GetParamManager() {return m_ParamManager; };
		#endif

		// Event handling functions
        virtual void                        vConstruct();
		virtual LYNXBOOL                    vCreate();
		virtual LYNXBOOL					vInit();
		virtual void						vLoop(float step);
		virtual void						vRender(CCameraContainer* cam = NULL);
		virtual LYNXBOOL					vDestroy();
		virtual void						vSerialize(CStream& stream) {};
		virtual void						vDeserialize(CStream& stream) {};

		// Scripting ------------------------------------------------------------------------------------------------------------------
		const LynxEngine::CAnsiString&					GetClassScriptClassName() {return m_ClassScriptClassName; };		
		void								CreateClassScript();	
		LYNXINLINE LynxEngine::ScriptSystem::CScript*	GetlpClassScript() {return m_lpClassScript; };
		LYNXBOOL							LoadSuperClassScript(LynxEngine::ScriptSystem::CScript* lpclass);
		
		void								ScriptBindObject();
		virtual LYNXBOOL					vRegisterScriptClassTable(LynxEngine::ScriptSystem::CScript* const lps, const LynxEngine::CAnsiString& classname);
		virtual LYNXBOOL					vRegisterScriptClassMethod(LynxEngine::ScriptSystem::CScript* const lps);
		virtual LYNXBOOL					vRegisterScriptClassVar(LynxEngine::ScriptSystem::CScript* const lps);
		LYNXBOOL							RegisterScriptClass(LynxEngine::ScriptSystem::CScript* const lps, const LynxEngine::CAnsiString& classname);						

		void								AddUserScriptFunc(const CAnsiString& name);
		void								OnUserScriptFunc(const CAnsiString& name);
	};		
	LYNXFORCEINLINE bool operator ==(const CObj* pobj, const LynxEngine::CString& name) {return ((pobj) ? ((*pobj) == name) : false); };
	LYNXFORCEINLINE bool operator ==(const CObj* pobj, LynxEngine::CString& name) {return ((pobj) ? ((*pobj) == name) : false); };
}

#endif