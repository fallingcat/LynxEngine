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

#ifndef __LYNXSCRIPTSYSTEM_H__
#define __LYNXSCRIPTSYSTEM_H__

#include <LynxSystem.h>
#include <LynxEngineClasses.h>

#if (defined (__iOS__) || defined (__ANDROID__))
	#define LYNX_SCRIPT_CLASS_PATH				"engine/script/"
	#define LYNX_GAME_SCRIPT_CLASS_PATH			"script/"
#elif defined __WINAPP__
	#define LYNX_SCRIPT_CLASS_PATH				"./engine/script/"
	#define LYNX_GAME_SCRIPT_CLASS_PATH			"./script/"
#else
	//#define LYNX_SCRIPT_CLASS_PATH				"../../script/"
	#define LYNX_SCRIPT_CLASS_PATH				"../../engine/script/"
	#define LYNX_GAME_SCRIPT_CLASS_PATH			"../script/"
#endif

namespace LynxEngine 
{
	namespace ScriptSystem 
	{
		class LYNXENGCLASS CScriptSystem  : public CSystem
		{	
			LYNX_DECLARE_CLASS(CScriptSystem);
		protected:						
			static CVMachine*					m_lpVMachine;
			CList<CString>						m_ScriptPathList;
			CList<CScriptPtr>					m_ScriptList;
			static CScriptPtr					m_NULL_Script;
			LYNXBOOL							m_ScriptPerInstance;
			static CAnsiString					m_EncryptingKey;
		public:			
			CScriptSystem(CEngine* const lpengine);
			virtual ~CScriptSystem();		

			virtual const LYNXCHAR*				vGetAPIString() = 0;

			LYNXINLINE LYNXBOOL					AllocateScriptPerInstance() const {return m_ScriptPerInstance; };
			LYNXINLINE CVMachine*				GetlpVMachine() {return m_lpVMachine; };
			CList<CString>&						GetScriptPathList() {return m_ScriptPathList; };
			CList<CScriptPtr>&					GetScriptList() {return m_ScriptList; };

			LYNXBOOL							CreateScript(CScriptPtr& s);
			void				 				AddScript(const CScriptPtr& s);
			void				 				RemoveScript(const CScriptPtr& s);
			void				 				RemoveScript(const CString& s);
			CScriptPtr&							FindScript(const CString& s);
			static void							SetEncryptingKey(const char *k) {m_EncryptingKey = k; }
			static CAnsiString&					GetEncryptingKey() {return m_EncryptingKey; }
			static  LYNXBOOL					EncryptScript(const LYNXCHAR *filename, const char* key = NULL) {return LYNX_TRUE; }

			LYNXINLINE static void				DestoryVMachine() { LYNXDEL m_lpVMachine; m_lpVMachine = NULL; }
		};	
	}
}

LynxEngine::ScriptSystem::CScriptSystem* CreateScriptSystem(LynxEngine::CEngine* const lpengine);

#endif