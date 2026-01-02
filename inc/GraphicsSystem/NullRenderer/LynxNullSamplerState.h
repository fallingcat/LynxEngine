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

#ifndef __LYNXNULLSAMPLERSTATE_H__
#define __LYNXNULLSAMPLERSTATE_H__

#include <GraphicsSystem/LynxSamplerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullSamplerState : public CSamplerState
		{
			LYNX_DECLARE_CLASS(CNullSamplerState);
		public:						
		protected:			
		public:						
			CNullSamplerState(CGraphicsSystem* pgs):CSamplerState(pgs) {};
			virtual ~CNullSamplerState() {};	

			virtual LYNXBOOL				vCreate(const CRenderer::SAMPLERSTATEDESC& desc) {CSamplerState::vCreate(desc); return LYNX_TRUE; };
			virtual void					vSet(int channel) {};
		};
	}		
}

#endif