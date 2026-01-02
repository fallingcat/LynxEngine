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
#include <Shader/LynxVoxelSpriteVS.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelSpriteVS::CVoxelSpriteVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldM = NULL;
            m_ViewProjM = NULL;
			m_CameraM = NULL;
			m_PositionArray = NULL;
            m_ColorArray = NULL;
            m_DiffuseColor = NULL;
			m_Color[0] = m_Color[1] = m_Color[2] = m_Color[3] = 1.0f;
			m_VoxelScale = 1.0f;
			m_VoxelScaleConst = NULL;
			m_Center = Math::CVector3(0.0f, 0.0f, 0.0f);
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelSpriteVS::~CVoxelSpriteVS(void)
		{
			if (m_WorldM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldM);
			if (m_ViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ViewProjM);
			if (m_CameraM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraM);
			if (m_DiffuseColor)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_DiffuseColor);							
			if (m_PositionArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_PositionArray);
            if (m_ColorArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ColorArray);
            if (m_VoxelScaleConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_VoxelScaleConst);			
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVoxelSpriteVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), CVoxelClusterContainer::m_SpriteBatchSize);
			ShaderMacro.Name = CAnsiString("VOXELSPRITE_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);			

			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelsprite.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelSpriteVS::vCreateConstantAndResource() 
		{ 
            if (!m_WorldM)
				m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");
            if (!m_ViewProjM)
                m_ViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");                       
			if (!m_CameraM)
				m_CameraM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCameraM");			
         
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
		void CVoxelSpriteVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX VM, VPM, WVM;
			CVector3 Scale;
			
			LYNX_ASSERT(cam);		

			lpc->GetTransformMatrix().GetScale(Scale);
            m_VoxelScale = static_cast<CVoxelClusterContainer*>(lpc)->GetCurrentVoxelScale() * 1.5f * Scale.x;
            
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());				
			VM = cam->GetTransformMatrix();		
			
			m_WorldM->vMap();				
                m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                m_ViewProjM->vSetMatrix(&VPM);
				m_CameraM->vSetMatrix(&VM);
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
