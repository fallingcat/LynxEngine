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

#ifndef __LYNXBTCOLLISIONSYSTEM_H__
#define __LYNXBTCOLLISIONSYSTEM_H__

#include <LynxSystem.h>
#include <btBulletCollisionCommon.h>
#include <CollisionSystem/LynxCollisionSystem.h>

namespace LynxEngine 
{
	namespace CollisionSystem 
	{
		class LYNXENGCLASS CBtCollisionSystem : public CCollisionSystem
		{
			LYNX_DECLARE_CLASS(CBtCollisionSystem);
		public:
			typedef enum {
				BP_SIMPLE = 0,
				BP_SAP,
				BP_DBVT,
			}BROADPHASETYPE;
		protected:
			btCollisionConfiguration*		m_CollisionConfiguration;
			btCollisionDispatcher*			m_Dispatcher;
			btBroadphaseInterface*			m_OverlappingPairCache;	
			btCollisionWorld*				m_CollisionWorld;
		private:			
		public:			
			CBtCollisionSystem(CEngine* const lpengine);
			virtual ~CBtCollisionSystem();		

			LYNXBOOL						vCreate(BROADPHASETYPE type = BP_DBVT);
			CCollisionObj*					vCreateCollisionObj(CCollisionObj::TYPE type);
		};	
		void BulletToMatrix4(LynxEngine::Math::CMatrix4& m, const btTransform& b);
		void Matrix4ToBullet(btTransform& b, const LynxEngine::Math::CMatrix4& m);
	}
}

#endif