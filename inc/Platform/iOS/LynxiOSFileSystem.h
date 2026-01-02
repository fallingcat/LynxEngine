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

#ifndef __LYNXIOSFILESYSTEM_H__
#define __LYNXIOSFILESYSTEM_H__

#include <FileSystem/LynxFileSystem.h>

namespace LynxEngine 
{
	namespace FileSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CFileSystem : public LynxEngine::FileSystem::CFileSystem
			{
				LYNX_DECLARE_CLASS(CFileSystem);
			private:		
			protected:
			public:			
				CFileSystem(CEngine* const lpengine);
				~CFileSystem();			

				LYNXBOOL							vCreate(void);
				virtual CString						vProcessPath(const CString& path);
                
                LYNXFORCEINLINE CString             GetAppBundlePath() const {return m_FileSearchingPathArray[1]; };
                CString                             GetAppBundlePath(const CString& path);
                LYNXFORCEINLINE CString             GetAppSupportPath() const {return m_FileSearchingPathArray[0]; };
                CString                             GetAppSupportPath(const CString& path);
            };
		}
	}
}

#endif