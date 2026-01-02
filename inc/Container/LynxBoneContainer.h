//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
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

#ifndef __LYNXBONECONTAINER_H__
#define __LYNXBONECONTAINER_H__

#include <Container/LynxContainer.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxEngine 
{
	class CEngine;
	class LYNXENGCLASS CBoneContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CBoneContainer);
	public:
		typedef struct {
			BYTE						NumBones;	
			LYNXREAL*					lpWeight;		
			BYTE*						lpBoneIndex;
			LPLYNXVECTOR3D				lpOffset;	
			LPLYNXVECTOR3D				lpOffsetNormal;
		}WEIGHTDATA;
	protected:
		int										m_BoneID;
		int										m_VertexConstantIndex;
		PhysicsSystem::CPhysicsObj*				m_lpPhyObj;
		Math::CMatrix4							m_Physics2ContainerM;
	public:				
		CBoneContainer(CEngine *lpengine);
		~CBoneContainer();		

		CBoneContainer&							operator =(const CBoneContainer& b);	

		LYNXFORCEINLINE int						GetBoneIndex() {return m_BoneID; };

		LYNXFORCEINLINE PhysicsSystem::CPhysicsObj*	GetlpPhyObj() {return m_lpPhyObj; };
		PhysicsSystem::CPhysicsObj*				CreatePhyObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g);
		LYNXFORCEINLINE void					SetPhysics2ContainerM(Math::CMatrix4& m) {m_Physics2ContainerM = m; };

		void									vInstance(CContainer& rhs);
		void									vCalculateLOD(CCameraContainer* cam);
		
		LYNXINLINE virtual LYNXBOOL				vPlay(CReal skip, LYNXPLAYMODE mode) {return  LYNX_TRUE; };
		LYNXINLINE virtual LYNXBOOL				vPlay(CReal skip) {return  LYNX_TRUE;};		
		virtual void							vRender(CCameraContainer* cam = NULL) {};		

		friend class CModelContainer;
	};
}
#endif