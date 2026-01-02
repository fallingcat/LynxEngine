#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsSystem.h>
#include <LynxBtPhysicsSpace.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtHullShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHullShape::CHullShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = HULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHullShape::~CHullShape(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CHullShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CHullShapeDesc* pHullDesc = (LynxEngine::PhysicsSystem::CHullShapeDesc*)desc;

			lynxMemCpy(&m_Desc, pHullDesc, sizeof(LynxEngine::PhysicsSystem::CHullShapeDesc));
			
			m_lpShape = LYNXGLOBAL_NEW btConvexHullShape((btScalar*)pHullDesc->m_VertexList, pHullDesc->m_NumVertices, pHullDesc->m_VertexStride);

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();		

			//m_lpShape->getContactBreakingThreshold(0.05f);

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CHullShape::vGetNumVertices()
		{
			return 0;//((btConvexHullShape*)m_lpShape)->getNumVertices();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CHullShape::vGetNumTriangles()
		{
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float* CHullShape::vGetVertices()
		{
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int* CHullShape::vGetTriangles()
		{
			return NULL;
		}
	}
}
