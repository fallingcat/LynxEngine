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
#include <Container/LynxSkyDomeContainer.h>
#include <Shader/LynxSkyLightPS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkyLightPS::CSkyLightPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkyLightPS::~CSkyLightPS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSkyLightPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/SkyLight.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkyLightPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			CSkyDomeContainer* lpSky = (CSkyDomeContainer*)lpc;

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));			

			float Temp[8] = {lpSky->GetInnerRadius(), lpSky->GetOuterRadius(), lpSky->GetKr4PI(), lpSky->GetKm4PI(),
							 lpSky->GetScale(), lpSky->GetRayleighScaleDepth(), (float)lpSky->GetNumSamples() };

			m_lpShader->vSetConstantF("gR1R2KrKm", &Temp[0], 4);	
			m_lpShader->vSetConstantF("gRayleigh", &Temp[4], 3);	
					
			m_lpShader->vSetConstantF("gSunPos", &lpSky->GetSunDir().x, 3);							
			m_lpShader->vSetConstantF("gEyePos", &lpSky->GetEyePos().x, 3);							
			m_lpShader->vSetConstantF("gInvWavelength", &lpSky->GetInvWavelength().x, 3);			
		}		
	}
}
