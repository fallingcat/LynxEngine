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

#ifndef __LYNXBUFFER_H__
#define __LYNXBUFFER_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CBuffer : public CResource
		{
			LYNX_DECLARE_CLASS(CBuffer);
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;	
			CTexture*						m_lpTexture;
			CRenderer::FORMAT				m_Format;
			void*							m_lpBuffer;
		public:						
			CBuffer(CGraphicsSystem* lpsys);
			virtual ~CBuffer();	

			LYNXINLINE CBuffer&				operator =(const CBuffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };		

			void							SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };	
			LYNXFORCEINLINE const void*		GetlpBuffer() {return m_lpBuffer; };

			LYNXFORCEINLINE void			SetlpTexture(CTexture* t) {m_lpTexture = t; };
			LYNXFORCEINLINE CTexture*		GetlpTexture() {return m_lpTexture; };

			virtual LYNXBOOL				vIsValid() = 0;
			virtual void					vGetDC(LYNXDC* pdc) = 0;
			virtual void					vReleaseDC(LYNXDC dc) = 0;

			virtual LYNXBOOL				vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable) = 0;
			virtual LYNXBOOL				vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE) = 0;
			LYNXINLINE CRenderer::FORMAT	GetFormat() {return m_Format; };
			virtual int						vGetNumMultiSamples() = 0;
			virtual int						vGetWidth() = 0;
			virtual int						vGetHeight() = 0;
			virtual void					vGetSize(int* w, int* h) = 0;
			virtual void					vSetSize(int w, int h) = 0;
			virtual void					vSetAsRenderTarget(int index) = 0;
			virtual void					vSetAsDepthBuffer() = 0;
			virtual LYNXBOOL				vSwapable() = 0;
			virtual void					vSwap() = 0;
			virtual void					vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect) = 0;
			virtual void					vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...) = 0;		
			virtual void					vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...) = 0;		
			virtual void*					vMap(int flag, LPLYNXRECT lprect = NULL) = 0; 
			virtual void					vUnmap() = 0;
		};
	}		
}

#endif