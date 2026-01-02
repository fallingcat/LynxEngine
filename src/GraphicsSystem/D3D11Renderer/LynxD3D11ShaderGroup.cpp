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
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexLayout.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11PixelShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderGroup::CD3D11ShaderGroup(CGraphicsSystem *pgs)
		: CShaderGroup(pgs)
		{		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderGroup::~CD3D11ShaderGroup(void)
		{
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11ShaderGroup::vCreate(void)
		{			
			m_lpVertexShader->vCreateConstantAndResource();
			m_lpPixelShader->vCreateConstantAndResource();
			
			CShaderGroup::vCreate();

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderGroup::vSet(CContainer* lpo, CCameraContainer* cam)
		{
			m_lpGraphicsSystem->GetlpRenderer()->SetVertexLayout(m_lpVertexLayout);
			m_lpVertexShader->vSet(lpo, cam);	
			m_lpPixelShader->vSet(lpo, cam);
		}
	}
}

#endif