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

#ifndef __LYNXOGLESTEXTURE_H__
#define __LYNXOGLESTEXTURE_H__

#include <GraphicsSystem/LynxTexture.h>
#include <GraphicsSystem/NullRenderer/LynxNullBuffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#elif defined(__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>	
#elif defined(__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>	
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESTexture : public CTexture
		{
			LYNX_DECLARE_CLASS(COGLESTexture);
		protected:	
			GLuint							m_GLHandle;
			int								m_Width;
			int								m_Height;
			GLenum							m_PixelFormat;
		public:						
			COGLESTexture(CGraphicsSystem* lpsys);
			virtual ~COGLESTexture();
						
			LYNXFORCEINLINE LYNXBOOL		vIsValid() {return (m_GLHandle)?LYNX_TRUE:LYNX_FALSE; };
			
			LYNXFORCEINLINE const GLuint	GetGLHandle() const {return m_GLHandle; };
			
			LYNXBOOL						vLoad(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format);
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE);
			CBuffer*						vSetAsRT(int channel);		
			LYNXFORCEINLINE int				vGetWidth() {return m_Width; };
			LYNXFORCEINLINE int				vGetHeight() {return m_Height; };
			LYNXFORCEINLINE int				vGetDepth() {return 0; };
			LYNXFORCEINLINE WORD			vGetMipMapLevels() { return 0; };
			void							vGetLevel(int face, int level);	
			void							vSet(int channel);
			void                            vUpdate(const int level, const void* data);
			
		private:
			void							LoadPVR(CStream& fs, PVRHEADER* lpheader);
			void							LoadTGA(CStream& fs, TGAHEADER* lpheader);
			void							LoadDDS(CStream& fs, DWORD len);
			LYNXBOOL						LoadTexturePVR(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureBMP(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
			LYNXBOOL						LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type);
		};
	}		
}

#endif

#endif