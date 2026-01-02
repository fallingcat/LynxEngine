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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexArray.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9VertexArray::CD3D9VertexArray(CGraphicsSystem *pgs)
		: CVertexArray(pgs)
		{	
			m_lpVB = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9VertexArray::~CD3D9VertexArray(void)
		{
			if (m_lpVB)
			{
				m_lpVB->Release();
				m_lpVB = NULL;
			}
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9VertexArray::vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo, void* data)
		{ 
			DWORD Usage;
			
			if (bbo)
			{
				Usage = 0;

				if (flag & CRenderer::DYNAMIC)
					Usage |= D3DUSAGE_DYNAMIC;

				if ((flag & CRenderer::WRITEONLY) || (flag & CRenderer::STATIC))
					Usage |= D3DUSAGE_WRITEONLY;

				CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
				if (FAILED(lpR->GetlpD3DDevice()->CreateVertexBuffer(lpl->vGetStride()*num, Usage, 0, D3DPOOL_DEFAULT, &m_lpVB, NULL)))		
				{
					return LYNX_FALSE;
				}				
				m_Stride = lpl->vGetStride();
				if (data)
				{
					void* pData = vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE);
					lynxMemCpy(pData, data, num*m_Stride);
					vUnmap();
				}
				m_Size = m_Stride * num;
				m_VideoMemoryUsage = m_Size;
			}
			else
			{
				//lpva->lpBuffer = LYNX_MALLOC(lpl->Stride*num);	
				return LYNX_FALSE;
			}
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9VertexArray::vSet(int stream, int first) 
		{ 
			//LYNX_ASSERT(m_lpVB);
			if (!m_lpVB)
				return;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpD3DDevice()->SetStreamSource(stream, m_lpVB, first*m_Stride, m_Stride);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D9VertexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			LYNX_ASSERT(m_lpVB);

			DWORD Flag;			
			
			switch (flag)
			{
				case CRenderer::LOCK_TO_READ :
					Flag = D3DLOCK_READONLY;
					break;

				case CRenderer::LOCK_TO_WRITE_DISCARD_OLD:
					Flag = D3DLOCK_DISCARD;
					break;

				case CRenderer::LOCK_TO_WRITE_NO_OVERWRITE:
					Flag = D3DLOCK_NOOVERWRITE;
					break;

				case CRenderer::LOCK_TO_WRITE :
					Flag = 0;
					break;

				case CRenderer::LOCK_TO_BOTH :
					Flag = 0;
					break;
			}		
			m_lpVB->Lock(start, size, (void **)&m_lpBuffer, Flag);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_BufferMapCounter, 1)

			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9VertexArray::vUnmap()
		{ 
			LYNX_ASSERT(m_lpVB);
			m_lpVB->Unlock();	
			m_lpBuffer = NULL;
		}
	}	
}

#endif