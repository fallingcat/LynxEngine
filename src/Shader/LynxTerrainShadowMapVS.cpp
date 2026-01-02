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
#include <Shader/LynxTerrainShadowMapVS.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTerrainShadowMapVS::CTerrainShadowMapVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CTerrainVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTerrainShadowMapVS::~CTerrainShadowMapVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTerrainShadowMapVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/terrain_shadowmap.vso"), &m_ShaderMacros);
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTerrainShadowMapVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

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
			if (lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight() && lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight()->GetLightType() == SceneSystem::CLight::POINT)
			{
				m_lpShader->vSetConstantB("gbPointLighting", TRUE);					
			}
			else
				m_lpShader->vSetConstantB("gbPointLighting", FALSE);					
		}	
	}
}
