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

#ifndef __LYNXOCEANMATERIAL_H__
#define __LYNXOCEANMATERIAL_H__

#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>
#include <AdvMaterialSystem/LynxAdvMaterial.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS COceanMaterial : public AMS::CAdvMaterial
		{	
			LYNX_DECLARE_CLASS(COceanMaterial);
		public:
			float							m_Time;
			float							m_WaveSpeed;
			BOOL							m_bUnderWater;
		public:			
			COceanMaterial();
			~COceanMaterial();

			virtual void					vInit(void);
			LYNXINLINE						SetUnderWater(BOOL b) {m_bUnderWater = b;};
			virtual void					vPlayAll(LYNXREAL skip, LYNXPLAYMODE pm);
			virtual void					vSetParameter(void);
		};		
	}
}

#endif