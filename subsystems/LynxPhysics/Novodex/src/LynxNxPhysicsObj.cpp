#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsObj.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw)
		: LynxEngine::PhysicsSystem::CPhysicsObj(lpw)
		{			
			m_Body = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::~CPhysicsObj(void)
		{
			vDestory();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::vCreate(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g)
		{
			m_PhysicsObjType = t;
			switch (t)
			{
				case LynxEngine::PhysicsSystem::CPhysicsObj::STATIC:
					return CreateStatic(dynamic_cast<CPhysicsShape*>(g));
					break;

				case LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC:
					return CreateDynamic(dynamic_cast<CPhysicsShape*>(g));
					break;
			}
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::CreateStatic(CPhysicsShape* lpg)
		{
			NxActorDesc			actorDesc;			

			actorDesc.setToDefault();						
			
			if (lpg)
			{
				if (lpg->GetlpObj())
				{
					lpg->GetlpObj()->GetShapeList().remove(lpg);
					((CPhysicsShape*)lpg)->DestoryPhyObj();
				}
				actorDesc.shapes.pushBack(&(lpg->vGetDesc()));
			}
			else
			{
				NxBoxShapeDesc boxDesc;
				boxDesc.setToDefault();
				boxDesc.dimensions.set(0.0f, 0.0f, 0.0f);
				boxDesc.localPose.t = NxVec3(0.0f, 0.0f, 0.0f);						
				actorDesc.shapes.pushBack(&boxDesc);
			}

			actorDesc.globalPose.t = NxVec3(0, 0, 0);
			m_Body = ((CPhysicsWorld*)m_lpWorld)->GetlpNxScene()->createActor(actorDesc);			
			if (!m_Body)
				return LYNX_FALSE;

			if (lpg)
			{
				m_ShapeList.push_back(lpg);
				lpg->SetlpObj(this);
				((CPhysicsShape*)lpg)->SetlpNxShape(m_Body->createShape(((CPhysicsShape*)lpg)->vGetDesc()));
				m_Body->updateMassFromShapes(0.0f, m_MassValue);
				m_Body->saveBodyToDesc(m_BodyDesc);
			}

			UpdateProperties();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::CreateDynamic(CPhysicsShape* lpg)
		{
			NxActorDesc			actorDesc;			

			actorDesc.setToDefault();			
			m_BodyDesc.setToDefault();
			
			if (lpg)
			{
				if (lpg->GetlpObj())
				{
					lpg->GetlpObj()->GetShapeList().remove(lpg);
					((CPhysicsShape*)lpg)->DestoryPhyObj();
				}
				actorDesc.density = 0.0f;
				lpg->vGetDesc().mass = m_MassValue;			
				m_BodyDesc.mass = m_MassValue;				
				actorDesc.body = &m_BodyDesc;			
				actorDesc.shapes.pushBack(&(lpg->vGetDesc()));
			}
			else
			{
				actorDesc.density = 0.0f;
				m_BodyDesc.mass = m_MassValue;
				m_BodyDesc.massSpaceInertia.x = 1.0f;
				m_BodyDesc.massSpaceInertia.y = 1.0f;
				m_BodyDesc.massSpaceInertia.z = 1.0f;
				actorDesc.body = &m_BodyDesc;										
			}
			actorDesc.globalPose.t = NxVec3(0, 0, 0);
			m_Body = ((CPhysicsWorld*)m_lpWorld)->GetlpNxScene()->createActor(actorDesc);	
			if (!m_Body)
				return LYNX_FALSE;

			if (lpg)
			{
				m_ShapeList.push_back(lpg);
				lpg->SetlpObj(this);
				((CPhysicsShape*)lpg)->SetlpNxShape(m_Body->createShape(((CPhysicsShape*)lpg)->vGetDesc()));
				m_Body->updateMassFromShapes(0.0f, m_MassValue);
				m_Body->saveBodyToDesc(m_BodyDesc);
			}

			UpdateProperties();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vDestory()
		{			
			vRemoveAllShapes();		
			((CPhysicsWorld*)m_lpWorld)->GetlpNxScene()->releaseActor((*m_Body));			
			m_Body = NULL;
			m_lpWorld = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetKinematic(LYNXBOOL b)
		{
			LynxEngine::PhysicsSystem::CPhysicsObj::vSetKinematic(b);
			if (b)
			{
				m_Body->raiseBodyFlag(NX_BF_KINEMATIC);
				vNeverSleep(LYNX_TRUE);
			}
			else
			{
				m_Body->clearBodyFlag(NX_BF_KINEMATIC);
				vNeverSleep(LYNX_FALSE);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vAddShape(LynxEngine::PhysicsSystem::CPhysicsShape *lpg)
		{
			LYNXVECTOR3D Pos;

			LYNX_ASSERT(lpg->GetlpObj() != this);

			if (lpg->GetlpObj())
			{
				lpg->GetlpObj()->GetShapeList().remove(lpg);
				((CPhysicsShape*)lpg)->DestoryPhyObj();
			}
			m_ShapeList.push_back(lpg);
			lpg->SetlpObj(this);
			((CPhysicsShape*)lpg)->SetlpNxShape(m_Body->createShape(((CPhysicsShape*)lpg)->vGetDesc()));
			m_Body->updateMassFromShapes(0.0f, m_MassValue);
			m_Body->saveBodyToDesc(m_BodyDesc);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape *lpg)
		{			
			m_ShapeList.remove(lpg);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveShape(int i)
		{
			LynxEngine::PhysicsSystem::CPhysicsShape *lpG = GetlpShape(i);
			if (lpG)
				vRemoveShape(lpG);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveAllShapes()
		{
			LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsShape*>::CIterator Geo = m_ShapeList.begin();
			while (Geo != m_ShapeList.end())
			{
				LynxEngine::PhysicsSystem::CPhysicsShape* lpGeo = *Geo;
				++Geo;
				vRemoveShape(lpGeo);				
			}
		}												
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetTransformMatrix(const CMatrix4& m) 
		{ 			
			NxMat34 M;

			M.setColumnMajor44(m.f);
			m_Body->setGlobalPose(M);
			m_M = m;

			/*for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsGeometry*>::CIterator Geo = m_GeometryList.begin(); Geo != m_GeometryList.end(); ++Geo)
			{				
				(*Geo)->vSetTransformMatrix(m); 				
			}*/			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsObj::vGetTransformMatrix()
		{
			NxMat34 M = m_Body->getGlobalPose();
			M.getColumnMajor44(m_M.f);

			return m_M;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vReset()
		{
			UpdateProperties();

			if (m_Body->isDynamic())
			{
				m_Body->setLinearVelocity(NxVec3(0.0f, 0.0f, 0.0f));
				m_Body->setAngularVelocity(NxVec3(0.0f, 0.0f, 0.0f));
				m_Body->setLinearMomentum(NxVec3(0.0f, 0.0f, 0.0f));
				m_Body->setAngularMomentum(NxVec3(0.0f, 0.0f, 0.0f));
			}
		}





		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CPhysicsObj::vSetGeometry(CPhysicsGeometry *lpg)
		//{
		//	m_lpGeometry = lpg;			
		//	((CNxPhyGeometry*)m_lpGeometry)->SetNxShape(m_Body->createShape(((CNxPhyGeometry*)lpg)->vGetDesc()));  
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CPhysicsObj::vUpdate()
		//{
		//	NxMat33 TransM = m_Body->getGlobalOrientation();
		//	NxVec3 Pos = m_Body->getGlobalPosition();			
		//		
		//	float Mat[9];
		//	TransM.getColumnMajor(Mat);

		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m11 = Mat[0];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m12 = Mat[1];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m13 = Mat[2];

		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m21 = Mat[3];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m22 = Mat[4];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m23 = Mat[5];

		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m31 = Mat[6];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m32 = Mat[7];
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m33 = Mat[8];

		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m41 = Pos.x;
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m42 = Pos.y;
		//	m_lpContainer->vGetlpObj3D()->LocalM.s.m43 = Pos.z;																	
		//}
	}
}

#endif