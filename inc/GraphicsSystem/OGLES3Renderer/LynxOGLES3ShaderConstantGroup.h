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

#ifndef __LYNXOGLES3SHADERCONSTANTGROUP_H__
#define __LYNXOGLES3SHADERCONSTANTGROUP_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderConstantGroup.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES3ShaderConstantGroup : public CShaderConstantGroup
		{	
			LYNX_DECLARE_CLASS(COGLES3ShaderConstantGroup);
		public:			
			COGLES3ShaderConstantGroup(CShader* lps, const char* name):CShaderConstantGroup(lps, name) {};
			virtual ~COGLES3ShaderConstantGroup() {};

			void*								vMap() {return NULL; };
			void								vUnmap() {};
			LYNXFORCEINLINE void				vSetUpdateFrequence(const CRenderer::CONSTANTUPDATEFREQENCE f) {m_UpdateFrequence = CRenderer::CUF_PER_FRAME; };	
		};		
	}
}

#endif

#endif