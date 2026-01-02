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

#ifdef __LUA__

	#include <LynxCore.h>
	#include <LynxLuaScript.h>
	#include <LynxLuaVMachine.h>

	#if ((defined(DEBUG) || defined(_DEBUG)))
		#define __LUA_DEBUG__					0
	#else
		#define __LUA_DEBUG__					0
	#endif

	namespace LynxScript 
	{
		namespace Lua 
		{	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScript::CScript(LynxEngine::ScriptSystem::CScriptSystem* lps)		
			: LynxEngine::ScriptSystem::CScript(lps)
			{
                m_OnConstruct = NULL;
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
                if (m_OnConstruct)
                    LYNXGLOBAL_DEL(m_OnConstruct);
				if (m_OnCreate)
					LYNXGLOBAL_DEL(m_OnCreate);
				if (m_OnInit)
					LYNXGLOBAL_DEL(m_OnInit);
				if (m_OnLoop)
					LYNXGLOBAL_DEL(m_OnLoop);
				if (m_OnRender)
					LYNXGLOBAL_DEL(m_OnRender);
				if (m_OnQuit)
					LYNXGLOBAL_DEL(m_OnQuit);
				if (m_OnDestroy)
					LYNXGLOBAL_DEL(m_OnDestroy);
				if (m_OnHit)
					LYNXGLOBAL_DEL(m_OnHit);			

				for (LynxEngine::CMap<LynxEngine::CAnsiString, LuaFunction<int>*>::CIterator Func = m_UserFuncMap.begin(); Func != m_UserFuncMap.end(); Func++)
				{
					if ((*Func).second)
						LYNXGLOBAL_DEL((*Func).second);
				}
				m_UserFuncMap.clear();
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScript::vCompile(const LYNXCHAR *filename, LynxEngine::CStream& stream)
			{
				#if (__LUA_DEBUG__ && defined(__WIN32__))				
					LynxEngine::CPlatformFileStream FileStream;
					LynxEngine::CStream* lpStream;
					
					if (m_lpEngine->GetlpFileSystem())
						lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
					else
						lpStream = &FileStream;

					if (!lpStream->vOpen(filename, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
						return LYNX_FALSE;

					lpStream->vClose();

					if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoFile(LynxEngine::CAnsiString(filename).c_str()) != 0)
					{
						return LYNX_FALSE;
					}
				#else
					#if 0 // using CRC code to check LUA files
						char* String = NULL;	
						LynxEngine::CPlatformFileStream FileStream;
						LynxEngine::CStream* lpStream;
						
						if (m_lpEngine->GetlpFileSystem())
							lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
						else
							lpStream = &FileStream;

						if (!lpStream->vOpen(filename, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
							return LYNX_FALSE;

						LYNXLONG Len = lpStream->vSize();
						String = LYNXGLOBAL_NEW char[Len+2];
						lpStream->vRead(String, sizeof(char), Len);
						String[Len] = NULL;
						lpStream->vClose();

						#if (!defined (_UNICODE) && !defined (UNICODE))
							for (int i=0; i<Len; i++)
							{
								// remove L befor "" to convert UNICODE string to ANSI string
								if (String[i] == 'L' && String[i+1] == '"')
								{
									String[i] = ' ';
								}
							}
						#endif					

						#if (defined __WIN32__ && !SHIPPING)
							LYNXCRC ScriptCRCCode = lynxComputeCRCSlow((unsigned char*)String, Len);
							LYNXCHAR Name[256], Ext[16], CRCFileName[256], CRCString[32];
							lynxSeparateFileExtByPath(Name, Ext, filename);
							lynxSprintf(CRCFileName, _T("%s.chk"), Name);		
							lynxSprintf(CRCString, _T("%08X"), ScriptCRCCode);
							LynxEngine::CPlatformFileStream File; 
							File.vOpen(CRCFileName, LynxEngine::CStream::Text|LynxEngine::CStream::Write);
							File.vWrite(LynxEngine::CString(CRCString));
							File.vClose();
						#endif

						#if (SHIPPING)
							LYNXCRC ScriptCRCCode = lynxComputeCRCSlow((unsigned char*)String, Len);
							LYNXCHAR Name[256], Ext[16], CRCFileName[256], CRCString[32];
							LynxEngine::CString OriginalCRCString;
							lynxSeparateFileExtByPath(Name, Ext, filename);
							lynxSprintf(CRCFileName, _T("%s.chk"), Name);		
							lynxSprintf(CRCString, _T("%08X"), ScriptCRCCode);
							LynxEngine::CPlatformFileStream File; 
							File.vOpen(CRCFileName, LynxEngine::CStream::Text|LynxEngine::CStream::Read);
							File.vRead(OriginalCRCString);
							File.vClose();
							LYNXCRC OriginalCRC = (LYNXCRC)lynxXtoi(OriginalCRCString.c_str());
							if (OriginalCRC != ScriptCRCCode)
								return LYNX_FALSE;		
						#endif

						if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoString(String) != 0)
						{
							LYNXGLOBAL_DEL_ARRAY(String);
							return LYNX_FALSE;
						}

						LYNXGLOBAL_DEL_ARRAY(String);
					#else  //RC2 Encrypt LUA file
						//const char RC2Key[] = "spaceqube.lynxengine.com";

						#if 0//SHIPPING							
                            LYNXCHAR Name[256], Ext[16];
                            LynxEngine::CString LUCFileName;
                
							lynxSeparateFileExtByPath(Name, Ext, filename);
                            LUCFileName = LynxEngine::CString(Name) + LynxEngine::CString(_T(".luc"));
							LynxEngine::CPlatformFileStream File;
							if (File.vOpen(LUCFileName, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
							{		
								int Len = File.vSize();
								char* CodeData = LYNXGLOBAL_NEW char[Len+1];
								File.vRead(CodeData, 1, Len);
								File.vClose();
								CodeData[Len] = NULL;
								char Data[1024*100];			
								lynxRC2Decrypt(Data, m_lpScriptSystem->GetEncryptingKey().c_str(), CodeData, Len);            		
								LYNXGLOBAL_DEL_ARRAY(CodeData);

                                #if (!defined (_UNICODE) && !defined (UNICODE))
                                    for (int i=0; i<strlen(Data); i++)
                                    {
                                        // remove L befor "" to convert UNICODE string to ANSI string
                                        if (Data[i] == 'L' && Data[i+1] == '"')
                                        {
                                            Data[i] = ' ';
                                        }
                                    }
                                #endif
                                
								if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoString(Data) != 0)
								{
                                    LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Error] Failed to execute script <"))+LynxEngine::CString(LUCFileName)+LynxEngine::CString(_T(">\r\n")));
									return LYNX_FALSE;
								}								
							}
							else
							{
                                LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Error] Unablle to find <"))+LynxEngine::CString(LUCFileName)+LynxEngine::CString(_T(">\r\n")));
                                
                                char* String = NULL;
                                LynxEngine::CPlatformFileStream FileStream;
                                LynxEngine::CStream* lpStream;
                                
                                if (m_lpEngine->GetlpFileSystem())
                                    lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
                                else
                                    lpStream = &FileStream;
                                
                                if (!lpStream->vOpen(filename, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
                                {
                                    LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Error] Failed to open script <"))+LynxEngine::CString(filename)+LynxEngine::CString(_T(">\r\n")));
									
                                    return LYNX_FALSE;
                                }
                                
                                LYNXLONG Len = lpStream->vSize();
                                String = LYNXGLOBAL_NEW char[Len+2];
                                lpStream->vRead(String, sizeof(char), Len);
                                String[Len] = NULL;
                                lpStream->vClose();
                            
                                #if (!defined (_UNICODE) && !defined (UNICODE))
                                    for (int i=0; i<Len; i++)
                                    {
                                        // remove L befor "" to convert UNICODE string to ANSI string
                                        if (String[i] == 'L' && String[i+1] == '"')
                                        {
                                            String[i] = ' ';
                                        }
                                    }
                                 #endif

                                if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoString(String) != 0)
                                {
                                    LYNXGLOBAL_DEL_ARRAY(String);
                                    LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Error] Failed to execute script <"))+LynxEngine::CString(filename)+LynxEngine::CString(_T(">\r\n")));
                                    return LYNX_FALSE;
                                }
                                
                                LYNXGLOBAL_DEL_ARRAY(String);
							}
						#else
							char* String = NULL;
                            LynxEngine::CPlatformFileStream FileStream;
                            LynxEngine::CStream* lpStream;
                            
                            if (m_lpEngine->GetlpFileSystem())
                                lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
                            else
                                lpStream = &FileStream;
                            
                            if (!lpStream->vOpen(filename, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
                                return LYNX_FALSE;
                            
                            LYNXLONG Len = lpStream->vSize();
                            String = LYNXGLOBAL_NEW char[Len+2];
                            lpStream->vRead(String, sizeof(char), Len);
                            String[Len] = NULL;
                            lpStream->vClose();
                            
                            #if (!defined (_UNICODE) && !defined (UNICODE))
                                for (int i=0; i<Len; i++)
                                {
                                    // remove L befor "" to convert UNICODE string to ANSI string
                                    if (String[i] == 'L' && String[i+1] == '"')
                                    {
                                        String[i] = ' ';
                                    }
                                }
                            #endif
                            
                            if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoString(String) != 0)
                            {
                                LYNXGLOBAL_DEL_ARRAY(String);
                                return LYNX_FALSE;
                            }                                                       

							// Write encrypted Lua files
							#if (defined __WIN32__) 
								LYNXCHAR Name[256], Ext[16], LUCFileName[256];
								lynxSeparateFileExtByPath(Name, Ext, filename);							
								char Code[1024*100];
								int KeyCodeLength = lynxRC2Encrypt(Code, m_lpScriptSystem->GetEncryptingKey().c_str(), String);
								lynxSprintf(LUCFileName, _T("%s.luc"), Name);		
								LynxEngine::CPlatformFileStream File;
								File.vOpen(LUCFileName, LynxEngine::CStream::Binary|LynxEngine::CStream::Write);
								File.vWrite(Code, 1, KeyCodeLength);
								File.vClose();								
							#endif

							LYNXGLOBAL_DEL_ARRAY(String);
						#endif
					#endif
				#endif
				
				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CScript::vRegisterClass(const LynxEngine::CAnsiString& name)
			{
				//char Name[256];
				//strcpy(Name, name.c_str());
				m_ClassObjectTable = ((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->GetGlobals().CreateTable(name.c_str());
				//((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->GetGlobals()[&Name[1]].CreateTable(m_ClassObjectTable);
				m_ClassMetaTable = m_ClassObjectTable.CreateTable("MetaTable");
				LPCD::MetaTable_IntegratePropertySupport(m_ClassMetaTable);		

				//((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DumpObject("@", m_ClassMetaTable, m_ClassObjectTable, LuaPlus::LuaState::DUMP_WRITEALL|LuaPlus::LuaState::DUMP_ALPHABETICAL);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vSetlpSuperClass(LynxEngine::ScriptSystem::CScriptPtr& base)
			{
				LynxEngine::ScriptSystem::CScript::vSetlpSuperClass(base);
				//m_ClassMetaTable["__index"] = ((CScript*)&base)->m_ClassMetaTable;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vSetupHandler()
			{
				LynxEngine::CAnsiString Name(m_Name);				
				
				LuaObject Obj = ((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->GetGlobal(Name.c_str());
				if (!Obj.IsNil())
				{
					LuaPlus::LuaObject LuaObj;

                    LuaObj = Obj.GetByName("OnConstruct");
					if (LuaObj.IsFunction())
					{
						if (m_OnConstruct)
						{
							LYNXGLOBAL_DEL(m_OnConstruct);
							m_OnConstruct = NULL;
						}
						m_OnConstruct = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);
					}
					LuaObj = Obj.GetByName("OnCreate");
					if (LuaObj.IsFunction())
					{
						if (m_OnCreate)
						{
							LYNXGLOBAL_DEL(m_OnCreate);
							m_OnCreate = NULL;
						}
						m_OnCreate = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);	
					}
					LuaObj = Obj.GetByName("OnInit");
					if (LuaObj.IsFunction())
					{
						if (m_OnInit)
						{
							LYNXGLOBAL_DEL(m_OnInit);
							m_OnInit = NULL;
						}
						m_OnInit = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);	
					}		
					LuaObj = Obj.GetByName("OnLoop");
					if (LuaObj.IsFunction())
					{
						if (m_OnLoop)
						{
							LYNXGLOBAL_DEL(m_OnLoop);
							m_OnLoop = NULL;
						}
						m_OnLoop = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);	
					}
					LuaObj = Obj.GetByName("OnRender");
					if (LuaObj.IsFunction())
					{
						if (m_OnRender)
						{
							LYNXGLOBAL_DEL(m_OnRender);
							m_OnRender = NULL;
						}
						m_OnRender = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);	
					}
					LuaObj = Obj.GetByName("OnQuit");
					if (LuaObj.IsFunction())
					{
						if (m_OnQuit)
						{
							LYNXGLOBAL_DEL(m_OnQuit);
							m_OnQuit = NULL;
						}
						m_OnQuit = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);
					}
					LuaObj = Obj.GetByName("OnDestroy");
					if (LuaObj.IsFunction())
					{
						if (m_OnDestroy)
						{
							LYNXGLOBAL_DEL(m_OnDestroy);
							m_OnDestroy = NULL;
						}
						m_OnDestroy = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);
					}
					LuaObj = Obj.GetByName("OnHit");
					if (LuaObj.IsFunction())
					{
						if (m_OnHit)
						{
							LYNXGLOBAL_DEL(m_OnHit);
							m_OnHit = NULL;
						}
						m_OnHit = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);
					}
				}			
				//((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DumpObject("@", m_ClassMetaTable, m_ClassObjectTable, LuaPlus::LuaState::DUMP_WRITEALL|LuaPlus::LuaState::DUMP_ALPHABETICAL);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CScript::vAddUserHandler(const LynxEngine::CAnsiString& name)
			{
				LynxEngine::CAnsiString Name(m_Name);				
				
				LuaObject Obj = ((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->GetGlobal(Name.c_str());
				if (!Obj.IsNil())
				{
					LuaPlus::LuaObject LuaObj;

                    LuaObj = Obj.GetByName(name.c_str());
					if (LuaObj.IsFunction())
					{
						if (m_UserFuncMap[name])
						{
							LYNXGLOBAL_DEL(m_UserFuncMap[name]);
							m_UserFuncMap[name] = NULL;
						}
						m_UserFuncMap[name] = LYNXGLOBAL_NEW LuaFunction<int>(LuaObj);
					}
				}
			}	
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CScript::vOnConstruct()
			{
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnConstruct)
						int x = (*m_OnConstruct)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnCreate()
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnCreate)
						int x = (*m_OnCreate)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnInit()
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnInit)
						int x = (*m_OnInit)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnLoop(float step)
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnLoop)
						int x = (*m_OnLoop)(step);
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnRender()
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnRender)
						int x = (*m_OnRender)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnQuit()
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnQuit)
						int x = (*m_OnQuit)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnDestroy()
			{	
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnDestroy)
						int x = (*m_OnDestroy)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void CScript::vOnHit()
			{
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_OnHit)
						int x = (*m_OnHit)();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScript::vHasUserFunc(const LynxEngine::CAnsiString& name)
			{
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_UserFuncMap[name])
						return LYNX_TRUE;
				}
				return LYNX_FALSE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CScript::vOnUserFunc(const LynxEngine::CAnsiString& name)
			{
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
				{
					if (m_UserFuncMap[name])
						int x = (*m_UserFuncMap[name])();
				}
			}
		}
	}

#endif