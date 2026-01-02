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
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{		
		IMPLEMENT_CLASSSCRIPT(CPhysicsObj, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::CPhysicsObj(CPhysicsWorld* const lpw)
		: CObj(lpw->GetlpPhysicsSystem()->GetlpEngine())
		{				
			m_lpWorld = lpw;
			m_lpRenObj = NULL;
			m_lpTargetContainer = NULL;
			m_MassValue = LYNX_REAL(1.0f);
			m_Density = LYNX_REAL(1.0f);
			m_bKinematic = LYNX_FALSE;
			m_bSleepAtStart = LYNX_FALSE;
			m_bNeverSleep = LYNX_FALSE;
			m_PhysicsStatus = ACTIVE;
			LYNX_IDENTITY_MATRIX(m_CMM);
			LYNX_IDENTITY_MATRIX(m_M);

			IMPLEMENT_PARAM_FLOAT(m_MassValue, 0)
			IMPLEMENT_PARAM_FLOAT(m_Density, 0)	
			IMPLEMENT_PARAM_BOOL(m_bKinematic, 0)
			IMPLEMENT_PARAM_BOOL(m_bSleepAtStart, 0)
			IMPLEMENT_PARAM_BOOL(m_bNeverSleep, 0)	
			IMPLEMENT_PARAM_LIST(m_ShapeList, CParam::LPOBJ, CParam::PARAM_NO_BROWSER|CParam::PARAM_SIZE_READONLY)		

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::~CPhysicsObj(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::UpdateProperties()
		{
			vSetMass(m_MassValue);
			vSetKinematic(m_bKinematic);
			vNeverSleep(m_bNeverSleep);
			vSleep(m_bSleepAtStart);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CPhysicsObj::GetMassData(CReal* lpdata)
		{
			switch (m_MassType)
			{
				default:
				case NULL_MASS:
					return 0;
					break;

				case BOX:
					lynxMemCpy(lpdata, m_Size, sizeof(LYNXREAL)*3);
					return 3;
					break;

				case SPHERE:
					lpdata[0] = m_Size[0];
					return 1;
					break;

				case CAPSULE:
					lynxMemCpy(lpdata, m_Size, sizeof(LYNXREAL)*2);
					return 2;					
					break;

				case CYLINDER:
					lynxMemCpy(lpdata, m_Size, sizeof(LYNXREAL)*2);
					return 2;					
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPhysicsObj::vSave(CStream& stream)
		{
			stream.vWrite(&m_PhysicsObjType, sizeof(TYPE), 1);
			stream.vWrite(&m_MassValue, sizeof(float), 1);

			DWORD NumShapes = m_ShapeList.size();
			stream.vWrite(&NumShapes, sizeof(DWORD), 1);
			CList<CPhysicsShape*>::CIterator Shape;
			for (Shape = m_ShapeList.begin(); Shape != m_ShapeList.end(); ++Shape)
			{
				(*Shape)->vSave(stream);
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPhysicsObj::vLoad(CStream& stream, LYNXBOOL skip)
		{
			int Index;
			CContainer* lpContainer = NULL;
			CModelContainer* lpModel = NULL;
			CPhysicsShape* lpShape;

			stream.vRead(&m_PhysicsObjType, sizeof(TYPE), 1);
			stream.vRead(&m_MassValue, sizeof(float), 1);

			DWORD NumShapes;
			stream.vRead(&NumShapes, sizeof(DWORD), 1);			

			for (int i=0; i<NumShapes; i++)
			{
				CPhysicsShape::TYPE ShapeType;
				CCreationDesc* lpDesc = CPhysicsShape::LoadDesc(ShapeType, stream, skip);

				lpContainer = m_lpRenObj->GetlpContainer();				

				switch (ShapeType)
				{
					case CPhysicsShape::HULL:
						stream.vRead(&Index, sizeof(int), 1);
						lpModel = dynamic_cast<CModelContainer*>(m_lpRenObj->GetlpContainer());
						if (lpModel && Index >= 0)
						{
							lpContainer = lpModel->GetlpMesh(Index);
						}

						if (m_PhysicsObjType == CPhysicsObj::STATIC)
							lpContainer->vSetupHullShapeDesc((CHullShapeDesc*)lpDesc, 0, LYNX_FALSE);
						else
							lpContainer->vSetupHullShapeDesc((CHullShapeDesc*)lpDesc, 0, LYNX_FALSE);
						break;

					case CPhysicsShape::TRIMESH:
						stream.vRead(&Index, sizeof(int), 1);
						lpModel = dynamic_cast<CModelContainer*>(m_lpRenObj->GetlpContainer());
						if (lpModel && Index >= 0)
						{
							lpContainer = lpModel->GetlpMesh(Index);
						}

						if (m_PhysicsObjType == CPhysicsObj::STATIC)
							lpContainer->vSetupTriMeshShapeDesc((CTriMeshShapeDesc*)lpDesc, 0, LYNX_FALSE);
						else
							lpContainer->vSetupTriMeshShapeDesc((CTriMeshShapeDesc*)lpDesc, 0, LYNX_FALSE);
						break;
				}
				lpShape = m_lpWorld->GetlpRootSpace()->vCreateShape(ShapeType, lpDesc, this);
				switch (ShapeType)
				{
					case CPhysicsShape::HULL:
						lpContainer->vReleaseHullShapeDesc((CHullShapeDesc*)lpDesc);
						lpShape->SetlpTargetContainer(lpContainer);
						break;

					case CPhysicsShape::TRIMESH:
						lpContainer->vReleaseTriMeshShapeDesc((CTriMeshShapeDesc*)lpDesc);
						lpShape->SetlpTargetContainer(lpContainer);
						break;
				}
				CShapeDesc* lpShapeDesc = (CShapeDesc*)lpDesc;

				vSetMass(m_MassValue);

				lpShape->vSetLocalTransformMatrix(lpShapeDesc->m_LocalM);								
				if (m_lpRenObj)
					vSetTransformMatrix(m_lpRenObj->GetTransformMatrix());
				else
					lpShape->vSetTransformMatrix(lpShapeDesc->m_TransformM);
			}

			return LYNX_TRUE;
		}
	}
}