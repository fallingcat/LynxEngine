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

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9RasterizerState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9RasterizerState::CD3D9RasterizerState(CGraphicsSystem* pgs)
		: CRasterizerState(pgs)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9RasterizerState::~CD3D9RasterizerState(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9RasterizerState::vCreate(const CRenderer::RASTERIZERSTATEDESC& desc)
		{
			CRasterizerState::vCreate(desc);
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9RasterizerState::vSet()
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			lpR->GetlpD3DDevice()->SetRenderState(D3DRS_FILLMODE, CD3D9Renderer::TranslateFillMode(m_Desc.FillMode));
			lpR->GetlpD3DDevice()->SetRenderState(D3DRS_CULLMODE, CD3D9Renderer::TranslateCullMode(m_Desc.CullMode));
		}
	}
}

#endif