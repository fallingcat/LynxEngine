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
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxPostSSAOLQPS.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxMaterialSystem.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostSSAOLQPS::CPostSSAOLQPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostSSAOLQPS::~CPostSSAOLQPS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPostSSAOLQPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/post_SSAO_LQ.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostSSAOLQPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			LYNXMATRIX ProjM;
			static Math::CMatrix4 ScaleM(0.5f,	0.0f,  0.0f,  0.0f,
										 0.0f, -0.5f,  0.0f,  0.0f,
										 0.0f,  0.0f,  1.0f,  0.0f,
										 0.5f,	0.5f,  0.0f,  1.0f);						

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			SceneSystem::CDeferredRenderer* lpSR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	

			m_lpShader->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str(), lpSR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetCurrent());			
			float NoiseTile[2];
			NoiseTile[0] = lpSR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetTexture()->vGetWidth()/m_lpGraphicsSystem->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetWidth();
			NoiseTile[1] = lpSR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetTexture()->vGetHeight()/m_lpGraphicsSystem->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetHeight();
			lynxMatrixXMatrix(&ProjM, &cam->GetProjectionMatrix(), &ScaleM);	
			m_lpShader->vSetConstantM("gProjM", &ProjM);				
			m_lpShader->vSetConstantF("gKernel", m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.SSAOSamplingKernel);	
			m_lpShader->vSetConstantF("gThreshold", m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.SSAOThreshold);
			if (m_lpGraphicsSystem->m_Config.SSAOQuality <= 1)
			{
				float RTSize[2] = {(float)lpSR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetCurrent()->vGetWidth(), (float)lpSR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetCurrent()->vGetHeight()};
				m_lpShader->vSetConstantF("gRTSize", RTSize, 2);					
			}
		}		
	}
}
