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
#include <Shader/LynxTintPS.h>
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
		CTintPS::CTintPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_TintColor_Const = NULL;
			m_TintColor.PackedColor = 0xffffffff;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTintPS::~CTintPS(void)
		{
			if (m_TintColor_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_TintColor_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTintPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/lynxTint.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTintPS::vCreateConstantAndResource() 
		{ 
			if (!m_TintColor_Const)
				m_TintColor_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gTintColor");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTintPS::SetParam() 
		{ 			
			m_TintColor_Const->vMap();
				float Color[4];
				Color[0] = m_TintColor.Red / 255.0f;
				Color[1] = m_TintColor.Green / 255.0f;
				Color[2] = m_TintColor.Blue / 255.0f;
				Color[3] = m_TintColor.Alpha / 255.0f;
				m_TintColor_Const->vSetVector4(Color, 1);
			m_TintColor_Const->vUnmap();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTintPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			SetParam();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));	
		}		
	}
}
