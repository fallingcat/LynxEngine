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

#ifndef __LYNXBTSPHERESHAPE_H__
#define __LYNXBTSPHERESHAPE_H__

#include <btBulletCollisionCommon.h>
#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CSphereShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CSphereShape);
		protected:
			LynxEngine::PhysicsSystem::CSphereShapeDesc	m_Desc;
		public:			
			CSphereShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CSphereShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE LynxEngine::PhysicsSystem::CSphereShapeDesc&	vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.m_Radius; return 1; };  
			virtual void							vSetSize(LYNXREAL* lpdata);  
		};	
	}
}

#endif