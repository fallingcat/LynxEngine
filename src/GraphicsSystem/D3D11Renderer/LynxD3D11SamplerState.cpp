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
#include <GraphicsSystem/D3D11Renderer/LynxD3D11SamplerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11SamplerState::CD3D11SamplerState(CGraphicsSystem *pgs)
		: CSamplerState(pgs)
		{	
			m_pSamplerState = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11SamplerState::~CD3D11SamplerState(void)
		{
			if (m_pSamplerState)
			{
				m_pSamplerState->Release();
				m_pSamplerState = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11SamplerState::vCreate(const CRenderer::SAMPLERSTATEDESC& desc) 
		{
			CSamplerState::vCreate(desc); 

			D3D11_SAMPLER_DESC Desc;
			Desc.Filter			= CD3D11Renderer::TranslateFilter(m_Desc.Filter);
			Desc.AddressU		= CD3D11Renderer::TranslateAddressMode(m_Desc.AddressU);
			Desc.AddressV		= CD3D11Renderer::TranslateAddressMode(m_Desc.AddressV);
			Desc.AddressW		= CD3D11Renderer::TranslateAddressMode(m_Desc.AddressW);
			Desc.MipLODBias		= m_Desc.MipLODBias;
			Desc.MaxAnisotropy	= m_Desc.MaxAnisotropy;
			Desc.ComparisonFunc	= CD3D11Renderer::TranslateTestFunction(m_Desc.ComparisonFunc);
			Desc.BorderColor[0]	= m_Desc.BorderColor.Red/255.0f;
			Desc.BorderColor[1]	= m_Desc.BorderColor.Green/255.0f;
			Desc.BorderColor[2]	= m_Desc.BorderColor.Blue/255.0f;
			Desc.BorderColor[3]	= m_Desc.BorderColor.Alpha/255.0f;
			Desc.MinLOD			= m_Desc.MinLOD;
			Desc.MaxLOD			= m_Desc.MaxLOD;
			
			HRESULT hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateSamplerState(&Desc, &m_pSamplerState);
			if (hResult != S_OK)
				return LYNX_FALSE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::vSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->VSSetSamplers(channel, 1, &m_pSamplerState);
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->PSSetSamplers(channel, 1, &m_pSamplerState);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::VSSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->VSSetSamplers(channel, 1, &m_pSamplerState);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::GSSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->GSSetSamplers(channel, 1, &m_pSamplerState);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::HSSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->HSSetSamplers(channel, 1, &m_pSamplerState);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::DSSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->DSSetSamplers(channel, 1, &m_pSamplerState);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11SamplerState::PSSet(int channel)
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->PSSetSamplers(channel, 1, &m_pSamplerState);
		}
	}
}

#endif