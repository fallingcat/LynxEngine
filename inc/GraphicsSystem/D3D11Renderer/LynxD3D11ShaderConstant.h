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

#ifndef __LYNXD3D11SHADERCONSTANT_H__
#define __LYNXD3D11SHADERCONSTANT_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11ShaderConstant : public CShaderConstant
		{	
			LYNX_DECLARE_CLASS(CD3D11ShaderConstant);
		public:						
		protected:	
			D3D11_SHADER_VARIABLE_DESC			m_D3DShaderVarDesc;
			D3D11_SHADER_INPUT_BIND_DESC 		m_D3DShaderInputDesc;
			ID3D11ShaderReflection*				m_lpReflection;
			CD3D11Buffer*						m_lpConstantBuffer;
		public:			
			CD3D11ShaderConstant(CShader* lps, const char* name);
			virtual ~CD3D11ShaderConstant();

			void*								vMap();
			void								vUnmap();

			LYNXFORCEINLINE LYNXBOOL			vIsValid() {return (m_lpConstantBuffer ? LYNX_TRUE : LYNX_FALSE); };

			void								vSetFloat(const float f);
			void								vSetFloat(const float *lpdata, int num);
			void								vSetInt(const int i);
			void								vSetInt(const int *lpdata, int num);
			void								vSetInt2(const int* i);
			void								vSetInt2(const int *lpdata, int num);
			void								vSetInt3(const int* i);
			void								vSetInt3(const int *lpdata, int num);
			void								vSetInt4(const int* i);
			void								vSetInt4(const int *lpdata, int num);
			void								vSetBool(const BOOL b);
			void								vSetBool(const BOOL *lpdata, int num);
			void								vSetVector2(const float* lpdata);
			void								vSetVector2(const float *lpdata, int num);
			void								vSetVector3(const float* lpdata);
			void								vSetVector3(const float *lpdata, int num);
			void								vSetVector4(const float* lpdata);
			void								vSetVector4(const float *lpdata, int num);
			void								vSetMatrix(LPCLYNXMATRIX lpm);
			void								vSetMatrix(LPCLYNXMATRIX lpm, int num);
		};		
	}
}

#endif

#endif