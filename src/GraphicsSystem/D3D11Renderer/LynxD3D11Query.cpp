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
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Query.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Query::CD3D11Query(CGraphicsSystem *pgs)
		: CQuery(pgs)
		{	
			m_lpQuery = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Query::~CD3D11Query(void)
		{
			if (m_lpQuery)
			{
				m_lpQuery->Release();
				m_lpQuery = NULL;
			}
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Query::vCreate(CRenderer::QUERYTYPE type) 
		{
			D3D11_QUERY_DESC Desc;

			Desc.Query = CD3D11Renderer::TranslateQueryType(type);			
			Desc.MiscFlags = 0;
			((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateQuery(&Desc, &m_lpQuery);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Query::vStart()
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->Begin(m_lpQuery);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Query::vEnd()
		{
			((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->End(m_lpQuery);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Query::vGetData(void* data, DWORD size, LYNXLONG wait_time)
		{
			return LYNX_FALSE;				

			if (wait_time < 0)
			{
				while (((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->GetData(m_lpQuery, data, size, 0) == S_FALSE);
				return LYNX_TRUE;
			}
			else
			{
				LYNXLONG T0 = lynxOSGetPerformanceCounter();
				do {
					if (((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpDeviceContext()->GetData(m_lpQuery, data, size, 0) == S_OK)
						return LYNX_TRUE;
				}while ( (lynxOSGetPerformanceCounter()-T0) < wait_time);

				return LYNX_FALSE;				
			}
			return LYNX_TRUE;				
		}
	}
}

#endif