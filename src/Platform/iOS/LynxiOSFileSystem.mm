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
#include <Platform/iOS/LynxiOSFileSystem.h>

namespace LynxEngine 
{	
	namespace FileSystem 
	{
		namespace iOS
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
                LYNXCHAR Buffer[2048];
                
                LynxEngine::FileSystem::CFileSystem::vCreate();
                
				m_FileSearchingPathArray.clear();
				
				NSFileManager* FileManager = [NSFileManager defaultManager];
				
				// Find the application support directory in the home directory.
				NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
				if ([AppSupportDir count] > 0)
				{
					// Append the bundle ID to the URL for the
					// Application Support directory
					NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
                    NSURL* AppLibFolderURL = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
                    NSString* AppLibFolderPath = [AppLibFolderURL path];
                    lynxNSString2cString(Buffer, AppLibFolderPath);
                    m_FileSearchingPathArray.push_back(CString(Buffer)+CString(_T("/")));
                    
                    BOOL IsDir = YES;
                    if ([FileManager fileExistsAtPath:AppLibFolderPath isDirectory:&IsDir] != YES)
                        [FileManager createDirectoryAtURL:AppLibFolderURL withIntermediateDirectories:YES attributes:nil error:NULL];
				}
	            
                NSURL* AppFolderURL = [[NSBundle mainBundle] bundleURL];
				NSString* AppFolderPath = [AppFolderURL path];
                lynxNSString2cString(Buffer, AppFolderPath);
                m_FileSearchingPathArray.push_back(CString(Buffer)+CString(_T("/")));
	            				
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
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CFileSystem::GetAppBundlePath(const CString& path)
            {
                return GetFullSearchingPath(1, path);
            }
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CFileSystem::GetAppSupportPath(const CString& path)
            {
                return GetFullSearchingPath(0, path);
            }
        }
	}
}