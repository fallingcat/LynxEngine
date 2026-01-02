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

#ifndef __LYNXVOXELCLUSTERONELIGHTINGFAKEAOLEVEL2SPECULARVS_H__
#define __LYNXVOXELCLUSTERONELIGHTINGFAKEAOLEVEL2SPECULARVS_H__

#include <Shader/LynxVoxelClusterOneLightingFakeAOSpecularVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelClusterOneLightingFakeAOLevel2SpecularVS : public CVoxelClusterOneLightingFakeAOSpecularVS
		{	
			LYNX_DECLARE_CLASS(CVoxelClusterOneLightingFakeAOLevel2SpecularVS);
		public:						
		protected:					            
		public:			
			CVoxelClusterOneLightingFakeAOLevel2SpecularVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelClusterOneLightingFakeAOLevel2SpecularVS();

			virtual LYNXBOOL					vCreate();            
		};		
	}		
}

#endif