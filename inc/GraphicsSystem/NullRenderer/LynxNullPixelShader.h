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

#ifndef __LYNXNULLPIXELSHADER_H__
#define __LYNXNULLPIXELSHADER_H__

#include <GraphicsSystem/LynxPixelShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullPixelShader : public CPixelShader
		{
			LYNX_DECLARE_CLASS(CNullPixelShader);
		protected:			
		public:						
			CNullPixelShader(CGraphicsSystem* lpsys):CPixelShader(lpsys) {};
			virtual ~CNullPixelShader() {};

			void							Release() {};
			LYNXINLINE LYNXBOOL				vIsValid() {return LYNX_TRUE; };
			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL) {return LYNX_TRUE; };
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL) {return LYNX_TRUE; };
			LYNXINLINE virtual const LYNXCHAR* vGetName() {return _T(""); };							

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
			LYNXINLINE void					vSetTexture(const int channel, CTexture* t) {};
			LYNXINLINE void					vSetTexture(const char *name, CTexture* t) {};
			LYNXINLINE void					vSetSampler(const char *name, CSamplerState* lps) {};
			LYNXINLINE int					vGetTextureIndex(const char *name) {return 0; };

		protected:
			void							vSet() {};
		};
	}		
}

#endif