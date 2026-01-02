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

#ifndef __LYNXAMBIENTLIGHT_H__
#define __LYNXAMBIENTLIGHT_H__

#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CAmbientLight : public CLight	
		{	
			LYNX_DECLARE_CLASS(CAmbientLight);
		protected:			
		public:			
			CAmbientLight(CScene* const lpscene);
			virtual ~CAmbientLight();		

			DECLARE_CLASSSCRIPT
			virtual LYNXBOOL			vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			virtual LYNXBOOL			vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			virtual void				vSetShaderParam(CCameraContainer* cam, CContainer* lpc);
			virtual void				vDeferredLighting(CCameraContainer* cam);

			virtual void				vRelease();
		};
	}
}

#endif