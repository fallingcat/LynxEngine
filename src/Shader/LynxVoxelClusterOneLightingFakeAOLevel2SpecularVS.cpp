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

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxRenderableObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxVoxelClusterOneLightingFakeAOLevel2SpecularVS.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterOneLightingFakeAOLevel2SpecularVS::CVoxelClusterOneLightingFakeAOLevel2SpecularVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CVoxelClusterOneLightingFakeAOSpecularVS(pgs)
		{
			m_bAOLevel2 = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterOneLightingFakeAOLevel2SpecularVS::~CVoxelClusterOneLightingFakeAOLevel2SpecularVS(void)
		{			
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVoxelClusterOneLightingFakeAOLevel2SpecularVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

            m_ShaderMacros.clear();
            
			lynxSprintf(Buffer, _T("%d"), CVoxelClusterContainer::m_BatchSize);
			ShaderMacro.Name = CAnsiString("VOXELCLUSTER_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);            
			
            ShaderMacro.Name = CAnsiString("FAKE_VOXEL_AO");
			#ifdef __FAKE_VOXEL_AO__
				ShaderMacro.Definition = CAnsiString("1");
			#else
				ShaderMacro.Definition = CAnsiString("0");
			#endif
			m_ShaderMacros.push_back(ShaderMacro);

			ShaderMacro.Name = CAnsiString("USE_FAKE_VOXEL_AO");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);

			ShaderMacro.Name = CAnsiString("FAKE_AO_LEVEL2");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);

			ShaderMacro.Name = CAnsiString("USE_SPECULAR");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelcluster_onelighting.vso"), &m_ShaderMacros);
		}        
	}
}
