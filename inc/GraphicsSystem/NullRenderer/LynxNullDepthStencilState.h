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

#ifndef __LYNXNULLDEPTHSTENCILSTATE_H__
#define __LYNXNULLDEPTHSTENCILSTATE_H__

#include <GraphicsSystem/LynxDepthStencilState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullDepthStencilState : public CDepthStencilState
		{
			LYNX_DECLARE_CLASS(CNullDepthStencilState);
		public:						
		protected:			
		public:						
			CNullDepthStencilState(CGraphicsSystem* pgs):CDepthStencilState(pgs) {};
			virtual ~CNullDepthStencilState() {};	

			virtual LYNXBOOL				vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc) {CDepthStencilState::vCreate(desc); return LYNX_TRUE; };
			virtual void					vSet() {};
		};
	}		
}

#endif