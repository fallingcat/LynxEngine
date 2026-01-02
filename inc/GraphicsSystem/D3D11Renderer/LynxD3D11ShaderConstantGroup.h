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

#ifndef __LYNXD3D11SHADERCONSTANTGROUP_H__
#define __LYNXD3D11SHADERCONSTANTGROUP_H__

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
#include <GraphicsSystem/LynxShaderConstantGroup.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11ShaderConstantGroup : public CShaderConstantGroup
		{	
			LYNX_DECLARE_CLASS(CD3D11ShaderConstantGroup);
		public:						
		protected:	
			ID3D11ShaderReflection*				m_lpReflection;
			CD3D11Buffer*						m_lpConstantBuffer;
		public:			
			CD3D11ShaderConstantGroup(CShader* lps, const char* name);
			virtual ~CD3D11ShaderConstantGroup();

			void*								vMap();
			void								vUnmap();
			LYNXFORCEINLINE void				vSetUpdateFrequence(const CRenderer::CONSTANTUPDATEFREQENCE f) {m_UpdateFrequence = f; };	
		};		
	}
}

#endif

#endif