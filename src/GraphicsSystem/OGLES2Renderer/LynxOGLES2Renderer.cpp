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

#include <LynxEngine_PCH.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#ifdef __ANDROID__
	#include <android/native_window_jni.h>
#endif

void lynxiOSSetupGLColorBufferFromContext();

#ifdef __ANDROID__
	#if !GL_OES_vertex_array_object || !GL_GLEXT_PROTOTYPES
		PFNGLBINDVERTEXARRAYOESPROC							glBindVertexArrayOES			= NULL;
		PFNGLDELETEVERTEXARRAYSOESPROC						glDeleteVertexArraysOES			= NULL;
		PFNGLGENVERTEXARRAYSOESPROC							glGenVertexArraysOES			= NULL;
	#endif
#else
	#if !GL_OES_vertex_array_object
		PFNGLBINDVERTEXARRAYOESPROC							glBindVertexArrayOES			= NULL;
		PFNGLDELETEVERTEXARRAYSOESPROC						glDeleteVertexArraysOES			= NULL;
		PFNGLGENVERTEXARRAYSOESPROC							glGenVertexArraysOES			= NULL;
	#endif
#endif

namespace LynxEngine
{
	namespace GraphicsSystem
	{
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
        void LogGLError(const CString& message)
        {
			GLenum Error;

			do {
				Error = glGetError();

				switch (Error)
				{
						//default:
						//    LYNX_LOG(glpLogger, CString(_T("[Error] glTexImage2D failed!\n")));
						//    break;
                    
					case GL_NO_ERROR:
						break;
                    
					case GL_INVALID_ENUM:
						LYNX_LOG(glpLogger, message + CString(_T("<GL_INVALID_ENUM>")));
						break;
                    
					case GL_INVALID_VALUE:
						LYNX_LOG(glpLogger, message + CString(_T("<GL_INVALID_VALUE>")));
						break;
                    
					case GL_INVALID_OPERATION:
						LYNX_LOG(glpLogger, message + CString(_T("<GL_INVALID_OPERATION>")));
						break;
                    
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						LYNX_LOG(glpLogger, message + CString(_T("<GL_INVALID_FRAMEBUFFER_OPERATION>")));
						break;
                    
					case GL_OUT_OF_MEMORY:
						LYNX_LOG(glpLogger, message + CString(_T("<GL_OUT_OF_MEMORY>")));
						break;
                    
				}
			}while (Error != GL_NO_ERROR);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		int COGLES2Renderer::GetIndexCount(const CRenderer::PRIMITIVETYPE type, const int primitive_count)
		{
			switch (type)
			{
				case CRenderer::TRIANGLE_LIST:
					return primitive_count*3;
				case CRenderer::TRIANGLE_STRIP:
				case CRenderer::TRIANGLE_FAN:
					return primitive_count + 2;
			};
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e)
		{
			GLenum PType;
			
			switch (e)
			{
				case CRenderer::POINT_LIST:
					PType = GL_POINTS;
					break;
				case CRenderer::LINE_LIST:
					PType = GL_LINES;
					break;
				case CRenderer::LINE_STRIP:
					PType = GL_LINE_STRIP;
					break;			
				case CRenderer::TRIANGLE_LIST:
					PType = GL_TRIANGLES;
					break;		
				case CRenderer::TRIANGLE_STRIP:
					PType = GL_TRIANGLE_STRIP;
					break;
				case CRenderer::TRIANGLE_FAN:
					PType = GL_TRIANGLE_FAN;
					break;	
				default:
					PType = GL_TRIANGLES;
					break;	
			}
			return PType;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLint COGLES2Renderer::TranslateAddressMode(CRenderer::ADDRESSMODE e)
		{
			switch (e)
			{
				default:
				case AM_WRAP:
					return GL_REPEAT;
					break;
				case AM_MIRROR:
					//Only for OGLES2 2.x
					return GL_MIRRORED_REPEAT;
					break;
				case AM_CLAMP:
					return GL_CLAMP_TO_EDGE;
					break;				
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum	COGLES2Renderer::TranslateCullMode(CRenderer::CULLMODE e)
		{
			switch (e)
			{
				case CRenderer::CULL_CW:
					return GL_CCW;
					break;
				default:
				case CRenderer::CULL_CCW:
					return GL_CW;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateTestFunction(CRenderer::TESTFUNC e)
		{
			switch (e)
			{
				case CRenderer::TF_NEVER:
					return GL_NEVER;
					break;				
				case CRenderer::TF_LESS:
					return GL_LESS;
					break;
				case CRenderer::TF_EQUAL:
					return GL_EQUAL;
					break;
				case CRenderer::TF_LEQUAL:
					return GL_LEQUAL;
					break;				
				case CRenderer::TF_GREATER:
					return GL_GREATER;
					break;
				case CRenderer::TF_NOTEQUAL:
					return GL_NOTEQUAL;
					break;
				case TF_GEQUAL:
					return GL_GEQUAL;
					break;
				default:
				case TF_ALWAYS:
					return GL_ALWAYS;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateStencilOP(CRenderer::STENCILOP e)
		{
			switch (e)
			{
				default:
				case CRenderer::SO_KEEP:
					return GL_KEEP;
					break;				
				case CRenderer::SO_ZERO:
					return GL_ZERO;
					break;
				case CRenderer::SO_REPLACE:
					return GL_REPLACE;
					break;
				case CRenderer::SO_INCR:
					return GL_INCR;
					break;				
				case CRenderer::SO_DECR:
					return GL_DECR;
					break;				
				case CRenderer::SO_INVERT:
					return GL_INVERT;
					break;				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateBlendFunction(CRenderer::BLENDFUNC e)
		{
			switch (e)
			{
				case BF_ZERO:
					return GL_ZERO;
					break;
				default:
				case BF_ONE:
					return GL_ONE;
					break;
				case BF_SRC_COLOR:
					return GL_SRC_COLOR;
					break;
				case BF_ONE_MINUS_SRC_COLOR:
					return GL_ONE_MINUS_SRC_COLOR;
					break;
				case BF_SRC_ALPHA:
					return GL_SRC_ALPHA;
					break;
				case BF_ONE_MINUS_SRC_ALPHA:
					return GL_ONE_MINUS_SRC_ALPHA;
					break;
				case BF_DEST_ALPHA:
					return GL_DST_ALPHA;
					break;
				case BF_ONE_MINUS_DEST_ALPHA:
					return GL_ONE_MINUS_DST_ALPHA;
					break;
				case BF_DEST_COLOR:
					return GL_DST_COLOR;
					break;
				case BF_ONE_MINUS_DEST_COLOR:
					return GL_ONE_MINUS_DST_COLOR;
					break;
				case BF_SRC_ALPHA_SAT:
					return GL_SRC_ALPHA;
					break;				
			}
		}
		/*
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLuint COGLES2Renderer::TranslateVertexSlotType(CRenderer::VERTEXSLOTTYPE e)
		{
			switch (e)
			{
				default:					
				case VST_POSITION:
					return GL_VERTEX_ARRAY;
					break;
				case VST_TANGENT:
					return 0;
					break;
				case VST_NORMAL:
					return GL_NORMAL_ARRAY;
					break;
				case VST_BINORMAL:
					return 0;
					break;
				case VST_TEX_COORD:
					return GL_TEXTURE_COORD_ARRAY;
					break;
				case VST_COLOR:
					return GL_COLOR_ARRAY;
					break;
				case VST_BLEND_WEIGHT:
					return 0;
					break;
				case VST_BLEND_INDICES:
					return 0;
					break;
				case VST_DEPTH:
					return 0;
					break;
				case VST_POINT_SIZE:
					return 0;
					break;
			}
		}
		*/
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateVertexSlotFormat(CRenderer::VERTEXSLOTFORMAT e)
		{
			switch (e)
			{
				default:
				case VSF_FLOAT:
					return GL_FLOAT;
					break;					
				case VSF_UBYTE:
					return GL_UNSIGNED_BYTE;
					break;
				case VSF_SHORT:
					return GL_SHORT;
					break;			
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateFormat(CRenderer::FORMAT f)
		{
			switch (f)
			{
				default:
				case CRenderer::FORMAT_R8G8B8:
					return GL_RGB;
					break;
				case CRenderer::FORMAT_R5G6B5:
					return GL_RGB;
					break;				
				case CRenderer::FORMAT_A1R5G5B5:
					return GL_RGBA;
					break;				
				case CRenderer::FORMAT_A4R4G4B4:
					return GL_RGBA;
					break;						
				case CRenderer::FORMAT_A8R8G8B8:
					return GL_RGBA;
					break;
                case CRenderer::FORMAT_L8:
					return GL_LUMINANCE;
					break;
				case CRenderer::FORMAT_RGBA_PVRTC_2:
					return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
					break;
				case CRenderer::FORMAT_RGB_PVRTC_2:
					return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
					break;
				case CRenderer::FORMAT_RGBA_PVRTC_4:
					return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
					break;
				case CRenderer::FORMAT_RGB_PVRTC_4:
					return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
					break;
				//case CRenderer::FORMAT_RGB8_ETC1:
				//	return GL_ETC1_RGB8_OES;
                case CRenderer::FORMAT_D16:
					return GL_DEPTH_COMPONENT16_OES;
					break;				
                case CRenderer::FORMAT_D24X8:
					return GL_DEPTH_COMPONENT24_OES;
					break;
				case CRenderer::FORMAT_D24S8:
					return GL_DEPTH24_STENCIL8_OES;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLES2Renderer::TranslateFormatToPixelType(CRenderer::FORMAT f)
		{
			switch (f)
			{
				case CRenderer::FORMAT_R5G6B5:
					return GL_UNSIGNED_SHORT_5_6_5;
					break;				
				case CRenderer::FORMAT_A1R5G5B5:
					return GL_UNSIGNED_SHORT_5_5_5_1;
					break;				
				case CRenderer::FORMAT_A4R4G4B4:
					return GL_UNSIGNED_SHORT_4_4_4_4;
					break;					
				case CRenderer::FORMAT_X8R8G8B8:
				case CRenderer::FORMAT_R8G8B8:					
				case CRenderer::FORMAT_A8R8G8B8:					
				case CRenderer::FORMAT_RGBA_PVRTC_2:
				case CRenderer::FORMAT_RGB_PVRTC_2:
				case CRenderer::FORMAT_RGBA_PVRTC_4:
				case CRenderer::FORMAT_RGB_PVRTC_4:
					return GL_UNSIGNED_BYTE;
					break;
				default:
					LYNX_ASSERT(0);
					return GL_UNSIGNED_BYTE;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		COGLES2Renderer::COGLES2Renderer() 
		{
			m_RendererType = OGLES2_RENDERER;
			m_FrameBuffer = 0;
            m_R2TFrameBuffer = 0;
			m_MSFrameBuffer = 0;

			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_FALSE;
			m_bCacheShaderGroup = LYNX_TRUE;
			m_bUseCachedState = LYNX_TRUE;
			m_bInvertRenderTarget = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		COGLES2Renderer::COGLES2Renderer(CGraphicsSystem* lpgs)
		: CRenderer(lpgs) 
		{			
			m_RendererType = OGLES2_RENDERER;
			m_FrameBuffer = 0;
            m_R2TFrameBuffer = 0;
			m_MSFrameBuffer = 0;
						
			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_FALSE;
			m_bCacheShaderGroup = LYNX_TRUE;
			m_bUseCachedState = LYNX_TRUE;
			m_bInvertRenderTarget = LYNX_TRUE;
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		COGLES2Renderer::~COGLES2Renderer() 
		{						
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString& COGLES2Renderer::ComputeCachedCompiledShaderPath() 
		{
			CString				Target;
			LYNXCHAR			Buffer[16];
			
			Target = LynxEngine::GetEnumStr(m_ShaderTarget);
			m_CachedCompiledShaderPath = CString(_T("../cached/graphics/")) + CString(GetAPIString()) + CString(_T("/")) + CString(GetDriverDesc()) + CString(_T("/shader/")) + Target + CString(_T("/"));						
			m_ShaderSrcPath.clear();			
			m_ShaderSrcPath.push_back(CString(_T("../engine/shader/ogles/")));			
			m_ShaderSrcPath.push_back(CString(_T("../shader/ogles/")));							
			m_ShaderSrcPath.push_back(CString(_T("../../engine/shader/ogles/")));			
			
			switch (m_ShaderTarget)
			{
				case SHADER_TARGET_11:
					AddDefinedValue(CAnsiString("SM_11"), CAnsiString("1"));
					break;
				case SHADER_TARGET_20:
					AddDefinedValue(CAnsiString("SM_20"), CAnsiString("1"));
					break;
				case SHADER_TARGET_2X:
					AddDefinedValue(CAnsiString("SM_2X"), CAnsiString("1"));
					break;
				case SHADER_TARGET_30:
					AddDefinedValue(CAnsiString("SM_30"), CAnsiString("1"));
					break;
				case SHADER_TARGET_40:
					AddDefinedValue(CAnsiString("SM_40"), CAnsiString("1"));
					break;
				case SHADER_TARGET_41:
					AddDefinedValue(CAnsiString("SM_41"), CAnsiString("1"));
					break;
				case SHADER_TARGET_50:
					AddDefinedValue(CAnsiString("SM_50"), CAnsiString("1"));
					break;
				default:
					AddDefinedValue(CAnsiString("SM_30"), CAnsiString("1"));
					break;
			}			
			AddDefinedValue(CAnsiString("PCF_4X4"), CAnsiString("1"));
			
			// TODO : Adjust the batch size according to the HW
			/*
			CSprite3DClusterContainer::m_BatchSize = 64;
			CSpriteGrassContainer::m_BatchSize = 64;
			CCrossSpriteGrassContainer::m_BatchSize = 64;
			
			lynxSprintf(Buffer, _T("%d"), CSprite3DClusterContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("SPRITE3DCLUSTER_BATCH_SIZE"), CAnsiString(Buffer));
			
			lynxSprintf(Buffer, _T("%d"), CSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("GRASS_BATCH_SIZE"), CAnsiString(Buffer));
			
			lynxSprintf(Buffer, _T("%d"), CCrossSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("CROSSGRASS_BATCH_SIZE"), CAnsiString(Buffer));			
			
			lynxSprintf(Buffer, _T("%d"), MAX_NUM_TEXTURE_CHANNEL);
			AddDefinedValue(CAnsiString("MAX_TEXTURE_CHANNELS"), CAnsiString(Buffer));	
			*/
			
			//AddDefinedValue(CString("PCF_2X2"), CString("1"));
			//AddDefinedValue(CString("PARALLAX"), CString("1"));
			
			return m_CachedCompiledShaderPath; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Renderer::Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Open()....\r\n"));
			lynxDebugOutputPrintf(_T("Open()....\r\n"));

			m_Window = wnd; 			

            lynxOSCreateMutex(&m_Mutex);
			m_bMutexLocked = LYNX_FALSE;
            
			GLenum DepthFormat = TranslateFormat(lpcp->DepthBufferFormat);
			
            #ifdef __iOS__
                #ifdef __GLKVIEW__
					glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_FrameBuffer);

                    GLint GLHandle;
                    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0, &GLHandle);
                    m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
                    m_lpBackBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat, 0, LYNX_FALSE);
                    m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
                    ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(GLHandle);
                    
                    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, &GLHandle);
                    m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
                    m_lpDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
                    m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
                    ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(GLHandle);
                #else
                    if (lpcp->NumMultiSamples <= 1)
                    {
                        glGenFramebuffers(1, &m_FrameBuffer);
                        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
                        
                        GLuint ColorBuffer, DepthBuffer;
                        glGenRenderbuffers(1, &ColorBuffer);
                        glBindRenderbuffer(GL_RENDERBUFFER, ColorBuffer);
                        lynxiOSSetupGLColorBufferFromContext();
                        //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBuffer);
                        m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
                        m_lpBackBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat, 0, LYNX_FALSE);
                        m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
                        ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(ColorBuffer);
						((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetFrameBuffer(m_FrameBuffer);
                        
                        m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
                        m_lpDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
                        m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->GetGLHandle());
                        // Attach to stencil
                        if (lpcp->DepthBufferFormat == CRenderer::FORMAT_D24S8)
                        {
                            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->GetGLHandle());
                        }
                        
                        /*
                        CTexturePtr DepthTexture;
                        GetlpEngine()->CreateResource(DepthTexture, TEXTURE);
                        if (!DepthTexture->vCreate(_T("SystemDepthBuffer"), lpcp->Width, lpcp->Height, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, CRenderer::FORMAT_D24S8))
                            return LYNX_FALSE;
                        m_lpDepthBuffer = CBufferPtr(DepthTexture->GetSrcBuffer());
						*/
                        
                        glGenFramebuffers(1, &m_R2TFrameBuffer);
                        glBindFramebuffer(GL_FRAMEBUFFER, m_R2TFrameBuffer);
                        
                        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
                    }
                    else 
                    {
                        glGenFramebuffers(1, &m_FrameBuffer);
                        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
                        
                        GLuint ColorBuffer;
                        glGenRenderbuffers(1, &ColorBuffer);
                        glBindRenderbuffer(GL_RENDERBUFFER, ColorBuffer);
                        //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
                        lynxiOSSetupGLColorBufferFromContext();
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBuffer);
                        m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpResolvedBackBuffer, LynxEngine::BUFFER);
                        m_lpResolvedBackBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat, 0, LYNX_FALSE);
                        m_lpResolvedBackBuffer->SetName(_T("SystemResolvedBackBuffer"));
                        ((COGLES2Buffer*)((CBuffer*)m_lpResolvedBackBuffer))->SetGLHandle(ColorBuffer);

                        GLuint MSColorBuffer, MSDepthBuffer;
                        glGenFramebuffers(1, &m_MSFrameBuffer);
                        glBindFramebuffer(GL_FRAMEBUFFER, m_MSFrameBuffer);
                        
                        glGenRenderbuffers(1, &MSColorBuffer);
                        glBindRenderbuffer(GL_RENDERBUFFER, MSColorBuffer);
                        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, lpcp->NumMultiSamples, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, MSColorBuffer);
                        m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
                        m_lpBackBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat, lpcp->NumMultiSamples, LYNX_FALSE);
                        m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
                        ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(MSColorBuffer);
                        ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetFrameBuffer(m_FrameBuffer);

                        glGenRenderbuffers(1, &MSDepthBuffer);
                        glBindRenderbuffer(GL_RENDERBUFFER, MSDepthBuffer);
                        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, lpcp->NumMultiSamples, DepthFormat, lpcp->Width, lpcp->Height);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MSDepthBuffer);
                        m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
                        m_lpDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, lpcp->NumMultiSamples, LYNX_FALSE);
                        m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
                        ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(MSDepthBuffer);
                        
                        glGenFramebuffers(1, &m_R2TFrameBuffer);
                        glBindFramebuffer(GL_FRAMEBUFFER, m_R2TFrameBuffer);
                        
                        glBindRenderbuffer(GL_RENDERBUFFER, MSDepthBuffer);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MSDepthBuffer);
                        
                        glBindFramebuffer(GL_FRAMEBUFFER, m_MSFrameBuffer);
                    }
                                                
                    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
                    {
                        LYNX_LOG(glpLogger, CString(GetClassName())+CString("::[Error] Failed to make complete framebuffer object!\r\n"));
                        return LYNX_FALSE;
                    }

					//m_lpOffscreenDepthBuffer = m_lpDepthBuffer;
            
                    m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpOffscreenDepthBuffer, LynxEngine::BUFFER);
                    m_lpOffscreenDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
                    m_lpOffscreenDepthBuffer->SetName(_T("OffscreenDepthBuffer"));
                #endif
            #else
				EGLBoolean bRet;

				#ifndef __TOOL__
					lynxOSMoveWindow(m_Window, 0, 0, lpcp->Width, lpcp->Height, LYNX_TRUE);			
				#endif

				// get egl display handle
				m_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
				if (m_EglDisplay == EGL_NO_DISPLAY)
				{
					//printf("Could not get EGL display\n");
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}

				// Initialize the display
				EGLint Major = 0;
				EGLint Minor = 0;
				bRet = eglInitialize(m_EglDisplay, &Major, &Minor);
				if (!bRet)
				{
					//printf("Could not initialize EGL display\n");
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}
				/*
				if (Major < 1 || Minor < 4)
				{
					// Does not support EGL 1.4
					//printf("System does not support at least EGL 1.4\n");
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}
				*/

				// Obtain the first configuration with a depth buffer
				const EGLint Attrs[] = {
						EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
						EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
						EGL_BLUE_SIZE, 8,
						EGL_GREEN_SIZE, 8,
						EGL_RED_SIZE, 8,
						//EGL_DEPTH_SIZE, 24, 
						//EGL_STENCIL_SIZE, 8, 
						EGL_NONE
				};

				const EGLint ContextAttrs[] = {
						EGL_CONTEXT_CLIENT_VERSION, 2, 
						EGL_NONE
				};
				EGLint NumConfig = 0;
				EGLConfig EglConfig = 0;
				bRet = eglChooseConfig(m_EglDisplay, Attrs, &EglConfig, 1, &NumConfig);
				if (!bRet || NumConfig == 0)
				{
					//printf("Could not find valid EGL config\n");
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}

				// Get the native visual id
				int NativeVid;
				if (!eglGetConfigAttrib(m_EglDisplay, EglConfig, EGL_NATIVE_VISUAL_ID, &NativeVid))
				{
					//printf("Could not get native visual id\n");
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}

				#ifdef __ANDROID__
					ANativeWindow_setBuffersGeometry(m_Window, lpcp->Width, lpcp->Height, NativeVid);
				#endif

				// Create a surface for the main window
				m_EglSurface = eglCreateWindowSurface(m_EglDisplay, EglConfig, m_Window, NULL);
				if (m_EglSurface == EGL_NO_SURFACE)
				{
					//printf("Could not create EGL surface\n");
					//DestroyNativeWin(nativeDisplay, nativeWin);
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}		

				// Create an OpenGL ES context
				m_EglContext = eglCreateContext(m_EglDisplay, EglConfig, 0, ContextAttrs);
				if (m_EglContext == EGL_NO_CONTEXT)
				{
					//printf("Could not create EGL context\n");
					//DestroyNativeWin(nativeDisplay, nativeWin);
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}

				// Make the context and surface current
				bRet = eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_EglContext);
				if(!bRet)
				{
					//printf("Could not activate EGL context\n");
					//DestroyNativeWin(nativeDisplay, nativeWin);
					//CloseNativeDisplay(nativeDisplay);
					return LYNX_FALSE;
				}

				m_LoadingContext = eglCreateContext(m_EglDisplay, EglConfig, m_EglContext, ContextAttrs);

				GLuint ColorBuffer, DepthBuffer;
				m_FrameBuffer = 0;
				GL_TRACE(glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_FrameBuffer);)
				GLint GLHandle;
                //glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &GLHandle);
                m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
                m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
                ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(0);
				((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetFrameBuffer(0);
                ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetInfo(lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat);

                //glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &GLHandle);
				#if 0                
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
					m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
					((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(0);
					((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetFrameBuffer(0);
					((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetInfo(lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat);
					GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, 0));
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
				#else
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
					m_lpDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
					m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->GetGLHandle());)
					// Attach to stencil
					if (lpcp->DepthBufferFormat == CRenderer::FORMAT_D24S8)
					{
						GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->GetGLHandle());)
					}
				#endif
				
				GL_TRACE(glGenFramebuffers(1, &m_R2TFrameBuffer));
				GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, m_R2TFrameBuffer));           
				
				GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

				//m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpOffscreenDepthBuffer, LynxEngine::BUFFER);
                //m_lpOffscreenDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
                //m_lpOffscreenDepthBuffer->SetName(_T("OffscreenDepthBuffer"));
				m_lpOffscreenDepthBuffer = m_lpDepthBuffer;

				/*
				glGenFramebuffers(1, &m_FrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
				
				GLuint ColorBuffer, DepthBuffer;
				glGenRenderbuffers(1, &ColorBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, ColorBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, lpcp->Width, lpcp->Height);				
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBuffer);
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
				m_lpBackBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->FrameBufferFormat, 0, LYNX_FALSE);
				m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
				((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(ColorBuffer);
					
				glGenRenderbuffers(1, &DepthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, DepthFormat, lpcp->Width, lpcp->Height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
				m_lpDepthBuffer->vCreate(m_Window, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, 0, LYNX_FALSE);
				m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
				((COGLES2Buffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(DepthBuffer);
				
				glGenFramebuffers(1, &m_R2TFrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, m_R2TFrameBuffer);
            
				glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);
            
				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
				
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
				{
					LYNX_LOG(glpLogger, CString(GetClassName())+CString("::[Error] Failed to make complete framebuffer object!\r\n"));
					return LYNX_FALSE;
				}
				*/
			#endif						
			GLint Param;
			GL_TRACE(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &Param);)
			m_Capbilities.MaxVertexShaderConstants = Param;  

			CVoxelClusterContainer::m_BatchSize = (m_Capbilities.MaxVertexShaderConstants - 20) / 3;
			CVoxelClusterContainer::m_SpriteBatchSize = (m_Capbilities.MaxVertexShaderConstants - 20) / 3;
			CSpriteClusterContainer::m_BatchSize = (m_Capbilities.MaxVertexShaderConstants - 20) / 3;
			CSprite3DClusterContainer::m_BatchSize = (m_Capbilities.MaxVertexShaderConstants - 20) / 4;

			SetViewportRect(0, 0, lpcp->Width, lpcp->Height);
			SetScissorRect(0, 0, lpcp->Width, lpcp->Height);
            EnableScissorRect(LYNX_FALSE);
            GL_TRACE(glDisable(GL_POLYGON_OFFSET_FILL);)
			//glDepthRangef(0.0f, 1.0f);
			//glEnable(GL_POLYGON_OFFSET_FILL);
			//glPolygonOffset(0.1f, 0.0f);
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			//glClearColor(1, 0, 0, 1);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//eglSwapBuffers(m_EglDisplay, m_EglSurface);

			m_Extensions = (LYNXANSICHAR*)glGetString(GL_EXTENSIONS);
			m_Version = (LYNXANSICHAR*)glGetString(GL_VERSION);
			m_Vendor = (LYNXANSICHAR*)glGetString(GL_VENDOR);
			m_Renderer = (LYNXANSICHAR*)glGetString(GL_RENDERER);
			
			#ifdef __ANDROID__
				if (m_Extensions.find("GL_OES_vertex_array_object") != CAnsiString::npos)
				{
					if (!glBindVertexArrayOES)
						glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress ( "glBindVertexArrayOES" );			
					if (!glDeleteVertexArraysOES)
						glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress ( "glDeleteVertexArraysOES" );
					if (!glGenVertexArraysOES)
						glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress ( "glGenVertexArraysOES" );
				}
			#endif

			LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Open() OK\r\n"));

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::Close()
		{
			CRenderer::Close();

			if (m_FrameBuffer)
				glDeleteFramebuffers(1, &m_FrameBuffer);
			if (m_R2TFrameBuffer)
				glDeleteFramebuffers(1, &m_R2TFrameBuffer);
            if (m_MSFrameBuffer)
				glDeleteFramebuffers(1, &m_MSFrameBuffer);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Renderer::RecreateSurface(LYNXWINDOW wnd)
		{
			LYNXBOOL bRet;

            #ifdef __iOS__
            #else
                m_Window = wnd;
                // Obtain the first configuration with a depth buffer
                const EGLint Attrs[] = {
                        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                        EGL_BLUE_SIZE, 8,
                        EGL_GREEN_SIZE, 8,
                        EGL_RED_SIZE, 8,
                        //EGL_DEPTH_SIZE, 24, 
                        //EGL_STENCIL_SIZE, 8, 
                        EGL_NONE
                };

                const EGLint ContextAttrs[] = {
                        EGL_CONTEXT_CLIENT_VERSION, 2, 
                        EGL_NONE
                };

                m_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                if (m_EglDisplay == EGL_NO_DISPLAY)
                {
                    //printf("Could not get EGL display\n");
                    //CloseNativeDisplay(nativeDisplay);
                    return LYNX_FALSE;
                }

                EGLint NumConfig = 0;
                EGLConfig EglConfig = 0;
                bRet = eglChooseConfig(m_EglDisplay, Attrs, &EglConfig, 1, &NumConfig);
                if (!bRet || NumConfig == 0)
                {
                    //printf("Could not find valid EGL config\n");
                    //CloseNativeDisplay(nativeDisplay);
                    return LYNX_FALSE;
                }

                // Get the native visual id
                int NativeVid;
                if (!eglGetConfigAttrib(m_EglDisplay, EglConfig, EGL_NATIVE_VISUAL_ID, &NativeVid))
                {
                    //printf("Could not get native visual id\n");
                    //CloseNativeDisplay(nativeDisplay);
                    return LYNX_FALSE;
                }

                #ifdef __ANDROID__
                    ANativeWindow_setBuffersGeometry(m_Window, GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(), GetlpEngine()->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight(), NativeVid);
                #endif

                // Create a surface for the main window
                m_EglSurface = eglCreateWindowSurface(m_EglDisplay, EglConfig, m_Window, NULL);
                if (m_EglSurface == EGL_NO_SURFACE)
                {
                    //printf("Could not create EGL surface\n");
                    //DestroyNativeWin(nativeDisplay, nativeWin);
                    //CloseNativeDisplay(nativeDisplay);
                    return LYNX_FALSE;
                }		
                
                // Make the context and surface current
                bRet = eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_EglContext);
                if(!bRet)
                {
                    //printf("Could not activate EGL context\n");
                    //DestroyNativeWin(nativeDisplay, nativeWin);
                    //CloseNativeDisplay(nativeDisplay);
                    return LYNX_FALSE;
                }
            #endif

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::Pause()
		{

		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::Resume(LYNXWINDOW wnd)
		{
			RecreateSurface(wnd);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::SetMainContext()
		{
            #if (defined __WIN32__ || defined __ANDROID__)
                eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_EglContext);
            #endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::SetLoadingContext()
		{
			#if (defined __WIN32__ || defined __ANDROID__)
				eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_LoadingContext);
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::End(void) 
		{ 
			LOCK_MUTEX 
			GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpBackBuffer))->GetGLHandle());) 
			UNLOCK_MUTEX 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
         void  COGLES2Renderer::Finish(void) 
		 { 
			LOCK_MUTEX 
			GL_TRACE(glFinish();) 
			UNLOCK_MUTEX 
		 }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
         void COGLES2Renderer::Flush(void) 
		 {
			LOCK_MUTEX 
			GL_TRACE(glFlush();) 
			UNLOCK_MUTEX 
		 }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::SetViewportRect(int x1, int y1, int x2, int y2) 
		{
			LOCK_MUTEX 
			CRenderer::SetViewportRect(x1, y1, x2, y2); 
			GetCachedRenderBuffer(0) ? glViewport(x1, GetCachedRenderBuffer(0)->vGetHeight()-y2, x2-x1, y2-y1) : glViewport(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1);
			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::SetScissorRect(int x1, int y1, int x2, int y2) 
		{
			LOCK_MUTEX 
			GetCachedRenderBuffer(0) ? glScissor(x1, GetCachedRenderBuffer(0)->vGetHeight()-y2, x2-x1, y2-y1) : glScissor(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1); 
			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::GetScissorRect(LPLYNXRECT r) 
		{
			LOCK_MUTEX 
			GLint Param[4];  
			GL_TRACE(glGetIntegerv(GL_SCISSOR_BOX, Param);) 
			r->x1 = Param[0]; 
			r->y1 = GetSystemBackBuffer()->vGetHeight() - Param[3 ]- Param[1]; 
			r->x2 = r->x1 + Param[2]; 
			r->y2 = r->y1 + Param[3]; 
			UNLOCK_MUTEX 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Renderer::EnableScissorRect(LYNXBOOL b) 
		{ 
			LOCK_MUTEX 
			LYNXBOOL bTemp = m_bCachedScissorEnabled; (b) ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST); 
			m_bCachedScissorEnabled = b; 
			UNLOCK_MUTEX 
			return bTemp; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		CBuffer* COGLES2Renderer::SetRenderBuffer(int index, CBuffer* lpb) 
		{ 
			if (m_bUseCachedState)
			{
				if (lpb == m_CachedRT[index]) 
					return lpb;
			}
            //DiscardRenderBuffer(index);
			CBuffer* lpOld = m_CachedRT[index];
			m_CachedRT[index] = lpb; 
			if (lpb)
            {
				lpb->vSetAsRenderTarget(index);
            }
			else
			{
				LOCK_MUTEX

				//GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, 0x00);) 	
                GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0x00);)

				UNLOCK_MUTEX
			}
			return lpOld;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		CBuffer* COGLES2Renderer::SetDepthBuffer(CBuffer* lpzb)
		{ 
			if (m_bUseCachedState)
			{
				if (lpzb == m_CachedDB) 
					return lpzb;
			}
				
			CBuffer* lpOld = m_CachedDB;
			m_CachedDB = lpzb; 
            if (lpzb)
				lpzb->vSetAsDepthBuffer();
			else
			{
				LOCK_MUTEX

				//GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, 0x00);) 	
				GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0x00);)
                GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0x00);)

				UNLOCK_MUTEX
			}
			return lpOld;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
        void COGLES2Renderer::DiscardRenderBuffer(int index)
        {
            //return;

			LOCK_MUTEX
			#ifdef __iOS__
				static const GLenum Discards[]  = {GL_COLOR_ATTACHMENT0};
				if (glDiscardFramebufferEXT)
					glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, Discards);
			#endif
			UNLOCK_MUTEX
        }   
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
        void COGLES2Renderer::DiscardDepthBuffer()
        {
            //return;

			LOCK_MUTEX
			#ifdef __iOS__
				static const GLenum Discards[]  = {GL_DEPTH_ATTACHMENT};
				if (glDiscardFramebufferEXT)
					glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, Discards);
			#endif
			UNLOCK_MUTEX
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s)
		{	
			LOCK_MUTEX

			GLbitfield Flag = 0;
			
            //const GLenum discards[]  = {GL_COLOR_ATTACHMENT0};
            //glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, discards);
            
			if (flag & RENDER_BUFFER)
			{
				Flag |= GL_COLOR_BUFFER_BIT;
				GL_TRACE(glClearColor(c.Red/255.0f, c.Green/255.0f, c.Blue/255.0f, c.Alpha/255.0f);)
			}
			if (flag & DEPTH_BUFFER)
			{
				GL_TRACE(glDepthMask(GL_TRUE);)
				Flag |= GL_DEPTH_BUFFER_BIT;
				GL_TRACE(glClearDepthf(z);)
			}
			if (flag & STENCIL_BUFFER)
			{
				Flag |= GL_STENCIL_BUFFER_BIT;
				GL_TRACE(glClearStencil(s);)
			}		
			GL_TRACE(glClear(Flag);)

			UNLOCK_MUTEX
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) 
		{
			LOCK_MUTEX

			START_COUNTER
			
			GL_TRACE(glDrawElements(TranslatePrimitiveType(type), GetIndexCount(type, primitive_count), 
						   ((COGLES2IndexArray*)m_CachedIndexArray)->GetIndexType(), 
						   (GLvoid *)(base_index*m_CachedIndexArray->GetStride()));)
								
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, primitive_count) 

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert)
		{
			LOCK_MUTEX

			START_COUNTER
			
			GL_TRACE(glDrawElements(TranslatePrimitiveType(lpp->Type), lpp->NumIndex, 
						   ((COGLES2IndexArray*)m_CachedIndexArray)->GetIndexType(),
						   (GLvoid *)(lpp->BaseIndex*m_CachedIndexArray->GetStride()));)
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, lpp->NumPrimitives) 

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert)
		{
			LOCK_MUTEX

			START_COUNTER
						
			GL_TRACE(glDrawArrays(TranslatePrimitiveType(type), base_vert, GetIndexCount(type, primitive_count));) 
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, primitive_count) 

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::SwapBackBuffer()
		{
			LOCK_MUTEX

			#ifdef __iOS__
				if (m_lpResolvedBackBuffer)
				{
					GLenum Attachments[] = {GL_DEPTH_ATTACHMENT};
					GL_TRACE(glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, Attachments);)
					GL_TRACE(glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, m_MSFrameBuffer);)
					GL_TRACE(glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, m_FrameBuffer);)
					// Call a resolve to combine buffers
					GL_TRACE(glResolveMultisampleFramebufferAPPLE();)
					//SetRenderBuffer
					GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, ((COGLES2Buffer*)((CBuffer*)m_lpResolvedBackBuffer))->GetGLHandle());)
				
					GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, m_MSFrameBuffer);)
				} 
			#elif defined __ANDROID__				
				eglSwapBuffers(m_EglDisplay, m_EglSurface);
			#elif defined __DESKTOP__				
				eglSwapBuffers(m_EglDisplay, m_EglSurface);
			#else
				eglSwapBuffers(m_EglDisplay, m_EglSurface);
			#endif

			ResetAllCounters();

			UNLOCK_MUTEX
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect)
        {
			if (lpdest == lpsrc)
				return;

            CBuffer* OldDepthBuffer = m_lpGraphicsSystem->SetDepthBuffer(NULL);
            m_lpGraphicsSystem->SetRenderBuffer(0, lpdest);	
            m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, lpdest->vGetWidth(), lpdest->vGetHeight());
            m_lpGraphicsSystem->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
            m_lpGraphicsSystem->SetupScreenSpaceQuadUVOffset(&(m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), NULL);	
            m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
            m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetPS()->GetlpShader()->vSetTexture("DiffuseMap", lpsrc->GetlpTexture());
            m_lpGraphicsSystem->DrawScreenSpaceQuad();
            m_lpGraphicsSystem->SetDepthBuffer(OldDepthBuffer);
        }
        //-------------------------------------------------------------------------------------------------------
		//
		//  TODO : Use vertex shader constant to change the position or
        //         modified created vertex buffer data
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::DrawScreenQuad(void* lpv, DWORD size)
        {
			CVertexArrayPtr VertexArray;
            
            m_lpGraphicsSystem->GetlpEngine()->CreateResource(VertexArray, VERTEX_ARRAY);
            
			CVertexLayoutPtr VertexLayoutPtr;
			m_lpGraphicsSystem->GetlpEngine()->CreateVertexLayout(VertexLayoutPtr, GetEnumStr(VT_SCREEN_QUAD));
			
			if (!VertexArray->vCreate(VertexLayoutPtr, 4, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpv))
				return;
			 
            m_lpGraphicsSystem->GetlpRenderer()->SetVertexArray(VertexArray, 0, 0);
            m_lpGraphicsSystem->GetlpRenderer()->DrawArrayBO(CRenderer::TRIANGLE_STRIP, 2, 0);			
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  TODO : Use vertex shader constant to change the position or
        //         modified created vertex buffer data
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Renderer::BadMutexLocking()
		{
			int xxx = 0;
		}
    }
}

#endif
