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

#ifndef __LYNXNULLBUFFER_H__
#define __LYNXNULLBUFFER_H__

#include <GraphicsSystem/LynxBuffer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullBuffer : public CBuffer
		{
			LYNX_DECLARE_CLASS(CNullBuffer);
		protected:			
		private:
		public:						
			CNullBuffer(CGraphicsSystem* lpsys):CBuffer(lpsys) {};
			virtual ~CNullBuffer() {};	

			LYNXINLINE CNullBuffer&			operator =(const CNullBuffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };		

			LYNXINLINE void*				vMap(int flag, LPLYNXRECT lprect = NULL) {return NULL; }; 
			LYNXINLINE void					vUnmap() {};

			LYNXINLINE LYNXBOOL				vIsValid() {return LYNX_TRUE; };
			LYNXINLINE void					vGetDC(LYNXDC* pdc) {};
			LYNXINLINE void					vReleaseDC(LYNXDC dc) {};

			LYNXINLINE LYNXBOOL				vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable) {return LYNX_TRUE; };
			LYNXINLINE LYNXBOOL				vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE) {return LYNX_FALSE; };
			LYNXINLINE int					vGetNumMultiSamples() { return 0; };
			LYNXINLINE int					vGetWidth() { return 0; };
			LYNXINLINE int					vGetHeight() { return 0; };
			LYNXINLINE void					vGetSize(int* w, int* h) { *w = 0; *h = 0; };
			LYNXINLINE void					vSetSize(int w, int h) { };
			LYNXINLINE void					vSetAsRenderTarget(int index) {};
			LYNXINLINE void					vSetAsDepthBuffer() {};
			LYNXINLINE LYNXBOOL				vSwapable() {return LYNX_FALSE; };
			LYNXINLINE void					vSwap() {};
			LYNXINLINE void					vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect) {};

			void vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...) 
			{
				LYNXCHAR				Buffer[512];
				va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
			}
			void vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...) 
			{
				LYNXCHAR				Buffer[512];
				va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
			}
		};
	}		
}

#endif