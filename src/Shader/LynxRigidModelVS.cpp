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
#include <Shader/LynxRigidModelVS.h>
#include <Container/LynxModelContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelVS::CRigidModelVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldViewProjM = NULL;
			m_WorldViewM = NULL;
			m_WorldM = NULL;		
			m_ProjectionM = NULL;
			m_TextureUVM = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRigidModelVS::~CRigidModelVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
			if (m_WorldViewM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewM);
			if (m_WorldM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldM);
			if (m_ProjectionM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ProjectionM);
			if (m_TextureUVM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_TextureUVM);
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
			if (!m_WorldViewM)
				m_WorldViewM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldView");
			if (!m_WorldM)
				m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");	
			if (!m_ProjectionM)
				m_ProjectionM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gProjection");	
			if (!m_TextureUVM)
				m_TextureUVM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gTextureUVM");	
		}					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRigidModelVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("model/lynxM.vso"), &m_ShaderMacros);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRigidModelVS::vSet(CContainer* lpc, CCameraContainer* cam) 
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

            #if defined(__MOBILE__) || defined(__OGLES2__)
                m_WorldViewM->vMap();
                    m_WorldViewM->vSetMatrix(&WVM);
                    m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                    m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
					if (m_TextureUVM)
						m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
                m_WorldViewM->vUnmap();
            #else
                m_WorldViewProjM->vMap();
                    m_WorldViewProjM->vSetMatrix(&WVPM);
                    //m_WorldViewM->vSetMatrix(&WVM);
                    m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
                    m_ProjectionM->vSetMatrix(&(cam->GetProjectionMatrix()));
					if (m_TextureUVM)
						m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
                m_WorldViewProjM->vUnmap();
            #endif

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
	}
}
