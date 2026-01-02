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

#ifndef __LYNXSKY_H__
#define __LYNXSKY_H__

#include <LynxRenderableObj.h>
#include <SceneSystem/StaticObj/LynxStaticObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CSky : public CStaticObj
		{
			LYNX_DECLARE_CLASS(CSky);
		protected:
			CLight*									m_SunLight;
		public:			
			CSky();
			CSky(CScene *lpscene);			
			virtual ~CSky();			

			//virtual LYNXBOOL						vCreate(void);
			virtual LYNXBOOL						vCreate(const LYNXCHAR *name, CContainer::TYPE mtype, CCreationDesc* desc);
			LYNXINLINE CLight*						GetSunLight() {return m_SunLight; };
		};
	}
}

#endif
