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

#ifndef __LYNXBTSPHERE_H__
#define __LYNXBTSPHERE_H__

#include <CollisionSystem/Bullet/LynxBtCollisionObj.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CBtSphere : public CBtCollisionObj
		{	
			LYNX_DECLARE_CLASS(CBtSphere);
		public:			
		protected:			
			CVector3				m_Center;
			float					m_Radius;			
		public:			
			CBtSphere();
			CBtSphere(const float r);
			CBtSphere(const CVector3& c, const float r);
			CBtSphere(const CBtSphere& p);
			virtual ~CBtSphere();
			
			LYNXFORCEINLINE const float& GetRadius() {return m_Radius; };
			LYNXFORCEINLINE void		Set(const float r) 
			{
				m_Radius = r; 
				if (m_Shape)
				{
					LYNXGLOBAL_DEL(m_Shape);
					m_Shape = NULL;
				}
				m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);
			}						

			void						vInstance(CCollisionObj& o);
			
			void						vRender(CCameraContainer& c);
			void						vSetupCollisionMatrix(CMatrix4& m);
			void						vSetupRenderMatrix(CMatrix4& m);
		};				
	}
}

#endif