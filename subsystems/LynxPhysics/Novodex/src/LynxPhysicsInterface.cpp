#include <LynxEngine_PCH.h>
#include <LynxNxPhysicsSystem.h>
#include <LynxDebugMemAllocator.h>

LynxPhysics::Novodex::CPhysicsSystem*	glpSys = NULL;

//#ifdef __LYNXENGINE_DLL__
#if (0)
	LynxEngine::CMemAllocator*				glpMemAllocator = NULL;
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LynxEngine::PhysicsSystem::CPhysicsSystem* CreateSystem(LynxEngine::CEngine* const lpengine)	
	{
		//glpMemAllocator = lpengine->
		glpMemAllocator = LYNXCORE_NEW LynxEngine::CDebugMemAllocator(lpengine);
		glpSys = LYNXNEW LynxPhysics::Novodex::CPhysicsSystem(lpengine); 
		
		return glpSys;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL ReleaseSystem()	
	{	
		LYNXCORE_DEL glpMemAllocator;
		LYNXDEL glpSys;

		return LYNX_TRUE;
	}
#else
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LynxEngine::PhysicsSystem::CPhysicsSystem* CreatePhysicsSystem(LynxEngine::CEngine* const lpengine)	
	{
		glpSys = LYNXNEW LynxPhysics::Novodex::CPhysicsSystem(lpengine); 
		return glpSys;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL ReleasePhysicsSystem()	
	{	
		LYNXDEL glpSys;
		return LYNX_TRUE;
	}
#endif