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
#include <Shader/LynxVoxelClusterVS.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterVS::CVoxelClusterVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{
			m_bAOLevel2 = LYNX_FALSE;

            m_WorldViewProjM = NULL;
			m_WorldM = NULL;
            m_ViewProjM = NULL;
			m_LightDirConst = NULL;
			m_LightColorConst = NULL;
            m_PositionArray = NULL;
            m_ColorArray = NULL;
            m_AOCodeArray = NULL;
			m_DiffuseColor = NULL;
			m_Color[0] = m_Color[1] = m_Color[2] = m_Color[3] = 1.0f;
			m_VoxelScale = 1.0f;
			m_VoxelScaleConst = NULL;
        }
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelClusterVS::~CVoxelClusterVS(void)
		{
            if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
            if (m_WorldM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldM);
			if (m_ViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ViewProjM);
			if (m_LightDirConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightDirConst);
			if (m_LightColorConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightColorConst);			
			if (m_DiffuseColor)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_DiffuseColor);							
			if (m_PositionArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_PositionArray);
            if (m_ColorArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ColorArray);
            if (m_AOCodeArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_AOCodeArray);

			if (m_VoxelScaleConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_VoxelScaleConst);			
        }						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVoxelClusterVS::vCreate() 
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
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelcluster.vso"), &m_ShaderMacros);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelClusterVS::vCreateConstantAndResource() 
		{ 
            if (!m_WorldM)
				m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");
            #ifdef __MOBILE__
                if (!m_ViewProjM)
                    m_ViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gViewProj");
            #else
                if (!m_WorldViewProjM)
                    m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
            #endif
         
			if (!m_VoxelScaleConst)
				m_VoxelScaleConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gVoxelScale");				

			if (!m_DiffuseColor)
				m_DiffuseColor = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gDiffuseColor");
            if (!m_PositionArray)
				m_PositionArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gPositionArray");
            if (!m_ColorArray)
				m_ColorArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gColorArray");			
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelClusterVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
            Math::CMatrix4 MM, WVPM, WVM, WSM, VPM;
			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();						
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
            
            #ifdef __MOBILE__
                lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());
            #else
                lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());
                lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());
            #endif
            m_VoxelScale = static_cast<CVoxelClusterContainer*>(lpc)->GetCurrentVoxelScale();
		    
            m_WorldM->vMap();				
                m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                #ifdef __MOBILE__
                    m_ViewProjM->vSetMatrix(&VPM);
                #else
                    m_WorldViewProjM->vSetMatrix(&WVPM);
                #endif
				if (lpc)
				{
					m_Color[0] = lpc->GetlpCurrentMaterial()->m_Diffuse.Red / 255.0f;
					m_Color[1] = lpc->GetlpCurrentMaterial()->m_Diffuse.Green / 255.0f;
					m_Color[2] = lpc->GetlpCurrentMaterial()->m_Diffuse.Blue / 255.0f;
					m_Color[3] = lpc->GetlpCurrentMaterial()->m_Diffuse.Alpha / 255.0f;
				}
				m_DiffuseColor->vSetVector4(m_Color);
				m_VoxelScaleConst->vSetFloat(m_VoxelScale);				
            m_WorldM->vUnmap();
		}
	}
}
