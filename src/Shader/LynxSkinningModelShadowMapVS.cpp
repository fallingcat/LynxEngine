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
#include <Shader/LynxSkinningModelShadowMapVS.h>
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
		CSkinningModelShadowMapVS::CSkinningModelShadowMapVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CSkinningModelVS(pgs)
		{	
			m_PointLighting_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkinningModelShadowMapVS::~CSkinningModelShadowMapVS(void)
		{
			if (m_PointLighting_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_PointLighting_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSkinningModelShadowMapVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_mbs_shadowmap.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkinningModelShadowMapVS::vCreateConstantAndResource() 
		{ 
			CSkinningModelVS::vCreateConstantAndResource();

			if (!m_PointLighting_Const)
				m_PointLighting_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbPointLighting");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkinningModelShadowMapVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 MM, WVPM, WVM, I;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

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

			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);
				m_WorldViewM->vSetMatrix(&WVM);
				m_WorldM->vSetMatrix(&I);				
			m_WorldViewProjM->vUnmap();	

			m_PointLighting_Const->vMap();
				if (lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight() && lpc->GetlpCurrentMaterial()->GetlpMaterialSystem()->GetlpCurrentLight()->GetLightType() == SceneSystem::CLight::POINT)
				{
					m_PointLighting_Const->vSetBool(TRUE);					
				}
				else
				{
					m_PointLighting_Const->vSetBool(FALSE);			
				}
			m_PointLighting_Const->vUnmap();

			CMeshContainer* lpMesh = (CMeshContainer*)lpc;			
			lpMesh->UpdateShaderConstant();		
			m_BoneMatrix->vMap();
				m_BoneMatrix->vSetMatrix(&lpMesh->m_BoneMatrixArray[0], (int)lpMesh->GetBoneShaderData().size());			
			m_BoneMatrix->vUnmap();			

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));		
		}
	}
}
