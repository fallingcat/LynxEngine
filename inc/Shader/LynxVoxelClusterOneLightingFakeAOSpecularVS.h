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

#ifndef __LYNXVOXELCLUSTERONELIGHTINGFAKEAOSPECULARVS_H__
#define __LYNXVOXELCLUSTERONELIGHTINGFAKEAOSPECULARVS_H__

#include <Shader/LynxVoxelClusterOneLightingFakeAOVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelClusterOneLightingFakeAOSpecularVS : public CVoxelClusterOneLightingFakeAOVS
		{	
			LYNX_DECLARE_CLASS(CVoxelClusterOneLightingFakeAOSpecularVS);
		public:						
		protected:					
            GraphicsSystem::CShaderConstant*	m_CameraPos;
			GraphicsSystem::CShaderConstant*	m_SpecularColor;            
		public:			
			CVoxelClusterOneLightingFakeAOSpecularVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelClusterOneLightingFakeAOSpecularVS();

			virtual LYNXBOOL					vCreate();
            virtual void                        vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);       
		};		
	}		
}

#endif