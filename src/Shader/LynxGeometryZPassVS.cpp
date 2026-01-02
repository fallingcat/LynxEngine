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
#include <Shader/LynxGeometryZPassVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometryZPassVS::CGeometryZPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CGeometryVS(pgs)
		{	
			m_WorldViewProjM = NULL;
			m_WorldViewM = NULL;
			m_WorldM = NULL;
			m_LastWorldViewProjM = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometryZPassVS::~CGeometryZPassVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
			if (m_WorldViewM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewM);
			if (m_WorldM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldM);
			if (m_LastWorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_LastWorldViewProjM);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGeometryZPassVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/geometry_zpass.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryZPassVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
			if (!m_WorldViewM)
				m_WorldViewM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldView");
			if (!m_WorldM)
				m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");
			if (!m_LastWorldViewProjM)
				m_LastWorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLastWorldViewProj");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryZPassVS::vSet(CContainer* lpc, CCameraContainer* cam) 
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
				m_LastWorldViewProjM->vSetMatrix(&lpc->GetLastWVPM());			
			m_WorldViewProjM->vUnmap();						
			
			if (cam->m_bMotionBlur)
				if (cam->m_bCameraMovingBlur)
						lpc->SetLastWVPM(WVPM);	

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
