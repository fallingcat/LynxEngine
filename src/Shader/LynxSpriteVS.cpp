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
#include <Shader/LynxSpriteVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteVS::CSpriteVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldViewProjM = NULL;
			m_Scale = NULL;
			m_TextureUVM = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteVS::~CSpriteVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);

			if (m_Scale)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_Scale);

			if (m_TextureUVM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_TextureUVM);				
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpriteVS::vCreate() 
		{ 
            return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/sprite.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");			

			if (!m_Scale)
				m_Scale = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSizeScale");

			if (!m_TextureUVM)
				m_TextureUVM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gTextureUVM");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteVS::SetParam(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 VM, CM, TM, WVPM, WVM, OrthM;
			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

            if (lpR->IsPortraitMode())
                lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachedRenderBuffer(0)->vGetHeight(), lpR->GetCachedRenderBuffer(0)->vGetWidth(), 0.0f, 100.0f);
            else
                lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachedRenderBuffer(0)->vGetWidth(), lpR->GetCachedRenderBuffer(0)->vGetHeight(), 0.0f, 100.0f);
			lynxMatrixXMatrix(&WVPM, &lpc->GetTransformMatrix(), &OrthM);
						
			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);	
				m_Scale->vSetVector3((float*)(&(static_cast<CSpriteContainer*>(lpc)->m_Scale.x)));
				if (m_TextureUVM)
					m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
			m_WorldViewProjM->vUnmap();						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 VM, CM, TM, WVPM, WVM, OrthM;
			LYNXRECT Rect;
			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			lpR->GetViewportRect(&Rect);

			lynxOrthoProjectionMatrix(&OrthM, (Rect.x2 - Rect.x1), (Rect.y2 - Rect.y1), 0.0f, 100.0f);

			/*
			if (lpR->IsPortraitMode())
				lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachGetCachedRenderBuffer(0)->vGetHeight(), lpR->GetCachedRenderBuffer(0)->vGetWidth(), 0.0f, 100.0f);
            else
                lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachedRenderBuffer(0)->vGetWidth(), lpR->GetCachedRenderBuffer(0)->vGetHeight(), 0.0f, 100.0f);
				*/
			//VM = cam->GetViewMatrix();		
			//CM = cam->GetTransformMatrix();
			//CM.m41 = CM.m42 = CM.m43 = 0.0f;
			//lynxMatrixXMatrix(&TM, &lpc->GetTransformMatrix(), &CM);
			//TM.m41 = lpc->GetTransformMatrix().m41;
			//TM.m42 = lpc->GetTransformMatrix().m42;
			//TM.m43 = lpc->GetTransformMatrix().m43;		
			//lynxMatrixXMatrix(&WVM, &TM, &VM);	
			lynxMatrixXMatrix(&WVPM, &lpc->GetTransformMatrix(), &OrthM);
						
			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);	
				m_Scale->vSetVector3((float*)(&(static_cast<CSpriteContainer*>(lpc)->m_Scale.x)));
				if (m_TextureUVM)
					m_TextureUVM->vSetMatrix(&(lpc->GetTextureUVMatrix()));
			m_WorldViewProjM->vUnmap();
			
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
	}
}
