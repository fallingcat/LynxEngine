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
#include <Shader/LynxRadialBlurPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRadialBlurPS::CRadialBlurPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_RadialCenter_Const = NULL;
			m_RadialCenter.u = 0.5f;
			m_RadialCenter.v = 0.5f;
			m_Distance = 0.5f;
			m_Weight = 1.0f;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRadialBlurPS::~CRadialBlurPS(void)
		{
			if (m_RadialCenter_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_RadialCenter_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRadialBlurPS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

            m_ShaderMacros.clear();

			#ifdef __DESKTOP__            
				ShaderMacro.Name = CAnsiString("NUM_SAMPLES");
				ShaderMacro.Definition = CAnsiString("32");
				m_ShaderMacros.push_back(ShaderMacro);
				//m_Distance = 0.6f;
				//m_Weight = 1.5f;
			#elif defined __MOBILE__
				ShaderMacro.Name = CAnsiString("NUM_SAMPLES");
				ShaderMacro.Definition = CAnsiString("8");
				m_ShaderMacros.push_back(ShaderMacro);
				//m_Distance = 0.5f;
			#endif
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxRadialBlur.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRadialBlurPS::vCreateConstantAndResource() 
		{ 
			if (!m_RadialCenter_Const)
				m_RadialCenter_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gRadialCenter");

			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRadialBlurPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			float Data[4];
			m_RadialCenter_Const->vMap();
				Data[0] = m_RadialCenter.u;
				Data[1] = m_RadialCenter.v;
				Data[2] = m_Distance;
				Data[3] = m_Weight;
				m_RadialCenter_Const->vSetVector4(Data);
			m_RadialCenter_Const->vUnmap();
			
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
