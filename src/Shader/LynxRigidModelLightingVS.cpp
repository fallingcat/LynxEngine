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
#include <Shader/LynxRigidModelLightingVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelLightingVS::CRigidModelLightingVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldViewProjM = NULL;
			m_WorldViewM = NULL;
			m_WorldM = NULL;		
			m_ProjectionM = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelLightingVS::~CRigidModelLightingVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
			if (m_WorldViewM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewM);
			if (m_WorldM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldM);
			if (m_ProjectionM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ProjectionM);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRigidModelLightingVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_m_lighting.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelLightingVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
			if (!m_WorldViewM)
				m_WorldViewM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldView");
			if (!m_WorldM)
				m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");	
			if (!m_ProjectionM)
				m_ProjectionM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gProjection");
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelLightingVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 MM, WVPM, WVM, I;
			
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
				m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
				m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
			m_WorldViewProjM->vUnmap();

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

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			/*
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

			CModelContainer* lpModel = Container_Cast<CModelContainer*>(lpc->GetlpParentContainer());
			CMeshContainer* lpMesh = (CMeshContainer*)lpc;			
			lpMesh->SetShaderConstant(m_lpShader);								
			*/
		}
	}
}
