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

#ifndef __LYNXRIGIDMODELGPASSVS_H__
#define __LYNXRIGIDMODELGPASSVS_H__

#include <Shader/LynxRigidModelVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CRigidModelGPassVS : public CRigidModelVS
		{	
			LYNX_DECLARE_CLASS(CRigidModelGPassVS);
		public:						
		protected:					
		public:			
			CRigidModelGPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CRigidModelGPassVS();

			virtual LYNXBOOL					vCreate();			
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif