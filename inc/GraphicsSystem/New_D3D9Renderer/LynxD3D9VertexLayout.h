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

#ifndef __LYNXD3D9VERTEXLAYOUT_H__
#define __LYNXD3D9VERTEXLAYOUT_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9VertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(CD3D9VertexLayout);
		public:			
		protected:			
			int										m_Stride;
			IDirect3DVertexDeclaration9*			m_Layout;
		public:			
			CD3D9VertexLayout(CGraphicsSystem* lpgs);
			virtual ~CD3D9VertexLayout();
			
			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num);
			LYNXBOOL								vCreate();
			LYNXINLINE DWORD						vGetStride() {return m_Stride; };
			void									vSet();
		private:
			D3DDECLTYPE								TranslateVertexSlotFormat(int i, int* offset);
		};		
	}		
}

#endif