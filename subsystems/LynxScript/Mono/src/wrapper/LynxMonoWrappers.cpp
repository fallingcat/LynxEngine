#include <LynxEngine_PCH.h>

#ifdef __MONO__

#include <wrapper/LynxMonoWrappers.h>
#include <LynxMonoVMachine.h>
#include <LynxObj.h>
#include <LynxSystem.h>
#include <LynxRenderableObj.h>
#include <GameFramework/LynxUIPage.h>

namespace LynxEngine
{
	namespace MonoWrapper
	{
		MonoImage*	gImage = NULL;
		MonoDomain* gDomain = NULL;
	}
}

#endif
