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

#ifndef __LYNXVOXELSPRITEONELIGHTINGVS_H__
#define __LYNXVOXELSPRITEONELIGHTINGVS_H__

#include <Shader/LynxVoxelSpriteVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelSpriteOneLightingVS : public CVoxelSpriteVS
		{	
			LYNX_DECLARE_CLASS(CVoxelSpriteOneLightingVS);
		public:									
		protected:								
			Math::CVector3						m_LightDir;
			float								m_LightColor[4];	
			float								m_AmbientLightColor[4];			

			GraphicsSystem::CShaderConstant*    m_LightDirConst;
			GraphicsSystem::CShaderConstant*    m_LightColorConst;
			GraphicsSystem::CShaderConstant*	m_AmbientColorConst;
			GraphicsSystem::CShaderConstant*	m_CameraPos;
			GraphicsSystem::CShaderConstant*	m_SpecularColor;   
			GraphicsSystem::CShaderConstant*	m_CenterConst;			
		public:			
			CVoxelSpriteOneLightingVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelSpriteOneLightingVS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);

			LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetPositionArrayConstant() {return m_PositionArray; };
            LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetColorArrayConstant() {return m_ColorArray; };            
		};		
	}		
}

#endif