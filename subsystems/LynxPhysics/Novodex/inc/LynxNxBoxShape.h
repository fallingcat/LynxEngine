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

#ifndef __LYNXNXBOXSHAPE_H__
#define __LYNXNXBOXSHAPE_H__

#ifdef __PHYSX__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CBoxShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CBoxShape);
		protected:
			NxBoxShapeDesc							m_Desc;
		public:			
			CBoxShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CBoxShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.dimensions.x; lpdata[1] = m_Desc.dimensions.y; lpdata[2] = m_Desc.dimensions.z; return 3; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {m_Desc.dimensions.x = lpdata[0]; m_Desc.dimensions.y = lpdata[1]; m_Desc.dimensions.z = lpdata[2]; ((NxBoxShape*)m_lpShape)->setDimensions(lpdata); };  
		};	
	}
}

#endif

#endif