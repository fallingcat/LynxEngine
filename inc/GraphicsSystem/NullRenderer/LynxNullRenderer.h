//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
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

#ifndef __LYNXNULLRENDERER_H__
#define __LYNXNULLRENDERER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/NullRenderer/LynxNullBuffer.h>
#include <GraphicsSystem/NullRenderer/LynxNullTexture.h>
#include <GraphicsSystem/NullRenderer/LynxNullQuery.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexLayout.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexShader.h>
#include <GraphicsSystem/NullRenderer/LynxNullPixelShader.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexArray.h>
#include <GraphicsSystem/NullRenderer/LynxNullIndexArray.h>
#include <GraphicsSystem/NullRenderer/LynxNullDepthStencilState.h>
#include <GraphicsSystem/NullRenderer/LynxNullBlendState.h>
#include <GraphicsSystem/NullRenderer/LynxNullRasterizerState.h>
#include <GraphicsSystem/NullRenderer/LynxNullSamplerState.h>
#include <GraphicsSystem/NullRenderer/LynxNullShaderGroup.h>
#include <GraphicsSystem/NullRenderer/LynxNullShaderConstant.h>
#include <GraphicsSystem/NullRenderer/LynxNullShaderConstantGroup.h>
#include <GraphicsSystem/NullRenderer/LynxNullShaderResource.h>

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class LYNXENGCLASS CNullRenderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(CNullRenderer);
		private:
			CMatrix4								m_WorldM, m_ViewM, m_ProjM;
		protected:		
		public:						
			LYNXFORCEINLINE  CNullRenderer() {m_RendererType = NULL_RENDERER; };
			LYNXFORCEINLINE  CNullRenderer(CGraphicsSystem* lpgs):CRenderer(lpgs) { m_RendererType = NULL_RENDERER; };
			LYNXFORCEINLINE  ~CNullRenderer() { };											
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("Null"); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return _T("Null Device"); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp) 
			{
				m_Window = wnd; 

				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
				m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
				SetRenderBuffer(0, m_lpBackBuffer);

				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
				m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
				SetDepthBuffer(m_lpDepthBuffer);

				return LYNX_TRUE; 
			}
			void									Close() {};
			void									Resize(int cx, int cy) {};

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return LYNX_FALSE; };
			
			LYNXFORCEINLINE void					Start(void) {};			
			LYNXFORCEINLINE void					End(void) {};
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					SwapBackBuffer() {};
			LYNXFORCEINLINE void					Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) {};

			LYNXFORCEINLINE void					SetWorldMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return m_WorldM; };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return m_ViewM; };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return m_ProjM;};
			
			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					UseFog(LYNXBOOL b) {};
			LYNXFORCEINLINE void					UseSpecular(LYNXBOOL b) {};

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = 0; *y = 0;};
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {};
			
			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetViewportRect(int x1, int y1, int x2, int y2) {};			
			LYNXFORCEINLINE void					SetScissorRect(int x1, int y1, int x2, int y2) {};
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			LYNXFORCEINLINE void					GetScissorRect(LPLYNXRECT lpsr) {};
			LYNXFORCEINLINE LYNXBOOL				EnableScissorRect(LYNXBOOL b) {return LYNX_TRUE;};
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW CNullQuery(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW CNullVertexLayout(m_lpGraphicsSystem); };

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			//LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {};
			LYNXFORCEINLINE void					DrawScreenQuad(void* lpv, DWORD size) {};
			LYNXFORCEINLINE void					DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			LYNXFORCEINLINE void					DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };			
			LYNXFORCEINLINE void					DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			LYNXFORCEINLINE void					DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };
			LYNXFORCEINLINE void					DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert) {START_COUNTER STOP_COUNTER(m_DrawTimeCounter) INC_COUNTER(m_DrawCallCounter, 1) };						
			LYNXFORCEINLINE void					DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) {};
			LYNXFORCEINLINE void					DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) {};
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW CNullBuffer(m_lpGraphicsSystem);};
			LYNXFORCEINLINE	CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) 
			{ 
				if (lpb == m_CachedRT[index]) 
					return lpb; 
				CBuffer* lpOld = m_CachedRT[index];
				m_CachedRT[index] = lpb; 
				return lpOld;
			}			
			LYNXFORCEINLINE CBuffer*				SetDepthBuffer(CBuffer* lpzb) 
			{ 
				if (lpzb == m_CachedDB) 
					return lpzb; 
				CBuffer* lpOld = m_CachedDB;
				m_CachedDB = lpzb; 
				return lpOld;
			}		
			LYNXFORCEINLINE LYNXERRORNO				CreateDepthBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms) { return LYNX_OK; };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE) { return LYNX_OK; };			
			LYNXFORCEINLINE void					BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect)  {};

			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW CNullTexture(m_lpGraphicsSystem); };
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
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW CNullVertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW CNullIndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath() {return CRenderer::ComputeCachedCompiledShaderPath(); };

			void									NullVertexShader() {};
			void									NullPixelShader() {};

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			
			LYNXFORCEINLINE virtual CVertexShader*	CreateVertexShader() {return LYNXNEW CNullVertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CPixelShader*	CreatePixelShader() {return LYNXNEW CNullPixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CShaderConstant*		CreateShaderConstant(CShader* lps, const char* name) {return LYNXNEW CNullShaderConstant(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstant(CShaderConstant* lps) { LYNXDEL lps; };
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW CNullShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) { LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return LYNXNEW CNullShaderResource(lps, name); };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return LYNXNEW CNullShaderResource(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) { };
			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW CNullShaderGroup(m_lpGraphicsSystem); };		
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
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW CNullDepthStencilState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW CNullBlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW CNullRasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW CNullSamplerState(m_lpGraphicsSystem); };

			LYNXFORCEINLINE DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) {return (DEPTHBUFFERMODE)0; };
			LYNXFORCEINLINE DEPTHBUFFERMODE			GetDepthBufferMode() {return (DEPTHBUFFERMODE)0; };		
			LYNXFORCEINLINE void					SetSRGBRead(int channel, LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetSRGBWrite(LYNXBOOL b) {};		
		};			
	}	
}		

#endif