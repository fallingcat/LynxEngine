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
#include <Shader/LynxPostSSAOPS.h>
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
		CPostSSAOPS::CPostSSAOPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_SSAOParam = NULL;
			m_ProjMatrix_Const = NULL;
			m_Kernel_Const = NULL;
			m_NoiseTile_Const = NULL;
			m_Threahold_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostSSAOPS::~CPostSSAOPS(void)
		{
			if (m_SSAOParam)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstantGroup(m_SSAOParam);

			if (m_ProjMatrix_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ProjMatrix_Const);
			if (m_Kernel_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Kernel_Const);
			if (m_NoiseTile_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_NoiseTile_Const);
			if (m_Threahold_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Threahold_Const);			
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPostSSAOPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/post_SSAO.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostSSAOPS::vCreateConstantAndResource() 
		{ 
			if (!m_SSAOParam)			
			{
				m_SSAOParam = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstantGroup(m_lpShader, "SSAOParam");
				m_SSAOParam->vSetUpdateFrequence(GraphicsSystem::CRenderer::CUF_WHEN_DIRTY);
			}

			if (!m_ProjMatrix_Const)
				m_ProjMatrix_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gProjM");
			if (!m_Kernel_Const)
				m_Kernel_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gKernel");
			if (!m_NoiseTile_Const)
				m_NoiseTile_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gNoiseTile");
			if (!m_Threahold_Const)
				m_Threahold_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gThreshold");						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostSSAOPS::UpdateConstants(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			LYNXMATRIX ProjM;
			static Math::CMatrix4 ScaleM(0.5f,	0.0f,  0.0f,  0.0f,
										 0.0f, -0.5f,  0.0f,  0.0f,
										 0.0f,  0.0f,  1.0f,  0.0f,
										 0.5f,	0.5f,  0.0f,  1.0f);		

			SceneSystem::CDeferredRenderer* lpR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());
			m_SSAOParam->vMap();
				float NoiseTile[2];
				NoiseTile[0] = lpR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetTexture()->vGetWidth()/m_lpGraphicsSystem->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetWidth();
				NoiseTile[1] = lpR->GetlpMap(SceneSystem::CRenderer::SSAOMAP)->GetTexture()->vGetHeight()/m_lpGraphicsSystem->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetHeight();				
				m_Threahold_Const->vSetFloat(m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.SSAOThreshold);				
				m_Kernel_Const->vSetFloat(m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.SSAOSamplingKernel);	
				m_NoiseTile_Const->vSetVector2(NoiseTile);		

				lynxMatrixXMatrix(&ProjM, &cam->GetProjectionMatrix(), &ScaleM);	
				m_ProjMatrix_Const->vSetMatrix(&ProjM);					
			m_SSAOParam->vUnmap();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostSSAOPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			SceneSystem::CDeferredRenderer* lpSR = dynamic_cast<SceneSystem::CDeferredRenderer*>(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer());

			m_lpShader->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str(), lpSR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetCurrent());
			m_lpShader->vSetTexture("NoiseMap", m_lpGraphicsSystem->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE));
			//m_lpGraphicsSystem->GetlpRenderer()->SetTexture(0, m_lpGraphicsSystem->GetlpDepthMap()->GetCurrent());
			//m_lpGraphicsSystem->GetlpRenderer()->SetTexture(2, m_lpGraphicsSystem->GetNoiseTexture());			
			
			switch (m_SSAOParam->GetUpdateFrequence())
			{
				case GraphicsSystem::CRenderer::CUF_PER_FRAME:			
					UpdateConstants(lpc, cam);
					break;

				case GraphicsSystem::CRenderer::CUF_WHEN_DIRTY:
					if (m_SSAOParam->IsDirty())
					{
						UpdateConstants(lpc, cam);
						m_SSAOParam->SetDirty(LYNX_FALSE);
					}
					break;
			}

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	
		}		
	}
}
