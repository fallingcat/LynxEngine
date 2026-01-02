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

#ifndef __LYNXRESOURCE_H__
#define __LYNXRESOURCE_H__

#include <LynxType.h>
#include <LynxObj.h>
#include <LynxString.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CResource : public CObj
	{	
		LYNX_DECLARE_CLASS(CResource);
	public:
		typedef enum{
			ORIGIN,
			INSTANCE,
			NUM_SOURCETYPE,
		}SOURCETYPE;
	private:
		static int						m_CreatedCounter;
	protected:			
		RESOURCEFLAG					m_ResourceFlag;
		RESOURCETYPE					m_ResourceType;
		SOURCETYPE						m_SourceType;				

		DWORD							m_SystemMemoryUsage;
		DWORD							m_VideoMemoryUsage;
	public:	
		CResource(CEngine* lpeng);
		virtual ~CResource();											

		LYNXINLINE virtual void			vInstance(const CResource& rhs) {CObj::vInstance(rhs); m_SourceType = INSTANCE; };
		LYNXINLINE RESOURCETYPE			GetResourceType() const {return m_ResourceType; };
		LYNXINLINE void					SetResourceFlag(RESOURCEFLAG f) {m_ResourceFlag = f; };
		LYNXINLINE RESOURCEFLAG			GetResourceFlag() const {return m_ResourceFlag; };
		LYNXINLINE void					SetSourceType(SOURCETYPE st) {m_SourceType = st; };
		LYNXINLINE SOURCETYPE			GetSourceType() const {return m_SourceType; };
		CResource&						operator =(const CResource& rhs) {m_Name = rhs.m_Name; return *this; };		
		virtual CString					vGetDesc() { return m_Name; };
		//virtual void					vDispose(CResourcePtr& ptr, DWORD count);
		virtual void					vDispose(DWORD count);

		DWORD							GetSystemMemoryUsage() {return m_SystemMemoryUsage; };
		DWORD							GetVideoMemoryUsage() {return m_VideoMemoryUsage; };
	};	
	LYNXINLINE bool operator ==(CResourcePtr& rc, const LYNXCRC& crc) { return (rc.GetPtr() ? (rc->GetCRCCode() == crc) : false); };
	LYNXINLINE bool operator ==(CResourcePtr& rc, LYNXCRC& crc) { return (rc.GetPtr() ? (rc->GetCRCCode() == crc) : false); };
}

#endif