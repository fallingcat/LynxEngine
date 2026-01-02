#include <LynxODEBoxPhysicsGeometry.h>
#include <LynxODEPhysicsSpace.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxPhyGeo::CBoxPhyGeo(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsGeometry(lps)
		{	
			m_Type = BOX;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxPhyGeo::~CBoxPhyGeo(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBoxPhyGeo::vCreate(LYNXREAL *lpdata)
		{			
			m_TransGeometry = dCreateGeomTransform(((CPhysicsSpace*)m_lpSpace)->GetSpace()); 
			dGeomSetPosition(m_TransGeometry, 0.0f, 0.0f, 0.0f);
			m_Geometry = dCreateBox(0, lpdata[0], lpdata[1], lpdata[2]);			
			dGeomTransformSetGeom(m_TransGeometry, m_Geometry); 
			dGeomTransformSetInfo(m_TransGeometry, 1);			

			return LYNX_TRUE;
		}	
	}
}