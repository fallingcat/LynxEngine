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

#ifndef __LYNXOGLESBUFFER_H__
#define __LYNXOGLESBUFFER_H__

#include <GraphicsSystem/LynxBuffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#define GL_GLEXT_PROTOTYPES	1
	#include <GLES/gl.h>
	#include <GLES/egl.h>
	#include <GLES/glext.h>
#elif defined(__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>	
#elif defined(__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>	
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESBuffer : public CBuffer
		{
			LYNX_DECLARE_CLASS(COGLESBuffer);
		public:
			typedef enum {
				BT_RENDERBUFFER,
				BT_TEXTURE2D
			}BUFFERTYPE;				
		protected:			
			BUFFERTYPE						m_BufferType;
			GLuint							m_GLHandle;
            int                             m_Width, m_Height;
		private:
		public:						
			COGLESBuffer(CGraphicsSystem* lpsys);
			virtual ~COGLESBuffer();

			LYNXINLINE COGLESBuffer&		operator =(const COGLESBuffer& b) {m_lpGraphicsSystem = b.m_lpGraphicsSystem; return (*this); };		
			
			LYNXFORCEINLINE const BUFFERTYPE GetBufferType() const {return m_BufferType; };
			LYNXFORCEINLINE void			SetBufferType(const BUFFERTYPE t) {m_BufferType = t; };
			
			LYNXFORCEINLINE const GLuint	GetGLHandle() const {return m_GLHandle; };
			LYNXFORCEINLINE void			SetGLHandle(const GLuint& i) 
			{
				#ifdef __iOS__
					if (m_GLHandle) 
						glDeleteRenderbuffersOES(1, &m_GLHandle); 
				
					m_GLHandle = i; 
				#endif
			}		
			LYNXINLINE void*				vMap(int flag, LPLYNXRECT lprect = NULL) {return NULL; }; 
			LYNXINLINE void					vUnmap() {};
			
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_GLHandle?LYNX_TRUE:LYNX_FALSE); };
			LYNXINLINE void					vGetDC(LYNXDC* pdc) {};
			LYNXINLINE void					vReleaseDC(LYNXDC dc) {};
			
			LYNXINLINE LYNXBOOL				vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable);
			LYNXFORCEINLINE LYNXBOOL		vCreate(int type, int size, int len, void* data = NULL, int flag = CRenderer::DYNAMIC|CRenderer::CPU_WRITE) {return LYNX_FALSE; };
			LYNXINLINE int					vGetNumMultiSamples() { return 0; };
			LYNXFORCEINLINE int				vGetWidth() {return m_Width; }
			LYNXFORCEINLINE int				vGetHeight() {return m_Height; }
			LYNXFORCEINLINE void			vGetSize(int* w, int* h) { *w = vGetWidth(); *h = vGetHeight(); };	
			LYNXINLINE void					vSetSize(int w, int h) { m_Width = w; m_Height = h; };
			LYNXFORCEINLINE void			vSetAsRenderTarget(int index) 
			{
                //glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_lpEngine->GetlpGraphicsSystem() m_FrameBuffer);
				#ifdef __iOS__
					switch (m_BufferType)
					{
						case BT_RENDERBUFFER:						
							glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_GLHandle);  	
							glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES+index, GL_RENDERBUFFER_OES, m_GLHandle);
							break;						
						case BT_TEXTURE2D:			
							glBindTexture(GL_TEXTURE_2D, m_GLHandle);	
							glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES+index, GL_TEXTURE_2D, m_GLHandle, 0);
							m_lpGraphicsSystem->GetlpRenderer()->SetTexture(0, NULL);
							break;
					}
					#ifdef _DEBUG
						GLenum Status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
						if (Status != GL_FRAMEBUFFER_COMPLETE_OES) 
						{
							LYNX_LOG(glpLogger, CString(_T("Can't bind texture <"))+m_Name+CString(_T("> as render target!\n")));
						}
					#endif
				#endif
			}
			LYNXFORCEINLINE void			vSetAsDepthBuffer()
			{
				#ifdef __iOS__
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_GLHandle);  	
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_GLHandle);
				#endif
			}
			LYNXINLINE LYNXBOOL				vSwapable() {return LYNX_FALSE; };
			LYNXINLINE void					vSwap() {};
			LYNXINLINE void					vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect) {};
			
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