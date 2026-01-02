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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11PixelShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderConstantGroup.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderConstantGroup::CD3D11ShaderConstantGroup(CShader *lps, const char* name)	
		: CShaderConstantGroup(lps, name)
		{		
			m_lpReflection = NULL;
			m_lpConstantBuffer = NULL;

			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					m_lpReflection = ((CD3D11VertexShader*)lps)->GetlpShaderReflection();
					m_lpConstantBuffer = ((CD3D11VertexShader*)lps)->GetConstantBuffer(name);
					break;
				case PIXEL_SHADER:
					m_lpReflection = ((CD3D11PixelShader*)lps)->GetlpShaderReflection();
					m_lpConstantBuffer = ((CD3D11PixelShader*)lps)->GetConstantBuffer(name);
					break;
				case HULL_SHADER:
					break;
				case DOMAIN_SHADER:
					break;
				case GEOMETRY_SHADER:
					break;
				case COMPUTE_SHADER:
					break;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderConstantGroup::~CD3D11ShaderConstantGroup(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D11ShaderConstantGroup::vMap()
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			return m_lpConstantBuffer->vMap(CRenderer::LOCK_TO_WRITE);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstantGroup::vUnmap()
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			m_lpConstantBuffer->vUnmap();
		}				
	}
}
#endif