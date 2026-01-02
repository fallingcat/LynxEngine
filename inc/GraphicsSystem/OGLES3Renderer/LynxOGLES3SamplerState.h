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

#ifndef __LYNXOGLES3SAMPLERSTATE_H__
#define __LYNXOGLES3SAMPLERSTATE_H__

#include <GraphicsSystem/LynxSamplerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES3SamplerState : public CSamplerState
		{
			LYNX_DECLARE_CLASS(COGLES3SamplerState);
		public:						
		protected:			
		public:						
			COGLES3SamplerState(CGraphicsSystem* pgs);
			virtual ~COGLES3SamplerState();	

			virtual LYNXBOOL				vCreate(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual void					vSet(int channel);
		};
	}		
}

#endif

#endif