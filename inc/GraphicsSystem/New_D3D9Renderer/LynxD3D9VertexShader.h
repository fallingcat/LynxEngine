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

#ifndef __LYNXD3D9VERTEXSHADER_H__
#define __LYNXD3D9VERTEXSHADER_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxVertexShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9VertexShader : public CVertexShader
		{
			LYNX_DECLARE_CLASS(CD3D9VertexShader);
		protected:			
			IDirect3DVertexShader9*			m_lpVS;
			ID3DXConstantTable*				m_lpConstantTable;
		public:						
			CD3D9VertexShader(CGraphicsSystem* lpsys);
			virtual ~CD3D9VertexShader();											

			LYNXINLINE CD3D9VertexShader&	operator =(const CD3D9VertexShader& rhs) {m_lpGraphicsSystem = rhs.m_lpGraphicsSystem; return (*this); };	
			
			void							Release();
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpVS?LYNX_TRUE:LYNX_FALSE); };			
			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CRenderer::SHADERMACRO* def = NULL, LPLYNXFILE lpfs = NULL, long offset = 0);
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CRenderer::SHADERMACRO* def, LPLYNXFILE lpfs = NULL, long offset = 0);			

			LYNXINLINE void					vSetConstantF(const char *name, const float f);
			LYNXINLINE void					vSetConstantF(const char *name, const float *lpdata, int num);
			LYNXINLINE void					vSetConstantI(const char *name, const int i);
			LYNXINLINE void					vSetConstantI(const char *name, const int *lpdata, int num);
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL b);
			LYNXINLINE void					vSetConstantB(const char *name, const BOOL *lpdata, int num);
			LYNXINLINE void					vSetConstantM(const char *name, LPCLYNXMATRIX lpm);
			void							vSetTexture(const char *name, CTexture* lpt);
			void							vSetSampler(const char *name, CSamplerState* lps);
			LYNXINLINE int					vGetTextureIndex(const char *name);

		protected:
			void							vSet();			
			LYNXBOOL						LoadCompiledShader(CStream& fs, const LYNXCHAR *filename);
			LYNXBOOL						LoadCachedShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CRenderer::SHADERMACRO* def, LYNXBOOL brecompile, LYNXBOOL bdebug);
		};
	}		
}

#endif