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

#ifndef __LYNXOGLESRASTERIZERSTATE_H__
#define __LYNXOGLESRASTERIZERSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRasterizerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESRasterizerState : public CRasterizerState
		{
			LYNX_DECLARE_CLASS(COGLESRasterizerState);
		public:			
		protected:			
		public:						
			COGLESRasterizerState(CGraphicsSystem* lpsys);
			virtual ~COGLESRasterizerState();

			virtual LYNXBOOL						vCreate(const CRenderer::RASTERIZERSTATEDESC& desc);
			virtual void							vSet();
		};			
	}		
}

#endif

#endif