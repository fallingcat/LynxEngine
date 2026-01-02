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
#include <Platform/iOS/LynxiOSURLStream.h>
#include <CoreFoundation/CoreFoundation.h>
#import <Foundation/NSString.h>

#define TIMEOUT                     30000
#define CHUNK_SIZE                  (32 * 1024)

namespace LynxEngine 
{	
	namespace iOS
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CURLStream::CURLStream(void)	
		{
			m_StreamType = URL_STREAM;
			m_bValid = LYNX_FALSE;
            m_RStream = NULL;
            m_WStream = NULL;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CURLStream::~CURLStream(void)
		{
			vClose();
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
        LYNXBOOL CURLStream::IsExisting(const CString& name)
        {
            CFStringRef URLString = CFStringCreateWithCString(NULL, (const char*)name.c_str(), NSASCIIStringEncoding);
            CFURLRef URL = CFURLCreateWithString(NULL, URLString, NULL);
            
            // If the URL is using ftp
            if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("ftp"), 0) == kCFCompareEqualTo)
            {
                CFReadStreamRef RStream = CFReadStreamCreateWithFTPURL(NULL, URL);
                if (!RStream)
                {
                    CFRelease(URLString);
                    CFRelease(URL);
                    return LYNX_FALSE;
                }
                CFRelease(URLString);
                CFRelease(URL);
                
                LYNXBOOL bValid = CFReadStreamOpen(RStream);
                if (!bValid)
                    return LYNX_FALSE;
                
                UInt8 Buffer[48];
                if (CFReadStreamRead(RStream, Buffer, 32) == -1)
                    return LYNX_FALSE;
                
                CFReadStreamClose(RStream);
                CFRelease(RStream);
            
                return LYNX_TRUE;
            }
            // If the URL is using http
            else if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("http"), 0) == kCFCompareEqualTo || CFStringCompare(CFURLCopyScheme(URL), CFSTR("https"), 0) == kCFCompareEqualTo)
            {
                static CFStringRef RequestHeader = CFSTR("Connection");
                static CFStringRef RequestHeaderValue = CFSTR("close");
                CFHTTPMessageRef Request = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("GET"), URL, kCFHTTPVersion1_0);
                CFHTTPMessageSetHeaderFieldValue(Request, RequestHeader, RequestHeaderValue);
                CFReadStreamRef RStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, Request);
                CFRelease(Request);
                
                if (!RStream)
                {
                    CFRelease(URLString);
                    CFRelease(URL);
                    return LYNX_FALSE;
                }
                CFRelease(URLString);
                CFRelease(URL);
                
                LYNXBOOL bValid = CFReadStreamOpen(RStream);
                if (!bValid)
                    return LYNX_FALSE;
                
                UInt8 Buffer[48];
                if (CFReadStreamRead(RStream, Buffer, 32) == -1)
                    return LYNX_FALSE;
                
                CFReadStreamClose(RStream);
                CFRelease(RStream);
                
                return LYNX_TRUE;
            }
            else
            {
                return LYNX_FALSE;
            }
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------		        
        LYNXBOOL CURLStream::vOpen(const CString& name, int flag) 
        {
            //UInt8 Buffer[CHUNK_SIZE + 8];
            UInt8* Buffer = NULL;
            CFStreamStatus Status;
            
            m_bValid = LYNX_FALSE;
            
            CFStringRef URLString = CFStringCreateWithCString(NULL, (const char*)name.c_str(), NSASCIIStringEncoding);
            CFURLRef URL = CFURLCreateWithString(NULL, URLString, NULL);
            
            m_CurrentProgressSize = 0;
            
            if (flag & Write)
            {
                // If the URL is using ftp
                if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("ftp"), 0) == kCFCompareEqualTo)
                {
                    m_WStream = CFWriteStreamCreateWithFTPURL(NULL, URL);
                    if (!m_WStream)
                    {
                        m_bValid = LYNX_FALSE;
                        CFRelease(URLString);
                        CFRelease(URL);
                        return LYNX_FALSE;
                    }
                    
                    CFRelease(URLString);
                    CFRelease(URL);
                    
                    //Boolean bSetProperty = CFReadStreamSetProperty(m_RStream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelSSLv2);
                    m_bValid = CFWriteStreamOpen(m_WStream);
                    if (!m_bValid)
                    {
                        m_bValid = LYNX_FALSE;
                        CFRelease(URLString);
                        CFRelease(URL);
                        return LYNX_FALSE;
                    }
                    
                    LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                    do {
                        Status = CFWriteStreamGetStatus(m_WStream);
                        if ((lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                        {
                            CFWriteStreamClose(m_WStream);
                            CFRelease(m_WStream);
                            m_WStream = NULL;
                            m_bValid = LYNX_FALSE;
                            return LYNX_FALSE;
                        }
                    }while (Status != kCFStreamStatusOpen);            
                    
                    m_FileStream.vOpen(m_UploadFilename, CStream::Binary|CStream::Read);

                        int FileSize, Size, WSize, BufferSize, ReadSize;
                    
                        FileSize = m_FileStream.vSize();
                        BufferSize = (FileSize > CHUNK_SIZE) ? CHUNK_SIZE : FileSize;
                        Buffer = LYNXNEW UInt8[BufferSize];
                        do {
                            ReadSize = (FileSize > BufferSize) ? BufferSize : FileSize;
                            Size = m_FileStream.vRead(Buffer, sizeof(UInt8), ReadSize);
                            
                            T = lynxOSGetPerformanceCounter();
                            do {
                                Status = CFWriteStreamGetStatus(m_WStream);
                                if (Status == kCFStreamStatusError || (lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                                {
                                    LYNXDEL_ARRAY(Buffer);
                                    CFWriteStreamClose(m_WStream);
                                    CFRelease(m_WStream);
                                    m_WStream = NULL;
                                    m_bValid = LYNX_FALSE;
                                    return LYNX_FALSE;
                                }
                            } while (Status == kCFStreamStatusWriting);
                            
                            WSize = CFWriteStreamWrite(m_WStream, Buffer, Size);
                            if (WSize < 0)
                            {
                                LYNXDEL_ARRAY(Buffer);
                                m_FileStream.vClose();
                                CFWriteStreamClose(m_WStream);
                                CFRelease(m_WStream);
                                m_WStream = NULL;
                                m_bValid = LYNX_FALSE;
                                return LYNX_FALSE;
                            }
                            m_CurrentProgressSize += WSize;
                            FileSize -= WSize;
                        }while (FileSize > 0);
                    
                    m_FileStream.vClose();
                    
                    LYNXDEL_ARRAY(Buffer);
                    m_bValid = LYNX_TRUE;
                    
                    return LYNX_TRUE;
                }
                // If the URL is using http
                else if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("http"), 0) == kCFCompareEqualTo || CFStringCompare(CFURLCopyScheme(URL), CFSTR("https"), 0) == kCFCompareEqualTo)
                {
                    return LYNX_FALSE;
                }
                else
                {
                    return LYNX_FALSE;
                }
            }
            else
            {
                // If the URL is using ftp
                if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("ftp"), 0) == kCFCompareEqualTo)
                {
                    m_RStream = CFReadStreamCreateWithFTPURL(NULL, URL);
                    if (!m_RStream)
                    {
                        m_bValid = LYNX_FALSE;
                      
                        CFRelease(URLString);
                        CFRelease(URL);
                        
                        return LYNX_FALSE;
                    }
                }
                // If the URL is using http
                else if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("http"), 0) == kCFCompareEqualTo || CFStringCompare(CFURLCopyScheme(URL), CFSTR("https"), 0) == kCFCompareEqualTo)
                {
                    //static CFStringRef RequestHeader = CFSTR("Connection");
                    //static CFStringRef RequestHeaderValue = CFSTR("close");
                    CFHTTPMessageRef Request = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("GET"), URL, kCFHTTPVersion1_1);
                    //CFHTTPMessageSetHeaderFieldValue(Request, RequestHeader, RequestHeaderValue);
                    m_RStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, Request);
                    
                    if (!m_RStream)
                    {
                        m_bValid = LYNX_FALSE;
                        CFRelease(URLString);
                        CFRelease(URL);
                        return LYNX_FALSE;
                    }
                    CFRelease(Request);
                }
                else
                {
                    m_bValid = LYNX_FALSE;
                    CFRelease(URLString);
                    CFRelease(URL);
                    return LYNX_FALSE;
                }
                CFRelease(URLString);
                CFRelease(URL);
                
                m_bValid = CFReadStreamOpen(m_RStream);
                if (!m_bValid)
                    return LYNX_FALSE;
                
                 LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                 do {
                     if ((lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                     {
                         CFReadStreamClose(m_RStream);
                         CFRelease(m_RStream);
                         m_RStream = NULL;
                         m_bValid = LYNX_FALSE;
                         return LYNX_FALSE;
                     }
                 }while (CFReadStreamGetStatus(m_RStream) != kCFStreamStatusOpen);
                
                m_FileStream.vOpen(_T("../temp/temp_read_stream.bin"), CStream::Binary|CStream::Write);
                
                int FileSize = 0, Size;
                Buffer = LYNXNEW UInt8[CHUNK_SIZE];
                
                do {
                    Size = CFReadStreamRead(m_RStream, Buffer, CHUNK_SIZE);
                    
                    if (Size < 0)
                    {
                        LYNXDEL_ARRAY(Buffer);
                        m_FileStream.vClose();
                        CFReadStreamClose(m_RStream);
                        CFRelease(m_RStream);
                        m_RStream = NULL;
                        m_bValid = LYNX_FALSE;
                        return LYNX_FALSE;
                    }
                    
                    LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                    do {
                        Status = CFReadStreamGetStatus(m_RStream);
                        if (Status == kCFStreamStatusError || (lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                        {
                            LYNXDEL_ARRAY(Buffer);
                            CFReadStreamClose(m_RStream);
                            CFRelease(m_RStream);
                            m_RStream = NULL;
                            m_bValid = LYNX_FALSE;
                            
                            return LYNX_FALSE;
                        }
                    } while (Status == kCFStreamStatusReading);
                    m_FileStream.vWrite(Buffer, sizeof(UInt8), Size);
                    m_CurrentProgressSize += Size;
                    FileSize += Size;
                }while (Size != 0);
                
                m_FileStream.vClose();
                
                m_FileStream.vOpen(_T("../temp/temp_read_stream.bin"), CStream::Binary|CStream::Read);
                
                LYNXDEL_ARRAY(Buffer);
                m_bValid = LYNX_TRUE;
                
                return LYNX_TRUE;               
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CURLStream::vClose() 
		{
            if (m_bValid)
            {
                CFStreamStatus Status;
                
                if (m_RStream)
                {
                    if (m_FileStream.IsValid())
                        m_FileStream.vClose();
                    CFReadStreamClose(m_RStream);
                    
                    LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                    do {
                        Status = CFReadStreamGetStatus(m_RStream);
                        if (Status == kCFStreamStatusError || (lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                        {
                            break;
                        }
                    } while (Status != kCFStreamStatusClosed);
                    
                    CFRelease(m_RStream);
                    m_RStream = NULL;
                }                
                else if (m_WStream)
                {
                    CFWriteStreamClose(m_WStream);
                    
                    LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                    do {
                        Status = CFWriteStreamGetStatus(m_WStream);
                        if (Status == kCFStreamStatusError || (lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                        {
                            break;
                        }
                    } while (Status != kCFStreamStatusClosed);
                    
                    CFRelease(m_WStream);
                    m_WStream = NULL;
                }                     	
            }			
			m_bValid = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vRead(void *lpdst, int size, int len) 
		{
            return m_FileStream.vRead(lpdst, size, len);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vRead(CString& str) 
		{
			return m_FileStream.vRead(str);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vWrite(const void *lpsrc, int size, int len)
		{
			return m_FileStream.vWrite(lpsrc, size, len);
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:   
        //-------------------------------------------------------------------------------------------------------
        int CURLStream::vWrite(const CString& str)
        {
            return 0;	
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vSeek(LYNXDWORD len, int flag)
		{
			return m_FileStream.vSeek(len, flag);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CURLStream::vTell()
		{
			return m_FileStream.vTell();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CURLStream::vSize()
		{
			return m_FileStream.vSize();
		}

		//-------------------------------------------------------------------------------------------------------
		//
		//  Implementation of CURLResumableStream
		//
		//-------------------------------------------------------------------------------------------------------
		CURLResumableStream::CURLResumableStream(void)	
			:CURLStream()
		{			
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CURLResumableStream::~CURLResumableStream(void)
		{
			vClose();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------		        
        LYNXBOOL CURLResumableStream::vOpen(const CString& name, int flag) 
        {            
            UInt8* Buffer = NULL;
            CFStreamStatus Status;
            
            m_bValid = LYNX_FALSE;
            
            CFStringRef URLString = CFStringCreateWithCString(NULL, (const char*)name.c_str(), NSASCIIStringEncoding);
            CFURLRef URL = CFURLCreateWithString(NULL, URLString, NULL);
            
            m_CurrentProgressSize = 0;
            
            // local file size as start download file size.
            int localFileLength = 0;
            
            if (flag & Write)
            {
                return CURLStream::vOpen(name, flag);                    
            }
            else
            {
                // assign target temp file.
                LYNXCHAR TempFile[256];
                
                // If the URL is using ftp
                if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("ftp"), 0) == kCFCompareEqualTo)
                {
                    m_RStream = CFReadStreamCreateWithFTPURL(NULL, URL);
                    if (!m_RStream)
                    {
                        m_bValid = LYNX_FALSE;
                      
                        CFRelease(URLString);
                        CFRelease(URL);
                        
                        return LYNX_FALSE;
                    }
                }
                // If the URL is using http
                else if (CFStringCompare(CFURLCopyScheme(URL), CFSTR("http"), 0) == kCFCompareEqualTo || CFStringCompare(CFURLCopyScheme(URL), CFSTR("https"), 0) == kCFCompareEqualTo)
                {
					LYNXCHAR FileNameWithoutExt[256], TempFileWithoutPath[256];
					lynxGetFilenameOnlyByPath(FileNameWithoutExt, name.c_str());
					lynxSprintf(TempFile, _T("../temp/%s.bin"), FileNameWithoutExt);
					lynxSprintf(TempFileWithoutPath, _T("/%s.bin"), FileNameWithoutExt);
				
                    CFHTTPMessageRef Request = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("GET"), URL, kCFHTTPVersion1_1);
                                        
                    NSFileManager* fileMgr = [NSFileManager defaultManager];
                    // NSString* filePath = (NSString*)CFURLGetString(URL);
                    FileSystem::iOS::CFileSystem* FileSystem = static_cast<FileSystem::iOS::CFileSystem*>(glpKernel->GetlpEngine()->GetlpFileSystem());
                    const CString& FullPath = FileSystem->GetAppSupportPath(TempFile);
                    NSString* FullPathStr = nil;
                    lynxcString2NSString(&FullPathStr, FullPath.c_str());
                    NSString* TempFileStr = nil;
					lynxcString2NSString(&TempFileStr, TempFileWithoutPath);
					
                    NSString* LocalTempFile = [NSString stringWithFormat:@"%@%@",
                                      [FullPathStr stringByDeletingLastPathComponent], TempFileStr];
                                        
                    if ([fileMgr fileExistsAtPath:LocalTempFile])
                    {
                        NSLog(@"[URLStream] localTempFile:%@ exist.", LocalTempFile);
                        
                        NSError* error = nil;
                        NSDictionary* fileDict = [fileMgr attributesOfItemAtPath:LocalTempFile error:&error];
                        
                        if (!error && fileDict) {
                            localFileLength = (int)[fileDict fileSize];
                            NSLog(@"[URLStream] localTempFile exists with bytes:%d", localFileLength);
                        }
                        else
                        {
                            if (error) {
                                NSLog(@"[URLStream] get filesize of localTempFile error:%@", [error description]);
                            }
                            if (!fileDict) {
                                NSLog(@"[URLStream] get filesize of localTempFile error:%@", [fileDict description]);
                            }
                            // [fileMgr createFileAtPath:filePath contents:nil attributes:nil];							
                        }
                    }
                    
                    if (localFileLength>0)
                    {
                        NSString* localFileLengthString = [NSString stringWithFormat:@"bytes=%d-", localFileLength];
                        CFStringRef ReqRangeKey = CFStringCreateWithCString(NULL, "Range", kCFStringEncodingUTF8);
                        CFStringRef RegRangeValue = (__bridge CFStringRef)localFileLengthString;
                        CFHTTPMessageSetHeaderFieldValue(Request, ReqRangeKey, RegRangeValue);
                    }
                    
                    m_RStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, Request);
                    
                    if (!m_RStream)
                    {
                        m_bValid = LYNX_FALSE;
                        CFRelease(URLString);
                        CFRelease(URL);
                        return LYNX_FALSE;
                    }
                    CFRelease(Request);
                }
                else
                {
                    m_bValid = LYNX_FALSE;
                    CFRelease(URLString);
                    CFRelease(URL);
                    return LYNX_FALSE;
                }
                CFRelease(URLString);
                CFRelease(URL);
                
                m_bValid = CFReadStreamOpen(m_RStream);
                if (!m_bValid)
                    return LYNX_FALSE;
                                                               
                // m_FileStream.vOpen(_T("../temp/temp_read_stream.bin"), CStream::Binary|CStream::Write);
                m_FileStream.vOpen(TempFile, CStream::Binary|CStream::Append);
                
                int Size;
                m_CurrentProgressSize = localFileLength;
                
                NSLog(@"[URLStream] getContentLength in bytes:%d, %d.",localFileLength ,m_FileStream.vSize());

                Buffer = LYNXNEW UInt8[CHUNK_SIZE];                
                do {
                    Size = CFReadStreamRead(m_RStream, Buffer, CHUNK_SIZE);
                    
                    if (Size < 0)
                    {
                        LYNXDEL_ARRAY(Buffer);
                        m_FileStream.vClose();
                        CFReadStreamClose(m_RStream);
                        CFRelease(m_RStream);
                        m_RStream = NULL;
                        m_bValid = LYNX_FALSE;
                        return LYNX_FALSE;
                    }
                    
                    LYNXLONGLONG T = lynxOSGetPerformanceCounter();
                    do {
                        Status = CFReadStreamGetStatus(m_RStream);
                        if (Status == kCFStreamStatusError || (lynxOSGetPerformanceCounter() - T)*1000.0f/((float)lynxOSGetPerformanceFrequency()) > TIMEOUT)
                        {
                            LYNXDEL_ARRAY(Buffer);
                            CFReadStreamClose(m_RStream);
                            CFRelease(m_RStream);
                            m_RStream = NULL;
                            m_bValid = LYNX_FALSE;
                            
                            return LYNX_FALSE;
                        }
                    } while (Status == kCFStreamStatusReading);
                    m_FileStream.vWrite(Buffer, sizeof(UInt8), Size);
                    // m_FileStream.vAppend(Buffer, sizeof(UInt8), Size);
                    m_CurrentProgressSize += Size;
                }while (Size != 0);
                
                m_FileStream.vClose();                
                m_FileStream.vOpen(TempFile, CStream::Binary|CStream::Read);
                
                LYNXDEL_ARRAY(Buffer);
                m_bValid = LYNX_TRUE;
                
                return LYNX_TRUE;               
            }
        }
	}
}
