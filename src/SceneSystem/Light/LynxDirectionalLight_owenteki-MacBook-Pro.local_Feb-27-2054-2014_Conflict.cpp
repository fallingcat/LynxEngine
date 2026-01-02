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
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxAtmosphere.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/Light/LynxDirectionalLight.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxShadowMap.h>
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <LynxPyramidProxy.h>
#include <SceneSystem/StaticObj/LynxSky.h>
#include <Container/LynxSkyDomeContainer.h>
#include <Container/LynxSkyLightMap.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDirectionalLight::CDirectionalLight(CScene* const lpscene)
		: CLight(lpscene)
		{
			m_LightType = DIRECTIONAL;
			m_Radius = 1000.0f;
			m_DepthBias = 0.0f;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDirectionalLight::~CDirectionalLight(void)
		{
			vRelease();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDirectionalLight::vRelease(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDirectionalLight::vCreate(int w, int h)
		{		
			LYNX_ASSERT(m_lpScene);

			if (m_bCastShadow)
			{
				if (!CreateShadowmap(w, h))
					return LYNX_FALSE;									
			}

			if (!CLight::vCreate())
				return LYNX_FALSE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDirectionalLight::CreateShadowmap(int w, int h)
		{		
			LYNX_ASSERT(m_lpScene);

			if (w != m_ShadowmapWidth || h !=  m_ShadowmapHeight)
			{
				if (m_lpShadowMap)
					LYNXDEL m_lpShadowMap;

				m_ShadowmapWidth = w;
				m_ShadowmapHeight = h;
			
				CRenderer* lpR = (m_lpScene->GetlpRenderer());
				CString Name = GetName() + _T(".ShadowMap");				
				if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::PCF && m_ShadowFilterType < GraphicsSystem::CShadowMap::VSM)
				{
					if (m_lpEngine->GetlpGraphicsSystem()->m_Config.bSharedShadowMap)
						m_lpShadowMap = (GraphicsSystem::CShadowMap*)(lpR->GetlpMap(CRenderer::SHARED_SHADOWMAP));
					else
					{					
						m_lpShadowMap = LYNXNEW GraphicsSystem::CShadowMap(m_lpEngine->GetlpGraphicsSystem());
						if (!m_lpShadowMap)
							return LYNX_FALSE;
						
						if (!((GraphicsSystem::CShadowMap*)m_lpShadowMap)->Create(m_lpEngine->GetlpGraphicsSystem()->m_Config.ShadowMapDepthFormat, 1, Name.c_str(), m_ShadowmapWidth, m_ShadowmapHeight, m_lpEngine->GetlpGraphicsSystem()->m_Config.ShadowMapFormat))								
							return LYNX_FALSE;	
					}
				}
				else if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
				{
					m_lpShadowMap = LYNXNEW GraphicsSystem::CShadowMap(m_lpEngine->GetlpGraphicsSystem());
					if (!m_lpShadowMap)
						return LYNX_FALSE;
					if (!((GraphicsSystem::CShadowMap*)m_lpShadowMap)->Create(GraphicsSystem::CRenderer::FORMAT_D24S8, 1, Name.c_str(), m_ShadowmapWidth, m_ShadowmapHeight, GraphicsSystem::CRenderer::FORMAT_G16R16))								
						return LYNX_FALSE;	
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDirectionalLight::vComputeVisObjList() 
		{
			m_VisDynamicOpaqueObjList.clear();
			
			for (int i=0; i<m_lpScene->GetNumStaticObjs(); i++)							
			{	
				//if (m_lpScene->GetlpStaticObj(i)->GetlpContainer()->m_bCastShadow && m_lpScene->GetlpStaticObj(i)->GetlpContainer()->GetContainerType() != LynxEngine::CContainer::TERRAIN)
				if (m_lpScene->GetlpStaticObj(i)->GetlpContainer()->m_bCastShadow)
					m_VisDynamicOpaqueObjList.push_back(m_lpScene->GetlpStaticObj(i));				
			}			
			
			for (int i=0; i<m_lpScene->GetNumDynamicObjs(); i++)							
			{
				if (m_lpScene->GetlpDynamicObj(i)->GetlpContainer()->m_bCastShadow)
					m_VisDynamicOpaqueObjList.push_back(m_lpScene->GetlpDynamicObj(i));				
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDirectionalLight::vGenerateShadowMap(CCameraContainer* cam)
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);			
			Math::CMatrix4 TM, TM2, Orth;			
			CList<CRenderableObj*>::CIterator Obj;
			MaterialSystem::CMaterial::TECHTYPE Tech;		
			GraphicsSystem::CBuffer*	MapRTs[2];	
			GraphicsSystem::CBuffer*	RTs[4];	
			

			LYNXRECT ViewportRect;
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetViewportRect(&ViewportRect);

			if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
				Tech = MaterialSystem::CMaterial::VARIANCE_SHADOWMAP;
			else
				Tech = MaterialSystem::CMaterial::SHADOWMAP;	

			static Math::CMatrix4 ScaleM(0.5f,      0.0f,        0.0f,        0.0f,
										 0.0f,     -0.5f,        0.0f,        0.0f,
										 0.0f,      0.0f,        1.0f,        0.0f,
										 0.5f,	    0.5f,		 0.0f,		  1.0f);
			m_Camera->m_Near = 100.0f;//0.1f;
			m_Camera->m_Far = m_Radius;		 
			CalculateViewMatrix();			
			lynxOrthoProjectionMatrix(&Orth, m_lpShadowMap->GetWidth(), m_lpShadowMap->GetHeight(), m_Camera->m_Near, m_Camera->m_Far);						
			
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(MapRTs, 0, m_lpShadowMap);		
			RTs[1] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);		
			RTs[2] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);		
			RTs[3] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);		

			if (m_lpScene->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap > 1)
			{
				cam->CalculateFarCornerDir(((float)m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth())/((float)m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight()));
				for (int i=0; i<m_lpScene->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap; i++)				
				{
					//Switch back to normal VPM			
					m_Camera->SetProjectionMatrix(Orth);
					CollisionSystem::CAABB FrustumAABB;
					cam->CalculateFrustumAABB(FrustumAABB, m_lpScene->GetlpRenderer()->GetShadowMapSplittedDepth(i), m_lpScene->GetlpRenderer()->GetShadowMapSplittedDepth(i+1));
					Math::CMatrix4 CropMatrix = CalculateCropMatrix(FrustumAABB);
					CropMatrix.m13 = 0.0f;
					CropMatrix.m23 = 0.0f;
					CropMatrix.m33 = 1.0f;
					CropMatrix.m43 = 0.0f;
					TM = Orth * CropMatrix;
					m_Camera->SetProjectionMatrix(TM);					
					TM = m_Camera->GetViewMatrix()*m_Camera->GetProjectionMatrix();															
					if (i == m_lpScene->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap-1)
					{
						m_lpEngine->GetlpGraphicsSystem()->SetTextureAsRenderBuffer(0, m_lpShadowMap->GetTexture());						
						m_LightViewProj = TM*ScaleM;				
					}
					else
					{
						m_lpEngine->GetlpGraphicsSystem()->SetTextureAsRenderBuffer(0, m_lpShadowMap->GetUtilTexture(i));
						m_SplittedLightViewProj[i] = TM*ScaleM;				
					}
					
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpShadowMap->GetWidth(), m_lpShadowMap->GetHeight());				
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);	

					for (Obj = m_VisDynamicOpaqueObjList.begin(); Obj != m_VisDynamicOpaqueObjList.end(); ++Obj)
					{
						(*Obj)->SetCurrentMaterialTechnique(Tech);
						(*Obj)->Render(m_Camera);
					}
				}				
			}
			else
			{
				m_Camera->SetProjectionMatrix(Orth);
				lynxMatrixXMatrix(&TM, &m_Camera->GetViewMatrix(), &m_Camera->GetProjectionMatrix());		
				lynxMatrixXMatrix(&m_LightViewProj, &TM, &ScaleM);

				m_lpEngine->GetlpGraphicsSystem()->SetTextureAsRenderBuffer(0, m_lpShadowMap->GetTexture());												
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpShadowMap->GetWidth(), m_lpShadowMap->GetHeight());				
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);	

				for (Obj = m_VisDynamicOpaqueObjList.begin(); Obj != m_VisDynamicOpaqueObjList.end(); ++Obj)
				{
					(*Obj)->SetCurrentMaterialTechnique(Tech);
					(*Obj)->Render(m_Camera);
				}			
			}			
			
			if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
			{
				// Variance shadow map
				m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(1, NULL);			
				m_lpShadowMap->DownSampled();
				m_lpShadowMap->BoxBlur(6, 0.5f);			
			}
			m_bShadowmapDirty = LYNX_FALSE;

			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(0, MapRTs[0]);
			m_lpEngine->GetlpGraphicsSystem()->SetDepthBuffer(MapRTs[1]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(1, RTs[1]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(2, RTs[2]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(3, RTs[3]);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(ViewportRect.x1, ViewportRect.y1, ViewportRect.x2, ViewportRect.y2);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDirectionalLight::vGenerateShadowTexture()
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);			
			Math::CMatrix4 TM, TM2, Orth;			
			CList<CRenderableObj*>::CIterator Obj;
			MaterialSystem::CMaterial::TECHTYPE Tech;		
			GraphicsSystem::CBuffer*	MapRTs[2];	
			GraphicsSystem::CBuffer*	RTs[4];				

			LYNXRECT ViewportRect;
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetViewportRect(&ViewportRect);
			Tech = MaterialSystem::CMaterial::SHADOW_TEXTURE;	
			static Math::CMatrix4 ScaleM(0.5f,      0.0f,        0.0f,        0.0f,
										 0.0f,     -0.5f,        0.0f,        0.0f,
										 0.0f,      0.0f,        1.0f,        0.0f,
										 0.5f,	    0.5f,		 0.0f,		  1.0f);
			
			m_Camera->m_Near = 50.0f;//0.1f;
			m_Camera->m_Far = 600.0f;//m_Radius;		 
			CalculateViewMatrix();			
			lynxOrthoProjectionMatrix(&Orth, m_lpShadowMap->GetWidth(), m_lpShadowMap->GetHeight(), m_Camera->m_Near, m_Camera->m_Far);						
			//m_Camera->SetProjectionMatrix(Orth);
			m_Camera->UpdateProjectionMatrix(1.0f);//
			lynxMatrixXMatrix(&TM, &m_Camera->GetViewMatrix(), &m_Camera->GetProjectionMatrix());		
			lynxMatrixXMatrix(&m_LightViewProj, &TM, &ScaleM);
			
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(MapRTs, 0, m_lpShadowMap);		
			RTs[1] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);		
			RTs[2] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);		
			RTs[3] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);		
			
			m_lpEngine->GetlpGraphicsSystem()->SetTextureAsRenderBuffer(0, m_lpShadowMap->GetTexture());												
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpShadowMap->GetWidth(), m_lpShadowMap->GetHeight());				
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);	

			for (Obj = m_VisDynamicOpaqueObjList.begin(); Obj != m_VisDynamicOpaqueObjList.end(); ++Obj)
			{
				(*Obj)->SetCurrentMaterialTechnique(Tech);
				LYNXCOLORRGBA OldDiffuseColor = (*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse;
				(*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse.Red = 0;
				(*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse.Green = 0;
				(*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse.Blue = 0;
				(*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse.Alpha = 255;				
				(*Obj)->Render(m_Camera);
				(*Obj)->GetlpContainer()->GetlpCurrentMaterial()->m_Diffuse = OldDiffuseColor;
			}
			
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(1, NULL);			
			m_lpShadowMap->DownSampled();
			m_lpShadowMap->BoxBlur(16, 2.0f);
			
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(0, MapRTs[0]);
			m_lpEngine->GetlpGraphicsSystem()->SetDepthBuffer(MapRTs[1]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(1, RTs[1]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(2, RTs[2]);
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(3, RTs[3]);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(ViewportRect.x1, ViewportRect.y1, ViewportRect.x2, ViewportRect.y2);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDirectionalLight::vSetShaderParam(CCameraContainer* cam, CContainer* lpc)
		{
			CVector3 Pos;
			float TexelGrad[2];
			float Color[3];
			LYNXMATRIX WLPM, WLM;

			GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

			cam->vGetPosition(Pos);
			lpR->GetCachedVertexShader()->vSetConstantF("gCameraPos", &Pos.x, 3);
			lynxMatrixXMatrix(&WLPM, &lpc->GetTransformMatrix(), &m_LightViewProj);			
			lynxMatrixXMatrix(&WLM, &lpc->GetTransformMatrix(), &GetCamera().GetViewMatrix());							
			lpR->GetCachedVertexShader()->vSetConstantM("gWorldLightViewProj", &WLPM);				
			lpR->GetCachedVertexShader()->vSetConstantM("gWorldLightView", &WLM);					
			GetPosition(Pos);
			lpR->GetCachedVertexShader()->vSetConstantF("gLightPos", &Pos.x, 3);
			
			if (lpc->m_bReceiveShadow)
			{
				lpR->GetCachedPixelShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP).c_str(), GetShadowMap().GetCurrent());								
				/*lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
				lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
				if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && 
					m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
					lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
				else
					lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_POINT, LYNX_TF_POINT, LYNX_TF_POINT); 				
				lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_CF_MODULATE, LYNX_CA_CURRENT, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);
				lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL+1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													*/

				TexelGrad[0] = 1.0f/(float)(GetShadowMap().GetWidth());
				TexelGrad[1] = 1.0f/(float)(GetShadowMap().GetHeight());
				lpR->GetCachedPixelShader()->vSetConstantF("gShadowmapTexelGrad", TexelGrad, 2);
			}
			Color[0] = m_Color.Red/255.0f;
			Color[1] = m_Color.Green/255.0f;
			Color[2] = m_Color.Blue/255.0f;
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Color", Color, 3);
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Intensity", m_Intensity);
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Radius", m_Radius);
			GetDirection(Pos);
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Dir", &Pos.x, 3);						
			lpR->GetCachedPixelShader()->vSetConstantF("gZBias", m_DepthBias);
			lpR->GetCachedPixelShader()->vSetConstantI("gShadowFilterType", m_ShadowFilterType);

			lpR->GetCachedPixelShader()->vSetConstantB("gbSelfIlluminated", lpc->m_bSelfIlluminated);
			lpR->GetCachedPixelShader()->vSetConstantF("gIlluminatingIntensity", lpc->m_IlluminatingIntensity);					
			lpR->GetCachedPixelShader()->vSetConstantF("gSpecularPower", lpc->GetlpCurrentMaterial()->m_SpPower);								

			lpR->GetCachedPixelShader()->vSetConstantB("gbShadowing", lpc->m_bReceiveShadow);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDirectionalLight::vDeferredLighting(CCameraContainer* cam) 
		{
			CDeferredRenderer* lpR = static_cast<CDeferredRenderer*>(m_lpScene->GetlpRenderer());
			GraphicsSystem::CGraphicsSystem* lpG = m_lpEngine->GetlpGraphicsSystem();											
			lpG->SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(&(lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::DIRECTIONAL_LIGHTING)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), *cam);						
			lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::DIRECTIONAL_LIGHTING)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(m_lpContainer, cam);
			lpG->DrawScreenSpaceQuad();								
		}
	}
}