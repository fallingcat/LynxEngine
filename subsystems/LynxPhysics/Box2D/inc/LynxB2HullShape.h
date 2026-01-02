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

#ifndef __LYNXNXHULLSHAPE_H__
#define __LYNXNXHULLSHAPE_H__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CHullShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CHullShape);
		protected:
			NxConvexShapeDesc						m_Desc;
		public:			
			CHullShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CHullShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {return 0; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {};  

			int										vGetNumVertices();
			int										vGetNumTriangles();
			float*									vGetVertices();
			int*									vGetTriangles();
		};	
	}
}

#endif