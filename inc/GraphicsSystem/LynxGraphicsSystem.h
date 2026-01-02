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

#ifndef __LYNXGRAPHICSSYSTEM_H__
#define __LYNXGRAPHICSSYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>
#include <LynxVertexLayoutManager.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxMap.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <GraphicsSystem/LynxVertexShader.h>
#include <GraphicsSystem/LynxPixelShader.h>
#include <GraphicsSystem/LynxTexture.h>
#include <GraphicsSystem/LynxStreamingTexture.h>
#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxIndexArray.h>
#include <GraphicsSystem/LynxBuffer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CGraphicsSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CGraphicsSystem);
		public:			
			typedef struct {
				CRenderer::INITGRAPHICPARAM	GraphicParam;
			
				LYNXBOOL				bCompileShaderOnTheFly;
				LYNXBOOL				bAlwaysRecompileShader;
				CRenderer::SHADERTARGET	ShaderTarget;

				LYNXBOOL				bDeferredShading; 	
				LYNXBOOL				bSharedShadowMap; 	

				LYNXBOOL				bGammaCorrection;

				LYNXBOOL				bHDRRendering;
				CRenderer::FORMAT		LightingFormat;
				CRenderer::FORMAT		ToneMappingFormat;

				CRenderer::FORMAT		SSAOMapFormat;
				CRenderer::FORMAT		GBufferFormat;				
				int						MaxSplittedShadowMap;
				CRenderer::FORMAT		ShadowMapFormat;
				CRenderer::FORMAT		CubeShadowMapFormat;				
				CRenderer::FORMAT		ShadowMapDepthFormat;				
				CRenderer::FORMAT		DepthMapFormat;

				int						PostprocessQuality;
				int						SSAOQuality;
				int						RNRMapSize;
				int						ShadowMapSize;
				int						CubeShadowMapSize;

				LYNXBOOL				bCreateVoxelSpriteSource;	
				LYNXBOOL				bOverwriteVoxelSpriteRender;
			}CONFIG;			

			LYNXBOOL					m_bNeedToRecompileShader;
			CONFIG						m_Config;
			LYNXBOOL					m_bOverwriteMaterialColor;
			LYNXCOLORRGBA				m_ConstantColor;			
		protected:						
			CScreenOutputDevice*		m_lpScreenOutputDevice;
			MaterialSystem::CMaterialSystem* m_lpMaterialSystem;
			CRenderer*					m_lpRenderer;	
            LYNXRECT                    m_ViewportRect;
			LYNXPLANE					m_ClipPlanes[6];				
			
			CArray<CTexturePtr>			m_SystemTextures;			
					
			CArray<MaterialSystem::CMaterialPtr> m_PostProcessMaterails;
			MaterialSystem::CMaterialPtr m_ConsoleMaterail;

			CScreenQuad*				m_lpScreenQuad;	
			CArray<CVertexLayoutPtr>	m_VertexLayouts;					

			CMap*						m_lpMaps[NUM_MAPTYPE];
		private:
			void						InitMembers();
		protected:
			virtual void				vLoadGraphicParam(const LYNXCHAR* filename);
		public:			
			CGraphicsSystem(CEngine* const lpengine, CRenderer* ren = NULL);
			virtual ~CGraphicsSystem();		

			// Script functions ----------------------------------------------------------------------------------------------
			DECLARE_CLASSSCRIPT			

			void						Print(int x, int y, LYNXCHAR const *str)  
			{
				LYNXCOLORRGBA Color;
				Color.PackedColor = 0xffffffff;
				m_lpRenderer->GetSystemBackBuffer()->vPrintf(NULL, x, y, &Color, str); 
			}

			void						PrintWithColor(int x, int y, LYNXCHAR const *str, const LYNXCOLORRGBA& color)  
			{
				m_lpRenderer->GetSystemBackBuffer()->vPrintf(NULL, x, y, &color, str); 
			}

            LYNXINLINE LYNXRECT			GetViewportRect() const {return m_ViewportRect; };			
			LYNXINLINE void             SetViewportRect(LYNXRECT& rect) {m_ViewportRect = rect; m_lpRenderer->SetViewportRect(m_ViewportRect.x1, m_ViewportRect.y1, m_ViewportRect.x2, m_ViewportRect.y2); };
            LYNXINLINE void             SetViewportRect() {m_lpRenderer->SetViewportRect(m_ViewportRect.x1, m_ViewportRect.y1, m_ViewportRect.x2, m_ViewportRect.y2); };
			LYNXINLINE void             SetFullScreenViewport() {m_lpRenderer->SetViewportRect(0, 0, GetCurrentRenderBuffer(0)->vGetWidth(), GetCurrentRenderBuffer(0)->vGetHeight()); };
			LYNXINLINE void				GetScissorRect(LPLYNXRECT r) {m_lpRenderer->GetScissorRect(r); r->x1 -= m_ViewportRect.x1; r->x2 -= m_ViewportRect.x1; r->y1 -= m_ViewportRect.y1; r->y2 -= m_ViewportRect.y1;  }
			LYNXINLINE void				SetScissorRect(LPLYNXRECT r) {m_lpRenderer->SetScissorRect(m_ViewportRect.x1+r->x1, m_ViewportRect.y1+r->y1, m_ViewportRect.x1+r->x2, m_ViewportRect.y1+r->y2); }
            
			LYNXINLINE int				GetBackbufferWidth() {return m_lpRenderer->GetSystemBackBuffer()->vGetWidth(); };			
			LYNXINLINE int				GetBackbufferHeight() {return m_lpRenderer->GetSystemBackBuffer()->vGetHeight(); };			
			// Script functions ----------------------------------------------------------------------------------------------

			void						DefaultSetting(void);
			virtual LYNXFORCEINLINE LYNXBOOL vCreate(void) {return vCreate(CRenderer::VOID_RENDERER); };
			virtual LYNXBOOL			vCreate(CRenderer::RENDERERTYPE rt);
			virtual LYNXBOOL			vInit() {return LYNX_FALSE; };
			virtual LYNXBOOL			vInit(LYNXWINDOW wnd, CRenderer::INITGRAPHICPARAM* lpcp);
			virtual LYNXBOOL			vInit(LYNXWINDOW wnd, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);
			virtual LYNXBOOL			vInitDefalutResources();					
			void						Resize(int cx, int cy);
			virtual LYNXBOOL			vResizeMaps(int cx, int cy);
			virtual void				vLoadConfig(const LYNXCHAR* filename);			
			const CONFIG&				GetConfig() {return m_Config; };

			LYNXINLINE MaterialSystem::CMaterialSystem*		GetlpMaterialSystem() {return m_lpMaterialSystem;};				

			const LYNXCHAR*				GetAPIString() {return m_lpRenderer->GetAPIString(); };
			LYNXINLINE CRenderer*		GetlpRenderer() {return m_lpRenderer; };
			LYNXINLINE void				SetlpRenderer(CRenderer* ren) {m_lpRenderer = ren; };			

			LYNXINLINE MaterialSystem::CMaterialPtr& GetConsoleMaterial() {return m_ConsoleMaterail; };			
			LYNXINLINE MaterialSystem::CMaterialPtr& GetPostProcessMaterial(int i) {return m_PostProcessMaterails[i]; };

			LYNXINLINE CTexturePtr&		GetSystemTexture(SYSTEMTEXTURETYPE t) {return m_SystemTextures[t]; };
			
			//static CVertexLayoutPtr&	GetVertexLayout(int i) {return m_VertexLayouts[i]; };
			//static void					AddVertexLayout(int i, CVertexLayoutPtr& vl);			

			LYNXINLINE CBufferPtr&		GetSystemBackBuffer() {return m_lpRenderer->GetSystemBackBuffer(); };			
			LYNXINLINE CBufferPtr&		GetSystemDepthBuffer() {return m_lpRenderer->GetSystemDepthBuffer(); };

			LYNXINLINE void				ClearRenderBuffer(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) { return m_lpRenderer->Clear(flag, c, z, s); };			
			LYNXINLINE CBuffer*			SetRenderBuffer(int index, CBuffer* lpb) { return m_lpRenderer->SetRenderBuffer(index, lpb); };			
			LYNXINLINE CBuffer*			SetDepthBuffer(CBuffer* lpzb) { return m_lpRenderer->SetDepthBuffer(lpzb); };	
			LYNXINLINE CBuffer*			GetCurrentRenderBuffer(int index) { return m_lpRenderer->GetCachedRenderBuffer(index); };			
			LYNXINLINE CBuffer*			GetCurrentDepthBuffer() { return m_lpRenderer->GetCachedDepthBuffer(); };	
			LYNXINLINE LYNXDWORD		SetColorWriteMask(LYNXDWORD m) {return m_lpRenderer->SetColorWriteMask(m); };
			
			void						Printf(LPLYNXRECT lprect, int x, int y, LPLYNXCOLORRGBA c, LYNXCHAR const *fmt, ...)  
			{
				LYNXCHAR				Buffer[512];
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);				
				va_end(argptr);
				m_lpRenderer->GetSystemBackBuffer()->vPrintf(lprect, x, y, c, Buffer); 
			}
			void						Printf(LPLYNXRECT lprect, int x, int y, LPLYNXCOLORRGBA c, LPLYNXCOLORRGBA sc, LYNXCHAR const *fmt, ...)  
			{
				LYNXCHAR				Buffer[512];
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
				m_lpRenderer->GetSystemBackBuffer()->vPrintf(lprect, x, y, c, sc, Buffer); 
			}
			void						Printf(CBuffer* lpb, LPLYNXRECT lprect, int x, int y, LPLYNXCOLORRGBA c, LYNXCHAR const *fmt, ...)  
			{
				LYNXCHAR				Buffer[512];
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
				lpb->vPrintf(lprect, x, y, c, Buffer); 
			}
			void						Printf(CBuffer* lpb, LPLYNXRECT lprect, int x, int y, LPLYNXCOLORRGBA c, LPLYNXCOLORRGBA sc, LYNXCHAR const *fmt, ...)  
			{
				LYNXCHAR				Buffer[512];
    			va_list					argptr;

				va_start(argptr,fmt);		
				lynxVsnPrintf(Buffer, 510, fmt, argptr);
				va_end(argptr);
				lpb->vPrintf(lprect, x, y, c, sc, Buffer); 
			}

			LYNXINLINE CScreenOutputDevice* GetlpScreenOutputDevice() {return m_lpScreenOutputDevice; };

			// Render state functions
			void						ConvertScreenSpaceToScissorSpace(LYNXRECT* r, float x1, float y1, float x2, float y2);
			
			LYNXINLINE void				Clipping(LYNXBOOL b) {m_lpRenderer->Clipping(b); };
			LYNXINLINE void				SetClipPlane(int i, LPLYNXPLANE p) {m_lpRenderer->SetClipPlane(i, p); };
			void						SetClipPlane(int index, LPLYNXPLANE lpp, CCameraContainer& camera);
			void						SetClipPlane(int index, LPLYNXPLANE lpp, CMatrix4& view, CMatrix4& proj);
			LYNXINLINE void				GetClipPlane(int i, LPLYNXPLANE p) {m_lpRenderer->GetClipPlane(i, p); };
			void						PopClipPlaneStates();
			void						PushClipPlaneStates();	

			void						SetupScreenSpaceQuadUVOffset(MaterialSystem::CPass* pass, LYNXUVPOINT* lpuv = NULL);
			void						SetupScreenSpaceQuadBlurTexcoord_UVOffset(MaterialSystem::CPass* pass, LYNXUVPOINT* bt, LYNXUVPOINT* lpuv = NULL);
			void						SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(MaterialSystem::CPass* pass, CCameraContainer& cam, LYNXUVPOINT* lpuv = NULL);
			void						DrawScreenSpaceQuad();
			void						DrawScreenSpaceQuad(CTexture*t, int x, int y, int w, int h);
			void						DrawScreenSpaceQuad(float x1, float y1, float x2, float y2);						
			//void						DrawScreenSpaceDepthQuad(CCameraContainer& cam, float x1, float y1, float x2, float y2);
			LYNXINLINE void				BltTexture(CTexture* dest, CTexture* src) {m_lpRenderer->BltBuffer(dest->GetSrcBuffer(), NULL, src->GetSrcBuffer(), NULL); };	
			LYNXINLINE void				BltTexture(CBuffer* dest, CTexture* src) {m_lpRenderer->BltBuffer(dest, NULL, src->GetSrcBuffer(), NULL); };	
			LYNXINLINE void				BltTextureToBackBuffer(CTexture* src) {m_lpRenderer->BltBuffer(GetSystemBackBuffer(), NULL, src->GetSrcBuffer(), NULL); };	
			LYNXINLINE void				BltTextureToBackBuffer(CTexture* src, LYNXRECT* rect) {m_lpRenderer->BltBuffer(GetSystemBackBuffer(), rect, src->GetSrcBuffer(), NULL); };	
					
			LYNXINLINE CMap*			GetlpMap(MAPTYPE type) const {return m_lpMaps[(int)type]; };
			LYNXINLINE COffscreen*		GetlpOffscreen() {return (COffscreen*)m_lpMaps[OFFSCREEN]; };
			LYNXINLINE COffscreen*		GetlpUITransition() {return (COffscreen*)m_lpMaps[UI_TRANSITION]; };
			
			CBuffer*					DisableRenderBuffer(int index) {return m_lpRenderer->SetRenderBuffer(index, NULL); };
			void						SetMapAsRenderBuffer(CBuffer** lplastbuffers, int index, CMap* lpmap);
			void						SetMapAsRenderBuffer(CBuffer** lplastbuffers, int index, CMap* lpmap, int face);
			CBuffer*					SetTextureAsRenderBuffer(int index, CTexture* t) {return m_lpRenderer->SetRenderBuffer(index, t->GetSrcBuffer()); };
			LYNXINLINE CBuffer*			SetSystemBackBufferAsRenderBuffer(int index) {return m_lpRenderer->SetRenderBuffer(index, m_lpRenderer->GetSystemBackBuffer()); };
			LYNXINLINE CBuffer*			SetSystemDepthBufferAsDepthBuffer() {return m_lpRenderer->SetDepthBuffer(m_lpRenderer->GetSystemDepthBuffer()); };
			
			void						DownSampled(CTexturePtr& dest, CTexturePtr& src);
			void						DownSampled(CTexturePtr& dest, LPLYNXRECT rect, CTexturePtr& src);
			CTexture*					BoxBlur(CTexturePtr& t1, CTexturePtr& t2, int times = 1, float offset = 1.0f);
			CTexture*					MaxPass(CTexturePtr& t1, CTexturePtr& t2);			
			CTexture*					GaussinBlur(CTexturePtr& t1, CTexturePtr& t2, float radius, float offset = 1.0f);	
			CTexture*					BoxBlurEdgeCheck(CTexturePtr& t1, CTexturePtr& t2, int times = 7, float sharpness = 16.0f, float offset = 1.0f);
			CTexture*					RadialBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& center, const float dist = 0.5f, const float weight = 1.0f);	
			CTexture*					HaloRadialBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& center, const float halo_radius = 0.05f, const float dist = 0.5f, const float weight = 1.0f);	
			CTexture*					DirectionalBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& dir, const float dist, const float weight);
			void						DirectionalBlur(CTexturePtr& t1, CBuffer* buffer, const CVector2& dir, const float dist, const float weight);
			CTexture*					VelocityMapBlur(CTexturePtr& t1, CTexturePtr& t2, CTexturePtr& vel, const float dist, const float weight);
			CTexture*					BilateralBlur(CTexturePtr& t1, CTexturePtr& t2, float radius = 7.0f, float sharpness = 16.0f, float offset = 1.0f);
			void						DrawScanLineNoiseQuad(CTexturePtr& t1, CTexturePtr& t2, float scale);
			void						DrawGrayQuad(CTexturePtr& t1, const float time);
			void						DrawTintQuad(CTexturePtr& t1, const float time, const LYNXCOLORRGBA& color);
			void						DrawRectWireframe(LYNXRECT&rect, LYNXCOLORRGBA& color);

			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);						

		friend class CEngine;
		};	

		//LYNXINLINE CResource* CreateTerrainVertexDesc(CGraphicsSystem* g) { return LYNXNEW GraphicsSystem::CVertexDesc<TERRAINVERTEX>; };
	}
	LYNXENGCLASS CString				GetEnumStr(GraphicsSystem::VERTEXTYPE e);			
}

#endif