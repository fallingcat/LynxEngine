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
#include <Shader/LynxFontStringVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFontStringVS::CFontStringVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_WorldViewProjM = NULL;
			m_PositionArray = NULL;
			m_TexCoordArray = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFontStringVS::~CFontStringVS(void)
		{
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
			if (m_PositionArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_PositionArray);
			if (m_TexCoordArray)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_TexCoordArray);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CFontStringVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), CTextureFontString::m_BatchSize);
			ShaderMacro.Name = CAnsiString("FONTSTRING_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);			

			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/fontstring.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFontStringVS::vCreateConstantAndResource() 
		{ 
			if (m_WorldViewProjM)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_WorldViewProjM);
			
			m_WorldViewProjM = NULL;
			
			if (!m_WorldViewProjM)
				m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");			

			if (!m_PositionArray)
				m_PositionArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gPositionArray");			

			if (!m_TexCoordArray)
				m_TexCoordArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gTexCoordArray");			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFontStringVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			Math::CMatrix4 VM, CM, TM, WVPM, WVM, OrthM;
			LYNXRECT Rect;
			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();			

			lpR->GetViewportRect(&Rect);

			lynxOrthoProjectionMatrix(&OrthM, (Rect.x2 - Rect.x1), (Rect.y2 - Rect.y1), 0.0f, 100.0f);
			//lynxOrthoProjectionMatrix(&OrthM, lpR->GetCachedRenderBuffer(0)->vGetWidth(), lpR->GetCachedRenderBuffer(0)->vGetHeight(), 0.0f, 100.0f);
			lynxMatrixXMatrix(&WVPM, &lpc->GetTransformMatrix(), &OrthM);

			m_WorldViewProjM->vMap();
				m_WorldViewProjM->vSetMatrix(&WVPM);	
			m_WorldViewProjM->vUnmap();
			
			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFontStringVS::SetParam(float* pos, float* tex, int num) 
		{ 
			m_PositionArray->vMap();
				m_PositionArray->vSetVector4((float*)pos, num);
				m_TexCoordArray->vSetVector4((float*)tex, num);
			m_PositionArray->vUnmap();
		}
	}
}
