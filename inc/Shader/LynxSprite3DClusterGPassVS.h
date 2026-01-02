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

#ifndef __LYNXSPRITE3DCLUSTERGPASSVS_H__
#define __LYNXSPRITE3DCLUSTERGPASSVS_H__

#include <Shader/LynxSprite3DClusterVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSprite3DClusterGPassVS : public CSprite3DClusterVS
		{	
			LYNX_DECLARE_CLASS(CSprite3DClusterGPassVS);
		public:						
		protected:					
		public:			
			CSprite3DClusterGPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSprite3DClusterGPassVS();

			virtual LYNXBOOL					vCreate();			
		};		
	}		
}

#endif