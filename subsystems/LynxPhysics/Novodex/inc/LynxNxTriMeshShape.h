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

#ifndef __LYNXNXTRIMESHSHAPE_H__
#define __LYNXNXTRIMESHSHAPE_H__

#ifdef __PHYSX__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CTriMeshShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CTriMeshShape);
		protected:
			NxTriangleMeshShapeDesc					m_Desc;			
		public:			
			CTriMeshShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CTriMeshShape();

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

#endif