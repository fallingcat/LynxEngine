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

#ifndef __LYNXBTCAPSULE_H__
#define __LYNXBTCAPSULE_H__

#include <CollisionSystem/Bullet/LynxBtCollisionObj.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CBtCapsule : public CBtCollisionObj
		{	
			LYNX_DECLARE_CLASS(CBtCapsule);
		public:			
		protected:			
			CVector3				m_Center;
			float					m_Height;
			float					m_Radius;			
		public:			
			CBtCapsule();
			CBtCapsule(const CVector3& c, const float r, const float h);
			CBtCapsule(const CBtCapsule& p);
			virtual ~CBtCapsule();	
			
			LYNXFORCEINLINE CVector3& GetCenter() {return m_Center; }
			LYNXFORCEINLINE const float& GetRadius() {return m_Radius; };
			LYNXFORCEINLINE const float& GetHeight() {return m_Height; };			
			LYNXFORCEINLINE void		Set(const CVector3& c, const float r, const float h) 
			{
				m_Center = c;
				m_Radius = r; 
				m_Height = h; 
				if (m_Shape)
				{
					LYNXGLOBAL_DEL(m_Shape);
					m_Shape = NULL;
				}
				m_Shape = LYNXGLOBAL_NEW btCapsuleShape(m_Radius, m_Height);				
			}						

			void						vInstance(CCollisionObj& o);		
			
			void						vRender(CCameraContainer& c);
			void						vSetupCollisionMatrix(CMatrix4& m);
			void						vSetupRenderMatrix(CMatrix4& m);
		};				
	}
}

#endif