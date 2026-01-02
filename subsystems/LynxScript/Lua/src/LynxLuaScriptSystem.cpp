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

#ifdef __LUA__

	#include <LynxCore.h>
	#include <LynxLuaScriptSystem.h>
	#include <LynxLuaVMachine.h>

	namespace LynxScript 
	{
		namespace Lua 
		{	
			//CResource* CScriptSystem::CreateScript() { return LYNXNEW GraphicsSystem::CVertexArray; }
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CScriptSystem::CScriptSystem(LynxEngine::CEngine* const lpengine)		
			: LynxEngine::ScriptSystem::CScriptSystem(lpengine)
			{			
				m_ScriptPerInstance = LYNX_FALSE;
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

				int Size = 0;
				#ifdef __TOOL__
					Size = 4096;
				#else
					Size = 1024;
				#endif
				if (!m_lpVMachine->vCreate(Size))
					return LYNX_FALSE;

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScriptSystem::vOnInit()		
			{
				//m_ResourceFactory.Register(CResource::SCRIPT,				CreateScript);
				return LYNX_TRUE;
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CScriptSystem::EncryptScript(const LYNXCHAR *filename, const char* key)
			{
				char* String = NULL;
                LynxEngine::CPlatformFileStream FileStream;
                
				LynxEngine::CStream* lpStream = &FileStream;
                
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
                
				// Write encrypted Lua files
				#if (defined __WIN32__) 
					LYNXCHAR Name[256], Ext[16], LUCFileName[256];
					lynxSeparateFileExtByPath(Name, Ext, filename);							
					char Code[1024*100];					
					const char* Key = NULL;

					if (key)
						Key = key;
					else
						Key = m_EncryptingKey.c_str();

					int KeyCodeLength = lynxRC2Encrypt(Code, Key, String);
					lynxSprintf(LUCFileName, _T("%s.luc"), Name);		
					LynxEngine::CPlatformFileStream File;
					File.vOpen(LUCFileName, LynxEngine::CStream::Binary|LynxEngine::CStream::Write);
					File.vWrite(Code, 1, KeyCodeLength);
					File.vClose();								
				#endif

				LYNXGLOBAL_DEL_ARRAY(String);						
				
				return LYNX_TRUE;
			}
		}
	}

#endif