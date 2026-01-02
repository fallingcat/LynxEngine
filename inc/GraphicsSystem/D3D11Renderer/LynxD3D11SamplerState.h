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

#ifndef __LYNXD3D11SAMPLERSTATE_H__
#define __LYNXD3D11SAMPLERSTATE_H__

#include <DXGI.h>
#include <D3Dcompiler.h>
#ifdef __WINAPP__
	#include <d3d11_1.h>
#else
	#include <d3d11.h>
	#include <d3dx11.h>
	#include <DXErr.h>
#endif
#include <GraphicsSystem/LynxSamplerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D11SamplerState : public CSamplerState
		{
			LYNX_DECLARE_CLASS(CD3D11SamplerState);
		public:						
		protected:			
			ID3D11SamplerState*				m_pSamplerState;
		public:						
			CD3D11SamplerState(CGraphicsSystem* pgs);
			virtual ~CD3D11SamplerState();	

			void							VSSet(int channel);
			void							GSSet(int channel);
			void							HSSet(int channel);
			void							DSSet(int channel);
			void							PSSet(int channel);

			virtual LYNXBOOL				vCreate(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual void					vSet(int channel);
		};
	}		
}

#endif

#endif