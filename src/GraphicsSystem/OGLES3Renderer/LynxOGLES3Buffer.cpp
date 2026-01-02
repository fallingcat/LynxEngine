//##########################################################################
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
//  ?°?˙:
//
//###########################################################################

#include <LynxEngine_PCH.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Renderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Buffer.h>

#ifdef __iOS__
	extern "C" void lynxiOSSaveImageDataToPhoto(void* data, int width, int height, int dataLength);
    extern "C" void lynxiOSGetImagePNGRepresentation(void** ppdata, int* len, void* data, int width, int height, int dataLength);
    extern "C" void lynxiOSGetImageJPGRepresentation(void** ppdata, int* len, void* data, int width, int height, int dataLength, float compress);
#endif

#ifdef __ANDROID__
	#include <Platform/Android/LynxAndroidGameFrameworkKernel.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3Buffer::COGLES3Buffer(CGraphicsSystem* lpsys)
		: CBuffer(lpsys) 		
		{				
			m_GLHandle = 0;
            m_Width = 0;
            m_Height = 0;
			m_BufferType = BT_RENDERBUFFER;
            m_FrameBuffer = 0;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3Buffer::~COGLES3Buffer()
		{
            if (m_FrameBuffer)
                glDeleteFramebuffers(1, &m_FrameBuffer);
            
			glDeleteRenderbuffers(1, &m_GLHandle); 			
			m_GLHandle = 0;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   If wnd == NULL means to create renderable buffer
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3Buffer::vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable) 
		{
            if (format == CRenderer::FORMAT_D16 || 
                format == CRenderer::FORMAT_D24X8 ||
                format == CRenderer::FORMAT_D24S8)
            {
                GL_TRACE(glGenRenderbuffers(1, &m_GLHandle));
                GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle));
                GL_TRACE(glRenderbufferStorage(GL_RENDERBUFFER, COGLES3Renderer::TranslateFormat(format), w, h));				
            }
            m_Format = format; 
            m_Width = w;
            m_Height = h;
            
            return LYNX_TRUE;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void COGLES3Buffer::vSetAsRenderTarget(int index)
        {
			/*GLint FrameBuffer = 0;
			GL_TRACE(glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_FrameBuffer);)
			if (FrameBuffer == 0)
				return;
				*/

			switch (m_BufferType)
            {
                case BT_RENDERBUFFER:
					//GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
					static_cast<COGLES3Renderer*>(m_lpGraphicsSystem->GetlpRenderer())->BindFrameBuffer();
					GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle);)
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_RENDERBUFFER, m_GLHandle);)
                    break;
                    
                case BT_TEXTURE2D:
					//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
					static_cast<COGLES3Renderer*>(m_lpGraphicsSystem->GetlpRenderer())->BindR2TFrameBuffer();
                    GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle));
					GL_TRACE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
					GL_TRACE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
					GL_TRACE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GL_TRACE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    GL_TRACE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, m_GLHandle, 0));
                    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GLHandle, 0);
                    break;
            }
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void COGLES3Buffer::vSetAsDepthBuffer()
        {
			/*GLint FrameBuffer = 0;
			GL_TRACE(glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_FrameBuffer);)
			if (FrameBuffer == 0)
				return;
				*/

            switch (m_BufferType)
            {
                case BT_RENDERBUFFER:
					GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle);)
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_GLHandle);)
					if (m_Format == CRenderer::FORMAT_D24S8)
						GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_GLHandle);)
                    break;
                    
                case BT_TEXTURE2D:
                    //static_cast<COGLES3Renderer*>(m_lpGraphicsSystem->GetlpRenderer())->BindR2TFrameBuffer();
                    GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle));
                    GL_TRACE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GLHandle, 0));
                    GL_TRACE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_GLHandle, 0));
                    break;
            }
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
		void COGLES3Buffer::vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect)
        {
            int X, Y;
            int Width, Height;
            
            if (rect)
            {
                X = rect->x1;
                Y = rect->y1;
                Width = rect->x2 - rect->x1;
                Height = rect->y2 - rect->y1;
            }
            else
            {
                X = 0;
                Y = 0;
                Width = m_Width;
                Height = m_Height;                
            }
            int dataLength = Width * Height * 4;
            GLubyte *data = (GLubyte*)malloc(dataLength * sizeof(GLubyte));
            
            // Read pixel data from the framebuffer
            GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle));  
            GL_TRACE(glPixelStorei(GL_PACK_ALIGNMENT, 4));
            GL_TRACE(glReadPixels(X, Y, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, data));
            
			#ifdef __iOS__
				//lynxiOSSaveImageDataToPhoto(data, Width, Height, dataLength);
                void* PNGData;
                int PNGDataLength;
                lynxiOSGetImagePNGRepresentation(&PNGData, &PNGDataLength, data, Width, Height, dataLength);
                //lynxiOSGetImageJPGRepresentation(&PNGData, &PNGDataLength, data, Width, Height, dataLength, 0.5f);
                CPlatformFileStream File;
                if (File.vOpen(name, CStream::Write|CStream::Binary))
                {
                    File.vWrite(PNGData, 1, PNGDataLength);
                    File.vClose();
                }
			#endif

			#ifdef __ANDROID__
				JNI_ATTACH_CURRENT_THREAD

				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID savePng = env->GetMethodID(cls, "SavePNG", "(Ljava/lang/String;[BIII)V");
				jbyteArray bitmapdata = env->NewByteArray (dataLength);
				env->SetByteArrayRegion (bitmapdata, 0, dataLength, (jbyte*)data);
				jstring str = env->NewModifiedUTF8String(env, name);
				env->CallVoidMethod(app->activity->clazz, savePng, str, bitmapdata, dataLength * sizeof(GLubyte), Width, Height);

				env->DeleteLocalRef(str);
				env->DeleteLocalRef(bitmapdata);

				JNI_DETACH_CURRENT_THREAD
			#endif
      
            // Clean up
            free(data);
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
        int COGLES3Buffer::vGetWidth()
        {
            //if (m_lpGraphicsSystem->GetlpRenderer()->IsPortraitMode())
            //    return m_Height;
            //else
                return m_Width;
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------
        int COGLES3Buffer::vGetHeight()
        {
            //if (m_lpGraphicsSystem->GetlpRenderer()->IsPortraitMode())
            //    return m_Width;
            //else
                return m_Height;
        }
	}
}

#endif

