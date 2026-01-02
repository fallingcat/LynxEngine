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

#ifndef __LYNXB2BOXSHAPE_H__
#define __LYNXB2BOXSHAPE_H__

#include <Box2D/Box2D.h>
#include <LynxB2PhysicsShape.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CBoxShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CBoxShape);
		protected:			
			LynxEngine::Math::CVector2				m_Size;
		public:			
			CBoxShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CBoxShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			
			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Size.x; lpdata[1] = m_Size.y; return 2; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {((b2PolygonShape*)m_lpShape)->SetAsBox(lpdata[0], lpdata[1]); m_Size.x = lpdata[0]; m_Size.y = lpdata[1]; };
		};	
	}
}

#endif