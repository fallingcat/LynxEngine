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
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexArray.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexArray::CD3D11VertexArray(CGraphicsSystem *pgs)
		: CVertexArray(pgs)
		{	
			m_lpVB = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexArray::~CD3D11VertexArray(void)
		{
			if (m_lpVB)
			{
				m_lpVB->Release();
				m_lpVB = NULL;
			}
			m_SystemMemoryUsage = 0;
			m_VideoMemoryUsage = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexArray::vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo, void* data) 
		{
			D3D11_BUFFER_DESC Desc;
			D3D11_SUBRESOURCE_DATA InitData;
			int Flag = 0;
			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT; 

			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			m_Stride = lpl->vGetStride();
			m_Size = num*m_Stride;

			if (flag & CRenderer::WRITEONLY)
				Flag = 0;
			if (flag & CRenderer::CPU_WRITE)
				Flag |= D3D11_CPU_ACCESS_WRITE;
			if (flag & CRenderer::CPU_READ)
				Flag |= D3D11_CPU_ACCESS_READ;

			if (flag & CRenderer::STATIC)
				Usage = D3D11_USAGE_IMMUTABLE;
			else if (flag & CRenderer::DYNAMIC)
				Usage = D3D11_USAGE_DYNAMIC;
			else if (flag & CRenderer::STAGING)
				Usage = D3D11_USAGE_STAGING;
			else
				Usage = D3D11_USAGE_DEFAULT;
										
			Desc.Usage = Usage;
			Desc.ByteWidth = m_Size;
			Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			Desc.CPUAccessFlags = Flag;
			Desc.MiscFlags = 0;
			InitData.pSysMem = data;
			if (FAILED(lpR->GetlpD3DDevice()->CreateBuffer(&Desc, &InitData, &m_lpVB))) 
			{
				return LYNX_FALSE;
			}
			m_VideoMemoryUsage = m_Size;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D11VertexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			LYNX_ASSERT(m_lpVB);

			D3D11_MAP Flag;			
			
			switch (flag)
			{
				case CRenderer::LOCK_TO_READ :
					Flag = D3D11_MAP_READ;
					break;

				case CRenderer::LOCK_TO_WRITE_DISCARD_OLD:
					Flag = D3D11_MAP_WRITE_DISCARD;
					break;

				case CRenderer::LOCK_TO_WRITE_NO_OVERWRITE:
					Flag = D3D11_MAP_WRITE_NO_OVERWRITE;
					break;

				case CRenderer::LOCK_TO_WRITE :
					Flag = D3D11_MAP_WRITE;
					break;

				case CRenderer::LOCK_TO_BOTH :
					Flag = D3D11_MAP_READ_WRITE;
					break;
			}

			LYNXCHAR Str[512], Buffer[512];
			HRESULT hResult;

			D3D11_MAPPED_SUBRESOURCE MappedSubResource;
			hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Map(m_lpVB, 0, Flag, 0, &MappedSubResource);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("Error : Fail to map vertex array(D3D Error :  %s\r\n"), Str);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
				return NULL;
			}
			m_lpBuffer = MappedSubResource.pData;

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_BufferMapCounter, 1)

			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexArray::vUnmap()
		{
			LYNX_ASSERT(m_lpVB != NULL);
			((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Unmap(m_lpVB, 0);
			m_lpBuffer = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexArray::vSet(int stream, int first)
		{
			UINT Stride[] = {m_Stride};
			UINT Offset[] = {first};
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->IASetVertexBuffers(stream, 1, &m_lpVB, Stride, Offset);
		}
	}
}

#endif