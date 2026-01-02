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

#ifndef __LYNXOGLESDEPTHSTENCILSTATE_H__
#define __LYNXOGLESDEPTHSTENCILSTATE_H__

#include <GraphicsSystem/LynxDepthStencilState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESDepthStencilState : public CDepthStencilState
		{
			LYNX_DECLARE_CLASS(COGLESDepthStencilState);
		public:						
		protected:			
		public:						
			COGLESDepthStencilState(CGraphicsSystem* pgs);
			virtual ~COGLESDepthStencilState();

			virtual LYNXBOOL				vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc);
			virtual void					vSet();
		};
	}		
}

#endif

#endif