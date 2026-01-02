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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxMemoryStream.h>
#include <LynxZipStream.h>

namespace LynxEngine 
{	
   //-------------------------------------------------------------------------------------------------------
    //
    //  說明:   
    //-------------------------------------------------------------------------------------------------------
    static voidpf ZCALLBACK zlib_OpenFile(voidpf opaque, const char* filename, int mode)
    {
		CPlatformFileStream* lpFile = LYNXNEW CPlatformFileStream;

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

        if (!lpFile->vOpen(filename, Mode))
		{
			LYNXDEL(lpFile);
			return NULL;
		}
        return lpFile;
    }
    //-------------------------------------------------------------------------------------------------------
    //
    //  說明:   
    //-------------------------------------------------------------------------------------------------------
    static uLong ZCALLBACK zlib_ReadFile(voidpf opaque, voidpf stream, void* buf, uLong size)
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
    static uLong ZCALLBACK zlib_WriteFile(voidpf opaque, voidpf stream, const void* buf, uLong size)
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
    static long ZCALLBACK zlib_TellFile(voidpf opaque, voidpf stream)
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
    static long ZCALLBACK zlib_SeekFile(voidpf  opaque, voidpf stream, uLong offset, int origin)
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
    static int ZCALLBACK zlib_CloseFile(voidpf opaque, voidpf stream)
    {
        int ret = 0;
        CPlatformFileStream* lpFile = (CPlatformFileStream*)stream;        
        lpFile->vClose();
		LYNXDEL(lpFile);

        return ret;
    }
    //-------------------------------------------------------------------------------------------------------
    //
    //  說明:   
    //-------------------------------------------------------------------------------------------------------
    static int ZCALLBACK zlib_ErrorFile(voidpf opaque, voidpf stream)
    {
        int ret = 0;
        //ret = ferror((FILE *)stream);
        return ret;
    }



//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static voidpf ZCALLBACK zlib_OpenFile(voidpf opaque, const char* filename, int mode)
//	{
//		FILE* file = NULL;
//		const char* mode_fopen = NULL;
//		if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
//			mode_fopen = "rb";
//		else
//		if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
//			mode_fopen = "r+b";
//		else
//		if (mode & ZLIB_FILEFUNC_MODE_CREATE)
//			mode_fopen = "wb";
//
//		if ((filename!=NULL) && (mode_fopen != NULL))
//			file = fopen(filename, mode_fopen);
//		return file;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static uLong ZCALLBACK zlib_ReadFile(voidpf opaque, voidpf stream, void* buf, uLong size)
//	{
//		uLong ret;
//		ret = (uLong)fread(buf, 1, (size_t)size, (FILE *)stream);
//		return ret;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static uLong ZCALLBACK zlib_WriteFile(voidpf opaque, voidpf stream, const void* buf, uLong size)
//	{
//		uLong ret;
//		ret = (uLong)fwrite(buf, 1, (size_t)size, (FILE *)stream);
//		return ret;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static long ZCALLBACK zlib_TellFile(voidpf opaque, voidpf stream)
//	{
//		long ret;
//		ret = ftell((FILE *)stream);
//		return ret;
//	}	
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static long ZCALLBACK zlib_SeekFile(voidpf  opaque, voidpf stream, uLong offset, int origin)
//	{
//		int fseek_origin=0;
//		long ret;
//		switch (origin)
//		{
//		case ZLIB_FILEFUNC_SEEK_CUR :
//			fseek_origin = SEEK_CUR;
//			break;
//		case ZLIB_FILEFUNC_SEEK_END :
//			fseek_origin = SEEK_END;
//			break;
//		case ZLIB_FILEFUNC_SEEK_SET :
//			fseek_origin = SEEK_SET;
//			break;
//		default: return -1;
//		}
//		ret = 0;
//		if (fseek((FILE *)stream, offset, fseek_origin) != 0)
//			ret = -1;
//		return ret;
//	}	
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static int ZCALLBACK zlib_CloseFile(voidpf opaque, voidpf stream)
//	{
//		int ret;
//		ret = fclose((FILE *)stream);
//		return ret;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static int ZCALLBACK zlib_ErrorFile(voidpf opaque, voidpf stream)
//	{
//		int ret;
//		ret = ferror((FILE *)stream);
//		return ret;
//	}
//#ifdef __X64__	
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static voidpf ZCALLBACK zlib_OpenFile64(voidpf opaque, const void* filename, int mode)
//	{
//		FILE* file = NULL;
//		const char* mode_fopen = NULL;
//		if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
//			mode_fopen = "rb";
//		else
//			if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
//				mode_fopen = "r+b";
//			else
//				if (mode & ZLIB_FILEFUNC_MODE_CREATE)
//					mode_fopen = "wb";
//		
//		if ((filename!=NULL) && (mode_fopen != NULL))
//			file = fopen64((const char*)filename, mode_fopen);
//		return file;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static ZPOS64_T ZCALLBACK zlib_TellFile64(voidpf opaque, voidpf stream)
//	{
//		ZPOS64_T ret;
//		ret = ftello64((FILE *)stream);
//		return ret;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	//
//	//  說明:   
//	//-------------------------------------------------------------------------------------------------------
//	static long ZCALLBACK zlib_SeekFile64(voidpf  opaque, voidpf stream, ZPOS64_T offset, int origin)
//	{
//		int fseek_origin=0;
//		long ret;
//		switch (origin)
//		{
//			case ZLIB_FILEFUNC_SEEK_CUR :
//				fseek_origin = SEEK_CUR;
//				break;
//			case ZLIB_FILEFUNC_SEEK_END :
//				fseek_origin = SEEK_END;
//				break;
//			case ZLIB_FILEFUNC_SEEK_SET :
//				fseek_origin = SEEK_SET;
//				break;
//			default: return -1;
//		}
//		ret = 0;
//		
//		if(fseeko64((FILE *)stream, offset, fseek_origin) != 0)
//			ret = -1;
//		
//		return ret;
//	}
//#endif
    zlib_filefunc_def CZipStream::m_ZLibFileFuncDef;
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CZipStream::CZipStream(void)	
	{
		m_ZipFile = NULL;
		m_StreamType = ZIP_STREAM;
		m_lpMemStream = NULL;
		m_bSearchFisrt = LYNX_TRUE;
        m_lpFileStream = LYNXNEW CPlatformFileStream;
		//m_lpFileStream = LYNXNEW CMemoryStream;
		m_ZLibFileFuncDef.zopen_file	= zlib_OpenFile;
		m_ZLibFileFuncDef.zclose_file	= zlib_CloseFile;
		m_ZLibFileFuncDef.zerror_file	= zlib_ErrorFile;
		m_ZLibFileFuncDef.zread_file	= zlib_ReadFile;
		m_ZLibFileFuncDef.zwrite_file	= zlib_WriteFile;
		m_ZLibFileFuncDef.ztell_file	= zlib_TellFile;
		m_ZLibFileFuncDef.zseek_file	= zlib_SeekFile;		
		m_Password = "";
		m_ExtractingProgress = 0.0f;
		m_TotalFiles = 0;
		m_CurrentExtractedFiles = 0;
		m_TotalSize = 0;
		m_CurrentExtractedSize = 0;
		m_OpenedCount = 0;
		m_bUsed = LYNX_FALSE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CZipStream::~CZipStream(void)
	{
		if (m_lpFileStream)
			LYNXDEL(m_lpFileStream);

		if (m_lpMemStream)
		{
			m_lpMemStream->vClose();
			LYNXDEL(m_lpMemStream);
			m_lpMemStream = NULL;
		}
		m_bUsed = LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::OpenZipFile(const CString& name, LYNXBOOL bsearch)
	{
	#if 0
		return TRUE;
	#else
		LYNXCHAR FullName[128];
		char AnsiFullName[128];
		int	 i, ci;		

		m_bSearchFisrt = bsearch;

		lynxStrCpy(FullName, name.c_str());
		lynxStrLower(FullName);	
		lynxAnsiStr(AnsiFullName, FullName);
		m_ZipFile = unzOpen2(AnsiFullName, &m_ZLibFileFuncDef);
		if (!m_ZipFile)
		{
			m_bValid = LYNX_FALSE;	
			m_OpenedCount = 0;
			return FALSE;
		}

		m_ZipFilename = name;
		m_bSearch = bsearch;

		unz_global_info GlobalInfo;
		unzGetGlobalInfo(m_ZipFile, &GlobalInfo);
		m_TotalFiles = GlobalInfo.number_entry;
		m_CurrentExtractedFiles = 0;		
		m_bValid = LYNX_FALSE;	
		m_OpenedCount = 0;
		
		return TRUE;
	#endif
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CZipStream&	CZipStream::operator =(const CZipStream& rhs)
	{
		if (rhs.m_ZipFile)
			m_ZipFile = rhs.m_ZipFile;
			//OpenZipFile(rhs.m_ZipFilename, rhs.m_bSearch);		

		return (*this);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::FindFile(const CAnsiString& filename)
	{
		LYNXCHAR Name[128];	
		LYNXCHAR FullName[128];	
		char AnsiStr[128];
		int i, ci;

		lynxStrCpy(Name, filename.c_str());
		lynxStrCpy(FullName, Name);
		for (i=0, ci=0; i<lynxStrLen(Name); i++)
		{
			if (Name[i] == _T('\\'))
			{
				if (Name[i+1] == _T('\\'))
				{
					FullName[ci++] = _T('/');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('/');				
				}
			}
			else if (Name[i] == _T('/') && Name[i+1] == _T('/'))
			{
				FullName[ci++] = _T('/');							
				i++;
			}
			else if (i == 0 && Name[i] == '.') // handle "../" and "./"
			{
				if ((i+1) < lynxStrLen(Name) && Name[i+1] == '.')			
				{
					i += 2;
				}
				else
				{
					i += 1;
				}
			}
			else
				FullName[ci++] = Name[i];
		}				
		FullName[ci++] = lynxFromAnsi(0x00);	
		lynxStrLower(FullName);	
		
		if (FullName[0] == _T('/'))
		{
			lynxAnsiStr(AnsiStr, &FullName[1]);
			if (unzLocateFile(m_ZipFile, AnsiStr, 2) != UNZ_OK)
            {
				return LYNX_FALSE;
			}
		}
		else
		{
			lynxAnsiStr(AnsiStr, FullName);
			if (unzLocateFile(m_ZipFile, AnsiStr, 2) != UNZ_OK)
            {
                return LYNX_FALSE;
            }
		}	
		return LYNX_TRUE;
	}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CZipStream::OpenZipFile(const CString& name, void* lpd, LYNXLONG size)
	//{
	//	m_ZipFile = unzOpenArray((unsigned char*)lpd, size);
	//	if (!m_ZipFile)
	//		return FALSE;

	//	m_bValid = LYNX_TRUE;

	//	return TRUE;
	//}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CZipStream::CloseZipFile()
	{
		if (m_ZipFile)
			unzClose(m_ZipFile);
		m_ZipFile = NULL;

		vClose();

		m_OpenedCount = 0;
		m_bValid = LYNX_FALSE;	
		m_bUsed = LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::ReadCurrentFile(LYNXANSICHAR* filename, int len) 
	{		
		DWORD Len;
		unz_file_info info;

		if (!m_Password.empty())
		{			
			if (unzOpenCurrentFilePassword(m_ZipFile, m_Password.c_str()) != UNZ_OK)
			{
				return FALSE;
			}
		}
		else
		{
			if (unzOpenCurrentFile(m_ZipFile) != UNZ_OK)
			{
				return FALSE;
			}
		}

		if (unzGetCurrentFileInfo(m_ZipFile, &info, filename, len, NULL, 0, NULL, 0) != UNZ_OK)
        {
            unzCloseCurrentFile(m_ZipFile);
            return FALSE;
        }

		Len = info.uncompressed_size;	

		if (m_lpMemStream)
		{
			m_lpMemStream->vClose();
			LYNXDEL(m_lpMemStream);
			m_lpMemStream = NULL;
		}

		if (Len)
		{
			m_lpMemStream = LYNXNEW CMemoryStream;
			m_lpMemStream->Alloc(Len);
			
			if (unzReadCurrentFile(m_ZipFile, m_lpMemStream->GetCurrentPtr(), Len) < 0)
            {
                m_lpMemStream->vClose();
                LYNXDEL(m_lpMemStream);
                m_lpMemStream = NULL;
                
                unzCloseCurrentFile(m_ZipFile);
                return LYNX_FALSE;
            }
		}
		unzCloseCurrentFile(m_ZipFile);	

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::OpenPlatformFile(const CString& name, int flag) 
	{
		m_lpFileStream->vClose();
		if (m_lpFileStream->vOpen(name, flag))
		{			
			m_bValid = LYNX_TRUE;		
			m_OpenedCount = 1;
			return LYNX_TRUE;
		}
		else
		{
			m_bValid = LYNX_FALSE;
			m_OpenedCount = 0;
			m_bUsed = LYNX_FALSE;
			return LYNX_FALSE;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::vOpen(const CString& name, int flag) 
	{
		DWORD Len;
		unz_file_info info;
		LYNXCHAR Name[128];	
		LYNXCHAR FullName[128];	
		char AnsiStr[128];
		int	 i, ci;
		
		#ifdef _DEBUG
			m_Filename = name;			
		#endif

		if (!m_ZipFile)
		{
			return OpenPlatformFile(name, flag);
		}

		if (m_bSearchFisrt)
		{			
			m_lpFileStream->vClose();
			if (m_lpFileStream->vOpen(name, flag))
			{
				m_bValid = LYNX_TRUE;		
				m_OpenedCount = 1;
				return LYNX_TRUE;
			}
			else
			{
				if (!m_ZipFile)
				{
					m_bValid = LYNX_FALSE;
					m_OpenedCount = 0;
					m_bUsed = LYNX_FALSE;
					return LYNX_FALSE;
				}
			}
		}		
		
		lynxStrCpy(Name, name.c_str());
		lynxStrCpy(FullName, Name);
		for (i=0, ci=0; i<lynxStrLen(Name); i++)
		{
			if (Name[i] == _T('\\'))
			{
				if (Name[i+1] == _T('\\'))
				{
					FullName[ci++] = _T('/');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('/');				
				}
			}
			else if (Name[i] == _T('/') && Name[i+1] == _T('/'))
			{
				FullName[ci++] = _T('/');							
				i++;
			}
			else if (i == 0 && Name[i] == '.') // handle "../" and "./"
			{
				if ((i+1) < lynxStrLen(Name) && Name[i+1] == '.')			
				{
					i += 2;
				}
				else
				{
					i += 1;
				}
			}
			else
				FullName[ci++] = Name[i];
		}				
		FullName[ci++] = lynxFromAnsi(0x00);	
		lynxStrLower(FullName);	
		
		if (FullName[0] == _T('/'))
		{
			lynxAnsiStr(AnsiStr, &FullName[1]);
			if (unzLocateFile(m_ZipFile, AnsiStr, 2) != UNZ_OK)
            {
                return OpenPlatformFile(name, flag);
            }
		}
		else
		{
			lynxAnsiStr(AnsiStr, FullName);
			if (unzLocateFile(m_ZipFile, AnsiStr, 2) != UNZ_OK)
            {
                return OpenPlatformFile(name, flag);
            }
		}	
		
		if (!ReadCurrentFile())
		{
			return OpenPlatformFile(name, flag);
		}		

		m_bValid = LYNX_TRUE;		
		m_OpenedCount++;

		return TRUE;    
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CZipStream::vClose() 
	{
		if (m_ZipFile && m_lpMemStream)
		{
			m_lpMemStream->vClose();
			LYNXDEL(m_lpMemStream);
			m_lpMemStream = NULL;
			m_OpenedCount--;			
		}
		else
		{
			if (m_lpFileStream && m_lpFileStream->IsValid())
			{
				m_lpFileStream->vClose();
				m_OpenedCount--;
			}
			m_bValid = LYNX_FALSE;	
			m_bUsed = LYNX_FALSE;			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CZipStream::vRead(void *lpdst, int size, int len) 
	{
		if (m_lpFileStream->IsValid())
			return m_lpFileStream->vRead(lpdst, size, len);
		else if (m_lpMemStream)
			return m_lpMemStream->vRead(lpdst, size, len);

		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CZipStream::vRead(CString& str) 
	{
		if (m_lpFileStream->IsValid())
			return m_lpFileStream->vRead(str);			
		else if (m_lpMemStream)
			return m_lpMemStream->vRead(str);			

		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CZipStream::vWrite(const void *lpsrc, int size, int len)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CZipStream::vWrite(const CString& str)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CZipStream::vSeek(LYNXDWORD len, int flag)
	{
		if (m_lpFileStream->IsValid())
			return m_lpFileStream->vSeek(len, flag);
		else if (m_lpMemStream)
			return m_lpMemStream->vSeek(len, flag);

		return -1;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CZipStream::vTell()
	{
		if (m_lpFileStream->IsValid())
			return m_lpFileStream->vTell();	
		else if (m_lpMemStream)
			return m_lpMemStream->vTell();	

		return 0;	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CZipStream::vSize()
	{
		if (m_lpFileStream->IsValid())
			return m_lpFileStream->vSize();
		else if (m_lpMemStream)
			return m_lpMemStream->vSize();

		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CZipStream::CopyToFile(CPlatformFileStream& fs)
	{
		if (m_lpMemStream)
		{
			fs.vWrite(m_lpMemStream->GetCurrentPtr(), sizeof(char), m_lpMemStream->vSize());
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	zipFile CZipStream::CreateZipFile(const CAnsiString& filename)
	{
		m_ZLibFileFuncDef.zopen_file	= zlib_OpenFile;
		m_ZLibFileFuncDef.zclose_file	= zlib_CloseFile;
		m_ZLibFileFuncDef.zerror_file	= zlib_ErrorFile;
		m_ZLibFileFuncDef.zread_file	= zlib_ReadFile;
		m_ZLibFileFuncDef.zwrite_file	= zlib_WriteFile;
		m_ZLibFileFuncDef.ztell_file	= zlib_TellFile;
		m_ZLibFileFuncDef.zseek_file	= zlib_SeekFile;		

		zipFile zf = zipOpen2(filename.c_str(), APPEND_STATUS_CREATE, NULL, &m_ZLibFileFuncDef);

		return zf;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CZipStream::AddFile2ZipFile(zipFile zf, const CString& filename, const CAnsiString& name, const CAnsiString& password)
	{
		zip_fileinfo zfi;
		CPlatformFileStream File;
		unsigned char Buffer[1024];
		LYNXDWORD CRC = crc32(0L, Z_NULL, 0);
		int Size, Len;

		if (password.empty())
		{
			int ret = zipOpenNewFileInZip(zf,
				name.c_str(),
				&zfi,
				NULL,   0,
				NULL,   0,
				NULL,
				Z_DEFLATED,
				Z_BEST_COMPRESSION
			);
		}
		else
		{
			if (File.vOpen(filename, CStream::Binary|CStream::Read))
			{	
				Len = File.vSize();
				unsigned char* Buffer = LYNXGLOBAL_NEW unsigned char[Len];
				Size = Len;
				//Size = (Len < 10000) ? Len : 10000;
				File.vRead(Buffer, 1, Size);			
				File.vClose();			
				CRC = crc32(CRC, Buffer, Size);
				LYNXGLOBAL_DEL_ARRAY(Buffer);
			}

			int ret = zipOpenNewFileInZip3(zf,
				name.c_str(),
				&zfi,
				NULL,   0,
				NULL,   0,
				NULL,
				Z_DEFLATED,
				Z_BEST_COMPRESSION,
				0,
				-MAX_WBITS,//15,
				DEF_MEM_LEVEL,//8,
				Z_DEFAULT_STRATEGY,
				password.c_str(),
				CRC
			);
		}		
		
		if (File.vOpen(filename, CStream::Binary|CStream::Read))
		{	
			Len = File.vSize();
			do {
				Size = (Len < 1024) ? Len : 1024;
				File.vRead(Buffer, 1, Size);
				zipWriteInFileInZip(zf, Buffer, Size);
				Len -= Size;
			}while (Len);

			File.vClose();			
		}
		zipCloseFileInZip(zf);
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CZipStream::ExtractAll(const CString& path) 
	{
		CString FullPath;
		LYNXDWORD Len;
		LYNXANSICHAR Filename[2048];
		CPlatformFileStream File;

		if (!m_ZipFile)
        {
           	return LYNX_FALSE;
        }

		if (unzGoToFirstFile(m_ZipFile) != UNZ_OK)
			return LYNX_FALSE;

		if (!ReadCurrentFile(Filename, 2040))
            return LYNX_FALSE;
        
		m_CurrentExtractedFiles++;
		if (m_lpMemStream)
		{			
			FullPath = path + CString(Filename);
			Len = m_lpMemStream->vSize();
			if (File.vOpen(FullPath, Binary|Write))
			{
				File.vWrite(m_lpMemStream->GetCurrentPtr(), 1, Len);
				File.vClose();				
			}
			m_lpMemStream->vClose();
			LYNXDEL(m_lpMemStream);
			m_lpMemStream = NULL;
		}

		while (unzGoToNextFile(m_ZipFile) != UNZ_END_OF_LIST_OF_FILE)
		{
			if (!ReadCurrentFile(Filename, 2040))
                return LYNX_FALSE;
            
			m_CurrentExtractedFiles++;
			if (m_lpMemStream)
			{				
				FullPath = path + CString(Filename);
				Len = m_lpMemStream->vSize();
				if (File.vOpen(FullPath, Binary|Write))
				{
					File.vWrite(m_lpMemStream->GetCurrentPtr(), 1, Len);
					File.vClose();					
				}                
                m_lpMemStream->vClose();
                LYNXDEL(m_lpMemStream);
                m_lpMemStream = NULL;
			}
		}

		return TRUE;    
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CZipStream::CloseZipFile(zipFile zf)
	{
		zipClose(zf, NULL);
	}	
}