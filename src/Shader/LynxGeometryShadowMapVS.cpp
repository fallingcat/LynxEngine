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
#include <Shader/LynxGeometryShadowMapVS.h>
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
		CGeometryShadowMapVS::CGeometryShadowMapVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CGeometryVS(pgs)
		{
			m_bPointLighting_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometryShadowMapVS::~CGeometryShadowMapVS(void)
		{
			if (m_bPointLighting_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_bPointLighting_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGeometryShadowMapVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/geometry_simple_shadowmap.vso"), &m_ShaderMacros);
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryShadowMapVS::vCreateConstantAndResource() 
		{ 
			CGeometryVS::vCreateConstantAndResource();
			if (!m_bPointLighting_Const)
				m_bPointLighting_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbPointLighting");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryShadowMapVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

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
			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);				
				m_WorldViewM->vSetMatrix(&WVM);
				m_WorldM->vSetMatrix(&lpc->GetTransformMatrix());
			m_WorldViewProjM->vUnmap();	

			m_bPointLighting_Const->vMap();
				if (lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight() && lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight()->GetLightType() == SceneSystem::CLight::POINT)
				{
					m_bPointLighting_Const->vSetBool(TRUE);					
				}
				else
				{
					m_bPointLighting_Const->vSetBool(FALSE);			
				}
			m_bPointLighting_Const->vUnmap();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}	
	}
}
