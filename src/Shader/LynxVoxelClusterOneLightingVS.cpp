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
#include <Shader/LynxVoxelClusterOneLightingVS.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterOneLightingVS::CVoxelClusterOneLightingVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CVoxelClusterVS(pgs)
		{			
        }
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterOneLightingVS::~CVoxelClusterOneLightingVS(void)
		{
        }						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVoxelClusterOneLightingVS::vCreate() 
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
			ShaderMacro.Definition = CAnsiString("0");
			m_ShaderMacros.push_back(ShaderMacro);

			ShaderMacro.Name = CAnsiString("FAKE_AO_LEVEL2");
			ShaderMacro.Definition = CAnsiString("0");
			m_ShaderMacros.push_back(ShaderMacro);

			ShaderMacro.Name = CAnsiString("USE_SPECULAR");
			ShaderMacro.Definition = CAnsiString("0");
			m_ShaderMacros.push_back(ShaderMacro);
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelcluster_onelighting.vso"), &m_ShaderMacros);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelClusterOneLightingVS::vCreateConstantAndResource() 
		{ 
            CVoxelClusterVS::vCreateConstantAndResource();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelClusterOneLightingVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
            Math::CMatrix4 MM, WVPM, WVM, WSM, VPM;
			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();						
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());  
            lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());	
		    
            m_WorldViewProjM->vMap();
                m_WorldViewProjM->vSetMatrix(&WVPM);				
                m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
				
				// TODO : find out the bug of ES2 shader
				#ifdef __iOS__
					m_ViewProjM->vSetMatrix(&VPM);
				#endif
            m_WorldViewProjM->vUnmap();		
		}
	}
}
