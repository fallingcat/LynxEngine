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
#include <GraphicsSystem/LynxOffscreen.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxDeferredAmbientLightingPS.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <SceneSystem/Light/LynxAmbientLight.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredAmbientLightingPS::CDeferredAmbientLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_LightData_Const = NULL;
			m_bHDRRendering_Const = NULL;
			m_Exposure_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredAmbientLightingPS::~CDeferredAmbientLightingPS(void)
		{
			if (m_LightData_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightData_Const);
			if (m_bHDRRendering_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_bHDRRendering_Const);
			if (m_Exposure_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Exposure_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDeferredAmbientLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/def_ambientlight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredAmbientLightingPS::vCreateConstantAndResource() 
		{ 
			if (!m_LightData_Const)
				m_LightData_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLight");
			if (!m_bHDRRendering_Const)
				m_bHDRRendering_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbHDRRendering");
			if (!m_Exposure_Const)
				m_Exposure_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gExposure");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredAmbientLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			m_lpShader->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), lpR->GetlpGBuffer(SceneSystem::CDeferredRenderer::DIFFUSE)->GetTexture());			

			SceneSystem::CAmbientLight* lpAL = ((SceneSystem::CAmbientLight*)(lpc->GetlpRenObj()));

			m_LightData_Const->vMap();
				LIGHTDATA  Data;
				Data.Color[0] = lpAL->m_Color.Red/255.0f;
				Data.Color[1] = lpAL->m_Color.Green/255.0f;
				Data.Color[2] = lpAL->m_Color.Blue/255.0f;
				Data.Intensity = lpAL->m_Intensity;
				m_LightData_Const->vSetFloat((float*)&Data, sizeof(LIGHTDATA)/sizeof(float));			
				m_bHDRRendering_Const->vSetBool(m_lpGraphicsSystem->m_Config.bHDRRendering);
				m_Exposure_Const->vSetFloat(cam->m_Exposure);
			m_LightData_Const->vUnmap();

			m_lpGraphicsSystem->GetlpRenderer()->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		
		}		
	}
}
