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

#ifndef __LYNXNULLSHADERCONSTANTGROUP_H__
#define __LYNXNULLSHADERCONSTANTGROUP_H__

#include <GraphicsSystem/LynxShaderConstantGroup.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullShaderConstantGroup : public CShaderConstantGroup
		{	
			LYNX_DECLARE_CLASS(CNullShaderConstantGroup);
		public:						
		protected:				
		public:			
			CNullShaderConstantGroup(CShader* lps, const char* name):CShaderConstantGroup(lps, name) {};
			virtual ~CNullShaderConstantGroup() {};

			void*								vMap() {return NULL; };
			void								vUnmap() {};
			LYNXFORCEINLINE void				vSetUpdateFrequence(const CRenderer::CONSTANTUPDATEFREQENCE f) {m_UpdateFrequence = CRenderer::CUF_PER_FRAME; };	
		};		
	}
}

#endif