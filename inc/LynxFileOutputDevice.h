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

#ifndef __LYNXFILEOUTPUTDEVICE_H__
#define __LYNXFILEOUTPUTDEVICE_H__

#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CFileOutputDevice : public COutputDevice
	{	
		LYNX_DECLARE_CLASS(CFileOutputDevice);
	protected:				
		CString					m_FileName;		
		CPlatformFileStream*	m_File;
	public:		
		CFileOutputDevice();
		virtual ~CFileOutputDevice();		

		virtual void			vOpen(const CString& name);
		virtual void			vClose();
		virtual void			vClear();
		virtual void			vAppend(const CString& str);
		virtual void			vOutput(const CString& str);
	};		
}

#endif