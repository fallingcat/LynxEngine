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

#ifndef __LYNXODECAPSULEPHYSICSGEOMETRY_H__
#define __LYNXODECAPSULEPHYSICSGEOMETRY_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <LynxODEPhysicsGeometry.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CCapsulePhyGeo : public CPhysicsGeometry
		{	
			DECLARE_CLASS(CCapsulePhyGeo);
		public:
			CCapsulePhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CCapsulePhyGeo();

            virtual LYNXBOOL						vCreate(LYNXREAL *lpdata);	

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {dGeomCCylinderGetParams(m_Geometry, &lpdata[0], &lpdata[1]); return 2; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {dGeomCCylinderSetParams(m_Geometry, lpdata[0], lpdata[1]); };  
		};	
	}
}

#endif