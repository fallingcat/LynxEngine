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

#ifndef __LYNXSHADOWMAP_H__
#define __LYNXSHADOWMAP_H__

#include <GraphicsSystem/LynxMap.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CShadowMap : public CMap
		{	
			LYNX_DECLARE_CLASS(CShadowMap);
		public:					
			typedef enum {
				SHADOWMAP,
				CUBE_SHADOWMAP,
			}TYPE;

			typedef enum{
				PCF = 0,
				PCF_1X1 = PCF,
				PCF_2X2,
				PCF_3X3,
				PCF_4X4,
				VSM,
				VSM_1X1 = VSM,					
				VSM_2X2,					
				VSM_3X3,					
				VSM_4X4,					
				NUM_SHADOWFILTER_TYPES,
			}SHADOWFILTERTYPE;
			
		protected:					
			
		public:			
			CShadowMap();
			CShadowMap(CGraphicsSystem* lpgs);
			virtual ~CShadowMap();		
			
			LYNXBOOL								Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize = 0, int shift = 0);
		};	
	}
}
#endif