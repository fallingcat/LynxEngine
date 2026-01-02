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

#ifndef __LYNXCOLLISIONOBJ_H__
#define __LYNXCOLLISIONOBJ_H__

#include <LynxGameObj.h>
#include <LynxMathWrap.h>

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CCollisionObj : public CObj
		{	
			LYNX_DECLARE_CLASS(CCollisionObj);
		public:
			enum TYPE{
				NONE = -1,
				PLANE,			/*!< 平面碰撞。 */  
				AABB,			/*!< AABB(Axis Aligned Boundary Box)碰撞。 */  
				OBB,			/*!< OBB(Oriented Boundary Box)碰撞。 */  
				CYLINDER,		/*!< 圓柱碰撞。 */  
				CAPSULE,		/*!< 膠囊碰撞。 */  
				SPHERE,			/*!< 圓球碰撞。 */  
				HULL,			/*!< hull碰撞。 */  
				TRIMESH,		/*!< 多邊形碰撞。 */  
				NUM_TYPE,		
			};
			Math::CVector3						m_CollisionNormal;
			float								m_CollisionDistance;
			LYNXBOOL							m_bSkipTransfomScale;
		protected:		
			CCollisionSystem*					m_lpSystem;
			TYPE								m_Type;
			CContainer*							m_lpParentContainer;	
			LynxEngine::Math::CMatrix4			m_OffsetM;
		public:			
			CCollisionObj();
			virtual ~CCollisionObj();						

			LYNXFORCEINLINE void				SetOffsetM(const LynxEngine::Math::CMatrix4& m) { m_OffsetM = m; };
			LYNXFORCEINLINE LynxEngine::Math::CMatrix4 GetOffsetM() const { return m_OffsetM; };

			LYNXINLINE TYPE						GetType() const {return m_Type; };
			virtual void						vRelease(void);	
			virtual void						vInstance(CCollisionObj& o) {};
			virtual	LYNXBOOL					vHitTest(CCollisionObj& o) {return LYNX_FALSE; };				
			LYNXFORCEINLINE	void				SetParentContainer(CContainer* const p) {m_lpParentContainer = p; };
			LYNXFORCEINLINE	CContainer*			GetParentContainer() const {return m_lpParentContainer; };
			CCollisionObj&						operator=(CCollisionObj& rhs) {this->vInstance(rhs); return (*this);};						
			virtual void						vRender(CCameraContainer& c) {};
		};
	}
}

#endif