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

#ifndef __LYNXSPOTLIGHT_H__
#define __LYNXSPOTLIGHT_H__

#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CSpotLight : public CLight
		{						
		public:			
		protected:						
		public:
			LYNX_DECLARE_CLASS(CSpotLight);
			CSpotLight(CScene* const lpscene);
			virtual ~CSpotLight();		

			DECLARE_CLASSSCRIPT
			virtual LYNXBOOL			vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			virtual  LYNXBOOL			vRegisterScriptClassVar(ScriptSystem::CScript* const lps);

			virtual LYNXBOOL			vCreate(int w = 512, int h = 512);
			LYNXBOOL					CreateShadowmap(int w, int h);
			virtual void				vComputeVisObjList();	
			virtual void				vGenerateShadowMap(CCameraContainer* cam);
			virtual void				vComputeProxy();
			virtual void				vSetShaderParam(CCameraContainer* cam, CContainer* lpc);
			virtual void				vDeferredLighting(CCameraContainer* cam);		
		};
	}
}

#endif