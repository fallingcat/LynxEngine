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
#include <Shader/LynxCrossSpriteGrassZPassVS.h>
#include <Container/LynxCrossSpriteGrassContainer.h>
//#include <LynxSpeedTreeContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassZPassVS::CCrossSpriteGrassZPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CCrossSpriteGrassVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassZPassVS::~CCrossSpriteGrassZPassVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCrossSpriteGrassZPassVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/crossspritegrass_zpass.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CCrossSpriteGrassZPassVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));

			CSpriteGrassContainer* lpC = (CSpriteGrassContainer*)lpc;			
			lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					

			m_lpShader->vSetConstantM("gWorldViewProj", &WVPM);
			m_lpShader->vSetConstantM("gWorldView", &WVM);	
			m_lpShader->vSetConstantM("gWorld", &lpc->GetTransformMatrix());	
			m_lpShader->vSetConstantM("gLastWorldViewProj", &lpc->GetLastWVPM());
			//m_lpShader->vSetConstantF("gWindMatrix", CSpeedTreeContainer::GetWindMatrix(), 4*16);				
			
			if (cam->m_bMotionBlur)
				if (cam->m_bCameraMovingBlur)
						lpc->SetLastWVPM(WVPM);														
		}
	}
}
