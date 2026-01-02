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

#include <LynxEngine_PCH.h>
#include <Container/LynxModelContainer.h>
#include <PhysicsSystem/LynxPhysicsRagdoll.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::vCreatePhysics(PhysicsSystem::CPhysicsWorld* lpw, PhysicsSystem::CPhysicsShape* lps)
		{
			LYNX_GUARD(CDynamicObj::vCreatePhysics)

			if (m_lpPhyObj)
				m_lpPhyObj->vDestory();

			LYNX_ASSERT(lpw != NULL);
			
			m_lpPhyObj = lpw->vCreateObj(PhysicsSystem::CPhysicsObj::DYNAMIC, lps);
			if (!m_lpPhyObj)
				return LYNX_FALSE;
				//throw CException(_T("This dynamic object without physical object"));
			m_lpPhyObj->vSetTransformMatrix(GetTransformMatrix());

			CString Name;

			Name = GetName() + CString(".PhyObj");
			m_lpPhyObj->SetName(Name);
			m_lpPhyObj->SetlpRenObj(this);

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::CreatePhysics(PhysicsSystem::CPhysicsShape::TYPE shape_type)
		{
			LYNX_GUARD(CDynamicObj::vCreatePhysics)

			if (m_lpPhyObj)
				m_lpPhyObj->vDestory();

			PhysicsSystem::CPhysicsShape* lpShape = NULL;
			//lpShape = m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0)->GetlpSpace(0)->vCreateShape(shape_type, const CCreationDesc *desc);			
			m_lpPhyObj = m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0)->vCreateObj(PhysicsSystem::CPhysicsObj::DYNAMIC, lpShape);
			if (!m_lpPhyObj)
				return LYNX_FALSE;
				//throw CException(_T("This dynamic object without physical object"));
			m_lpPhyObj->vSetTransformMatrix(GetTransformMatrix());

			CString Name;

			Name = GetName() + CString(".PhyObj");
			m_lpPhyObj->SetName(Name);
			m_lpPhyObj->SetlpRenObj(this);

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::CreateRagdoll(PhysicsSystem::CPhysicsWorld* lpw)
		{
			if (m_lpContainer->GetContainerType() != CContainer::MODEL)
				return LYNX_FALSE;
			
			m_lpRagdoll = LYNXNEW PhysicsSystem::CRagdoll(lpw);
			m_lpRagdoll->SetlpContainer(m_lpContainer);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::vGenerateRagdoll(PhysicsSystem::CPhysicsWorld* lpw)
		{
			if (!CreateRagdoll(lpw))
				return LYNX_FALSE;
			
			m_lpRagdoll->vCreate();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::SetMass(PhysicsSystem::CPhysicsObj::MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata) 
		{
			LYNX_ASSERT(m_lpPhyObj); 
			m_lpPhyObj->vSetMass(t, density, lpdata);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::SetMass(LYNXREAL mass) 
		{
			LYNX_ASSERT(m_lpPhyObj); 
			m_lpPhyObj->vSetMass(mass);	
		}				
	}
}