#ifndef __LYNXSDKENUMSTRING_H__
#define __LYNXSDKENUMSTRING_H__

#include <LynxGraphics.h>
#include <LynxEngineClasses.h>

namespace LynxSDK
{
	namespace Graphics
	{
		namespace Enum_String
		{
			#define STRING_LENGTH				256
			typedef char						ENUM_STR[STRING_LENGTH];

			LYNXENGCLASS extern char			ZBModeList[LYNX_NUM_ZBUFFER_MODE][STRING_LENGTH];			
			LYNXENGCLASS extern LYNXINT			ZBModeValueList[LYNX_NUM_ZBUFFER_MODE];

			LYNXENGCLASS extern char			CullModeList[LYNX_NUM_CULL_MODE][STRING_LENGTH];			
			LYNXENGCLASS extern LYNXINT			CullModeValueList[LYNX_NUM_CULL_MODE];

			LYNXENGCLASS extern char			BlendFuncList[LYNX_NUM_BLEND_FUNC][STRING_LENGTH];			
			LYNXENGCLASS extern LYNXINT			BlendFuncValueList[LYNX_NUM_BLEND_FUNC]; 

			LYNXENGCLASS ENUM_STR*				GetPEmitterNosieTypeStr();
		}
	}
}

#endif