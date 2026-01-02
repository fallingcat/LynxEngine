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

#ifndef __LYNXB2CIRCLESHAPE_H__
#define __LYNXB2CIRCLESHAPE_H__

#include <Box2D/Box2D.h>
#include <LynxB2PhysicsShape.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CCircleShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CCircleShape);
		protected:			
			LynxEngine::Math::CReal					m_Radius;
		public:			
			CCircleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CCircleShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			
			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Radius; return 1; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {((b2CircleShape*)m_lpShape)->m_radius = lpdata[0]; m_Radius = lpdata[0]; };
		};	
	}
}

#endif