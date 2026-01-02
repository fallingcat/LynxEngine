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

#ifndef __LYNXD3D9SHADERRESOURCE_H__
#define __LYNXD3D9SHADERRESOURCE_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/LynxShaderResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9ShaderResource : public CShaderResource
		{	
			LYNX_DECLARE_CLASS(CD3D9ShaderResource);
		public:						
		protected:				
		public:			
			CD3D9ShaderResource(CShader* lps, const char* name);
			CD3D9ShaderResource(CShader* lps, const char* name, CResource* r);
			virtual ~CD3D9ShaderResource();			
		};		
	}
}

#endif

#endif