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
#include <Shader/LynxRigidModelShadowMapVS.h>
#include <Container/LynxModelContainer.h>
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
		CRigidModelShadowMapVS::CRigidModelShadowMapVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CSkinningModelVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelShadowMapVS::~CRigidModelShadowMapVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRigidModelShadowMapVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_m_shadowmap.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelShadowMapVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			CModelContainer* lpC = (CModelContainer*)lpc;
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

			CModelContainer* lpModel = Container_Cast<CModelContainer*>(lpc->GetlpParentContainer());
			CMeshContainer* lpMesh = (CMeshContainer*)lpc;			
			lpMesh->UpdateShaderConstant();		
			m_lpShader->vSetConstantM("gBoneMatrix", &lpMesh->m_BoneMatrixArray[0], (int)lpMesh->GetBoneShaderData().size());			

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
	}
}
