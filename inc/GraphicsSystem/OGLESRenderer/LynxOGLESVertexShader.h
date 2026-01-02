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

#ifndef __LYNXOGLESVERTEXSHADER_H__
#define __LYNXOGLESVERTEXSHADER_H__

#include <GraphicsSystem/LynxVertexShader.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESVertexShader : public CVertexShader
		{
			LYNX_DECLARE_CLASS(COGLESVertexShader);
		protected:
			GLuint							m_GLHandle;
		public:						
			COGLESVertexShader(CGraphicsSystem* lpsys);
			virtual ~COGLESVertexShader();

			void							Release();
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_GLHandle?LYNX_TRUE:LYNX_FALSE); };

			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);			

			LYNXINLINE void					vSetConstantF(const char *name, const float f) {};
			LYNXINLINE void					vSetConstantF(const char *name, const float *lpdata, int num) {};
			LYNXINLINE void					vSetConstantI(const char *name, const int i) {};
			LYNXINLINE void					vSetConstantI(const char *name, const int *lpdata, int num) {};
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL b) {};
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL *lpdata, int num) {};
			LYNXINLINE void					vSetConstantV2(const char *name, const float *lpdata) {};
			LYNXINLINE void					vSetConstantV2(const char *name, const float *lpdata, int num) {};
			LYNXINLINE void					vSetConstantV3(const char *name, const float *lpdata) {};
			LYNXINLINE void					vSetConstantV3(const char *name, const float *lpdata, int num) {};
			LYNXINLINE void					vSetConstantV4(const char *name, const float *lpdata) {};
			LYNXINLINE void					vSetConstantV4(const char *name, const float *lpdata, int num) {};
			LYNXINLINE void					vSetConstantM(const char *name, LPCLYNXMATRIX lpm) {};
			LYNXINLINE void					vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) {};
			LYNXINLINE void					vSetTexture(const char *name, CTexture* lpt);
			LYNXINLINE void					vSetTexture(const int channel, CTexture* t) {};
			LYNXINLINE void					vSetSampler(const char *name, CSamplerState* lps);
			LYNXINLINE int					vGetTextureIndex(const char *name);

		protected:
			void							vSet();
			LYNXBOOL						LoadCompiledShader(CStream& fs, const LYNXCHAR *filename);
			LYNXBOOL						LoadCachedShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug);
		};
	}		
}

#endif

#endif