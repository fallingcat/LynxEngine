#include <LynxEngine_PCH.h>

#include <LynxEngine.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/DynObj/LynxActor.h>

namespace LynxEngine
{
	namespace SceneSystem
	{
		CActor::CActor(CScene* const s)
		: CDynamicObj(s)
		{		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CActor::~CActor()
		{		
		}
	}
}
