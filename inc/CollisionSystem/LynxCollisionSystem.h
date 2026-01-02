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

#ifndef __LYNXCOLLISIONSYSTEM_H__
#define __LYNXCOLLISIONSYSTEM_H__

#include <LynxSystem.h>
#include <CollisionSystem/LynxCollisionObj.h>

namespace LynxEngine 
{
	namespace CollisionSystem 
	{
		class LYNXENGCLASS CCollisionSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CCollisionSystem);		
		protected:
			CList<CCollisionObj*>			m_CollisionObjList;
			float							m_HitTestOffset;
		private:			
		public:			
			CCollisionSystem(CEngine* const lpengine);
			virtual ~CCollisionSystem();		

			LYNXFORCEINLINE	void			SetHitTestOffset(float offset) {m_HitTestOffset = offset; };
			LYNXFORCEINLINE	float			GetHitTestOffset() {return m_HitTestOffset; };
			
			LYNXBOOL						vCreate(void);
			virtual CCollisionObj*			vCreateCollisionObj(CCollisionObj::TYPE type);
		};	
	}
}

#endif