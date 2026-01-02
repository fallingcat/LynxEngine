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
#include <Shader/LynxPostDOFPS.h>
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
		CPostDOFPS::CPostDOFPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_FocalRange_Const = NULL;
			m_Kenel_Const = NULL;
			m_ProjMatrix_Const = NULL;
			m_ScreenSize_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostDOFPS::~CPostDOFPS(void)
		{
			if (m_FocalRange_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_FocalRange_Const);
			if (m_Kenel_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Kenel_Const);
			if (m_ProjMatrix_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ProjMatrix_Const);
			if (m_ScreenSize_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ScreenSize_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPostDOFPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/post_dof.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostDOFPS::vCreateConstantAndResource() 
		{ 
			if (!m_FocalRange_Const)
				m_FocalRange_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gFocalRange");
			if (!m_Kenel_Const)
				m_Kenel_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gKernel");
			if (!m_ProjMatrix_Const)
				m_ProjMatrix_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gProjM");
			if (!m_ScreenSize_Const)
				m_ScreenSize_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gScreenQuadSize");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostDOFPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			m_FocalRange_Const->vMap();
				m_FocalRange_Const->vSetVector4((float*)cam->m_FocalRange);			
				m_Kenel_Const->vSetFloat(cam->m_DOFBlurKernel);
				m_ProjMatrix_Const->vSetMatrix(&(cam->GetProjectionMatrix()));

				float ScreenQuadSize[2];						
				ScreenQuadSize[0] = 3.0f/((float)(lpR->GetCachedRenderBuffer(0)->vGetWidth()));
				ScreenQuadSize[1] = 3.0f/((float)(lpR->GetCachedRenderBuffer(0)->vGetHeight()));
				m_ScreenSize_Const->vSetVector2(ScreenQuadSize);	
			m_FocalRange_Const->vUnmap();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	
		}		
	}
}
