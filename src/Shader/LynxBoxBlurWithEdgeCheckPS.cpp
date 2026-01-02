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
#include <Shader/LynxBoxBlurWithEdgeCheckPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxBlurWithEdgeCheckPS::CBoxBlurWithEdgeCheckPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_DepthThresholdValue = 0.3f;
			m_DepthThreshold = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxBlurWithEdgeCheckPS::~CBoxBlurWithEdgeCheckPS(void)
		{
			if (m_DepthThreshold)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_DepthThreshold);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBoxBlurWithEdgeCheckPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/LynxBoxBlurWithEdgeCheck.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBoxBlurWithEdgeCheckPS::vCreateConstantAndResource() 
		{ 
			if (!m_DepthThreshold)
				m_DepthThreshold = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gDepthThreshold");

			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBoxBlurWithEdgeCheckPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			SceneSystem::CRenderer* lpSR = m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer();			

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));

			m_lpShader->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str(), lpSR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetCurrent());			
			m_DepthThreshold->vSetFloat(m_DepthThresholdValue);
		}		
	}
}
