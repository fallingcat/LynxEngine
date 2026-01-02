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

#include <LynxEngine_PCH.h>

namespace LynxEngine 
{	
	namespace ScriptSystem 
	{		
		CAnsiString			CScriptSystem::m_EncryptingKey = "ugI6{}#/n7""4.@Eh]+yX!Kb|";
		CScriptPtr			CScriptSystem::m_NULL_Script((ScriptSystem::CScript*)(NULL));				
		CVMachine*			CScriptSystem::m_lpVMachine;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScriptSystem::CScriptSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{			
			m_lpVMachine = NULL;			
			m_ScriptPathList.clear();
			#ifdef __MOBILE__
				m_ScriptPathList.push_back(CString(_T("script/")));
				m_ScriptPathList.push_back(CString(_T("engine/script/")));			
			#else				
				m_ScriptPathList.push_back(CString(_T("../script/")));
				m_ScriptPathList.push_back(CString(_T("../../engine/script/")));			
				m_ScriptPathList.push_back(CString(_T("../engine/script/")));			
			#endif
			
			m_ScriptList.clear();
			m_ScriptPerInstance = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScriptSystem::~CScriptSystem(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));			

			// Need to release unused script before releasing virtual machine
			/*
			CList<CScriptPtr>::CIterator Script;
			for (Script=m_ScriptList.begin(); Script != m_ScriptList.end(); Script++)
			{
				GetlpEngine()->DestroyResource((*Script)->GetName());			
				(*Script).Reset();
			}
			*/
			m_ScriptList.clear();

			DestoryVMachine();

			m_lpEngine->GC();			
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CScriptSystem::CreateScript(CScriptPtr& s)
		{
			m_lpEngine->CreateResource(s, SCRIPT);
			AddScript(s);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CScriptSystem::AddScript(const CScriptPtr& s)
		{
			m_ScriptList.push_back(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CScriptSystem::RemoveScript(const CScriptPtr& s)
		{
			m_ScriptList.remove(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CScriptSystem::RemoveScript(const CString& s)
		{
			CScriptPtr& Script = FindScript(s);
			if (Script)
				RemoveScript(Script);			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		CScriptPtr& CScriptSystem::FindScript(const CString& s)
		{
			CList<CScriptPtr>::CIterator Script;

			Script = LynxEngine::find(m_ScriptList.begin(), m_ScriptList.end(), s);
			if (Script != m_ScriptList.end())
			{			
				return (*Script);
			}
			return m_NULL_Script;
		}
	}
}