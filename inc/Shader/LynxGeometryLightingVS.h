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

#ifndef __LYNXGEOMETRYLIGHTINGVS_H__
#define __LYNXGEOMETRYLIGHTINGVS_H__

#include <Shader/LynxGeometryVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CGeometryLightingVS : public CGeometryVS
		{	
			LYNX_DECLARE_CLASS(CGeometryLightingVS);
		public:						
		protected:					
		public:			
			CGeometryLightingVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CGeometryLightingVS();

			virtual LYNXBOOL					vCreate();		
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif