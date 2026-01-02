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

#ifndef __LYNXONELIGHTINGPS_H__
#define __LYNXONELIGHTINGPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS COneLightingPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(COneLightingPS);
		public:						
		protected:					
			float								m_AmbientColor[4];
			GraphicsSystem::CShaderConstant*    m_AmbientColorConst;
			float								m_LightColor[4];
			GraphicsSystem::CShaderConstant*    m_LightColorConst;
			GraphicsSystem::CShaderConstant*	m_ConstantColor_Const;
		public:			
			COneLightingPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~COneLightingPS();

			virtual LYNXBOOL					vCreate();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
			virtual void						vCreateConstantAndResource();
		};		
	}		
}

#endif