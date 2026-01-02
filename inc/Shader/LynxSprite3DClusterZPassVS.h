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

#ifndef __LYNXSPRITE3DCLUSTERZPASSVS_H__
#define __LYNXSPRITE3DCLUSTERZPASSVS_H__

#include <Shader/LynxSprite3DClusterVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSprite3DClusterZPassVS : public CSprite3DClusterVS
		{	
			LYNX_DECLARE_CLASS(CSprite3DClusterZPassVS);
		public:						
		protected:					
		public:			
			CSprite3DClusterZPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSprite3DClusterZPassVS();

			virtual LYNXBOOL					vCreate();		
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif