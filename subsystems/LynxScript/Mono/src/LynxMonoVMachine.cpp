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
//#include <LuaPlusAddons.h>
#include <LynxCore.h>
#include <LynxGameObj.h>

#ifdef __MONO__
	#include <mono/jit/jit.h>
	#include <mono/metadata/object.h>
	#include <mono/metadata/environment.h>
	#include <mono/metadata/assembly.h>
	#include <mono/metadata/debug-helpers.h>
	#include <LynxMonoVMachine.h>
	#include <LynxMonoScript.h>
	#include <wrapper/LynxMonoWrappers.h>

	namespace LynxScript 
	{
		namespace Mono 
		{			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CVMachine::CVMachine(LynxEngine::ScriptSystem::CScriptSystem* lps)		
			: LynxEngine::ScriptSystem::CVMachine(lps)
			{
				m_Domain = NULL;
				m_Assembly = NULL;
				m_Image = NULL;
				m_GameAssembly = NULL;
				m_GameImage = NULL;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CVMachine::~CVMachine(void)
			{
				vDestory();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CVMachine::vCreate(int size)		
			{			
				mono_set_dirs("./mono/lib", "./mono/etc");
				
				//mono_config_parse (NULL);				

				m_Domain = mono_jit_init(LYNX_SCRIPT_CLASS_PATH"mono_script.dll");				
				if (!m_Domain)
					return LYNX_FALSE;

				/* Loading an assembly makes the runtime setup everything
				 * needed to execute it. If we're just interested in the metadata
				 * we'd use mono_image_load (), instead and we'd get a MonoImage*.
				 */
				m_Assembly = mono_domain_assembly_open(m_Domain, LYNX_SCRIPT_CLASS_PATH"mono_script.dll");				
				if (!m_Assembly)
					return LYNX_FALSE;
				/*
				 * mono_jit_exec() will run the Main() method in the assembly.
				 * The return value needs to be looked up from
				 * System.Environment.ExitCode.
				 */
				mono_jit_exec(m_Domain, m_Assembly, 0, NULL);

				m_Image = mono_assembly_get_image(m_Assembly);

				LynxEngine::MonoWrapper::InitClasses(m_Image, m_Domain);
				
				return LYNX_TRUE;
			}	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CVMachine::LoadGameImage(LynxEngine::CString& filename)
			{
				m_GameAssembly = mono_domain_assembly_open(m_Domain, LynxEngine::CAnsiString(filename).c_str());
				if (!m_GameAssembly)
					return LYNX_FALSE;

				//mono_jit_exec(m_Domain, m_GameAssembly, 0, NULL);

				m_GameImage = mono_assembly_get_image(m_GameAssembly);

				MonoClass* GlobalVarClass = mono_class_from_name(m_Image, "LynxEngine", "GlobalVar");	
				MonoVTable* GlobalVarVTable = mono_class_vtable(m_Domain, GlobalVarClass);
				MonoClass* GameGlobalVarClass = mono_class_from_name(m_GameImage, "LynxEngine", "GlobalVar");	
				if (GlobalVarClass && GameGlobalVarClass)
				{
					void* Value[1];

					MonoClassField* Field = mono_class_get_field_from_name(GlobalVarClass, "Engine");
					mono_field_static_get_value(GlobalVarVTable, Field, &Value);					
					MonoClassField* GameField = mono_class_get_field_from_name(GameGlobalVarClass, "Engine");
					mono_field_static_set_value(GlobalVarVTable, GameField, &Value);
				}

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CVMachine::vDestory()
			{
				if (m_Domain)
					mono_jit_cleanup(m_Domain);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LynxEngine::ScriptSystem::CScript* const CVMachine::vCreateScript() 
			{
				return LYNXNEW CScript(m_lpScriptSystem); 
			}								
		}
	}
#endif	