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
#include <Platform/iOS/LynxiOSZipStream.h>

#import <Foundation/NSString.h>
#import <Foundation/NSFileHandle.h>

namespace LynxEngine 
{	
	namespace iOS
	{
        CFileStream CZipStream::m_FileStream;
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static voidpf ZCALLBACK iOS_zlib_OpenFile(voidpf opaque, const char* filename, int mode)
        {
			int Mode = CStream::Binary;

			if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
			{
				Mode |= CStream::Read;
			}
			else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
			{
				Mode |= CStream::Read;//mode_fopen = "r+b";
			}
			else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
			{
				Mode |= CStream::Write;
			}        
			
            CZipStream::m_FileStream.vOpen(filename, Mode);
            return &CZipStream::m_FileStream;
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static uLong ZCALLBACK iOS_zlib_ReadFile(voidpf opaque, voidpf stream, void* buf, uLong size)
        {
            uLong ret;
            
            CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;
            ret = lpFile->vRead(buf, 1, (size_t)size);
            return ret;
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static uLong ZCALLBACK iOS_zlib_WriteFile(voidpf opaque, voidpf stream, const void* buf, uLong size)
        {
            uLong ret;
            
            CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;
            ret = lpFile->vWrite(buf, 1, (size_t)size);
            return ret;
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static long ZCALLBACK iOS_zlib_TellFile(voidpf opaque, voidpf stream)
        {
            long ret;
            CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;
            ret = lpFile->vTell();
            return ret;
        }	
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static long ZCALLBACK iOS_zlib_SeekFile(voidpf  opaque, voidpf stream, uLong offset, int origin)
        {
            int fseek_origin=0;
            long ret;
            CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;
            switch (origin)
            {
                case ZLIB_FILEFUNC_SEEK_CUR :
                    fseek_origin = CStream::SeekCur;
                    break;
                case ZLIB_FILEFUNC_SEEK_END :
                    fseek_origin = CStream::SeekEnd;
                    break;
                case ZLIB_FILEFUNC_SEEK_SET :
                    fseek_origin = CStream::SeekSet;
                    break;
                default: return -1;
            }
            ret = 0;
            if (lpFile->vSeek(offset, fseek_origin) != 0)
                ret = -1;
            return ret;
        }	
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static int ZCALLBACK iOS_zlib_CloseFile(voidpf opaque, voidpf stream)
        {
            int ret;
            CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;
            
            lpFile->vClose();
            return ret;
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:   
        //-------------------------------------------------------------------------------------------------------
        static int ZCALLBACK iOS_zlib_ErrorFile(voidpf opaque, voidpf stream)
        {
            int ret = 0;
            //ret = ferror((FILE *)stream);
            return ret;
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CZipStream::CZipStream(void)	
		{
            m_ZLibFileFuncDef.zopen_file	= iOS_zlib_OpenFile;
            m_ZLibFileFuncDef.zclose_file	= iOS_zlib_CloseFile;
            m_ZLibFileFuncDef.zerror_file	= iOS_zlib_ErrorFile;
            m_ZLibFileFuncDef.zread_file	= iOS_zlib_ReadFile;
            m_ZLibFileFuncDef.zwrite_file	= iOS_zlib_WriteFile;
            m_ZLibFileFuncDef.ztell_file	= iOS_zlib_TellFile;
            m_ZLibFileFuncDef.zseek_file	= iOS_zlib_SeekFile;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CZipStream::~CZipStream(void)
		{
		}
	}
}