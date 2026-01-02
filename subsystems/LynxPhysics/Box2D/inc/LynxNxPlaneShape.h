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

#ifndef __LYNXNXPLANESHAPE_H__
#define __LYNXNXPLANESHAPE_H__

#include <NxPhysics.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CPlaneShape : public CPhysicsShape
		{	
			LYNX_DECLARE_CLASS(CPlaneShape);
		protected:
			NxPlaneShapeDesc							m_Desc;
		public:			
			CPlaneShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps);
			virtual ~CPlaneShape();

			virtual LYNXBOOL						vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);			
			LYNXINLINE NxShapeDesc&					vGetDesc() {return m_Desc; };

			LYNXINLINE virtual int					vGetSize(LYNXREAL* lpdata) const {lpdata[0] = m_Desc.normal.x; lpdata[1] = m_Desc.normal.y; lpdata[2] = m_Desc.normal.z; lpdata[3] = m_Desc.d; return 4; };  
			LYNXINLINE virtual void					vSetSize(LYNXREAL* lpdata) {m_Desc.normal.x = lpdata[0]; m_Desc.normal.y = lpdata[1]; m_Desc.normal.z = lpdata[2]; m_Desc.d = lpdata[3]; ((NxPlaneShape*)m_lpShape)->setPlane(lpdata, m_Desc.d); };  
		};	
	}
}

#endif