//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/07/04
//  Last Update : 
//
//  說明:
//
//      LynxGraphic負責任何與API相關的實際動作,使得GameSystem的使用
//      者可以在不同API間以相同方式工作 
//
//###########################################################################

#ifndef __LYNXOGLES3RENDERER_H__
#define __LYNXOGLES3RENDERER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Buffer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Texture.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Query.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexLayout.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3PixelShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexArray.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3IndexArray.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3DepthStencilState.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3BlendState.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3RasterizerState.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3SamplerState.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderConstant.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderResource.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderConstantGroup.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderGroup.h>

#ifdef __WIN32__
	#define GL_GLEXT_PROTOTYPES	1
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <EGL/egl.h>	
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
		class LYNXENGCLASS COGLES3Renderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(COGLES3Renderer);
		private:
			GLuint									m_FrameBuffer;
            GLuint									m_R2TFrameBuffer;
			GLuint									m_MSFrameBuffer;
         	CBufferPtr								m_lpResolvedBackBuffer;		
			
			CMatrix4								m_WorldM, m_ViewM, m_ProjM;			
		protected:		
			#if (defined __WIN32__ || defined __ANDROID__)
				EGLDisplay							m_EglDisplay;
				EGLSurface							m_EglSurface;
				EGLContext							m_EglContext;
			#endif
			CAnsiString								m_Extensions;
		public:						
			COGLES3Renderer();
			COGLES3Renderer(CGraphicsSystem* lpgs);
			~COGLES3Renderer();											
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("OGL ES 2.0"); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return _T("OGL ES 2.0 Device"); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp);			
			void									Close();
			void									Resize(int cx, int cy) {};

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return (m_FrameBuffer?LYNX_TRUE:LYNX_FALSE); };
			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return LYNX_FALSE; };
			
			LYNXFORCEINLINE void					Start(void) { };			
			LYNXFORCEINLINE void					End(void) { GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, ((COGLES3Buffer*)((CBuffer*)m_lpBackBuffer))->GetGLHandle());) };
            LYNXFORCEINLINE void                    Finish(void) {GL_TRACE(glFinish();) };
            LYNXFORCEINLINE void                    Flush(void) {GL_TRACE(glFlush();) };
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			void									SwapBackBuffer();
			void									Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s);
			
			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetWorldMatrix(const CMatrix4& m) {m_WorldM = m; };
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {m = m_WorldM; };
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return m_WorldM; };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {m_ViewM = m; };
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {m = m_ViewM; };
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return m_ViewM; };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {m_ProjM = m; };
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {m = m_ProjM; };
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return m_ProjM;};
			LYNXFORCEINLINE void					UseFog(LYNXBOOL b) {};
			LYNXFORCEINLINE void					UseSpecular(LYNXBOOL b) {};

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = 0; *y = 0;};
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {};
			
			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetViewportRect(int x1, int y1, int x2, int y2) {CRenderer::SetViewportRect(x1, y1, x2, y2); GetCachedRenderBuffer(0) ? glViewport(x1, GetCachedRenderBuffer(0)->vGetHeight()-y2, x2-x1, y2-y1) : glViewport(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1); };
			LYNXFORCEINLINE void					SetScissorRect(int x1, int y1, int x2, int y2) {GetCachedRenderBuffer(0) ? glScissor(x1, GetCachedRenderBuffer(0)->vGetHeight()-y2, x2-x1, y2-y1) : glScissor(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1);};
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			LYNXFORCEINLINE void					GetScissorRect(LPLYNXRECT r) {GLint Param[4];  GL_TRACE(glGetIntegerv(GL_SCISSOR_BOX, Param);) r->x1 = Param[0]; r->y1 = GetSystemBackBuffer()->vGetHeight() - Param[3 ]- Param[1]; r->x2 = r->x1 + Param[2]; r->y2 = r->y1 + Param[3]; }
			LYNXFORCEINLINE LYNXBOOL				EnableScissorRect(LYNXBOOL b) { LYNXBOOL bTemp = m_bCachedScissorEnabled; (b) ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST); m_bCachedScissorEnabled = b; return bTemp; };
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW COGLES3Query(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW COGLES3VertexLayout(m_lpGraphicsSystem); };

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			//LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {};
			LYNXFORCEINLINE void					DrawScreenQuad(void* lpv, DWORD size);
			void									DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count);
			void									DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert);	
			LYNXFORCEINLINE void					DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			LYNXFORCEINLINE void					DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			void									DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert);						
			LYNXFORCEINLINE void					DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) {};
			LYNXFORCEINLINE void					DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) {};
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW COGLES3Buffer(m_lpGraphicsSystem);};
			LYNXFORCEINLINE	CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) 
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
					//GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, 0x00);) 	
                    GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0x00);)
				}
				return lpOld;
			}
			LYNXFORCEINLINE CBuffer*				SetDepthBuffer(CBuffer* lpzb)
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
					//GL_TRACE(glBindRenderbuffer(GL_RENDERBUFFER, 0x00);) 	
					GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0x00);)
                    GL_TRACE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0x00);)
				}
				return lpOld;
			}
            LYNXFORCEINLINE void                    DiscardRenderBuffer(int index)
            {
                //return;
				#ifdef __iOS__
					static const GLenum Discards[]  = {GL_COLOR_ATTACHMENT0};
					if (glDiscardFramebufferEXT)
						glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, Discards);
				#endif
            }   
            LYNXFORCEINLINE void                    DiscardDepthBuffer()
            {
                //return;
				#ifdef __iOS__
					static const GLenum Discards[]  = {GL_DEPTH_ATTACHMENT};
					if (glDiscardFramebufferEXT)
						glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, Discards);
				#endif
            }
			LYNXFORCEINLINE LYNXERRORNO				CreateDepthBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms) { return LYNX_OK; };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE) { return LYNX_OK; };			
			void                                    BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect);

			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW COGLES3Texture(m_lpGraphicsSystem); };
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT fmt) { return LYNX_TRUE; };

			//Vertex Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW COGLES3VertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW COGLES3IndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath();
			

			void									NullVertexShader() {};
			void									NullPixelShader() {};

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			LYNXFORCEINLINE void					SetVertexShaderVersion(const LYNXCHAR *ver) {};
			LYNXFORCEINLINE void					SetPixelShaderVersion(const LYNXCHAR *ver) {};

			LYNXFORCEINLINE CVertexShader*			CreateVertexShader() {return LYNXNEW COGLES3VertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CPixelShader*			CreatePixelShader() {return LYNXNEW COGLES3PixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CShaderConstant*		CreateShaderConstant(CShader *lps, const char *name) 
			{
				CShaderConstant* pConstant = LYNXNEW COGLES3ShaderConstant(lps, name); 
				/*
                if (!pConstant)
					return NULL;

				if (!pConstant->vIsValid())
				{
					LYNXDEL pConstant;
					return NULL;
				}
                */

				return pConstant;			
			}
			LYNXFORCEINLINE void					ReleaseShaderConstant(CShaderConstant* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW COGLES3ShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) {LYNXDEL lpsc; };

			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return LYNXNEW COGLES3ShaderResource(lps, name); };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return LYNXNEW COGLES3ShaderResource(lps, name, r); };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) {LYNXDEL lpsc; };

			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW COGLES3ShaderGroup(m_lpGraphicsSystem); };
			LYNXFORCEINLINE void					ReleaseShaderGroup(CShaderGroup* lpsg) {LYNXDEL lpsg; };
			void									SetShaderGroup(CShaderGroup* lpsg, CContainer* lpo, CCameraContainer* cam);	
			
			LYNXFORCEINLINE void					SetVSConstantF(int i, const LYNXREAL *lpdata, int num) {};
			LYNXFORCEINLINE void					GetVSConstantF(int i, LYNXREAL *lpdata, int num) {};
			LYNXFORCEINLINE void					SetVSConstantI(int i, const int *lpdata, int num) {};
			LYNXFORCEINLINE void					GetVSConstantI(int i, int *lpdata, int num) {};
			LYNXFORCEINLINE void					SetVSConstantB(int i, const BOOL *lpdata, int num) {};
			LYNXFORCEINLINE void					GetVSConstantB(int i, BOOL *lpdata, int num) {};

			LYNXFORCEINLINE void					SetPSConstantF(int i, const LYNXREAL *lpdata, int num) {};
			LYNXFORCEINLINE void					GetPSConstantF(int i, LYNXREAL *lpdata, int num) {};
			LYNXFORCEINLINE void					SetPSConstantI(int i, const int *lpdata, int num) {};
			LYNXFORCEINLINE void					GetPSConstantI(int i, int *lpdata, int num) {};
			LYNXFORCEINLINE void					SetPSConstantB(int i, const BOOL *lpdata, int num) {};
			LYNXFORCEINLINE void					GetPSConstantB(int i, BOOL *lpdata, int num) {};

			//Render State ----------------------------------------------------------------------------------------------------------------------------------------------			
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW COGLES3DepthStencilState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW COGLES3BlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW COGLES3RasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW COGLES3SamplerState(m_lpGraphicsSystem); };

			LYNXFORCEINLINE DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) {return (DEPTHBUFFERMODE)0; };
			LYNXFORCEINLINE DEPTHBUFFERMODE			GetDepthBufferMode() {return (DEPTHBUFFERMODE)0; };		
			LYNXFORCEINLINE LYNXDWORD				SetColorWriteMask(LYNXDWORD m) {return 0; };			
			LYNXFORCEINLINE void					SetSRGBRead(int channel, LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetSRGBWrite(LYNXBOOL b) {};
            
            LYNXFORCEINLINE void                    BindR2TFrameBuffer()
            {
                GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, m_R2TFrameBuffer);)
            }			
            LYNXFORCEINLINE void                    BindFrameBuffer()
            {
                GL_TRACE(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);)
            }
			static int								GetIndexCount(const CRenderer::PRIMITIVETYPE type, const int primitive_count);
			//static D3DQUERYTYPE					TranslateQueryType(CRenderer::QUERYTYPE type);
			static GLenum							TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e);
			static GLenum							TranslateFormat(CRenderer::FORMAT f);
			static GLenum							TranslateFormatToPixelType(CRenderer::FORMAT f);
			//static CRenderer::FORMAT				TranslateFormat(D3DFORMAT f);
			static GLenum							TranslateBlendFunction(CRenderer::BLENDFUNC e);
			//static D3DBLENDOP						TranslateBlendOP(CRenderer::BLENDOP e);
			static GLenum							TranslateTestFunction(CRenderer::TESTFUNC e);
			static GLenum							TranslateStencilOP(CRenderer::STENCILOP e);
			//static D3DFILLMODE					TranslateFillMode(CRenderer::FILLMODE e);
			static GLenum							TranslateCullMode(CRenderer::CULLMODE e);
			//static D3DTEXTUREFILTERTYPE			TranslateFilter(CRenderer::FILTERMODE e);
			static GLint							TranslateAddressMode(CRenderer::ADDRESSMODE e);
			static GLuint							TranslateVertexSlotType(CRenderer::VERTEXSLOTTYPE e);
			static GLenum							TranslateVertexSlotFormat(CRenderer::VERTEXSLOTFORMAT e);
		};
        
	}
}
/*
#ifdef __ANDROID__
	#if !GL_OES_vertex_array_object || !GL_GLEXT_PROTOTYPES
		extern PFNGLBINDVERTEXARRAYOESPROC						glBindVertexArrayOES;
		extern PFNGLDELETEVERTEXARRAYSOESPROC					glDeleteVertexArraysOES;
		extern PFNGLGENVERTEXARRAYSOESPROC						glGenVertexArraysOES;
	#endif
#else
	#if !GL_OES_vertex_array_object
		extern PFNGLBINDVERTEXARRAYOESPROC						glBindVertexArrayOES;
		extern PFNGLDELETEVERTEXARRAYSOESPROC					glDeleteVertexArraysOES;
		extern PFNGLGENVERTEXARRAYSOESPROC						glGenVertexArraysOES;
	#endif
#endif
*/
#endif

#endif