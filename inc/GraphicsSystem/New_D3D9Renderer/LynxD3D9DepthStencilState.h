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

#ifndef __LYNXD3D9DEPTHSTENCILSTATE_H__
#define __LYNXD3D9DEPTHSTENCILSTATE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/LynxDepthStencilState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9DepthStencilState : public CDepthStencilState
		{
			LYNX_DECLARE_CLASS(CD3D9DepthStencilState);
		public:						
		protected:			
		public:						
			CD3D9DepthStencilState(CGraphicsSystem* pgs);
			virtual ~CD3D9DepthStencilState();	

			virtual LYNXBOOL				vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc);
			virtual void					vSet();
		};
	}		
}

#endif