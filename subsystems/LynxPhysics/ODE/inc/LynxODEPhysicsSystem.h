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

#ifndef __LYNXODEPHYSICSSYSTEM_H__
#define __LYNXODEPHYSICSSYSTEM_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>

#ifdef __LYNXPHYSICS_LIB__
	#define	LYNXPHYSICSCLASS						
#elif __LYNXPHYSICS_DLL__
	#define	LYNXPHYSICSCLASS						LYNXEXPORT
#else
	#define LYNXPHYSICSCLASS						LYNXIMPORT
#endif	

#ifdef __LYNXPHYSICS_LIB__
	#define	LYNXPHYSICSAPI						
#elif __LYNXPHYSICS_DLL__
	#define	LYNXPHYSICSAPI						LYNXEXPORT
#else
	#define LYNXPHYSICSAPI						LYNXIMPORT
#endif	


namespace LynxPhysics 
{
	namespace ODE 
	{
		#define	PHYSICS_MAT3_2_MATRIX(mat, mat3)			mat.s.m11 = mat3[0];	\
															mat.s.m12 = mat3[4];	\
															mat.s.m13 = mat3[8];	\
															mat.s.m21 = mat3[1];	\
															mat.s.m22 = mat3[5];	\
															mat.s.m23 = mat3[9];	\
															mat.s.m31 = mat3[2];	\
															mat.s.m32 = mat3[6];	\
															mat.s.m33 = mat3[10];	
																
		#define	MATRIX_2_PHYSICS_MAT3(mat3, mat)			mat3[0] = mat.s.m11;	\
															mat3[4] = mat.s.m12; 	\
															mat3[8] = mat.s.m13;	\
															mat3[1] = mat.s.m21;	\
															mat3[5] = mat.s.m22;	\
															mat3[9] = mat.s.m23;	\
															mat3[2] = mat.s.m31;	\
															mat3[6] = mat.s.m32;	\
															mat3[10] = mat.s.m33;		

		class CPhysicsSystem : public LynxEngine::PhysicsSystem::CPhysicsSystem
		{	
			DECLARE_CLASS(CPhysicsSystem);
		protected:						
		public:			
			CPhysicsSystem();
			virtual ~CPhysicsSystem();

			LYNXINLINE const char*							vGetAPIString() {return "ODE"; };
			LYNXBOOL										vInit() {return LYNX_TRUE; };
			LynxEngine::PhysicsSystem::CPhysicsWorld* const	vCreateWorld();
		};	
	}
}

#endif