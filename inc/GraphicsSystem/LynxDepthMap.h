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

#ifndef __LYNXDEPTHMAP_H__
#define __LYNXDEPTHMAP_H__

#include <GraphicsSystem/LynxMap.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CDepthMap : public CMap
		{	
		public:		
		protected:							

		public:
			LYNX_DECLARE_CLASS(CDepthMap);
			CDepthMap();
			CDepthMap(CGraphicsSystem* lpgs);
			virtual ~CDepthMap();		

			LYNXBOOL							Create(const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize = 2, int shift = 1);
		};	
	}
}
#endif