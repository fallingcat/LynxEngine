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

#ifndef __LYNXD3D9PIXELSHADER_H__
#define __LYNXD3D9PIXELSHADER_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxPixelShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9PixelShader : public CPixelShader
		{
			LYNX_DECLARE_CLASS(CD3D9PixelShader);
		protected:
			IDirect3DPixelShader9*			m_lpPS;
			ID3DXConstantTable*				m_lpConstantTable;
		public:						
			CD3D9PixelShader(CGraphicsSystem* lpsys);
			virtual ~CD3D9PixelShader();

			LYNXINLINE CD3D9PixelShader&	operator =(const CD3D9PixelShader& rhs) {m_lpGraphicsSystem = rhs.m_lpGraphicsSystem; return (*this); };	
			
			void							Release();
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpPS?LYNX_TRUE:LYNX_FALSE); };
			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);			

			LYNXFORCEINLINE ID3DXConstantTable* GetlpConstantTable() {return m_lpConstantTable; };

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

			void							vSetTexture(const int channel, CTexture* t);
			void							vSetTexture(const char *name, CTexture* t);
			void							vSetSampler(const char *name, CSamplerState* lps);
			int								vGetTextureIndex(const char *name);

		protected:
			void							vSet();
			LYNXBOOL						LoadCompiledShader(CStream& fs, const LYNXCHAR *filename);
			LYNXBOOL						CompileShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug);
		};
	}		
}

#endif

#endif