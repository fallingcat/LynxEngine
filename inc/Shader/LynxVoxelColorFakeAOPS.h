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

#ifndef __LYNXVOXELCOLORFAKEAOPS_H__
#define __LYNXVOXELCOLORFAKEAOPS_H__

#include <Shader/LynxVoxelColorPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CVoxelColorFakeAOPS : public CVoxelColorPS
		{	
			LYNX_DECLARE_CLASS(CVoxelColorFakeAOPS);
		public:						
		protected:					
		public:			
			CVoxelColorFakeAOPS();
			CVoxelColorFakeAOPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CVoxelColorFakeAOPS();

			virtual LYNXBOOL					vCreate();			
		};		
	}		
}

#endif