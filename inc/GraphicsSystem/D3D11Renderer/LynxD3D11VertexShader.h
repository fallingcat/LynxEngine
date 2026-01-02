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

#ifndef __LYNXD3D11VERTEXSHADER_H__
#define __LYNXD3D11VERTEXSHADER_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxVertexShader.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11VertexShader : public CVertexShader
		{
			LYNX_DECLARE_CLASS(CD3D11VertexShader);
		protected:
			static const int				MAX_BOUND_CB = 16;
			ID3D11VertexShader*				m_lpVS;		
			ID3D11ShaderReflection*			m_lpReflection;
			CArray<CD3D11Buffer*>			m_ConstantBufferArray;
			int								m_NumBoundCBs;
			ID3D11Buffer*					m_D3DBufferArray[MAX_BOUND_CB];
		public:						
			CD3D11VertexShader(CGraphicsSystem* lpsys);
			virtual ~CD3D11VertexShader();

			void							Release();
			LYNXINLINE LYNXBOOL				vIsValid() {return (m_lpVS?LYNX_TRUE:LYNX_FALSE); };			
			LYNXBOOL						vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);
			LYNXBOOL						vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL);			
			
			LYNXFORCEINLINE ID3D11ShaderReflection* GetlpShaderReflection() {return m_lpReflection; };
			LYNXFORCEINLINE CD3D11Buffer*	GetConstantBuffer(int i) {return m_ConstantBufferArray[i]; };
			CD3D11Buffer*					GetConstantBuffer(const CAnsiString& name);

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
			LYNXINLINE void					vSetTexture(const char *name, CTexture* lpt);
			LYNXINLINE void					vSetSampler(const char *name, CSamplerState* lps);
			LYNXINLINE int					vGetTextureIndex(const char *name);

		protected:
			void							vSet();
			void							CreateConstantBuffers();
			LYNXBOOL						LoadCompiledShader(CStream& fs, const LYNXCHAR *filename);
			LYNXBOOL						LoadCachedShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug);
		};
	}		
}

#endif

#endif