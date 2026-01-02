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

#ifndef __LYNXD3D9VERTEXARRAY_H__
#define __LYNXD3D9VERTEXARRAY_H__


#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxVertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9VertexArray : public CVertexArray
		{
			LYNX_DECLARE_CLASS(CD3D9VertexArray);
		protected:			
			IDirect3DVertexBuffer9*			m_lpVB;
		public:						
			CD3D9VertexArray(CGraphicsSystem* lpsys);
			virtual ~CD3D9VertexArray();	
			
			LYNXINLINE CD3D9VertexArray&	operator =(const CD3D9VertexArray& t) {return (*this); };		
			
			LYNXBOOL						vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL);
			void							vSet(int stream, int first);			
			void*							vMap(int start, int size, CRenderer::LOCKMODE flag);			
			void							vUnmap();			
			LYNXINLINE CString				vGetDesc() { LYNXCHAR Desc[256]; lynxSprintf(Desc, _T("%s(size = %.2fK)"), m_Name.c_str(), (float)m_Size/(1024.0f)); return CString(Desc); };
		};
	}		
}

#endif

#endif