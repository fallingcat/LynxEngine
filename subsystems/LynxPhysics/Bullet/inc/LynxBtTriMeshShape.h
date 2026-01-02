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

#ifndef __LYNXBTTRIMESHSHAPE_H__
#define __LYNXBTTRIMESHSHAPE_H__

#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CTriMeshShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CTriMeshShape);
		protected:
			LynxEngine::PhysicsSystem::CTriMeshShapeDesc m_Desc;
			btTriangleIndexVertexArray*				m_MeshInterface;
		public:			
			CTriMeshShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CTriMeshShape();

            virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			

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