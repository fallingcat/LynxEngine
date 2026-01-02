#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsSystem.h>
#include <LynxBtPhysicsSpace.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtTriMeshShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTriMeshShape::CTriMeshShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = TRIMESH;
			m_MeshInterface = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTriMeshShape::~CTriMeshShape(void)
		{
			if (m_MeshInterface)
				LYNXGLOBAL_DEL(m_MeshInterface);

			if (m_lpShape)
			{
				LYNXGLOBAL_DEL(m_lpShape);
				m_lpShape = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTriMeshShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CTriMeshShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CTriMeshShapeDesc*)desc;

			lynxMemCpy(&m_Desc, pDesc, sizeof(LynxEngine::PhysicsSystem::CTriMeshShapeDesc));

			m_MeshInterface = LYNXGLOBAL_NEW btTriangleIndexVertexArray();
			btIndexedMesh Part;

			Part.m_vertexBase			= (const unsigned char*)pDesc->m_VertexList;
			Part.m_vertexStride			= pDesc->m_VertexStride;
			Part.m_numVertices			= pDesc->m_NumVertices;
			Part.m_triangleIndexBase	= (const unsigned char*)pDesc->m_TriangleList;
			Part.m_triangleIndexStride	= pDesc->m_TriangleStride;
			Part.m_numTriangles			= pDesc->m_NumTriangles;
			Part.m_indexType			= PHY_INTEGER;
			m_MeshInterface->addIndexedMesh(Part, PHY_INTEGER);
			
			// This is for static object
			m_lpShape = LYNXGLOBAL_NEW btBvhTriangleMeshShape(m_MeshInterface, LYNX_TRUE);

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
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CTriMeshShape::vGetNumTriangles()
		{
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float* CTriMeshShape::vGetVertices()
		{
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int* CTriMeshShape::vGetTriangles()
		{
			return NULL;
		}
	}
}
