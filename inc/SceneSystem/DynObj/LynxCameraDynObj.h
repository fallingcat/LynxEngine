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

#ifndef __LYNXCAMERADYNOBJ_H__
#define __LYNXCAMERADYNOBJ_H__

#include <LynxSDK.h>
#include <SceneSystem/LynxDynObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CCameraDynObj : public CDynamicObj, public LYNXCAMERA
		{	
			LYNX_DECLARE_CLASS(CCameraDynObj);
		protected:		
		public:			
			CCameraDynObj() {};
			~CCameraDynObj() {};

		friend class CScene;
		};
	}
}

#endif