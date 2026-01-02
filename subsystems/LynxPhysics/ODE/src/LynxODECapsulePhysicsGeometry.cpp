#include <LynxODECapsulePhysicsGeometry.h>
#include <LynxODEPhysicsSpace.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsulePhyGeo::CCapsulePhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsGeometry(lps)
		{	
			m_Type = CAPSULE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsulePhyGeo::~CCapsulePhyGeo(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCapsulePhyGeo::vCreate(LYNXREAL *lpdata)
		{			
			m_TransGeometry = dCreateGeomTransform(((CPhysicsSpace*)m_lpSpace)->GetSpace()); 
			m_Geometry = dCreateCCylinder(0, lpdata[0], lpdata[1]);				
			dGeomTransformSetGeom(m_TransGeometry, m_Geometry); 
			dGeomTransformSetInfo(m_TransGeometry, 1);			

			return LYNX_TRUE;
		}	
	}
}
