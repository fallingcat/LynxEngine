//###########################################################################
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXODEPHYSICSGEOMETRY_H__
#define __LYNXODEPHYSICSGEOMETRY_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsGeometry.h>
#include <LynxODEPhysicsSystem.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CPhysicsGeometry : public LynxEngine::PhysicsSystem::CPhysicsGeometry
		{	
			DECLARE_CLASS(CPhysicsGeometry);
		protected:	
			dGeomID 								m_Geometry, m_TransGeometry;			
		public:			
			CPhysicsGeometry(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CPhysicsGeometry();			

			LYNXINLINE void							vDestory();
			LYNXINLINE void							SetBody(dBodyID* lpb) {(lpb) ? dGeomSetBody(m_TransGeometry, (*lpb)) : dGeomSetBody(m_TransGeometry, NULL); };
			LYNXINLINE void							vSetObj(LynxEngine::PhysicsSystem::CPhysicsObj* const lpo);// {SetBody(&(((CODEPhyObj *)lpo)->GetBody())); };
			LYNXINLINE void							vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dGeomSetPosition(m_Geometry, x, y, z); };
			LYNXINLINE void							vSetPosition(LPLYNXVECTOR3D lppos) {dGeomSetPosition(m_Geometry, lppos->s.x, lppos->s.y, lppos->s.z); };
			LYNXINLINE void							vSetTransformMatrix(const LYNXMATRIX& m) { dMatrix3 mat; MATRIX_2_PHYSICS_MAT3(mat, (m)); dGeomSetRotation(m_TransGeometry, mat); dGeomSetPosition(m_TransGeometry, m.s.m41, m.s.m42, m.s.m43); };
			LYNXINLINE void							vSetLocalTransformMatrix(const LYNXMATRIX& m) { dMatrix3 mat; MATRIX_2_PHYSICS_MAT3(mat, (m)); dGeomSetRotation(m_Geometry, mat); dGeomSetPosition(m_Geometry, m.s.m41, m.s.m42, m.s.m43); };
		};	
	}
}

#endif