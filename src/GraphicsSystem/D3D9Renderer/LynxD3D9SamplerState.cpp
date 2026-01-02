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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9SamplerState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9SamplerState::CD3D9SamplerState(CGraphicsSystem* pgs)
		: CSamplerState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9SamplerState::~CD3D9SamplerState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9SamplerState::vCreate(const CRenderer::SAMPLERSTATEDESC& desc)
		{
			CSamplerState::vCreate(desc);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9SamplerState::vSet(int channel)
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_ADDRESSU, CD3D9Renderer::TranslateAddressMode(m_Desc.AddressU));				
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_ADDRESSV, CD3D9Renderer::TranslateAddressMode(m_Desc.AddressV));				
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_ADDRESSW, CD3D9Renderer::TranslateAddressMode(m_Desc.AddressW));		

			switch (m_Desc.Filter)
			{
				default:
				case CRenderer::FM_MIN_MAG_MIP_POINT:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_MIN_MAG_POINT_MIP_LINEAR:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_MIN_POINT_MAG_LINEAR_MIP_POINT:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_MIN_POINT_MAG_MIP_LINEAR:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_MIN_LINEAR_MAG_MIP_POINT:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_MIN_MAG_LINEAR_MIP_POINT:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_MIN_MAG_MIP_LINEAR:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_ANISOTROPIC:
					m_MinFilter = D3DTEXF_ANISOTROPIC;
					m_MagFilter = D3DTEXF_ANISOTROPIC;
					m_MipFilter = D3DTEXF_ANISOTROPIC;
					break;
				case CRenderer::FM_COMPARISON_MIN_MAG_MIP_POINT:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
					m_MinFilter = D3DTEXF_POINT;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_POINT;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_POINT;
					break;
				case CRenderer::FM_COMPARISON_MIN_MAG_MIP_LINEAR:
					m_MinFilter = D3DTEXF_LINEAR;
					m_MagFilter = D3DTEXF_LINEAR;
					m_MipFilter = D3DTEXF_LINEAR;
					break;
				case CRenderer::FM_COMPARISON_ANISOTROPIC:
					m_MinFilter = D3DTEXF_ANISOTROPIC;
					m_MagFilter = D3DTEXF_ANISOTROPIC;
					m_MipFilter = D3DTEXF_ANISOTROPIC;
					break;
			}
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_MAGFILTER, m_MinFilter);	
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_MINFILTER, m_MagFilter);	
			lpR->GetlpD3DDevice()->SetSamplerState(channel, D3DSAMP_MIPFILTER, m_MipFilter);	
		}
	}
}

#endif