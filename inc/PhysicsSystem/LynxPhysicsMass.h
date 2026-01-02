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

#ifndef __LYNXPHYSICMASS_H__
#define __LYNXPHYSICMASS_H__

#include <LynxGameObj.h>

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class CPhysicsSpace;
		class LYNXENGCLASS CPhysicsMass : public CGameObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsGeometry);
		public:
			enum TYPE{
				NULL_GEOMETRY = 0,
				BOX,
				SPHERE,	
				CAPPEDCYLINDER,	
				CYLINDER,
				PLANE,
				TRIMESH,
				NUM_TYPE,	
			};
		protected:
			TYPE							m_Type;	
			CPhysicsSpace*					m_lpSpace;						
		public:			
			CPhysicsGeometry();
			~CPhysicsGeometry();

			virtual LYNXBOOL				vCreate(CPhysicsSpace* s, LYNXREAL *lpdata) = 0;
			virtual void					vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void					vSetPosition(LPLYNXVECTOR3D lppos) = 0;
		};	
	}
}

#endif