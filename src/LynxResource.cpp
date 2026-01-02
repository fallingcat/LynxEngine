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
#include <LynxResource.h>
#include <LynxEngine.h>
#include <LynxLogger.h>

namespace LynxEngine 
{	
	int CResource::m_CreatedCounter = 0;	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CResource::CResource(CEngine* lpeng)	
	: CObj(lpeng)
	{
		LYNXCHAR Name[128];

		m_ResourceType = NUM_RESOURCETYPE;
		m_ResourceFlag = USER_DEFINED;
		m_SourceType = ORIGIN;		

		lynxSprintf(Name, _T("Resource_%d"), m_CreatedCounter);		
		m_ID = m_CreatedCounter;

		//if (m_ID == 182)
		//	DebugBreak();

		m_CreatedCounter++;
		SetName(Name);

		m_SystemMemoryUsage = 0;
		m_VideoMemoryUsage = 0;

		//m_lpEngine->GetResourceManager().Add(this);		
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CResource::~CResource(void)
	{			
		CString Log;

		//m_lpEngine->DestroyResource(GetName());
		/*
		Log.resize(1024);
		Log = GetClassName()+CString(_T("::"))+GetEnumStr(m_ResourceType)+CString(_T("("))+GetName()+CString(_T(") released\r\n"));
		LYNX_LOG(glpLogger, Log);
		*/
		//LYNX_LOG(glpLogger, GetClassName()+CString(_T("::"))+GetEnumStr(m_ResourceType)+CString(_T("("))+GetName()+CString(_T(") released\r\n")));
		//LYNX_LOG(glpLogger, GetEnumStr(m_ResourceType)+CString(_T("("))+GetName()+CString(_T(") released\r\n")));

		m_SystemMemoryUsage = 0;
		m_VideoMemoryUsage = 0;
	}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CResource::vDispose(CResourcePtr& ptr, DWORD count)
	//{			
	//	//char Count[32];
	//	//LYNX_LOG(glpLogger, GetClassName()+CString(_T("::Dispose("))+GetName()+CString(_T(") : "))+CString(_T(itoa(count, Count, 10)))+CString(_T("\r\n")));
	//	if (count == 1)
	//	{
	//		if (m_ResourceType != BUFFER)
	//			//m_lpEngine->DestroyResource(GetName());
	//			m_lpEngine->DestroyResource(ptr);
	//	}
	//}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CResource::vDispose(DWORD count)
	{			
		char Count[32];
		CString Log;

		/*
		Log.resize(1024);
		Log = CString(GetClassName())+CString(_T("::Dispose("))+GetName()+CString(_T(") : "))+CString((itoa(count, Count, 10)))+CString(_T("\r\n"));
		LYNX_LOG(glpLogger, Log);
		*/

		//LYNX_LOG(glpLogger, GetClassName()+CString(_T("::Dispose("))+GetName()+CString(_T(") : "))+CString(_T(itoa(count, Count, 10)))+CString(_T("\r\n")));
		//LYNX_LOG(glpLogger, CString(_T("Dispose("))+GetName()+CString(_T(") : "))+CString(_T(itoa(count, Count, 10)))+CString(_T("\r\n")));
		
		//if (count == 1)
		{
			//if (m_lpEngine)
			//	m_lpEngine->DestroyResource(GetName());
			//m_lpEngine->DestroyResource(ptr);
		}		
	}
}