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

#ifndef __LYNXSKYLIGHTMAP_H__
#define __LYNXSKYLIGHTMAP_H__

#include <GraphicsSystem/LynxMap.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CSkyLightMap : public GraphicsSystem::CMap
	{	
		LYNX_DECLARE_CLASS(CSkyLightMap);
	public:											
	protected:					
	public:			
		CSkyLightMap();
		CSkyLightMap(GraphicsSystem::CGraphicsSystem* lpgs);
		virtual ~CSkyLightMap();		

		LYNXBOOL								Create(int mipmaplevel, const LYNXCHAR *name, int w, int h, GraphicsSystem::CRenderer::FORMAT format);
	};		
}
#endif