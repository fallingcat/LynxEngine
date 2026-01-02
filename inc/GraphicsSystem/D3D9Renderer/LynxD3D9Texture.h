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

#ifndef __LYNXD3D9TEXTURE_H__
#define __LYNXD3D9TEXTURE_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>	

#include <GraphicsSystem/LynxTexture.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9Texture : public CTexture
		{
			LYNX_DECLARE_CLASS(CD3D9Texture);
		protected:		
			IDirect3DBaseTexture9*			m_lpTexture;
			CBufferPtr						m_lpMSSrcBuffer;
		public:						
			CD3D9Texture(CGraphicsSystem* lpsys);
			virtual ~CD3D9Texture();				
			
			LYNXFORCEINLINE LYNXBOOL		vIsValid() {return (m_lpTexture?LYNX_TRUE:LYNX_FALSE); };			
			LYNXBOOL						vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format);
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE);
			void							vSet(int channel);
			CBuffer*						vSetAsRT(int channel);			
			LYNXFORCEINLINE int				vGetWidth() {return m_lpSrcBuffer->vGetWidth(); };
			LYNXFORCEINLINE int				vGetHeight() {return m_lpSrcBuffer->vGetHeight(); };
			LYNXFORCEINLINE int				vGetDepth() {return 0; };			
			LYNXFORCEINLINE WORD			vGetMipMapLevels() { return 0; };		
			LYNXFORCEINLINE void			vGetLevel(int face, int level);				
			LYNXFORCEINLINE void            vUpdate(const int level, const void* data) {};
		private:
			IDirect3DSurface9*				GetMipMapSurface(int face, int level);
			LYNXINLINE void					ReleaseMipMapSurface(IDirect3DSurface9* s) { LYNX_ASSERT(s); s->Release(); };

			void							LoadTGA(CStream& fs, TGAHEADER* lpheader);
			void							LoadPNG(BYTE* data, png_infop info);
			void							LoadDDS(CStream& fs, DWORD len);			

			LYNXBOOL						LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						DelayLoadTexturePNG(CStream& fs);
			LYNXBOOL						LoadTexturePNG(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureDDS(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureOthers(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);

			LYNXBOOL						DelayLoad(CStream& fs);
		};
	}		
}

#endif

#endif