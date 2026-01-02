#include <LynxODESpherePhysicsGeometry.h>
#include <LynxODEPhysicsSpace.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpherePhyGeo::CSpherePhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsGeometry(lps)
		{	
			m_Type = SPHERE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSpherePhyGeo::~CSpherePhyGeo(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSpherePhyGeo::vCreate(LYNXREAL *lpdata)
		{			
			m_TransGeometry = dCreateGeomTransform(((CPhysicsSpace*)m_lpSpace)->GetSpace()); 
			m_Geometry = dCreateSphere(0, lpdata[0]);	
			dGeomTransformSetGeom(m_TransGeometry, m_Geometry); 
			dGeomTransformSetInfo(m_TransGeometry, 1);			

			return LYNX_TRUE;
		}	
	}
}