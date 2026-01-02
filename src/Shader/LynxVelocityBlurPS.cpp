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
#include <Shader/LynxVelocityBlurPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVelocityMapBlurPS::CVelocityMapBlurPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_BlurDir.x = 0.0f;
			m_BlurDir.y = 1.0f;
			m_Distance = 0.1f;
			m_Weight = 1.0f;			
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVelocityMapBlurPS::~CVelocityMapBlurPS(void)
		{
			if (m_BlurDir_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_BlurDir_Const);			
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CVelocityMapBlurPS::vCreate() 
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
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxDirectionalBlur.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVelocityMapBlurPS::vCreateConstantAndResource() 
		{ 
			if (!m_BlurDir_Const)
				m_BlurDir_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gBlurDir");			

			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVelocityMapBlurPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			float Data[4];
			m_BlurDir_Const->vMap();
				Data[0] = m_BlurDir.x;
				Data[1] = m_BlurDir.y;
				Data[2] = m_Distance;
				Data[3] = m_Weight;
				m_BlurDir_Const->vSetVector4(Data);	
			m_BlurDir_Const->vUnmap();
			
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
