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
#include <Shader/LynxVoxelSpriteOneLightingVS.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelSpriteOneLightingVS::CVoxelSpriteOneLightingVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CVoxelSpriteVS(pgs)
		{	
			m_LightDirConst = NULL;
			m_LightColorConst = NULL;
			m_AmbientColorConst = NULL;
			m_CameraPos = NULL;
			m_SpecularColor = NULL;
			m_CenterConst = NULL;

			m_LightDir = Math::CVector3(0.407f, 0.339f, -0.848f);
			m_LightColor[0] = 0.7f;
			m_LightColor[1] = 0.7f;
			m_LightColor[2] = 0.7f;
			m_LightColor[3] = 1.0f;	

			m_AmbientLightColor[0] = 0.4f;
			m_AmbientLightColor[1] = 0.4f;
			m_AmbientLightColor[2] = 0.4f;
			m_AmbientLightColor[3] = 1.0f;			
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelSpriteOneLightingVS::~CVoxelSpriteOneLightingVS(void)
		{
			if (m_AmbientColorConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_AmbientColorConst);            
			if (m_LightDirConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightDirConst);
			if (m_LightColorConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightColorConst);			
			if (m_CameraPos)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraPos);
			if (m_SpecularColor)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SpecularColor);            
			if (m_CenterConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CenterConst);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVoxelSpriteOneLightingVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), CVoxelClusterContainer::m_SpriteBatchSize);
			ShaderMacro.Name = CAnsiString("VOXELSPRITE_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);			

			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelsprite_onelighting.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelSpriteOneLightingVS::vCreateConstantAndResource() 
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

			if (!m_LightDirConst)
				m_LightDirConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightDir");

			if (!m_LightColorConst)
				m_LightColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightColor");

			if (!m_AmbientColorConst)
				m_AmbientColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gAmbientColor");

			if (!m_CameraPos)
				m_CameraPos = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCameraPos");			
			if (!m_SpecularColor)
				m_SpecularColor = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSpecularColor");            

			if (!m_CenterConst)
				m_CenterConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCenter");				
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVoxelSpriteOneLightingVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX VM, VPM, WVM;
			CVector3 Scale;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());				
			VM = cam->GetTransformMatrix();		
			
			lpc->GetTransformMatrix().GetScale(Scale);
            m_VoxelScale = static_cast<CVoxelClusterContainer*>(lpc)->GetCurrentVoxelScale() * 1.5f * Scale.x;
            
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

				SceneSystem::CLight* lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
				if (lpL)
				{
					if (lpL->GetLightType() == SceneSystem::CLight::DIRECTIONAL)
					{
						lpL->GetDirection(m_LightDir);							
						m_LightDir = -1.0f * m_LightDir;
					}
					m_LightColor[0] = (lpL->m_Color.Red * lpL->m_Intensity) / 255.0f;
					m_LightColor[1] = (lpL->m_Color.Green * lpL->m_Intensity) / 255.0f;
					m_LightColor[2] = (lpL->m_Color.Blue * lpL->m_Intensity)/ 255.0f;
					m_LightColor[3] = 1.0f;
				}
				m_LightDirConst->vSetVector3((float*)(&(m_LightDir.x)));					
				m_LightColorConst->vSetVector4(m_LightColor);

				lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight();
				if (lpL)
				{
					m_AmbientLightColor[0] = (lpL->m_Color.Red * lpL->m_Intensity)/ 255.0f;
					m_AmbientLightColor[1] = (lpL->m_Color.Green * lpL->m_Intensity) / 255.0f;
					m_AmbientLightColor[2] = (lpL->m_Color.Blue * lpL->m_Intensity) / 255.0f;
					m_AmbientLightColor[3] = 1.0f;
				}
				m_AmbientColorConst->vSetVector4(m_AmbientLightColor);				

				float Color[4];
				Math::CVector3 Pos;
				cam->vGetPosition(Pos);
				m_CameraPos->vSetVector3(&(Pos.x));			
				Color[0] = lpc->GetlpCurrentMaterial()->m_Specular.Red / 255.0f;
				Color[1] = lpc->GetlpCurrentMaterial()->m_Specular.Green / 255.0f;
				Color[2] = lpc->GetlpCurrentMaterial()->m_Specular.Blue / 255.0f;
				Color[3] = 1.0f;
				m_SpecularColor->vSetVector4(Color);				

				m_CenterConst->vSetVector3((float*)(&(m_Center.x)));
            m_WorldM->vUnmap();
		}
	}
}
