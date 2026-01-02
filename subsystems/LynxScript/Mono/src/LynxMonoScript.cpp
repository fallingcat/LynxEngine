//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
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
	#include <LynxMonoScript.h>
	#include <LynxMonoVMachine.h>
	#include <wrapper/LynxMonoWrappers.h>

	namespace LynxScript 
	{
		namespace Mono 
		{	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScript::CScript(LynxEngine::ScriptSystem::CScriptSystem* lps)		
			: LynxEngine::ScriptSystem::CScript(lps)
			{		
				m_Class = NULL;
				m_ClassObj = NULL;
				m_ClassObj_GCHandle = NULL;

				m_OnCreate = NULL;
				m_OnInit = NULL;
				m_OnLoop = NULL;
				m_OnRender = NULL;
				m_OnQuit = NULL;
				m_OnDestroy = NULL;
				m_OnHit = NULL;				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScript::~CScript(void)
			{
				if (m_ClassObj_GCHandle)
					mono_gchandle_free(m_ClassObj_GCHandle);
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vRegisterClass(const LynxEngine::CAnsiString& name)
			{
				m_Class = mono_class_from_name(((Mono::CVMachine*)(m_lpScriptSystem->GetlpVMachine()))->GetImage(), "LynxEngine", name.c_str());
				if (!m_Class && ((Mono::CVMachine*)(m_lpScriptSystem->GetlpVMachine()))->GetGameImage())
					m_Class = mono_class_from_name(((Mono::CVMachine*)(m_lpScriptSystem->GetlpVMachine()))->GetGameImage(), "LynxEngine", name.c_str());

				if (m_Class)
				{
					m_ClassObj = mono_object_new(((Mono::CVMachine*)(m_lpScriptSystem->GetlpVMachine()))->GetDomain(), m_Class);
					if (m_ClassObj)
					{
						/* mono_object_new () only allocates the storage: 
						 * it doesn't run any constructor. Tell the runtime to run
						 * the default argumentless constructor.
						 */
						mono_runtime_object_init(m_ClassObj);
						// Prevent GC to collect this object
						m_ClassObj_GCHandle = mono_gchandle_new(m_ClassObj, true);
					}
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vBindObject(void* t) 
			{
				if (m_ClassObj)
				{
					CObj* lpObj = (CObj*)t;
					unsigned long Ptr = (unsigned long)(t);
					MonoClass* ObjClass = mono_class_from_name(((Mono::CVMachine*)(m_lpScriptSystem->GetlpVMachine()))->GetImage(), "LynxEngine", "CObj");	
					if (ObjClass)
					{
						MonoMethod* Method = mono_class_get_method_from_name(ObjClass, "SetPtr", 1);
						void* Param[1];
						Param[0] = &Ptr;
						mono_runtime_invoke(Method, m_ClassObj, Param, NULL);					
					}
				}
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vSetupHandler()
			{
				MonoMethod* lpMethod = NULL;

				if (m_Class)
				{
					lpMethod = mono_class_get_method_from_name(m_Class, "OnCreate", 0);
					if (lpMethod)
						m_OnCreate = lpMethod;//(ONCREATE)mono_method_get_unmanaged_thunk(lpMethod);

					lpMethod = mono_class_get_method_from_name(m_Class, "OnInit", 0);
					if (lpMethod)
						m_OnInit = lpMethod;//(ONINIT)mono_method_get_unmanaged_thunk(lpMethod);

					lpMethod = mono_class_get_method_from_name(m_Class, "OnLoop", 1);
					if (lpMethod)
						m_OnLoop = lpMethod;//(ONINIT)mono_method_get_unmanaged_thunk(lpMethod);

					lpMethod = mono_class_get_method_from_name(m_Class, "OnRender", 0);
					if (lpMethod)
						m_OnRender = lpMethod;//(ONINIT)mono_method_get_unmanaged_thunk(lpMethod);

					lpMethod = mono_class_get_method_from_name(m_Class, "OnQuit", 0);
					if (lpMethod)
						m_OnQuit = lpMethod;//(ONINIT)mono_method_get_unmanaged_thunk(lpMethod);


					/*
					LuaObject Obj = ((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->GetGlobal(Name.c_str());
					if (!Obj.IsNil())
					{
						LuaPlus::LuaObject LuaObj;

						LuaObj = Obj.GetByName("OnCreate");
						if (LuaObj.IsFunction())
						{
							if (m_OnCreate)
							{
								LYNXDEL m_OnCreate;
								m_OnCreate = NULL;
							}
							m_OnCreate = LYNXNEW LuaFunction<int>(LuaObj);	
						}
						LuaObj = Obj.GetByName("OnInit");
						if (LuaObj.IsFunction())
						{
							if (m_OnInit)
							{
								LYNXDEL m_OnInit;
								m_OnInit = NULL;
							}
							m_OnInit = LYNXNEW LuaFunction<int>(LuaObj);	
						}		
						LuaObj = Obj.GetByName("OnLoop");
						if (LuaObj.IsFunction())
						{
							if (m_OnLoop)
							{
								LYNXDEL m_OnLoop;
								m_OnLoop = NULL;
							}
							m_OnLoop = LYNXNEW LuaFunction<int>(LuaObj);	
						}
						LuaObj = Obj.GetByName("OnRender");
						if (LuaObj.IsFunction())
						{
							if (m_OnRender)
							{
								LYNXDEL m_OnRender;
								m_OnRender = NULL;
							}
							m_OnRender = LYNXNEW LuaFunction<int>(LuaObj);	
						}
						LuaObj = Obj.GetByName("OnQuit");
						if (LuaObj.IsFunction())
						{
							if (m_OnQuit)
							{
								LYNXDEL m_OnQuit;
								m_OnQuit = NULL;
							}
							m_OnQuit = LYNXNEW LuaFunction<int>(LuaObj);
						}
						LuaObj = Obj.GetByName("OnDestroy");
						if (LuaObj.IsFunction())
						{
							if (m_OnDestroy)
							{
								LYNXDEL m_OnDestroy;
								m_OnDestroy = NULL;
							}
							m_OnDestroy = LYNXNEW LuaFunction<int>(LuaObj);
						}
						LuaObj = Obj.GetByName("OnHit");
						if (LuaObj.IsFunction())
						{
							if (m_OnHit)
							{
								LYNXDEL m_OnHit;
								m_OnHit = NULL;
							}
							m_OnHit = LYNXNEW LuaFunction<int>(LuaObj);
						}
					}	
					*/
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnCreate()
			{	
				if (m_OnCreate)
					//return m_OnCreate();
					mono_runtime_invoke(m_OnCreate, m_ClassObj, NULL, NULL);					
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnInit()
			{	
				if (m_OnInit)
					//return m_OnInit();
					mono_runtime_invoke(m_OnInit, m_ClassObj, NULL, NULL);					
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnLoop(float step)
			{	
				static void* Param[1];
				static float Step;

				if (m_OnLoop)
				{
					//return m_OnInit();
					Step = step;
					Param[0] = &Step;
					mono_runtime_invoke(m_OnLoop, m_ClassObj, Param, NULL);					
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnRender()
			{	
				if (m_OnRender)
				{
					mono_runtime_invoke(m_OnRender, m_ClassObj, NULL, NULL);					
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnQuit()
			{	
				if (m_OnQuit)
				{
					mono_runtime_invoke(m_OnQuit, m_ClassObj, NULL, NULL);					
				}
			}
		}		
	}
#endif
