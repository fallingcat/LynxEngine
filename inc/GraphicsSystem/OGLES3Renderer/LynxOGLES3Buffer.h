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

#ifndef __LYNXOGLES3BUFFER_H__
#define __LYNXOGLES3BUFFER_H__

#include <GraphicsSystem/LynxBuffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__	
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#elif defined (__iOS__)
	#import <OpenGLES/ES3/gl.h>
	#import <OpenGLES/ES3/glext.h>
#elif defined (__ANDROID__)	
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#endif

namespace LynxEngine
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES3Buffer : public CBuffer
		{
			LYNX_DECLARE_CLASS(COGLES3Buffer);
		public:
			typedef enum {
				BT_RENDERBUFFER,
				BT_TEXTURE2D
			}BUFFERTYPE;				
		protected:			
			BUFFERTYPE						m_BufferType;
			GLuint							m_GLHandle;
            int                             m_Width, m_Height;
            GLuint							m_FrameBuffer;
		private:
		public:						
			COGLES3Buffer(CGraphicsSystem* lpsys);
			virtual ~COGLES3Buffer();

			LYNXINLINE COGLES3Buffer&		operator =(const COGLES3Buffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };		

			LYNXFORCEINLINE const BUFFERTYPE GetBufferType() const {return m_BufferType; };
			LYNXFORCEINLINE void			SetBufferType(const BUFFERTYPE t) {m_BufferType = t; };
			
			LYNXFORCEINLINE GLuint			GetFrameBuffer() {return m_FrameBuffer; };
			LYNXFORCEINLINE void			SetFrameBuffer(GLuint f) {m_FrameBuffer = f; };
					
			LYNXFORCEINLINE const GLuint	GetGLHandle() const {return m_GLHandle; };
			LYNXFORCEINLINE void			SetGLHandle(const GLuint& i)
			{
				if (m_GLHandle) 
				{
					GL_TRACE(glDeleteRenderbuffers(1, &m_GLHandle);)
				}
					
				m_GLHandle = i;
			}
            LYNXFORCEINLINE void			SetInfo(DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format)
            {
                m_Format = format;
                m_Width = w;
                m_Height = h;
            }
            LYNXFORCEINLINE void            GenerateFrameBuffer()
            {
                GL_TRACE(glGenFramebuffers(1, &m_FrameBuffer);)
            }
			LYNXINLINE void*				vMap(int flag, LPLYNXRECT lprect = NULL) {return NULL; }; 
			LYNXINLINE void					vUnmap() {};

			LYNXINLINE LYNXBOOL				vIsValid() {return (m_GLHandle?LYNX_TRUE:LYNX_FALSE); };
			LYNXINLINE void					vGetDC(LYNXDC* pdc) {};
			LYNXINLINE void					vReleaseDC(LYNXDC dc) {};

			LYNXINLINE LYNXBOOL				vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable);
			LYNXFORCEINLINE LYNXBOOL		vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE) {return LYNX_FALSE; };
			LYNXINLINE int					vGetNumMultiSamples() { return 0; };
			//LYNXFORCEINLINE int				vGetWidth() {return m_Width; }
			//LYNXFORCEINLINE int				vGetHeight() {return m_Height; }
			int                             vGetWidth();
			int                             vGetHeight();
			
            LYNXFORCEINLINE void			vGetSize(int* w, int* h) { *w = vGetWidth(); *h = vGetHeight(); };
			LYNXINLINE void					vSetSize(int w, int h) { m_Width = w; m_Height = h; };
			void                            vSetAsRenderTarget(int index);
			void                            vSetAsDepthBuffer();
			LYNXINLINE LYNXBOOL				vSwapable() {return LYNX_FALSE; };
			LYNXINLINE void					vSwap() {};
			void                            vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect);

			void vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...) 
			{
				LYNXCHAR				Buffer[512];
				LYNXCHAR				*Ch;	
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
			}
			void vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...) 
			{
				LYNXCHAR				Buffer[512];
				LYNXCHAR				*Ch;	
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
			}
		};
	}		
}

#endif

#endif