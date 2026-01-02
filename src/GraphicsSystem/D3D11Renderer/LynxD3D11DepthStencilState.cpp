//##########################################################################
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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11DepthStencilState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11DepthStencilState::CD3D11DepthStencilState(CGraphicsSystem* pgs)
		: CDepthStencilState(pgs)
		{	
			m_pDSState = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11DepthStencilState::~CD3D11DepthStencilState(void)
		{
			if (m_pDSState)
			{
				m_pDSState->Release();
				m_pDSState = NULL;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11DepthStencilState::vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			CDepthStencilState::vCreate(desc);

			D3D11_DEPTH_STENCIL_DESC Desc;

			if (m_Desc.DepthMode == CRenderer::DB_NONE)
				Desc.DepthEnable				= LYNX_FALSE;
			else
				Desc.DepthEnable				= LYNX_TRUE;
			if (m_Desc.DepthMode == CRenderer::DB_C || m_Desc.DepthMode == CRenderer::DB_NONE)
				Desc.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ZERO;
			else
				Desc.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ALL;

			Desc.DepthFunc						= CD3D11Renderer::TranslateTestFunction(m_Desc.DepthFunc);
			Desc.StencilEnable					= m_Desc.StencilEnable;
			Desc.StencilReadMask				= m_Desc.StencilReadMask;
			Desc.StencilWriteMask				= m_Desc.StencilWriteMask;
			Desc.FrontFace.StencilFailOp		= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilFailOp);
			Desc.FrontFace.StencilDepthFailOp	= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilDepthFailOp);
			Desc.FrontFace.StencilPassOp		= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilPassOp);
			Desc.FrontFace.StencilFunc			= CD3D11Renderer::TranslateTestFunction(m_Desc.StencilFunc);
			Desc.BackFace.StencilFailOp			= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilFailOp);
			Desc.BackFace.StencilDepthFailOp	= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilDepthFailOp);
			Desc.BackFace.StencilPassOp			= CD3D11Renderer::TranslateStencilOP(m_Desc.StencilPassOp);
			Desc.BackFace.StencilFunc			= CD3D11Renderer::TranslateTestFunction(m_Desc.StencilFunc);

			HRESULT hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateDepthStencilState(&Desc, &m_pDSState);
			if (hResult != S_OK)
				return LYNX_FALSE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11DepthStencilState::vSet()
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->OMSetDepthStencilState(m_pDSState, m_StencilRef);
		}
	}
}

#endif