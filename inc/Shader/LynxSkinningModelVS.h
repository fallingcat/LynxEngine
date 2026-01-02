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

#ifndef __LYNXSKINNINGMODELVS_H__
#define __LYNXSKINNINGMODELVS_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSkinningModelVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CSkinningModelVS);
		public:						
		protected:					
			GraphicsSystem::CShaderConstant*	m_WorldViewProjM;
			GraphicsSystem::CShaderConstant*	m_WorldViewM;
			GraphicsSystem::CShaderConstant*	m_WorldM;
			GraphicsSystem::CShaderConstant*	m_ProjectionM;
			GraphicsSystem::CShaderConstant*	m_BoneMatrix;
		public:			
			CSkinningModelVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSkinningModelVS();
			
			LYNXBOOL							vCreate();
			void								vCreateConstantAndResource(); 
			void								vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif