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
#include <GraphicsSystem/D3D11Renderer/LynxD3D11BlendState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11BlendState::CD3D11BlendState(CGraphicsSystem *pgs)
		: CBlendState(pgs)
		{	
			m_pBlendState = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11BlendState::~CD3D11BlendState(void)
		{
			if (m_pBlendState)
			{
				m_pBlendState->Release();
				m_pBlendState = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11BlendState::vCreate(const CRenderer::BLENDSTATEDESC& desc) 
		{
			CBlendState::vCreate(desc); 

			D3D11_BLEND_DESC Desc;

			Desc.AlphaToCoverageEnable	= m_Desc.bAlphaToCoverage;
			Desc.IndependentBlendEnable	= m_Desc.bIndependentBlend;
			for (int i=0; i<8; i++)
			{
				Desc.RenderTarget[i].BlendEnable			= m_Desc.RTBlendParam[i].bBlend;
				Desc.RenderTarget[i].SrcBlend				= CD3D11Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[i].SrcBlend);
				Desc.RenderTarget[i].DestBlend				= CD3D11Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[i].DestBlend);
				Desc.RenderTarget[i].BlendOp				= CD3D11Renderer::TranslateBlendOP(m_Desc.RTBlendParam[i].BlendOp);
				Desc.RenderTarget[i].SrcBlendAlpha			= CD3D11Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[i].SrcBlendAlpha);
				Desc.RenderTarget[i].DestBlendAlpha			= CD3D11Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[i].SrcBlendAlpha);
				Desc.RenderTarget[i].BlendOpAlpha			= CD3D11Renderer::TranslateBlendOP(m_Desc.RTBlendParam[i].BlendOpAlpha);
				Desc.RenderTarget[i].RenderTargetWriteMask	= m_Desc.RTBlendParam[i].RTWriteMask;
			}

			HRESULT hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateBlendState(&Desc, &m_pBlendState);
			if (hResult != S_OK)
				return LYNX_FALSE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11BlendState::vSet()
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->OMSetBlendState(m_pBlendState, NULL, 0xffffffff);
		}
	}
}

#endif