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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXBTCOLLISIONOBJ_H__
#define __LYNXBTCOLLISIONOBJ_H__

#include <CollisionSystem/LynxCollisionObj.h>
#include <btBulletCollisionCommon.h>
#include <LynxMathWrap.h>

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CBtCollisionObj : public CCollisionObj
		{	
			LYNX_DECLARE_CLASS(CBtCollisionObj);
		public:
		protected:		
			btConvexShape*							m_Shape;
		public:			
			CBtCollisionObj();
			virtual ~CBtCollisionObj();		

			LYNXBOOL								DefaultHitTest(CCollisionObj& o);
			
			LYNXFORCEINLINE const btConvexShape*	GetShape() const { return m_Shape; }
			LYNXBOOL								vHitTest(CCollisionObj& o);
			virtual LYNXBOOL						HitTest(CBtCollisionObj& o);
			virtual void							vSetupCollisionMatrix(CMatrix4& m) {};
			virtual void							vSetupRenderMatrix(CMatrix4& m) {};
		};
	}
}

#endif