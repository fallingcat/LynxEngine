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
#include <Shader/LynxSpriteClusterVS.h>
#include <Container/LynxSprite3DClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteClusterVS::CSpriteClusterVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{
			m_WorldViewProjM = NULL;
			m_PositionArray = NULL;
            m_ColorArray = NULL;
            m_SizeArray = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpriteClusterVS::~CSpriteClusterVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);

			if (m_PositionArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_PositionArray);
            if (m_ColorArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ColorArray);
            if (m_SizeArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SizeArray);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpriteClusterVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), CSpriteClusterContainer::m_BatchSize);
			ShaderMacro.Name = CAnsiString("SPRITECLUSTER_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/spritecluster.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteClusterVS::vCreateConstantAndResource() 
		{ 
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
						
            if (!m_PositionArray)
				m_PositionArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gPositionArray");
			if (!m_SizeArray)
				m_SizeArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSizeArray");
            if (!m_ColorArray)
				m_ColorArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gColorArray");			
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSpriteClusterVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 OrthM, WVPM;
			LYNXRECT Rect;

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			lpR->GetViewportRect(&Rect);

			lynxOrthoProjectionMatrix(&OrthM, (Rect.x2 - Rect.x1), (Rect.y2 - Rect.y1), 0.0f, 100.0f);
			//lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachedRenderBuffer(0)->vGetWidth(), lpR->GetCachedRenderBuffer(0)->vGetHeight(), 0.0f, 100.0f);		
			lynxMatrixXMatrix(&WVPM, &lpc->GetTransformMatrix(), &OrthM);
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);			
			m_WorldViewProjM->vUnmap();
		}
	}
}
