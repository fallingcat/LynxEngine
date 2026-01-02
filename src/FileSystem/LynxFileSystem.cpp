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
#include <FileSystem/LynxFileSystem.h>

namespace LynxEngine 
{	
	namespace FileSystem 
	{
		CArray<CString>				CFileSystem::m_FileSearchingPathArray;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileSystem::CFileSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{
			m_lpCurrentPackage = NULL;
			m_Update = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileSystem::~CFileSystem(void)
		{
			m_FileSearchingPathArray.clear();
			m_UsedFileMap.clear();
			m_FileList.clear();
		
			if (m_lpCurrentPackage)
			{
				ClosePackage();
			}

			if (m_DefaultPackage)
			{
				m_DefaultPackage->Close();
				LYNXDEL m_DefaultPackage;
				m_DefaultPackage = NULL;
			}

			if (m_Update)
			{
				LYNXDEL m_Update;
				m_Update = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CFileSystem::vCreate(void)
		{
			m_Update = LYNXNEW CUpdate(this);

			m_DefaultPackage = LYNXNEW CPackage(this);
			m_lpCurrentPackage = m_DefaultPackage;

			m_FileSearchingPathArray.clear();
			m_FileSearchingPathArray.push_back(_T(""));
			m_FileSearchingPathArray.push_back(_T(""));

			#ifdef __DESKTOP__
				/*
					BuildFileList(_T("../"));
					BuildFileList(_T("../../engine/"));
					CPlatformFileStream File;
					File.vOpen(_T("../file_list.txt"), CStream::Write | CStream::Text);
					for (CMap<CString, LYNXBOOL>::CIterator Pos = m_FileList.begin(); Pos != m_FileList.end(); Pos++)
					{
						File.vWrite(((*Pos).first + CString("\n")));
					}
					File.vClose();
				*/
			#endif

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CFileSystem::OpenPackage(const CString& name)
		{
			//m_lpCurrentPackage = LYNXNEW CPackage(this);

			return m_lpCurrentPackage->Open(name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFileSystem::ClosePackage()
		{
			//m_lpCurrentPackage->Close();
			//LYNXDEL m_lpCurrentPackage;
			//m_lpCurrentPackage = NULL;
			m_lpCurrentPackage = m_DefaultPackage;
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:
        //-------------------------------------------------------------------------------------------------------
        CString CFileSystem::vProcessPath(const CString& path)
        {
            CString ProcessedPath;
            
            #ifdef __WINAPP__
                if (path.c_str()[0] == _T('.') && path.c_str()[1] == _T('.') && (path.c_str()[2] == _T('/') || path.c_str()[2] == _T('\\')))
                    ProcessedPath = &(path.c_str()[3]);
                else
                    ProcessedPath = path;
            #else
                ProcessedPath = path;
            #endif
           
            return ProcessedPath;
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:
        //-------------------------------------------------------------------------------------------------------
        CString CFileSystem::GetFullSearchingPath(int index, const CString& path)
        {
            CString ProcessedPath = vProcessPath(path);          			
            return (m_FileSearchingPathArray[index] + ProcessedPath);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CFileSystem::BuildFileList(const CString& src)
		{
			LYNXFINDDATA FindData, FindFiles;
			LYNXCHAR Filename[128], Path[256], Dest[256], Src[256], NewSrc[256], SearchPath[256], SubPath[256];
			CString SrcPath;			

			SrcPath = src;
			lynxStrCpy(SubPath, &(src.c_str()[3]));

			lynxInitFindData(&FindFiles);
			lynxSprintf(Path, _T("%s*.*"), SrcPath.c_str());
			lynxFindFiles(Path, &FindFiles);
			for (int j = 0; j<FindFiles.NumFounds; j++)
			{
				lynxSprintf(Src, _T("%s%s"), SrcPath.c_str(), FindFiles.lpFounds[j]);
				//if (Stream.vOpen(Src))
				{
					//Stream.vClose();
					lynxStrLower(Src);
					m_FileList[Src] = LYNX_TRUE;
				}
			}
			lynxFreeFounds(&FindFiles);

			lynxInitFindData(&FindData);
			lynxSprintf(SearchPath, _T("%s*"), SrcPath.c_str());
			lynxFindFolders(SearchPath, &FindData);
			for (int i = 0; i<FindData.NumFounds; i++)
			{
				if (lynxStrStr(FindData.lpFounds[i], "dev"))
					continue;

				lynxSprintf(Path, _T("%s%s/"), SrcPath.c_str(), FindData.lpFounds[i]);
				BuildFileList(CString(Path));
			}
			lynxFreeFounds(&FindData);			
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  :
        //-------------------------------------------------------------------------------------------------------
        LYNXBOOL CFileSystem::FindFile(const CString& path)
        {
            if (m_FileList.empty())
            {
                static CPlatformFileStream File;
                LYNXBOOL bRet = File.vOpen(path);
                if (bRet)
                {
                    File.vClose();
                    return LYNX_TRUE;
                }
                else
                {
					bRet = File.vOpen(path + CString(_T(".z")));
					if (bRet)
					{
						File.vClose();
						return LYNX_TRUE;
					}
					else
					{
						return LYNX_FALSE;
					}
                }
            }
            else        
            {
				LYNXBOOL bRet = m_FileList.is_exist(path);
				if (bRet)
				{
					return LYNX_TRUE;
				}
				else
				{
					bRet = m_FileList.is_exist(path + CString(_T(".z")));
					if (bRet)
					{
						return LYNX_TRUE;
					}
					else
					{
						return LYNX_FALSE;
					}
				}
            }
        }
	}
}