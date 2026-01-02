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

#ifndef __LYNXNXHEIGHTFIELDSHAPE_H__
#define __LYNXNXHEIGHTFIELDSHAPE_H__

#ifdef __PHYSX__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CHeightFieldShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CHeightFieldShape);
		protected:
			NxHeightFieldShapeDesc 					m_Desc;
		public:			
			CHeightFieldShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CHeightFieldShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {return 0; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {};  
		};	
	}
}

#endif

#endif