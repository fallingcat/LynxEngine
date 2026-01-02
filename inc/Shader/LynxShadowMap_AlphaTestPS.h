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

#ifndef __LYNXSHADOWMAPALPHATESTPS_H__
#define __LYNXSHADOWMAPALPHATESTPS_H__

#include <Shader/LynxShadowMapPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CShadowMap_AlphaTestPS : public CShadowMapPS
		{	
			LYNX_DECLARE_CLASS(CShadowMap_AlphaTestPS);
		public:						
		protected:					
		public:			
			CShadowMap_AlphaTestPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CShadowMap_AlphaTestPS();

			virtual LYNXBOOL					vCreate();
		};		
	}		
}

#endif