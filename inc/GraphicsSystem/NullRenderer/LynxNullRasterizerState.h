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

#ifndef __LYNXNULLRASTERIZERSTATE_H__
#define __LYNXNULLRASTERIZERSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRasterizerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullRasterizerState : public CRasterizerState
		{
			LYNX_DECLARE_CLASS(CNullRasterizerState);
		public:			
		protected:			
		public:						
			CNullRasterizerState(CGraphicsSystem* lpsys):CRasterizerState(lpsys) {};
			virtual ~CNullRasterizerState() {};	

			virtual LYNXBOOL						vCreate(const CRenderer::RASTERIZERSTATEDESC& desc) {CRasterizerState::vCreate(desc); return LYNX_TRUE; };
			virtual void							vSet() {};
		};			
	}		
}

#endif