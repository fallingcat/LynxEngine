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

#ifndef __LYNXDECALCONTAINER_H__
#define __LYNXDECALCONTAINER_H__

#include <LynxEngineClasses.h>
#include <LynxArray.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	/**	
	*  @brief Decal container class。
	* 
	*  This class presents the decal in the scene. This class renders its proxy to mark the area in stencil buffer where
	*  the pixels are covered by decal.
	*	
	*/
	class LYNXENGCLASS CDecalContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CDecalContainer);
	public:		
	protected:					
	public:
		CDecalContainer();
		CDecalContainer(CEngine *lpengine);
		virtual ~CDecalContainer();			
	};
}
#endif