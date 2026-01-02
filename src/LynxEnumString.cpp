#include <LynxString.h>
#include <LynxEnumString.h>
#include <LynxGraphics.h>

#ifdef __TOOL__

namespace LynxEngine
{
	ENUMDATA	LYNXDEPTHBUFFERMODE_EnumData[] = { {_T("Always"),			LYNX_DB_ALWAYS },
												   {_T("None"),				LYNX_DB_NONE },
												   {_T("Compare & Write"),	LYNX_DB_CW }, 
												   {_T("Write"),			LYNX_DB_W },
												   {_T("Compare"),			LYNX_DB_C} };

	ENUMDATA	LYNXCULLMODE_EnumData[] = { {_T("None"),		LYNX_CULL_NONE },
											{_T("Cull CCW"),	LYNX_CULL_CCW },
											{_T("Cull CW"),		LYNX_CULL_CW } }; 

	ENUMDATA	LYNXTEXTUREADDRESSMODE_EnumData[] = { {_T("Wrap"),		LYNX_ADDRESS_WRAP},		
													  {_T("Mirror"),	LYNX_ADDRESS_MIRROR},	
													  {_T("Clamp"),		LYNX_ADDRESS_CLAMP},		
													  {_T("Border"),	LYNX_ADDRESS_BORDER} };													   

	ENUMDATA	LYNXTEXTUREFILTERMODE_EnumData[] = { {_T("Point"),			LYNX_TF_POINT },
													 {_T("Linear"),			LYNX_TF_LINEAR },
													 {_T("Anisotropic"),	LYNX_TF_ANISOTROPIC },
													 {_T("PyramidalQuad"),	LYNX_TF_PYRAMIDALQUAD },
													 {_T("GaussianQuad"),	LYNX_TF_GAUSSIANQUAD} };

	ENUMDATA	LYNXCOMBINEFUNC_EnumData[] = { {_T("None"),									LYNX_CF_NONE},			
											   {_T("Select Arg0"),							LYNX_CF_SELECT_ARG0},
											   {_T("Select Arg1"),							LYNX_CF_SELECT_ARG1},
											   {_T("Modulate"),								LYNX_CF_MODULATE},		
											   {_T("Modulate 2X"),							LYNX_CF_MODULATE2X},	
											   {_T("Modulate 4X"),							LYNX_CF_MODULATE4X},	
											   {_T("Add"),									LYNX_CF_ADD},				
											   {_T("Add Signed"),							LYNX_CF_ADD_SIGNED},	
											   {_T("Add Signed X2"),						LYNX_CF_ADD_SIGNED2X},
											   {_T("Substract"),							LYNX_CF_SUBTRACT},		
											   {_T("Smooth"),								LYNX_CF_ADD_SMOOTH},	
											   {_T("Blend Diffuse Alpha"),					LYNX_CF_BLEND_DIFFUSE_ALPHA},
											   {_T("Blend Texture Alpha"),					LYNX_CF_BLEND_TEXTURE_ALPHA},
											   {_T("Blend Factor Alpha"),					LYNX_CF_BLEND_FACTOR_ALPHA},
											   {_T("Blend Texture Premultiplied Alpha"),	LYNX_CF_BLEND_TEXTURE_ALPHAPM},
											   {_T("Blend Current Alpha"),					LYNX_CF_BLEND_CURRENT_ALPHA},
											   {_T("Premodulate"),							LYNX_CF_BLEND_PREMODULATE},
											   {_T("Modulate Alpha Add Color"),				LYNX_CF_MODULATE_ALPHA_ADD_COLOR},
											   {_T("Modulate Color Add Alpha"),				LYNX_CF_MODULATE_COLOR_ADD_ALPHA},
											   {_T("Modulate Inverse Alpha Add Color"),		LYNX_CF_MODULATE_INV_ALPHA_ADD_COLOR},
											   {_T("Modulate Inverse Color Add Alpha"),		LYNX_CF_MODULATE_INV_COLOR_ADD_ALPHA},
											   {_T("Bump Environment Mapping"),				LYNX_CF_BUMP_ENVMAP},
											   {_T("Bump Environment Mapping Luminance"),	LYNX_CF_BUMPE_NVMAP_LUMINANCE},
											   {_T("Dot3"),									LYNX_CF_DOT3},		
											   {_T("Multiply Add"),							LYNX_CF_MULTIPLY_ADD},
											   {_T("Interpolate"),							LYNX_CF_INTERPOLATE},		
											   {_T("Dot3 RGBA"),							LYNX_CF_DOT3_RGBA}, };	

	ENUMDATA	TRANSPARENTTYPE_EnumData[] = { {_T("Opaque"),			MaterialSystem::CMaterial::OPAQUE_MATERIAL },
											   {_T("Transparent"),		MaterialSystem::CMaterial::TRANSPARENT_MATERIAL },
											   {_T("Translucent"),		MaterialSystem::CMaterial::TRANSLUCENT_MATERIAL } };
}

#endif