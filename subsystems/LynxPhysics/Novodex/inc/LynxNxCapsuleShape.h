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

#ifndef __LYNXNXCAPSULESHAPE_H__
#define __LYNXNXCAPSULESHAPE_H__

#ifdef __PHYSX__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CCapsuleShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CCapsuleShape);
		protected:
			NxCapsuleShapeDesc						m_Desc;
		public:
			CCapsuleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CCapsuleShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.radius; lpdata[1] = m_Desc.height; return 2; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {m_Desc.radius = lpdata[0]; m_Desc.height = lpdata[1]; ((NxCapsuleShape*)m_lpShape)->setDimensions(m_Desc.radius, m_Desc.height); };  
		};	
	}
}

#endif

#endif