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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>

void lynxiOSSetupGLColorBufferFromContext();

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		int COGLESRenderer::GetIndexCount(const CRenderer::PRIMITIVETYPE type, const int primitive_count)
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
		GLenum COGLESRenderer::TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e)
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
		GLint COGLESRenderer::TranslateAddressMode(CRenderer::ADDRESSMODE e)
		{
			switch (e)
			{
				default:
				case AM_WRAP:
					return GL_REPEAT;
					break;
				case AM_MIRROR:
					//Only for OGLES 2.x
					return GL_REPEAT;
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
		GLenum	COGLESRenderer::TranslateCullMode(CRenderer::CULLMODE e)
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
		GLenum COGLESRenderer::TranslateTestFunction(CRenderer::TESTFUNC e)
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
		GLenum COGLESRenderer::TranslateStencilOP(CRenderer::STENCILOP e)
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
		GLenum COGLESRenderer::TranslateBlendFunction(CRenderer::BLENDFUNC e)
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
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLuint COGLESRenderer::TranslateVertexSlotType(CRenderer::VERTEXSLOTTYPE e)
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
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		GLenum COGLESRenderer::TranslateVertexSlotFormat(CRenderer::VERTEXSLOTFORMAT e)
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
		GLenum COGLESRenderer::TranslateFormat(CRenderer::FORMAT f)
		{
			switch (f)
			{
				case CRenderer::FORMAT_R8G8B8:
					return GL_RGB;
					break;
				case CRenderer::FORMAT_A8R8G8B8:
					return GL_RGBA;
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
                case CRenderer::FORMAT_D16:
					return GL_DEPTH_COMPONENT16_OES;
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
		COGLESRenderer::COGLESRenderer() 
		{
			m_RendererType = OGLES_RENDERER;
			m_FrameBuffer = 0;
			m_MSFrameBuffer = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		COGLESRenderer::COGLESRenderer(CGraphicsSystem* lpgs)
		: CRenderer(lpgs) 
		{
			m_RendererType = OGLES_RENDERER;
			m_FrameBuffer = 0;
			m_MSFrameBuffer = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		COGLESRenderer::~COGLESRenderer() 
		{			
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESRenderer::Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp)
		{
			m_Window = wnd; 
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Open()....\r\n"));

			GLenum DepthFormat = TranslateFormat(lpcp->DepthBufferFormat);
			
			#ifdef __iOS__
				if (lpcp->NumMultiSamples <= 1)
				{
					glGenFramebuffersOES(1, &m_FrameBuffer);
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_FrameBuffer);
					
					GLuint ColorBuffer, DepthBuffer;
					glGenRenderbuffersOES(1, &ColorBuffer);
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, ColorBuffer);
					//glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
					lynxiOSSetupGLColorBufferFromContext();
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, ColorBuffer);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
					m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
					((COGLESBuffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(ColorBuffer);
					
					glGenRenderbuffersOES(1, &DepthBuffer);
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, DepthBuffer);
					glRenderbufferStorageOES(GL_RENDERBUFFER_OES, DepthFormat, lpcp->Width, lpcp->Height);
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, DepthBuffer);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
					m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
					((COGLESBuffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(DepthBuffer);
					
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_FrameBuffer);
				}
				else 
				{					
					glGenFramebuffersOES(1, &m_FrameBuffer);
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_FrameBuffer);
					
					GLuint ColorBuffer;
					glGenRenderbuffersOES(1, &ColorBuffer);
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, ColorBuffer);
					//glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
					lynxiOSSetupGLColorBufferFromContext();
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, ColorBuffer);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpResolvedBackBuffer, LynxEngine::BUFFER);
					m_lpResolvedBackBuffer->SetName(_T("SystemResolvedBackBuffer"));
					((COGLESBuffer*)((CBuffer*)m_lpResolvedBackBuffer))->SetGLHandle(ColorBuffer);
					
					GLuint MSColorBuffer, MSDepthBuffer;
					glGenFramebuffersOES(1, &m_MSFrameBuffer);
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_MSFrameBuffer);
					
					glGenRenderbuffersOES(1, &MSColorBuffer);
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, MSColorBuffer);
					glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, lpcp->NumMultiSamples, GL_RGBA8_OES, lpcp->Width, lpcp->Height);
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, MSColorBuffer);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
					m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
					((COGLESBuffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(MSColorBuffer);
					
					glGenRenderbuffersOES(1, &MSDepthBuffer);
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, MSDepthBuffer);
					glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, lpcp->NumMultiSamples, DepthFormat, lpcp->Width, lpcp->Height);
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, MSDepthBuffer);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
					m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
					((COGLESBuffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(MSDepthBuffer);
					
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_MSFrameBuffer);
				}
				
				if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
				{
					LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Failed to make complete framebuffer object!\r\n"));
					return LYNX_FALSE;
				}		
			#elif defined (__ANDROID__)				
				//glGenFramebuffers(1, &m_FrameBuffer);
				//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
					
				GLint ColorBuffer;
				GLint DepthBuffer;
				//glGenRenderbuffers(1, &ColorBuffer);
				//glBindRenderbuffer(GL_RENDERBUFFER, ColorBuffer);
				//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, lpcp->Width, lpcp->Height);				
				//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBuffer);

				glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &ColorBuffer);
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
				m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
				((COGLESBuffer*)((CBuffer*)m_lpBackBuffer))->SetGLHandle(ColorBuffer);
				LYNXCHAR Str[256];
				lynxSprintf(Str, _T("::Color Buffer %d\r\n"), ColorBuffer);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Str));					
				
				//glGenRenderbuffers(1, &DepthBuffer);
				//glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
				//glRenderbufferStorage(GL_RENDERBUFFER, DepthFormat, lpcp->Width, lpcp->Height);
				//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);

				glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &DepthBuffer);
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
				m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
				((COGLESBuffer*)((CBuffer*)m_lpDepthBuffer))->SetGLHandle(DepthBuffer);
				lynxSprintf(Str, _T("::Depth Buffer %d\r\n"), DepthBuffer);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Str));					
					
				//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

				LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Create frame buffers OK\r\n"));
			#endif
			
			glDisable(GL_LIGHTING);
			SetViewportRect(0, 0, lpcp->Width, lpcp->Height);
			SetScissorRect(0, 0, lpcp->Width, lpcp->Height);
			glDepthRangef(0.0f, 1.0f);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(0.0f, 0.0f);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glEnable(GL_TEXTURE_2D);

			LYNX_LOG(glpLogger, CString(GetClassName())+CString("::Open() OK\r\n"));
			
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::Close()
		{
			#ifdef __iOS__
				if (m_FrameBuffer)
					glDeleteFramebuffersOES(1, &m_FrameBuffer);
				if (m_MSFrameBuffer)
					glDeleteFramebuffersOES(1, &m_MSFrameBuffer);
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s)
		{		
			GLbitfield Flag = 0;
			
			if (flag & RENDER_BUFFER)
			{
				Flag |= GL_COLOR_BUFFER_BIT;
				glClearColor(c.Red/255.0f, c.Green/255.0f, c.Blue/255.0f, c.Alpha/255.0f);
			}
			if (flag & DEPTH_BUFFER)
			{
				Flag |= GL_DEPTH_BUFFER_BIT;
				glClearDepthf(z);
			}
			if (flag & STENCIL_BUFFER)
			{
				Flag |= GL_STENCIL_BUFFER_BIT;
				glClearStencil(s);
			}		
			glClear(Flag);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) 
		{
			START_COUNTER
			
			glDrawElements(TranslatePrimitiveType(type), GetIndexCount(type, primitive_count), 
						   ((COGLESIndexArray*)m_CachedIndexArray)->GetIndexType(), 
						   (GLvoid *)(base_index*m_CachedIndexArray->GetStride()));
								
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert)
		{
			START_COUNTER
			
			glDrawElements(TranslatePrimitiveType(lpp->Type), lpp->NumIndex, 
						   ((COGLESIndexArray*)m_CachedIndexArray)->GetIndexType(),
						   (GLvoid *)(lpp->BaseIndex*m_CachedIndexArray->GetStride()));
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert)
		{
			START_COUNTER
						
			glDrawArrays(TranslatePrimitiveType(type), base_vert, GetIndexCount(type, primitive_count)); 
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:    
		//-------------------------------------------------------------------------------------------------------
		void COGLESRenderer::SwapBackBuffer()
		{
			#ifdef __iOS__
				if (m_lpResolvedBackBuffer)
				{
					GLenum Attachments[] = {GL_DEPTH_ATTACHMENT_OES};
					glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, Attachments);
					glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, m_MSFrameBuffer);
					glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, m_FrameBuffer);
					// Call a resolve to combine buffers
					glResolveMultisampleFramebufferAPPLE();
					//SetRenderBuffer
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)m_lpResolvedBackBuffer))->GetGLHandle());
				
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_MSFrameBuffer);
				}
			#endif
		}
	}	
}

#endif