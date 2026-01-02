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
#include <Shader/LynxDeferredDirectionalLightingPS.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <SceneSystem/Light/LynxDirectionalLight.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredDirectionalLightingPS::CDeferredDirectionalLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_LightData_Const = NULL;
			m_BoolData_Const = NULL;
			m_Exposure_Const = NULL;
			m_ZBias_Const = NULL;
			m_ViewToWorld_Const = NULL;
			for (int i=0; i<MAX_SHADOWMAP; i++)
				m_LightViewProj_Const[i] = NULL;							
			m_SplittedDepth_Const = NULL;
			m_ShadowmapGrad_Const = NULL;
			m_NumShadowmap_Const = NULL;

			m_DiffuseMap_Resource = NULL;
			m_NormalMap_Resource = NULL;
			m_SpecularMap_Resource = NULL;
			m_DepthMap_Resource = NULL;
			for (int i=0; i<MAX_SHADOWMAP; i++)
				m_ShadowMap_Resource[i] = NULL;										
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredDirectionalLightingPS::~CDeferredDirectionalLightingPS(void)
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
			for (int i=0; i<MAX_SHADOWMAP; i++)
			{
				if (m_LightViewProj_Const[i])
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightViewProj_Const[i]);								
			}
			if (m_SplittedDepth_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SplittedDepth_Const);								
			if (m_ShadowmapGrad_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ShadowmapGrad_Const);			
			if (m_NumShadowmap_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_NumShadowmap_Const);			


			if (m_DiffuseMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_DiffuseMap_Resource);						
			if (m_NormalMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_NormalMap_Resource);									
			if (m_SpecularMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_SpecularMap_Resource);						
			if (m_DepthMap_Resource)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_DepthMap_Resource);									
			for (int i=0; i<MAX_SHADOWMAP; i++)
			{
				if (m_ShadowMap_Resource[i])
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_ShadowMap_Resource[i]);								
			}
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDeferredDirectionalLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/def_directionallight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredDirectionalLightingPS::vCreateConstantAndResource() 
		{ 
			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			if (!m_LightData_Const)
				m_LightData_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLight");
			if (!m_BoolData_Const)
				m_BoolData_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbShadowing");
			if (!m_Exposure_Const)
				m_Exposure_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gExposure");
			if (!m_ZBias_Const)
				m_ZBias_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gZBias");
			if (!m_ViewToWorld_Const)
				m_ViewToWorld_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gViewToWorld");
			if (!m_SplittedDepth_Const)
				m_SplittedDepth_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSplittedDepth");	
			if (!m_ShadowmapGrad_Const)
				m_ShadowmapGrad_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gShadowmapTexelGrad");	
			if (!m_NumShadowmap_Const)
				m_NumShadowmap_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gNumSplittedShadowMap");	

			char ConstantName[64];
			for (int i=0; i<MAX_SHADOWMAP; i++)
			{
				if (!m_LightViewProj_Const[i])
				{
					sprintf(ConstantName, "gLightViewProj[%d]", i);
					m_LightViewProj_Const[i] = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, ConstantName);
				}				
			}			

			if (!m_DiffuseMap_Resource)
				m_DiffuseMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader,MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
			if (!m_NormalMap_Resource)
				m_NormalMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
			if (!m_SpecularMap_Resource)
				m_SpecularMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
			if (!m_DepthMap_Resource)
				m_DepthMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str());
			for (int i=0; i<MAX_SHADOWMAP; i++)
			{
				if (!m_ShadowMap_Resource[i])
				{
					sprintf(ConstantName, "%s[%d]", MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP).c_str() , i);
					m_ShadowMap_Resource[i] = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, ConstantName);				
				}
			}			
			vBindResource();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredDirectionalLightingPS::vBindResource()
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
				//if (m_ShadowMap_Resource)
				//	m_ShadowMap_Resource->SetResource(lpR->GetlpMap(SceneSystem::CRenderer::SHARED_SHADOWMAP)->GetTexture());									
				//if (!m_ProjectedLightMap_Resource)
				//	m_ProjectedLightMap_Resource = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(m_lpShader, MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::CMaterialSystem::PROJECTED_LIGHTMAP).c_str());			
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredDirectionalLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			//Set resources			
			m_DiffuseMap_Resource->Set();
			m_NormalMap_Resource->Set();
			m_SpecularMap_Resource->Set();
			m_DepthMap_Resource->Set();
			
			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			//m_DepthMap_Resource->vSet(lpR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetTexture());
			//m_DiffuseMap_Resource->vSet(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::DIFFUSE)->GetTexture());
			//m_NormalMap_Resource->vSet(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::NORMAL)->GetTexture());
			//m_SpecularMap_Resource->vSet(lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::SPECULAR)->GetTexture());

			float SplittedDepth[MAX_SHADOWMAP];

			m_LightData_Const->vMap();
				SceneSystem::CDirectionalLight* lpDL = ((SceneSystem::CDirectionalLight*)(lpc->GetlpRenObj()));				
				if (m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap > 1)
				{
					for (int i=0; i<m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap; i++)	
					{
						if (i != m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap-1)					
						{
							m_ShadowMap_Resource[i]->Set(lpDL->GetShadowMap().GetUtilTexture(i));
							m_LightViewProj_Const[i]->vSetMatrix(&(lpDL->m_SplittedLightViewProj[i]));
						}
						else
						{
							m_ShadowMap_Resource[i]->Set(lpDL->GetShadowMap().GetTexture());
							m_LightViewProj_Const[i]->vSetMatrix(&(lpDL->m_LightViewProj));
						}
						SplittedDepth[i] = m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetShadowMapSplittedDepth(i+1);
					}
					m_lpShader->vSetConstantF("gSplittedDepth", SplittedDepth, 4);						
				}
				else
				{
					m_ShadowMap_Resource[0]->Set(lpDL->GetShadowMap().GetTexture());
					m_LightViewProj_Const[0]->vSetMatrix(&(lpDL->m_LightViewProj));
				}
				float TexelGrad[2];
				TexelGrad[0] = 1.0f/(lpDL->GetShadowMap().GetTexture()->vGetWidth());
				TexelGrad[1] = 1.0f/(lpDL->GetShadowMap().GetTexture()->vGetHeight());
				m_ShadowmapGrad_Const->vSetVector2(TexelGrad);			

				// Set constants
				LIGHTDATA LightData;

				LightData.Color[0] = lpDL->m_Color.Red/255.0f;
				LightData.Color[1] = lpDL->m_Color.Green/255.0f;
				LightData.Color[2] = lpDL->m_Color.Blue/255.0f;
				LightData.Intensity = lpDL->m_Intensity;
				Math::CVector3 Pos;
				lpDL->GetPosition(Pos);
				LightData.Pos[0] = Pos.x;
				LightData.Pos[1] = Pos.y;
				LightData.Pos[2] = Pos.z;
				lpDL->GetDirection(Pos);
				LightData.Dir[0] = Pos.x;
				LightData.Dir[1] = Pos.y;
				LightData.Dir[2] = Pos.z;
				m_LightData_Const->vSetFloat((float*)&LightData, sizeof(LIGHTDATA)/sizeof(float));

				BOOL BoolData[2] = {static_cast<BOOL>(lpDL->m_bCastShadow), static_cast<BOOL>(m_lpGraphicsSystem->m_Config.bHDRRendering)};
				m_BoolData_Const->vSetBool(BoolData, 2);

				//m_NumShadowmap_Const->vSetInt(m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap);
				m_Exposure_Const->vSetFloat(cam->m_Exposure);
				m_ZBias_Const->vSetFloat(lpDL->m_DepthBias);
				m_ViewToWorld_Const->vSetMatrix(&cam->GetTransformMatrix());
				
				/*if (((CSky*)m_lpEngine->GetlpSceneSystem()->GetlpScene(0)->GetlpEnvironment()->GetlpSky())->GetSunLight() == this)
				{
					m_lpShader->vSetConstantB("gbSunLight", TRUE);
					m_lpShader->vSetTexture("SunLightMap", ((CSkyDomeContainer*)(m_lpEngine->GetlpSceneSystem()->GetlpScene(0)->GetlpEnvironment()->GetlpSky()->GetlpContainer()))->GetSkyLightMap().GetCurrent());			
				}
				else
				{
					m_lpShader->vSetConstantB("gbSunLight", FALSE);
				}*/
			m_LightData_Const->vUnmap();

			m_lpGraphicsSystem->GetlpRenderer()->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		
		}		
	}
}
