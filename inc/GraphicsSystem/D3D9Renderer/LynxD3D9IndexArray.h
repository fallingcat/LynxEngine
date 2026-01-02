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

#ifndef __LYNXD3D9INDEXARRAY_H__
#define __LYNXD3D9INDEXARRAY_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxIndexArray.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9IndexArray : public CIndexArray
		{
			LYNX_DECLARE_CLASS(CD3D9IndexArray);
		protected:
			IDirect3DIndexBuffer9*			m_lpIB;
		public:						
			CD3D9IndexArray(CGraphicsSystem* lpsys);
			virtual ~CD3D9IndexArray();
			
			LYNXINLINE CD3D9IndexArray&		operator =(const CD3D9IndexArray& t) {return (*this); };		
			
			LYNXBOOL						vCreate(int stride, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL);
			void							vSet(int first);			
			void*							vMap(int start, int size, CRenderer::LOCKMODE flag);			
			void							vUnmap();			
		};
	}		
}

#endif

#endif