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
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/Light/LynxSpotLight.h>
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

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CSpotLight, CLight)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpotLight::CSpotLight(CScene* const lpscene)
		: CLight(lpscene)
		{
			m_LightType = SPOT;

			INIT_CLASSSCRIPT

			IMPLEMENT_PARAM_FLOAT(m_Deg, 0)
			IMPLEMENT_PARAM_FLOAT(m_FadeOutDeg, 0)
			IMPLEMENT_PARAM_BOOL_COMMENT(m_bProjectedLightMap, _T("Does this light use projected lightmap or not"), 0)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpotLight::~CSpotLight(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpotLight::vCreate(int w, int h)
		{		
			LYNX_ASSERT(m_lpScene);

			if (m_bCastShadow)
			{
				if (!CreateShadowmap(w, h))
					return LYNX_FALSE;									
			}

			if (!CLight::vCreate())
				return LYNX_FALSE;

			CPyramidProxy* pProxy = LYNXNEW CPyramidProxy(m_lpEngine);
			m_lpContainer->GetProxyList().push_back(pProxy);
			pProxy->SetParent(this, NULL);
			pProxy->SetName(m_Name + CString(_T(".Proxy00")));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpotLight::vComputeProxy()
		{
			Math::CVector3 V[5];
			int I[18] = { 0, 1, 2,
						  0, 2, 4,
						  0, 4, 3,
						  0, 3, 1,
						  3, 2, 1,
						  3, 4, 2 };
			float R;

			R = m_Radius*tanf(m_Deg*LYNXD2R);
			V[0] = CVector3(0.0f, 0.0f, 0.0f);
			V[1] = CVector3(-R,  R, m_Radius);
			V[2] = CVector3( R,  R, m_Radius);
			V[3] = CVector3(-R, -R, m_Radius);
			V[4] = CVector3( R, -R, m_Radius);
			(*(m_lpContainer->GetProxyList().begin()))->vCreate(5, V, 18, I);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpotLight::CreateShadowmap(int w, int h)
		{		
			LYNX_ASSERT(m_lpScene);
			
			if (w != m_ShadowmapWidth || h !=  m_ShadowmapHeight)
			{
				if (m_lpShadowMap)
					LYNXDEL m_lpShadowMap;

				m_ShadowmapWidth = w;
				m_ShadowmapHeight = h;

				CString Name = GetName() + _T(".ShadowMap");
				if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::PCF && m_ShadowFilterType < GraphicsSystem::CShadowMap::VSM)
				{
					if (m_lpEngine->GetlpGraphicsSystem()->m_Config.bSharedShadowMap)
						m_lpShadowMap = (GraphicsSystem::CShadowMap*)(m_lpScene->GetlpRenderer()->GetlpMap(CRenderer::SHARED_SHADOWMAP));
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
		void CSpotLight::vComputeVisObjList() 
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
		void CSpotLight::vGenerateShadowMap(CCameraContainer* cam)
		{
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);			
			LYNXMATRIX TM, TM2;
			float RatioW, RatioH;

			if (m_ShadowmapWidth < m_lpShadowMap->GetWidth())
			{
				m_ActualShadowMapWidth = m_ShadowmapWidth;
				m_ActualShadowMapHeight = m_ShadowmapHeight;
			}
			else
			{
				m_ActualShadowMapWidth = m_lpShadowMap->GetWidth();
				m_ActualShadowMapHeight = m_lpShadowMap->GetHeight();
			}

			RatioW = (float)m_ActualShadowMapWidth/(float)m_lpShadowMap->GetWidth();
			RatioH = (float)m_ActualShadowMapHeight/(float)m_lpShadowMap->GetHeight();

			Math::CMatrix4 ScaleM(0.5f*RatioW,	0.0f,		  0.0f,	0.0f,
								  0.0f,			-0.5f*RatioH, 0.0f, 0.0f,
								  0.0f,			0.0f,		  1.0f, 0.0f,
								  0.5f*RatioW,	0.5f*RatioH,  0.0f,	1.0f);
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

			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(MapRTs, 0, m_lpShadowMap);
			RTs[1] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);		
			RTs[2] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);		
			RTs[3] = m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);		

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_ActualShadowMapWidth, m_ActualShadowMapHeight);				
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);	

			CalculateViewMatrix();			
			m_Camera->m_Near = 1.0f;
			m_Camera->m_Far = m_Radius;
			m_Camera->SetFOV(m_Deg*2.0f, m_Deg*2.0f);
			m_Camera->UpdateProjectionMatrix(1.0f);
			lynxMatrixXMatrix(&TM, &m_Camera->GetViewMatrix(), &m_Camera->GetProjectionMatrix());									
			lynxMatrixXMatrix(&m_LightViewProj, &TM, &ScaleM);					

			for (Obj = m_VisDynamicOpaqueObjList.begin(); Obj != m_VisDynamicOpaqueObjList.end(); ++Obj)
			{
				(*Obj)->SetCurrentMaterialTechnique(Tech);
				(*Obj)->vRender(m_Camera);
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
		void CSpotLight::vSetShaderParam(CCameraContainer* cam, CContainer* lpc)
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
			float Rad = m_Deg*LYNXD2R;
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Rad", Rad);								
			Rad = m_FadeOutDeg*LYNXD2R;
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.FadeOutRad", Rad);								
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
		void CSpotLight::vDeferredLighting(CCameraContainer* cam) 
		{			
			CDeferredRenderer* lpR = static_cast<CDeferredRenderer*>(m_lpScene->GetlpRenderer());
			GraphicsSystem::CGraphicsSystem* lpG = m_lpEngine->GetlpGraphicsSystem();		
			//cam->CalculateFarCornerDir(((float)lpG->GetlpOffscreen()->GetTexture()->vGetWidth())/((float)lpG->GetlpOffscreen()->GetTexture()->vGetHeight()));			
			lpG->SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(&(lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::SPOT_LIGHTING)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), *cam);						
			lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::SPOT_LIGHTING)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(m_lpContainer, cam);	

			/*
			ComputeScreenSpaceAABB(cam);
			lpG->EnableScissorRect(LYNX_TRUE);
			LYNXRECT Rect;
			lpG->ConvertScreenSpaceToScissorSpace(&Rect, m_ScreenSpaceAABB.Min().x, m_ScreenSpaceAABB.Min().y, m_ScreenSpaceAABB.Max().x, m_ScreenSpaceAABB.Max().y);
			lpG->SetScissorRect(Rect.x1, Rect.y1, Rect.x2, Rect.y2);
			*/

			lpG->DrawScreenSpaceQuad();								
			
			/*
			ComputeScreenSpaceAABB(cam);			
			if (!(m_ScreenSpaceAABB.Min().x >= 1.0f || m_ScreenSpaceAABB.Max().x <= -1.0f || m_ScreenSpaceAABB.Min().y >= 1.0f || m_ScreenSpaceAABB.Max().y <= -1.0f))
			{
				float x1 = LYNX_MAX(m_ScreenSpaceAABB.Min().x, -1.0f);
				float y1 = LYNX_MAX(m_ScreenSpaceAABB.Min().y, -1.0f);
				float x2 = LYNX_MIN(m_ScreenSpaceAABB.Max().x,  1.0f);
				float y2 = LYNX_MIN(m_ScreenSpaceAABB.Max().y,  1.0f);								
				lpG->DrawScreenSpaceDepthQuad(*cam, x1, y1, x2, y2);								
			}
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSpotLight::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSpotLight::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			return LYNX_TRUE;
		}
	}
}