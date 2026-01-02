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

#ifndef __LYNXDIFFUSEMAPXALPHAPS_H__
#define __LYNXDIFFUSEMAPXALPHAPS_H__

#include <Shader/LynxDiffuseMapXColorPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDiffuseMapXAlphaPS : public CDiffuseMapXColorPS
		{	
			LYNX_DECLARE_CLASS(CDiffuseMapXAlphaPS);
		public:									
		protected:											
		public:			
			CDiffuseMapXAlphaPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDiffuseMapXAlphaPS();
			
			virtual LYNXBOOL					vCreate();						
		};		
	}		
}

#endif