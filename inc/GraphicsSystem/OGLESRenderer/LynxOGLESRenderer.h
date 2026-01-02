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

#ifndef __LYNXOGLESRENDERER_H__
#define __LYNXOGLESRENDERER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESBuffer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESTexture.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESQuery.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexLayout.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexShader.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESPixelShader.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexArray.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESIndexArray.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESDepthStencilState.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESBlendState.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESRasterizerState.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESSamplerState.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESShaderConstant.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESShaderGroup.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESShaderConstantGroup.h>

#ifdef __WIN32__
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#elif defined (__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#elif defined (__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class LYNXENGCLASS COGLESRenderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(COGLESRenderer);
		private:
			GLuint									m_FrameBuffer;
			GLuint									m_MSFrameBuffer;
			CBufferPtr								m_lpResolvedBackBuffer;
			
			CMatrix4								m_WorldM, m_ViewM, m_ProjM;
		protected:		
		public:						
			COGLESRenderer();
			COGLESRenderer(CGraphicsSystem* lpgs);
			~COGLESRenderer();											
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("OGL ES 1.1"); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return _T("OGL ES 1.1 Device"); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp);			
			void									Close();
			void									Resize(int cx, int cy) {};

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return (m_FrameBuffer?LYNX_TRUE:LYNX_FALSE); };
			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return LYNX_FALSE; };
			
			LYNXFORCEINLINE void					Start(void) { };			
			LYNXFORCEINLINE void					End(void) 
			{
				#ifdef __iOS__
					glBindRenderbufferOES(GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)m_lpBackBuffer))->GetGLHandle()); 
				#endif
			}
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					SwapBackBuffer();
			void									Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s);
			
			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetWorldMatrix(const CMatrix4& m) {m_WorldM = m; CMatrix4 M = m_WorldM * m_ViewM; glMatrixMode(GL_MODELVIEW); glLoadMatrixf((GLfloat *)&(M)); };
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {m = m_WorldM; };
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return m_WorldM; };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {m_ViewM = m; CMatrix4 M = m_WorldM * m_ViewM; glMatrixMode(GL_MODELVIEW); glLoadMatrixf((GLfloat *)&(M)); };
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {m = m_ViewM; };
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return m_ViewM; };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {m_ProjM = m; glMatrixMode(GL_PROJECTION); glLoadMatrixf((GLfloat *)&(m)); };
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {m = m_ProjM; };
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return m_ProjM;};
			LYNXFORCEINLINE void					UseFog(LYNXBOOL b) {};
			LYNXFORCEINLINE void					UseSpecular(LYNXBOOL b) {};

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = 0; *y = 0;};
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {};
			
			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetViewportRect(int x1, int y1, int x2, int y2) {glViewport(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1); };			
			LYNXFORCEINLINE void					SetScissorRect(int x1, int y1, int x2, int y2) {glScissor(x1, GetSystemBackBuffer()->vGetHeight()-y2, x2-x1, y2-y1);};
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			LYNXFORCEINLINE void					GetScissorRect(LPLYNXRECT lpsr) {};
			LYNXFORCEINLINE LYNXBOOL				EnableScissorRect(LYNXBOOL b) { LYNXBOOL bTemp = m_bCachedScissorEnabled; (b) ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST); m_bCachedScissorEnabled = b; return bTemp; };
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW COGLESQuery(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW COGLESVertexLayout(m_lpGraphicsSystem); };

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			//LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {};
			LYNXFORCEINLINE void					DrawScreenQuad(void* lpv, DWORD size) {};
			void									DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count);
			void									DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert);	
			LYNXFORCEINLINE void					DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			LYNXFORCEINLINE void					DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			void									DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert);						
			LYNXFORCEINLINE void					DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) {};
			LYNXFORCEINLINE void					DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) {};
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW COGLESBuffer(m_lpGraphicsSystem);};
			LYNXFORCEINLINE	CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) 
			{ 				
				if (lpb == m_CachedRT[index]) 
					return lpb; 			
				
				CBuffer* lpOld = m_CachedRT[index];
				m_CachedRT[index] = lpb; 

				#ifdef __iOS__
					if (lpb)
					{
						glBindRenderbufferOES(GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)lpb))->GetGLHandle());
						glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES+index, GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)lpb))->GetGLHandle());
					}
					else
					{
						glBindRenderbufferOES(GL_RENDERBUFFER_OES, NULL); 	
						glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES+index, GL_RENDERBUFFER_OES, NULL);
					}
				#endif
				return lpOld;
			}			
			LYNXFORCEINLINE CBuffer*				SetDepthBuffer(CBuffer* lpzb) 
			{ 
				if (lpzb == m_CachedDB) 
					return lpzb; 
				
				CBuffer* lpOld = m_CachedDB;
				m_CachedDB = lpzb; 

				#ifdef __iOS__
					if (lpzb)
					{
						glBindRenderbufferOES(GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)lpzb))->GetGLHandle());
						glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, ((COGLESBuffer*)((CBuffer*)lpzb))->GetGLHandle());
					}
					else
					{
						glBindRenderbufferOES(GL_RENDERBUFFER_OES, NULL); 	
						glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, NULL);
					}
				#endif
				return lpOld;
			}		
			LYNXFORCEINLINE LYNXERRORNO				CreateDepthBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms) { return LYNX_OK; };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE) { return LYNX_OK; };			
			LYNXFORCEINLINE void					BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect)  {};

			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW COGLESTexture(m_lpGraphicsSystem); };
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
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW COGLESVertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW COGLESIndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath() {return CRenderer::ComputeCachedCompiledShaderPath(); };

			void									NullVertexShader() {};
			void									NullPixelShader() {};

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			LYNXFORCEINLINE void					SetVertexShaderVersion(const LYNXCHAR *ver) {};
			LYNXFORCEINLINE void					SetPixelShaderVersion(const LYNXCHAR *ver) {};

			LYNXFORCEINLINE virtual CVertexShader*	CreateVertexShader() {return LYNXNEW COGLESVertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CPixelShader*	CreatePixelShader() {return LYNXNEW COGLESPixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CShaderConstant* CreateShaderConstant(CShader *lps, const char *name) {return LYNXNEW COGLESShaderConstant(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstant(CShaderConstant* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW COGLESShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return NULL; };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return NULL; };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW COGLESShaderGroup(m_lpGraphicsSystem); };			
			LYNXFORCEINLINE void					ReleaseShaderGroup(CShaderGroup* lpsg) {LYNXDEL lpsg; };
			
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
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW COGLESDepthStencilState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW COGLESBlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW COGLESRasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW COGLESSamplerState(m_lpGraphicsSystem); };

			LYNXFORCEINLINE DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) {return (DEPTHBUFFERMODE)0; };
			LYNXFORCEINLINE DEPTHBUFFERMODE			GetDepthBufferMode() {return (DEPTHBUFFERMODE)0; };		
			LYNXFORCEINLINE LYNXDWORD				SetColorWriteMask(LYNXDWORD m) {return 0; };			
			LYNXFORCEINLINE void					SetSRGBRead(int channel, LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetSRGBWrite(LYNXBOOL b) {};		
			
			static int								GetIndexCount(const CRenderer::PRIMITIVETYPE type, const int primitive_count);
			//static D3DQUERYTYPE					TranslateQueryType(CRenderer::QUERYTYPE type);
			static GLenum							TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e);
			static GLenum							TranslateFormat(CRenderer::FORMAT f);
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

#endif

#endif