#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsShape.h>
#include <LynxNxPhysicsObj.h>
#include <LynxNxPhysicsSpace.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::CPhysicsShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: LynxEngine::PhysicsSystem::CPhysicsShape(lps)
		{	
			m_lpShape = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::~CPhysicsShape(void)
		{
			vDestory();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::vDestory() 
		{ 
			if (m_lpObj)
			{
				m_lpObj->vRemoveShape(this);
				((CPhysicsObj*)m_lpObj)->GetlpNxActor()->releaseShape(*m_lpShape);
				m_lpObj = NULL;
			}
			m_lpShape = NULL; 			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::CreateStaticPhyObj() 
		{
			m_lpObj = m_lpSpace->GetlpWorld()->vCreateObj(LynxEngine::PhysicsSystem::CPhysicsObj::STATIC, this);			
			m_lpObj->SetName(GetName() + LynxEngine::CString(_T(".PhyObj")));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::DestoryPhyObj() 
		{
			if (m_lpObj)
			{
				m_lpObj->GetlpWorld()->vDeleteObj(m_lpObj);
				m_lpObj = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsShape::vSetTransformMatrix(const CMatrix4& m) 
		{ 			
			NxMat34 M;

			M.setColumnMajor44(m.f);
			m_lpShape->setGlobalPose(M);	
			m_M = m;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsShape::vGetTransformMatrix()
		{
			NxMat34 M = m_lpShape->getGlobalPose();
			M.getColumnMajor44(m_M.f);

			return m_M;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsShape::vSetLocalTransformMatrix(const CMatrix4& m) 
		{ 			
			NxMat34 M;

			M.setColumnMajor44(m.f);			
			m_lpShape->setLocalPose(M);			
			m_LocalM = m;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsShape::vGetLocalTransformMatrix()
		{
			NxMat34 M = m_lpShape->getLocalPose();
			M.getColumnMajor44(m_LocalM.f);

			return m_LocalM;
		}
	}
}

#endif