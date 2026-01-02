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

#ifndef __LYNXANDROIDGRAPHICSSYSTEM_H__
#define __LYNXANDROIDGRAPHICSSYSTEM_H__

#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{	
		namespace Android
		{
			class LYNXENGCLASS CGraphicsSystem : public LynxEngine::GraphicsSystem::CGraphicsSystem
			{
				LYNX_DECLARE_CLASS(CGraphicsSystem);
			public:				
			protected:						
			public:			
				CGraphicsSystem(CEngine* const lpengine, CRenderer* ren = NULL);
				virtual ~CGraphicsSystem();		

				//DECLARE_CLASSSCRIPT
				void						vLoadConfig(const LYNXCHAR* filename);
				LYNXBOOL					vCreate(CRenderer::RENDERERTYPE rt);
				LYNXBOOL					vInit() {return LYNX_FALSE; };
				LYNXBOOL					vInit(LYNXWINDOW wnd, CRenderer::INITGRAPHICPARAM* lpcp);
				LYNXBOOL					vInit(LYNXWINDOW wnd, const int w = 0, const int h = 0);
				LYNXBOOL					vResizeMaps(int cx, int cy);
				LYNXBOOL					vInitDefalutResources(void);
				
			friend class CEngine;
			};
		}
	}				
}

#endif