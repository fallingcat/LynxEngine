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
#include <Shader/LynxPostMotionBlurPS.h>
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
		CPostMotionBlurPS::CPostMotionBlurPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_BlurScale_Const = NULL;
			m_BlurKernel_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostMotionBlurPS::~CPostMotionBlurPS(void)
		{
			if (m_BlurScale_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_BlurScale_Const);
			if (m_BlurKernel_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_BlurKernel_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPostMotionBlurPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/post_motion_blur.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostMotionBlurPS::vCreateConstantAndResource() 
		{ 
			if (!m_BlurScale_Const)
				m_BlurScale_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gBlurScale");
			if (!m_BlurKernel_Const)
				m_BlurKernel_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gBlurKernel");			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostMotionBlurPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			
			m_BlurScale_Const->vMap();
				if (cam->m_bCameraMovingBlur)
				{
					m_BlurScale_Const->vSetFloat(cam->m_CameraMovingBlurScale);					
					m_BlurKernel_Const->vSetInt(cam->m_CameraMovingBlurKernel);					
				}
				else
				{
					m_BlurScale_Const->vSetFloat(cam->m_MotionBlurScale);					
					m_BlurKernel_Const->vSetInt(cam->m_MotionBlurKernel);					
				}			
			m_BlurScale_Const->vUnmap();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	
		}		
	}
}
