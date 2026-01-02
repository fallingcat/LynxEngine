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
#include <Shader/LynxSpotLightingPS.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpotLightingPS::CSpotLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpotLightingPS::~CSpotLightingPS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpotLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/spotlight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpotLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		
			//m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->vSetShaderParam(cam, lpc);

			CVector3 Pos;
			float TexelGrad[2];
			float Color[3];
			LYNXMATRIX WLPM, WLM;

			SceneSystem::CSpotLight* lpSL = (SceneSystem::CSpotLight*)m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
			if (lpc->m_bReceiveShadow)
			{
				m_lpShader->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP).c_str(), lpSL->GetShadowMap().GetCurrent());												
				/*lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
				lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
				if (m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && 
					m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
					lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
				else
					lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_POINT, LYNX_TF_POINT, LYNX_TF_POINT); 				
				lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_CF_MODULATE, LYNX_CA_CURRENT, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);
				lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL+1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													*/

				TexelGrad[0] = 1.0f/(float)(lpSL->GetShadowMap().GetWidth());
				TexelGrad[1] = 1.0f/(float)(lpSL->GetShadowMap().GetHeight());
				m_lpShader->vSetConstantF("gShadowmapTexelGrad", TexelGrad, 2);
			}
			Color[0] = lpSL->m_Color.Red/255.0f;
			Color[1] = lpSL->m_Color.Green/255.0f;
			Color[2] = lpSL->m_Color.Blue/255.0f;
			m_lpShader->vSetConstantF("gLight.Color", Color, 3);
			m_lpShader->vSetConstantF("gLight.Intensity", lpSL->m_Intensity);
			m_lpShader->vSetConstantF("gLight.Radius", lpSL->m_Radius);
			float Rad = lpSL->m_Deg*LYNXD2R;
			m_lpShader->vSetConstantF("gLight.Rad", Rad);								
			Rad = lpSL->m_FadeOutDeg*LYNXD2R;
			m_lpShader->vSetConstantF("gLight.FadeOutRad", Rad);								
			lpSL->GetDirection(Pos);
			m_lpShader->vSetConstantF("gLight.Dir", &Pos.x, 3);						
			m_lpShader->vSetConstantF("gZBias", lpSL->m_DepthBias);
			m_lpShader->vSetConstantI("gShadowFilterType", lpSL->m_ShadowFilterType);

			m_lpShader->vSetConstantB("gbSelfIlluminated", lpc->m_bSelfIlluminated);
			m_lpShader->vSetConstantF("gIlluminatingIntensity", lpc->m_IlluminatingIntensity);					
			m_lpShader->vSetConstantF("gSpecularPower", lpc->GetlpCurrentMaterial()->m_SpPower);								

			m_lpShader->vSetConstantB("gbShadowing", lpc->m_bReceiveShadow);		
		}		
	}
}
