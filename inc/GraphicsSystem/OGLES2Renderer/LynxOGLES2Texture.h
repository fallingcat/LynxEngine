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

#ifndef __LYNXOGLES2TEXTURE_H__
#define __LYNXOGLES2TEXTURE_H__

#include <GraphicsSystem/LynxTexture.h>
#include <GraphicsSystem/NullRenderer/LynxNullBuffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <EGL/egl.h>
#elif __iOS__
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES2Texture : public CTexture
		{
			LYNX_DECLARE_CLASS(COGLES2Texture);
		protected:	
			GLuint							m_GLHandle;
			int								m_Width;
			int								m_Height;
			GLenum							m_PixelFormat;
		public:						
			COGLES2Texture(CGraphicsSystem* lpsys);
			virtual ~COGLES2Texture();
						
			LYNXFORCEINLINE LYNXBOOL		vIsValid() {return (m_GLHandle)?LYNX_TRUE:LYNX_FALSE; };
			
			LYNXFORCEINLINE const GLuint	GetGLHandle() const {return m_GLHandle; };
			
			LYNXBOOL						vLoad(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL						Create(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, GLuint handle);
			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format);
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE);
            void                            vUpdate(const int level, const void* data);
			CBuffer*						vSetAsRT(int channel);		
			LYNXFORCEINLINE int				vGetWidth() {return m_Width; };
			LYNXFORCEINLINE int				vGetHeight() {return m_Height; };
			LYNXFORCEINLINE int				vGetDepth() {return 0; };
			LYNXFORCEINLINE WORD			vGetMipMapLevels() { return 0; };
			void							vGetLevel(int face, int level);	
			void							vSet(int channel);
			
		private:
			void							LoadPVR(CStream& fs, PVRHEADER* lpheader);
			void							LoadPNG(BYTE* data, png_infop info);
			void							LoadTGA(CStream& fs, TGAHEADER* lpheader);
			void							LoadDDS(CStream& fs, DWORD len);
			LYNXBOOL						LoadTexturePVR(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureKTX(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTexturePNG(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						DelayLoadTexturePNG(CStream& fs);
			LYNXBOOL						LoadTextureBMP(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
		};
	}		
}

#endif

#endif