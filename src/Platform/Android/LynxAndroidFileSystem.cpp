//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Platform/Android/LynxAndroidFileSystem.h>

namespace LynxEngine 
{	
	namespace FileSystem 
	{
		namespace Android
		{
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CFileSystem::CFileSystem(CEngine* const lpengine)
			: LynxEngine::FileSystem::CFileSystem(lpengine)
			{				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CFileSystem::~CFileSystem(void)
			{				
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CFileSystem::vCreate(void)
			{
                LynxEngine::FileSystem::CFileSystem::vCreate();
                
				m_FileSearchingPathArray.clear();
				
				CString InternalPath = CString(static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->internalDataPath);

                m_FileSearchingPathArray.push_back(InternalPath+CString(_T("/")));
	            				
				return LYNX_TRUE;
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CFileSystem::vProcessPath(const CString& path)
			{
				CString ProcessedPath;
				
				if (path.c_str()[0] == _T('.') && path.c_str()[1] == _T('.') && (path.c_str()[2] == _T('/') || path.c_str()[2] == _T('\\')))
					ProcessedPath = &(path.c_str()[3]);
				else if (path.c_str()[0] == _T('.') && (path.c_str()[1] == _T('/') || path.c_str()[1] == _T('\\')))
					ProcessedPath = &(path.c_str()[2]);
				else
					ProcessedPath = path;
                
				return (ProcessedPath);
			}
		}
	}
}