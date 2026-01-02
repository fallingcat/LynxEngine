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
#include <Shader/LynxSpriteGrassFadeOutAmbientLightingPS.h>
#include <Container/LynxSprite3DClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteGrassFadeOutAmbLightingPS::CSpriteGrassFadeOutAmbLightingPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteGrassFadeOutAmbLightingPS::~CSpriteGrassFadeOutAmbLightingPS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpriteGrassFadeOutAmbLightingPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/spritegrass_fadeout_ambientlight.pso"), &m_ShaderMacros);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteGrassFadeOutAmbLightingPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;

			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));		

			float Color[3];
			SceneSystem::CAmbientLight* lpAL = (SceneSystem::CAmbientLight*)m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
			Color[0] = lpAL->m_Color.Red/255.0f;
			Color[1] = lpAL->m_Color.Green/255.0f;
			Color[2] = lpAL->m_Color.Blue/255.0f;
			m_lpShader->vSetConstantF("gLight.Color", Color, 3);
			m_lpShader->vSetConstantF("gLight.Intensity", lpAL->m_Intensity);			

			m_lpShader->vSetConstantB("gbAOMapping", LYNX_FALSE);//!m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->m_RenderOptions.bSSAOPass);		
			m_lpShader->vSetConstantB("gbSelfIlluminated", lpc->m_bSelfIlluminated);
			m_lpShader->vSetConstantF("gIlluminatingIntensity", lpc->m_IlluminatingIntensity);					
		}
	}
}
