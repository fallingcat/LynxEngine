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

#ifndef __LYNXD3D11SHADERGROUP_H__
#define __LYNXD3D11SHADERGROUP_H__

#include <LynxResource.h>
#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11ShaderGroup : public CShaderGroup
		{	
			LYNX_DECLARE_CLASS(CD3D11ShaderGroup);
		public:						
		protected:			
		public:			
			CD3D11ShaderGroup(CGraphicsSystem* lpg);
			virtual ~CD3D11ShaderGroup();
						
			LYNXBOOL							vCreate();
			void								vSet(CContainer* lpo, CCameraContainer* cam);
		};		
	}	
}

#endif

#endif