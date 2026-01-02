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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Platform/iOS/LynxiOSFileStream.h>

#import <Foundation/NSString.h>
#import <Foundation/NSFileHandle.h>

namespace LynxEngine 
{	
	namespace iOS
	{
        #ifdef _UNICODE
            #define STRING_ENCODING         NSUTF32LittleEndianStringEncoding
        #else
            #define STRING_ENCODING         NSUTF8StringEncoding//NSASCIIStringEncoding
        #endif
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileStream::CFileStream(void)	
		{
            m_lpFile = NULL;
			m_NSFile = nil;
            m_CurrentReadLine = 0;
            m_Input = nil;
            m_Lines = nil;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileStream::~CFileStream(void)
		{
            if (m_Input)
            {
                //[m_Input release];
                m_Input = nil;
                m_Lines = nil;
            }
			vClose();
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		        
        LYNXBOOL CFileStream::vOpen(const CString& name, int flag) 
        {
            CString ModName = name;
            ModName.ReplaceBackslashWithSlash();
            
            CString FullPath;
            
            m_bValid = LYNX_FALSE;
            m_lpFile = NULL;
            m_NSFile = nil;
            
            CString Filename;
            if (ModName.c_str()[0] == _T('.') && ModName.c_str()[1] == _T('.') && (ModName.c_str()[2] == _T('/') || ModName.c_str()[2] == _T('\\')))
                Filename = &(ModName.c_str()[3]);
            else if (ModName.c_str()[0] == _T('.') && (ModName.c_str()[1] == _T('/') || ModName.c_str()[1] == _T('\\')))
                Filename = &(ModName.c_str()[2]);
            else
                Filename = ModName;

            if (flag & ANSI)
            {
                m_Encoding = NSASCIIStringEncoding;
            }
            else if (flag & UTF8)
            {
                m_Encoding = NSUTF8StringEncoding;
            }
            else if (flag & UTF16)
            {
                m_Encoding = NSUTF16LittleEndianStringEncoding;
            }
            else
            {
                m_Encoding = NSASCIIStringEncoding;
            }

            
            LYNXCHAR Flag[32] = _T("");
            
            if (flag & Write)
                lynxStrCat(Flag, _T("w"));
            else if (flag & Read)
                lynxStrCat(Flag, _T("r"));
            
            if (flag & Binary)
                lynxStrCat(Flag, _T("b"));
            else if (flag & Text)
                lynxStrCat(Flag, _T("t"));
            else
                lynxStrCat(Flag, _T("b"));
            
            if (glpKernel && glpKernel->GetlpEngine() && glpKernel->GetlpEngine()->GetlpFileSystem())
            {
                FileSystem::iOS::CFileSystem* FileSystem = static_cast<FileSystem::iOS::CFileSystem*>(glpKernel->GetlpEngine()->GetlpFileSystem());
            
                if (flag & Write)
                {
                    NSFileManager* FileManager = [NSFileManager defaultManager];
                    
                    FullPath = FileSystem->GetAppSupportPath(Filename);
                    NSString* FullPathStr = nil;
                    lynxcString2NSString(&FullPathStr, FullPath.c_str());
                    
                    NSString* Path = [FullPathStr stringByDeletingLastPathComponent];
                    BOOL IsDir = YES;
                    if ([FileManager fileExistsAtPath:Path isDirectory:&IsDir] != YES)
                        [FileManager createDirectoryAtPath:Path withIntermediateDirectories:YES attributes:nil error:NULL];
                    
                    if ([FileManager fileExistsAtPath:FullPathStr] != YES)
                        [FileManager createFileAtPath:FullPathStr contents:nil attributes:nil];
                    
                    //m_NSFile = [NSFileHandle fileHandleForWritingAtPath:FullPathStr];
                    m_lpFile = _tfopen(FullPath.c_str(), Flag);
                }
                else if (flag & Read)
                {
                    for (int i=0; i<FileSystem->GetNumFileSearchingPath(); i++)
                    {
                        NSString* Name = nil;
                        lynxcString2NSString(&Name, ModName.c_str());
                        FullPath = FileSystem->GetFullSearchingPath(i, Filename);
                        NSString* FullPathStr = nil;
                        lynxcString2NSString(&FullPathStr, FullPath.c_str());
                        
                        m_lpFile = _tfopen(FullPath.c_str(), Flag);
                        if (m_lpFile != NULL)
                        {
                            break;
                        }
                        
                        //m_NSFile = [NSFileHandle fileHandleForReadingAtPath:FullPathStr];
                        //if (m_NSFile != NULL)
                        //{
                          //  break;
                        //}
                    }
                }
                else if (flag & Append)
                {
                    NSFileManager* FileManager = [NSFileManager defaultManager];
                    
                    FullPath = FileSystem->GetAppSupportPath(Filename);
                    NSString* FullPathStr = nil;
                    lynxcString2NSString(&FullPathStr, FullPath.c_str());
                    
                    NSString* Path = [FullPathStr stringByDeletingLastPathComponent];
                    BOOL IsDir = YES;
                    if ([FileManager fileExistsAtPath:Path isDirectory:&IsDir] != YES)
                        [FileManager createDirectoryAtPath:Path withIntermediateDirectories:YES attributes:nil error:NULL];
                    
                    if ([FileManager fileExistsAtPath:FullPathStr] != YES)
                        [FileManager createFileAtPath:FullPathStr contents:nil attributes:nil];
                    
                    //m_lpFile = _tfopen(FullPath.c_str(), "wa");
                    
                    m_NSFile = [NSFileHandle fileHandleForWritingAtPath:FullPathStr];
                    if (m_NSFile)
                    {
                        [m_NSFile truncateFileAtOffset:[m_NSFile seekToEndOfFile]];
                    }

                }
                else
                {
                    for (int i=0; i<FileSystem->GetNumFileSearchingPath(); i++)
                    {
                        FullPath = FileSystem->GetFullSearchingPath(i, Filename);
                        m_lpFile = _tfopen(FullPath.c_str(), "r");
                        if (m_lpFile != NULL)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                NSString* Name = nil;
                lynxcString2NSString(&Name, Filename.c_str());
                NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
                NSFileManager* FileManager = [NSFileManager defaultManager];
                NSURL* SearchURL = [[NSBundle mainBundle] bundleURL];
                SearchURL = [SearchURL URLByAppendingPathComponent:Name];
                
                NSURL* SearchURL2 = nil;
                // Find the application support directory in the home directory.
                NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
                if ([AppSupportDir count] > 0)
                {
                    // Append the bundle ID to the URL for the
                    // Application Support directory
                    SearchURL2 = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
                    SearchURL2 = [SearchURL2 URLByAppendingPathComponent:Name];
                }
                
                if (flag & Write)
                {
                    NSURL* PathURL = [SearchURL2 URLByDeletingLastPathComponent];
                    BOOL IsDir = YES;
                    if ([FileManager fileExistsAtPath:[PathURL path] isDirectory:&IsDir] != YES)
                        [FileManager createDirectoryAtURL:PathURL withIntermediateDirectories:YES attributes:nil error:NULL];
                    
                    [Name writeToURL:SearchURL2 atomically:YES];
                    m_NSFile = [NSFileHandle fileHandleForWritingToURL:SearchURL2 error:NULL];
                }
                else if (flag & Read)
                {
                    m_NSFile = [NSFileHandle fileHandleForReadingFromURL:SearchURL error:NULL];
                    if (m_NSFile == nil)
                    {
                        m_NSFile = [NSFileHandle fileHandleForReadingFromURL:SearchURL2 error:NULL];
                    }
                }
				else if (flag & Append)
                {
                   NSURL* PathURL = [SearchURL2 URLByDeletingLastPathComponent];
                    BOOL IsDir = YES;
                    if ([FileManager fileExistsAtPath:[PathURL path] isDirectory:&IsDir] != YES)
                        [FileManager createDirectoryAtURL:PathURL withIntermediateDirectories:YES attributes:nil error:NULL];
                    
                    [Name writeToURL:SearchURL2 atomically:YES];
                    m_NSFile = [NSFileHandle fileHandleForWritingToURL:SearchURL2 error:NULL];
                    if (m_NSFile)
                    {
                        [m_NSFile truncateFileAtOffset:[m_NSFile seekToEndOfFile]];
                    }
                }
                else
                {
                    m_NSFile = [NSFileHandle fileHandleForReadingFromURL:SearchURL error:NULL];
                    if (m_NSFile == nil)
                    {
                        m_NSFile = [NSFileHandle fileHandleForReadingFromURL:SearchURL2 error:NULL];
                    }
                }
            }
            
            if (m_NSFile == nil && m_lpFile == NULL)
            {
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::"))+CString(_T("[Error] Failed to open "))+FullPath+CString(_T("\r\n")));
                m_bValid = LYNX_FALSE;
                return LYNX_FALSE;
            }
            else
            {
                LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::"))+CString(_T("Opened "))+FullPath+CString(_T("\r\n")));
                m_CurrentReadLine = 0;
                m_bValid = LYNX_TRUE;
            
                return LYNX_TRUE;
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFileStream::vClose() 
		{
            if (m_lpFile)
                fclose(m_lpFile);
            else
            {
                if (m_bValid)
                {
                    [m_NSFile closeFile];
                    m_bValid = LYNX_FALSE;
                }
                if (m_Input)
                {
                    //[m_Input release];
                    m_Input = nil;
                    m_Lines = nil;
                }
                m_CurrentReadLine = 0;
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vRead(void *lpdst, int size, int len) 
		{
            if (m_lpFile)
                return fread(lpdst, size, len, m_lpFile);
            else
            {
                NSData* Input = [m_NSFile readDataOfLength:(size*len)];
                if (Input == nil)
                    return 0;
                if ([Input bytes] == nil)
                    return 0;
                
                lynxMemCpy(lpdst, [Input bytes], size*len);
                //lynxByteOrder(lpdst, size*len);
                
                return (size*len);
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vRead(CString& str) 
		{
            if (m_lpFile)
            {
                int Len = 0;
                
                if (m_lpFile!=NULL)
                {
                    LYNXCHAR Str[1024];
                    
                    int Last;
                    str = _T("");
                    do {
                        if (!_fgetts(Str, 1024, m_lpFile))
                            return 0;
                        str += CString(Str);
                        Last = lynxStrLen(Str) - 1;
                    } while (!feof(m_lpFile) && Str[Last] != _T('\n'));
                    
                    Len = str.length();
                }
                return Len;
            }
            else
            {
                if (!m_Input)
                {
                    m_Input = [[NSString alloc] initWithData: [m_NSFile availableData] encoding:m_Encoding];
                    if (m_Input)
                    {
                        m_Lines = [m_Input componentsSeparatedByString: @"\n"];
                    }
                }
                
                //LYNXCHAR* Buffer;//[4096*100];
                int ProcessedLen = 0;
                if (m_CurrentReadLine < [m_Lines count])
                {
                    ProcessedLen = [[m_Lines objectAtIndex:m_CurrentReadLine] length] + 1;
                    str = [[m_Lines objectAtIndex:m_CurrentReadLine] cStringUsingEncoding:STRING_ENCODING];
                    //Buffer = LYNXNEW LYNXCHAR[ProcessedLen+10];
                    //lynxNSString2cString(Buffer, [m_Lines objectAtIndex:m_CurrentReadLine]);
                    
                    //str = Buffer;
                    //LYNXDEL_ARRAY(Buffer);
                    m_CurrentReadLine++;
                }
                
                return ProcessedLen;
            }
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vWrite(const void *lpsrc, int size, int len)
		{
            if (m_lpFile)
                return (int)fwrite(lpsrc, size, len, m_lpFile);
            else
            {
                NSData* Data = [[NSData alloc] initWithBytes:lpsrc length:(size*len)];
                [m_NSFile writeData:Data];
                
                return (size*len);
            }
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        int CFileStream::vWrite(const CString& str)
        {
            if (m_lpFile)
                return (int)_fputts(str.c_str(), m_lpFile);
            else
            {
                NSString* Data = nil;
                lynxcString2NSString(&Data, str.c_str());
                [m_NSFile writeData:[Data dataUsingEncoding:m_Encoding]];
                
                return 0;
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vSeek(LYNXDWORD len, int flag)
		{
            if (m_lpFile)
                return fseek(m_lpFile, len, flag);
            else
            {
                unsigned long long Offset = 0;
                
                if (flag & SeekSet)
                    Offset = 0;
                else if (flag & SeekCur)
                    Offset = [m_NSFile offsetInFile];
                else if (flag & SeekEnd)
                {
                    [m_NSFile seekToEndOfFile];
                    Offset = [m_NSFile offsetInFile];
                }	
                Offset += len;
                
                [m_NSFile seekToFileOffset:Offset];
                
                return 0;
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CFileStream::vTell()
		{
            if (m_lpFile)
                return ftell(m_lpFile);
            else
            {
                LYNXDWORD Tell = [m_NSFile offsetInFile];
			
                return Tell;
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CFileStream::vSize()
		{
            if (m_lpFile)
            {
                LYNXDWORD	Len, Offset;
                Offset = ftell(m_lpFile);
                vSeek(0, SeekEnd);
                Len = ftell(m_lpFile);
                vSeek(Offset, SeekSet);
                return Len;
            }
            else
            {
                unsigned long long CurrOffset = [m_NSFile offsetInFile];
                [m_NSFile seekToEndOfFile];
                LYNXDWORD Tell = [m_NSFile offsetInFile];
                [m_NSFile seekToFileOffset:CurrOffset];
                
                return Tell;
            }
		}
    }
}