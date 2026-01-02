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

#ifndef __LYNXSPRITEVS_H__
#define __LYNXSPRITEVS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSpriteVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CSpriteVS);
		public:						
		protected:		
			GraphicsSystem::CShaderConstant*	m_WorldViewProjM;
			GraphicsSystem::CShaderConstant*	m_Scale;
			GraphicsSystem::CShaderConstant*	m_TextureUVM;
		public:			
			CSpriteVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSpriteVS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);

			void								SetParam(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif