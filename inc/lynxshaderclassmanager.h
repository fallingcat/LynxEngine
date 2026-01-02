//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXSHADERCLASSMANAGER_H__
#define __LYNXSHADERCLASSMANAGER_H__

#include <LynxObj.h>
#include <LynxManager.h>
#include <LynxFactory.h>
#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CShaderClassManager : public CManager<GraphicsSystem::CShaderClass, CAnsiString>	
	{	
		LYNX_DECLARE_CLASS(CShaderClassManager);
	protected:								
	public:			
		//CResourceManager():CManager<CResource, CResource::TYPE>() {};
		//CResourceManager(CEngineCore* e):CManager<CResource, CResource::TYPE>(e) {};
		//virtual ~CResourceManager();	
		
	};		
}
#endif