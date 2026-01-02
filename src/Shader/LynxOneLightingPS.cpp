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
#include <Shader/LynxOneLightingPS.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COneLightingPS::COneLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{
			m_AmbientColorConst = NULL;
			m_AmbientColor[0] = 1.0f;
			m_AmbientColor[1] = 1.0f;
			m_AmbientColor[2] = 1.0f;
			m_AmbientColor[3] = 1.0f;

			m_LightColorConst = NULL;
			m_LightColor[0] = 1.0f;
			m_LightColor[1] = 1.0f;
			m_LightColor[2] = 1.0f;
			m_LightColor[3] = 1.0f;

			m_ConstantColor_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COneLightingPS::~COneLightingPS(void)
		{			
			if (m_AmbientColorConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_AmbientColorConst);			

			if (m_LightColorConst)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LightColorConst);			

			if (m_ConstantColor_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ConstantColor_Const);							
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COneLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/onelight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COneLightingPS::vCreateConstantAndResource() 
		{ 
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());			
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP).c_str());			

			if (!m_AmbientColorConst)
				m_AmbientColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gAmbientColor");

			if (!m_LightColorConst)
				m_LightColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightColor");

			if (!m_ConstantColor_Const)
				m_ConstantColor_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gOutputColor");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COneLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		
			
			SceneSystem::CLight* lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight();
			if (lpL)
			{
				m_AmbientColor[0] = (lpL->m_Color.Red / 255.0f) * lpL->m_Intensity;
				m_AmbientColor[1] = (lpL->m_Color.Green / 255.0f) * lpL->m_Intensity;
				m_AmbientColor[2] = (lpL->m_Color.Blue / 255.0f) * lpL->m_Intensity;
				m_AmbientColor[3] = 1.0f;
			}

			lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
			if (lpL)
			{
				m_LightColor[0] = (lpL->m_Color.Red / 255.0f) * lpL->m_Intensity;
				m_LightColor[1] = (lpL->m_Color.Green / 255.0f) * lpL->m_Intensity;
				m_LightColor[2] = (lpL->m_Color.Blue / 255.0f) * lpL->m_Intensity;
				m_LightColor[3] = 1.0f;
			}			

            if (m_LightColorConst)
            {
                m_LightColorConst->vMap();
					m_AmbientColorConst->vSetVector4(m_AmbientColor);
                    m_LightColorConst->vSetVector4(m_LightColor);
                m_LightColorConst->vUnmap();
            }
			
			/*
			float Color[3];
			Color[0] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Color.Red/255.0f;
			Color[1] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Color.Green/255.0f;
			Color[2] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight()->m_Color.Blue/255.0f;
			m_lpShader->vSetConstantV3("gAmbientLight.Color", Color);	
			Color[0] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red/255.0f;
			Color[1] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green/255.0f;
			Color[2] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity * m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue/255.0f;
			m_lpShader->vSetConstantV3("gLight.Color", Color);			
			*/
		}		
	}
}
