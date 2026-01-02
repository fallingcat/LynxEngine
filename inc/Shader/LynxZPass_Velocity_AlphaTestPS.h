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

#ifndef __LYNXZPASSVELOCITYALPHATESTPS_H__
#define __LYNXZPASSVELOCITYALPHATESTPS_H__

#include <Shader/LynxZPass_VelocityPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CZPass_Velocity_AlphaTestPS : public CZPass_VelocityPS
		{	
			LYNX_DECLARE_CLASS(CZPass_Velocity_AlphaTestPS);
		public:						
		protected:					
		public:			
			CZPass_Velocity_AlphaTestPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CZPass_Velocity_AlphaTestPS();

			virtual LYNXBOOL					vCreate();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif