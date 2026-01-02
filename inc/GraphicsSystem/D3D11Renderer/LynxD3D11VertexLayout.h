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

#ifndef __LYNXD3D11VERTEXLAYOUT_H__
#define __LYNXD3D11VERTEXLAYOUT_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11VertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(CD3D11VertexLayout);
		public:			
		protected:					
			int										m_Stride;
			ID3D11InputLayout*						m_Layout;
		public:			
			CD3D11VertexLayout(CGraphicsSystem* lpgs);
			virtual ~CD3D11VertexLayout();

			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXBOOL								vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXINLINE DWORD						vGetStride() {return m_Stride; };
			void									vSet();
		protected:
			LPCSTR									TranslateSlotName(CRenderer::VERTEXSLOT& slot);
			DXGI_FORMAT								TranslateSlotFormat(CRenderer::VERTEXSLOT& slot);
			int										GetSlotStride(CRenderer::VERTEXSLOT& slot);
		};		
	}		
}

#endif

#endif