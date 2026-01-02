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

#ifndef __LYNXCLOUDCONTAINER_H__
#define __LYNXCLOUDCONTAINER_H__

#include <LynxEngineCore.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CCloudContainer : PUBLIC CContainer
	{	
		LYNX_DECLARE_CLASS(CCloudContainer);
	public:		
	protected:			
		CVolumetricCloudContainer*						m_lpVolumeticCloud;
		CSprite3DContainer*								m_lpSpriteCloud;
	public:		
		CCloudContainer();
		CCloudContainer(CEngineCore *lpengine);
		virtual ~CCloudContainer();				
		virtual void									vRender(CCameraContainer* cam = NULL);				

	friend class SceneSystem::CScene;		
	};
}

#endif