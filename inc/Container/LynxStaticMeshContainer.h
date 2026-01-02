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

#ifndef __LYNXSTATICMESHCONTAINER_H__
#define __LYNXSTATICMESHCONTAINER_H__

#include <Container/LynxGeometryContainer.h>

namespace LynxEngine 
{
	class CEngine;
	class LYNXENGCLASS CStaticMeshContainer : PUBLIC CGeometryContainer
	{	
		LYNX_DECLARE_CLASS(CStaticMeshContainer);
	protected:			
	public:		
		CStaticMeshContainer();
		CStaticMeshContainer(CEngine *lpengine);
		virtual ~CStaticMeshContainer();				
		
		virtual void									vRender(CCameraContainer* cam = NULL);				
		virtual void									vLighting(CCameraContainer* cam, SceneSystem::CLight* lit);
	};
}

#endif