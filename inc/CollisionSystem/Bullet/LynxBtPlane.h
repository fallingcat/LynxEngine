//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXBTPLANE_H__
#define __LYNXBTPLANE_H__

#include <CollisionSystem/Bullet/LynxBtCollisionObj.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CBtPlane : public CBtCollisionObj
		{	
			LYNX_DECLARE_CLASS(CBtPlane);
		public:			
		protected:			
			btStaticPlaneShape*		m_Shape;
			CVector3				m_Normal;
			float					m_Distance;			
		public:			
			CBtPlane();
			CBtPlane(const CVector3& n, const float d);
			virtual ~CBtPlane();	

			LYNXINLINE const CVector3& 	GetNormal() {return m_Normal; };
			LYNXINLINE const float& 	GetDistance() {return m_Distance; };			
			LYNXINLINE void				Set(const CVector3& n, const float d) 
			{
				m_Normal = n; 
				m_Distance = d; 
				if (m_Shape)
				{
					LYNXGLOBAL_DEL(m_Shape);
					m_Shape = NULL;
				}
				btVector3 Normal(m_Normal.x, m_Normal.y, m_Normal.z);
				m_Shape = LYNXGLOBAL_NEW btStaticPlaneShape(Normal, m_Distance);				
			}									
		};				
	}
}

#endif