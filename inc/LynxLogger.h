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

#ifndef __LYNXLOGGER_H__
#define __LYNXLOGGER_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CLogger
	{
		LYNX_DECLARE_CLASS(CLogger);
	public:					
		LYNXBOOL			m_bEnable;
	protected:		
		CString				m_String;
		COutputDevice*		m_lpOutputDevice;
	public:			
		CLogger();
		virtual ~CLogger();	

		virtual void		vCreate(COutputDevice* out, const CString& name);
		virtual void		vClearLog();
		virtual void		vLog(const CString& str);
		virtual void		vLog(const LYNXCHAR* str);
	private:			
	};
}

#endif
