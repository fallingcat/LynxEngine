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

#ifndef __LYNXB2PHYSICSSHAPE_H__
#define __LYNXB2PHYSICSSHAPE_H__

#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsShape.h>
#include <Box2D/Box2D.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CPhysicsShape : public LynxEngine::PhysicsSystem::CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CPhysicsShape);
		protected:	
			b2FixtureDef							m_FixtureDef;
			b2Shape* 								m_lpShape;		
		public:			
			CPhysicsShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CPhysicsShape();

			LYNXINLINE b2Shape*						Getb2Shape() const {return m_lpShape; };	 
			LYNXINLINE void							Setb2Shape(b2Shape* lps) {m_lpShape = lps; };	
			
			LYNXINLINE const b2FixtureDef&			Getb2FixtureDef() const {return m_FixtureDef; };	 
			
			LYNXINLINE void							vDestory();

			void									CreateStaticPhyObj();

			void									DestoryPhyObj();

			LYNXINLINE void							vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			LYNXINLINE void							vSetPosition(LPLYNXVECTOR3D lppos) { };
			void									vSetTransformMatrix(const CMatrix4& m);
			CMatrix4&								vGetTransformMatrix();
			void									vSetLocalTransformMatrix(const CMatrix4& m);			
			CMatrix4&								vGetLocalTransformMatrix();			
		};	
	}
}

#endif