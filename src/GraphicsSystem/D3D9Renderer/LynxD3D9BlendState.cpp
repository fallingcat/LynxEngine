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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9BlendState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9BlendState::CD3D9BlendState(CGraphicsSystem* pgs)
		: CBlendState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9BlendState::~CD3D9BlendState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9BlendState::vCreate(const CRenderer::BLENDSTATEDESC& desc)
		{
			CBlendState::vCreate(desc);
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9BlendState::vSet()
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			
			//if (m_Desc.RTBlendParam[0].bBlend)
			{
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, m_Desc.RTBlendParam[0].bBlend);
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_SRCBLEND, CD3D9Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[0].SrcBlend));
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_DESTBLEND, CD3D9Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[0].DestBlend));			
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_BLENDOP, CD3D9Renderer::TranslateBlendOP(m_Desc.RTBlendParam[0].BlendOp));
				lpR->SetColorWriteMask(m_Desc.RTBlendParam[0].RTWriteMask);			
			}

			//lpR->GetlpD3DDevice()->SetRenderState(D3DRS_SRCBLENDALPHA, CD3D9Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[0].SrcBlendAlpha));
			//lpR->GetlpD3DDevice()->SetRenderState(D3DRS_DESTBLENDALPHA, CD3D9Renderer::TranslateBlendFunction(m_Desc.RTBlendParam[0].DestBlendAlpha));
			//lpR->GetlpD3DDevice()->SetRenderState(D3DRS_BLENDOPALPHA, CD3D9Renderer::TranslateBlendOP(m_Desc.RTBlendParam[0].BlendOpAlpha));
		}
	}
}

#endif