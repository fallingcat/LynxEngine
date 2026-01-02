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

#ifndef __LYNXDEFERREDAMBIENTLIGHTINGPS_H__
#define __LYNXDEFERREDAMBIENTLIGHTINGPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDeferredAmbientLightingPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CDeferredAmbientLightingPS);
		public:						
			typedef struct {
				float						Color[3];
				float						Intensity;
			}LIGHTDATA;
		protected:					
			GraphicsSystem::CShaderConstant*	m_LightData_Const;
			GraphicsSystem::CShaderConstant*	m_bHDRRendering_Const;
			GraphicsSystem::CShaderConstant*	m_Exposure_Const;
		public:			
			CDeferredAmbientLightingPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDeferredAmbientLightingPS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif