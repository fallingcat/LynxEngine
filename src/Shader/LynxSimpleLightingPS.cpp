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
#include <SceneSystem/Light/LynxLight.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxSimpleLightingPS.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleLightingPS::CSimpleLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleLightingPS::~CSimpleLightingPS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSimpleLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/simplelight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSimpleLightingPS::vCreateConstantAndResource() 
		{ 
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());			
			m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSimpleLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));					

			/*
			m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->vSetShaderParam(cam, lpc);

			float Color[3];
			CVector3 Dir;

			m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->GetDirection(Dir);
			m_lpShader->vSetConstantF("gLightDir", (float*)&Dir, 3);

			Color[0] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red*m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity/255.0f;
			Color[1] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green*m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity/255.0f;
			Color[2] = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue*m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight()->m_Intensity/255.0f;
			m_lpShader->vSetConstantF("gLightColor", Color, 3);

			Color[0] = 0.3f;
			Color[1] = 0.3f;
			Color[2] = 0.3f;
			m_lpShader->vSetConstantF("gAmbientColor", Color, 3);
			*/


			//m_lpShader->vSetConstantB("gbSelfIlluminated", lpc->m_bSelfIlluminated);									
			//m_lpShader->vSetConstantF("gIlluminatingIntensity", lpc->m_IlluminatingIntensity);									
		}		
	}
}
