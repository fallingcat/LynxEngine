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

#ifndef __LYNXMAP_H__
#define __LYNXMAP_H__

#include <LynxObj.h>
#include <GraphicsSystem/LynxTexture.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		/**	
		*  @brief CMap is an object which can be used as texture and render targe.
		* 
		*  CMap is the object which can be used as texture and render target.
		*	
		*/
		class LYNXENGCLASS CMap : public CObj
		{	
			LYNX_DECLARE_CLASS(CMap);
		public:					
		protected:						
			CGraphicsSystem*						m_lpGraphicsSystem;							
			CTexturePtr								m_Texture;
			CTexture*								m_Last;
			CTexture*								m_Current;
			CArray<CTexturePtr>						m_UtilTextures;
			CBufferPtr								m_lpDepthBuffer;		
			LYNXBOOL								m_bSRGB; //Do the sRGB writing or not.	
			LYNXBOOL								m_bDepthTexture;	
			CTexturePtr								m_DummyTexture;
		public:			
			CMap();
			CMap(CGraphicsSystem* lpgs);
			virtual ~CMap();		

			LYNXINLINE CBufferPtr&					GetlpDepthBuffer() {return (m_lpDepthBuffer); };			
			LYNXINLINE void							SetlpDepthBuffer(CBufferPtr& lpzb) {m_lpDepthBuffer = lpzb; };	
			LYNXINLINE CBuffer*						GetlpColorBuffer() {return (m_bDepthTexture ? m_DummyTexture->GetSrcBuffer() : m_Texture->GetSrcBuffer()); };						
			LYNXINLINE int							GetWidth() {return m_Texture->vGetWidth(); };						
			LYNXINLINE int							GetHeight() {return m_Texture->vGetHeight(); };						
			LYNXINLINE void							SetTexture(CTexturePtr& t) {m_Texture = t; };						
			//LYNXINLINE void							SetTexture(LYNXTEXTUREINDEX ti) {((CD3D9Texture*)&(*m_Texture))->SetIndex(ti); };						
			LYNXFORCEINLINE CTexturePtr&			GetDummyTexture() { return m_DummyTexture; }; 
			LYNXINLINE CTexturePtr&					GetTexture() {return m_Texture; };			
			LYNXINLINE void							GetLevel(int face, int level) {m_Texture->vGetLevel(face, level); };	
			//LYNXINLINE void							ReleaseTextureBuffer() {lynxReleaseBuffer(m_Texture->vGetSrcBuffer()); };
			LYNXINLINE const LYNXBOOL				IsSRGB() const {return m_bSRGB; };						

			LYNXBOOL								Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize = 0, int shift = 0, LYNXBOOL bSRGB = LYNX_FALSE, LYNXBOOL blockable = LYNX_FALSE);
			LYNXBOOL								CreateUtilityTexture(const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift);
			LYNXINLINE CTexturePtr&					GetUtilTexture(int i) {return m_UtilTextures[i]; };
            LYNXINLINE void                         SetUtilTextureAsCurrent(int i) {m_Current = m_UtilTextures[i]; };
			LYNXINLINE DWORD						GetNumUtilTextures() {return (DWORD)m_UtilTextures.size(); };
			LYNXINLINE LYNXBOOL						HasDownsampled() {return (m_UtilTextures.size())?LYNX_TRUE:LYNX_FALSE; };
			void									DownSampled();
			void									DownSampled(CTexturePtr& t);
			void									DownSampled(LPLYNXRECT rect);
			void									BoxBlur(int times = 1, float offset = 1.0f);
			void									MaxPass();			
			void									GaussinBlur(float radius = 5.0f, float offset = 1.0f);			
			void									BoxBlurEdgeCheck(int times = 7, float sharpness = 16.0f, float offset = 1.0f);
			void									RadialBlur(const CVector2& center, const float dist = 0.5f, const float weight = 1.0f);			
			void									HaloRadialBlur(const CVector2& center, const float halo_radius = 0.05f, const float dist = 0.5f, const float weight = 1.0f);			
			void									DirectionalBlur(const CVector2& dir, const float dist, const float weight);
			void									VelocityMapBlur(CTexturePtr& vel, const float dist, const float weight);
			void									BilateralBlur(float radius = 7.0f, float sharpness = 16.0f, float offset = 1.0f);			
			LYNXINLINE CTexturePtr&					GetLast() 
            { 
                if (m_Last == m_Texture)
                    return m_Texture;
                else if (m_Last == m_UtilTextures[0])
                    return m_UtilTextures[0];
                else
                    return m_UtilTextures[1];
            }
            LYNXINLINE CTexturePtr&					GetCurrent()
            { 
                if (m_Current == m_Texture)
                    return m_Texture;
                else if (m_Current == m_UtilTextures[0])
                    return m_UtilTextures[0];
                else
                    return m_UtilTextures[1];
            }
			LYNXINLINE void							ResetCurrent() {m_Current = m_Texture; };
			LYNXINLINE void							Switch() { CTexture* Temp = m_Current; m_Current = m_Last; m_Last = Temp; };
		};	
	}
}

#endif