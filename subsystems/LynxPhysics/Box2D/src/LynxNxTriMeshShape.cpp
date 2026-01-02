#include <LynxEngine_PCH.h>
#include <NxCooking.h>
#include <LynxNxPhysicsSystem.h>
#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxTriMeshShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTriMeshShape::CTriMeshShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_Type = TRIMESH;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTriMeshShape::~CTriMeshShape(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTriMeshShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CTriMeshShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CTriMeshShapeDesc*)desc;

			NxTriangleMeshDesc MeshDesc;

			MeshDesc.numVertices			= pDesc->m_NumVertices;
			MeshDesc.pointStrideBytes		= pDesc->m_VertexStride;
			MeshDesc.points					= pDesc->m_VertexList;
			MeshDesc.numTriangles			= pDesc->m_NumTriangles;
			MeshDesc.triangleStrideBytes	= pDesc->m_TriangleStride;
			MeshDesc.triangles				= pDesc->m_TriangleList;
			MeshDesc.flags					= 0;//NX_MF_FLIPNORMALS ;						
	
			//cooking is a slow process and should generally be done offline.
			NxCookingInterface *pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
			pCooking->NxInitCooking();

				MemoryWriteBuffer Buf;
				pCooking->NxCookTriangleMesh(MeshDesc, Buf);
				// Create the mesh from the cooked data.
				// Please note about the created Convex Mesh, user needs to release it when no one uses it to save memory. It can be detected
				// by API "meshData->getReferenceCount() == 0". And, the release API is "gPhysicsSDK->releaseConvexMesh(*convexShapeDesc.meshData);"
				NxTriangleMesh* TriangleMesh = ((CPhysicsSystem*)(m_lpSpace->GetlpWorld()->GetlpPhysicsSystem()))->GetNxPhySDK()->createTriangleMesh(MemoryReadBuffer(Buf.data));

			pCooking->NxCloseCooking();

			m_Desc.meshData = TriangleMesh;

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
		int CTriMeshShape::vGetNumVertices()
		{
			return m_Desc.meshData->getCount(0, NX_ARRAY_VERTICES);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CTriMeshShape::vGetNumTriangles()
		{
			return m_Desc.meshData->getCount(0, NX_ARRAY_TRIANGLES);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float* CTriMeshShape::vGetVertices()
		{
			return (float*)(m_Desc.meshData->getBase(0, NX_ARRAY_VERTICES));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int* CTriMeshShape::vGetTriangles()
		{
			NxInternalFormat Format = m_Desc.meshData->getFormat(0, NX_ARRAY_TRIANGLES);  
			return (int*)(m_Desc.meshData->getBase(0, NX_ARRAY_TRIANGLES));			
		}
	}
}
