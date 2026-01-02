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

#ifndef __LYNXLUMINANCEMAP_H__
#define __LYNXLUMINANCEMAP_H__

#include <GraphicsSystem/LynxMap.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CLuminanceMap : public CMap
		{	
			LYNX_DECLARE_CLASS(CLuminanceMap);
		public:		
		protected:										
		public:			
			CLuminanceMap();
			CLuminanceMap(CGraphicsSystem* lpgs);
			virtual ~CLuminanceMap();		

			LYNXBOOL							Create(const LYNXCHAR *name, CRenderer::FORMAT format);			
		};	
	}
}
#endif