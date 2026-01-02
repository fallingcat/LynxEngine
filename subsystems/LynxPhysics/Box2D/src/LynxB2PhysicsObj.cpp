#include <LynxEngine_PCH.h>
#include <LynxB2PhysicsObj.h>
#include <LynxB2PhysicsWorld.h>
//#include <LynxB2PhysicsSpace.h>
#include <LynxB2PhysicsShape.h>

namespace LynxPhysics 
{
	namespace Box2D 
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
			m_BodyDef.type = b2_staticBody;
			m_BodyDef.position = b2Vec2(0.0f, 0.0f);
			m_BodyDef.angle = 0.0f;
			m_BodyDef.allowSleep = true;
			m_BodyDef.awake = true;
			m_BodyDef.active = true;
					
			/*
			if (lpg)
			{
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
			*/

			m_Body = ((CPhysicsWorld*)m_lpWorld)->Getb2World()->CreateBody(&m_BodyDef);			
			if (!m_Body)
				return LYNX_FALSE;
	
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::CreateDynamic(CPhysicsShape* lpg)
		{
			m_BodyDef.type = b2_dynamicBody;
			m_BodyDef.position = b2Vec2(0.0f, 0.0f);
			m_BodyDef.angle = 30.0f*LYNXD2R;
			m_BodyDef.allowSleep = true;
			m_BodyDef.awake = true;
			m_BodyDef.active = true;
			
			/*
			 if (lpg)
			 {
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
			 */
			
			m_Body = ((CPhysicsWorld*)m_lpWorld)->Getb2World()->CreateBody(&m_BodyDef);			
			if (!m_Body)
				return LYNX_FALSE;
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vDestory()
		{			
			vRemoveAllShapes();		
			((CPhysicsWorld*)m_lpWorld)->Getb2World()->DestroyBody(m_Body);	
			m_Body = NULL;
			m_lpWorld = NULL;
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
			m_Body->CreateFixture(&(dynamic_cast<CPhysicsShape*>(lpg)->Getb2FixtureDef()));
			/*
			LYNXVECTOR3D Pos;

			LYNX_ASSERT(lpg->GetlpObj() != this);

			((CPhysicsShape*)lpg)->DestoryPhyObj();
			m_ShapeList.push_back(lpg);
			lpg->SetlpObj(this);
			((CPhysicsShape*)lpg)->SetlpNxShape(m_Body->createShape(((CPhysicsShape*)lpg)->vGetDesc()));
			m_Body->updateMassFromShapes(0.0f, m_MassValue);
			m_Body->saveBodyToDesc(m_BodyDesc);
			 */
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape *lpg)
		{			
			m_ShapeList.remove(lpg);			
			lpg->GetlpSpace()->vDeleteShape(lpg);
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
			LynxEngine::Math::CVector3 Pos, Rotation;
			m.GetPos(Pos);
			m.GetRotation(Rotation);

			switch (m_lpWorld->GetPhysicsMap2RendererType())
			{
				case LynxEngine::PhysicsSystem::CPhysicsWorld::PMRT_XY_PLANE:		
					m_Body->SetTransform(b2Vec2(Pos.x, Pos.y), Rotation.z*LYNXD2R);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsWorld::PMRT_XZ_PLANE:
					m_Body->SetTransform(b2Vec2(Pos.x, Pos.z), Rotation.y*LYNXD2R);					
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsObj::vGetTransformMatrix()
		{
			b2Vec2 Pos = m_Body->GetPosition();
			LynxEngine::Math::CReal Rotation = m_Body->GetAngle()*LYNXR2D;
			
			switch (m_lpWorld->GetPhysicsMap2RendererType())
			{
				case LynxEngine::PhysicsSystem::CPhysicsWorld::PMRT_XY_PLANE:
					LynxEngine::Math::MakeRotateMatrix(m_M, Rotation, LYNX_Z_AXIS);
					m_M.m41 = Pos.x;
					m_M.m42 = Pos.y;
					break;

				case LynxEngine::PhysicsSystem::CPhysicsWorld::PMRT_XZ_PLANE:
					LynxEngine::Math::MakeRotateMatrix(m_M, Rotation, LYNX_Y_AXIS);
					m_M.m41 = Pos.x;
					m_M.m43 = Pos.y;
					break;
			}		
			return m_M;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vReset()
		{			
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