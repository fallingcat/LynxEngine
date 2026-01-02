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

#ifndef __LYNXIOSGRAPHICSSYSTEM_H__
#define __LYNXIOSGRAPHICSSYSTEM_H__

#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{	
		namespace iOS
		{
			class LYNXENGCLASS CGraphicsSystem : public LynxEngine::GraphicsSystem::CGraphicsSystem
			{
				LYNX_DECLARE_CLASS(CGraphicsSystem);
			public:				
			protected:		
				LYNXBOOL					InitDefalutResources(void);
			public:			
				CGraphicsSystem(CEngine* const lpengine, CRenderer* ren = NULL);
				virtual ~CGraphicsSystem();		

				//DECLARE_SCRIPTCLASS_HIERARCHY
				void						vLoadConfig(const LYNXCHAR* filename);
				LYNXBOOL					vCreate(CRenderer::RENDERERTYPE rt);
				LYNXBOOL					vInit() {return LYNX_FALSE; };
				LYNXBOOL					vInit(LYNXWINDOW wnd, CRenderer::INITGRAPHICPARAM* lpcp);
				LYNXBOOL					vInit(LYNXWINDOW wnd);
				LYNXBOOL					vResizeMaps(int cx, int cy);
				
			friend class CEngine;
			};
		}
	}				
}

#endif