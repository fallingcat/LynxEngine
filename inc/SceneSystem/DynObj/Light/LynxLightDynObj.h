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

#ifndef __LYNXLIGHTDYNOBJ_H__
#define __LYNXLIGHTDYNOBJ_H__

#include <LynxSDK.h>
#include <SceneSystem/DynObj/LynxDynObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CLightDynObj : public CDynamicObj
		{	
			LYNX_DECLARE_CLASS(CLightDynObj);
		public:
			typedef enum{
				AMBIENT,
				DIRECTIONAL,	
				POINT,						
				SPOT,				
				NUM_TYPE,
			}TYPE;
		protected:
			TYPE						m_Type;		
			char						m_Name[64];			
		public:						
			CLightDynObj() {};
			CLightDynObj(CSystem *lpscenesys) {};			
			~CLightDynObj() {};

		friend class CScene;
		};
	}
}

#endif