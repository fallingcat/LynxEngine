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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXRESOURCEMANAGER_H__
#define __LYNXRESOURCEMANAGER_H__

#include <LynxObj.h>
#include <LynxResource.h>
#include <LynxManager.h>
#include <LynxFactory.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CResourceManager : public CManager<CResource, RESOURCETYPE>
	{	
		LYNX_DECLARE_CLASS(CResourceManager);
	protected:								
	public:			
		CResourceManager(void) {};
		//CResourceManager(CEngineCore* e) {m_lpEngine = e; };
		//virtual ~CResourceManager();		
	};	
	LYNXINLINE bool operator == (CWeakPtr<CResourcePtr>& p, const CString& name) {return (p.IsValid()) ? ((*p)->GetName() == name) : false; };	
}

#endif