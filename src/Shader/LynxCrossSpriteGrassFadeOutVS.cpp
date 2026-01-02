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
#include <Shader/LynxCrossSpriteGrassFadeOutVS.h>
#include <Container/LynxSpriteGrassContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassFadeOutVS::CCrossSpriteGrassFadeOutVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassFadeOutVS::~CCrossSpriteGrassFadeOutVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCrossSpriteGrassFadeOutVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/crossspritegrass_fadeout.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CCrossSpriteGrassFadeOutVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			CSpriteGrassContainer* lpC = (CSpriteGrassContainer*)lpc;			

			if (cam->IsMirror())
			{
				lynxMatrixXMatrix(&MM, &lpc->GetTransformMatrix(), &cam->GetMirrorMatrix());	
				lynxMatrixXMatrix(&WVM, &MM, &cam->GetViewMatrix());	
				lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
			}
			else
			{
				lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());	
				lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
			}			
			m_lpShader->vSetConstantM("gWorldViewProj", &WVPM);
			m_lpShader->vSetConstantM("gWorldView", &WVM);	
			m_lpShader->vSetConstantM("gWorld", &lpc->GetTransformMatrix());			

			CVector3 Pos;
			LYNXMATRIX WLPM, WLM;

			SceneSystem::CLight* lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();

			cam->vGetPosition(Pos);
			m_lpShader->vSetConstantF("gCameraPos", &Pos.x, 3);
			lynxMatrixXMatrix(&WLPM, &lpc->GetTransformMatrix(), &lpL->m_LightViewProj);			
			lynxMatrixXMatrix(&WLM, &lpc->GetTransformMatrix(), &lpL->GetCamera().GetViewMatrix());							
			m_lpShader->vSetConstantM("gWorldLightViewProj", &WLPM);				
			m_lpShader->vSetConstantM("gWorldLightView", &WLM);					
			lpL->GetPosition(Pos);
			m_lpShader->vSetConstantF("gLightPos", &Pos.x, 3);
			if (lpL->GetLightType() == SceneSystem::CLight::DIRECTIONAL)
			{
				m_lpShader->vSetConstantB("gbDirectional", TRUE);
				lpL->GetDirection(Pos);
				m_lpShader->vSetConstantF("gLightDir", &Pos.x, 3);
			}
			else
				m_lpShader->vSetConstantB("gbDirectional", FALSE);

		}
	}
}
