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
#include <Shader/LynxRigidModelZPassVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelZPassVS::CRigidModelZPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CRigidModelVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelZPassVS::~CRigidModelZPassVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRigidModelZPassVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/model_m_zpass.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelZPassVS::vSet(CContainer* lpc, CCameraContainer* cam) 
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
			m_lpShader->vSetConstantM("gLastWorldViewProj", &lpc->GetLastWVPM());		

			if (cam->m_bMotionBlur)
				if (cam->m_bCameraMovingBlur)
						lpc->SetLastWVPM(WVPM);
		}
	}
}
