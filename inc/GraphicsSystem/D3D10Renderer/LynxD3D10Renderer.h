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

#ifndef __LYNXD3D10RENDERER_H__
#define __LYNXD3D10RENDERER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <DXGI.h>
#include <d3d10.h>
#include <d3dx10.h>

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		class LYNXENGCLASS CD3D10Renderer : public CRenderer
		{			
			LYNX_DECLARE_CLASS(CD3D10Renderer);
		protected:
			IDXGIFactory*				m_lpDXGIFactory;						
			IDXGISwapChain*				m_lpSwapChain;
			ID3D10Device*				m_lpD3DDevice;	
		public:						
			LYNXFORCEINLINE  CD3D10Renderer() {lynxInitRenderer(this); };
			LYNXFORCEINLINE  ~CD3D10Renderer() {lynxReleaseRenderer(this); };											
			
			LYNXFORCEINLINE WORD					GetVersion(void) {return lynxGetVersion(); };			
			LYNXFORCEINLINE const LYNXCHAR*			GetAPIString(void) {return lynxGetGraphicAPIString(); };			

			LYNXFORCEINLINE CRenderer&				SetCurrent(void) {return *((CD3D10Renderer*)(lynxSetCurrentRenderer(this))); };									
			LYNXFORCEINLINE CRenderer&				GetCurrentRenderer(void) {return *((CD3D10Renderer*)(lynxGetCurrentRenderer())); };									

			LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, LPLYNXINITGRAPHICPARAM	lpcp);
			void									Close();

			LYNXFORCEINLINE LYNXBOOL				IsHardwareBusy() {return lynxIsHardwareBusy(); };

			LYNXFORCEINLINE LYNXERRORNO				CreateDepthBuffer(LPLYNXBUFFER pbuffer, DWORD w, DWORD h, LYNXBUFFERFORMAT format, int num_ms) { return lynxCreateDepthBuffer(pbuffer, w, h, format, num_ms); };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderBuffer(LPLYNXBUFFER pbuffer, DWORD w, DWORD h, LYNXBUFFERFORMAT format, int num_ms) { return lynxCreateRenderBuffer(pbuffer, w, h, format, num_ms); };
			LYNXFORCEINLINE LYNXERRORNO				CreateRenderTexture(LPLYNXBUFFER lpzb, LPLYNXTEXTUREINDEX lptexindex, LPLYNXTEXTUREINFO *lplptexinfo, LYNXBUFFERFORMAT fmt, int mipmaplevel, const LYNXCHAR *name, int w, int h, LYNXTEXTURETYPE type, LYNXBUFFERFORMAT format, int ms) { return lynxCreateRenderTexture(lpzb, lptexindex, lplptexinfo, fmt, mipmaplevel, name, w, h, type, format, ms); };
			LYNXFORCEINLINE void					BltBuffer(LPLYNXBUFFER lpdest, LPLYNXBUFFER lpsrc)  {lynxBltBuffer(lpdest, lpsrc); };

			LYNXFORCEINLINE void					Start(void) {lynxStartRender(); };			
			LYNXFORCEINLINE void					End(void) {lynxEndRender(); };
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					ShowBackBuffer() {lynxSwap();	};
			LYNXFORCEINLINE void					Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) { lynxClear(flag, &c, z, s); };

			LYNXFORCEINLINE void					SetViewportRect(int x1, int y1, int x2, int y2) {lynxSetViewportRect(x1, y1, x2, y2); };
			LYNXFORCEINLINE void					GetViewportRect(LPLYNXVIEWPORT lpvp) {lynxGetViewportRect(lpvp); };			
			LYNXFORCEINLINE LYNXDWORD				GetClipPlaneState() {return lynxGetClipPlaneState(); };			
			LYNXFORCEINLINE void					Clipping(LYNXBOOL b) {lynxSetClipping(b); };
			LYNXFORCEINLINE void					SetClipPlane(int i, LPLYNXPLANE p) {lynxSetClipPlane(i, p); };
			LYNXFORCEINLINE void					GetClipPlane(int i, LPLYNXPLANE p) {lynxGetClipPlane(i, p); };

			LYNXFORCEINLINE int						SetWorldMatrix(const CMatrix4& m) {return lynxSetWorldMatrix(&m); };
			LYNXFORCEINLINE void					GetWorldMatrix(CMatrix4& m) {return lynxGetWorldMatrix(&m); };
			LYNXFORCEINLINE CMatrix4&				GetWorldMatrix() {return *((CMatrix4*)(lynxGetlpWorldMatrix())); };
			LYNXFORCEINLINE void					SetViewMatrix(const CMatrix4& m) {return lynxSetViewMatrix(&m); };
			LYNXFORCEINLINE void					GetViewMatrix(CMatrix4& m) {return lynxGetViewMatrix(&m); };
			LYNXFORCEINLINE CMatrix4&				GetViewMatrix() {return *((CMatrix4*)(lynxGetlpViewMatrix())); };
			LYNXFORCEINLINE void					SetProjectionMatrix(const CMatrix4& m) {return lynxSetProjectionMatrix(&m); };
			LYNXFORCEINLINE void					GetProjectionMatrix(CMatrix4& m) {return lynxGetProjectionMatrix(&m); };
			LYNXFORCEINLINE CMatrix4&				GetProjectionMatrix() {return *((CMatrix4*)(lynxGetlpProjectionMatrix())); };
			LYNXFORCEINLINE LYNXDWORD				SetColorWriteMask(LYNXDWORD m) {return lynxSetColorWriteMask(m); };

			LYNXFORCEINLINE void					InitVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxInitVertexLayout(lpvl); };
			LYNXFORCEINLINE void					ReleaseVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxReleaseVertexLayout(lpvl); };
			LYNXFORCEINLINE void					DuplicateVertexlayout(LPLYNXVERTEXLAYOUT lpvd, LPLYNXVERTEXLAYOUT lpvs) {lynxDuplicateVertexLayout(lpvd, lpvs); };
			LYNXFORCEINLINE void					VertexLayoutAddSlot(LPLYNXVERTEXLAYOUT lpvl, int stream, LYNXVERTEXSLOTTYPE type, DWORD type_index, LYNXVERTEXSLOTFORMAT format, DWORD number) {lynxVertexLayoutAddSlot(lpvl, stream, type, type_index, format, number); };
			LYNXFORCEINLINE void 					VertexLayoutEndSlot(LPLYNXVERTEXLAYOUT lpvl) {lynxVertexLayoutEndSlot(lpvl); };
			LYNXFORCEINLINE void					MakeVertexLayout(LPLYNXVERTEXLAYOUT lpvl, LPLYNXVERTEXSLOT lpslot, DWORD num) {lynxMakeVertexLayout(lpvl, lpslot, num); };
			LYNXFORCEINLINE DWORD					MakeVertexShaderDecl(LPLYNXVERTEXSHADERDECL lpvsdecl, LPCLYNXVERTEXSLOT lpslot, DWORD num) {return lynxMakeVertexShaderDecl(lpvsdecl, lpslot, num); };
			LYNXFORCEINLINE void					SetVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxSetVertexLayout(lpvl); };
			LYNXFORCEINLINE LPLYNXVERTEXLAYOUT		GetBuiltinVertexLayout(LYNXBUILTINVERTEXTYPE t) {return lynxGetBuiltinVertexLayout(t); };
			LYNXFORCEINLINE void					MakeBuiltinVertexLayout(LPLYNXVERTEXLAYOUT lpvl, LYNXBUILTINVERTEXTYPE t) {lynxMakeBuiltinVertexLayout(lpvl, t); };
			LYNXFORCEINLINE int						GetSlotOffset(LPLYNXVERTEXLAYOUT lpl, LYNXVERTEXSLOTTYPE type, int typeindex) {return lynxGetSlotOffset(lpl, type, typeindex); };
			LYNXFORCEINLINE void					SetupBuiltinVertexLayouts(void) {lynxSetupBuiltinVertexLayouts(); };
			LYNXFORCEINLINE void					ReleaseBuiltinVertexLayouts(void) {lynxReleaseBuiltinVertexLayouts(); };

			LYNXFORCEINLINE LYNXDEPTHBUFFERMODE		SetDepthBufferMode(LYNXDEPTHBUFFERMODE zbm) {return lynxSetDepthBufferMode(zbm); };
			LYNXFORCEINLINE LYNXDEPTHBUFFERMODE		GetDepthBufferMode() {return lynxGetDepthBufferMode(); };


			LYNXFORCEINLINE LYNXBOOL				CheckTextureFormat(LYNXBUFFERFORMAT bbfmt, LYNXBUFFERFORMAT fmt) { return lynxCheckTextureFormat(bbfmt, fmt); };
			LYNXFORCEINLINE LYNXERRORNO				CreateBlankTexture(LPLYNXTEXTUREINDEX lptexindex, LPLYNXTEXTUREINFO *lplptexinfo, int mipmaplevel, const LYNXCHAR *name, int w, int h, LYNXTEXTURETYPE type, LYNXBUFFERFORMAT format) {return (lynxCreateBlankTexture(lptexindex, lplptexinfo, mipmaplevel, name, w, h, type, format)); };
			LYNXFORCEINLINE LYNXTEXTUREINDEX		SetTexture(int channel, LYNXTEXTUREINDEX t) {return (lynxSetTexture(channel, t)); };

			LYNXFORCEINLINE LYNXBUFFER&				GetSystemBackBuffer() {return SystemData.BackBuffer; };			
			LYNXFORCEINLINE LYNXBUFFER&				GetSystemDepthBuffer() {return SystemData3D.DepthBuffer; };	
			LYNXFORCEINLINE void					GetSystemOrigin(int* x, int* y) {*x = SystemData.Origin.x; *y = SystemData.Origin.y; };
			LYNXFORCEINLINE void					SetSystemOrigin(int x, int y) {SystemData.Origin.x = x; SystemData.Origin.y = y; };

			LYNXFORCEINLINE LPLYNXBUFFER			GetBackBuffer() {return lpBackBuffer; };		
			LYNXFORCEINLINE LPLYNXBUFFER			GetRenderBuffer(int index) {return lpRenderBuffer[index]; };	
			LYNXFORCEINLINE LPLYNXBUFFER			GetDepthBuffer() {return lpDepthBuffer; };	
			LYNXFORCEINLINE LPLYNXBUFFER			SetBackBuffer(LPLYNXBUFFER lpb) {return lynxSetBackBuffer(lpb); };	
			LYNXFORCEINLINE LPLYNXBUFFER			SetRenderBuffer(int index, LPLYNXBUFFER lpb) {return lynxSetRenderBuffer(index, lpb); };			
			LYNXFORCEINLINE LPLYNXBUFFER			SetDepthBuffer(LPLYNXBUFFER lpzb) {return lynxSetDepthBuffer(lpzb); };			

			LYNXFORCEINLINE void					UpdateBufferToTexture(LYNXTEXTUREINDEX ti, LPLYNXBUFFER lpb) { lynxUpdateBufferToTexture(ti, lpb); };

			LYNXFORCEINLINE void					SetShaderBasePath(const LYNXCHAR *path) {lynxSetShaderBasePath(path); };			
			LYNXFORCEINLINE void					SetVertexShaderVersion(const LYNXCHAR *ver) {lynxSetVertexShaderVersion(ver); };
			LYNXFORCEINLINE void					SetPixelShaderVersion(const LYNXCHAR *ver) {lynxSetPixelShaderVersion(ver); };

			LYNXFORCEINLINE LYNXERRORNO				LoadVertexShader(LYNXVERTEXSHADERINDEX* si, const LYNXCHAR *filename, const LYNXCHAR *ver, LYNXFILE* fs, long offset) {return lynxLoadVertexShader(si, filename, ver, fs, offset); };			
			LYNXFORCEINLINE LYNXERRORNO				LoadCompiledVertexShader(LYNXVERTEXSHADERINDEX* si, LYNXCHAR *filename, LYNXFILE* fs, long offset) {return lynxLoadCompiledVertexShader(si, filename, fs, offset); };		
			LYNXFORCEINLINE LYNXERRORNO				ReleaseVertexShader(LYNXVERTEXSHADERINDEX si) {return lynxReleaseVertexShader(si); };					
			LYNXFORCEINLINE LYNXVERTEXSHADERINDEX	SetVertexShader(LYNXVERTEXSHADERINDEX si) {return lynxSetVertexShader(si); };					
			LYNXFORCEINLINE LYNXVERTEXSHADERINDEX	GetCurrentVertexShader() {return lynxGetCurrentVertexShader(); };					

			LYNXFORCEINLINE LYNXERRORNO				LoadPixelShader(LYNXPIXELSHADERINDEX* si, const LYNXCHAR *filename, const LYNXCHAR *ver, LYNXFILE* fs, long offset) {return lynxLoadPixelShader(si, filename, ver, fs, offset); };			
			LYNXFORCEINLINE LYNXERRORNO				LoadCompiledPixelShader(LYNXPIXELSHADERINDEX* si, LYNXCHAR *filename, LYNXFILE* fs, long offset) {return lynxLoadCompiledPixelShader(si, filename, fs, offset); };		
			LYNXFORCEINLINE LYNXERRORNO				ReleasePixelShader(LYNXPIXELSHADERINDEX si) {return lynxReleasePixelShader(si); };					
			LYNXFORCEINLINE LYNXPIXELSHADERINDEX	SetPixelShader(LYNXPIXELSHADERINDEX si) {return lynxSetPixelShader(si); };					
			LYNXFORCEINLINE LYNXPIXELSHADERINDEX	GetCurrentPixelShader() {return lynxGetCurrentPixelShader(); };					
			
			LYNXFORCEINLINE void					DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {lynxDrawStrip(lpv, x, y); };

			LYNXFORCEINLINE void					DrawElementsBO(LYNXPRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {lynxDrawElementsBO(type, base_vert, num_vert, base_index, primitive_count); };
			LYNXFORCEINLINE void					DrawPrimitiveBO(LPLYNXPRIMITIVE lpp, DWORD num_vert) {lynxDrawPrimitiveBO(lpp, num_vert); };			
			LYNXFORCEINLINE void					DrawElements(LYNXPRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {lynxDrawElements(type, base_vert, num_vert, base_index, primitive_count); };
			LYNXFORCEINLINE void					DrawPrimitive(LPLYNXPRIMITIVE lpp, DWORD num_vert) {lynxDrawPrimitive(lpp, num_vert); };
		};			
	}
}		

#endif