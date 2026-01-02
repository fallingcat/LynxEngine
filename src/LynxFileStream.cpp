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
#include <LynxFileStream.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CFileStream::CFileStream(void)	
	{
		m_StreamType = FILE_STREAM;
		m_lpFile = NULL;		
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CFileStream::~CFileStream(void)
	{
		vClose();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CFileStream::vOpen(const CString& name, int flag) 
	{
		#if (defined __GNUC__ && defined _UNICODE)
			return LYNX_FALSE;
		#else
			LYNXCHAR Flag[32] = _T("");

			m_Flag = flag;

			if (flag & Write)
			{
				lynxStrCat(Flag, _T("w"));
				LYNXCHAR Path[1024], Name[1024], Ext[1024];
				lynxSeparatePathFilenameExtByFullPath(Path, Name, Ext, name.c_str());
				if (lynxStrLen(Path) != 0)
					lynxCreateFolder(Path);
			}
			else if (flag & Read)
			{
				lynxStrCat(Flag, _T("r"));
			}
			else if (flag & Append)
			{
				lynxStrCat(Flag, _T("a"));
			}
			else
			{
				lynxStrCat(Flag, _T("r"));
			}

			if (flag & Binary)
				lynxStrCat(Flag, _T("b"));
			else if (flag & Text)
			{
				lynxStrCat(Flag, _T("t"));
				if (flag & ANSI)
				{
				}
				else if (flag & UTF8)
				{
					//lynxStrCat(Flag, _T(",ccs=UTF-8"));				
				}				
				else if (flag & UTF16)
				{
					lynxStrCat(Flag, _T(",ccs=UTF-16LE"));				
				}
				else
				{					
					#if defined(_UNICODE) || defined (UNICODE)
						lynxStrCat(Flag, _T(",ccs=UTF-8"));				
					#endif
				}
			}
			else
				lynxStrCat(Flag, _T("b"));

			if (glpKernel && glpKernel->GetlpEngine() && glpKernel->GetlpEngine()->GetlpFileSystem())
			{
				CString FullPath;
				FileSystem::CFileSystem* FileSystem = glpKernel->GetlpEngine()->GetlpFileSystem();
				for (int i = 0; i < FileSystem->GetNumFileSearchingPath(); i++)
				{
					FullPath = FileSystem->GetFullSearchingPath(i, name);
					m_lpFile = _tfopen(FullPath.c_str(), Flag);
					if (m_lpFile != NULL)
					{
						vSeek(0, SeekSet);
						m_bValid = LYNX_TRUE;

						#if defined( __WIN32__) && !SHIPPING
							if (FullPath.find(_T("cached")) == CString::npos && FullPath.find(_T("../../engine")) == CString::npos)
							{
								const LYNXCHAR* Filename = &(FullPath.c_str()[3]);
								FileSystem->m_UsedFileMap[Filename] = 1;
							}
						#endif
						return TRUE;
					}
				}				
			}
			else
			{
				m_lpFile = _tfopen(name.c_str(), Flag);
				if (m_lpFile != NULL)
				{
					vSeek(0, SeekSet);
					m_bValid = LYNX_TRUE;
					return TRUE;
				}
			}			

			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::")) + CString(_T("[Error] Failed to open ")) + name + CString(_T("\r\n")));
			return FALSE;			
		#endif
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CFileStream::vClose() 
	{
		if (m_lpFile)
		{
			fclose(m_lpFile);			
			m_lpFile = NULL;
		}
		m_bValid = LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CFileStream::vRead(void *lpdst, int size, int len) 
	{
		return (int)fread(lpdst, size, len, m_lpFile);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CFileStream::vRead(CString& str) 
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
		
		return str.length();

		/*
		int Len = vSize() + 10;

		LYNXCHAR* Str = (LYNXCHAR*)(LYNXNEW LYNXANSICHAR[Len]);

		if (_fgetts(Str, Len, m_lpFile))
		{			
			str = Str;
			LYNXDEL_ARRAY(Str);
			return str.length();
		}
		else
		{
			LYNXDEL_ARRAY(Str);
			return 0;		
		}
		*/
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CFileStream::vWrite(const void *lpsrc, int size, int len)
	{
		return (int)fwrite(lpsrc, size, len, m_lpFile);	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CFileStream::vWrite(const CString& str)
	{
		return (int)_fputts(str.c_str(), m_lpFile);	
	}	
	#ifdef _UNICODE
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vRead(CAnsiString& str) 
		{
			char Str[1024];

			if (fgets(Str, 1024, m_lpFile))
			{
				str = Str;
				return str.length();
			}
			else
				return 0;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vWrite(const CAnsiString& str)
		{
			return (int)fputs(str.c_str(), m_lpFile);	
		}
	#endif
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CFileStream::vSeek(LYNXDWORD len, int flag)
	{
		return fseek(m_lpFile, len, flag);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CFileStream::vTell()
	{
		return ftell(m_lpFile);	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CFileStream::vSize()
	{
		LYNXDWORD	Len, Offset;

		Offset = ftell(m_lpFile); 
		vSeek(0, SeekEnd);
		Len = ftell(m_lpFile); 
		vSeek(Offset, SeekSet);

		return Len;
	}
}