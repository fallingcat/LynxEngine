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

#ifndef __LYNXODESPHEREPHYSICSGEOMETRY_H__
#define __LYNXODESPHEREPHYSICSGEOMETRY_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <LynxODEPhysicsGeometry.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CSpherePhyGeo : public CPhysicsGeometry
		{	
			DECLARE_CLASS(CSpherePhyGeo);
		public:			
			CSpherePhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CSpherePhyGeo();

            virtual LYNXBOOL						vCreate(LYNXREAL *lpdata);	

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = dGeomSphereGetRadius(m_Geometry); return 1; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {dGeomSphereSetRadius(m_Geometry, lpdata[0]); };  
		};	
	}
}

#endif