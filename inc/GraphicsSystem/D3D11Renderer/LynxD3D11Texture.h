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

#ifndef __LYNXD3D11TEXTURE_H__
#define __LYNXD3D11TEXTURE_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxTexture.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Buffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11Texture : public CTexture
		{
			LYNX_DECLARE_CLASS(CD3D11Texture);
		protected:			
			LYNXBOOL						LoadTGA(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc);
		public:						
			CD3D11Texture(CGraphicsSystem* lpsys);
			virtual ~CD3D11Texture();	
						
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpSrcBuffer) ? LYNX_TRUE : LYNX_FALSE; };
			LYNXBOOL						Create(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, UINT bindflag, D3D11_SUBRESOURCE_DATA* data = NULL);
			LYNXBOOL						vLoad(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format) {SetName(name); return LYNX_TRUE; };
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE);
			void							vSet(int channel);
			CBuffer*						vSetAsRT(int channel);
			LYNXFORCEINLINE int				vGetWidth() {return m_lpSrcBuffer->vGetWidth(); };
			LYNXFORCEINLINE int				vGetHeight() {return m_lpSrcBuffer->vGetHeight(); };
			LYNXFORCEINLINE int				vGetDepth() {return 0; };
			LYNXFORCEINLINE WORD			vGetMipMapLevels() { return 0; };
			//LYNXINLINE LYNXTEXTURETYPE	GetType() { return m_lpInfo->Type; };
			LYNXFORCEINLINE void			vGetLevel(int face, int level) { };
			LYNXFORCEINLINE void            vUpdate(const int level, const void* data) {};
		};
	}		
}

#endif

#endif