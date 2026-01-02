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
#include <MaterialSystem/LynxMaterialSystem.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <Shader/LynxProxyVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CProxyVS::CProxyVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldViewProjM = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CProxyVS::~CProxyVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CProxyVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CProxyVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/proxy.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CProxyVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX WVPM, WVM;
			
			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());									

			m_WorldViewProjM->vMap();				
				m_WorldViewProjM->vSetMatrix(&WVPM);			
			m_WorldViewProjM->vUnmap();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
