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
#include <GraphicsSystem/D3D11Renderer/LynxD3D11IndexArray.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11IndexArray::CD3D11IndexArray(CGraphicsSystem *pgs)
		: CIndexArray(pgs)
		{	
			m_lpIB = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11IndexArray::~CD3D11IndexArray(void)
		{
			if (m_lpIB)
			{
				m_lpIB->Release();
				m_lpIB = NULL;
			}
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11IndexArray::vCreate(int stride, int num, int flag, LYNXBOOL bbo, void* data)
		{ 
			D3D11_BUFFER_DESC Desc;
			D3D11_SUBRESOURCE_DATA InitData;
			int Flag = 0;
			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT; 

			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			m_Stride = stride;
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
			Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			Desc.CPUAccessFlags = Flag;
			Desc.MiscFlags = 0;
			InitData.pSysMem = data;
			if (FAILED(lpR->GetlpD3DDevice()->CreateBuffer(&Desc, &InitData, &m_lpIB))) 
			{
				return LYNX_FALSE;
			}

			switch (m_Stride)
			{
				case 1:
					m_Format = DXGI_FORMAT_R8_UINT;
					break;
				case 2:
					m_Format = DXGI_FORMAT_R16_UINT;
					break;
				case 4:
					m_Format = DXGI_FORMAT_R32_UINT;
					break;
			}
			m_VideoMemoryUsage = m_Size;

			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11IndexArray::vSet(int first) 
		{ 
			//LYNX_ASSERT(m_lpIndexBuffer);
			if (!m_lpIB)
				return;

			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpDeviceContext()->IASetIndexBuffer(m_lpIB, m_Format, first);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D11IndexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			LYNX_ASSERT(m_lpIB);

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
			hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Map(m_lpIB, 0, Flag, 0, &MappedSubResource);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("Error : Fail to map index array(D3D Error :  %s\r\n"), Str);
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
		void CD3D11IndexArray::vUnmap()
		{ 
			LYNX_ASSERT(m_lpIB != NULL);
			((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Unmap(m_lpIB, 0);	
			m_lpBuffer = NULL;
		}
	}	
}

#endif
