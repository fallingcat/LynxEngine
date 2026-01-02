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

#ifndef __LYNXD3D11STENCILSTATE_H__
#define __LYNXD3D11STENCILSTATE_H__

#include <GraphicsSystem/LynxStencilState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11StencilState : public CStencilState
		{
			LYNX_DECLARE_CLASS(CD3D11StencilState);
		public:						
		protected:			
		public:						
			CD3D11StencilState(CGraphicsSystem* pgs):CStencilState(pgs) {};
			virtual ~CD3D11StencilState() {};

			virtual void					vCreate(const CRenderer::STENCILSTATEDESC& desc) {CStencilState::vCreate(desc); };
			virtual void					vSet() {};
		};
	}		
}

#endif

#endif