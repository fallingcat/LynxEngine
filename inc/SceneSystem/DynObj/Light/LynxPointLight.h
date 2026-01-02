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

#ifndef __LYNXPOINTLIGHT_H__
#define __LYNXPOINTLIGHT_H__

#include <LynxSDK.h>
#include <SceneSystem/DynObj/Light/LynxLightDynObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class CScene;

		class LYNXENGCLASS CPointLight : public CLightDynObj
		{	
			LYNX_DECLARE_CLASS(CPointLight);
		protected:			
		public:			
			CPointLight() {};
			CPointLight(CScene *lpscene) {m_lpScene = lpscene;};
			~CPointLight() {};

		friend class CScene;
		};
	}
}

#endif