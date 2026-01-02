#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsSystem.h>
#include <LynxBtPhysicsWorld.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSystem::CPhysicsSystem(LynxEngine::CEngine* const lpengine)
		: LynxEngine::PhysicsSystem::CPhysicsSystem(lpengine)
		{			
			m_CollisionThreshold = 0.0f;
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
		LYNXBOOL CPhysicsSystem::vCreate(void)
		{			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsSystem::vInit(void)
		{
			CSystem::vInit();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsWorld* const CPhysicsSystem::vCreateWorld(void)
		{
			//GUARD(PhysicsSystem::vCreateWorld)		

			LynxEngine::PhysicsSystem::CPhysicsWorld* lpWorld = LYNXNEW CPhysicsWorld(this);

			if (lpWorld)
			{
				lpWorld->vCreate();
				AddWorld(lpWorld);
			}
			return lpWorld;

			//UNGUARD
			//catch (LynxEngine::CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
			//DEFAULTCATCH
		}
	}
}