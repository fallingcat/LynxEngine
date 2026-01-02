#include <LynxEngine_PCH.h>
#include <LynxLuaScriptSystem.h>
			
#ifdef __LYNXSCRIPT_DLL__
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LynxEngine::ScriptSystem::CScriptSystem* CreateSystem(LynxEngine::CEngine* const lpengine)	
	{
		return (LYNXNEW LynxScript::Lua::CScriptSystem(lpengine));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL ReleaseSystem()	
	{	
		return LYNX_TRUE;
	}	
#else
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LynxEngine::ScriptSystem::CScriptSystem* CreateScriptSystem(LynxEngine::CEngine* const lpengine)	
	{
		return (LYNXNEW LynxScript::Lua::CScriptSystem(lpengine));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL ReleaseScriptSystem()	
	{	
		return LYNX_TRUE;
	}
#endif
