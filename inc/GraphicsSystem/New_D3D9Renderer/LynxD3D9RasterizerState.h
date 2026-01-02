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

#ifndef __LYNXD3D9RASTERIZERSTATE_H__
#define __LYNXD3D9RASTERIZERSTATE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <LynxResource.h>
#include <GraphicsSystem/LynxRasterizerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9RasterizerState : public CRasterizerState
		{
			LYNX_DECLARE_CLASS(CD3D9RasterizerState);
		public:			
		protected:			
		public:						
			CD3D9RasterizerState(CGraphicsSystem* lpsys);
			virtual ~CD3D9RasterizerState();	

			virtual LYNXBOOL						vCreate(const CRenderer::RASTERIZERSTATEDESC& desc);
			virtual void							vSet();
		};			
	}		
}

#endif