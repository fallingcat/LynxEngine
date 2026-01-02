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

#ifndef __LYNXOGLES2RENDERER_H__
#define __LYNXOGLES2RENDERER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Buffer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Texture.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Query.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexLayout.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2PixelShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexArray.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2IndexArray.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2DepthStencilState.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2BlendState.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2RasterizerState.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2SamplerState.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderConstant.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderResource.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderConstantGroup.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderGroup.h>

#ifdef __WIN32__
	#define GL_GLEXT_PROTOTYPES	1
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <EGL/egl.h>	
#elif defined (__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#elif defined (__ANDROID__)	
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif


namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class LYNXENGCLASS COGLES2Renderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(COGLES2Renderer);
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
				EGLContext							m_LoadingContext;
			#endif			
			CAnsiString								m_Extensions;
		public:		
			LYNXMUTEX								m_Mutex;	
			LYNXBOOL								m_bMutexLocked;

			COGLES2Renderer();
			COGLES2Renderer(CGraphicsSystem* lpgs);
			~COGLES2Renderer();											
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("OGL ES 2.0"); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return _T("OGL ES 2.0 Device"); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp);			
			void									Close();
			void									Resize(int cx, int cy) {};
			void									Pause();
			void									Resume(LYNXWINDOW wnd);
			LYNXBOOL								RecreateSurface(LYNXWINDOW wnd);

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return (m_FrameBuffer?LYNX_TRUE:LYNX_FALSE); };
			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return LYNX_FALSE; };
			
			LYNXFORCEINLINE void					Start(void) { };			
			void									End(void);
            void									Finish(void);
            void									Flush(void);
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			void									SwapBackBuffer();
			void									Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s);
			
			// Context
			void									SetMainContext();
			void									SetLoadingContext();

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
			void									SetViewportRect(int x1, int y1, int x2, int y2);
			void									SetScissorRect(int x1, int y1, int x2, int y2);
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			void									GetScissorRect(LPLYNXRECT r);
			LYNXBOOL								EnableScissorRect(LYNXBOOL b);
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW COGLES2Query(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW COGLES2VertexLayout(m_lpGraphicsSystem); };

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
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW COGLES2Buffer(m_lpGraphicsSystem);};
			CBuffer*								SetRenderBuffer(int index, CBuffer* lpb); 
			CBuffer*								SetDepthBuffer(CBuffer* lpzb);
            void					                DiscardRenderBuffer(int index);
            void								    DiscardDepthBuffer();
			LYNXFORCEINLINE LYNXERRORNO				CreateDepthBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms) { return LYNX_OK; };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE) { return LYNX_OK; };			
			void                                    BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect);

			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW COGLES2Texture(m_lpGraphicsSystem); };
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
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW COGLES2VertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW COGLES2IndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath();
			

			void									NullVertexShader() {};
			void									NullPixelShader() {};

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			LYNXFORCEINLINE void					SetVertexShaderVersion(const LYNXCHAR *ver) {};
			LYNXFORCEINLINE void					SetPixelShaderVersion(const LYNXCHAR *ver) {};

			LYNXFORCEINLINE CVertexShader*			CreateVertexShader() {return LYNXNEW COGLES2VertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CPixelShader*			CreatePixelShader() {return LYNXNEW COGLES2PixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CShaderConstant*		CreateShaderConstant(CShader *lps, const char *name) 
			{
				CShaderConstant* pConstant = LYNXNEW COGLES2ShaderConstant(lps, name); 
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
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW COGLES2ShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) {LYNXDEL lpsc; };

			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return LYNXNEW COGLES2ShaderResource(lps, name); };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return LYNXNEW COGLES2ShaderResource(lps, name, r); };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) {LYNXDEL lpsc; };

			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW COGLES2ShaderGroup(m_lpGraphicsSystem); };
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
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW COGLES2DepthStencilState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW COGLES2BlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW COGLES2RasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW COGLES2SamplerState(m_lpGraphicsSystem); };

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

			void									BadMutexLocking();
		};
        
	}
}

#define OGLES2RENDERER								static_cast<LynxEngine::GraphicsSystem::COGLES2Renderer*>(glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer())
#if 1
	#define LOCK_MUTEX						
	#define UNLOCK_MUTEX							
#else
	#define LOCK_MUTEX\
		if (OGLES2RENDERER->m_bMutexLocked)\
			OGLES2RENDERER->BadMutexLocking();\
		lynxOSLockMutex(&(OGLES2RENDERER->m_Mutex));\
		OGLES2RENDERER->m_bMutexLocked = LYNX_TRUE;
	
	#define UNLOCK_MUTEX								lynxOSUnlockMutex(&(OGLES2RENDERER->m_Mutex)); OGLES2RENDERER->m_bMutexLocked = LYNX_FALSE;
#endif

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

#endif

#endif