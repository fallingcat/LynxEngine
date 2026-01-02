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

#ifndef __LYNXTERRAIN_H__
#define __LYNXTERRAIN_H__

#include <LynxRenderableObj.h>
#include <SceneSystem/StaticObj/LynxStaticObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CTerrain : public CStaticObj
		{
			LYNX_DECLARE_CLASS(CTerrain);
		public:			
			CTerrain(CScene *lpscene);			
			virtual ~CTerrain();						

			virtual void						vRelease(void);
			virtual LYNXBOOL					vCreate();			
		};
	}
}

#endif
