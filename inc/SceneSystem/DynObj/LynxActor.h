#ifndef __LYNXACTOR_H__
#define __LYNXACTOR_H__

#include <SceneSystem/DynObj/LynxDynObj.h>

namespace LynxEngine
{
	namespace SceneSystem
	{
		class CActor : public CDynamicObj
		{
		public:
		protected:
		public:
			CActor(CScene* const s);
			~CActor();
		};
	}
}

#endif

