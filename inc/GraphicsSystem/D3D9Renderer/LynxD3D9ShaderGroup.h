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

#ifndef __LYNXD3D9SHADERGROUP_H__
#define __LYNXD3D9SHADERGROUP_H__

#include <LynxResource.h>
#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9ShaderGroup : public CShaderGroup
		{	
			LYNX_DECLARE_CLASS(CD3D9ShaderGroup);
		public:						
		protected:			
		public:			
			CD3D9ShaderGroup(CGraphicsSystem* lpg);
			virtual ~CD3D9ShaderGroup();
						
			LYNXBOOL							vCreate();
			void								vSet(CContainer* lpo, CCameraContainer* cam);
		};		
	}	
}
#endif

#endif