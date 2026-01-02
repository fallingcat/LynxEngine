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
#include <Shader/LynxBilateralBlurPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBilateralBlurPS::CBilateralBlurPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_BlurParam[0] = 7.0f;
			m_BlurParam[1] = 1.0f/32.0f;
			m_BlurParam[2] = 160.0f;
			m_BlurParam_Const = NULL;
			m_UVOffsetDir_Const = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBilateralBlurPS::~CBilateralBlurPS(void)
		{
			if (m_BlurParam_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_BlurParam_Const);
			if (m_UVOffsetDir_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_UVOffsetDir_Const);	
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBilateralBlurPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxBilateralBlur.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBilateralBlurPS::vCreateConstantAndResource() 
		{ 
			if (!m_BlurParam_Const)
				m_BlurParam_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "g_BlurParam");
			if (!m_UVOffsetDir_Const)
				m_UVOffsetDir_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gUVOffsetDir");

			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str());						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBilateralBlurPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			
			SceneSystem::CRenderer* lpSR = m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer();			

			m_lpShader->GetResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str())->Set(lpSR->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetCurrent());	

			m_BlurParam_Const->vMap();
				m_BlurParam_Const->vSetVector4(m_BlurParam);
				m_UVOffsetDir_Const->vSetVector2((float*)(&m_UVOffsetDir));
			m_BlurParam_Const->vUnmap();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
