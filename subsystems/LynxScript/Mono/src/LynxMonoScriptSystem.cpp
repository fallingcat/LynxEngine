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
#include <LynxCore.h>

#ifdef __MONO__
	#include <LynxMonoScriptSystem.h>
	#include <LynxMonoVMachine.h>

	namespace LynxScript 
	{
		namespace Mono 
		{	
			//CResource* CScriptSystem::CreateScript() { return LYNXNEW GraphicsSystem::CVertexArray; }
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScriptSystem::CScriptSystem(LynxEngine::CEngine* const lpengine)		
			: LynxEngine::ScriptSystem::CScriptSystem(lpengine)
			{			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScriptSystem::~CScriptSystem(void)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScriptSystem::vCreate()		
			{
				m_lpVMachine = LYNXNEW CVMachine(this);
				if (!m_lpVMachine)
					return LYNX_FALSE;

				if (!m_lpVMachine->vCreate(1024))
					return LYNX_FALSE;

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScriptSystem::vOnInit()		
			{			
				return LYNX_TRUE;
			}			
		}
	}
#endif