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
#include <PhysicsSystem/LynxPhysicsRagdoll.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRagdoll::CRagdoll(CPhysicsWorld* const lpw)		 
		{				
			m_lpWorld = lpw;
			m_lpContainer = NULL;
			m_PhyObjList.clear();
			m_PhyShapeList.clear();
			m_PhyJointList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRagdoll::~CRagdoll(void)
		{
			Release();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRagdoll::Release(void)
		{
			CList<CPhysicsObj*>::CIterator Obj;
			for (Obj=m_PhyObjList.begin(); Obj != m_PhyObjList.end(); ++Obj)
				m_lpWorld->GetlpEngine()->vDestroyPhysicsObj(*Obj);
			m_PhyObjList.clear();

			CList<CPhysicsShape*>::CIterator Shape;
			for (Shape = m_PhyShapeList.begin(); Shape != m_PhyShapeList.end(); ++Shape)
				m_lpWorld->GetlpEngine()->vDestroyPhysicsShape(*Shape);
			m_PhyShapeList.clear();

			CList<CPhysicsJoint*>::CIterator Joint;
			for (Joint = m_PhyJointList.begin(); Joint != m_PhyJointList.end(); ++Joint)
				m_lpWorld->GetlpEngine()->vDestroyPhysicsJoint(*Joint);
			m_PhyJointList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRagdoll::vCreate(void)
		{
			CModelContainer* lpModel = dynamic_cast<CModelContainer*>(m_lpContainer);
			if (lpModel)
			{	
				Release();

				PhysicsSystem::CSphereShapeDesc SphereDesc;		
				PhysicsSystem::CCapsuleShapeDesc Desc;				
				PhysicsSystem::CConeJointDesc JointDesc;
				PhysicsSystem::CPhysicsObj* lpObj;
				PhysicsSystem::CPhysicsObj* lpParentObj;
				PhysicsSystem::CPhysicsShape* lpShape;
				PhysicsSystem::CPhysicsJoint* lpJoint;
				Math::CMatrix4 TM, InverseM;
				float Len;
				Math::CReal Deg(90.0f);
				Math::MakeRotateMatrix(TM, Deg, LYNX_Z_AXIS);
				for (int i=0; i<lpModel->GetNumBones(); i++)				
				{	
					if (lpModel->GetlpBone(i)->GetNumChildren() > 0 && lpModel->GetlpBone(i)->GetlpParent() && lpModel->GetlpBone(i)->GetlpParent()->GetContainerType() == CContainer::BONE)
					{
						if (!lynxStrStr(lpModel->GetlpBone(i)->GetName().c_str(), _T("Finger")) &&
							!lynxStrStr(lpModel->GetlpBone(i)->GetName().c_str(), _T("Toe")) )
						{
							Math::CVector3 Pos0, Pos1, Dir, YAxis;
							lpModel->GetlpBone(i)->vGetPosition(Pos0);
							lpModel->GetlpBone(i)->GetlpChild(0)->vGetPosition(Pos1);
							Dir = Pos1 - Pos0;
							Len = Dir.Length(3) * 0.5f;
							if (lynxStrStr(lpModel->GetlpBone(i)->GetName().c_str(), _T("Head")))
							{
								SphereDesc.m_Radius = Len*0.8f;

								TM.m41 = Len;															
								Math::InverseMatrix(InverseM, TM);
								// The matrix to convert physics transform matrix to bone space
								((CBoneContainer*)lpModel->GetlpBone(i))->SetPhysics2ContainerM(InverseM);

								lpObj = ((CBoneContainer*)lpModel->GetlpBone(i))->CreatePhyObj(PhysicsSystem::CPhysicsObj::DYNAMIC, NULL);																				
								// Set the m_lpRenObj as the CModel's m_lpRenObj
								lpObj->SetlpRenObj(lpModel->GetlpRenObj());
								lpObj->SetlpTargetContainer(lpModel->GetlpBone(i));
								lpShape = m_lpWorld->GetlpEngine()->vCreatePhysicsShape(PhysicsSystem::CPhysicsShape::SPHERE, &SphereDesc, lpObj);								
								lpShape->vSetLocalTransformMatrix(TM);
								// Set the m_lpTargetContainer of the shape
								lpShape->SetlpTargetContainer(lpModel->GetlpBone(i));
								lpObj->SetName(lpModel->GetlpBone(i)->GetName()+CString(_T(".PhyObj")));
								lpShape->SetName(lpModel->GetlpBone(i)->GetName()+CString(_T(".PhyShape")));
								Math::CMatrix4 M = lpModel->GetlpBone(i)->GetTransformMatrix();
								M.Normalise();
								lpObj->vSetTransformMatrix(M);						
								lpObj->vSetMass(0.1f);

								AddObj(lpObj);
								AddShape(lpShape);
							}
							else
							{
								Desc.m_Radius = 0.5f;
								Desc.m_Length = Len - Desc.m_Radius;								
								if (Desc.m_Length <= 0.0f)
								{
									Desc.m_Length = 0.1f;
									Desc.m_Radius = 0.1f;
								}
								if (Desc.m_Length > 0.0f)
								{
									if (Desc.m_Length < Desc.m_Radius)
										Desc.m_Radius = Desc.m_Length * 0.7f;
									
									TM.m41 = Len;			
									Math::InverseMatrix(InverseM, TM);
									// The matrix to convert physics transform matrix to bone space
									((CBoneContainer*)lpModel->GetlpBone(i))->SetPhysics2ContainerM(InverseM);

									lpObj = ((CBoneContainer*)lpModel->GetlpBone(i))->CreatePhyObj(PhysicsSystem::CPhysicsObj::DYNAMIC, NULL);																					
									// Set the m_lpRenObj as the CModel's m_lpRenObj
									lpObj->SetlpRenObj(lpModel->GetlpRenObj());
									lpObj->SetlpTargetContainer(lpModel->GetlpBone(i));
									lpShape = m_lpWorld->GetlpEngine()->vCreatePhysicsShape(PhysicsSystem::CPhysicsShape::CAPSULE, &Desc, lpObj);									
									lpShape->vSetLocalTransformMatrix(TM);
									// Set the m_lpTargetContainer of the shape
									lpShape->SetlpTargetContainer(lpModel->GetlpBone(i));
									lpObj->SetName(lpModel->GetlpBone(i)->GetName()+CString(_T(".PhyObj")));
									lpShape->SetName(lpModel->GetlpBone(i)->GetName()+CString(_T(".PhyShape")));
									Math::CMatrix4 M = lpModel->GetlpBone(i)->GetTransformMatrix();
									M.Normalise();
									lpObj->vSetTransformMatrix(M);
									lpObj->vSetMass(0.1f);

									AddObj(lpObj);
									AddShape(lpShape);
								}
							}
						}
					}
				}

				for (int i=0; i<lpModel->GetNumBones(); i++)				
				{	
					if (lpModel->GetlpBone(i)->GetlpParent() && lpModel->GetlpBone(i)->GetlpParent()->GetContainerType() == CContainer::BONE)
					{
						lpObj = ((CBoneContainer*)(lpModel->GetlpBone(i)))->GetlpPhyObj();
						lpParentObj = ((CBoneContainer*)(lpModel->GetlpBone(i)->GetlpParent()))->GetlpPhyObj();

						if (lpObj && lpParentObj)
						{
							JointDesc.m_bCollisionBetweenObj = LYNX_FALSE;
							JointDesc.m_SwingLimit = 0.0f;
							JointDesc.m_TwistLimitLow = 0.0f;
							JointDesc.m_TwistLimitHigh = 0.0f;
							lpJoint = m_lpWorld->GetlpEngine()->vCreatePhysicsJoint(PhysicsSystem::CPhysicsJoint::CONE, &JointDesc);
							lpJoint->SetName(lpModel->GetlpBone(i)->GetName() + CString(_T(".PhyJoint")));
							lpJoint->vAttach(lpParentObj, lpObj);
							Math::CVector3 Pos;
							lpObj->vGetPosition(Pos);							
							//lpJoint->vSetAnchorPosition(Pos);

							AddJoint(lpJoint);
						}
					}
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRagdoll::SetupFromContainer(void)
		{
			CModelContainer* lpModel = dynamic_cast<CModelContainer*>(m_lpContainer);
			if (lpModel)
			{	
				PhysicsSystem::CPhysicsObj* lpObj;
				PhysicsSystem::CPhysicsObj* lpObj2;
				PhysicsSystem::CPhysicsShape* lpShape;
				PhysicsSystem::CPhysicsJoint* lpJoint;
				Math::CMatrix4 TM, InverseM;
				float Len;
				Math::CReal Deg(90.0f);
				Math::MakeRotateMatrix(TM, Deg, LYNX_Z_AXIS);
				for (int i=0; i<lpModel->GetNumBones(); i++)				
				{	
					if (((CBoneContainer*)lpModel->GetlpBone(i))->GetlpPhyObj())
					{
						Math::CMatrix4 M = lpModel->GetlpBone(i)->GetTransformMatrix();
						M.Normalise();
						//lpObj->vSetTransformMatrix(M);												
					}
				}

				//for (int i=0; i<lpModel->GetNumBones(); i++)				
				//{	
				//	if (lpModel->GetlpBone(i)->GetlpParent() && lpModel->GetlpBone(i)->GetlpParent()->GetContainerType() == CContainer::BONE)
				//	{
				//		lpObj = ((CBoneContainer*)(lpModel->GetlpBone(i)))->GetlpPhyObj();
				//		lpObj2 = ((CBoneContainer*)(lpModel->GetlpBone(i)->GetlpParent()))->GetlpPhyObj();

				//		if (lpObj && lpObj2)
				//		{
				//			JointDesc.m_bCollisionBetweenObj = LYNX_FALSE;
				//			JointDesc.m_SwingLimit = 0.0f;
				//			JointDesc.m_TwistLimitLow = 0.0f;
				//			JointDesc.m_TwistLimitHigh = 0.0f;
				//			lpJoint = m_lpWorld->GetlpEngine()->vCreatePhysicsJoint(PhysicsSystem::CPhysicsJoint::CONE, &JointDesc);
				//			lpJoint->SetName(lpObj->GetName()+CString(_T(".PhyJoint")));
				//			lpJoint->vAttach(lpObj2, lpObj);
				//			Math::CVector3 Pos;
				//			lpObj->vvGetPosition(Pos);							
				//			//lpJoint->vSetAnchorPosition(Pos);

				//			m_PhyJointList.push_back(lpJoint);
				//		}
				//	}
				//}
			}
		}		
	}
}