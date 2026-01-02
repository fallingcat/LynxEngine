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

#ifndef __LYNXVOXELCOLORFAKEAOLEVEL2PS_H__
#define __LYNXVOXELCOLORFAKEAOLEVEL2PS_H__

#include <Shader/LynxVoxelColorPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelColorFakeAOLevel2PS : public CVoxelColorPS
		{	
			LYNX_DECLARE_CLASS(CVoxelColorFakeAOLevel2PS);
		public:						
		protected:					
		public:			
			CVoxelColorFakeAOLevel2PS();
			CVoxelColorFakeAOLevel2PS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelColorFakeAOLevel2PS();

			virtual LYNXBOOL					vCreate();			
		};		
	}		
}

#endif