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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//--------------------------------------------------------------------------
//  
//
//###########################################################################

#ifndef __LYNXSYSTEM_H__
#define __LYNXSYSTEM_H__

#include <LynxObj.h>
#include <LynxArray.h>
#include <LynxList.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CSystem : public CObj
	{	
		LYNX_DECLARE_CLASS(CSystem);
	protected:	
	public:		
		CSystem(CEngine* const lpengine);
		virtual ~CSystem();

		DECLARE_CLASSSCRIPT
		
		virtual LYNXBOOL						vCreate(void) = 0;	
		virtual LYNXBOOL						vInit();		
		virtual void							vLoop(float step);
		virtual LYNXBOOL						vDestroy();
	};			
}

#endif