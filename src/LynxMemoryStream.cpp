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
#include <LynxMemoryStream.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CMemoryStream::CMemoryStream(void)	
	{
		m_StreamType = MEMORY_STREAM;
		m_lpBuffer = NULL;
		m_CurrentOffset = 0;
		m_Length = 0;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CMemoryStream::~CMemoryStream(void)
	{
		vClose();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CMemoryStream::Alloc(LYNXDWORD size)
	{
		if (m_lpBuffer)
		{
			LYNXFREE(m_lpBuffer);
			m_lpBuffer = NULL;
		}
		m_Length = size;
		m_lpBuffer = LYNXALLOC(size);
		m_CurrentOffset = 0;
		m_bValid = LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CMemoryStream::GetCurrentPtr()
	{
		void* lpSrc = (LYNXBYTE*)m_lpBuffer + m_CurrentOffset;
		return lpSrc;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CMemoryStream::vOpen(const CString& name, int flag) 
	{
		CPlatformFileStream FileStream;

		LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open "))+name+CString(_T("...\r\n")));

		m_bValid = LYNX_FALSE;
		if (!FileStream.vOpen(name, flag))
			return LYNX_FALSE;

		m_Length = FileStream.vSize();
		Alloc(m_Length);		
		FileStream.vRead(m_lpBuffer, 1, m_Length);
		FileStream.vClose();

		m_bValid = LYNX_TRUE;

		return TRUE;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CMemoryStream::vClose() 
	{
		if (m_lpBuffer)
		{
			LYNXFREE(m_lpBuffer);
			m_lpBuffer = NULL;
			m_CurrentOffset = 0;
			m_Length = 0;
		}
		m_bValid = LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CMemoryStream::vRead(void *lpdst, int size, int len) 
	{
		DWORD Len = size*len;

		if (Len <= 0)
			return 0;

		if (m_CurrentOffset > m_Length)
			return 0;

		if ((m_CurrentOffset + Len) > m_Length)
		{
			Len = m_Length - m_CurrentOffset;
		}
		
		void* lpSrc = (LYNXBYTE*)m_lpBuffer + m_CurrentOffset;
		lynxMemCpy(lpdst, lpSrc, Len);
		m_CurrentOffset += Len;

		return Len;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CMemoryStream::vRead(CString& str) 
	{
		LYNXANSICHAR Buffer[2048];
		int	Index = 0;		

		while (Index < 2047)
		{
			vRead(&Buffer[Index], sizeof(LYNXANSICHAR), 1);
			if (Buffer[Index] == 0x0d)
			{
				Buffer[Index] = 0x00;
				Index++;
				vRead(&Buffer[Index], sizeof(LYNXANSICHAR), 1);
				if (Buffer[Index] == 0x0a)
					break;
			}
			Index++;
		}		

		str = Buffer;
		
		return Index;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CMemoryStream::vWrite(const void *lpsrc, int size, int len)
	{
		DWORD Len = size*len;

		if (Len <= 0)
			return 0;

		if (m_CurrentOffset > m_Length)
			return 0;

		if ((m_CurrentOffset + Len) > m_Length)
		{
			Len = m_Length - m_CurrentOffset;
		}
		
		void* lpDst = (LYNXBYTE*)m_lpBuffer + m_CurrentOffset;
		lynxMemCpy(lpDst, lpsrc, Len);
		m_CurrentOffset += Len;

		return Len;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CMemoryStream::vWrite(const CString& str)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CMemoryStream::vSeek(LYNXDWORD len, int flag)
	{
		DWORD Len = len;

		switch (flag)
		{
			case CStream::SeekCur:
				break;

			case CStream::SeekSet:
				m_CurrentOffset = 0;
				break;

			case CStream::SeekEnd:
				m_CurrentOffset = m_Length;				
				break;
		}
		if (m_CurrentOffset > m_Length || m_CurrentOffset < 0)
			return -1;

		if ((m_CurrentOffset + Len) > m_Length)
			return -1;

		if ((m_CurrentOffset + Len) < 0)
			return -1;

		m_CurrentOffset += Len;

		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CMemoryStream::vTell()
	{
		return m_CurrentOffset;	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXDWORD CMemoryStream::vSize()
	{
		return m_Length;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CMemoryStream::Set(void* buffer, LYNXDWORD size)
	{
		m_lpBuffer = buffer;
		m_Length = size;
		m_CurrentOffset = 0;
		m_bValid = LYNX_TRUE;
	}
}