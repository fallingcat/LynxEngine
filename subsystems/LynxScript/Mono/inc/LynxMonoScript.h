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

#ifndef __LYNXMONOCRIPT_H__
#define __LYNXMONOCRIPT_H__

#include <LuaPlus.h>
#include <ScriptSystem/LynxScript.h>
#include <LynxLuaBuiltinTypes.h>

#ifdef __MONO__

	#include <mono/jit/jit.h>
	#include <mono/metadata/object.h>
	#include <mono/metadata/environment.h>
	#include <mono/metadata/assembly.h>
	#include <mono/metadata/debug-helpers.h>	

	namespace LynxScript 
	{
		namespace Mono 
		{	
			class CScript : public LynxEngine::ScriptSystem::CScript
			{	
				LYNX_DECLARE_CLASS(CScript);
			public:
			protected:					
				MonoClass*							m_Class;
				MonoObject*							m_ClassObj;
				guint32								m_ClassObj_GCHandle;

				//ONCREATE							m_OnCreate;
				//ONINIT								m_OnInit;
				MonoMethod*							m_OnCreate;
				MonoMethod*							m_OnInit;
				MonoMethod*							m_OnLoop;
				MonoMethod*							m_OnRender;
				MonoMethod*							m_OnQuit;
				MonoMethod*							m_OnDestroy;
				MonoMethod*							m_OnHit;
			public:			
				CScript(LynxEngine::ScriptSystem::CScriptSystem* lps);
				virtual ~CScript();		

				LYNXFORCEINLINE MonoObject*			GetClassObj() {return m_ClassObj; }

				virtual void						vBindObject(void* t);

				virtual void						vRegisterClass(const LynxEngine::CAnsiString& name);
				virtual	void						vSetupHandler();			
				virtual LYNXBOOL					vLoad(const LYNXCHAR *filename, LynxEngine::CStream& stream)  {return LYNX_TRUE; }			
				virtual LYNXBOOL					vCompile(const LYNXCHAR *filename, LynxEngine::CStream& stream) {return LYNX_TRUE; }
				virtual LYNXBOOL					vExecute(const LYNXANSICHAR *func) {return LYNX_TRUE; }
				static virtual LYNXBOOL				vEncrypt(const LYNXCHAR *filename) {return LYNX_TRUE;}

				virtual void						vOnCreate();
				virtual void						vOnInit();
				virtual void						vOnLoop(float step);
				virtual void						vOnRender();
				virtual void						vOnQuit();
				virtual void						vOnDestroy()  {}
				virtual void						vOnHit()  {}
			};	
		}
	}

#endif

#endif
