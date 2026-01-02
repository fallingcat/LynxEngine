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
#include <Shader/LynxGeometryWithViewDirVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometryWithViewDirVS::CGeometryWithViewDirVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CGeometryVS(pgs)
		{
			m_CameraPos_Const = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometryWithViewDirVS::~CGeometryWithViewDirVS(void)
		{
			if (m_CameraPos_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraPos_Const);			
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryWithViewDirVS::vCreateConstantAndResource() 
		{ 
			CGeometryVS::vCreateConstantAndResource();
			if (!m_CameraPos_Const)
				m_CameraPos_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCameraPos");			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGeometryWithViewDirVS::vCreate() 
		{ 
            GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
	        m_ShaderMacros.clear();
			ShaderMacro.Name = CAnsiString("VERTEXCOLOR");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);
			ShaderMacro.Name = CAnsiString("VIEWERDIR");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);
            
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/geometry.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometryWithViewDirVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM, WSM, VPM;
			
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
                
                lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());
                //lynxTransposeMatrix(&WVPM);
            }			
			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);                
				m_WorldViewM->vSetMatrix(&WVM);
				m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
				m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
				CVector3 Pos;
				cam->vGetPosition(Pos);
				m_CameraPos_Const->vSetVector3(&Pos.x);
			m_WorldViewProjM->vUnmap();			

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
	}
}
