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

#ifndef __LYNXD3D9QUERY_H__
#define __LYNXD3D9QUERY_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/LynxQuery.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9Query : public CQuery
		{
			LYNX_DECLARE_CLASS(CQuery);
		protected:
			IDirect3DQuery9*				m_lpQuery;
		public:						
			CD3D9Query(CGraphicsSystem* lpgs);
			virtual ~CD3D9Query();				
			
			virtual LYNXBOOL				vIsValid();						
			virtual void					vCreate(CRenderer::QUERYTYPE type);
			virtual void					vStart();			
			virtual void					vEnd();		
			virtual LYNXBOOL				vGetData(void* data, DWORD size, LYNXLONG wait_time);		
		};
	}		
}

#endif
