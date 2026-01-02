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

#ifndef __LYNXNXPHYSICSSHAPE_H__
#define __LYNXNXPHYSICSSHAPE_H__

#ifdef __PHYSX__

#include <NxPhysics.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CPhysicsShape : public LynxEngine::PhysicsSystem::CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CPhysicsShape);
		protected:	
			NxShape* 								m_lpShape;
			//dBodyID*								m_lpBody;
		public:			
			CPhysicsShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CPhysicsShape();

			LYNXINLINE virtual NxShapeDesc&			vGetDesc() = 0;
			LYNXINLINE NxShape*						GetlpNxShape() const {return m_lpShape; };	 
			LYNXINLINE void							SetlpNxShape(NxShape*  lps) {m_lpShape = lps; };	 			
			LYNXINLINE void							vDestory();

			void									CreateStaticPhyObj();

			void									DestoryPhyObj();

			LYNXINLINE void							vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_lpShape->setGlobalPosition(NxVec3(x, y, z)); };
			LYNXINLINE void							vSetPosition(LPLYNXVECTOR3D lppos) {m_lpShape->setGlobalPosition(NxVec3(lppos->x, lppos->y, lppos->z)); };
			void									vSetTransformMatrix(const CMatrix4& m);
			CMatrix4&								vGetTransformMatrix();
			void									vSetLocalTransformMatrix(const CMatrix4& m);			
			CMatrix4&								vGetLocalTransformMatrix();			
		};	
	}
}

#endif

#endif