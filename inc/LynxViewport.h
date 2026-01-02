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

#ifndef __LYNXVIEWPORT_H__
#define __LYNXVIEWPORT_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	class CEngine;
	class LYNXENGCLASS CViewport : public CObj
	{	
		LYNX_DECLARE_CLASS(CViewport);
	protected:								
	public:		
		CViewport();
		CViewport(CEngine* lpengine);
		virtual ~CViewport();		
	};	
}
#endif