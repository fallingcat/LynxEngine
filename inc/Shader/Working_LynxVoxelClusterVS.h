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

#ifndef __LYNXVOXELCLUSTERVS_H__
#define __LYNXVOXELCLUSTERVS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		template <LYNXBOOL bFakeAO, LYNXBOOL bFakeAOLevel2, LYNXBOOL bSpecular>
		class LYNXENGCLASS CVoxelClusterVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CVoxelClusterVS);
		public:						
		protected:					
			LYNXBOOL							m_bAOLevel2;			

            GraphicsSystem::CShaderConstant*	m_WorldViewProjM;
			GraphicsSystem::CShaderConstant*	m_WorldM;
            GraphicsSystem::CShaderConstant*    m_ViewProjM;
            GraphicsSystem::CShaderConstant*    m_PositionArray;
            GraphicsSystem::CShaderConstant*    m_ColorArray;
            GraphicsSystem::CShaderConstant*    m_AOCodeArray;			
		public:			
			CVoxelClusterVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelClusterVS();
            
			virtual LYNXBOOL					vCreate();
            virtual void                        vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
            
			LYNXFORCEINLINE const LYNXBOOL					 IsAOLevel2() const {return m_bAOLevel2; }
            LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetPositionArrayConstant() {return m_PositionArray; };
            LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetColorArrayConstant() {return m_ColorArray; };
            LYNXFORCEINLINE GraphicsSystem::CShaderConstant* GetAOCodeArrayConstant() {return m_AOCodeArray; };			
        };		
	}		

	#define IMPLEMENT_VOXELCLUSTERVS(bFakeAO, bFakeAOLevel2, bSpecular) \
		typedef Shader::CVoxelClusterVS<bFakeAO, bFakeAOLevel2, bSpecular>			Shader::CVoxelColorPS##bFakeAO##bSpecular;\
		IMPLEMENT_SHADERCLASS(CVoxelClusterVS##bFakeAO##bSpecular)
}

#endif
