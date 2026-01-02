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

#ifndef __LYNXCUBESHADOWMAP_H__
#define __LYNXCUBESHADOWMAP_H__

#include <GraphicsSystem/LynxShadowMap.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CCubeShadowMap : public CShadowMap
		{	
			LYNX_DECLARE_CLASS(CCubeShadowMap);
		public:								
		protected:					
		public:			
			CCubeShadowMap();
			CCubeShadowMap(CGraphicsSystem* lpgs);
			virtual ~CCubeShadowMap();		
					
			LYNXBOOL								Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format);						
		};	
	}
}
#endif