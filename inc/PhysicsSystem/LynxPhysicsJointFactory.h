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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXPHYSICSJOINTFACTORY_H__
#define __LYNXPHYSICSJOINTFACTORY_H__

#include <LynxEngineClasses.h>
#include <LynxAlgorithm.h>
#include <LynxObj.h>
#include <LynxFactory.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	namespace PhysicsSystem
	{
		class LYNXENGCLASS CJointFactory : public CFactory<CPhysicsJoint, LYNXGUID, CPhysicsWorld*>
		{	
			LYNX_DECLARE_CLASS(CJointFactory);
		protected:		
			CList<CPhysicsJoint*>				m_JointList;	
		public:			
			CJointFactory() {};
			virtual ~CJointFactory()
			{
				#ifdef __TOOL__
					m_JointList.clear();
				#endif
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CPhysicsJoint* CreateObject(const LYNXGUID& id)
			{
				CPhysicsJoint* lpJ = CFactory<CPhysicsJoint, LYNXGUID, CPhysicsWorld*>::CreateObject(id);
				m_JointList.push_back(lpJ);

				return lpJ;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void DestroyObject(CPhysicsJoint* const lpc)
			{
				m_JointList.remove(lpc);
				LYNXDEL lpc;			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CList<CPhysicsJoint*>& GetJointList()
			{
				return m_JointList;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CPhysicsJoint* Find(const CString& name)
			{
				CList<CPhysicsJoint*>::CIterator Con = LynxEngine::find(m_JointList.begin(), m_JointList.end(), name);
				if (Con != m_JointList.end())
					return (*Con);

				return NULL;	
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CPhysicsJoint* Find(const LYNXCHAR *name)
			{
				return Find(CString(name));
			}
		};
	}
}
#endif