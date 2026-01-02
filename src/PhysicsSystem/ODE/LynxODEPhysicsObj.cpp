#ifdef __ODE_PHY_ENGINE__

#include <PhysicsSystem/ODE/LynxODEPhysicsObj.h>
#include <PhysicsSystem/ODE/LynxODEPhysicsWorld.h>
#include <PhysicsSystem/ODE/LynxODEBoxPhysicsGeometry.h>
#include <PhysicsSystem/ODE/LynxODESpherePhysicsGeometry.h>
#include <PhysicsSystem/ODE/LynxODECapsulePhysicsGeometry.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CODEPhyObj::CODEPhyObj(void)
		{			
			m_Body = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CODEPhyObj::~CODEPhyObj(void)
		{
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CODEPhyObj::vCreate(CPhysicsWorld *lpw)
		{
			CODEPhyWorld*				lpWorld;
			LYNXVECTOR3D				Pos;
			LYNXMATRIX					M;

			m_lpWorld = lpw;
			lpWorld = (CODEPhyWorld*)m_lpWorld;
			m_Body = dBodyCreate(lpWorld->GetWorld());

			if (m_Body <= 0)
				return LYNX_FALSE;

			m_MassValue = 1.0f;			
			m_Density = 1.0f;
			dMassSetSphere(&m_Mass, m_Density, 1.0f);			
			dBodySetMass(m_Body, &m_Mass);
			dMassTranslate(&m_Mass, m_CMM.s.m41, m_CMM.s.m42, m_CMM.s.m43);					
			dBodyEnable(m_Body);
			lynxMemCpy(&M, &(GetTransformMatrix()), sizeof(LYNXMATRIX));

			dMatrix3 Mat; 
			MATRIX_2_PHYSICS_MAT3(Mat, M)
			dBodySetRotation(m_Body, Mat);
			dBodySetPosition(m_Body, M.s.m41, M.s.m42, M.s.m43);	
			lpw->AddObj(this);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vDestory()
		{
			LYNX_ASSERT(m_Body > 0);

			dBodyDestroy(m_Body);
			m_lpWorld->DeleteObj(this);
			m_lpWorld = NULL;
			vDeleteAllPhyGeos();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata)
		{	
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
		void CODEPhyObj::vAddPhyGeo(CPhysicsGeometry *lpg)
		{
			m_GeometryList.push_back(lpg);
			((CODEPhyGeometry*)lpg)->SetBody(&m_Body);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CODEPhyObj::vAddPhyGeo(CPhysicsSpace* s, CPhysicsGeometry::TYPE type, LYNXREAL *lpdata)
		{
			CODEBoxPhyGeo *lpBoxGeo;
			CODESpherePhyGeo *lpSphereGeo;
			CODECapsulePhyGeo *lpCapsuleGeo;

			switch (type)
			{
				case BOX:
					lpBoxGeo = LYNXNEW CODEBoxPhyGeo();
					if (!lpBoxGeo->vCreate(s, lpdata))
						return LYNX_FALSE;
					vAddPhyGeo(lpBoxGeo);
					break;

				case SPHERE:
					lpSphereGeo = LYNXNEW CODESpherePhyGeo();
					if (!lpSphereGeo->vCreate(s, lpdata))
						return LYNX_FALSE;
					vAddPhyGeo(lpSphereGeo);
					break;

				case CAPSULE:
					lpCapsuleGeo = LYNXNEW CODECapsulePhyGeo();
					if (!lpCapsuleGeo->vCreate(s, lpdata))
						return LYNX_FALSE;
					vAddPhyGeo(lpCapsuleGeo);
					break;
			}						
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vRemovePhyGeo(CPhysicsGeometry *lpg)
		{			
			m_GeometryList.remove(lpg);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vRemovePhyGeo(int i)
		{
			CPhysicsGeometry *lpG = GetlpGeometry(i);
			if (lpG)
				vRemovePhyGeo(lpG);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vRemoveAllPhyGeos()
		{
			CIterator<CPhysicsGeometry>		*lpGeo;

			lpGeo = m_GeometryList.begin();
			for (int i=0; i<m_GeometryList.size(); i++)
			{				
				vRemovePhyGeo(lpGeo->GetlpObj());
				lpGeo = lpGeo->GetNext();				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vDeletePhyGeo(CPhysicsGeometry *lpg)
		{
			lpg->vDestory();
			m_GeometryList.remove(lpg);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vDeletePhyGeo(int i)
		{
			CPhysicsGeometry *lpG = GetlpGeometry(i);
			if (lpG)
				vDeletePhyGeo(lpG);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vDeleteAllPhyGeos()
		{
			CIterator<CPhysicsGeometry>		*lpGeo;

			lpGeo = m_GeometryList.begin();
			for (int i=0; i<m_GeometryList.size(); i++)
			{				
				vDeletePhyGeo(lpGeo->GetlpObj());
				lpGeo = lpGeo->GetNext();				
			}
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vUpdate()
		{
			const dReal			*lpPos, *lpTransM;		

			lpTransM = dBodyGetRotation(m_Body);
			lpPos = dBodyGetPosition(m_Body);			
				
			PHYSICS_MAT3_2_MATRIX(m_PhyM, (lpTransM));
			m_PhyM.s.m41 = lpPos[0];  m_PhyM.s.m42 = lpPos[1];  m_PhyM.s.m43 = lpPos[2]; 
			/*
			PHYSICS_MAT3_2_MATRIX(m_lpContainer->vGetlpObj3D()->LocalM, (lpTransM));
			m_lpContainer->vGetlpObj3D()->LocalM.s.m41 = lpPos[0];
			m_lpContainer->vGetlpObj3D()->LocalM.s.m42 = lpPos[1];
			m_lpContainer->vGetlpObj3D()->LocalM.s.m43 = lpPos[2];																	
			*/
			PHYSICS_MAT3_2_MATRIX(m_lpContainer->vGetlpObj3D()->GlobalM, (lpTransM));
			m_lpContainer->vGetlpObj3D()->GlobalM.s.m41 = lpPos[0];
			m_lpContainer->vGetlpObj3D()->GlobalM.s.m42 = lpPos[1];
			m_lpContainer->vGetlpObj3D()->GlobalM.s.m43 = lpPos[2];																	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) 
		{ 
			dBodySetPosition(m_Body, x, y, z); 

			CIterator<CPhysicsGeometry> *lpGeo = m_GeometryList.begin();
			for (int i=0; i<m_GeometryList.size(); i++)
			{				
				lpGeo->GetlpObj()->vSetPosition(x, y, z);
				lpGeo = lpGeo->GetNext();				
			}			
			CRenderableObj::SetPosition(x, y, z); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vSetPosition(LPLYNXVECTOR3D lppos) 
		{ 
			dBodySetPosition(m_Body, lppos->s.x, lppos->s.y, lppos->s.z); 

			CIterator<CPhysicsGeometry> *lpGeo = m_GeometryList.begin();
			for (int i=0; i<m_GeometryList.size(); i++)
			{				
				lpGeo->GetlpObj()->vSetPosition(lppos);
				lpGeo = lpGeo->GetNext();				
			}			
			CRenderableObj::SetPosition(lppos); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CODEPhyObj::vSetTransformMatrix(const LYNXMATRIX& m) 
		{ 
			dMatrix3 mat; 
			MATRIX_2_PHYSICS_MAT3(mat, (m)); 
			dBodySetRotation(m_Body, mat); 
			dBodySetPosition(m_Body, m.s.m41, m.s.m42, m.s.m43); 

			CIterator<CPhysicsGeometry> *lpGeo = m_GeometryList.begin();
			for (int i=0; i<m_GeometryList.size(); i++)
			{				
				lpGeo->GetlpObj()->vSetTransformMatrix(m); 
				lpGeo = lpGeo->GetNext();				
			}
			CRenderableObj::SetTransformMatrix(m); 
		}
	}
}

#endif