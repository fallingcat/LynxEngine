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

#ifndef __LYNXBTPHYSICSSHAPE_H__
#define __LYNXBTPHYSICSSHAPE_H__

#include <btBulletCollisionCommon.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CPhysicsShape : public LynxEngine::PhysicsSystem::CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CPhysicsShape);
		protected:	
			LynxEngine::Math::CMatrix4				m_LocalXTransformMatrix;
			btCollisionShape* 						m_lpShape;			
		protected:
			void									RecreatePhysicsObj();
		public:			
			CPhysicsShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CPhysicsShape();

			LYNXFORCEINLINE btCollisionShape*		GetbtShape() const {return m_lpShape; };	 
			LYNXFORCEINLINE void					SetbtShape(btCollisionShape* lps) {m_lpShape = lps; };	 
			
			void									Resize();

			void									vDestory();
			void									CreateStaticPhyObj();
			void									DestoryPhyObj();

			LYNXINLINE void							vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			LYNXINLINE void							vSetPosition(LPLYNXVECTOR3D lppos) {};
			void									vSetTransformMatrix(const CMatrix4& m);
			CMatrix4&								vGetTransformMatrix();
			void									vSetLocalTransformMatrix(const CMatrix4& m);			
			CMatrix4&								vGetLocalTransformMatrix();			
		};	
	}
}

#endif