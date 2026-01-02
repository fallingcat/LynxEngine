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

#ifndef __LYNXOGLES2PIXELSHADER_H__
#define __LYNXOGLES2PIXELSHADER_H__

#include <GraphicsSystem/LynxPixelShader.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <EGL/egl.h>
#elif __iOS__
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES2PixelShader : public CPixelShader
		{
			LYNX_DECLARE_CLASS(COGLES2PixelShader);
		protected:			
			GLuint							m_GLHandle;
			GLuint							m_Program;
			COGLES2ShaderGroup*				m_lpShaderGroup;
		public:						
			COGLES2PixelShader(CGraphicsSystem* lpsys);
			virtual ~COGLES2PixelShader();

			void							Release() {};
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_GLHandle?LYNX_TRUE:LYNX_FALSE); };
			
			LYNXFORCEINLINE const GLuint	GetGLHandle() const {return m_GLHandle; };
			
			LYNXFORCEINLINE void			SetlpShaderGroup(COGLES2ShaderGroup* lpg) {m_lpShaderGroup = lpg; };
			LYNXFORCEINLINE COGLES2ShaderGroup*	GetlpShaderGroup() {return m_lpShaderGroup; };
			LYNXFORCEINLINE void			SetProgram(GLuint p) {m_Program = p; };
						
			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL)  {return LYNX_FALSE; };			
			LYNXINLINE virtual const LYNXCHAR* vGetName() {return _T(""); };							

			LYNXINLINE void					vSetConstantF(const char *name, const float f);
			LYNXINLINE void					vSetConstantF(const char *name, const float *lpdata, int num);
			LYNXINLINE void					vSetConstantI(const char *name, const int i);
			LYNXINLINE void					vSetConstantI(const char *name, const int *lpdata, int num);
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL b);
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL *lpdata, int num);
			LYNXINLINE void					vSetConstantV2(const char *name, const float *lpdata);
			LYNXINLINE void					vSetConstantV2(const char *name, const float *lpdata, int num);
			LYNXINLINE void					vSetConstantV3(const char *name, const float *lpdata);
			LYNXINLINE void					vSetConstantV3(const char *name, const float *lpdata, int num);
			LYNXINLINE void					vSetConstantV4(const char *name, const float *lpdata);
			LYNXINLINE void					vSetConstantV4(const char *name, const float *lpdata, int num);
			LYNXINLINE void					vSetConstantM(const char *name, LPCLYNXMATRIX lpm);
			LYNXINLINE void					vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num);
			LYNXINLINE void					vSetTexture(const int channel, CTexture* t);
			void							vSetTexture(const char *name, CTexture* lpt);
			LYNXINLINE void					vSetSampler(const char *name, CSamplerState* lps);
			LYNXINLINE int					vGetTextureIndex(const char *name);

		protected:
			void							vSet();
		};
	}		
}

#endif

#endif