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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Query.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Query::CD3D9Query(CGraphicsSystem *pgs)
		: CQuery(pgs)
		{	
			m_lpQuery = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Query::~CD3D9Query(void)
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
		LYNXBOOL CD3D9Query::vIsValid()
		{
			return (m_lpQuery) ? LYNX_TRUE : LYNX_FALSE;
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Query::vCreate(CRenderer::QUERYTYPE type) 
		{
			D3DQUERYTYPE Type = CD3D9Renderer::TranslateQueryType(type);			
			((CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateQuery(Type, &m_lpQuery);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Query::vStart()
		{
			m_lpQuery->Issue(D3DISSUE_BEGIN);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Query::vEnd()
		{
			m_lpQuery->Issue(D3DISSUE_END);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Query::vGetData(void* data, DWORD size, LYNXLONG wait_time)
		{
			if (wait_time < 0)
			{
				while (m_lpQuery->GetData(data, size, D3DGETDATA_FLUSH) == S_FALSE);
				return LYNX_TRUE;
			}
			else
			{
				LYNXLONG T0 = lynxOSGetPerformanceCounter();
				do {
					if (m_lpQuery->GetData(data, size, D3DGETDATA_FLUSH) == S_OK)
						return LYNX_TRUE;
				}while ( (lynxOSGetPerformanceCounter()-T0) < wait_time);

				return LYNX_FALSE;				
			}
		}
	}
}

#endif