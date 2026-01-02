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
//  Created by Owen Wu : 2006/11/16
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANIMATIONTREE_H__
#define __LYNXANIMATIONTREE_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace Animation
	{
		//class CAnimation;
		class LYNXENGCLASS CAnimationTree : public CObj
		{	
			LYNX_DECLARE_CLASS(CAnimationTree);
		public:	
		protected:													
		public:			
			CAnimationTree();			
			virtual ~CAnimationTree();	

			virtual void						vInstance(CAnimationTree& rhs);
		};
	}
}
#endif

