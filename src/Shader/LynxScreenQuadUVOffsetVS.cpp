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
#include <Shader/LynxScreenQuadUVOffsetVS.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScreenQuadUVOffsetVS::CScreenQuadUVOffsetVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_UVOffset[0].u = m_UVOffset[0].v = 0.0f;
			m_UVOffset[1].u = m_UVOffset[1].v = 0.0f;
			m_UVOffset[2].u = m_UVOffset[2].v = 0.0f;
			m_UVOffset[3].u = m_UVOffset[3].v = 0.0f;
			m_UVOffset_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScreenQuadUVOffsetVS::~CScreenQuadUVOffsetVS(void)
		{
			if (m_UVOffset_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_UVOffset_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScreenQuadUVOffsetVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("common/lynxScreenQuadUVOffset.vso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScreenQuadUVOffsetVS::vCreateConstantAndResource() 
		{ 
			if (!m_UVOffset_Const)
				m_UVOffset_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gUVOffset");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScreenQuadUVOffsetVS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			m_UVOffset_Const->vMap();
				m_UVOffset_Const->vSetVector2((float*)&m_UVOffset, 4);
			m_UVOffset_Const->vUnmap();

			lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
