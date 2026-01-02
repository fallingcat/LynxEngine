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

#ifndef __LYNXINSTANCEGROUP_H__
#define __LYNXINSTANCEGROUP_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CInstanceGroup : public CObj 
		{	
			LYNX_DECLARE_CLASS(CInstanceGroup);
		public:			
		protected:			
			CScene*										m_lpScene;
			CRenderableObj*								m_lpOriginalObj;
			CList<CRenderableObj*>						m_InstnaceList;			
		public:			
			CInstanceGroup(CScene* lps);
			virtual ~CInstanceGroup();

			LYNXINLINE void								Add(CRenderableObj* lpo) {m_InstnaceList.push_back(lpo); };			
		};
	}
}

#endif