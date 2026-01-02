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

#ifndef __LYNXODEBOXPHYSICSGEOMETRY_H__
#define __LYNXODEBOXPHYSICSGEOMETRY_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsGeometry.h>
#include <LynxODEPhysicsGeometry.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CBoxPhyGeo : public CPhysicsGeometry
		{	
			DECLARE_CLASS(CBoxPhyGeo);
		public:			
			CBoxPhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CBoxPhyGeo();

            virtual LYNXBOOL						vCreate(LYNXREAL *lpdata);		

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {dVector3 s; dGeomBoxGetLengths(m_Geometry, s); lpdata[0] = s[0]; lpdata[1] = s[1]; lpdata[2] = s[2]; return 3; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {dGeomBoxSetLengths(m_Geometry, lpdata[0], lpdata[1], lpdata[2]); };  
		};	
	}
}

#endif