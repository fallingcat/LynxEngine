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
#include <LynxAlgorithm.h>
#include <ScriptSystem/LynxVMachine.h>

namespace LynxEngine 
{	
	namespace ScriptSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVMachine::CVMachine(CScriptSystem* lps)
		: CObj(lps->GetlpEngine())
		{		
			m_lpScriptSystem = lps;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVMachine::~CVMachine(void)
		{			
		}		
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//CScriptClass* CVMachine::AddClass(const LynxEngine::CString& s)
		//{
		//	CScriptClass* lpClass = FindClass(s);
		//	if (!lpClass)
		//	{
		//		lpClass = vCreateClass();
		//		lpClass->SetName(s);
		//		m_ClassList.push_back(lpClass);
		//	}
  //          return lpClass;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//CScriptClass* CVMachine::FindClass(const LynxEngine::CString& s)
		//{
		//	CList<CScriptClass*>::CIterator Class;

		//	Class = LynxEngine::find(m_ClassList.begin(), m_ClassList.end(), s);
		//	if (Class != m_ClassList.end())
		//	{			
		//		return (*Class);
		//	}
		//	return NULL;
		//}
	}
}