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

#ifndef __LYNXDEFERREDSPOTLIGHTINGPS_H__
#define __LYNXDEFERREDSPOTLIGHTINGPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDeferredSpotLightingPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CDeferredSpotLightingPS);
		public:						
			typedef struct {
				float			Pos[3];
				float			Radius; 
				float			Color[3]; 
				float			Intensity;				   				
				float			Dir[3];			  
				float			Rad;	    
				float			FadeOutRad;  
			}LIGHTDATA;
		protected:	
			GraphicsSystem::CShaderConstant*	m_LightData_Const;
			GraphicsSystem::CShaderConstant*	m_BoolData_Const;
			GraphicsSystem::CShaderConstant*	m_Exposure_Const;
			GraphicsSystem::CShaderConstant*	m_ZBias_Const;
			GraphicsSystem::CShaderConstant*	m_ViewToWorld_Const;
			GraphicsSystem::CShaderConstant*	m_LightViewProj_Const;
			GraphicsSystem::CShaderConstant*	m_ShadowmapGrad_Const;
			
			GraphicsSystem::CShaderResource*	m_DiffuseMap_Resource;
			GraphicsSystem::CShaderResource*	m_NormalMap_Resource;
			GraphicsSystem::CShaderResource*	m_SpecularMap_Resource;
			GraphicsSystem::CShaderResource*	m_DepthMap_Resource;
			GraphicsSystem::CShaderResource*	m_ShadowMap_Resource;
			GraphicsSystem::CShaderResource*	m_ProjectedLightMap_Resource;
		public:			
			CDeferredSpotLightingPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDeferredSpotLightingPS();

			LYNXBOOL							vCreate();
			void								vBindResource();
			void								vCreateConstantAndResource();
			void								vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif