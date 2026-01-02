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
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>
#include <SceneSystem/Light/LynxAmbientLight.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>

#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif


namespace LynxEngine 
{
	namespace SceneSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CAmbientLight, CLight)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAmbientLight::CAmbientLight(CScene* const lpscene)
		: CLight(lpscene)
		{
			m_LightType = AMBIENT;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAmbientLight::~CAmbientLight(void)
		{
			vRelease();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAmbientLight::vRelease(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAmbientLight::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAmbientLight::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAmbientLight::vSetShaderParam(CCameraContainer* cam, CContainer* lpc)
		{
			float Color[3];

			GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();
			Color[0] = m_Color.Red/255.0f;
			Color[1] = m_Color.Green/255.0f;
			Color[2] = m_Color.Blue/255.0f;
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Color", Color, 3);
			lpR->GetCachedPixelShader()->vSetConstantF("gLight.Intensity", m_Intensity);			

			lpR->GetCachedPixelShader()->vSetConstantB("gbAOMapping", !m_lpScene->GetlpRenderer()->m_RenderingOptions.bSSAOPass);		
			lpR->GetCachedPixelShader()->vSetConstantB("gbSelfIlluminated", lpc->m_bSelfIlluminated);
			lpR->GetCachedPixelShader()->vSetConstantF("gIlluminatingIntensity", lpc->m_IlluminatingIntensity);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CAmbientLight::vDeferredLighting(CCameraContainer* cam) 
		{
			CDeferredRenderer* lpR = static_cast<CDeferredRenderer*>(m_lpScene->GetlpRenderer());
			GraphicsSystem::CGraphicsSystem* lpG = m_lpEngine->GetlpGraphicsSystem();
			lpG->SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(&(lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::AMBIENT_LIGHTING)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), *cam);						
			lpR->GetlpDeferredLightingMaterial(CDeferredRenderer::AMBIENT_LIGHTING)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(m_lpContainer, cam);	

			lpG->DrawScreenSpaceQuad();
		}
	}
}