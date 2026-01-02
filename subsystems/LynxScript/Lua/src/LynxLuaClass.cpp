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

#include <LynxEngine_PCH.h>
#include <LuaPlus.h>
#include <LynxCore.h>
#include <LynxLuaClass.h>
#include <LynxLuaVMachine.h>

using namespace LuaPlus;

namespace LynxScript 
{
	namespace Lua 
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScriptClass::CScriptClass(LynxEngine::ScriptSystem::CVMachine* vm)	
		: LynxEngine::ScriptSystem::CScriptClass(vm)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScriptClass::~CScriptClass(void)
		{			
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScriptClass::vRegisterClass(const LynxEngine::CAnsiString& name)
		{
			m_FuncTable = ((CVMachine*)m_lpVMachine)->GetState()->GetGlobals().CreateTable("FuncTable");
			LPCD::MetaTable_IntegratePropertySupport(m_FuncTable);					
			m_Class = ((CVMachine*)m_lpVMachine)->GetState()->GetGlobals().CreateTable(name.c_str());			
		}						
	}
}