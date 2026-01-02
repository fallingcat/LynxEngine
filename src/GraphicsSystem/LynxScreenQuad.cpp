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
#include <GraphicsSystem/LynxScreenQuad.h>
#include <Shader/LynxScreenQuadUVOffsetVS.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScreenQuad::CScreenQuad(CGraphicsSystem *pgs)
		{					
			m_lpGraphicsSystem = pgs;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScreenQuad::~CScreenQuad(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScreenQuad::Create(LYNXBOOL bInvertTexCoordV)
		{	
			GraphicsSystem::SCREENQUADVERTEX* lpSV = LYNXGLOBAL_NEW GraphicsSystem::SCREENQUADVERTEX[4];
			lpSV[0].x = -1.0f;	lpSV[0].y =  1.0f;	lpSV[0].u = 0.0f; lpSV[0].v = 0.0f;	lpSV[0].Index = 0; 
			lpSV[1].x =  1.0f;	lpSV[1].y =  1.0f;	lpSV[1].u = 1.0f; lpSV[1].v = 0.0f;	lpSV[1].Index = 1; 
			lpSV[2].x = -1.0f;	lpSV[2].y = -1.0f;	lpSV[2].u = 0.0f; lpSV[2].v = 1.0f;	lpSV[2].Index = 2; 
			lpSV[3].x =  1.0f;	lpSV[3].y = -1.0f;	lpSV[3].u = 1.0f; lpSV[3].v = 1.0f;	lpSV[3].Index = 3; 
			
			if (bInvertTexCoordV)
			{
				lpSV[0].v = 1.0f;
				lpSV[1].v = 1.0f;
				lpSV[2].v = 0.0f;
				lpSV[3].v = 0.0f;		
			}
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_VertexArray, VERTEX_ARRAY);

			CVertexLayoutPtr VertexLayoutPtr;
			m_lpGraphicsSystem->GetlpEngine()->CreateVertexLayout(VertexLayoutPtr, GetEnumStr(VT_SCREEN_QUAD));
			
			if (!m_VertexArray->vCreate(VertexLayoutPtr, 4, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpSV))
				return LYNX_FALSE;

			LYNXGLOBAL_DEL_ARRAY(lpSV);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScreenQuad::Render()
		{
			m_lpGraphicsSystem->GetlpRenderer()->SetVertexArray(m_VertexArray, 0, 0);
			m_lpGraphicsSystem->GetlpRenderer()->DrawArrayBO(CRenderer::TRIANGLE_STRIP, 2, 0);
		}	
	}
}