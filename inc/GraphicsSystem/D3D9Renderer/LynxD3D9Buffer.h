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

#ifndef __LYNXD3D9BUFFER_H__
#define __LYNXD3D9BUFFER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/LynxBuffer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9Buffer : public CBuffer
		{
			LYNX_DECLARE_CLASS(CD3D9Buffer);
		protected:		
			IDirect3DSurface9*				m_lpSurface;		
			IDirect3DSwapChain9*			m_lpSwapChain;

			int								m_NumSamples;
			DWORD							m_Width;
			DWORD							m_Height;
			int								m_lPitch;
		private:
		public:						
			CD3D9Buffer(CGraphicsSystem* lpsys);
			virtual ~CD3D9Buffer();	

			LYNXINLINE CD3D9Buffer&			operator =(const CD3D9Buffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };					

			LYNXINLINE IDirect3DSurface9*	GetlpSurface() {return m_lpSurface; };
			void							SetlpSurface(IDirect3DSurface9* s);
			LYNXINLINE IDirect3DSwapChain9*	GetlpSwapChain() {return m_lpSwapChain; };
			LYNXINLINE void					SetlplpSwapChain(IDirect3DSwapChain9* s) {m_lpSwapChain = s; };
			LYNXINLINE int					GetPitch() {return m_lPitch; };					

			void*							vMap(int flag, LPLYNXRECT lprect = NULL); 
			void							vUnmap();

			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpSurface?LYNX_TRUE:LYNX_FALSE); };
			LYNXINLINE void					vGetDC(LYNXDC* pdc) {m_lpSurface->GetDC(pdc); };
			LYNXINLINE void					vReleaseDC(LYNXDC dc) {m_lpSurface->ReleaseDC(dc); };

			LYNXBOOL						vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable);
			LYNXBOOL						vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE) {return LYNX_FALSE; };
			LYNXINLINE int					vGetNumMultiSamples() { return m_NumSamples; };
			LYNXINLINE int					vGetWidth() { return m_Width; };
			LYNXINLINE int					vGetHeight() { return m_Height; };
			LYNXINLINE void					vGetSize(int* w, int* h) { *w = m_Width; *h = m_Height; };
			LYNXINLINE void					vSetSize(int w, int h) {};

			void							vSetAsRenderTarget(int index);
			void							vSetAsDepthBuffer();

			LYNXINLINE LYNXBOOL				vSwapable() {return (m_lpSwapChain?LYNX_TRUE:LYNX_FALSE); };
			LYNXINLINE void					vSwap() {m_lpSwapChain->Present(NULL, NULL, NULL, NULL, 0); };

			void							vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect);
			
			void							vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...); 
			void							vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...); 
		};		
	}		
}

#endif

#endif