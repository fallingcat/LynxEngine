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
			CRenderer::TEXTURETYPE			m_TextureType;
			IDirect3DBaseTexture9*			m_lpTexture;
			CBufferPtr						m_lpMSSrcBuffer;
		public:						
			CD3D9Texture(CGraphicsSystem* lpsys);
			virtual ~CD3D9Texture();				
			
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpTexture?LYNX_TRUE:LYNX_FALSE); };			
			LYNXBOOL						vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format);
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE);
			void							vSet(int channel);
			CBuffer*						vSetAsRT(int channel);			
			LYNXINLINE int					vGetWidth() {return m_lpSrcBuffer->vGetWidth(); };
			LYNXINLINE int					vGetHeight() {return m_lpSrcBuffer->vGetHeight(); };
			LYNXINLINE int					vGetDepth() {return 0; };			
			LYNXINLINE WORD					vGetMipMapLevels() { return 0; };
			//LYNXINLINE LYNXTEXTURETYPE		GetType() { return m_lpInfo->Type; };
			LYNXINLINE CBuffer&				vGetSrcBuffer() {return (*m_lpSrcBuffer); };
			LYNXINLINE void					vGetLevel(int face, int level);			
			CRenderer::TEXTURETYPE			vGetType();
		private:
			IDirect3DSurface9*				GetMipMapSurface(int face, int level);
			LYNXINLINE void					ReleaseMipMapSurface(IDirect3DSurface9* s) { LYNX_ASSERT(s); s->Release(); };

			void							LoadTGA(CStream& fs, TGAHEADER* lpheader);
			void							LoadDDS(CStream& fs, DWORD len);
			LYNXBOOL						LoadTextureBMP(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureDDS(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
		};
	}		
}

#endif