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

#ifndef __LYNXNXSPHERESHAPE_H__
#define __LYNXNXSPHERESHAPE_H__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CSphereShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CSphereShape);
		protected:
			NxSphereShapeDesc						m_Desc;
		public:			
			CSphereShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CSphereShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.radius; return 1; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {m_Desc.radius = lpdata[0]; ((NxSphereShape*)m_lpShape)->setRadius(m_Desc.radius); };  
		};	
	}
}

#endif