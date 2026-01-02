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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexLayout.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9PixelShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderGroup::CD3D9ShaderGroup(CGraphicsSystem *pgs)
		: CShaderGroup(pgs)
		{		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderGroup::~CD3D9ShaderGroup(void)
		{
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9ShaderGroup::vCreate(void)
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
		void CD3D9ShaderGroup::vSet(CContainer* lpo, CCameraContainer* cam)
		{
			m_lpGraphicsSystem->GetlpRenderer()->SetVertexLayout(m_lpVertexLayout);
			m_lpVertexShader->vSet(lpo, cam);	
			m_lpPixelShader->vSet(lpo, cam);
		}
	}
}

#endif