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
#include <Shader/LynxSkinningModelOneLightingVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkinningModelOneLightingVS::CSkinningModelOneLightingVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkinningModelOneLightingVS::~CSkinningModelOneLightingVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSkinningModelOneLightingVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_mbs_onelighting.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkinningModelOneLightingVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 MM, WVPM, WVM, I;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			I.Identity();
			CModelContainer* lpC = (CModelContainer*)lpc;
			if (cam->IsMirror())
			{
				MM = cam->GetMirrorMatrix();;	
				lynxMatrixXMatrix(&WVM, &MM, &cam->GetViewMatrix());	
				lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
			}
			else
			{
				WVM = cam->GetViewMatrix();	
				lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
			}			
			m_lpShader->vSetConstantM("gWorldViewProj", &WVPM);
            m_lpShader->vSetConstantM("gProjection", &(cam->GetProjectionMatrix()));
			m_lpShader->vSetConstantM("gWorldView", &WVM);	
			m_lpShader->vSetConstantM("gWorld", &I);	

			CModelContainer* lpModel = Container_Cast<CModelContainer*>(lpc->GetlpParentContainer());
			CMeshContainer* lpMesh = (CMeshContainer*)lpc;			
			lpMesh->UpdateShaderConstant();		
			m_lpShader->vSetConstantM("gBoneMatrix", &lpMesh->m_BoneMatrixArray[0], (int)lpMesh->GetBoneShaderData().size());			

			CVector3 Pos;
			LYNXMATRIX WLPM, WLM;
            
			SceneSystem::CLight* lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
            
			cam->vGetPosition(Pos);
			m_lpShader->vSetConstantV3("gCameraPos", &Pos.x);
			lynxMatrixXMatrix(&WLPM, &lpc->GetTransformMatrix(), &lpL->m_LightViewProj);
			lynxMatrixXMatrix(&WLM, &lpc->GetTransformMatrix(), &lpL->GetCamera().GetViewMatrix());
			m_lpShader->vSetConstantM("gWorldLightViewProj", &WLPM);
			m_lpShader->vSetConstantM("gWorldLightView", &WLM);
			lpL->GetPosition(Pos);
			m_lpShader->vSetConstantV3("gLightPos", &Pos.x);
			if (lpL->GetLightType() == SceneSystem::CLight::DIRECTIONAL)
			{
				m_lpShader->vSetConstantB("gbDirectional", TRUE);
				lpL->GetDirection(Pos);
				m_lpShader->vSetConstantV3("gLightDir", &Pos.x);
			}
			else
				m_lpShader->vSetConstantB("gbDirectional", FALSE);
		}
	}
}
