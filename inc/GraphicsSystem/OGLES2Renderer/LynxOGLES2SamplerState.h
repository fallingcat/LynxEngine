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

#ifndef __LYNXOGLES2SAMPLERSTATE_H__
#define __LYNXOGLES2SAMPLERSTATE_H__

#include <GraphicsSystem/LynxSamplerState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES2SamplerState : public CSamplerState
		{
			LYNX_DECLARE_CLASS(COGLES2SamplerState);
		public:						
		protected:			
		public:						
			COGLES2SamplerState(CGraphicsSystem* pgs);
			virtual ~COGLES2SamplerState();	

			virtual LYNXBOOL				vCreate(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual void					vSet(int channel);
		};
	}		
}

#endif

#endif