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
#include <Shader/LynxPostAverageLuminancePS.h>
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
		CPostAverageLuminancePS::CPostAverageLuminancePS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_Param = NULL;
			m_Offset_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPostAverageLuminancePS::~CPostAverageLuminancePS(void)
		{
			if (m_Param)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstantGroup(m_Param);

			if (m_Offset_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Offset_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPostAverageLuminancePS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/post_averageluminance.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostAverageLuminancePS::vCreateConstantAndResource() 
		{ 
			if (!m_Param)			
			{
				m_Param = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstantGroup(m_lpShader, "Param");
				m_Param->vSetUpdateFrequence(GraphicsSystem::CRenderer::CUF_WHEN_DIRTY);
			}

			if (!m_Offset_Const)
				m_Offset_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gOffset");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostAverageLuminancePS::SetParam() 
		{ 			
			m_Param->vMap();
				m_Offset_Const->vSetVector2((float*)m_SampleOffset, 16);
			m_Param->vUnmap();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPostAverageLuminancePS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			switch (m_Param->GetUpdateFrequence())
			{
				case GraphicsSystem::CRenderer::CUF_PER_FRAME:			
					SetParam();
					break;

				case GraphicsSystem::CRenderer::CUF_WHEN_DIRTY:
					if (m_Param->IsDirty())
					{
						SetParam();
						m_Param->SetDirty(LYNX_FALSE);
					}
					break;
			}

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	
		}		
	}
}
