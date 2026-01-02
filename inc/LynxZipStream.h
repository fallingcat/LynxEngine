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

#ifndef __LYNXZIPSTREAM_H__
#define __LYNXZIPSTREAM_H__

#include <LynxEngineClasses.h>
#include <LynxType.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base stream object class of the entire engine。
	* 
	*  The basic stream object class of the entire engine. 
	*	
	*/
	class LYNXENGCLASS CZipStream : public CStream
	{	
		LYNX_DECLARE_CLASS(CZipStream);
	public:		
		static zlib_filefunc_def    m_ZLibFileFuncDef;
		int							m_TotalFiles;
		int							m_CurrentExtractedFiles;
		unsigned long				m_TotalSize;
		unsigned long				m_CurrentExtractedSize;
		float						m_ExtractingProgress;
	protected:					        
		CString						m_ZipFilename;
		LYNXBOOL					m_bSearch;
		unzFile						m_ZipFile;
		CMemoryStream*				m_lpMemStream;
		LYNXBOOL					m_bSearchFisrt;
		CPlatformFileStream*		m_lpFileStream;
		//CMemoryStream*				m_lpFileStream;
		CAnsiString					m_Password;	
		int							m_OpenedCount;	
		LYNXBOOL					m_bUsed;
		#ifdef _DEBUG
			CString					m_Filename;
		#endif
	public:	
		CZipStream();
		virtual ~CZipStream();

		CZipStream&					operator =(const CZipStream& z);

		LYNXFORCEINLINE void		SetPassword(const CAnsiString& pw) {m_Password = pw; }

		LYNXFORCEINLINE	int			GetOpenedCount() {return m_OpenedCount;}

		virtual LYNXBOOL			vOpen(const CString& name, int flag = Binary|Read);
		virtual void				vClose();
		virtual int					vRead(void *lpdst, int size, int len);
		virtual int					vRead(CString& str);
		virtual int					vWrite(const void *lpsrc, int size, int len);
		virtual int					vWrite(const CString& str);
		virtual int					vSeek(LYNXDWORD len, int flag);
		virtual LYNXDWORD			vTell();
		virtual LYNXDWORD			vSize();		
		#ifdef _UNICODE
			virtual int				vRead(CAnsiString& str) {return 0; };
			virtual int				vWrite(const CAnsiString& str) {return 0; };
		#endif

		void						CopyToFile(CPlatformFileStream& fs);

		LYNXBOOL					OpenZipFile(const CString& name, LYNXBOOL bsearch = LYNX_TRUE);
		//LYNXBOOL					OpenZipFile(const CString& name, void* lpd, LYNXLONG size);
		void						CloseZipFile();

		LYNXBOOL					FindFile(const CAnsiString& filename);

		static zipFile				CreateZipFile(const CAnsiString& filename);
		static void					AddFile2ZipFile(zipFile zf, const CString& filename, const CAnsiString& name, const CAnsiString& password="");		
		static void					CloseZipFile(zipFile zf);

		LYNXBOOL					ExtractAll(const CString& path);	
		
		LYNXFORCEINLINE void		Used(LYNXBOOL b) {m_bUsed = b; }	
		LYNXFORCEINLINE LYNXBOOL	IsUsed() {return m_bUsed; }	
	private:
		LYNXBOOL					ReadCurrentFile(LYNXANSICHAR* filename = NULL, int len = 0);
		LYNXBOOL					OpenPlatformFile(const CString& name, int flag);
	};		
}

#endif