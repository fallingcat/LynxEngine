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

#ifndef __LYNXCREATIONDESC_H__
#define __LYNXCREATIONDESC_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CCreationDesc);
	public:				
		CStream*				m_lpStream;
		FileSystem::CPackage*	m_lpPackage;
		long					m_Offset;	
		CString					m_Name;
		CString					m_FileName;
		LYNXBOOL				m_bCreateMaterial;	
		CString					m_MaterialName;

		CCreationDesc() {Default(); };
		void Default() { m_Name.clear(); m_FileName.clear();  m_MaterialName.clear(); m_lpStream = NULL;  m_lpPackage = NULL, m_Offset = 0; m_bCreateMaterial = LYNX_TRUE; };
	};		
}
#endif