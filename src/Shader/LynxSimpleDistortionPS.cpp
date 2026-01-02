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
#include <Shader/LynxSimpleDistortionPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleDistortionPS::CSimpleDistortionPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{
			m_DistortionParam_Const = NULL;
			m_ConstantColor_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleDistortionPS::~CSimpleDistortionPS(void)
		{
			if (m_DistortionParam_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_DistortionParam_Const);

			if (m_ConstantColor_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ConstantColor_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSimpleDistortionPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxSimpleDistortion.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSimpleDistortionPS::vCreateConstantAndResource() 
		{ 
			if (!m_DistortionParam_Const)
				m_DistortionParam_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gDistortionParam");

			if (!m_ConstantColor_Const)
				m_ConstantColor_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gOutputColor");

			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::REFRACTIONMAP).c_str());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSimpleDistortionPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		

			m_DistortionParam.ScreenSize[0]		= lpR->GetCachedRenderBuffer(0)->vGetWidth();
			m_DistortionParam.ScreenSize[1]		= lpR->GetCachedRenderBuffer(0)->vGetHeight();
			m_DistortionParam.RefractionIndex	= lpc->GetlpCurrentMaterial()->m_RefractionIndex;			
			m_DistortionParam.RefractionOffset	= lpc->GetlpCurrentMaterial()->m_RefractionOffest;
			m_DistortionParam_Const->vSetVector4((float*)&m_DistortionParam);

			if (lpc)
			{
				float Color[4];
				Color[0] = lpc->GetlpCurrentMaterial()->m_Diffuse.Red/255.0f;
				Color[1] = lpc->GetlpCurrentMaterial()->m_Diffuse.Green/255.0f;
				Color[2] = lpc->GetlpCurrentMaterial()->m_Diffuse.Blue/255.0f;
				Color[3] = lpc->GetlpCurrentMaterial()->m_Diffuse.Alpha/255.0f;
				m_ConstantColor_Const->vSetVector4(Color);
			}			
		}		
	}
}
