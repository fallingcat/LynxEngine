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

#ifndef __LYNXRENDERER_H__
#define __LYNXRENDERER_H__

#include <LynxSDK/LynxMathWrap_Opt.h>
#include <LynxGraphics.h>
#include <LynxResource.h>

using namespace LynxSDK::Math;

namespace LynxSDK
{
	namespace Graphics
	{	
		class LYNXENGCLASS CRenderer : public LYNXRENDERER
		{			
			LYNX_DECLARE_CLASS(CRenderer);
		public:						
			LYNXFORCEINLINE  CRenderer() {lynxInitRenderer(this); };
			LYNXFORCEINLINE  ~CRenderer() {lynxReleaseRenderer(this); };											
			
			static LYNXFORCEINLINE WORD				GetVersion(void) {return lynxGetVersion(); };			
			static LYNXFORCEINLINE const LYNXCHAR*	GetAPIString(void) {return lynxGetGraphicAPIString(); };			

			LYNXFORCEINLINE CRenderer&				SetCurrent(void) {return *((CRenderer*)(lynxSetCurrentRenderer(this))); };									
			static LYNXFORCEINLINE CRenderer&		GetCurrentRenderer(void) {return *((CRenderer*)(lynxGetCurrentRenderer())); };									

			LYNXFORCEINLINE const LYNXVIDEOMODE&	GetCurrentVideoMode(void) {return SystemData.CurrentVideoMode; };

			LYNXBOOL								Open(LYNXWINDOW wnd, LPLYNXINITGRAPHICPARAM	lpcp);
			void									Close();

			static LYNXFORCEINLINE LYNXBOOL			IsHardwareBusy() {return lynxIsHardwareBusy(); };

			static LYNXFORCEINLINE LYNXERRORNO		CreateDepthBuffer(LPLYNXBUFFER pbuffer, DWORD w, DWORD h, LYNXBUFFERFORMAT format, int num_ms) { return lynxCreateDepthBuffer(pbuffer, w, h, format, num_ms); };
			static LYNXFORCEINLINE LYNXERRORNO		CreateRenderBuffer(LPLYNXBUFFER pbuffer, DWORD w, DWORD h, LYNXBUFFERFORMAT format, int num_ms) { return lynxCreateRenderBuffer(pbuffer, w, h, format, num_ms); };
			static LYNXFORCEINLINE LYNXERRORNO		CreateRenderTexture(LPLYNXBUFFER lpzb, LPLYNXTEXTUREINDEX lptexindex, LPLYNXTEXTUREINFO *lplptexinfo, LYNXBUFFERFORMAT fmt, int mipmaplevel, const LYNXCHAR *name, int w, int h, LYNXTEXTURETYPE type, LYNXBUFFERFORMAT format, int ms) { return lynxCreateRenderTexture(lpzb, lptexindex, lplptexinfo, fmt, mipmaplevel, name, w, h, type, format, ms); };
			static LYNXFORCEINLINE void				BltBuffer(LPLYNXBUFFER lpdest, LPLYNXBUFFER lpsrc)  {lynxBltBuffer(lpdest, lpsrc); };

			static LYNXFORCEINLINE void				Start(void) {lynxStartRender(); };			
			static LYNXFORCEINLINE void				End(void) {lynxEndRender(); };
			//LYNXFORCEINLINE void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			LYNXFORCEINLINE void					ShowBackBuffer() {lynxSwap();	};
			static LYNXFORCEINLINE void				Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) { lynxClear(flag, &c, z, s); };

			static LYNXFORCEINLINE void				SetViewportRect(int x1, int y1, int x2, int y2) {lynxSetViewportRect(x1, y1, x2, y2); };
			static LYNXFORCEINLINE void				GetViewportRect(LPLYNXVIEWPORT lpvp) {lynxGetViewportRect(lpvp); };			
			static LYNXFORCEINLINE LYNXDWORD		GetClipPlaneState() {return lynxGetClipPlaneState(); };			
			static LYNXFORCEINLINE void				Clipping(LYNXBOOL b) {lynxSetClipping(b); };
			static LYNXFORCEINLINE void				SetClipPlane(int i, LPLYNXPLANE p) {lynxSetClipPlane(i, p); };
			static LYNXFORCEINLINE void				GetClipPlane(int i, LPLYNXPLANE p) {lynxGetClipPlane(i, p); };

			static LYNXFORCEINLINE int				SetWorldMatrix(const CMatrix4& m) {return lynxSetWorldMatrix(&m); };
			static LYNXFORCEINLINE void				GetWorldMatrix(CMatrix4& m) {return lynxGetWorldMatrix(&m); };
			static LYNXFORCEINLINE CMatrix4&		GetWorldMatrix() {return *((CMatrix4*)(lynxGetlpWorldMatrix())); };
			static LYNXFORCEINLINE void				SetViewMatrix(const CMatrix4& m) {return lynxSetViewMatrix(&m); };
			static LYNXFORCEINLINE void				GetViewMatrix(CMatrix4& m) {return lynxGetViewMatrix(&m); };
			static LYNXFORCEINLINE CMatrix4&		GetViewMatrix() {return *((CMatrix4*)(lynxGetlpViewMatrix())); };
			static LYNXFORCEINLINE void				SetProjectionMatrix(const CMatrix4& m) {return lynxSetProjectionMatrix(&m); };
			static LYNXFORCEINLINE void				GetProjectionMatrix(CMatrix4& m) {return lynxGetProjectionMatrix(&m); };
			static LYNXFORCEINLINE CMatrix4&		GetProjectionMatrix() {return *((CMatrix4*)(lynxGetlpProjectionMatrix())); };
			static LYNXFORCEINLINE LYNXDWORD		SetColorWriteMask(LYNXDWORD m) {return lynxSetColorWriteMask(m); };

			static LYNXFORCEINLINE void				InitVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxInitVertexLayout(lpvl); };
			static LYNXFORCEINLINE void				ReleaseVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxReleaseVertexLayout(lpvl); };
			static LYNXFORCEINLINE void				DuplicateVertexlayout(LPLYNXVERTEXLAYOUT lpvd, LPLYNXVERTEXLAYOUT lpvs) {lynxDuplicateVertexLayout(lpvd, lpvs); };
			static LYNXFORCEINLINE void				VertexLayoutAddSlot(LPLYNXVERTEXLAYOUT lpvl, int stream, LYNXVERTEXSLOTTYPE type, DWORD type_index, LYNXVERTEXSLOTFORMAT format, DWORD number) {lynxVertexLayoutAddSlot(lpvl, stream, type, type_index, format, number); };
			static LYNXFORCEINLINE void 			VertexLayoutEndSlot(LPLYNXVERTEXLAYOUT lpvl) {lynxVertexLayoutEndSlot(lpvl); };
			static LYNXFORCEINLINE void				MakeVertexLayout(LPLYNXVERTEXLAYOUT lpvl, LPLYNXVERTEXSLOT lpslot, DWORD num) {lynxMakeVertexLayout(lpvl, lpslot, num); };
			static LYNXFORCEINLINE DWORD			MakeVertexShaderDecl(LPLYNXVERTEXSHADERDECL lpvsdecl, LPCLYNXVERTEXSLOT lpslot, DWORD num) {return lynxMakeVertexShaderDecl(lpvsdecl, lpslot, num); };
			static LYNXFORCEINLINE void				SetVertexLayout(LPLYNXVERTEXLAYOUT lpvl) {lynxSetVertexLayout(lpvl); };
			static LYNXFORCEINLINE LPLYNXVERTEXLAYOUT GetBuiltinVertexLayout(LYNXBUILTINVERTEXTYPE t) {return lynxGetBuiltinVertexLayout(t); };
			static LYNXFORCEINLINE void				MakeBuiltinVertexLayout(LPLYNXVERTEXLAYOUT lpvl, LYNXBUILTINVERTEXTYPE t) {lynxMakeBuiltinVertexLayout(lpvl, t); };
			static LYNXFORCEINLINE int				GetSlotOffset(LPLYNXVERTEXLAYOUT lpl, LYNXVERTEXSLOTTYPE type, int typeindex) {return lynxGetSlotOffset(lpl, type, typeindex); };
			static LYNXFORCEINLINE void				SetupBuiltinVertexLayouts(void) {lynxSetupBuiltinVertexLayouts(); };
			static LYNXFORCEINLINE void				ReleaseBuiltinVertexLayouts(void) {lynxReleaseBuiltinVertexLayouts(); };

			static LYNXFORCEINLINE LYNXDEPTHBUFFERMODE	SetDepthBufferMode(LYNXDEPTHBUFFERMODE zbm) {return lynxSetDepthBufferMode(zbm); };
			static LYNXFORCEINLINE LYNXDEPTHBUFFERMODE	GetDepthBufferMode() {return lynxGetDepthBufferMode(); };


			static LYNXFORCEINLINE LYNXBOOL			CheckTextureFormat(LYNXBUFFERFORMAT bbfmt, LYNXBUFFERFORMAT fmt) { return lynxCheckTextureFormat(bbfmt, fmt); };
			static LYNXFORCEINLINE LYNXERRORNO		CreateBlankTexture(LPLYNXTEXTUREINDEX lptexindex, LPLYNXTEXTUREINFO *lplptexinfo, int mipmaplevel, const LYNXCHAR *name, int w, int h, LYNXTEXTURETYPE type, LYNXBUFFERFORMAT format) {return (lynxCreateBlankTexture(lptexindex, lplptexinfo, mipmaplevel, name, w, h, type, format)); };
			static LYNXFORCEINLINE LYNXTEXTUREINDEX SetTexture(int channel, LYNXTEXTUREINDEX t) {return (lynxSetTexture(channel, t)); };

			LYNXFORCEINLINE LYNXBUFFER&				GetSystemBackBuffer() {return SystemData.BackBuffer; };			
			LYNXFORCEINLINE LYNXBUFFER&				GetSystemDepthBuffer() {return SystemData3D.DepthBuffer; };	

			LYNXFORCEINLINE LPLYNXBUFFER			GetBackBuffer() {return lpBackBuffer; };		
			LYNXFORCEINLINE LPLYNXBUFFER			GetRenderBuffer(int index) {return lpRenderBuffer[index]; };	
			LYNXFORCEINLINE LPLYNXBUFFER			GetDepthBuffer() {return lpDepthBuffer; };	
			static LYNXFORCEINLINE LPLYNXBUFFER		SetBackBuffer(LPLYNXBUFFER lpb) {return lynxSetBackBuffer(lpb); };	
			static LYNXFORCEINLINE LPLYNXBUFFER		SetRenderBuffer(int index, LPLYNXBUFFER lpb) {return lynxSetRenderBuffer(index, lpb); };			
			static LYNXFORCEINLINE LPLYNXBUFFER		SetDepthBuffer(LPLYNXBUFFER lpzb) {return lynxSetDepthBuffer(lpzb); };			

			static LYNXFORCEINLINE void				UpdateBufferToTexture(LYNXTEXTUREINDEX ti, LPLYNXBUFFER lpb) { lynxUpdateBufferToTexture(ti, lpb); };

			static LYNXFORCEINLINE void				SetShaderBasePath(const LYNXCHAR *path) {lynxSetShaderBasePath(path); };			
			static LYNXFORCEINLINE void				SetVertexShaderVersion(const LYNXCHAR *ver) {lynxSetVertexShaderVersion(ver); };
			static LYNXFORCEINLINE void				SetPixelShaderVersion(const LYNXCHAR *ver) {lynxSetPixelShaderVersion(ver); };

			static LYNXFORCEINLINE LYNXERRORNO		LoadVertexShader(LYNXVERTEXSHADERINDEX* si, const LYNXCHAR *filename, const LYNXCHAR *ver, LYNXFILE* fs, long offset) {return lynxLoadVertexShader(si, filename, ver, fs, offset); };			
			static LYNXFORCEINLINE LYNXERRORNO		LoadCompiledVertexShader(LYNXVERTEXSHADERINDEX* si, LYNXCHAR *filename, LYNXFILE* fs, long offset) {return lynxLoadCompiledVertexShader(si, filename, fs, offset); };		
			static LYNXFORCEINLINE LYNXERRORNO		ReleaseVertexShader(LYNXVERTEXSHADERINDEX si) {return lynxReleaseVertexShader(si); };					
			static LYNXFORCEINLINE LYNXVERTEXSHADERINDEX SetVertexShader(LYNXVERTEXSHADERINDEX si) {return lynxSetVertexShader(si); };					
			static LYNXFORCEINLINE LYNXVERTEXSHADERINDEX GetCurrentVertexShader() {return lynxGetCurrentVertexShader(); };					

			static LYNXFORCEINLINE LYNXERRORNO		LoadPixelShader(LYNXPIXELSHADERINDEX* si, const LYNXCHAR *filename, const LYNXCHAR *ver, LYNXFILE* fs, long offset) {return lynxLoadPixelShader(si, filename, ver, fs, offset); };			
			static LYNXFORCEINLINE LYNXERRORNO		LoadCompiledPixelShader(LYNXPIXELSHADERINDEX* si, LYNXCHAR *filename, LYNXFILE* fs, long offset) {return lynxLoadCompiledPixelShader(si, filename, fs, offset); };		
			static LYNXFORCEINLINE LYNXERRORNO		ReleasePixelShader(LYNXPIXELSHADERINDEX si) {return lynxReleasePixelShader(si); };					
			static LYNXFORCEINLINE LYNXPIXELSHADERINDEX SetPixelShader(LYNXPIXELSHADERINDEX si) {return lynxSetPixelShader(si); };					
			static LYNXFORCEINLINE LYNXPIXELSHADERINDEX GetCurrentPixelShader() {return lynxGetCurrentPixelShader(); };					
			
			static LYNXFORCEINLINE void				DrawStrip(LPLYNXRENDERVERTEX lpv, int x, int y) {lynxDrawStrip(lpv, x, y); };

			static LYNXFORCEINLINE void				DrawElementsBO(LYNXPRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {lynxDrawElementsBO(type, base_vert, num_vert, base_index, primitive_count); };
			static LYNXFORCEINLINE void				DrawPrimitiveBO(LPLYNXPRIMITIVE lpp, DWORD num_vert) {lynxDrawPrimitiveBO(lpp, num_vert); };			
			static LYNXFORCEINLINE void				DrawElements(LYNXPRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {lynxDrawElements(type, base_vert, num_vert, base_index, primitive_count); };
			static LYNXFORCEINLINE void				DrawPrimitive(LPLYNXPRIMITIVE lpp, DWORD num_vert) {lynxDrawPrimitive(lpp, num_vert); };
		};			
	}
}		

#endif