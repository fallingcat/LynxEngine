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

#ifndef __LYNXFILESYSTEM_H__
#define __LYNXFILESYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace FileSystem 
	{
		class LYNXENGCLASS CFileSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CFileSystem);
		public:
			CMap<CString, int>					m_UsedFileMap;
			CMap<CString, LYNXBOOL>				m_FileList;
		protected:
			CPackage*							m_DefaultPackage;
			CPackage*							m_lpCurrentPackage;
			static CArray<CString>				m_FileSearchingPathArray;
			CUpdate*							m_Update;
		public:
			CFileSystem(CEngine* const lpengine);
			~CFileSystem();

			LYNXBOOL							vCreate(void);

			LYNXBOOL							OpenPackage(const CString& name);
			void								ClosePackage();
			CPackage*							GetPackage() { return m_lpCurrentPackage; };

			LYNXFORCEINLINE int					GetNumFileSearchingPath() const{ return m_FileSearchingPathArray.size(); };
			virtual CString						vProcessPath(const CString& path);
			virtual CString						GetFullSearchingPath(int index, const CString& path);

			LYNXFORCEINLINE CUpdate*			GetUpdate() const{ return m_Update; };
			void								BuildFileList(const CString& path);
            LYNXBOOL                            FindFile(const CString& path);
		};
	}
}

#endif