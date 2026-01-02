#include <LynxODEPhysicsWorld.h>
#include <LynxODEPhysicsSpace.h>
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
		CPhysicsSpace::CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld* const lpw)
		: LynxEngine::PhysicsSystem::CPhysicsSpace(lpw)
		{
			m_Space = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::~CPhysicsSpace(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsSpace::vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent)
		{
			if (parent)
				m_Space = dHashSpaceCreate(((CPhysicsSpace *)parent)->m_Space);
			else
				m_Space = dHashSpaceCreate(NULL);

			/*
			dVector3			Center, Ext;

			Center[0] = 0.0f;
			Center[1] = 0.0f;
			Center[2] = 0.0f;
			Ext[0] = 4000.0f;
			Ext[1] = 4000.0f;
			Ext[2] = 4000.0f;
			if (parent)
				m_Space = dQuadTreeSpaceCreate(((CODEPhySpace *)parent)->m_Space, &Center, &Ext, 30);//dSimpleSpaceCreate(lpparentsp->Space);//dHashSpaceCreate(lpparentsp->Space);
			else
				m_Space = dQuadTreeSpaceCreate(NULL, &Center, &Ext, 30);//dSimpleSpaceCreate(0);//dHashSpaceCreate(0);
			*/

			dCreatePlane(m_Space, 0.0f, 1.0f, 0.0f, 0.0f);	
			dCreatePlane(m_Space, 0.0f, -1.0f, 0.0f, -393.0f);	

			dCreatePlane(m_Space, -1.0f, 0.0f, 0.0f, -208.0f);	
			dCreatePlane(m_Space, 1.0f, 0.0f, 0.0f, -184.0f);
			dCreatePlane(m_Space, 0.0f, 0.0f, -1.0f, -176.0f);
			dCreatePlane(m_Space, 0.0f, 0.0f, 1.0f, -216.0f);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSpace::vDestory()
		{
			//dBodyDestroy(m_Body);			
			//m_lpWorld = NULL;
			//vDeleteAllPhyGeos();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsGeometry* const CPhysicsSpace::vCreateGeometry(LynxEngine::PhysicsSystem::CPhysicsGeometry::TYPE type, LYNXREAL *lpdata)
		{
			LynxEngine::PhysicsSystem::CPhysicsGeometry *lpGeo;
			
			switch (type)
			{
				case LynxEngine::PhysicsSystem::CPhysicsGeometry::BOX:
					lpGeo = LYNXNEW CBoxPhyGeo(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsGeometry::SPHERE:
					lpGeo = LYNXNEW CSpherePhyGeo(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsGeometry::CAPSULE:
					lpGeo = LYNXNEW CCapsulePhyGeo(this);					
					break;
			}						
			if (!lpGeo->vCreate(lpdata))
				return LYNX_FALSE;

			AddGeo(lpGeo);

			return lpGeo;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSpace::vDeleteGeometry(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg)
		{
			lpg->vDestory();
			DeleteGeo(lpg);
			delete lpg;
		}
	}
}