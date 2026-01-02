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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <PhysicsSystem/LynxPhysicsShape.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(PhysicsSystem::CPhysicsShape::TYPE e)
	{
		CString				ShapeType;

		switch (e)
		{
			case PhysicsSystem::CPhysicsShape::BOX:
				ShapeType = _T("Box");
				break;

			case PhysicsSystem::CPhysicsShape::SPHERE:
				ShapeType = _T("Sphere");
				break;

			case PhysicsSystem::CPhysicsShape::CAPSULE:
				ShapeType = _T("Capsule");
				break;

			case PhysicsSystem::CPhysicsShape::CYLINDER:
				ShapeType = _T("Cylinder");
				break;

			case PhysicsSystem::CPhysicsShape::PLANE:
				ShapeType = _T("Plane");
				break;

			case PhysicsSystem::CPhysicsShape::HULL:
				ShapeType = _T("Hull");
				break;

			case PhysicsSystem::CPhysicsShape::TRIMESH:				
				ShapeType = _T("TriMesh");
				break;
		}
		return ShapeType;
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	namespace PhysicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::CPhysicsShape(CPhysicsSpace* const lps)
		: CGameObj(lps->GetlpWorld()->GetlpPhysicsSystem()->GetlpEngine())
		{	
			m_PhysicsShapeType = NULL_SHAPE;			
			m_lpTargetContainer = NULL;
			m_lpSpace = lps;
			m_lpObj = NULL;
			m_M.Identity();
			m_LocalM.Identity();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::~CPhysicsShape(void)
		{
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPhysicsShape::vSave(CStream& stream)
		{
			stream.vWrite(&m_PhysicsShapeType, sizeof(TYPE), 1);
			stream.vWrite(&m_LocalM, sizeof(LYNXMATRIX), 1);
			stream.vWrite(&m_M, sizeof(LYNXMATRIX), 1);

			LYNXREAL Size[8];
			int Index = -1;
			CModelContainer* lpModel = NULL;

			switch (m_PhysicsShapeType)
			{
				case BOX:
					vGetSize(Size);
					stream.vWrite(Size, sizeof(float), 3);
					break;
				case SPHERE:
					vGetSize(Size);
					stream.vWrite(Size, sizeof(float), 1);
					break;
				case TRIMESH:						
					lpModel = dynamic_cast<CModelContainer*>(m_lpObj->GetlpRenObj()->GetlpContainer());
					if (lpModel)
					{				
						Index = lpModel->GetMeshIndex(m_lpTargetContainer->GetName());
					}
					stream.vWrite(&Index, sizeof(int), 1);
					// TODO, shape object is resposible for writing data
					break;
				case HULL:					
					lpModel = dynamic_cast<CModelContainer*>(m_lpObj->GetlpRenObj()->GetlpContainer());
					if (lpModel)
					{				
						Index = lpModel->GetMeshIndex(m_lpTargetContainer->GetName());
					}
					stream.vWrite(&Index, sizeof(int), 1);
					// TODO, shape object is resposible for writing data
					break;
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		CCreationDesc* CPhysicsShape::LoadDesc(TYPE& type, CStream& stream, LYNXBOOL skip)
		{
			static LynxEngine::PhysicsSystem::CSphereShapeDesc	SphereDesc;
			static LynxEngine::PhysicsSystem::CBoxShapeDesc		BoxDesc;
			static LynxEngine::PhysicsSystem::CTriMeshShapeDesc	TriMeshDesc;
			static LynxEngine::PhysicsSystem::CHullShapeDesc	HullDesc;
			
			stream.vRead(&type, sizeof(TYPE), 1);			

			LYNXREAL Size[8];
			switch (type)
			{
				case BOX:					
					stream.vRead(&BoxDesc.m_LocalM, sizeof(LYNXMATRIX), 1);
					stream.vRead(&BoxDesc.m_TransformM, sizeof(LYNXMATRIX), 1);
					stream.vRead(Size, sizeof(float), 3);
					BoxDesc.Default();
					BoxDesc.m_Extension.x = Size[0];
					BoxDesc.m_Extension.y = Size[1];
					BoxDesc.m_Extension.z = Size[2];
					return &BoxDesc;
					break;
				case SPHERE:					
					stream.vRead(&SphereDesc.m_LocalM, sizeof(LYNXMATRIX), 1);
					stream.vRead(&SphereDesc.m_TransformM, sizeof(LYNXMATRIX), 1);
					stream.vRead(Size, sizeof(float), 1);
					SphereDesc.Default();
					SphereDesc.m_Radius = Size[0];
					return &SphereDesc;
					break;
				case TRIMESH:										
					stream.vRead(&TriMeshDesc.m_LocalM, sizeof(LYNXMATRIX), 1);
					stream.vRead(&TriMeshDesc.m_TransformM, sizeof(LYNXMATRIX), 1);
					// TODO, shape object is resposible for writing data so no calculation when loading
					return &TriMeshDesc;
					break;
				case HULL:										
					stream.vRead(&HullDesc.m_LocalM, sizeof(LYNXMATRIX), 1);
					stream.vRead(&HullDesc.m_TransformM, sizeof(LYNXMATRIX), 1);
					// TODO, shape object is resposible for writing data so no calculation when loading
					return &HullDesc;
					break;
				default:
					return NULL;
					break;
			}
		}
	}
}