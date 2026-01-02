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
#include <Shader/LynxSkinningModelGPassVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkinningModelGPassVS::CSkinningModelGPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CSkinningModelVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSkinningModelGPassVS::~CSkinningModelGPassVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSkinningModelGPassVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_mbs_gpass.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSkinningModelGPassVS::vSet(CContainer* lpc, CCameraContainer* cam) 
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

			CMeshContainer* lpMesh = (CMeshContainer*)lpc;			
			lpMesh->UpdateShaderConstant();		
			m_BoneMatrix->vMap();
				m_BoneMatrix->vSetMatrix(&lpMesh->m_BoneMatrixArray[0], (int)lpMesh->GetBoneShaderData().size());			
			m_BoneMatrix->vUnmap();			

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
	}
}
