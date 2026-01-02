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
#include <LynxObjProxy.h>
#include <LynxEngine.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObjProxy::CObjProxy(void)
	{
		m_ProxyType = PROXY_GEOMETRY;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObjProxy::CObjProxy(CEngine* lpe)
	: CGameObj(lpe)
	{	
		m_ProxyType = PROXY_GEOMETRY;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObjProxy::~CObjProxy(void)
	{
	}		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CObjProxy::vCreate(DWORD numv, Math::CVector3* lpv, DWORD numi, int* lpi)
	{
		m_V.resize(numv);
		m_T.resize(numi/3);

		/*
		if (m_VertexArray)
		{
			m_lpEngine->DestroyResource(m_VertexArray->GetName());
		}
		if (m_IndexArray)
		{
			m_lpEngine->DestroyResource(m_IndexArray->GetName());
		}
		*/

		m_VertexArray.Reset();
		m_IndexArray.Reset();

		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);	

		m_VertexArray->SetName(m_Name + CString(_T(".VertexArray")));
		m_IndexArray->SetName(m_Name + CString(_T(".IndexArray")));

		for (int i=0; i<numv; i++)
			m_V[i] = lpv[i];
		for (int i=0; i<numi/3; i++)
		{
			m_T[i].v1 = lpi[i*3+0];
			m_T[i].v2 = lpi[i*3+1];
			m_T[i].v3 = lpi[i*3+2];
		}

		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_VECTOR3)), (int)numv, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpv);	

		WORD* lpI = LYNXGLOBAL_NEW WORD[numi];
		for (int i=0; i<numi; i++)
			lpI[i] = lpi[i];
		m_IndexArray->vCreate(sizeof(WORD), numi,GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);
		LYNXGLOBAL_DEL_ARRAY(lpI);	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CObjProxy::vRender(CCameraContainer* cam)
	{
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();
		lpR->SetVertexArray(m_VertexArray, 0, 0);
		lpR->SetIndexArray(m_IndexArray, 0);						
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, (int)m_V.size(), 0, (int)m_T.size());		
	}	
}