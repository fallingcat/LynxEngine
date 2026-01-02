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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9PixelShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderResource::CD3D9ShaderResource(CShader *lps, const char* name)	
		: CShaderResource(lps, name)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderResource::CD3D9ShaderResource(CShader *lps, const char* name, CResource* r)	
		: CShaderResource(lps, name, r)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderResource::~CD3D9ShaderResource(void)
		{				
		}						
	}
}
#endif