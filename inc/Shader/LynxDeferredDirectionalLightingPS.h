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

#ifndef __LYNXDEFERREDDIRECTIONALLIGHTINGPS_H__
#define __LYNXDEFERREDDIRECTIONALLIGHTINGPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDeferredDirectionalLightingPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CDeferredDirectionalLightingPS);
		public:					
			typedef struct {
				float			Pos[3];
				float			Color[3];    
				float			Intensity;
				float			Dir[3];				
			}LIGHTDATA;
			static const int					MAX_SHADOWMAP = 4;
		protected:					
			GraphicsSystem::CShaderConstant*	m_LightData_Const;
			GraphicsSystem::CShaderConstant*	m_BoolData_Const;
			GraphicsSystem::CShaderConstant*	m_Exposure_Const;
			GraphicsSystem::CShaderConstant*	m_ZBias_Const;
			GraphicsSystem::CShaderConstant*	m_ViewToWorld_Const;
			GraphicsSystem::CShaderConstant*	m_LightViewProj_Const[MAX_SHADOWMAP];
			GraphicsSystem::CShaderConstant*	m_SplittedDepth_Const;
			GraphicsSystem::CShaderConstant*	m_ShadowmapGrad_Const;			
			GraphicsSystem::CShaderConstant*	m_NumShadowmap_Const;
			
			GraphicsSystem::CShaderResource*	m_DiffuseMap_Resource;
			GraphicsSystem::CShaderResource*	m_NormalMap_Resource;
			GraphicsSystem::CShaderResource*	m_SpecularMap_Resource;
			GraphicsSystem::CShaderResource*	m_DepthMap_Resource;
			GraphicsSystem::CShaderResource*	m_ShadowMap_Resource[MAX_SHADOWMAP];			
		public:			
			CDeferredDirectionalLightingPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDeferredDirectionalLightingPS();

			LYNXBOOL							vCreate();
			void								vCreateConstantAndResource();
			void								vBindResource();
			void								vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif