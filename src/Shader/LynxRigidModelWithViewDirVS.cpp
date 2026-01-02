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
#include <Shader/LynxRigidModelWithViewDirVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelWithViewDirVS::CRigidModelWithViewDirVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CRigidModelVS(pgs)
		{	
			m_CameraPos_Const = NULL;	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelWithViewDirVS::~CRigidModelWithViewDirVS(void)
		{
			if (m_CameraPos_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraPos_Const);			
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelWithViewDirVS::vCreateConstantAndResource() 
		{ 
			CRigidModelVS::vCreateConstantAndResource();
			if (!m_CameraPos_Const)
				m_CameraPos_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCameraPos");			
		}					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRigidModelWithViewDirVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
	        m_ShaderMacros.clear();
			ShaderMacro.Name = CAnsiString("VIEWERDIR");
			ShaderMacro.Definition = CAnsiString("1");
			m_ShaderMacros.push_back(ShaderMacro);
            
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("model/lynxM.vso"), &m_ShaderMacros);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelWithViewDirVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;
			Math::CMatrix4 World, View, Proj, Temp, PVW; 
			
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

            #ifdef __MOBILE__
                m_WorldViewM->vMap();
                    m_WorldViewM->vSetMatrix(&WVM);
                    m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                    m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
                    CVector3 Pos;
                    cam->vGetPosition(Pos);
                    m_CameraPos_Const->vSetVector3(&Pos.x);
					if (m_TextureUVM)
						m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
                m_WorldViewM->vUnmap();
            #else
                m_WorldViewProjM->vMap();
                    m_WorldViewProjM->vSetMatrix(&WVPM);
                    //m_WorldViewM->vSetMatrix(&WVM);
                    m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                    m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
                    CVector3 Pos;
                    cam->vGetPosition(Pos);
                    m_CameraPos_Const->vSetVector3(&Pos.x);
					if (m_TextureUVM)
						m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
                m_WorldViewProjM->vUnmap();
            #endif
			
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));		
		}
	}
}
