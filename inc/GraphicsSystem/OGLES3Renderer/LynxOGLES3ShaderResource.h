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

#ifndef __LYNXOGLES3SHADERRESOURCE_H__
#define __LYNXOGLES3SHADERRESOURCE_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxShaderResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES3ShaderResource : public CShaderResource
		{	
			LYNX_DECLARE_CLASS(COGLES3ShaderResource);
		public:						
		protected:				
		public:			
			COGLES3ShaderResource(CShader* lps, const char* name);
			COGLES3ShaderResource(CShader* lps, const char* name, CResource* r);
			virtual ~COGLES3ShaderResource();
		};		
	}
}

#endif

#endif