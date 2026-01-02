//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/07/04
//  Last Update : 
//
//  說明:
//
//      LynxGraphic負責任何與API相關的實際動作,使得GameSystem的使用
//      者可以在不同API間以相同方式工作 
//
//###########################################################################

#ifndef __LYNXRENDERER_H__
#define __LYNXRENDERER_H__

#include <LynxMathWrap.h>
#include <LynxResource.h>
#include <LynxLogger.h>
#include <LynxEngineClasses.h>

using namespace LynxEngine::Math;

#ifdef __PROFILE__
	#define RESET_COUNTER(c)				c = 0L;
	#define START_COUNTER					LYNXLONG T0 = lynxOSGetPerformanceCounter();
	#define STOP_COUNTER(c)					c += lynxOSGetPerformanceCounter() - T0;
	#define INC_COUNTER(c, s)				c+=s;	
#else
	#define RESET_COUNTER(c)
	#define START_COUNTER
	#define STOP_COUNTER(c)
	#define INC_COUNTER(c, s)
#endif
#define RESET_ALL_COUNTERS					RESET_COUNTER(m_SetRenderStateTimeCounter)\
											RESET_COUNTER(m_SetRenderStateCounter)\
											RESET_COUNTER(m_DrawTimeCounter)\
											RESET_COUNTER(m_DrawCallCounter)\
											RESET_COUNTER(m_PrimitiveCounter)\
											RESET_COUNTER(m_SetTextureCounter)\
											RESET_COUNTER(m_SetShaderCounter)\
											RESET_COUNTER(m_SetShaderConstantCounter)\
											RESET_COUNTER(m_BufferMapCounter)

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		void										LogGLError(const CString& message);

		#ifdef _DEBUG
			#define OGLES2_LogError(c)								LogGLError(c);
			#define GL_TRACE(c)										c; LogGLError("[Error] GL error!!");
		#else
			#define OGLES2_LogError(c)							
			#define GL_TRACE(c)										c;
		#endif

		class LYNXENGCLASS CRenderer : public CObj
		{			
			LYNX_DECLARE_CLASS(CRenderer);
		public:			
			static const int MAX_NUM_MRT					= 8;
			static const int MAX_NUM_TEXTURE_CHANNEL		= 16;

			static const int STATIC							= 0x1;
			static const int DYNAMIC						= 0x2;
			static const int STAGING						= 0x4;
			static const int WRITEONLY						= 0x8;
			static const int CPU_WRITE						= 0x10;
			static const int CPU_READ						= 0x20;

			static const int RENDER_BUFFER					= 0x1;
			static const int DEPTH_BUFFER					= 0x2;
			static const int STENCIL_BUFFER					= 0x4;
			static const int CONSTANT_BUFFER				= 0x8;
			static const int STREAM_BUFFER					= 0x10;
			static const int UNORDERED_BUFFER				= 0x20;

			static const int SHADER_DEBUG_NONE				= 0;
			static const int SHADER_DEBUG_VS				= 1;
			static const int SHADER_DEBUG_PS				= 2;

			static const int ZERO_CHANNEL					= 0x0;	
			static const int RED_CHANNEL					= 0x1;
			static const int GREEN_CHANNEL					= 0x2;
			static const int BLUE_CHANNEL					= 0x4;
			static const int ALPHA_CHANNEL					= 0x8;
			static const int ALL_CHANNELS					= RED_CHANNEL|GREEN_CHANNEL|BLUE_CHANNEL|ALPHA_CHANNEL;

			typedef DWORD							FIXEDVERTEXLAYOUT;

			typedef enum {
				VOID_RENDERER = -1,
				NULL_RENDERER = 0,
				D3D9_RENDERER,
				D3D10_RENDERER,
				D3D11_RENDERER,
				OGL_RENDERER,
				OGLES_RENDERER,
				OGLES2_RENDERER,
				OGLES3_RENDERER,
				NUM_RENDERERTYPE,
			}RENDERERTYPE;	
			
			typedef enum {
				RDT_NULL = 0,
				RDT_REFERNCE,
				RDT_SOFTWARE,
				RDT_HARDWARE,
				RDT_DEFAULT,
				NUM_RENDERDEVICETYPE,
			}RENDERDEVICETYPE;

			typedef struct {
				DWORD					MaxVertexShaderConstants;
				DWORD					MaxTextureWidth;
				DWORD					MaxTextureHeight;
				DWORD					MaxTextureChannels;	
				DWORD					MaxRenderBuffers;
				DWORD					MaxVideoMemory;
			}CAPS;

			typedef enum {
				QT_OCCLUSION = 0,
				QT_OCCLUSION_PREDICATE,
				NUM_QUERYTYPE,
			}QUERYTYPE;

			typedef enum {
				TT_2D_TEXTURE = 0,
				TT_3D_TEXTURE,
				TT_CUBE_TEXTURE,
				TT_2D_TEXTURE_ARRAY,
				TT_3D_TEXTURE_ARRAY,
				TT_CUBE_TEXTURE_ARRAY,
				NUM_TEXTURETYPE,
			}TEXTURETYPE;

			typedef enum{
				LOCK_TO_READ,					/*!< Lock 後讀取。 */  	
				LOCK_TO_WRITE,					/*!< Lock 後寫入。 */  	
				LOCK_TO_WRITE_DISCARD_OLD,		/*!< Lock 後寫入。 */  	
				LOCK_TO_WRITE_NO_OVERWRITE,		/*!< Lock 後寫入。 */  	
				LOCK_TO_BOTH,					/*!< Lock 後讀取及寫入。 */  	
				NUM_LOCKMODE,		
			}LOCKMODE;			

			typedef enum {				
				SHADER_TARGET_11,
				SHADER_TARGET_20,
				SHADER_TARGET_2X,
				SHADER_TARGET_30,
				SHADER_TARGET_40,
				SHADER_TARGET_41,
				SHADER_TARGET_50,
				NUM_SHADERTARGET,
			}SHADERTARGET;

			typedef enum {				
				FEATURE_D3D11,
				FEATURE_D3D10,
				FEATURE_D3D10_1,
				FEATURE_D3D9_3,
				FEATURE_OGL2,
				FEATURE_OGL3,
				FEATURE_OGL4,
				NUM_RENDERERFEATURE,
			}RENDERERFEATURE;

			typedef enum{
				FORMAT_NULL = -1,
				FORMAT_R8G8B8,
				FORMAT_A8R8G8B8,          
				FORMAT_X8R8G8B8,				
				FORMAT_R5G6B5,             
				FORMAT_X1R5G5B5,           
				FORMAT_A1R5G5B5,           
				FORMAT_A4R4G4B4,           
				FORMAT_R3G3B2,             
				FORMAT_A8,                 
				FORMAT_A8R3G3B2,           
				FORMAT_X4R4G4B4,           
				FORMAT_A2B10G10R10,        
				FORMAT_A8B8G8R8,           
				FORMAT_X8B8G8R8,           
				FORMAT_G16R16,             
				FORMAT_A2R10G10B10,        
				FORMAT_A16B16G16R16,       
				FORMAT_A8P8,               
				FORMAT_P8,                 

				FORMAT_L8,                 
				FORMAT_A8L8,               
				FORMAT_A4L4,               

				FORMAT_V8U8,               
				FORMAT_L6V5U5,             
				FORMAT_X8L8V8U8,           
				FORMAT_Q8W8V8U8,           
				FORMAT_V16U16,             
				FORMAT_A2W10V10U10,        

				FORMAT_UYVY,               
				FORMAT_R8G8_B8G8,          
				FORMAT_YUY2,               
				FORMAT_G8R8_G8B8,          
				FORMAT_DXT1,               
				FORMAT_DXT2,               
				FORMAT_DXT3,               
				FORMAT_DXT4,               
				FORMAT_DXT5,   
				FORMAT_ATI1N,               
				FORMAT_ATI2N,  

				FORMAT_D16_LOCKABLE,       
				FORMAT_D32,                
				FORMAT_D15S1,              
				FORMAT_D24S8,              
				FORMAT_D24X8,              
				FORMAT_D24X4S4,            
				FORMAT_D16,                

				FORMAT_D32F_LOCKABLE,      
				FORMAT_D24FS8,             

				FORMAT_L16,                

				FORMAT_VERTEXDATA,         
				FORMAT_INDEX16,            
				FORMAT_INDEX32,            

				FORMAT_Q16W16V16U16,       

				FORMAT_MULTI2_ARGB8,       

				// Floating point surface formats
				// s10e5 formats (16-bits per channel)
				FORMAT_R16F,                
				FORMAT_G16R16F,             
				FORMAT_A16B16G16R16F,       

				// IEEE s23e8 formats (32-bits per channel)
				FORMAT_R32F,                 
				FORMAT_G32R32F,              
				FORMAT_A32B32G32R32F,        

				FORMAT_CxV8U8,           

				FORMAT_R8G8B8A8,
				FORMAT_R8G8B8A8_SRGB,
				FORMAT_DXT1_SRGB,
				FORMAT_DXT2_SRGB,
				FORMAT_DXT3_SRGB,
				FORMAT_B8G8R8A8,
				FORMAT_B8G8R8A8_SRGB,
				FORMAT_B8G8R8X8,
				FORMAT_B8G8R8X8_SRGB,
				FORMAT_R10G10B10A2,
				
				FORMAT_RGB_PVRTC_2,
				FORMAT_RGBA_PVRTC_2,
				FORMAT_RGB_PVRTC_4,
				FORMAT_RGBA_PVRTC_4,

				FORMAT_RGB8_ETC1,
				FORMAT_RGB8_ETC2,
				FORMAT_RGBA8_ETC2,


				/*DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
				DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
				DXGI_FORMAT_R32G32B32A32_UINT           = 3,
				DXGI_FORMAT_R32G32B32A32_SINT           = 4,
				DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
				DXGI_FORMAT_R32G32B32_FLOAT             = 6,
				DXGI_FORMAT_R32G32B32_UINT              = 7,
				DXGI_FORMAT_R32G32B32_SINT              = 8,
				DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
				DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
				DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
				DXGI_FORMAT_R16G16B16A16_UINT           = 12,
				DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
				DXGI_FORMAT_R16G16B16A16_SINT           = 14,
				DXGI_FORMAT_R32G32_TYPELESS             = 15,
				DXGI_FORMAT_R32G32_FLOAT                = 16,
				DXGI_FORMAT_R32G32_UINT                 = 17,
				DXGI_FORMAT_R32G32_SINT                 = 18,
				DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
				DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
				DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
				DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
				DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
				DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
				DXGI_FORMAT_R10G10B10A2_UINT            = 25,
				DXGI_FORMAT_R11G11B10_FLOAT             = 26,
				DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
				DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
				DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
				DXGI_FORMAT_R8G8B8A8_UINT               = 30,
				DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
				DXGI_FORMAT_R8G8B8A8_SINT               = 32,
				DXGI_FORMAT_R16G16_TYPELESS             = 33,
				DXGI_FORMAT_R16G16_FLOAT                = 34,
				DXGI_FORMAT_R16G16_UNORM                = 35,
				DXGI_FORMAT_R16G16_UINT                 = 36,
				DXGI_FORMAT_R16G16_SNORM                = 37,
				DXGI_FORMAT_R16G16_SINT                 = 38,
				DXGI_FORMAT_R32_TYPELESS                = 39,
				DXGI_FORMAT_D32_FLOAT                   = 40,
				DXGI_FORMAT_R32_FLOAT                   = 41,
				DXGI_FORMAT_R32_UINT                    = 42,
				DXGI_FORMAT_R32_SINT                    = 43,
				DXGI_FORMAT_R24G8_TYPELESS              = 44,
				DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
				DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
				DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
				DXGI_FORMAT_R8G8_TYPELESS               = 48,
				DXGI_FORMAT_R8G8_UNORM                  = 49,
				DXGI_FORMAT_R8G8_UINT                   = 50,
				DXGI_FORMAT_R8G8_SNORM                  = 51,
				DXGI_FORMAT_R8G8_SINT                   = 52,
				DXGI_FORMAT_R16_TYPELESS                = 53,
				DXGI_FORMAT_R16_FLOAT                   = 54,
				DXGI_FORMAT_D16_UNORM                   = 55,
				DXGI_FORMAT_R16_UNORM                   = 56,
				DXGI_FORMAT_R16_UINT                    = 57,
				DXGI_FORMAT_R16_SNORM                   = 58,
				DXGI_FORMAT_R16_SINT                    = 59,
				DXGI_FORMAT_R8_TYPELESS                 = 60,
				DXGI_FORMAT_R8_UNORM                    = 61,
				DXGI_FORMAT_R8_UINT                     = 62,
				DXGI_FORMAT_R8_SNORM                    = 63,
				DXGI_FORMAT_R8_SINT                     = 64,
				DXGI_FORMAT_A8_UNORM                    = 65,
				DXGI_FORMAT_R1_UNORM                    = 66,
				DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
				DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
				DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
				DXGI_FORMAT_BC1_TYPELESS                = 70,
				DXGI_FORMAT_BC1_UNORM                   = 71,
				DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
				DXGI_FORMAT_BC2_TYPELESS                = 73,
				DXGI_FORMAT_BC2_UNORM                   = 74,
				DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
				DXGI_FORMAT_BC3_TYPELESS                = 76,
				DXGI_FORMAT_BC3_UNORM                   = 77,
				DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
				DXGI_FORMAT_BC4_TYPELESS                = 79,
				DXGI_FORMAT_BC4_UNORM                   = 80,
				DXGI_FORMAT_BC4_SNORM                   = 81,
				DXGI_FORMAT_BC5_TYPELESS                = 82,
				DXGI_FORMAT_BC5_UNORM                   = 83,
				DXGI_FORMAT_BC5_SNORM                   = 84,
				DXGI_FORMAT_B5G6R5_UNORM                = 85,
				DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
				DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
				DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
				DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
				DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
				DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
				DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
				DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
				DXGI_FORMAT_BC6H_TYPELESS               = 94,
				DXGI_FORMAT_BC6H_UF16                   = 95,
				DXGI_FORMAT_BC6H_SF16                   = 96,
				DXGI_FORMAT_BC7_TYPELESS                = 97,
				DXGI_FORMAT_BC7_UNORM                   = 98,
				DXGI_FORMAT_BC7_UNORM_SRGB              = 99,*/

				NUM_FORMAT,
			}FORMAT;

			typedef enum{
				TF_NEVER,						/*!< Never passes. */  	
				TF_LESS,						/*!< Passes if the incoming value is less than the stored value. */  	
				TF_EQUAL,						/*!< Passes if the incoming value is equal to the stored value. */  	
				TF_LEQUAL,						/*!< Passes if the incoming value is less than or equal to the stored value.  */  	 
				TF_GREATER,						/*!< Passes if the incoming value is greater than the stored value.  */  	 
				TF_NOTEQUAL,					/*!< Passes if the incoming value is not equal to the stored value.  */  	
				TF_GEQUAL,						/*!< Passes if the incoming value is greater than or equal to the stored value.  */  	 
				TF_ALWAYS,						/*!< Always passes. */  	
				NUM_TESTFUNC,
			}TESTFUNC;

			typedef enum {
				BF_ZERO = 0,					/*!< Blend factor is (0, 0, 0, 0). */  	
				BF_ONE,							/*!< Blend factor is (1, 1, 1, 1).  */  	
				BF_SRC_COLOR,					/*!< Blend factor is (Rs,Gs,Bs,As).  */  	
				BF_ONE_MINUS_SRC_COLOR,			/*!< Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As).  */  			
				BF_SRC_ALPHA,					/*!< Blend factor is (As, As, As, As).  */  	
				BF_ONE_MINUS_SRC_ALPHA,			/*!< Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As).  */  	
				BF_DEST_ALPHA,					/*!< Blend factor is (Ad, Ad, Ad, Ad).  */  	
				BF_ONE_MINUS_DEST_ALPHA,		/*!< Blend factor is (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad).  */  	
				BF_DEST_COLOR,					/*!< Blend factor is (Rd, Gd, Bd, Ad).  */  	
				BF_ONE_MINUS_DEST_COLOR,		/*!< Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad).  */  	
				BF_SRC_ALPHA_SAT,				/*!< Blend factor is (f, f, f, 1); f = min(A, 1 - Ad).  */  	
				BF_BOTH_SRC_ALPHA,				/*!< */  	
				BF_BOTH_ONE_MINUS_SRC_ALPHA,	/*!< Source blend factor is (1 - As, 1 - As, 1 - As, 1 - As), and destination blend factor is (As, As, As, As); the destination blend selection is overridden. This blend mode is supported only for the D3DRS_SRCBLEND render state.  */  	
				BF_BLEND_FACTOR,				/*!< Constant color blending factor used by the frame-buffer blender. This blend mode is supported only if D3DPBLEND_BLENDFACTOR is on. */  	
				BF_ONE_MINUS_BLEND_FACTOR,		/*!< Inverted constant color-blending factor used by the frame-buffer blender. This blend mode is supported only if the D3DPBLEND_BLENDFACTOR bit is set in the SrcBlendCaps or DestBlendCaps members of D3DCAPS9. */  	
				NUM_BLENDFUNC,
			}BLENDFUNC;

			typedef enum {
				BO_ADD = 0,
				BO_SUBTRACT,
				BO_REVSUBTRACT,
				BO_MIN,
				BO_MAX,
				NUM_BLENDOP,
			}BLENDOP;			

			typedef enum {
				BLT_ALPHA = 0,
				BLT_ADD,
				BLT_SUB,
				BLT_COLOR,
				NUM_BLENDTYPES,
			}BLENDTYPE;

			typedef enum {
				CF_NONE = 0,
				CF_SELECT_ARG0,
				CF_SELECT_ARG1,
				CF_MODULATE,
				CF_MODULATE2X,
				CF_MODULATE4X,
				CF_ADD,
				CF_ADD_SIGNED,
				CF_ADD_SIGNED2X,
				CF_SUBTRACT,
				CF_ADD_SMOOTH,
				CF_BLEND_DIFFUSE_ALPHA,
				CF_BLEND_TEXTURE_ALPHA,
				CF_BLEND_FACTOR_ALPHA,
				CF_BLEND_TEXTURE_ALPHAPM,
				CF_BLEND_CURRENT_ALPHA,
				CF_BLEND_PREMODULATE,
				CF_MODULATE_ALPHA_ADD_COLOR,
				CF_MODULATE_COLOR_ADD_ALPHA,
				CF_MODULATE_INV_ALPHA_ADD_COLOR,
				CF_MODULATE_INV_COLOR_ADD_ALPHA,
				CF_BUMP_ENVMAP,
				CF_BUMPE_NVMAP_LUMINANCE,
				CF_DOT3,
				CF_MULTIPLY_ADD,
				CF_INTERPOLATE,
				CF_DOT3_RGBA,
				NUM_COMBINEFUNC,
			}COMBINEFUNC;

			typedef enum {
				CA_CONSTANT = 0,				/*!< Select a constant from a texture stage. The default value is 0xffffffff. */
				CA_CURRENT,						/*!< The texture argument is the result of the previous blending stage. In the first texture stage (stage 0), this argument is equivalent to D3DTA_DIFFUSE. If the previous blending stage uses a bump-map texture (the D3DTOP_BUMPENVMAP operation), the system chooses the texture from the stage before the bump-map texture. If s represents the current texture stage and s - 1 contains a bump-map texture, this argument becomes the result output by texture stage s - 2. Permissions are read/write.  */
				CA_DIFFUSE,						/*!< The texture argument is the diffuse color interpolated from vertex components during Gouraud shading. If the vertex does not contain a diffuse color, the default color is 0xffffffff. Permissions are read-only.  */
				CA_MASK,						/*!< Mask value for all arguments; not used when setting texture arguments.  */
				CA_SPECULAR,					/*!< The texture argument is the specular color interpolated from vertex components during Gouraud shading. If the vertex does not contain a specular color, the default color is 0xffffffff. Permissions are read-only.  */
				CA_TEMP,						/*!< The texture argument is a temporary register color for read or write. D3DTA_TEMP is supported if the D3DPMISCCAPS_TSSARGTEMP device capability is present. The default value for the register is (0.0, 0.0, 0.0, 0.0). Permissions are read/write.  */
				CA_TEXTURE,						/*!< The texture argument is the texture color for this texture stage. Permissions are read-only.  */
				CA_TFACTOR,						/*!< The texture argument is the texture factor set in a previous call to the IDirect3DDevice9::SetRenderState with the D3DRS_TEXTUREFACTOR render-state value. Permissions are read-only.  */
				NUM_COMBINEARG,
			}COMBINEARG;

			typedef enum {		
				TF_POINT = 0,
				TF_LINEAR,
				TF_ANISOTROPIC,	
				TF_PYRAMIDALQUAD,	
				TF_GAUSSIANQUAD,
				NUM_TEXTUREFILTERMODE,
			}TEXTUREFILTERMODE;

			typedef enum {				
				DB_ALWAYS = 0,
				DB_NONE,
				DB_CW,
				DB_W,
				DB_C,	
				NUM_DEPTHBUFFERMODE,
			}DEPTHBUFFERMODE;		

			typedef enum{
				SO_KEEP = 0,		/*!< Keeps the current value. */
				SO_ZERO,			/*!< Sets the stencil buffer value to zero. */
				SO_REPLACE,			/*!< Sets the stencil buffer value to ref, as specified by glStencilFunc. */
				SO_INCR,			/*!< Increments the current stencil buffer value. Clamps to the maximum representable unsigned value. */
				SO_DECR,			/*!< Decrements the current stencil buffer value. Clamps to zero. */
				SO_INVERT,			/*!< Bitwise inverts the current stencil buffer value. */
				NUM_STENCILOP,
			}STENCILOP;

			typedef enum{
				FM_WIREFRAME = 0,	/*!< 畫框線。 */  	
				FM_SOLID,			/*!< 畫實體。 */  	
				NUM_FILLMODE,		
			}FILLMODE;

			typedef enum{
				CULL_NONE = 0,
				CULL_CCW,
				CULL_CW,
				NUM_CULLMODE,
			}CULLMODE;

			typedef enum {
				FM_MIN_MAG_MIP_POINT = 0,
				FM_MIN_MAG_POINT_MIP_LINEAR,
				FM_MIN_POINT_MAG_LINEAR_MIP_POINT,
				FM_MIN_POINT_MAG_MIP_LINEAR,
				FM_MIN_LINEAR_MAG_MIP_POINT,
				FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
				FM_MIN_MAG_LINEAR_MIP_POINT,
				FM_MIN_MAG_MIP_LINEAR,
				FM_ANISOTROPIC,
				FM_COMPARISON_MIN_MAG_MIP_POINT,
				FM_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
				FM_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
				FM_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
				FM_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
				FM_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
				FM_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
				FM_COMPARISON_MIN_MAG_MIP_LINEAR,
				FM_COMPARISON_ANISOTROPIC,
				NUM_FILTERMODE,
			}FILTERMODE;

			typedef enum{
				AM_WRAP = 0,		/*!< 重複模式，while (u > 1.0) u -= 1.0;，while (u < 0.0) u += 1.0;。 */  			
				AM_MIRROR,			/*!< 鏡射模式，當貼圖軸大於1.0或小於0.0時。 */  		
				AM_CLAMP,			/*!< 延伸模式，當貼圖軸大於1.0或小於0.0時，皆以邊緣顏色取代。 */  		
				AM_BORDER,			/*!< 邊緣模式，當貼圖軸大於1.0或小於0.0時，皆以border顏色取代。 */  			
				AM_MIRROR_ONCE,
				NUM_ADDRESSMODE,		
			}ADDRESSMODE;

			typedef enum {
				POINT_LIST,
				LINE_LIST,
				LINE_STRIP,
				LINE_LOOP,
				TRIANGLE_LIST,
				TRIANGLE_STRIP,
				TRIANGLE_FAN,
				QUAD_LIST,
				QUAD_STRIP,
				POLYGON,
				NUM_PRIMITIVETYPE,
			}PRIMITIVETYPE;						

			typedef enum{
				VST_NULL = -1,
				VST_POSITION,
				VST_TANGENT,
				VST_NORMAL,
				VST_BINORMAL,
				VST_TEX_COORD,	
				VST_COLOR,	
				VST_BLEND_WEIGHT,
				VST_BLEND_INDICES,
				VST_DEPTH,
				VST_POINT_SIZE,
				NUM_VERTEXSLOTTYPE,
			}VERTEXSLOTTYPE;

			typedef enum{
				VSF_NULL = -1,
				VSF_UBYTE,
				VSF_SHORT,
				VSF_FLOAT,
				NUM_VERTEXSLOTFORMAT,
			}VERTEXSLOTFORMAT;

			typedef enum {
				CT_BOOL,
				CT_BOOL_ARRAY,
				CT_INT,
				CT_INT_ARRAY,
				CT_FLOAT,
				CT_FLOAT_ARRAY,
				CT_VECTOR2,
				CT_VECTOR2_ARRAY,
				CT_VECTOR3,
				CT_VECTOR3_ARRAY,
				CT_VECTOR4,
				CT_VECTOR4_ARRAY,
				CT_MATRIX,
				CT_MATRIX_ARRAY,
				NUM_CONSTANTTYPE,
			}CONSTANTTYPE;

			typedef enum {
				CUF_PER_FRAME,
				CUF_WHEN_DIRTY,				
				NUM_CONSTANTUPDATEFREQUENCE,
			}CONSTANTUPDATEFREQENCE;

			typedef struct {
				BYTE							Stream;					
				VERTEXSLOTTYPE					Type;
				CAnsiString						TypeName;			
				DWORD							TypeIndex;
				VERTEXSLOTFORMAT				Format;
				DWORD							Number;
			}VERTEXSLOT;		


		/*	typedef struct {
				DEPTHBUFFERMODE					DepthMode;	
				TESTFUNC						DepthFunc;

				LYNXINLINE void Default()
				{
					DepthMode = DB_CW;
					DepthFunc = TF_LEQUAL;
				}
			}DEPTHSTATEDESC;*/

			typedef struct {
				int                             Width;
				int                             Height;
				int                             bpp;				
				BYTE                            RBits;
				BYTE                            GBits;
				BYTE                            BBits;				
				DWORD							RefreshRate;
				FORMAT							Format;					
			}VIDEOMODE;			

			typedef struct {
				DEPTHBUFFERMODE					DepthMode;	
				TESTFUNC						DepthFunc;
				LYNXBOOL						StencilEnable;
				STENCILOP						StencilFailOp;
				STENCILOP						StencilDepthFailOp;
				STENCILOP						StencilPassOp;
				TESTFUNC						StencilFunc;
				unsigned char					StencilRef;
				unsigned char					StencilWriteMask;
				unsigned char					StencilReadMask;				

				LYNXINLINE void Default()
				{
					DepthMode			= DB_CW;
					DepthFunc			= TF_LEQUAL;
					StencilEnable		= LYNX_FALSE;
					StencilFailOp		= SO_KEEP;
					StencilDepthFailOp	= SO_KEEP;
					StencilPassOp		= SO_KEEP;
					StencilFunc			= TF_ALWAYS;
					StencilRef			= 0x0;
					StencilWriteMask	= 0xff;
					StencilReadMask		= 0xff;
				}
			}DEPTHSTENCILSTATEDESC;

			struct RTBLENDPARAM {
				LYNXBOOL		bBlend;
				BLENDFUNC		SrcBlend;
				BLENDFUNC		DestBlend;
				BLENDOP			BlendOp;
				BLENDFUNC		SrcBlendAlpha;
				BLENDFUNC		DestBlendAlpha;
				BLENDOP			BlendOpAlpha;
				unsigned char	RTWriteMask;

				LYNXINLINE void Default()
				{
					bBlend			= LYNX_TRUE;
					SrcBlend		= BF_ONE;
					DestBlend		= BF_ZERO;
					BlendOp			= BO_ADD;
					SrcBlendAlpha	= BF_ONE;
					DestBlendAlpha	= BF_ZERO;
					BlendOpAlpha	= BO_ADD;
					RTWriteMask		= ALL_CHANNELS;
				}

				LYNXINLINE RTBLENDPARAM& operator =(const RTBLENDPARAM& rhs) 
				{
					bBlend = rhs.bBlend;
					SrcBlend = rhs.SrcBlend;
					DestBlend = rhs.DestBlend;
					BlendOp = rhs.BlendOp;
					SrcBlendAlpha = rhs.SrcBlendAlpha;
					DestBlendAlpha = rhs.DestBlendAlpha;
					BlendOpAlpha = rhs.BlendOpAlpha;
					RTWriteMask = rhs.RTWriteMask;
					return *this;
				}

				LYNXINLINE bool operator ==(const RTBLENDPARAM& rhs) const  
				{
					return (bBlend == rhs.bBlend &&
							SrcBlend == rhs.SrcBlend &&
							DestBlend == rhs.DestBlend &&
							BlendOp == rhs.BlendOp &&
							SrcBlendAlpha == rhs.SrcBlendAlpha &&
							DestBlendAlpha == rhs.DestBlendAlpha &&
							BlendOpAlpha == rhs.BlendOpAlpha &&
							RTWriteMask == rhs.RTWriteMask);
				}
			};

			typedef struct {
				LYNXBOOL					bAlphaToCoverage;
				LYNXBOOL					bIndependentBlend;
				RTBLENDPARAM				RTBlendParam[MAX_NUM_MRT];

				LYNXINLINE void Default()
				{
					bAlphaToCoverage	= LYNX_FALSE;
					bIndependentBlend	= LYNX_FALSE;
					for (int i=0; i<MAX_NUM_MRT; ++i)
						RTBlendParam[i].Default();
				}
			}BLENDSTATEDESC;

			typedef struct {
				FILLMODE					FillMode;
				CULLMODE					CullMode;				

				LYNXINLINE void Default()
				{
					FillMode = FM_SOLID;
					CullMode = CULL_CCW;
				}
			}RASTERIZERSTATEDESC;

			typedef struct {
				FILTERMODE					Filter;	
				ADDRESSMODE					AddressU;
				ADDRESSMODE					AddressV;
				ADDRESSMODE					AddressW;
				float						MipLODBias;
				DWORD						MaxAnisotropy;
				TESTFUNC					ComparisonFunc;
				LYNXCOLORRGBA				BorderColor;
				float						MinLOD;
				float						MaxLOD;	

				LYNXINLINE void Default()
				{
					Filter					= FM_MIN_MAG_LINEAR_MIP_POINT;
					AddressU				= AM_WRAP;
					AddressV				= AM_WRAP;
					AddressW				= AM_WRAP;
					MipLODBias				= 0.0f;
					MaxAnisotropy			= 0;
					ComparisonFunc			= TF_ALWAYS;
					BorderColor.PackedColor = 0xffffffff;
					MinLOD					= 0.0f;
					MaxLOD					= 256.0f;
				}
			}SAMPLERSTATEDESC;

			typedef struct {
				int								MipMapLevel;
				int								LODBias;
				LYNXBOOL						bSRGB;
				float							AlphaReference;
				TEXTURETYPE						Type;
                FORMAT                          TargetFormat;
                FORMAT                          SourceFormat;
                int                             Width;
                int                             Height;
				LYNXBOOL						bLoadFromMemory;
                const void*                     lpData;
				SAMPLERSTATEDESC				SamplerDesc;
				LYNXBOOL						bDelayLoading;

				LYNXINLINE void Default()
				{
					MipMapLevel			= 0;
					LODBias				= 0;
					bSRGB				= LYNX_FALSE;		
					AlphaReference		= 0.0f;
					Type				= TT_2D_TEXTURE;
                    TargetFormat        = FORMAT_A8R8G8B8;
                    SourceFormat        = FORMAT_A8R8G8B8;
					bLoadFromMemory		= LYNX_FALSE;
                    lpData              = NULL;
                    Width               = -1;
                    Height              = -1;   
					bDelayLoading		= LYNX_FALSE;	
					SamplerDesc.Default();
				}
			}TEXTUREDESC;

			typedef	struct {
				LYNXREAL					x, y, z;	
				LYNXREAL					nx, ny, nz;	
				DWORD						color; 
				DWORD						specular; 
				LYNXUVPOINT					TexCoord[8]; 
			}FIXEDVERTEX;

			typedef struct {
				RENDERERTYPE				RendererType;
				int							Width;
				int                         Height;				
				FORMAT						FrameBufferFormat;
				FORMAT						DepthBufferFormat;	
				RENDERDEVICETYPE			RenderDeviceType;
				RENDERERFEATURE				RendererFeature;
				LYNXBOOL					bHardware;	
				LYNXBOOL					bFullScreen;
				LYNXBOOL					bWaitVSync;
				DWORD						RefreshRate;
				//LYNXINTERLACETYPE			InterlaceType;
				//LYNXTVSCANMODE				TVScanMode;
				//LYNXFRAMEMODE				FrameMode;
				DWORD						NumMultiSamples;
				DWORD						ShaderDebuggingFlag;
			}INITGRAPHICPARAM;

			typedef struct {
				PRIMITIVETYPE				Type;	
				unsigned int				BaseVertex;
				unsigned int				BaseIndex;
				DWORD						NumIndex;
				unsigned short				*lpIndex;	
				DWORD						NumPrimitives;
			}PRIMITIVE;

			typedef struct {		
				CAnsiString					Name;	
				CAnsiString					Definition;	
			}SHADERMACRO;	
			
			CArray<SHADERMACRO>				m_ShaderMacros;

			//Profile data
			DWORD							m_SetRenderStateTimeCounter;
			DWORD							m_SetRenderStateCounter;	
			DWORD							m_DrawTimeCounter;
			DWORD							m_DrawCallCounter;	
			DWORD							m_PrimitiveCounter;
			DWORD							m_SetTextureCounter;
			DWORD							m_SetShaderCounter;
			DWORD							m_SetShaderConstantCounter;
			DWORD							m_BufferMapCounter;
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;	

			RENDERERTYPE					m_RendererType;
			CAPS							m_Capbilities;

			CString							m_ShaderErrorMessage;
			CString							m_LastErrorMessage;

			CArray<VIDEOMODE>				m_VideoModeList;
			CBufferPtr						m_lpBackBuffer;
			CBufferPtr						m_lpDepthBuffer;		
			CBufferPtr						m_lpOffscreenDepthBuffer;		

			LYNXWINDOW						m_Window;
			LYNXPOINT2D						m_WindowOrigin;
            LYNXBOOL                        m_bPortraitMode;

			static FIXEDVERTEXLAYOUT		m_SystemFixedVertexLayout;
			
			CString							m_CurrentTexturePath;
            CList<CString>                  m_TextureSearchPathList;
			CList<CString>					m_ShaderSrcPath;
			CString							m_UserShaderSrcPath;
			CString							m_CachedCompiledShaderPath;
			SHADERTARGET					m_ShaderTarget;
			Math::CVector2					m_Pixel2TexelOffset;
			LYNXBOOL						m_bInvertRenderTarget;
			
			LYNXBOOL						m_bHardwareBusy;
			LYNXBOOL						m_bUseCachedState;
            LYNXBOOL                        m_bSharedShaderGroup;
			LYNXBOOL						m_bSharedShaderClass;
			LYNXBOOL						m_bCacheShaderGroup;
			CBuffer*						m_CachedRT[MAX_NUM_MRT];
			CBuffer*						m_CachedDB;
			CTexture*						m_CachedTexture[MAX_NUM_TEXTURE_CHANNEL];
			CVertexShader*					m_CachedVS;
			CPixelShader*					m_CachedPS;
			CVertexLayout*					m_CachedVertexLayout;
			CVertexArray*					m_CachedVertexArray;
			CIndexArray*					m_CachedIndexArray;
			LYNXRECT						m_CachedViewportRect;

			CShaderGroup*					m_CachedShaderGroup;
			CDepthStencilState*				m_CachedDepthStencilState;
			CBlendState*					m_CachedBlendState;
			CRasterizerState*				m_CachedRasterizerState;
			CSamplerState*					m_CachedSamplerState[MAX_NUM_TEXTURE_CHANNEL];		
			LYNXDWORD						m_CachedColorWriteMask;

			LYNXBOOL						m_bCachedScissorEnabled;

			CAnsiString						m_Version;	
			CAnsiString						m_Vendor;
			CAnsiString						m_Renderer;

			CList<CResourcePtr>				m_CachedResourceList;
		public:						
			CRenderer();
			CRenderer(CGraphicsSystem* lpgs);
			virtual ~CRenderer();											
						
			LYNXFORCEINLINE RENDERERTYPE	GetRendererType() const {return m_RendererType; };
			virtual const LYNXCHAR*			GetAPIString(void) = 0;
            
            LYNXFORCEINLINE const LYNXBOOL	IsPortraitMode() const {return m_bPortraitMode; };
            LYNXFORCEINLINE void            SetPortraitMode(const LYNXBOOL b) {m_bPortraitMode = b; };

			LYNXFORCEINLINE LYNXBOOL		IsRenderTargetInvert() {return m_bInvertRenderTarget; }

			LYNXFORCEINLINE const CAPS		GetCaps() const {return m_Capbilities; };
			LYNXFORCEINLINE const LYNXCHAR*	GetTexturePath(void) {return m_CurrentTexturePath.c_str(); };	
			LYNXFORCEINLINE void			SetTexturePath(const LYNXCHAR* path) {m_CurrentTexturePath = path; };
            LYNXFORCEINLINE void            ResetTextureSearchPathList() {m_TextureSearchPathList.clear(); };
            LYNXFORCEINLINE void            AddTextureSearchPath(const CString& path) {m_TextureSearchPathList.push_back(path); };
			LYNXFORCEINLINE CList<CString>& GetTextureSearchPathList() {return m_TextureSearchPathList; };
			virtual const LYNXCHAR*			GetDriverDesc(void) = 0;

			virtual LYNXBOOL				Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp) = 0;
			virtual void					Close();
			virtual void					Resize(int cx, int cy) = 0;
			virtual void					Pause() {};
			virtual void					Resume(LYNXWINDOW wnd) {};

			virtual LYNXBOOL				IsValid() = 0;
			LYNXFORCEINLINE LYNXWINDOW		GetWindow() {return m_Window; };
			void							SetHardwareBusy(LYNXBOOL b) {m_bHardwareBusy = b; };			
			virtual LYNXBOOL				IsHardwareBusy() {return m_bHardwareBusy; };			

			virtual void					Start(void) = 0;		
			virtual void					End(void) = 0;
            LYNXFORCEINLINE virtual void	Finish(void) {};
            LYNXFORCEINLINE virtual void	Flush(void) {};
			//virtual void					ShowBackBuffer(int x, int y, int width, int height) {lynxSwapRect(x, y, width, height, SystemData.bFullScreen); };
			virtual void					SwapBackBuffer() = 0;
			virtual void					Clear(int flag, LYNXCOLORRGBA& c, LYNXREAL z, int s) = 0;			
			
			// Context
			virtual void					SetMainContext() {};
			virtual void					SetLoadingContext() {};

			//Fixed functions ----------------------------------------------------------------------------------------------------------------------------------------
			virtual void					SetWorldMatrix(const CMatrix4& m) = 0;
			virtual void					GetWorldMatrix(CMatrix4& m) = 0;
			virtual CMatrix4&				GetWorldMatrix() = 0;
			virtual void					SetViewMatrix(const CMatrix4& m) = 0;
			virtual void					GetViewMatrix(CMatrix4& m) = 0;
			virtual CMatrix4&				GetViewMatrix() = 0;
			virtual void					SetProjectionMatrix(const CMatrix4& m) = 0;
			virtual void					GetProjectionMatrix(CMatrix4& m) = 0;
			virtual CMatrix4&				GetProjectionMatrix() = 0;
			
			virtual void					UseFog(LYNXBOOL b) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			virtual void					UseSpecular(LYNXBOOL b) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			virtual void					UseAlphaBlend(LYNXBOOL b) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			virtual void					UseAlphaTest(LYNXBOOL b) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			virtual void					SetAlphaReference(float af) {glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			static FIXEDVERTEXLAYOUT		GetSystemFixedVertexLayout() { return  m_SystemFixedVertexLayout; };
			virtual void					SetFixedVertexLayout(FIXEDVERTEXLAYOUT vs) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};

			virtual void					DrawStrip(FIXEDVERTEX* lpv, int x, int y) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};			
			virtual void					DrawTriangle(FIXEDVERTEX& v1, FIXEDVERTEX& v2, FIXEDVERTEX& v3) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};	
			virtual void					DrawTriangleStrip(FIXEDVERTEX* lpv) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};	
			virtual void					DrawTriangleFans(FIXEDVERTEX* lpv, int num) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};	
			virtual void					DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};
			virtual void					DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert) {};//{glpLogger->vLog(CString(_T("Warning :"))+CString(GetClassName())+CString(_T(" doesn't support fixed function.\r\n")));};		

			//System ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void					GetSystemOrigin(int* x, int* y) = 0;
			virtual void					SetSystemOrigin(int x, int y) = 0;

			//Viewport ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE virtual void	SetViewportRect(int x1, int y1, int x2, int y2) {m_CachedViewportRect.x1 = x1; m_CachedViewportRect.x2 = x2; m_CachedViewportRect.y1 = y1; m_CachedViewportRect.y2 = y2; };
			LYNXFORCEINLINE virtual void	SetViewportRect(const LYNXRECT& rect) {SetViewportRect(rect.x1, rect.y1, rect.x2, rect.y2); };
			LYNXFORCEINLINE virtual void	GetViewportRect(LPLYNXRECT lpvr) {*lpvr = m_CachedViewportRect; };	
			virtual void					SetScissorRect(int x1, int y1, int x2, int y2) = 0;
			virtual void					SetScissorRect(LPLYNXRECT r) = 0;
			virtual void					GetScissorRect(LPLYNXRECT lpsr) = 0;
			virtual LYNXBOOL				EnableScissorRect(LYNXBOOL b) = 0;
            LYNXFORCEINLINE LYNXBOOL        IsScissorRectEnabled() {return m_bCachedScissorEnabled; };
			virtual LYNXDWORD				GetClipPlaneState() = 0;	
			virtual void					Clipping(LYNXBOOL b) = 0;
			virtual void					SetClipPlane(int i, LPLYNXPLANE p) = 0;
			virtual void					GetClipPlane(int i, LPLYNXPLANE p) = 0;

			//Query -----------------------------------------------------------------------------------------------------------------------------------------------------
			virtual CQuery*					CreateQuery() = 0;

			//Vertex Layout ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual CVertexLayout*			CreateVertexLayout() = 0;
			CVertexLayout*					SetVertexLayout(CVertexLayout* vl);
			void							CreateVertexLayout(CVertexLayoutPtr& v, CString& name, CRenderer::VERTEXSLOT* vs, int num);
			LYNXINLINE CVertexLayout*		GetCachedVertexLayout() {return m_CachedVertexLayout; };	

			//Rendering Primitives ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual void					DrawScreenQuad(void* lpv, DWORD size) = 0;
			virtual void					DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) = 0;
			virtual void					DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert) = 0;
			virtual	void					DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert) = 0;						
			virtual void					DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) = 0;
			virtual void					DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) = 0;
			
			//Buffer ----------------------------------------------------------------------------------------------------------------------------------------------			
			LYNXFORCEINLINE CBufferPtr&		GetSystemBackBuffer() {return m_lpBackBuffer; };			
			LYNXFORCEINLINE CBufferPtr&		GetSystemDepthBuffer() {return m_lpDepthBuffer; };			
			LYNXFORCEINLINE CBufferPtr&		GetOffscreenDepthBuffer() {return m_lpOffscreenDepthBuffer; };					
			
			virtual CBuffer*				CreateBuffer() = 0;
						
			virtual CBuffer*				SetRenderBuffer(int index, CBuffer* lpb) = 0;
			virtual CBuffer*				GetCachedDepthBuffer() {return m_CachedDB; };
			virtual CBuffer*				SetDepthBuffer(CBuffer* lpzb) = 0;
			LYNXINLINE CBuffer*				GetCachedRenderBuffer(int i) {return m_CachedRT[i]; };	
            virtual void                    DiscardRenderBuffer(int index) {};
            virtual void                    DiscardDepthBuffer() {};
            
			LYNXBOOL						CreateDepthBuffer(CBufferPtr& buffer, DWORD w, DWORD h, FORMAT format, int num_ms);
			LYNXBOOL						CreateRenderBuffer(CBufferPtr& buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable = LYNX_FALSE);			
			virtual void					BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect) = 0;
			
			//Texture ----------------------------------------------------------------------------------------------------------------------------------------------
			static  DWORD					GetFormatBytes(FORMAT f);
			virtual CTexture*				CreateTexture() = 0;
			virtual LYNXBOOL				CheckTextureFormat(FORMAT bbfmt, FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckTextureFormat(FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckDepthFormat(FORMAT bbfmt, FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckDepthFormat(FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckRTFormat(FORMAT bbfmt, FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckRTFormat(FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckFormatSRGBRead(FORMAT bbfmt, FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckFormatSRGBRead(FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckFormatSRGBWrite(FORMAT bbfmt, FORMAT fmt) = 0;			
			virtual LYNXBOOL				CheckFormatSRGBWrite(FORMAT fmt) = 0;			
			LYNXBOOL						LoadTexture(CTexturePtr& t, CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL);
			CTexture*						SetTexture(int channel, CTexture* t);

			//Vertex Array ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual CVertexArray*			CreateVertexArray() = 0;
			CVertexArray*					SetVertexArray(CVertexArray* va, int stream, int first);
			LYNXINLINE CVertexArray*		GetCachedVertexArray() {return m_CachedVertexArray; };	
			//LYNXINLINE void					SetCachedVertexArray(CVertexArray* a) {m_CachedVertexArray = a; };	
			//Index Array ----------------------------------------------------------------------------------------------------------------------------------------------
			virtual CIndexArray*			CreateIndexArray() = 0;			
			CIndexArray*					SetIndexArray(CIndexArray* ia, int first);
			LYNXINLINE CIndexArray*			GetCachedIndexArray() {return m_CachedIndexArray; };	
			//LYNXINLINE void					SetCachedIndexArray(CIndexArray* a) {m_CachedIndexArray = a; };	
			//Shader ----------------------------------------------------------------------------------------------------------------------------------------------		
			LYNXFORCEINLINE const Math::CVector2& GetPixel2TexelOffset() const {return m_Pixel2TexelOffset; };			
			void							GetSubPixelOffset(LYNXUVPOINT* lpuv);
			virtual void					SetShaderTarget(SHADERTARGET t) {m_ShaderTarget = t; };
			LYNXINLINE SHADERTARGET			GetShaderTarget() {return m_ShaderTarget; };			
			LYNXINLINE CString&				GetCachedCompiledShaderPath() {return m_CachedCompiledShaderPath; };
			void							AddDefinedValue(LynxEngine::CAnsiString& def, LynxEngine::CAnsiString& value);
			void							AddDefinedValue(const char* def, const char* value);
			virtual CString&				ComputeCachedCompiledShaderPath();
			LYNXBOOL						NeedToRecompileShader();

			virtual void					NullVertexShader() = 0;
			virtual void					NullPixelShader() = 0;

			virtual void					SetShaderBasePath(const LYNXCHAR *path) = 0;	
			LYNXINLINE CList<CString>&		GetShaderSrcPathList() {return m_ShaderSrcPath; };				
			LYNXINLINE CString&				GetUserShaderSrcPath() {return m_UserShaderSrcPath; };				

			virtual CVertexShader*			CreateVertexShader() = 0;
			virtual CPixelShader*			CreatePixelShader() = 0;
			virtual CShaderConstant*		CreateShaderConstant(CShader* lps, const char* name) = 0;
			virtual void					ReleaseShaderConstant(CShaderConstant* lpsc) = 0;
			virtual CShaderConstantGroup*	CreateShaderConstantGroup(CShader* lps, const char* name) = 0;
			virtual void					ReleaseShaderConstantGroup(CShaderConstantGroup* lpsc) = 0;
			virtual CShaderResource*		CreateShaderResource(CShader* lps, const char* name) = 0;
			virtual CShaderResource*		CreateShaderResource(CShader* lps, const char* name, CResource* r) = 0;
			virtual void					ReleaseShaderResource(CShaderResource* lpsc) = 0;
			virtual CShaderGroup*			CreateShaderGroup() = 0;
			virtual void					ReleaseShaderGroup(CShaderGroup* lpsg) = 0;
			CShaderGroup*					vSetShaderGroup(CShaderGroup* lpsg, CContainer* lpo, CCameraContainer* cam);
			LYNXFORCEINLINE CShaderGroup*	GetCachedShaderGroup() {return m_CachedShaderGroup; };	
			
			LYNXBOOL						LoadVertexShader(CShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def = NULL, CStream* stream = NULL); 
			LYNXBOOL						LoadVertexShader(CVertexShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def = NULL, CStream* stream = NULL);
			CVertexShader*					SetVertexShader(CVertexShader* vs);
			LYNXBOOL						LoadPixelShader(CShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def = NULL, CStream* stream = NULL);
			LYNXBOOL						LoadPixelShader(CPixelShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def = NULL, CStream* stream = NULL);
			CPixelShader*					SetPixelShader(CPixelShader* ps);			
			LYNXFORCEINLINE CVertexShader*	GetCachedVertexShader() {return m_CachedVS; };				
			LYNXFORCEINLINE CPixelShader*	GetCachedPixelShader() {return m_CachedPS; };
						
			virtual void					SetVSConstantF(int i, const LYNXREAL *lpdata, int num) = 0;
			virtual void					GetVSConstantF(int i, LYNXREAL *lpdata, int num) = 0;
			virtual void					SetVSConstantI(int i, const int *lpdata, int num) = 0;
			virtual void					GetVSConstantI(int i, int *lpdata, int num) = 0;
			virtual void					SetVSConstantB(int i, const BOOL *lpdata, int num) = 0;
			virtual void					GetVSConstantB(int i, BOOL *lpdata, int num) = 0;

			virtual void					SetPSConstantF(int i, const LYNXREAL *lpdata, int num) = 0;
			virtual void					GetPSConstantF(int i, LYNXREAL *lpdata, int num) = 0;
			virtual void					SetPSConstantI(int i, const int *lpdata, int num) = 0;
			virtual void					GetPSConstantI(int i, int *lpdata, int num) = 0;
			virtual void					SetPSConstantB(int i, const BOOL *lpdata, int num) = 0;
			virtual void					GetPSConstantB(int i, BOOL *lpdata, int num) = 0;

			//Render State ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void			UseCachedState(LYNXBOOL b) {m_bUseCachedState = b; };
			LYNXFORCEINLINE void			SetCachedTexture(int channel, CTexture* t) {m_CachedTexture[channel] = t; };
            LYNXFORCEINLINE void            SharedShaderGroup(LYNXBOOL b) {m_bSharedShaderGroup = b; }
            LYNXFORCEINLINE LYNXBOOL        IsSharedShaderGroup() {return m_bSharedShaderGroup; }
			LYNXFORCEINLINE void            SharedShaderClass(LYNXBOOL b) {m_bSharedShaderClass = b; }
            LYNXFORCEINLINE LYNXBOOL        IsSharedShaderClass() {return m_bSharedShaderClass; }
			LYNXFORCEINLINE void            CacheShaderGroup(LYNXBOOL b) { m_bCacheShaderGroup = b; }
			LYNXFORCEINLINE LYNXBOOL        IsCacheShaderGroup() { return m_bCacheShaderGroup; }
            
			virtual CDepthStencilState*		CreateDepthStencilState() = 0;
			void							CreateDepthStencilState(CDepthStencilStatePtr& s, const DEPTHSTENCILSTATEDESC& desc);
			CDepthStencilState*				SetDepthStencilState(CDepthStencilState* s);
			LYNXINLINE CDepthStencilState*	GetCachedDepthStencilState() {return m_CachedDepthStencilState; };	
			//LYNXINLINE void					SetCachedStencilState(CStencilState* s) {m_CachedStencilState = s; };	
			
			virtual CBlendState*			CreateBlendState() = 0;
			void							CreateBlendState(CBlendStatePtr& s, const BLENDSTATEDESC& desc);
			CBlendState*					SetBlendState(CBlendState* s);
			LYNXINLINE CBlendState*			GetCachedBlendState() {return m_CachedBlendState; };	
			//LYNXINLINE void					SetCachedBlendState(CBlendState* s) {m_CachedBlendState = s; };	

			virtual CRasterizerState*		CreateRasterizerState() = 0;
			void							CreateRasterizerState(CRasterizerStatePtr& s, const RASTERIZERSTATEDESC& desc);
			CRasterizerState*				SetRasterizerState(CRasterizerState* s);
			LYNXINLINE CRasterizerState*	GetCachedRasterizerState() {return m_CachedRasterizerState; };	
			//LYNXINLINE void					SetCachedBlendState(CBlendState* s) {m_CachedBlendState = s; };	

			virtual CSamplerState*			CreateSamplerState() = 0;
			void							CreateSamplerState(CSamplerStatePtr& s, const SAMPLERSTATEDESC& desc);
			CSamplerState*					SetSamplerState(int channel, CSamplerState* s);
			LYNXINLINE CSamplerState*		GetCachedSamplerState(int channel) {return m_CachedSamplerState[channel]; };	
			//LYNXINLINE void					SetCachedBlendState(CBlendState* s) {m_CachedBlendState = s; };	
			

			//virtual DEPTHBUFFERMODE			SetDepthBufferMode(DEPTHBUFFERMODE zbm) = 0;
			//virtual DEPTHBUFFERMODE			GetDepthBufferMode() = 0;	
			virtual LYNXFORCEINLINE LYNXDWORD SetColorWriteMask(LYNXDWORD m) {return 0; };
			virtual void					SetSRGBRead(int channel, LYNXBOOL b) = 0;
			virtual void					SetSRGBWrite(LYNXBOOL b) = 0;

			virtual CString&				GetShaderErrorMessage() {return m_ShaderErrorMessage; };
			virtual CString&				GetLastErrorMessage() {return m_LastErrorMessage; };

			//Profile functions ----------------------------------------------------------------------------------------------------------------------------------------------
			LYNXFORCEINLINE void			ResetAllCounters() {RESET_ALL_COUNTERS };
			LYNXFORCEINLINE DWORD			GetDrawCallCounter() {return m_DrawCallCounter; };
			LYNXFORCEINLINE DWORD			GetDrawTimeCounter() {return m_DrawTimeCounter; };
			LYNXFORCEINLINE DWORD			GetPrimitiveCounter() {return m_PrimitiveCounter; };
			LYNXFORCEINLINE DWORD			GetSetRSCounter() {return m_SetRenderStateCounter; };
			LYNXFORCEINLINE DWORD			GetSetTextureCounter() {return m_SetTextureCounter; };
			LYNXFORCEINLINE DWORD			GetSetShaderCounter() {return m_SetShaderCounter; };
			LYNXFORCEINLINE DWORD			GetSetShaderConstantCounter() {return m_SetShaderConstantCounter; };
			LYNXFORCEINLINE DWORD			GetBufferMapCounter() {return m_BufferMapCounter; };			

			CAnsiString&					GetVersion() {return m_Version; }
			CAnsiString&					GetVendor() {return m_Vendor; }
			CAnsiString&					GetRenderer() {return m_Renderer; }

			LYNXFORCEINLINE void			CacheResource(CResourcePtr& p) { m_CachedResourceList.push_back(p); }
			void							ClearCachedResource();
		};			
	}		
}		

#endif