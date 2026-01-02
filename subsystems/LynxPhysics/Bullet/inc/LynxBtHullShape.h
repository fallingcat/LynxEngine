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

#ifndef __LYNXBTHULLSHAPE_H__
#define __LYNXBTHULLSHAPE_H__

#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CHullShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CHullShape);
		protected:
			LynxEngine::PhysicsSystem::CHullShapeDesc m_Desc;
		public:			
			CHullShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CHullShape();

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