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

#ifndef __LYNXOGLES2DEPTHSTENCILSTATE_H__
#define __LYNXOGLES2DEPTHSTENCILSTATE_H__

#include <GraphicsSystem/LynxDepthStencilState.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES2DepthStencilState : public CDepthStencilState
		{
			LYNX_DECLARE_CLASS(COGLES2DepthStencilState);
		public:						
		protected:			
		public:						
			COGLES2DepthStencilState(CGraphicsSystem* pgs);
			virtual ~COGLES2DepthStencilState();

			virtual LYNXBOOL				vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc);
			virtual void					vSet();
		};
	}		
}

#endif

#endif