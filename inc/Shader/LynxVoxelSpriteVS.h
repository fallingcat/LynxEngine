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

#ifndef __LYNXVOXELSPRITEVS_H__
#define __LYNXVOXELSPRITEVS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelSpriteVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CVoxelSpriteVS);
		public:						
			float								m_VoxelScale;
			Math::CVector3						m_Center;
		protected:					
			float								m_Color[4];
			
			GraphicsSystem::CShaderConstant*	m_WorldM;
            GraphicsSystem::CShaderConstant*    m_ViewProjM;			
			GraphicsSystem::CShaderConstant*	m_CameraM;
			GraphicsSystem::CShaderConstant*	m_DiffuseColor;
            GraphicsSystem::CShaderConstant*    m_PositionArray;
            GraphicsSystem::CShaderConstant*    m_ColorArray;            
			GraphicsSystem::CShaderConstant*    m_VoxelScaleConst;				
		public:			
			CVoxelSpriteVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelSpriteVS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);

			LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetPositionArrayConstant() {return m_PositionArray; };
            LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetColorArrayConstant() {return m_ColorArray; };            
		};		
	}		
}

#endif