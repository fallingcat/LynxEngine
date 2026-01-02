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

#ifndef __LYNXPHYSICSWORLD_H__
#define __LYNXPHYSICSWORLD_H__

#include <LynxEngineClasses.h>
//#include <PhysicsSystem/LynxPhysicsShape.h>
//#include <PhysicsSystem/LynxPhysicsSpace.h>
//#include <PhysicsSystem/LynxPhysicsObj.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>
#include <PhysicsSystem/LynxPhysicsJointFactory.h>

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CPhysicsWorld : public CGameObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsWorld);
		public:
			// {B97D3EA6-B65F-4dc1-A9CA-91B1CD20D636}
			static const LYNXGUID				GUID_BALLJOINT; 
			// {61561C43-0F81-4bc4-BEE3-CBB8005D8782}
			static const LYNXGUID				GUID_HINGJOINT; 
			// {862B3744-0EE6-425f-8772-028656B47FE4}
			static const LYNXGUID				GUID_SLIDERJOINT;

			typedef enum {
				PMRT_XY_PLANE = 0,
				PMRT_XZ_PLANE,
			}PHYSICSMAPTORENDERERTYPE;
		protected:			
			CPhysicsSystem*						m_lpPhysicsSystem;							
			CJointFactory						m_JointFactory;

			PHYSICSMAPTORENDERERTYPE			m_PhysicsMap2RendererType;

			CPhysicsSpace*						m_lpRootSpace;
			LYNXBOOL							m_bFastStep;			
			LYNXVECTOR3D						m_Gravity;	
			CList<CPhysicsSpace*>				m_SpaceList;
			CList<CPhysicsObj*>					m_ObjList;
			CList<CPhysicsJoint*>				m_JointList;		
		public:			
			CPhysicsWorld(CPhysicsSystem* const lpsys);
			virtual ~CPhysicsWorld();			
			
			LYNXFORCEINLINE CPhysicsSystem* const	GetlpPhysicsSystem() {return m_lpPhysicsSystem; };
			
			LYNXFORCEINLINE PHYSICSMAPTORENDERERTYPE GetPhysicsMap2RendererType() {return m_PhysicsMap2RendererType; }

			virtual LYNXBOOL					vCreate() = 0;
			LYNXFORCEINLINE void				SetFastStep(LYNXBOOL b) {m_bFastStep = b; };
			LYNXFORCEINLINE LYNXBOOL 			GetFastStep() {return m_bFastStep; };
			virtual void						vCollisionLoop() = 0;
			virtual void						vStep(float step) = 0;		
			virtual void 						vSetGravity(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void 						vSetGravity(LPLYNXVECTOR3D lpg) = 0;

			// Space --------------------------------------------------------------------------------------------
			LYNXFORCEINLINE CPhysicsSpace* const GetlpRootSpace() {return m_lpRootSpace; };
			LYNXFORCEINLINE DWORD				GetNumSpaces() {return (DWORD)m_SpaceList.size(); };
			LYNXFORCEINLINE CPhysicsSpace* const GetlpSpace(int i) {return m_SpaceList.get(i); };			
			LYNXFORCEINLINE void				AddSpace(CPhysicsSpace* const lps) {m_SpaceList.push_back(lps); };
			LYNXFORCEINLINE void				DeleteSpace(CPhysicsSpace* const lps) {m_SpaceList.remove(lps); };
			virtual CPhysicsSpace* const 		vCreateSpace(CPhysicsSpace* const parent) = 0;
			virtual void						vDeleteSpace(CPhysicsSpace* const lpo) = 0;					
			
			// Object -------------------------------------------------------------------------------------------
			LYNXFORCEINLINE DWORD				GetNumObjs() {return (DWORD)m_ObjList.size(); };
			LYNXFORCEINLINE CPhysicsObj* const	GetlpObj(int i) {return m_ObjList.get(i); };	
			LYNXFORCEINLINE void				AddObj(CPhysicsObj* const lpo) {m_ObjList.push_back(lpo); };
			LYNXFORCEINLINE void				DeleteObj(CPhysicsObj* const lpo) {m_ObjList.remove(lpo); };
			virtual CPhysicsObj* const			vCreateObj(CPhysicsObj::TYPE t, CPhysicsShape* g = NULL) = 0;
			virtual void						vDeleteObj(CPhysicsObj* const lpo) = 0;					
			CPhysicsObj*						FindObj(const CString& name);


			// Joint -------------------------------------------------------------------------------------------
			LYNXFORCEINLINE DWORD				GetNumJoints() {return (DWORD)m_JointList.size(); };
			LYNXFORCEINLINE CPhysicsJoint* const GetlpJoint(int i) {return m_JointList.get(i); };	
			LYNXFORCEINLINE void				AddJoint(CPhysicsJoint* const lpj) {m_JointList.push_back(lpj); };
			LYNXFORCEINLINE void				DeleteJoint(CPhysicsJoint* const lpj) {m_JointList.remove(lpj); };
			virtual CPhysicsJoint* const		vCreateJoint(CPhysicsJoint::TYPE type, const CCreationDesc* desc) = 0;
			virtual void						vDeleteJoint(CPhysicsJoint* const lpj) = 0;					
		};	
	}
}

#endif