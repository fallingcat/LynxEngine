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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9IndexArray.h>
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
		CD3D9IndexArray::CD3D9IndexArray(CGraphicsSystem *pgs)
		: CIndexArray(pgs)
		{	
			m_lpIB = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9IndexArray::~CD3D9IndexArray(void)
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
		LYNXBOOL CD3D9IndexArray::vCreate(int stride, int num, int flag, LYNXBOOL bbo, void* data)
		{ 
			DWORD Usage = 0;

			if (flag & CRenderer::DYNAMIC)
				Usage |= D3DUSAGE_DYNAMIC;

			if (flag & CRenderer::WRITEONLY)
				Usage |= D3DUSAGE_WRITEONLY;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			if (FAILED(lpR->GetlpD3DDevice()->CreateIndexBuffer(stride*num, Usage, (stride==2)?D3DFMT_INDEX16:D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_lpIB, NULL)))
			{
				return LYNX_FALSE;
			}
			m_Stride = stride;
			if (data)
			{
				void* pData = vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE);
				lynxMemCpy(pData, data, num*stride);
				vUnmap();
			}
			m_Size = m_Stride * num;
			m_VideoMemoryUsage = m_Size;
			
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9IndexArray::vSet(int first) 
		{ 
			//LYNX_ASSERT(m_lpIB);
			if (!m_lpIB)
				return;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpD3DDevice()->SetIndices(m_lpIB);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D9IndexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			LYNX_ASSERT(m_lpIB);

			DWORD					Flag;			
			
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
			m_lpIB->Lock(start, size, (void **)&m_lpBuffer, Flag);	

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_BufferMapCounter, 1)

			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9IndexArray::vUnmap()
		{ 
			LYNX_ASSERT(m_lpIB);
			m_lpIB->Unlock();				
			m_lpBuffer = NULL;
		}
	}	
}

#endif