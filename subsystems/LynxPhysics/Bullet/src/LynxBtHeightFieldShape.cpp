#include <LynxEngine_PCH.h>
#include <NxCooking.h>
#include <LynxNxPhysicsSystem.h>
#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxHeightFieldShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHeightFieldShape::CHeightFieldShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_Type = TRIMESH;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHeightFieldShape::~CHeightFieldShape(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CHeightFieldShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CHeightFieldShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CHeightFieldShapeDesc*)desc;

			NxHeightFieldDesc HeightFieldDesc;

			HeightFieldDesc.setToDefault();
			HeightFieldDesc.nbColumns			= pDesc->m_NumZ;
			HeightFieldDesc.nbRows				= pDesc->m_NumX;
			HeightFieldDesc.convexEdgeThreshold	= 0;
			// allocate storage for samples
			HeightFieldDesc.samples				= LYNXCORE_NEW NxU32[HeightFieldDesc.nbColumns*HeightFieldDesc.nbRows];
			HeightFieldDesc.sampleStride		= sizeof(NxU32);
			char* currentByte = (char*)HeightFieldDesc.samples;
			for (NxU32 row = 0; row < HeightFieldDesc.nbRows; row++)
			{
				for (NxU32 column = 0; column < HeightFieldDesc.nbColumns; column++)
				{
					NxI16 height = (NxI16)(pDesc->m_HeightMap[column*(pDesc->m_NumX)+row] * 256.0f);				
					
					NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
					currentSample->height = height;
					currentSample->tessFlag = 0;
					currentSample->materialIndex0 = 0;
					currentSample->materialIndex1 = 0;				
					currentByte += HeightFieldDesc.sampleStride;
				}
			}
			NxHeightField* HeightField = ((CPhysicsSystem*)(m_lpSpace->GetlpWorld()->GetlpPhysicsSystem()))->GetNxPhySDK()->createHeightField(HeightFieldDesc);

			m_Desc.setToDefault();
			m_Desc.heightField				= HeightField;
			m_Desc.heightScale				= 1.0f/256.0f;
			m_Desc.columnScale				= pDesc->m_SizeZ / NxReal(HeightFieldDesc.nbColumns-1);
			m_Desc.rowScale					= pDesc->m_SizeX / NxReal(HeightFieldDesc.nbRows-1);
			m_Desc.meshFlags				= NX_MESH_SMOOTH_SPHERE_COLLISIONS;
			m_Desc.materialIndexHighBits	= 0;
			m_Desc.holeMaterial				= 2;

			m_Desc.localPose.t.x			= -pDesc->m_SizeX*0.5f;
			m_Desc.localPose.t.z			= -pDesc->m_SizeZ*0.5f;

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}			
	}
}
