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

#ifndef __LYNXPHYSICSRAGDOLL_H__
#define __LYNXPHYSICSRAGDOLL_H__

#include <LynxGameObj.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsShape.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CRagdoll : public CObj
		{	
			LYNX_DECLARE_CLASS(CRagdoll);		
		public:			
		protected:
			CPhysicsWorld*						m_lpWorld;
			CContainer*							m_lpContainer;
			CList<CPhysicsObj*>					m_PhyObjList;
			CList<CPhysicsShape*>				m_PhyShapeList;
			CList<CPhysicsJoint*>				m_PhyJointList;		
			LYNXBOOL							m_bActive;
		protected:
			void								Release(void);
		public:				
			CRagdoll(CPhysicsWorld* const lpw);			
			virtual ~CRagdoll();

			LYNXFORCEINLINE CPhysicsWorld*		GetlpWorld() {return m_lpWorld; };	
			LYNXFORCEINLINE	void				SetlpContainer(CContainer* lpc) {m_lpContainer = lpc; };
			LYNXBOOL							vCreate();
			void								SetupFromContainer(void);

			CList<CPhysicsObj*>&				GetObjList() {return m_PhyObjList; };
			LYNXFORCEINLINE void				AddObj(CPhysicsObj* o) {m_PhyObjList.push_back(o); };
			CList<CPhysicsShape*>&				GetShapeList() {return m_PhyShapeList; };
			LYNXFORCEINLINE void				AddShape(CPhysicsShape* s) {m_PhyShapeList.push_back(s); };
			CList<CPhysicsJoint*>&				GetJointList() {return m_PhyJointList; };
			LYNXFORCEINLINE void				AddJoint(CPhysicsJoint* j) {m_PhyJointList.push_back(j); };

			LYNXFORCEINLINE LYNXBOOL			IsActive() {return m_bActive; };	
			LYNXFORCEINLINE void				Activate(LYNXBOOL b) {m_bActive = b; };	

			LYNXFORCEINLINE void				UpdateTransform(OBJUPDATETYPE type) {m_lpContainer->vRagDollUpdate(type); };
		};	
	}
}

#endif