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

#ifndef __LYNXSPRITEGRASSGPASSVS_H__
#define __LYNXSPRITEGRASSGPASSVS_H__

#include <Shader/LynxSpriteGrassVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSpriteGrassGPassVS : public CSpriteGrassVS
		{	
			LYNX_DECLARE_CLASS(CSpriteGrassGPassVS);
		public:			
			CSpriteGrassGPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSpriteGrassGPassVS();

			virtual LYNXBOOL					vCreate();			
		};		
	}		
}

#endif