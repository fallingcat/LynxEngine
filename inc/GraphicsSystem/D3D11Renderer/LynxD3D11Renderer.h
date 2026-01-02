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

#ifndef __LYNXD3D11RENDERER_H__
#define __LYNXD3D11RENDERER_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Buffer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Texture.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Query.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexLayout.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11PixelShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexArray.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11IndexArray.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11DepthStencilState.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11BlendState.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11RasterizerState.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11SamplerState.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderConstant.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderConstantGroup.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderResource.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WINAPP__
	#define DXGetErrorString(h)				_T("DX Error")
#endif

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class CD3D11Include : public ID3DInclude
		{	
		protected:
			CString m_CurrentShaderPath;
		public:			
			void ComputeCurrentShaderPath(const LYNXCHAR* path);
			COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
			COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
		};

		class LYNXENGCLASS CD3D11Renderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(CD3D11Renderer);
		public:
			CD3D11Include							m_ShaderIncludeHandler;	
		private:			
			CMatrix4								m_WorldM, m_ViewM, m_ProjM;
		protected:		
			typedef struct {
				IDXGIAdapter*				lpAdapter;
				CArray<IDXGIOutput*>		OutputList;

				CString						Name;
				CString						Desc;
				DWORD						VendorId;
				DWORD						DeviceId;
				DWORD						SubSysId;
				DWORD						Revision;
				LYNXLONGLONG				DedicatedVideoMemory;
				LYNXLONGLONG				DedicatedSystemMemory;
				LYNXLONGLONG				SharedSystemMemory;
			}DRIVERDATA;
			
			#ifdef __WINAPP__
				IDXGIFactory2*			m_lpDXGIFactory;						
				IDXGISwapChain1*		m_lpSwapChain;
				ID3D11Device1*			m_lpD3DDevice;	
				ID3D11DeviceContext1*	m_lpImmediateContext;
			#else
				IDXGIFactory*			m_lpDXGIFactory;						
				IDXGISwapChain*			m_lpSwapChain;
				ID3D11Device*			m_lpD3DDevice;	
				ID3D11DeviceContext*	m_lpImmediateContext;
			#endif
			
			//LPD3DXFONT								m_lpDefaultFont;
			CArray<DRIVERDATA>						m_DriverList;
			int										m_CurrentDriver;
			CArray<DXGI_MODE_DESC>					m_DisplayModeList;
			ID3D11RenderTargetView*					m_CachedRTView[8];
			ID3D11DepthStencilView*					m_CachedDSView;			
		public:						
			CD3D11Renderer();
			CD3D11Renderer(CGraphicsSystem* lpgs);
			~CD3D11Renderer();											
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("Direct3D 11"); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return m_DriverList[m_CurrentDriver].Desc.c_str(); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp);
			void									Close();
			LYNXFORCEINLINE void					Resize(int cx, int cy) { SetViewportRect(0, 0, cx, cy); };

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return LYNX_TRUE; };			
			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return LYNX_FALSE; };
			
			LYNXFORCEINLINE void					Start(void) {};			
			LYNXFORCEINLINE void					End(void) {};
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					SwapBackBuffer() {m_lpSwapChain->Present(0, 0); };
			LYNXFORCEINLINE void					Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) 
			{
				float Color[4] = {c.Red/255.0f, c.Green/255.0f, c.Blue/255.0f, c.Alpha/255.0f}; 

				if (flag & RENDER_BUFFER)
				{
					for (int i=0; i<8; i++)
					{
						if (m_CachedRTView[i])
							m_lpImmediateContext->ClearRenderTargetView(m_CachedRTView[i], Color);
					}
				}

				UINT Flag = 0;
				if (flag & DEPTH_BUFFER)
					Flag |= D3D11_CLEAR_DEPTH;
				if (flag & STENCIL_BUFFER)
					Flag |= D3D11_CLEAR_STENCIL;
				if (Flag && m_CachedDSView)
					m_lpImmediateContext->ClearDepthStencilView(m_CachedDSView, Flag, z, s);
			}			
			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetWorldMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return m_WorldM; };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return m_ViewM; };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {};
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {};
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return m_ProjM;};

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = 0; *y = 0;};
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {};
			
			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetViewportRect(int x1, int y1, int x2, int y2);			
			LYNXFORCEINLINE void					SetScissorRect(int x1, int y1, int x2, int y2);
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			LYNXFORCEINLINE void					GetScissorRect(LPLYNXRECT lpsr) {};
			LYNXFORCEINLINE LYNXBOOL				EnableScissorRect(LYNXBOOL b) {return LYNX_TRUE;};
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW CD3D11Query(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW CD3D11VertexLayout(m_lpGraphicsSystem); };

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			//LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {};
			LYNXFORCEINLINE void					DrawScreenQuad(void* lpv, DWORD size) {};
			LYNXFORCEINLINE void					DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count);
			LYNXFORCEINLINE void					DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert);
			LYNXFORCEINLINE void					DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert);
			LYNXFORCEINLINE void					DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) {};
			LYNXFORCEINLINE void					DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) {};
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CBufferPtr&				GetSystemBackBuffer() {return m_lpBackBuffer; };			
			LYNXFORCEINLINE CBufferPtr&				GetSystemDepthBuffer() {return m_lpDepthBuffer; };			
			
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW CD3D11Buffer(m_lpGraphicsSystem);};
			LYNXFORCEINLINE	CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) 
			{ 
				if (lpb == m_CachedRT[index]) 
					return lpb; 
				CBuffer* lpOld = m_CachedRT[index];
				m_CachedRT[index] = lpb;
				if (lpb)
					m_CachedRTView[index] = ((CD3D11Buffer*)lpb)->GetlpRTView();
				else
					m_CachedRTView[index] = NULL;
				m_lpImmediateContext->OMSetRenderTargets(8, m_CachedRTView, m_CachedDSView);

				return lpOld;
			}			
			LYNXFORCEINLINE CBuffer*				SetDepthBuffer(CBuffer* lpzb) 
			{ 
				if (lpzb == m_CachedDB) 
					return lpzb; 
				CBuffer* lpOld = m_CachedDB;
				m_CachedDB = lpzb; 
				if (lpzb)
					m_CachedDSView = ((CD3D11Buffer*)lpzb)->GetlpDSView();
				else
					m_CachedDSView = NULL;
				m_lpImmediateContext->OMSetRenderTargets(8, m_CachedRTView, m_CachedDSView);

				return lpOld;
			}		
			//LYNXERRORNO								CreateDepthBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms);
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(CBuffer* buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE) { return LYNX_OK; };			
			void									BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect);

			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW CD3D11Texture(m_lpGraphicsSystem); };
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT bbfmt, FORMAT fmt) { return LYNX_TRUE; };
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT fmt) { return LYNX_TRUE; };			
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT bbfmt, FORMAT fmt) { return CheckSRGB(fmt); };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT fmt)  { return CheckFormatSRGBRead(m_lpBackBuffer->GetFormat(), fmt); };			
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT bbfmt, FORMAT fmt) { return CheckSRGB(fmt); };
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT fmt)  { return CheckFormatSRGBWrite(m_lpBackBuffer->GetFormat(), fmt); };			

			//Vertex Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW CD3D11VertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW CD3D11IndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath();

			void									NullVertexShader() {};
			void									NullPixelShader() {};

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			
			LYNXFORCEINLINE CVertexShader*			CreateVertexShader() {return LYNXNEW CD3D11VertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CPixelShader*			CreatePixelShader() {return LYNXNEW CD3D11PixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CShaderConstant*		CreateShaderConstant(CShader* lps, const char* name) 
			{
				CShaderConstant* pConstant = LYNXNEW CD3D11ShaderConstant(lps, name); 
				if (!pConstant)
					return NULL;

				if (!pConstant->vIsValid())
				{
					LYNXDEL pConstant;
					return NULL;
				}

				return pConstant;
			}
			LYNXFORCEINLINE void					ReleaseShaderConstant(CShaderConstant* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW CD3D11ShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return LYNXNEW CD3D11ShaderResource(lps, name); };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return LYNXNEW CD3D11ShaderResource(lps, name, r); };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW CD3D11ShaderGroup(m_lpGraphicsSystem); };	
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
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW CD3D11DepthStencilState(m_lpGraphicsSystem); };			
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW CD3D11BlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW CD3D11RasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW CD3D11SamplerState(m_lpGraphicsSystem); };

			LYNXFORCEINLINE DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) {return (DEPTHBUFFERMODE)0; };
			LYNXFORCEINLINE DEPTHBUFFERMODE			GetDepthBufferMode() {return (DEPTHBUFFERMODE)0; };		
			LYNXFORCEINLINE void					SetSRGBRead(int channel, LYNXBOOL b) {};
			LYNXFORCEINLINE void					SetSRGBWrite(LYNXBOOL b) {};		

			LYNXFORCEINLINE ID3D11Device*			GetlpD3DDevice() const {return m_lpD3DDevice; };
			LYNXFORCEINLINE ID3D11DeviceContext*	GetlpDeviceContext() const {return m_lpImmediateContext; };
			static DXGI_FORMAT						TranslateFormat(FORMAT e, LYNXBOOL bSRGB = LYNX_FALSE);
			static D3D11_FILL_MODE					TranslateFillMode(CRenderer::FILLMODE e);
			static D3D11_CULL_MODE					TranslateCullMode(CRenderer::CULLMODE e);
			static D3D11_COMPARISON_FUNC			TranslateTestFunction(CRenderer::TESTFUNC e);
			static D3D11_STENCIL_OP					TranslateStencilOP(CRenderer::STENCILOP e);
			static D3D11_FILTER						TranslateFilter(CRenderer::FILTERMODE e);
			static D3D11_TEXTURE_ADDRESS_MODE		TranslateAddressMode(CRenderer::ADDRESSMODE e);
			static D3D11_BLEND						TranslateBlendFunction(CRenderer::BLENDFUNC e);
			static D3D11_BLEND_OP					TranslateBlendOP(CRenderer::BLENDOP e);
			static D3D11_QUERY						TranslateQueryType(CRenderer::QUERYTYPE type);
			static DXGI_FORMAT						GetCorrespondingTextureFormat(DXGI_FORMAT f);
			static DXGI_FORMAT						GetCorrespondingShaderResourceViewFormat(DXGI_FORMAT f);

			// D3D11Render function
			ID3D11RenderTargetView**				GetCachedRTView() {return m_CachedRTView; };
			ID3D11DepthStencilView*					GetCacahedDSView() {return m_CachedDSView; };
		private:
			void									AddDriver(int index, IDXGIAdapter* lpa);
			void									FindAllDrivers();
			void									FindAllDisplayModes(FORMAT f, int driver = 0, int output = 0);
			DWORD									GetVertexCount(CRenderer::PRIMITIVETYPE e, DWORD primitive_count);
			D3D11_PRIMITIVE_TOPOLOGY				TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e);
			static LYNXBOOL							CheckSRGB(FORMAT e);
		};			
	}	
}		

#endif

#endif
