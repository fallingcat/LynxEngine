//###########################################################################
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

#ifndef __LYNXBTCAPSULESHAPE_H__
#define __LYNXBTCAPSULESHAPE_H__

#include <btBulletCollisionCommon.h>
#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CCapsuleShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CCapsuleShape);
		protected:
			LynxEngine::PhysicsSystem::CCapsuleShapeDesc 	m_Desc;
		public:
			CCapsuleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CCapsuleShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);
			LYNXINLINE LynxEngine::PhysicsSystem::CCapsuleShapeDesc& vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.m_Radius; lpdata[1] = m_Desc.m_Length; return 2; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata);
		};	
	}
}

#endif