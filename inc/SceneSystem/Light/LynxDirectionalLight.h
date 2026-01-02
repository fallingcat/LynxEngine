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

#ifndef __LYNXDIRECTIONALLIGHT_H__
#define __LYNXDIRECTIONALLIGHT_H__

#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CDirectionalLight : public CLight
		{						
		public:
			Math::CMatrix4				m_SplittedLightViewProj[4];
		protected:									
		public:
			LYNX_DECLARE_CLASS(CDirectionalLight);
			CDirectionalLight(CScene* const lpscene);
			virtual ~CDirectionalLight();		

			virtual void				vRelease();
			virtual LYNXBOOL			vCreate(int w = 512, int h = 512);
			LYNXBOOL					CreateShadowmap(int w, int h);
			virtual void				vComputeVisObjList();
			virtual void				vGenerateShadowMap(CCameraContainer* cam);
			virtual void				vGenerateShadowTexture();
			virtual void				vSetShaderParam(CCameraContainer* cam, CContainer* lpc);
			virtual void				vDeferredLighting(CCameraContainer* cam);
		};
	}
}

#endif