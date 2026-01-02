#include <LynxODEPhysicsGeometry.h>
#include <LynxODEPhysicsSpace.h>
#include <LynxODEPhysicsObj.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsGeometry::CPhysicsGeometry(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: LynxEngine::PhysicsSystem::CPhysicsGeometry(lps)
		{	
			m_TransGeometry =0;
			m_Geometry = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsGeometry::~CPhysicsGeometry(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsGeometry::vDestory() 
		{ 
			LYNX_ASSERT(m_Geometry > 0); 
			dGeomDestroy(m_Geometry); 
			LYNX_ASSERT(m_TransGeometry > 0); 
			dGeomDestroy(m_TransGeometry); 			
			m_lpObj = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsGeometry::vSetObj(LynxEngine::PhysicsSystem::CPhysicsObj* const lpo) 
		{
			if (lpo)			
			{
				SetBody(&(((CPhysicsObj *)lpo)->GetBody())); 							
				m_lpObj = lpo;
			}
			else
			{
				SetBody(NULL); 			
				m_lpObj = NULL;
			}
		}
	}
}