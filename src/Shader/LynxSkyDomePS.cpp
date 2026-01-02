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
#include <Shader/LynxSkyDomePS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkyDomePS::CSkyDomePS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkyDomePS::~CSkyDomePS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSkyDomePS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/SkyDome.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkyDomePS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			CSkyDomeContainer* lpSky = (CSkyDomeContainer*)lpc;

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));			
			m_lpShader->vSetConstantF("gSunDir", &lpSky->GetSunDir().x, 3);																	
			m_lpShader->vSetConstantF("gInvWavelength", &lpSky->GetInvWavelength().x, 3);
			m_lpShader->vSetConstantF("gHG", &lpSky->GetHG().x, 3);			
			float Temp[4] = { (float)lpSky->GetSkyLightMapSize(), 1.0f / (float)lpSky->GetSkyLightMapSize(), lpSky->GetKrESun(), lpSky->GetKmESun()};
			m_lpShader->vSetConstantF("gTextureSizeAndKrKmData", &Temp[0], 4);			
			m_lpShader->vSetConstantB("gHDRRendering", m_lpGraphicsSystem->m_Config.bHDRRendering);
			m_lpShader->vSetConstantF("gExposure", cam->m_Exposure);
		}		
	}
}
