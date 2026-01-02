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
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxCameraContainer.h>
#include <MaterialSystem/LynxMaterial.h>
#include <SceneSystem/Light/LynxPointLight.h>
#include <Shader/LynxDeferredPointLightingPS.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredPointLightingPS::CDeferredPointLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_LightData_Const = NULL;
			m_BoolData_Const = NULL;
			m_Exposure_Const = NULL;
			m_ZBias_Const = NULL;
			m_ViewToWorld_Const = NULL;
			m_LightViewProj_Const = NULL;
			m_ShadowmapGrad_Const = NULL;

			m_DiffuseMap_Resource = NULL;
			m_NormalMap_Resource = NULL;
			m_SpecularMap_Resource = NULL;
			m_DepthMap_Resource = NULL;
			m_ShadowMap_Resource = NULL;
			m_ProjectedLightMap_Resource = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredPointLightingPS::~CDeferredPointLightingPS(void)
		{
			if (m_LightData_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightData_Const);
			if (m_BoolData_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_BoolData_Const);
			if (m_Exposure_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Exposure_Const);
			if (m_ZBias_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ZBias_Const);			
			if (m_ViewToWorld_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ViewToWorld_Const);
			if (m_LightViewProj_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightViewProj_Const);
			if (m_ShadowmapGrad_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ShadowmapGrad_Const);			

			if (m_DiffuseMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_DiffuseMap_Resource);						
			if (m_NormalMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_NormalMap_Resource);									
			if (m_SpecularMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_SpecularMap_Resource);						
			if (m_DepthMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_DepthMap_Resource);									
			if (m_ShadowMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_ShadowMap_Resource);						
			if (m_ProjectedLightMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_ProjectedLightMap_Resource);									
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDeferredPointLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/def_pointlight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredPointLightingPS::vCreateConstantAndResource() 
		{ 
			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			if (!m_LightData_Const)
				m_LightData_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLight");
			if (!m_BoolData_Const)
				m_BoolData_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbShadowing");
			if (!m_Exposure_Const)
				m_Exposure_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gExposure");			
			if (!m_ViewToWorld_Const)
				m_ViewToWorld_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gViewToWorld");
			if (!m_LightViewProj_Const)
				m_LightViewProj_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightViewProj");

			if (!m_ZBias_Const)
				m_ZBias_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gZBias");
			if (!m_ShadowmapGrad_Const)
				m_ShadowmapGrad_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gShadowmapTexelGrad");	

			if (!m_DiffuseMap_Resource)
				m_DiffuseMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader,MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
			if (!m_NormalMap_Resource)
				m_NormalMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
			if (!m_SpecularMap_Resource)
				m_SpecularMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
			if (!m_DepthMap_Resource)
				m_DepthMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str());
			if (!m_ShadowMap_Resource)
				m_ShadowMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP).c_str());				
			if (!m_ProjectedLightMap_Resource)
				m_ProjectedLightMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::PROJECTED_LIGHTMAP).c_str());			

			vBindResource();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredPointLightingPS::vBindResource()
		{
			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			if (lpR)
			{
				if (m_DiffuseMap_Resource)
					m_DiffuseMap_Resource->SetResource(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::DIFFUSE)->GetTexture());
				if (m_NormalMap_Resource)
					m_NormalMap_Resource->SetResource(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::NORMAL)->GetTexture());
				if (m_SpecularMap_Resource)
					m_SpecularMap_Resource->SetResource(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::SPECULAR)->GetTexture());						
				if (m_DepthMap_Resource)
					m_DepthMap_Resource->SetResource(lpR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetTexture());									
				if (m_ShadowMap_Resource)
					m_ShadowMap_Resource->SetResource(lpR->GetlpMap(SceneSystem::CRenderer::SHARED_CUBESHADOWMAP)->GetTexture());									
				//if (!m_ProjectedLightMap_Resource)
				//	m_ProjectedLightMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::CMaterialSystem::PROJECTED_LIGHTMAP).c_str());			
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredPointLightingPS::SetResources(CContainer* lpc, CCameraContainer* cam) 
		{
			m_DiffuseMap_Resource->Set();
			m_NormalMap_Resource->Set();
			m_SpecularMap_Resource->Set();
			m_DepthMap_Resource->Set();
			
			SceneSystem::CPointLight* lpPL = ((SceneSystem::CPointLight*)(lpc->GetlpRenObj()));

			if (lpPL->IsProjectedLightMapUsed())
				m_ProjectedLightMap_Resource->Set(lpPL->GetProjectedLightMap());	
			if (lpPL->m_bCastShadow)
				m_ShadowMap_Resource->Set(lpPL->GetShadowMap().GetCurrent());							
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredPointLightingPS::UpdateConstants(CContainer* lpc, CCameraContainer* cam) 
		{
			SceneSystem::CPointLight* lpPL = ((SceneSystem::CPointLight*)(lpc->GetlpRenObj()));

			m_ShadowmapGrad_Const->vMap();				
				if (lpPL->m_bCastShadow)
				{
					float TexelGrad[2];			
					TexelGrad[0] = 1.0f/(float)(lpPL->GetShadowMap().GetWidth());
					TexelGrad[1] = 1.0f/(float)(lpPL->GetShadowMap().GetHeight());
					m_ShadowmapGrad_Const->vSetVector2(TexelGrad);				
					m_ZBias_Const->vSetFloat(lpPL->m_DepthBias);
				}				
			m_ShadowmapGrad_Const->vUnmap();								
					
			m_Exposure_Const->vMap();
				// Set constants
				LIGHTDATA LightData;
				
				LightData.Color[0] = lpPL->m_Color.Red/255.0f;
				LightData.Color[1] = lpPL->m_Color.Green/255.0f;
				LightData.Color[2] = lpPL->m_Color.Blue/255.0f;
				LightData.Intensity = lpPL->m_Intensity;
				LightData.Radius = lpPL->m_Radius;
				Math::CVector3 Pos;
				lpPL->GetPosition(Pos);
				LightData.Pos[0] = Pos.x;
				LightData.Pos[1] = Pos.y;
				LightData.Pos[2] = Pos.z;
				m_LightData_Const->vSetFloat((float*)&LightData, sizeof(LIGHTDATA)/sizeof(float));

				BOOL BoolData[3] = {static_cast<BOOL>(lpPL->m_bCastShadow), static_cast<BOOL>(m_lpGraphicsSystem->m_Config.bHDRRendering), static_cast<BOOL>(lpPL->IsProjectedLightMapUsed())};
				m_BoolData_Const->vSetBool(BoolData, 3);
				
				m_ViewToWorld_Const->vSetMatrix(&cam->GetTransformMatrix());
				m_LightViewProj_Const->vSetMatrix(&lpPL->m_LightViewProj);	
				m_Exposure_Const->vSetFloat(cam->m_Exposure);
			m_Exposure_Const->vUnmap();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredPointLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			//Set resources
			SetResources(lpc, cam);
			//Update constants
			UpdateConstants(lpc, cam);
			
			m_lpGraphicsSystem->GetlpRenderer()->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		
		}		
	}
}
