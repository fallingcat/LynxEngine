#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <NxCooking.h>
#include <LynxNxPhysicsSystem.h>
#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxHullShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
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
			LynxEngine::PhysicsSystem::CHullShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CHullShapeDesc*)desc;

			NxConvexMeshDesc  MeshDesc;

			MeshDesc.numVertices			= pDesc->m_NumVertices;
			MeshDesc.pointStrideBytes		= pDesc->m_VertexStride;
			MeshDesc.points					= pDesc->m_VertexList;
			MeshDesc.flags					= NX_CF_COMPUTE_CONVEX;						
	
			//cooking is a slow process and should generally be done offline.
			NxCookingInterface *pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
			pCooking->NxInitCooking();
				MemoryWriteBuffer Buf;
				pCooking->NxCookConvexMesh(MeshDesc, Buf);
				// Create the mesh from the cooked data.
				// Please note about the created Convex Mesh, user needs to release it when no one uses it to save memory. It can be detected
				// by API "meshData->getReferenceCount() == 0". And, the release API is "gPhysicsSDK->releaseConvexMesh(*convexShapeDesc.meshData);"
				NxConvexMesh* ConvexMesh = ((CPhysicsSystem*)(m_lpSpace->GetlpWorld()->GetlpPhysicsSystem()))->GetNxPhySDK()->createConvexMesh(MemoryReadBuffer(Buf.data));
			pCooking->NxCloseCooking();
			m_Desc.meshData = ConvexMesh;

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CHullShape::vGetNumVertices()
		{
			return m_Desc.meshData->getCount(0, NX_ARRAY_VERTICES);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CHullShape::vGetNumTriangles()
		{
			return m_Desc.meshData->getCount(0, NX_ARRAY_TRIANGLES);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float* CHullShape::vGetVertices()
		{
			return (float*)(m_Desc.meshData->getBase(0, NX_ARRAY_VERTICES));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int* CHullShape::vGetTriangles()
		{
			NxInternalFormat Format = m_Desc.meshData->getFormat(0, NX_ARRAY_TRIANGLES);  
			return (int*)(m_Desc.meshData->getBase(0, NX_ARRAY_TRIANGLES));			
		}
	}
}

#endif