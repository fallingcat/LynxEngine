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

#ifndef __LYNXSTATICOBJ_SHIFT_MATERIAL_H__
#define __LYNXSTATICOBJ_SHIFT_MATERIAL_H__

#include <SceneSystem/LynxScnMaterial.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CStaticObjShiftMaterial : public CSceneMaterial
		{	
			LYNX_DECLARE_CLASS(CStaticObjShiftMaterial);
		public:
			float							m_UShiftSpeed, m_VShiftSpeed;
		protected:
			float							m_Time;
			
		public:			
			CStaticObjShiftMaterial();
			~CStaticObjShiftMaterial();

			LYNXINLINE void					SetShiftSpeed(float us, float vs) {m_UShiftSpeed = us; m_VShiftSpeed = vs;};
			virtual void					vLoad(LPLYNXFILE lpf);
			virtual void					vPlayAll(LYNXREAL skip, LYNXPLAYMODE pm);
			virtual void					vSetParameter(void);
		};		
	}
}

#endif