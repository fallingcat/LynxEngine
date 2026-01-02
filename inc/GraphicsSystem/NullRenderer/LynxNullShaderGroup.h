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

#ifndef __LYNXNULLSHADERGROUP_H__
#define __LYNXNULLSHADERGROUP_H__

#include <LynxResource.h>
#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderGroup.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullShaderGroup : public CShaderGroup
		{	
			LYNX_DECLARE_CLASS(CNullShaderGroup);
		public:						
		protected:			
		public:			
			CNullShaderGroup(CGraphicsSystem* lpg):CShaderGroup(lpg) {};
			virtual ~CNullShaderGroup() {};
						
			LYNXBOOL							vCreate() {return LYNX_TRUE; }
			void								vSet(CContainer* lpo, CCameraContainer* cam) {}
		};		
	}	
}
#endif