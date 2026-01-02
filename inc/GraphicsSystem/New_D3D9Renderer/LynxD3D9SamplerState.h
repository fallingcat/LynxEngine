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

#ifndef __LYNXD3D9SAMPLERSTATE_H__
#define __LYNXD3D9SAMPLERSTATE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <GraphicsSystem/LynxSamplerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CD3D9SamplerState : public CSamplerState
		{
			LYNX_DECLARE_CLASS(CD3D9SamplerState);
		public:						
		protected:						
			D3DTEXTUREFILTERTYPE			m_MinFilter, m_MagFilter, m_MipFilter;
		public:						
			CD3D9SamplerState(CGraphicsSystem* pgs);
			virtual ~CD3D9SamplerState();	

			virtual LYNXBOOL				vCreate(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual void					vSet(int channel);
		};
	}		
}

#endif