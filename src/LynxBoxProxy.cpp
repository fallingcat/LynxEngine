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
#include <LynxBoxProxy.h>
#include <LynxEngine.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CBoxProxy::CBoxProxy(void)
	{		
		m_ProxyType = PROXY_BOX;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CBoxProxy::CBoxProxy(CEngine* lpe)
	: CObjProxy(lpe)
	{
		m_ProxyType = PROXY_BOX;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CBoxProxy::~CBoxProxy(void)
	{
		m_V.clear();
		m_T.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CBoxProxy::Create(Math::CVector3& c, Math::CVector3& ext)
	{
		m_V.resize(8);	
		m_T.resize(12);	

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

		Math::CVector3* lpV = LYNXGLOBAL_NEW Math::CVector3[8];
		lpV[0] = m_V[0] = c + Math::CVector3(-ext.x,	-ext.y,	-ext.z);
		lpV[1] = m_V[1] = c + Math::CVector3( ext.x,	-ext.y,	-ext.z);
		lpV[2] = m_V[2] = c + Math::CVector3(-ext.x,	-ext.y,	 ext.z);
		lpV[3] = m_V[3] = c + Math::CVector3( ext.x,	-ext.y,	 ext.z);
		lpV[4] = m_V[4] = c + Math::CVector3(-ext.x,	 ext.y,	-ext.z);
		lpV[5] = m_V[5] = c + Math::CVector3( ext.x,	 ext.y,	-ext.z);
		lpV[6] = m_V[6] = c + Math::CVector3(-ext.x,	 ext.y,	 ext.z);
		lpV[7] = m_V[7] = c + Math::CVector3( ext.x,	 ext.y,	 ext.z);		
		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_VECTOR3)), (int)8, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpV);
		LYNXGLOBAL_DEL_ARRAY(lpV);
		
		WORD* lpI = LYNXGLOBAL_NEW WORD[36];
		lpI[0]	= 0;	lpI[1]	= 1;	lpI[2]	= 3;
		lpI[3]	= 0;	lpI[4]	= 3;	lpI[5]	= 2;
		lpI[6]	= 4;	lpI[7]	= 7;	lpI[8]	= 5;
		lpI[9]	= 4;	lpI[10] = 6;	lpI[11] = 7;
		lpI[12] = 1;	lpI[13] = 7;	lpI[14] = 3;
		lpI[15] = 1;	lpI[16] = 5;	lpI[17] = 7;
		lpI[18] = 0;	lpI[19] = 6;	lpI[20] = 4;
		lpI[21] = 0;	lpI[22] = 2;	lpI[23] = 6;
		lpI[24] = 2;	lpI[25] = 3;	lpI[26] = 7;
		lpI[27] = 2;	lpI[28] = 7;	lpI[29] = 6;
		lpI[30] = 0;	lpI[31] = 5;	lpI[32] = 1;
		lpI[33] = 0;	lpI[34] = 4;	lpI[35] = 5;
		m_IndexArray->vCreate(sizeof(WORD), 36, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);

		for (int i=0; i<12; i++)
		{
			m_T[i].v1 = lpI[i*3+0];
			m_T[i].v2 = lpI[i*3+1];
			m_T[i].v3 = lpI[i*3+2];
		}

		LYNXGLOBAL_DEL_ARRAY(lpI);
	}
}