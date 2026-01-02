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

#ifndef __LYNXTERRAINGPASSVS_H__
#define __LYNXTERRAINGPASSVS_H__

#include <Shader/LynxTerrainVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CTerrainGPassVS : public CTerrainVS
		{	
			LYNX_DECLARE_CLASS(CTerrainGPassVS);
		public:						
		protected:					
		public:			
			CTerrainGPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CTerrainGPassVS();

			virtual LYNXBOOL					vCreate();
		};		
	}		
}

#endif