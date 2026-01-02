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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9DepthStencilState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9DepthStencilState::CD3D9DepthStencilState(CGraphicsSystem* pgs)
		: CDepthStencilState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9DepthStencilState::~CD3D9DepthStencilState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9DepthStencilState::vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			return CDepthStencilState::vCreate(desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9DepthStencilState::vSet()
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			switch (m_Desc.DepthMode)
			{
				case CRenderer::DB_NONE:
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
					break;
				case CRenderer::DB_ALWAYS:
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
					break;
				case CRenderer::DB_C:
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
					break;
				case CRenderer::DB_CW:
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
					break;
				case CRenderer::DB_W:
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
					lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
					break;
			}
			lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZFUNC, CD3D9Renderer::TranslateTestFunction(m_Desc.DepthFunc));
			//lpR->GetlpD3DDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);

			lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILENABLE , m_Desc.StencilEnable);
			if (m_Desc.StencilEnable)
			{
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILFUNC, CD3D9Renderer::TranslateTestFunction(m_Desc.StencilFunc));
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILREF, m_Desc.StencilRef);			
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILMASK, m_Desc.StencilReadMask);
				//lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILWRITEMASK, m_Desc.StencilWriteMask);

				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILFAIL, CD3D9Renderer::TranslateStencilOP(m_Desc.StencilFailOp));			
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILZFAIL, CD3D9Renderer::TranslateStencilOP(m_Desc.StencilDepthFailOp));			
				lpR->GetlpD3DDevice()->SetRenderState(D3DRS_STENCILPASS, CD3D9Renderer::TranslateStencilOP(m_Desc.StencilPassOp));			
			}			
		}
	}
}

#endif