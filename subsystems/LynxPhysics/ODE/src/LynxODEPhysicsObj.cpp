#include <LynxODEPhysicsObj.h>
#include <LynxODEPhysicsWorld.h>
#include <LynxODEPhysicsGeometry.h>
#include <LynxODEBoxPhysicsGeometry.h>
#include <LynxODESpherePhysicsGeometry.h>
#include <LynxODECapsulePhysicsGeometry.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld* const lpw)
		: LynxEngine::PhysicsSystem::CPhysicsObj(lpw)
		{			
			m_Body = 0;			
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
		LYNXBOOL CPhysicsObj::vCreate()
		{
			CPhysicsWorld*				lpWorld;			
			LYNXREAL					R;

			lpWorld = (CPhysicsWorld*)m_lpWorld;
			m_Body = dBodyCreate(lpWorld->GetWorld());

			if (m_Body <= 0)
				return LYNX_FALSE;

			m_MassValue = 1.0f;	
			R = 1.0f;
			vSetMass(SPHERE, 1.0f, &R);			
			dMassTranslate(&m_Mass, m_CMM.s.m41, m_CMM.s.m42, m_CMM.s.m43);					
			dBodyEnable(m_Body);			
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vDestory()
		{
			LYNX_ASSERT(m_Body > 0);

			dBodyDestroy(m_Body);			
			m_lpWorld = NULL;
			vRemoveAllPhyGeos();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata)
		{	
			m_MassType = t;
			m_Density = density;
			switch (t)
			{
				case NULL_MASS:
					dMassSetZero(&m_Mass);
					break;

				case BOX:
					dMassSetBox(&m_Mass, m_Density, lpdata[0], lpdata[1], lpdata[2]);	
					break;

				case SPHERE:
					dMassSetSphere(&m_Mass, m_Density, lpdata[0]);	
					break;

				case CAPSULE:
					dMassSetCappedCylinder(&m_Mass, m_Density, 3, lpdata[0], lpdata[1]);
					break;

				case CYLINDER:
					dMassSetCylinder(&m_Mass, m_Density, 3, lpdata[0], lpdata[1]); 
					break;
			}			
			dBodySetMass(m_Body, &m_Mass);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vAddPhyGeo(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg)
		{
			LYNXVECTOR3D Pos;

			m_GeometryList.push_back(lpg);
			vGetPosition(&Pos);
			lpg->vSetPosition(0.0f, 0.0f, 0.0f);			
			((CPhysicsGeometry*)lpg)->SetBody(&m_Body);						
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemovePhyGeo(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg)
		{			
			((CPhysicsGeometry*)lpg)->SetBody(NULL);						
			m_GeometryList.remove(lpg);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemovePhyGeo(int i)
		{
			LynxEngine::PhysicsSystem::CPhysicsGeometry *lpG = GetlpGeometry(i);
			if (lpG)
				vRemovePhyGeo(lpG);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveAllPhyGeos()
		{
			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsGeometry*>::CIterator Geo = m_GeometryList.begin(); Geo != m_GeometryList.end(); ++Geo)
			{				
				vRemovePhyGeo((*Geo));				
			}
		}												
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetTransformMatrix(const LYNXMATRIX& m) 
		{ 
			dMatrix3 mat; 
			MATRIX_2_PHYSICS_MAT3(mat, (m)); 
			dBodySetRotation(m_Body, mat); 
			dBodySetPosition(m_Body, m.s.m41, m.s.m42, m.s.m43); 

			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsGeometry*>::CIterator Geo = m_GeometryList.begin(); Geo != m_GeometryList.end(); ++Geo)
			{				
				(*Geo)->vSetTransformMatrix(m); 				
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsObj::vGetTransformMatrix()
		{
			const dReal *lpPos, *lpTransM;		

			lpTransM = dBodyGetRotation(m_Body);
			lpPos = dBodyGetPosition(m_Body);			
				
			PHYSICS_MAT3_2_MATRIX(m_PhyM, (lpTransM));
			m_PhyM.s.m41 = lpPos[0];  m_PhyM.s.m42 = lpPos[1];  m_PhyM.s.m43 = lpPos[2]; 

			return m_PhyM;
		}		
	}
}