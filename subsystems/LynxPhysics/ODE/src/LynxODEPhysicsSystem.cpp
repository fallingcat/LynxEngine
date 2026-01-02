#include <LynxODEPhysicsSystem.h>
#include <LynxODEPhysicsWorld.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSystem::CPhysicsSystem(void)
		{	
			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSystem::~CPhysicsSystem(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsWorld* const CPhysicsSystem::vCreateWorld(void)
		{
			LynxEngine::PhysicsSystem::CPhysicsWorld* lpWorld = LYNXNEW CPhysicsWorld(this);

			if (lpWorld)
			{
				lpWorld->vCreate();
				AddWorld(lpWorld);
			}
			return lpWorld;
		}
	}
}