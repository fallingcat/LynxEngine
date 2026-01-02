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

#ifndef __LYNXBTBOXSHAPE_H__
#define __LYNXBTBOXSHAPE_H__

#include <btBulletCollisionCommon.h>
#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CBoxShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CBoxShape);
		protected:			
			LynxEngine::PhysicsSystem::CBoxShapeDesc	m_Desc;
		public:			
			CBoxShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CBoxShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE LynxEngine::PhysicsSystem::CBoxShapeDesc& GetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.m_Extension.x; lpdata[1] = m_Desc.m_Extension.y; lpdata[2] = m_Desc.m_Extension.z; return 3; };  
			virtual void							vSetSize(LYNXREAL* lpdata);
		};	
	}
}

#endif