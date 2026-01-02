#ifndef __LYNXSCRIPT_SCRIPTSYSTEM_H__
#define __LYNXSCRIPT_SCRIPTSYSTEM_H__

#include <ScriptSystem/LynxScriptSystem.h>

#ifdef __LYNXSCRIPT_LIB__
	#define	LYNXSCRIPTCLASS						
#elif __LYNXSCRIPT_DLL__
	#define	LYNXSCRIPTCLASS						LYNXEXPORT
#else
	#define LYNXSCRIPTCLASS						LYNXIMPORT
#endif	

#ifdef __LYNXSCRIPT_LIB__
	#define	LYNXSCRIPTAPI						
#elif __LYNXSCRIPT_DLL__
	#define	LYNXSCRIPTAPI						LYNXEXPORT
#else
	#define LYNXSCRIPTAPI						LYNXIMPORT
#endif	

namespace LynxScript 
{
	class /*LYNXSCRIPTCLASS*/ CScriptSystem : public LynxEngine::ScriptSystem::CScriptSystem
	{
	protected:						
	public:
		DECLARE_CLASS(CScriptSystem);
		CScriptSystem() {};
		virtual ~CScriptSystem() {};
	};
}

#endif