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
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11RasterizerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11RasterizerState::CD3D11RasterizerState(CGraphicsSystem *pgs)
		: CRasterizerState(pgs)
		{	
			m_pRasterizerState = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11RasterizerState::~CD3D11RasterizerState(void)
		{
			if (m_pRasterizerState)
			{
				m_pRasterizerState->Release();
				m_pRasterizerState = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11RasterizerState::vCreate(const CRenderer::RASTERIZERSTATEDESC& desc) 
		{
			CRasterizerState::vCreate(desc); 

			D3D11_RASTERIZER_DESC Desc;

			Desc.FillMode				= CD3D11Renderer::TranslateFillMode(m_Desc.FillMode);
			Desc.CullMode				= CD3D11Renderer::TranslateCullMode(m_Desc.CullMode);
			Desc.FrontCounterClockwise	= FALSE;
			Desc.DepthBias				= 0;
			Desc.DepthBiasClamp			= 0.0f;
			Desc.SlopeScaledDepthBias	= 0.0f;
			Desc.DepthClipEnable		= TRUE;
			Desc.ScissorEnable			= FALSE;
			Desc.MultisampleEnable		= FALSE;
			Desc.AntialiasedLineEnable	= FALSE;

			HRESULT hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateRasterizerState(&Desc, &m_pRasterizerState);			
			if (hResult != S_OK)
				return LYNX_FALSE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11RasterizerState::vSet()
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->RSSetState(m_pRasterizerState);
		}
	}
}

#endif