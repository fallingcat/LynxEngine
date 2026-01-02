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

#ifndef __LYNXD3D9RENDERER_H__
#define __LYNXD3D9RENDERER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/D3D9Renderer/LynxD3D9Buffer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Texture.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Query.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexLayout.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9PixelShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexArray.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9IndexArray.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9DepthStencilState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9BlendState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9RasterizerState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9SamplerState.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderConstant.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderConstantGroup.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderResource.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderGroup.h>

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class CD3D9Include : public ID3DXInclude
		{
		protected:
			CString m_CurrentShaderPath;
		public:			
			void ComputeCurrentShaderPath(const LYNXCHAR* path);
			COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);			
			COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);			
		};

		class LYNXENGCLASS CD3D9Renderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(CD3D9Renderer);
		public:
			typedef struct {				
				CString						Name;
				CString						Desc;
				CString						DeviceName;
				DWORD						VendorId;
				DWORD						DeviceId;
				DWORD						SubSysId;
				DWORD						Revision;
				DWORD						DedicatedVideoMemory;
				DWORD						DedicatedSystemMemory;
				DWORD						SharedSystemMemory;
			}DRIVERDATA;

			typedef	struct {
				float					sx, sy, sz;
				float					rhw; 
				D3DCOLOR				color; 
			}D3DTLVERTEX;

			typedef	struct {
				float					x, y, z;	
				D3DCOLOR				color; 
			}D3DVERTEX;

			CD3D9Include							m_ShaderIncludeHandler;
		private:			
			CMatrix4								m_WorldM, m_ViewM, m_ProjM;
		protected:			
			CArray<DRIVERDATA>						m_DriverList;
			int										m_CurrentDriver;

			IDirect3D9*								m_lpD3D;
			IDirect3DDevice9*						m_lpD3DDevice;
			IDirect3DSwapChain9*					m_lpSwapChain;
			LPD3DXFONT								m_lpSysFont;						
		public:						
			CD3D9Renderer();
			CD3D9Renderer(CGraphicsSystem* lpgs);
			~CD3D9Renderer();														
			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return _T("Direct3D 9"); };						
			LYNXFORCEINLINE const LYNXCHAR*			GetDriverDesc(void) {return m_DriverList[m_CurrentDriver].Desc.c_str(); };

			//LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp);
			void									Close();
			void									Resize(int cx, int cy);

			LYNXFORCEINLINE LYNXBOOL				IsValid() {return (m_lpD3DDevice?LYNX_TRUE:LYNX_FALSE); };
			LYNXFORCEINLINE LYNXWINDOW				GetWindow() {return m_Window; };			
			
			LYNXFORCEINLINE void					Start(void) {m_lpD3DDevice->BeginScene(); SetHardwareBusy(LYNX_TRUE);};			
			LYNXFORCEINLINE void					End(void) {m_lpD3DDevice->EndScene(); SetHardwareBusy(LYNX_FALSE); };
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					SwapBackBuffer() {m_lpSwapChain->Present(NULL, NULL, NULL, NULL, 0); ResetAllCounters(); };
			LYNXFORCEINLINE void					Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) 
			{
				DWORD Flag = 0;
				if (flag & RENDER_BUFFER)
					Flag |= D3DCLEAR_TARGET;
				if (flag & DEPTH_BUFFER)
					Flag |= D3DCLEAR_ZBUFFER;
				if (flag & STENCIL_BUFFER)
					Flag |= D3DCLEAR_STENCIL;
	            	
				m_lpD3DDevice->Clear(0, NULL, Flag, D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha), z, (DWORD)s);					
			}			
			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					SetWorldMatrix(const CMatrix4& m) {m_WorldM = m; m_lpD3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&m); };
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {m = m_WorldM; };
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return m_WorldM; };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {m_ViewM = m; m_lpD3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&m); };
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {m = m_ViewM; };
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return m_ViewM; };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {m_ProjM = m; m_lpD3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&m); };
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {m = m_ProjM; };
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return m_ProjM;};
			
			LYNXFORCEINLINE void					UseFog(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_FOGENABLE, b); };
			LYNXFORCEINLINE void					UseSpecular(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, b); };
			LYNXFORCEINLINE void					UseAlphaBlend(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, b); };			
			LYNXFORCEINLINE void					UseAlphaTest(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, b); };			
			LYNXFORCEINLINE void					SetAlphaReference(float af) {m_lpD3DDevice->SetRenderState(D3DRS_ALPHAREF, LYNX_ROUND(af*255)); };						
			LYNXFORCEINLINE void					SetFixedVertexLayout(FIXEDVERTEXLAYOUT vs) {m_lpD3DDevice->SetFVF(vs); m_CachedVertexLayout = NULL; };

			void									DrawStrip(FIXEDVERTEX* lpv, int x, int y);
			void									DrawTriangle(FIXEDVERTEX& v1, FIXEDVERTEX& v2, FIXEDVERTEX& v3);
			void									DrawTriangleStrip(FIXEDVERTEX* lpv);
			void									DrawTriangleFans(FIXEDVERTEX* lpv, int num);
			void									DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count);
			void									DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert);
			void									DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c);
			void									DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c);

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = m_WindowOrigin.x; *y = m_WindowOrigin.y;};
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {m_WindowOrigin.x = x; m_WindowOrigin.y = y; };
			
			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			void									SetViewportRect(int x1, int y1, int x2, int y2);			
			void									SetScissorRect(int x1, int y1, int x2, int y2);
			LYNXFORCEINLINE void					SetScissorRect(LPLYNXRECT r) {SetScissorRect(r->x1, r->y1, r->x2, r->y2); };
			void									GetScissorRect(LPLYNXRECT lpsr);
			LYNXFORCEINLINE LYNXBOOL				EnableScissorRect(LYNXBOOL b) { LYNXBOOL bTemp = m_bCachedScissorEnabled; m_lpD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, b); m_bCachedScissorEnabled = b; return bTemp; };
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return 0; };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_CLIPPING , b); };
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {};
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {};			

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			CQuery*									CreateQuery() {return LYNXNEW CD3D9Query(m_lpGraphicsSystem); };

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexLayout*			CreateVertexLayout() {return LYNXNEW CD3D9VertexLayout(m_lpGraphicsSystem); };

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			//LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {};
			void									DrawScreenQuad(void* lpv, DWORD size);
			void									DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count);
			void									DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert);
			void									DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert);			
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------			
			LYNXFORCEINLINE virtual CBuffer*		CreateBuffer() {return LYNXNEW CD3D9Buffer(m_lpGraphicsSystem);};
			LYNXFORCEINLINE	CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) 
			{ 
				if (lpb == m_CachedRT[index]) 
					return lpb; 
				CBuffer* lpOld = m_CachedRT[index];
				m_CachedRT[index] = lpb;
				if (lpb)
					lpb->vSetAsRenderTarget(index);  	
				else
					m_lpD3DDevice->SetRenderTarget(index, NULL);  	

				return lpOld;
			}			
			LYNXFORCEINLINE CBuffer*				SetDepthBuffer(CBuffer* lpzb) 
			{ 
				if (lpzb == m_CachedDB) 
					return lpzb; 
				CBuffer* lpOld = m_CachedDB;
				m_CachedDB = lpzb;
				if (lpzb)
					lpzb->vSetAsDepthBuffer();	
				else
					m_lpD3DDevice->SetDepthStencilSurface(NULL);  	

				return lpOld;
			}		
			LYNXFORCEINLINE void					BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect)  
			{
				LPRECT	lpDest, lpSrc;
				RECT	Dest, Src;
				
				if (lpdest == lpsrc)
					return;

				if (drect)
				{
					Dest.left	= drect->x1;
					Dest.top	= drect->y1;
					Dest.right	= drect->x2;
					Dest.bottom = drect->y2;
					lpDest		= &Dest;
				}
				else
					lpDest = NULL;

				if (srect)
				{
					Src.left	= srect->x1;
					Src.top		= srect->y1;
					Src.right	= srect->x2;
					Src.bottom	= srect->y2;
					lpSrc		= &Src;
				}
				else
					lpSrc = NULL;

				m_lpD3DDevice->StretchRect(((CD3D9Buffer*)((CBuffer*)lpsrc))->GetlpSurface(), lpSrc, ((CD3D9Buffer*)((CBuffer*)lpdest))->GetlpSurface(), lpDest, D3DTEXF_POINT);
			}
			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual CTexture*		CreateTexture() {return LYNXNEW CD3D9Texture(m_lpGraphicsSystem); };
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT bbfmt, FORMAT fmt) 
			{ 
				return ((m_lpD3D->CheckDeviceFormat(m_CurrentDriver, D3DDEVTYPE_HAL, TranslateFormat(bbfmt), 0, D3DRTYPE_TEXTURE, TranslateFormat(fmt)) == D3D_OK)?LYNX_TRUE:LYNX_FALSE); 
			}
			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(FORMAT fmt) { return CheckTextureFormat(m_lpBackBuffer->GetFormat(), fmt); };
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT bbfmt, FORMAT fmt)
			{ 
				return ((m_lpD3D->CheckDeviceFormat(m_CurrentDriver, D3DDEVTYPE_HAL, TranslateFormat(bbfmt), D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, TranslateFormat(fmt)) == D3D_OK)?LYNX_TRUE:LYNX_FALSE); 
			}
			LYNXFORCEINLINE LYNXBOOL				CheckDepthFormat(FORMAT fmt) { return CheckDepthFormat(m_lpBackBuffer->GetFormat(), fmt); };
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT bbfmt, FORMAT fmt)
			{ 
				return ((m_lpD3D->CheckDeviceFormat(m_CurrentDriver, D3DDEVTYPE_HAL, TranslateFormat(bbfmt), D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, TranslateFormat(fmt)) == D3D_OK)?LYNX_TRUE:LYNX_FALSE); 
			}
			LYNXFORCEINLINE LYNXBOOL				CheckRTFormat(FORMAT fmt) { return CheckRTFormat(m_lpBackBuffer->GetFormat(), fmt); };			
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT bbfmt, FORMAT fmt)
			{ 
				return ((m_lpD3D->CheckDeviceFormat(m_CurrentDriver, D3DDEVTYPE_HAL, TranslateFormat(bbfmt), D3DUSAGE_QUERY_SRGBREAD, D3DRTYPE_TEXTURE, TranslateFormat(fmt)) == D3D_OK)?LYNX_TRUE:LYNX_FALSE); 
			}
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBRead(FORMAT fmt)  { return CheckFormatSRGBRead(m_lpBackBuffer->GetFormat(), fmt); };			
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT bbfmt, FORMAT fmt)
			{ 
				return ((m_lpD3D->CheckDeviceFormat(m_CurrentDriver, D3DDEVTYPE_HAL, TranslateFormat(bbfmt), D3DUSAGE_RENDERTARGET|D3DUSAGE_QUERY_SRGBWRITE, D3DRTYPE_TEXTURE, TranslateFormat(fmt)) == D3D_OK)?LYNX_TRUE:LYNX_FALSE); 
			}
			LYNXFORCEINLINE LYNXBOOL				CheckFormatSRGBWrite(FORMAT fmt)  { return CheckFormatSRGBWrite(m_lpBackBuffer->GetFormat(), fmt); };			

			//Vertex Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CVertexArray*			CreateVertexArray() {return LYNXNEW CD3D9VertexArray(m_lpGraphicsSystem); };
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CIndexArray*			CreateIndexArray() {return LYNXNEW CD3D9IndexArray(m_lpGraphicsSystem); };
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------			
			virtual void							SetShaderTarget(SHADERTARGET t) {CRenderer::SetShaderTarget(t); };
			virtual CString&						ComputeCachedCompiledShaderPath();

			void									NullVertexShader() {m_lpD3DDevice->SetVertexShader(NULL); };
			void									NullPixelShader() {m_lpD3DDevice->SetPixelShader(NULL); };

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {};			
			
			LYNXFORCEINLINE CVertexShader*			CreateVertexShader() {return LYNXNEW CD3D9VertexShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CPixelShader*			CreatePixelShader() {return LYNXNEW CD3D9PixelShader(m_lpGraphicsSystem); };
			LYNXFORCEINLINE CShaderConstant*		CreateShaderConstant(CShader* lps, const char* name) 
			{
				CShaderConstant* pConstant = LYNXNEW CD3D9ShaderConstant(lps, name); 
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
			LYNXFORCEINLINE CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) {return LYNXNEW CD3D9ShaderConstantGroup(lps, name); };
			LYNXFORCEINLINE void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name) {return LYNXNEW CD3D9ShaderResource(lps, name); };
			LYNXFORCEINLINE CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) {return LYNXNEW CD3D9ShaderResource(lps, name, r); };
			LYNXFORCEINLINE void					ReleaseShaderResource(CShaderResource* lpsc) {LYNXDEL lpsc; };
			LYNXFORCEINLINE CShaderGroup*			CreateShaderGroup() {return LYNXNEW CD3D9ShaderGroup(m_lpGraphicsSystem); };
			LYNXFORCEINLINE void					ReleaseShaderGroup(CShaderGroup* lpsg) {LYNXDEL lpsg; };
						
			LYNXFORCEINLINE void					SetVSConstantF(int i, const LYNXREAL *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantF(i, (float*)lpdata, num); };
			LYNXFORCEINLINE void					GetVSConstantF(int i, LYNXREAL *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantF(i, (float*)lpdata, num);};
			LYNXFORCEINLINE void					SetVSConstantI(int i, const int *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantI(i, lpdata, num);};
			LYNXFORCEINLINE void					GetVSConstantI(int i, int *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantI(i, lpdata, num);};
			LYNXFORCEINLINE void					SetVSConstantB(int i, const BOOL *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantB(i, lpdata, num);};
			LYNXFORCEINLINE void					GetVSConstantB(int i, BOOL *lpdata, int num) {m_lpD3DDevice->SetVertexShaderConstantB(i, lpdata, num);};

			LYNXFORCEINLINE void					SetPSConstantF(int i, const LYNXREAL *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantF(i, (float*)lpdata, num); };
			LYNXFORCEINLINE void					GetPSConstantF(int i, LYNXREAL *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantF(i, (float*)lpdata, num);};
			LYNXFORCEINLINE void					SetPSConstantI(int i, const int *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantI(i, lpdata, num);};
			LYNXFORCEINLINE void					GetPSConstantI(int i, int *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantI(i, lpdata, num);};
			LYNXFORCEINLINE void					SetPSConstantB(int i, const BOOL *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantB(i, lpdata, num);};
			LYNXFORCEINLINE void					GetPSConstantB(int i, BOOL *lpdata, int num) {m_lpD3DDevice->SetPixelShaderConstantB(i, lpdata, num);};

			//Render State ----------------------------------------------------------------------------------------------------------------------------------------------			
			LYNXFORCEINLINE virtual CDepthStencilState*	CreateDepthStencilState() {return LYNXNEW CD3D9DepthStencilState(m_lpGraphicsSystem); };			
			LYNXFORCEINLINE virtual CBlendState*		CreateBlendState() {return LYNXNEW CD3D9BlendState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CRasterizerState*	CreateRasterizerState() {return LYNXNEW CD3D9RasterizerState(m_lpGraphicsSystem); };
			LYNXFORCEINLINE virtual CSamplerState*		CreateSamplerState() {return LYNXNEW CD3D9SamplerState(m_lpGraphicsSystem); };

			//LYNXFORCEINLINE DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) {return (DEPTHBUFFERMODE)0; };
			//LYNXFORCEINLINE DEPTHBUFFERMODE			GetDepthBufferMode() {return (DEPTHBUFFERMODE)0; };		
			LYNXDWORD								SetColorWriteMask(LYNXDWORD m);
			LYNXFORCEINLINE void					SetSRGBRead(int channel, LYNXBOOL b) {m_lpD3DDevice->SetSamplerState(channel, D3DSAMP_SRGBTEXTURE , b); };
			LYNXFORCEINLINE void					SetSRGBWrite(LYNXBOOL b) {m_lpD3DDevice->SetRenderState(D3DRS_SRGBWRITEENABLE , b); };		

			static D3DQUERYTYPE						TranslateQueryType(CRenderer::QUERYTYPE type);
			static D3DPRIMITIVETYPE					TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e);
			static D3DFORMAT						TranslateFormat(CRenderer::FORMAT f);
			static CRenderer::FORMAT				TranslateFormat(D3DFORMAT f);
			static D3DBLEND							TranslateBlendFunction(CRenderer::BLENDFUNC e);
			static D3DBLENDOP						TranslateBlendOP(CRenderer::BLENDOP e);
			static D3DCMPFUNC						TranslateTestFunction(CRenderer::TESTFUNC e);
			static D3DSTENCILOP						TranslateStencilOP(CRenderer::STENCILOP e);
			static D3DFILLMODE						TranslateFillMode(CRenderer::FILLMODE e);
			static D3DCULL							TranslateCullMode(CRenderer::CULLMODE e);
			static D3DTEXTUREFILTERTYPE				TranslateFilter(CRenderer::FILTERMODE e);
			static D3DTEXTUREADDRESS				TranslateAddressMode(CRenderer::ADDRESSMODE e);
			static D3DDECLUSAGE						TranslateVertexSlotType(CRenderer::VERTEXSLOTTYPE e);

			LYNXFORCEINLINE IDirect3DDevice9*		GetlpD3DDevice() {return m_lpD3DDevice; };
			LYNXFORCEINLINE LPD3DXFONT				GetlpSystemFont() {return m_lpSysFont; };
		private:
			void									FindAllDrivers();
			void									FindAllDisplayModes(FORMAT f, int driver = 0, int output = 0);
			DWORD									FindRefreshRate(DWORD w, DWORD h);
		};			
	}	
}		

#endif

#endif