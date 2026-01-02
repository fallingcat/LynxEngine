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

#ifndef __LYNXIOSENGINE_H__
#define __LYNXIOSENGINE_H__

#include <LynxEngine.h>

namespace LynxEngine 
{
	namespace iOS
	{
		class LYNXENGCLASS CEngine : public LynxEngine::CEngine
		{	
			LYNX_DECLARE_CLASS(CEngine);
		public:					
		protected:				
			
			
			virtual void									vInitSystemFont();
		public:		
			CEngine();
			CEngine(GameFramework::CKernel* lpk);
			virtual ~CEngine();		
			
			virtual LYNXBOOL								vCreate(GameFramework::CKernel::INIT_DESC* desc = NULL);
			virtual LYNXBOOL								vCreateSubSystems(GameFramework::CKernel::INIT_DESC* desc = NULL);

			virtual LYNXBOOL								vInit(LYNXWINDOW &win, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);			
			virtual LYNXBOOL								vInit(LYNXWINDOW &win, GraphicsSystem::CRenderer::INITGRAPHICPARAM* lpcp);				
			virtual LYNXBOOL								vInit(LYNXWINDOW &win, INIT_DESC& desc);						
			
			virtual void									vRegisterDefaultResources();
			virtual void									vRegisterDefaultContainers();	
			virtual void									vRegisterDefaultVertexLayouts();
			virtual void									vRegisterDefaultShaderClasses();
			
			// Video
			virtual CVideo*									vCreateVideo(const LynxEngine::CCreationDesc* desc);
			virtual void									vReleaseVideo(const LynxEngine::CVideo* video);
		protected:
			//void											LoadConfig();
		};
	}		
}

#endif