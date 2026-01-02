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

#ifndef __LYNXD3D11INDEXARRAY_H__
#define __LYNXD3D11INDEXARRAY_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxIndexArray.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11IndexArray : public CIndexArray
		{
			LYNX_DECLARE_CLASS(CD3D11IndexArray);
		protected:
			ID3D11Buffer*					m_lpIB;
			DXGI_FORMAT						m_Format;
		public:						
			CD3D11IndexArray(CGraphicsSystem* lpsys);
			virtual ~CD3D11IndexArray();
			
			LYNXINLINE CD3D11IndexArray&		operator =(const CD3D11IndexArray& t) {return (*this); };		

			LYNXBOOL						vCreate(int stride, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL);
			LYNXINLINE void					vSet(int first);			
			LYNXINLINE void*				vMap(int start, int size, CRenderer::LOCKMODE flag);
			LYNXINLINE void					vUnmap();			
			LYNXINLINE CString				vGetDesc() { LYNXCHAR Desc[256]; lynxSprintf(Desc, _T("%s(size = %d)"), m_Name.c_str(), m_Size); return CString(Desc); };
		};
	}		
}

#endif

#endif