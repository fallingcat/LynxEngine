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

#ifndef __LYNXNULLBLENDSTATE_H__
#define __LYNXNULLBLENDSTATE_H__

#include <GraphicsSystem/LynxBlendState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullBlendState : public CBlendState
		{
			LYNX_DECLARE_CLASS(CNullBlendState);
		public:						
		protected:			
		public:						
			CNullBlendState(CGraphicsSystem* pgs):CBlendState(pgs) {};
			virtual ~CNullBlendState() {};	

			virtual LYNXBOOL				vCreate(const CRenderer::BLENDSTATEDESC& desc) {CBlendState::vCreate(desc); return LYNX_TRUE; };
			virtual void					vSet() {};
		};
	}		
}

#endif