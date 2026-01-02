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

#ifndef __LYNXD3D11BUFFER_H__
#define __LYNXD3D11BUFFER_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxBuffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11Buffer : public CBuffer
		{
			LYNX_DECLARE_CLASS(CD3D11Buffer);
		protected:			
			ID3D11Resource*				m_lpResource;
			ID3D11ShaderResourceView*	m_lpSRView;	
			ID3D11RenderTargetView*		m_lpRTView;
			ID3D11DepthStencilView*		m_lpDSView;

			int								m_Width, m_Height;
		private:
		public:						
			CD3D11Buffer(CGraphicsSystem* lpsys);
			virtual ~CD3D11Buffer();

			LYNXINLINE CD3D11Buffer&		operator =(const CD3D11Buffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };		


			LYNXINLINE void					SetlpResource(ID3D11Resource* lpr) {m_lpResource = lpr;};
			LYNXINLINE ID3D11Resource*		GetlpResource() {return m_lpResource;};

			LYNXINLINE void					SetlpSRView(ID3D11ShaderResourceView* lpr) {m_lpSRView = lpr;};
			LYNXINLINE ID3D11ShaderResourceView* GetlpSRView() {return m_lpSRView;};

			LYNXINLINE void					SetlpRTView(ID3D11RenderTargetView* lpr) {m_lpRTView = lpr;};
			LYNXINLINE ID3D11RenderTargetView* GetlpRTView() {return m_lpRTView;};
			
			LYNXINLINE void					SetlpDSView(ID3D11DepthStencilView* lpr) {m_lpDSView = lpr;};
			LYNXINLINE ID3D11DepthStencilView* GetlpDSView() {return m_lpDSView;};

			LYNXINLINE void*				vMap(int flag, LPLYNXRECT lprect = NULL); 
			LYNXINLINE void					vUnmap();

			LYNXINLINE LYNXBOOL				vIsValid() {return LYNX_TRUE; };
			LYNXINLINE void					vGetDC(LYNXDC* pdc) {};
			LYNXINLINE void					vReleaseDC(LYNXDC dc) {};

			LYNXBOOL						vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable);
			LYNXBOOL						vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE);
			LYNXINLINE int					vGetNumMultiSamples() { return 0; };
			LYNXINLINE int					vGetWidth() { return m_Width; };
			LYNXINLINE int					vGetHeight() { return m_Height; };
			LYNXINLINE void					vGetSize(int* w, int* h) { *w = m_Width; *h = m_Height; };
			LYNXINLINE void					vSetSize(int w, int h) {m_Width = w;  m_Height = h; };
			
			LYNXINLINE LYNXBOOL				vSwapable() {return LYNX_FALSE; };
			LYNXINLINE void					vSwap() {};
			LYNXINLINE void					vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect) {};

			LYNXINLINE void					vSetAsRenderTarget(int index);
			LYNXINLINE void					vSetAsDepthBuffer() {};

			void							vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...);
			void							vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...); 
		};
	}		
}

#endif

#endif