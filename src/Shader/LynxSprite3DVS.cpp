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
#include <Shader/LynxSprite3DVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSprite3DVS::CSprite3DVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSprite3DVS::~CSprite3DVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSprite3DVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/sprite3d.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSprite3DVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX VM, CM, TM, WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			VM = cam->GetViewMatrix();		
			CM = cam->GetTransformMatrix();
			CM.m41 = CM.m42 = CM.m43 = 0.0f;
			lynxMatrixXMatrix(&TM, &lpc->GetTransformMatrix(), &CM);
			TM.m41 = lpc->GetTransformMatrix().m41;
			TM.m42 = lpc->GetTransformMatrix().m42;
			TM.m43 = lpc->GetTransformMatrix().m43;		
			lynxMatrixXMatrix(&WVM, &TM, &VM);	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					

			m_lpShader->vSetConstantM("gWorldViewProj", &WVPM);
			m_lpShader->vSetConstantM("gWorldView", &WVM);	
			m_lpShader->vSetConstantM("gWorld", &lpc->GetTransformMatrix());	
		}
	}
}
