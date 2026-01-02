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
#include <FileSystem/LynxPackage.h>

namespace LynxEngine 
{	
	namespace FileSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPackage::CPackage(CFileSystem* const lpsys)
		: CObj(lpsys->GetlpEngine())
		{				
			m_lpFileSystem = lpsys;
			m_bVaild = LYNX_FALSE;
			m_ZipStreamList.resize(32);
			for (int i=0; i<m_ZipStreamList.size(); i++)
			{
				//m_ZipStreamList[i] = LYNXNEW CZipStream;
				//m_ZipStreamList[i] = LYNXNEW CPlatformFileStream;
				m_ZipStreamList[i] = LYNXNEW CMemoryStream;				
			}

			m_PackFile = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPackage::~CPackage(void)
		{
			CArray<CStream*>::CIterator Stream;
			
			for (Stream = m_ZipStreamList.begin(); Stream != m_ZipStreamList.end(); ++Stream)
			{
				(*Stream)->vClose();
				//((CZipStream*)(*Stream))->CloseZipFile();
				LYNXDEL (*Stream);
			}
			m_ZipStreamList.clear();	

			if (m_PackFile)
			{
                m_PackFile->vClose();
				LYNXDEL (m_PackFile);
				m_PackFile = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPackage::Open(const CString& name)
		{
			if (name != m_Name)
			{
				SetName(name);

				m_bVaild = LYNX_TRUE;

				CArray<CStream*>::CIterator Stream;

				for (Stream = m_ZipStreamList.begin(); Stream != m_ZipStreamList.end(); ++Stream)
				{
					(*Stream)->vClose();
					//((CZipStream*)(*Stream))->CloseZipFile();	
					//((CZipStream*)(*Stream))->Used(LYNX_FALSE);
				}								

				CPlatformFileStream File;
				CString Line, Name;
				PACKDATA Data;

				if (File.vOpen(name + CString(_T(".meta")), CStream::Text | CStream::Read))
				{
					while (File.vRead(Line))
					{
						CString::size_type Start = 0;
						CString::size_type End = Line.find_first_of(',');
						CString Token = Line.substr(Start, End);
						Name = Token;
						Name.ReplaceBackslashWithSlash();

						Start = End + 1;
						End = Line.find_first_of(',', Start);
						Token = Line.substr(Start, End);
						m_PackMeta[Name].Size = lynxAtoul(Token.c_str());

						Start = End + 1;
						End = Line.find_first_of(',', Start);
						Token = Line.substr(Start, End);
						m_PackMeta[Name].Offset = lynxAtoul(Token.c_str());
					}
				}				

				m_PackFile = LYNXNEW CMemoryStream;
				m_PackFile->vOpen(name, CStream::Binary | CStream::Read);
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPackage::OpenFile(CStream* stream, const CString& name, const int flag)
		{
			CPlatformFileStream File;
			CMemoryStream* Stream = static_cast<CMemoryStream*>(stream);

			CString Name = name;
			Name.ReplaceBackslashWithSlash();
			if (m_PackMeta.is_exist(Name))
			{
				Stream->Alloc(m_PackMeta[Name].Size);
				m_PackFile->vSeek(m_PackMeta[Name].Offset, CStream::SeekSet);
				m_PackFile->vRead(Stream->GetCurrentPtr(), 1, m_PackMeta[Name].Size);				
				return LYNX_TRUE;
			}
			 else
			{
				return (Stream->vOpen(name, flag));

			}
			return LYNX_FALSE;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPackage::Close()
		{
			CArray<CStream*>::CIterator Stream;
			
			for (Stream = m_ZipStreamList.begin(); Stream != m_ZipStreamList.end(); ++Stream)
			{
				(*Stream)->vClose();
				//((CZipStream*)(*Stream))->CloseZipFile();	
				//((CZipStream*)(*Stream))->Used(LYNX_FALSE);
			}			
			m_bVaild = LYNX_FALSE;

			/*
			if (m_PackageZipStream)
			{
				m_PackageZipStream->CloseZipFile();
				LYNXDEL(m_PackageZipStream);
				m_PackageZipStream = NULL;
			}
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStream* CPackage::GetStream() 
		{
			for (int i=0; i<m_ZipStreamList.size(); i++)
			{
				//if (((CZipStream*)m_ZipStreamList[i])->GetOpenedCount() <= 0 && !m_ZipStreamList[i]->IsValid() && !((CZipStream*)m_ZipStreamList[i])->IsUsed())
				if (!m_ZipStreamList[i]->IsValid())
				{
					//((CZipStream*)m_ZipStreamList[i])->OpenZipFile(m_Name, LYNX_FALSE);//LYNX_TRUE);
					//((CZipStream*)m_ZipStreamList[i])->Used(LYNX_TRUE);
					//((CZipStream*)m_ZipStreamList[i])->SetPassword("Oh!SpaceQube");
					return m_ZipStreamList[i];
				}
			}
			//CZipStream* lpZipStream = LYNXNEW CZipStream;
			//CStream* lpZipStream = LYNXNEW CPlatformFileStream;
			CStream* lpZipStream = LYNXNEW CMemoryStream;
			m_ZipStreamList.push_back(lpZipStream);
			//lpZipStream->OpenZipFile(m_Name, LYNX_FALSE);//LYNX_TRUE);
			//lpZipStream->Used(LYNX_TRUE);
			//lpZipStream->SetPassword("Oh!SpaceQube");			
			
			return static_cast<CStream*>(lpZipStream); 			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPackage::DiscardStream(CStream* fs) 
		{		
			//fs->vClose();
			/*
			if (fs->GetStreamType() == CStream::ZIP_STREAM && ((CZipStream*)fs)->IsUsed())
			{
				((CZipStream*)fs)->vClose();
				((CZipStream*)fs)->CloseZipFile();
				((CZipStream*)fs)->Used(LYNX_FALSE);
			}
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPackage::ClearInvalidStream() 
		{
			int RemovedStreams = 0;

			/*
			CArray<CStream*>::CIterator Stream;		

			Stream = m_ZipStreamList.begin();
			while (Stream != m_ZipStreamList.end())
			{	
				if (((CZipStream*)(*Stream))->IsUsed())
				{
					if (!(*Stream)->IsValid() && ((CZipStream*)(*Stream))->GetOpenedCount() <= 0)
					{
						((CZipStream*)(*Stream))->Used(LYNX_FALSE);
						RemovedStreams++;
					}					
				}
				++Stream;						
			}
			
			LYNXCHAR Buffer[256];
			lynxSprintf(Buffer, _T("::Remove (%d/%d) invalid streams\r\n"), RemovedStreams, m_ZipStreamList.size());		

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));			
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPackage::ClearClosedStream() 
		{
			return;

			/*
			int OriginalNumStreams = m_ZipStreamList.size();

			CList<CStream*>::CIterator Stream;		

			Stream = m_ZipStreamList.begin();
			while (Stream != m_ZipStreamList.end())
			{	
				if ((*Stream)->IsClosed())
				{
					CList<CStream*>::CIterator Cur = Stream;		
					++Stream;		
					LYNXDEL (*Cur);
					m_ZipStreamList.remove(*Cur);
				}
				else
				{
					++Stream;		
				}
			}
			
			LYNXCHAR Buffer[256];
			lynxSprintf(Buffer, _T("::Remove closed stream(%d -> %d)\r\n"), OriginalNumStreams, m_ZipStreamList.size());		

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));
			*/
		}
	}
}
