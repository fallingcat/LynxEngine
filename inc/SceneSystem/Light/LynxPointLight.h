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

#ifndef __LYNXPOINTLIGHT_H__
#define __LYNXPOINTLIGHT_H__

#include <GraphicsSystem/LynxShadowmap.h>
#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CPointLight : public CLight
		{						
		public:			
		protected:						
		public:
			LYNX_DECLARE_CLASS(CPointLight);
			CPointLight(CScene* const lpscene);
			virtual ~CPointLight();		
			
			virtual LYNXBOOL			vCreate(int w = 512, int h = 512);	
			LYNXBOOL					CreateShadowmap(int w, int h);	
			virtual void				vComputeProxy();
			virtual void				vComputeVisObjList();	
			virtual void				vGenerateShadowMap(CCameraContainer* cam);
			virtual void				vSetShaderParam(CCameraContainer* cam, CContainer* lpc);
			virtual void				vDeferredLighting(CCameraContainer* cam);

			DECLARE_CLASSSCRIPT
		};
	}
}

#endif