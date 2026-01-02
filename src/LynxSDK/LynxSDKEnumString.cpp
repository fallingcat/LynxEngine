#include <LynxSDK//LynxSDKEnumString.h>

namespace LynxSDK
{
	namespace Graphics
	{
		namespace Enum_String
		{
			char			ZBModeList[LYNX_NUM_ZBUFFER_MODE][STRING_LENGTH] = {"Always", "None", "Compare & Write", "Write", "Compare" };			
			//LYNXEXPORT LYNXINT	ZBModeValueList[LYNX_NUM_ZBUFFER_MODE] = {LYNX_ZB_ALWAYS, LYNX_ZB_NONE, LYNX_ZB_CW, LYNX_ZB_W, LYNX_ZB_C};

			char			CullModeList[LYNX_NUM_CULL_MODE][STRING_LENGTH] = {"None", "Cull CCW", "Cull CW"};			
			//LYNXEXPORT LYNXINT	CullModeValueList[LYNX_NUM_CULL_MODE] = {LYNX_CULL_NONE, LYNX_CULL_CCW, LYNX_CULL_CW};

			char			BlendFuncList[LYNX_NUM_BLEND_FUNC][STRING_LENGTH] = {"Zero", "One", 
																						"Source Color", "One Minus Source Color",
																						"Source Alpha", "One Minus Source Alpha", 
																						"Destination Alpha", "One Minus Destination Alpha",
																						"Destination Color", "One Minus Destination Color",
																						"Source Alpha Saturate", "Both Source Alpha",
																						"Both One Minus Source Alpha", "Blend Factor",
																						"One Minus Blend Factor"};			
			/*LYNXEXPORT LYNXINT	BlendFuncValueList[LYNX_NUM_BLEND_FUNC] = {LYNX_BFI_ZERO, 
																				LYNX_BFI_ONE, 
																				LYNX_BFI_SRC_COLOR, 
																				LYNX_BFI_ONE_MINUS_SRC_COLOR,
																				LYNX_BFI_SRC_ALPHA,
																				LYNX_BFI_ONE_MINUS_SRC_ALPHA,
																				LYNX_BFI_DEST_ALPHA,
																				LYNX_BFI_ONE_MINUS_DEST_ALPHA,
																				LYNX_BFI_DEST_COLOR,
																				LYNX_BFI_ONE_MINUS_DEST_COLOR,
																				LYNX_BFI_SRC_ALPHA_SAT,
																				LYNX_BFI_BOTH_SRC_ALPHA,
																				LYNX_BFI_BOTH_ONE_MINUS_SRC_ALPHA,
																				LYNX_BFI_BLEND_FACTOR,
																				LYNX_BFI_ONE_MINUS_BLEND_FACTOR};*/

			//LYNXEXPORT char			PEmitterNoiseTypeList[NUM_LYNX_PARTICLE_NOISE_TYPE][STRING_LENGTH] = {"None", "Random", "Zegma"};	
			//LYNXEXPORT ENUM_STR*	GetPEmitterNosieTypeStr() { return PEmitterNoiseTypeList;};
		}
	}
}