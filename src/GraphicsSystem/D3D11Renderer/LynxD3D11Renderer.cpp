//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2006/11/07
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/LynxBuffer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxCrossSpriteGrassContainer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine
{
	namespace GraphicsSystem
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Include::ComputeCurrentShaderPath(const LYNXCHAR* path)
		{
			LYNXBOOL bFound;
			LYNXCHAR CurrentShaderPath[512];

			bFound = LYNX_FALSE;
			lynxStrCpy(CurrentShaderPath, path);
			for (int i=lynxStrLen(CurrentShaderPath); i>=0; i--)
			{
				if (CurrentShaderPath[i] == L'/')
				{
					CurrentShaderPath[i+1] = NULL;
					bFound = LYNX_TRUE;
					break;
				}
			}
			if (bFound)
				m_CurrentShaderPath = CurrentShaderPath;
			else
				m_CurrentShaderPath = _T("");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CD3D11Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			CString Path = m_CurrentShaderPath + CString(pFileName); 
			CPlatformFileStream Stream;

			if (!Stream.vOpen(Path, (CStream::Read|CStream::Binary)))
				return S_FALSE;

			*pBytes = Stream.vSize();
			(*ppData) = LYNXNEW BYTE[*pBytes];
			Stream.vRead((void*)(*ppData), 1, *pBytes);

			Stream.vClose();

			return S_OK;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CD3D11Include::Close(LPCVOID pData)
		{
			LYNXDEL [] pData;

			return S_OK;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_QUERY CD3D11Renderer::TranslateQueryType(CRenderer::QUERYTYPE type)
		{
			switch (type)
			{
				default:
				case QT_OCCLUSION:
					return D3D11_QUERY_OCCLUSION;
					break;				
				case QT_OCCLUSION_PREDICATE:
					return D3D11_QUERY_OCCLUSION_PREDICATE;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		DXGI_FORMAT	CD3D11Renderer::GetCorrespondingTextureFormat(DXGI_FORMAT f)
		{
			switch (f)
			{
				case DXGI_FORMAT_D24_UNORM_S8_UINT:
				case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
					return DXGI_FORMAT_R24G8_TYPELESS;

				case DXGI_FORMAT_D16_UNORM:				
					return DXGI_FORMAT_R16_TYPELESS;

				default:
					return f;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		DXGI_FORMAT	CD3D11Renderer::GetCorrespondingShaderResourceViewFormat(DXGI_FORMAT f)
		{
			switch (f)
			{
				case DXGI_FORMAT_D24_UNORM_S8_UINT:
				case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
					return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

				case DXGI_FORMAT_D16_UNORM:				
					return DXGI_FORMAT_R16_FLOAT;

				default:
					return f;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		DXGI_FORMAT	CD3D11Renderer::TranslateFormat(FORMAT e, LYNXBOOL bSRGB)
		{
			switch (e)
			{
				case GraphicsSystem::CRenderer::FORMAT_R8G8B8:
					if (bSRGB)
						return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
					else
						return DXGI_FORMAT_R8G8B8A8_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_A8B8G8R8:
				case GraphicsSystem::CRenderer::FORMAT_A8R8G8B8:          
					if (bSRGB)
						return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
					else
						return DXGI_FORMAT_R8G8B8A8_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_X8B8G8R8:
				case GraphicsSystem::CRenderer::FORMAT_X8R8G8B8:           
					return DXGI_FORMAT_B8G8R8X8_UNORM;
					break;
				/*case GraphicsSystem::CRenderer::FORMAT_R5G6B5:             
					Format = _T("R5G6B5");
					break;
				case GraphicsSystem::CRenderer::FORMAT_X1R5G5B5:           
					Format = _T("X1R5G5B5");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A1R5G5B5:           
					Format = _T("A1R5G5B5");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A4R4G4B4:           
					Format = _T("A4R4G4B4");
					break;
				case GraphicsSystem::CRenderer::FORMAT_R3G3B2:             
					Format = _T("R3G3B2");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A8:                 
					Format = _T("A8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A8R3G3B2:           
					Format = _T("A8R3G3B2");
					break;
				case GraphicsSystem::CRenderer::FORMAT_X4R4G4B4:           
					Format = _T("X4R4G4B4");
					break;*/
				case GraphicsSystem::CRenderer::FORMAT_R10G10B10A2:
				case GraphicsSystem::CRenderer::FORMAT_A2B10G10R10:        
					return DXGI_FORMAT_R10G10B10A2_UNORM;
					break;
				/*
				case GraphicsSystem::CRenderer::FORMAT_G16R16:             
					Format = _T("G16R16");
					break;*/
				case GraphicsSystem::CRenderer::FORMAT_A2R10G10B10:        
					return DXGI_FORMAT_R10G10B10A2_UNORM;
					break;
				/*case GraphicsSystem::CRenderer::FORMAT_A16B16G16R16:       
					Format = _T("A16B16G16R16");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A8P8:               
					Format = _T("A8P8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_P8:                 
					Format = _T("P8");
					break;
				*/
				case GraphicsSystem::CRenderer::FORMAT_L8:                 
					return DXGI_FORMAT_R8_UNORM;
					break;
				/*
				case GraphicsSystem::CRenderer::FORMAT_A8L8:               
					Format = _T("A8L8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A4L4:               
					Format = _T("A4L4");
					break;
				case GraphicsSystem::CRenderer::FORMAT_V8U8:               
					Format = _T("V8U8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_L6V5U5:             
					Format = _T("L6V5U5");
					break;
				case GraphicsSystem::CRenderer::FORMAT_X8L8V8U8:           
					Format = _T("X8L8V8U8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_Q8W8V8U8:           
					Format = _T("Q8W8V8U8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_V16U16:             
					Format = _T("V16U16");
					break;
				case GraphicsSystem::CRenderer::FORMAT_A2W10V10U10:        
					Format = _T("A2W10V10U10");
					break;
				case GraphicsSystem::CRenderer::FORMAT_UYVY:               
					Format = _T("UYVY");
					break;
				case GraphicsSystem::CRenderer::FORMAT_R8G8_B8G8:          
					Format = _T("R8G8_B8G8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_YUY2:               
					Format = _T("YUY2");
					break;
				case GraphicsSystem::CRenderer::FORMAT_G8R8_G8B8:          
					Format = _T("G8R8_G8B8");
					break;
				*/
				case GraphicsSystem::CRenderer::FORMAT_DXT1:  
					if (bSRGB)
						return DXGI_FORMAT_BC1_UNORM_SRGB;
					else
						return DXGI_FORMAT_BC1_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_DXT2:               
					if (bSRGB)
						return DXGI_FORMAT_BC2_UNORM_SRGB;
					else
						return DXGI_FORMAT_BC2_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_DXT3:               
					if (bSRGB)
						return DXGI_FORMAT_BC3_UNORM_SRGB;
					else						
						return DXGI_FORMAT_BC3_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_DXT4:               
					return DXGI_FORMAT_BC4_UNORM;
					break;
				case GraphicsSystem::CRenderer::FORMAT_DXT5:   
					return DXGI_FORMAT_BC5_UNORM;
					break;
				/*
				case GraphicsSystem::CRenderer::FORMAT_ATI1N:               
					Format = _T("ATI1N");
					break;
				case GraphicsSystem::CRenderer::FORMAT_ATI2N:  
					Format = _T("ATI2N");
					break;
				case GraphicsSystem::CRenderer::FORMAT_D16_LOCKABLE:       
					Format = _T("D16_LOCKABLE");
					break;*/
				case GraphicsSystem::CRenderer::FORMAT_D32:                
					return DXGI_FORMAT_D32_FLOAT;
					break;
				//case GraphicsSystem::CRenderer::FORMAT_D15S1:              
					//Format = _T("D15S1");
					//break;
				case GraphicsSystem::CRenderer::FORMAT_D24S8:              
					return DXGI_FORMAT_D24_UNORM_S8_UINT;
					break;
				case GraphicsSystem::CRenderer::FORMAT_D24X8:              
					return DXGI_FORMAT_R24_UNORM_X8_TYPELESS ;
					break;
				/*
				case GraphicsSystem::CRenderer::FORMAT_D24X4S4:            
					Format = _T("D24X4S4");
					break;*/
				case GraphicsSystem::CRenderer::FORMAT_D16:                
					return DXGI_FORMAT_D16_UNORM;
					break;
				/*case GraphicsSystem::CRenderer::FORMAT_D32F_LOCKABLE:  
					Format = _T("D32F_LOCKABLE");
					break;
				case GraphicsSystem::CRenderer::FORMAT_D24FS8:            
					Format = _T("D24FS8");
					break;
				case GraphicsSystem::CRenderer::FORMAT_L16:                
					Format = _T("L16");
					break;
				case GraphicsSystem::CRenderer::FORMAT_VERTEXDATA:         
					Format = _T("VERTEXDATA");
					break;
				case GraphicsSystem::CRenderer::FORMAT_INDEX16:            
					Format = _T("INDEX16");
					break;
				case GraphicsSystem::CRenderer::FORMAT_INDEX32:            
					Format = _T("INDEX32");
					break;
				case GraphicsSystem::CRenderer::FORMAT_Q16W16V16U16:       					
					Format = _T("Q16W16V16U16");
					break;
				case GraphicsSystem::CRenderer::FORMAT_MULTI2_ARGB8:       
					Format = _T("MULTI2_ARGB8");
					break;*/
				case GraphicsSystem::CRenderer::FORMAT_R16F:                
					return DXGI_FORMAT_R16_FLOAT;
					break;
				case GraphicsSystem::CRenderer::FORMAT_G16R16F:             
					return DXGI_FORMAT_R16G16_FLOAT;
					break;
				case GraphicsSystem::CRenderer::FORMAT_A16B16G16R16F:       
					return DXGI_FORMAT_R16G16B16A16_FLOAT;
					break;				
				case GraphicsSystem::CRenderer::FORMAT_R32F:                 
					return DXGI_FORMAT_R32_FLOAT;
					break;				
				case GraphicsSystem::CRenderer::FORMAT_G32R32F:              
					return DXGI_FORMAT_R32G32_FLOAT;
					break;
				case GraphicsSystem::CRenderer::FORMAT_A32B32G32R32F:        
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				/*
				case GraphicsSystem::CRenderer::FORMAT_CxV8U8:           
					Format = _T("CxV8U8");
					break;
				*/

				default:
					LYNX_ASSERT(0);
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_FILL_MODE	CD3D11Renderer::TranslateFillMode(CRenderer::FILLMODE e)
		{
			switch (e)
			{
				case CRenderer::FM_WIREFRAME:
					return D3D11_FILL_WIREFRAME;
					break;
				default:
				case CRenderer::FM_SOLID:
					return D3D11_FILL_SOLID;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_CULL_MODE	CD3D11Renderer::TranslateCullMode(CRenderer::CULLMODE e)
		{
			switch (e)
			{
				case CRenderer::CULL_NONE:
					return D3D11_CULL_NONE;
					break;				
				case CRenderer::CULL_CW:
					return D3D11_CULL_FRONT;
					break;
				default:
				case CRenderer::CULL_CCW:
					return D3D11_CULL_BACK;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_COMPARISON_FUNC CD3D11Renderer::TranslateTestFunction(CRenderer::TESTFUNC e)
		{
			switch (e)
			{
				case CRenderer::TF_NEVER:
					return D3D11_COMPARISON_NEVER;
					break;				
				case CRenderer::TF_LESS:
					return D3D11_COMPARISON_LESS;
					break;
				case CRenderer::TF_EQUAL:
					return D3D11_COMPARISON_EQUAL;
					break;
				case CRenderer::TF_LEQUAL:
					return D3D11_COMPARISON_LESS_EQUAL;
					break;				
				case CRenderer::TF_GREATER:
					return D3D11_COMPARISON_GREATER;
					break;
				case CRenderer::TF_NOTEQUAL:
					return D3D11_COMPARISON_NOT_EQUAL;
					break;
				case TF_GEQUAL:
					return D3D11_COMPARISON_GREATER_EQUAL;
					break;
				default:
				case TF_ALWAYS:
					return D3D11_COMPARISON_ALWAYS;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_STENCIL_OP CD3D11Renderer::TranslateStencilOP(CRenderer::STENCILOP e)
		{
			switch (e)
			{
				default:
				case CRenderer::SO_KEEP:
					return D3D11_STENCIL_OP_KEEP;
					break;				
				case CRenderer::SO_ZERO:
					return D3D11_STENCIL_OP_ZERO;
					break;
				case CRenderer::SO_REPLACE:
					return D3D11_STENCIL_OP_REPLACE;
					break;
				case CRenderer::SO_INCR:
					return D3D11_STENCIL_OP_INCR;
					break;				
				case CRenderer::SO_DECR:
					return D3D11_STENCIL_OP_DECR;
					break;				
				case CRenderer::SO_INVERT:
					return D3D11_STENCIL_OP_INVERT;
					break;				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_FILTER CD3D11Renderer::TranslateFilter(CRenderer::FILTERMODE e)
		{
			switch (e)
			{
				default:
				case FM_MIN_MAG_MIP_POINT:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case FM_MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				case FM_MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				case FM_MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				case FM_MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				case FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				case FM_MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				case FM_MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				case FM_ANISOTROPIC:
					return D3D11_FILTER_ANISOTROPIC;
				case FM_COMPARISON_MIN_MAG_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
				case FM_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
				case FM_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
				case FM_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
				case FM_COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
				case FM_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				case FM_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
				case FM_COMPARISON_MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				case FM_COMPARISON_ANISOTROPIC:
					return D3D11_FILTER_COMPARISON_ANISOTROPIC;
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_TEXTURE_ADDRESS_MODE CD3D11Renderer::TranslateAddressMode(CRenderer::ADDRESSMODE e)
		{
			switch (e)
			{
				default:
				case AM_WRAP:
					return D3D11_TEXTURE_ADDRESS_WRAP;
					break;
				case AM_MIRROR:
					return D3D11_TEXTURE_ADDRESS_MIRROR;
					break;
				case AM_CLAMP:
					return D3D11_TEXTURE_ADDRESS_CLAMP;
					break;
				case AM_BORDER:
					return D3D11_TEXTURE_ADDRESS_BORDER;
					break;
				case AM_MIRROR_ONCE:
					return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
					break;
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_BLEND CD3D11Renderer::TranslateBlendFunction(CRenderer::BLENDFUNC e)
		{
			switch (e)
			{
				case BF_ZERO:
					return D3D11_BLEND_ZERO;
					break;
				default:
				case BF_ONE:
					return D3D11_BLEND_ONE;
					break;
				case BF_SRC_COLOR:
					return D3D11_BLEND_SRC_COLOR;
					break;
				case BF_ONE_MINUS_SRC_COLOR:
					return D3D11_BLEND_INV_SRC_COLOR;
					break;
				case BF_SRC_ALPHA:
					return D3D11_BLEND_SRC_ALPHA;
					break;
				case BF_ONE_MINUS_SRC_ALPHA:
					return D3D11_BLEND_INV_SRC_ALPHA;
					break;
				case BF_DEST_ALPHA:
					return D3D11_BLEND_DEST_ALPHA;
					break;
				case BF_ONE_MINUS_DEST_ALPHA:
					return D3D11_BLEND_INV_DEST_ALPHA;
					break;
				case BF_DEST_COLOR:
					return D3D11_BLEND_DEST_COLOR;
					break;
				case BF_ONE_MINUS_DEST_COLOR:
					return D3D11_BLEND_INV_DEST_COLOR;
					break;
				case BF_SRC_ALPHA_SAT:
					return D3D11_BLEND_SRC_ALPHA_SAT;
					break;
				case BF_BOTH_SRC_ALPHA:
					return D3D11_BLEND_SRC_ALPHA;
					break;
				case BF_BOTH_ONE_MINUS_SRC_ALPHA:
					return D3D11_BLEND_INV_SRC_ALPHA;
					break;
				case BF_BLEND_FACTOR:
					return D3D11_BLEND_BLEND_FACTOR;
					break;
				case BF_ONE_MINUS_BLEND_FACTOR:
					return D3D11_BLEND_INV_BLEND_FACTOR;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_BLEND_OP CD3D11Renderer::TranslateBlendOP(CRenderer::BLENDOP e)
		{
			switch (e)
			{
				default:
				case BO_ADD:
					return D3D11_BLEND_OP_ADD;
					break;
				case BO_SUBTRACT:
					return D3D11_BLEND_OP_SUBTRACT;
					break;
				case BO_REVSUBTRACT:
					return D3D11_BLEND_OP_REV_SUBTRACT;
					break;
				case BO_MIN:
					return D3D11_BLEND_OP_MIN;
					break;
				case BO_MAX:
					return D3D11_BLEND_OP_MAX;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3D11_PRIMITIVE_TOPOLOGY CD3D11Renderer::TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e)
		{
			D3D11_PRIMITIVE_TOPOLOGY PType;

			switch (e)
			{
				case CRenderer::POINT_LIST:
					PType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
					break;
				case CRenderer::LINE_LIST:
					PType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
					break;
				case CRenderer::LINE_STRIP:
					PType = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
					break;			
				case CRenderer::TRIANGLE_LIST:
					PType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					break;		
				case CRenderer::TRIANGLE_STRIP:
					PType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
					break;
				case CRenderer::TRIANGLE_FAN:
					PType = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
					break;	
				default:
					PType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					break;	
			}
			return PType;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		DWORD CD3D11Renderer::GetVertexCount(CRenderer::PRIMITIVETYPE e, DWORD primitive_count)
		{
			DWORD VertexCount = 0;

			switch (e)
			{
				case CRenderer::POINT_LIST:
					VertexCount = primitive_count;
					break;
				case CRenderer::LINE_LIST:
					VertexCount = primitive_count*3;
					break;
				case CRenderer::LINE_STRIP:
					VertexCount = primitive_count+2;
					break;			
				case CRenderer::TRIANGLE_LIST:
					VertexCount = primitive_count*3;
					break;		
				case CRenderer::TRIANGLE_STRIP:
					VertexCount = primitive_count+2;
					break;
				case CRenderer::TRIANGLE_FAN:
					VertexCount = primitive_count+2;
					break;	
				default:
					VertexCount = primitive_count;
					break;	
			}
			return VertexCount;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Renderer::CheckSRGB(FORMAT e)
		{
			switch (e)
			{
				case GraphicsSystem::CRenderer::FORMAT_A8R8G8B8:
				case GraphicsSystem::CRenderer::FORMAT_A8B8G8R8:
				case GraphicsSystem::CRenderer::FORMAT_X8R8G8B8:
				case GraphicsSystem::CRenderer::FORMAT_X8B8G8R8:
				case GraphicsSystem::CRenderer::FORMAT_DXT1:  
				case GraphicsSystem::CRenderer::FORMAT_DXT2:               
				case GraphicsSystem::CRenderer::FORMAT_DXT3:               				
					return LYNX_TRUE;
					break;

				default:
					return LYNX_FALSE;				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Renderer::CD3D11Renderer() 
		{
			m_RendererType = D3D11_RENDERER;

			m_DriverList.clear();

			for (int i=0; i<8; i++)
				m_CachedRTView[i] = NULL;
			m_CachedDSView = NULL;

			m_Pixel2TexelOffset.x = 0.0f;
			m_Pixel2TexelOffset.y = 0.0f;

			m_bInvertRenderTarget = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Renderer::CD3D11Renderer(CGraphicsSystem* lpgs):CRenderer(lpgs) 
		{
			m_RendererType = D3D11_RENDERER;

			m_DriverList.clear();

			for (int i=0; i<8; i++)
				m_CachedRTView[i] = NULL;
			m_CachedDSView = NULL;

			m_Pixel2TexelOffset.x = 0.0f;
			m_Pixel2TexelOffset.y = 0.0f;

			m_bInvertRenderTarget = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Renderer::~CD3D11Renderer() 
		{
			for (int i=0; i<m_DriverList.size(); ++i)
				m_DriverList[i].OutputList.clear();
			m_DriverList.clear();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString& CD3D11Renderer::ComputeCachedCompiledShaderPath() 
		{
			#ifdef __WINAPP__
				CString				Target;
				LYNXCHAR			Buffer[16];
			
				Target = LynxEngine::GetEnumStr(m_ShaderTarget);
				m_CachedCompiledShaderPath = CString(_T("./cached/graphics/")) + CString(GetAPIString()) + CString(_T("/")) + CString(GetDriverDesc()) + CString(_T("/shader/")) + Target + CString(_T("/"));			
				m_ShaderSrcPath.clear();
				m_ShaderSrcPath.push_back(CString(_T("../../engine/shader/d3d/")));			
				m_ShaderSrcPath.push_back(CString(_T("../engine/shader/d3d/")));			
				m_ShaderSrcPath.push_back(CString(_T("../shader/d3d/")));			

				m_UserShaderSrcPath = _T("./shader/d3d/");

				m_ShaderMacros.clear();
				switch (m_ShaderTarget)
				{
					case SHADER_TARGET_11:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("10"));
						break;
					case SHADER_TARGET_20:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("20"));
						break;
					case SHADER_TARGET_2X:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("21"));
						break;
					case SHADER_TARGET_30:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("30"));
						break;
					case SHADER_TARGET_40:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("40"));
						break;
					case SHADER_TARGET_41:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("41"));
						break;
					case SHADER_TARGET_50:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("50"));
						break;
					default:
						AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("30"));
						break;
				}			
				//AddDefinedValue(CAnsiString("PCF_1X1"), CAnsiString("1"));
				AddDefinedValue(CAnsiString("PCF_4X4"), CAnsiString("1"));
				//AddDefinedValue(CAnsiString("PCF_2X2"), CAnsiString("1"));

				lynxSprintf(Buffer, _T("%d"), CSpriteGrassContainer::m_BatchSize);
				AddDefinedValue(CAnsiString("GRASS_BATCH_SIZE"), CAnsiString(Buffer));

				lynxSprintf(Buffer, _T("%d"), CCrossSpriteGrassContainer::m_BatchSize);
				AddDefinedValue(CAnsiString("CROSSGRASS_BATCH_SIZE"), CAnsiString(Buffer));			

				lynxSprintf(Buffer, _T("%d"), MAX_NUM_TEXTURE_CHANNEL);
				AddDefinedValue(CAnsiString("MAX_TEXTURE_CHANNELS"), CAnsiString(Buffer));						
			
				return m_CachedCompiledShaderPath; 
			#else
				return CRenderer::ComputeCachedCompiledShaderPath();
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::AddDriver(int index, IDXGIAdapter* lpa)
		{
			DRIVERDATA				Data;
			LYNXCHAR				DescStr[512];
			DXGI_ADAPTER_DESC		Desc;
			IDXGIOutput*			Output;
			DXGI_OUTPUT_DESC		OutputDesc;

			Data.lpAdapter = lpa;
			Data.lpAdapter->GetDesc(&Desc);
			lynxFromUniStr(DescStr, Desc.Description);
			// Kill the all "space" character at the end of the string
			for (int j=lynxStrLen(DescStr)-1; j>=0; j--)
			{
				if (DescStr[j] == L' ')
					DescStr[j] = 0x00;
				else
					break;
			}
			Data.Desc = DescStr;
			Data.DedicatedSystemMemory = Desc.DedicatedSystemMemory;
			Data.DedicatedVideoMemory = Desc.DedicatedVideoMemory;
			Data.SharedSystemMemory = Desc.SharedSystemMemory;
			Data.VendorId = Desc.VendorId;
			Data.DeviceId = Desc.DeviceId;
			Data.Revision = Desc.Revision;
			Data.SubSysId = Desc.SubSysId;				

			LYNXCHAR Buffer[512];
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] Description : %s\r\n"), index, Data.Desc.c_str());
			LYNX_LOG(glpLogger, CString(Buffer));
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] VendorID : %d\r\n"), index, Desc.VendorId);
			LYNX_LOG(glpLogger, CString(Buffer));
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] DeviceID : %d\r\n"), index, Desc.DeviceId);
			LYNX_LOG(glpLogger, CString(Buffer));
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] DedicatedVideoMemory : %.1fMB\r\n"), index, (float)Desc.DedicatedVideoMemory/(1024.0f*1024.0f));
			LYNX_LOG(glpLogger, CString(Buffer));
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] DedicatedSystemMemory : %.1fMB\r\n"), index, (float)Desc.DedicatedSystemMemory/(1024.0f*1024.0f));
			LYNX_LOG(glpLogger, CString(Buffer));
			lynxSprintf(Buffer, _T("\tDirect3D driver[%d] SharedSystemMemory : %.1fMB\r\n"), index, (float)Desc.SharedSystemMemory/(1024.0f*1024.0f));
			LYNX_LOG(glpLogger, CString(Buffer));
							
			int OutputIndex = 0;
			while (Data.lpAdapter->EnumOutputs(OutputIndex, &Output) != DXGI_ERROR_NOT_FOUND)
			{
				Output->GetDesc(&OutputDesc);
				lynxFromUniStr(DescStr, OutputDesc.DeviceName);
				Data.OutputList.push_back(Output);
				lynxSprintf(Buffer, _T("\t\tOutput[%d] : Name : %s\r\n"), OutputIndex, DescStr);
				LYNX_LOG(glpLogger, CString(Buffer));					
				++OutputIndex;
			}	
			m_DriverList.push_back(Data);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::FindAllDrivers()
		{
			IDXGIAdapter*			lpAdapter;

			for (int i=0; i<m_DriverList.size(); ++i)
				m_DriverList[i].OutputList.clear();
			m_DriverList.clear();

			int DriverIndex = 0;
			while (m_lpDXGIFactory->EnumAdapters(DriverIndex, &lpAdapter) != DXGI_ERROR_NOT_FOUND)
			{
				AddDriver(DriverIndex, lpAdapter);
				++DriverIndex;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::FindAllDisplayModes(FORMAT f, int driver, int output)
		{
			LYNXCHAR Buffer[512];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all display mode...\r\n")));

			lynxSprintf(Buffer, _T("Current driver[%d] : %s, Format : %s\r\n"), driver, m_DriverList[driver].Desc.c_str(), GetEnumStr(f).c_str());
			LYNX_LOG(glpLogger, CString(Buffer));								

			UINT NumVideoModes = 0;
			m_DriverList[driver].OutputList[output]->GetDisplayModeList(TranslateFormat(f), DXGI_ENUM_MODES_INTERLACED|DXGI_ENUM_MODES_SCALING, &NumVideoModes, NULL);
			m_VideoModeList.resize(NumVideoModes);
			m_DisplayModeList.resize(NumVideoModes);

			DXGI_MODE_DESC *lpModeDesc = LYNXNEW DXGI_MODE_DESC[NumVideoModes];
			m_DriverList[driver].OutputList[output]->GetDisplayModeList(TranslateFormat(f), DXGI_ENUM_MODES_INTERLACED|DXGI_ENUM_MODES_SCALING, &NumVideoModes, lpModeDesc);
			for (int i=0; i<NumVideoModes; i++)
			{
				m_VideoModeList[i].RefreshRate = lpModeDesc[i].RefreshRate.Numerator;
				m_VideoModeList[i].Width = lpModeDesc[i].Width;
				m_VideoModeList[i].Height = lpModeDesc[i].Height;
				//m_VideoModeList[i].Format = lpModeDesc[i].Format;				
				m_VideoModeList[i].bpp = 32;				
				lynxMemCpy(&m_DisplayModeList[i], &lpModeDesc[i], sizeof(DXGI_MODE_DESC));
				lynxSprintf(Buffer, _T("\tDisplay mode[%d] : %dx%dx%d@%d\n"), i, m_VideoModeList[i].Width, m_VideoModeList[i].Height, m_VideoModeList[i].bpp, m_VideoModeList[i].RefreshRate);
				LYNX_LOG(glpLogger, CString(Buffer));								
			}
			LYNXDEL lpModeDesc;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all display mode OK.\r\n")));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Renderer::Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp)
		{
			LYNXCHAR Str[512], Buffer[512];
			D3D_DRIVER_TYPE DriverType ;

			m_Window = wnd; 

			#ifdef __WINAPP__
				// This flag adds support for surfaces with a different color channel ordering than the API default.
				// It is recommended usage, and is required for compatibility with Direct2D.
				UINT CreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

				#if defined(_DEBUG)
					// If the project is in a debug build, enable debugging via SDK Layers with this flag.
					CreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
				#endif

				// This array defines the set of DirectX hardware feature levels this app will support.
				// Note the ordering should be preserved.
				// Don't forget to declare your application's minimum required feature level in its
				// description.  All applications are assumed to support 9.1 unless otherwise stated.
				D3D_FEATURE_LEVEL FeatureLevels[] = 
				{
					D3D_FEATURE_LEVEL_11_1,
					D3D_FEATURE_LEVEL_11_0,
					D3D_FEATURE_LEVEL_10_1,
					D3D_FEATURE_LEVEL_10_0,
					D3D_FEATURE_LEVEL_9_3,
					D3D_FEATURE_LEVEL_9_2,
					D3D_FEATURE_LEVEL_9_1
				};

				ID3D11Device* lpD3DDevice = NULL;
				ID3D11DeviceContext *lpD3DContext  = NULL;
				// Create the DX11 API device object, and get a corresponding context.
				HRESULT hResult = D3D11CreateDevice(
					NULL,						// specify null to use the default adapter
					D3D_DRIVER_TYPE_HARDWARE,
					NULL,	                    // leave as nullptr unless software device
					CreationFlags,              // optionally set debug and Direct2D compatibility flags
					FeatureLevels,              // list of feature levels this app can support
					ARRAYSIZE(FeatureLevels),   // number of entries in above list
					D3D11_SDK_VERSION,          // always set this to D3D11_SDK_VERSION
					&lpD3DDevice,				// returns the Direct3D device created
					NULL,				        // returns feature level of device created
					&lpD3DContext				// returns the device immediate context
					);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to create D3D Device(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}
				hResult = lpD3DDevice->QueryInterface(__uuidof(ID3D11Device1), (void **)&m_lpD3DDevice);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to get ID3D11Device1 interface(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}
				hResult = lpD3DContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&m_lpImmediateContext);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to get ID3D11DeviceContext1 interface(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}

				// Create a descriptor for the swap chain.
				DXGI_SWAP_CHAIN_DESC1 SCDesc = {0};
				SCDesc.Width = lpcp->Width;
				SCDesc.Height = lpcp->Height;
				SCDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//TranslateFormat(lpcp->FrameBufferFormat);
				SCDesc.Stereo = false; 
				if (lpcp->NumMultiSamples == 0)
					SCDesc.SampleDesc.Count = 1;
				else
					SCDesc.SampleDesc.Count = lpcp->NumMultiSamples;
				SCDesc.SampleDesc.Quality = 0;
				SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				SCDesc.BufferCount = 2;
				SCDesc.Scaling = DXGI_SCALING_NONE;
				SCDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // we recommend using this swap effect for all applications
				SCDesc.Flags = 0;

				/*
				SCDesc.Width = 0;                                     // use automatic sizing
				SCDesc.Height = 0;
				SCDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // this is the most common swapchain format
				SCDesc.Stereo = false; 
				SCDesc.SampleDesc.Count = 1;                          // don't use multi-sampling
				SCDesc.SampleDesc.Quality = 0;
				SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				SCDesc.BufferCount = 2;                               // use two buffers to enable flip effect
				SCDesc.Scaling = DXGI_SCALING_NONE;
				SCDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // we recommend using this swap effect for all applications
				SCDesc.Flags = 0;
				*/

				// Once the desired swap chain description is configured, it must be created on the same adapter as our D3D Device
				// First, retrieve the underlying DXGI Device from the D3D Device
				IDXGIDevice1* dxgiDevice;
				hResult = m_lpD3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&dxgiDevice);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to get IDXGIDevice1 interface(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}
				
				// Identify the physical adapter (GPU or card) this device is running on.
				IDXGIAdapter* dxgiAdapter;
				hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to get adapter interface(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}

				m_CurrentDriver = 0;
				AddDriver(m_CurrentDriver, dxgiAdapter);

				// And obtain the factory object that created it.
				hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void **)&m_lpDXGIFactory);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to get IDXGIFactory2 interface(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}

				// Create the swapchain
				hResult = m_lpDXGIFactory->CreateSwapChainForCoreWindow(
					m_lpD3DDevice, 
					reinterpret_cast<IUnknown*>(m_Window),
					&SCDesc, 
					NULL,
					&m_lpSwapChain);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to create swap chain(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}
				    
				// Ensure that DXGI does not queue more than one frame at a time. This both reduces 
				// latency and ensures that the application will only render after each VSync, minimizing 
				// power consumption.
				dxgiDevice->SetMaximumFrameLatency(1);				
			#else
				CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_lpDXGIFactory));
				FindAllDrivers();
				m_CurrentDriver = 0;
				FindAllDisplayModes(lpcp->FrameBufferFormat, m_CurrentDriver);

				if (lpcp->bFullScreen)
					lynxOSMoveWindow(m_Window, 0, 0, lpcp->Width, lpcp->Height, LYNX_TRUE);
				else			
					lynxOSMoveWindow(m_Window, 0, 0, lpcp->Width+GetSystemMetrics(SM_CYFRAME)*2, lpcp->Height+GetSystemMetrics(SM_CYFRAME)*2, LYNX_TRUE);			
			
				m_lpDXGIFactory->MakeWindowAssociation(m_Window, 0);
				switch (lpcp->RenderDeviceType)
				{
					case RDT_NULL:
						DriverType = D3D_DRIVER_TYPE_NULL;
						break;
					case RDT_REFERNCE:
						DriverType = D3D_DRIVER_TYPE_REFERENCE;
						break;
					case RDT_SOFTWARE:
						DriverType = D3D_DRIVER_TYPE_SOFTWARE;
						break;
					case RDT_HARDWARE:
						DriverType = D3D_DRIVER_TYPE_HARDWARE;
						break;
					case RDT_DEFAULT:
						DriverType = D3D_DRIVER_TYPE_UNKNOWN;
						break;				
				}
				D3D_FEATURE_LEVEL FeatureLevel[4] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3};
				D3D11_CREATE_DEVICE_FLAG DeviceFlag = (D3D11_CREATE_DEVICE_FLAG)(D3D11_CREATE_DEVICE_SINGLETHREADED);			
				HRESULT hResult = D3D11CreateDevice(m_DriverList[0].lpAdapter, DriverType, (HMODULE)NULL, DeviceFlag, FeatureLevel, 4, D3D11_SDK_VERSION, &m_lpD3DDevice, NULL, &m_lpImmediateContext);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to create D3D Device(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}

				DXGI_SWAP_CHAIN_DESC		SCDesc = {0};
				DXGI_MODE_DESC				DesiredDesc;			
			
				SCDesc.BufferDesc.Width  = lpcp->Width;
				SCDesc.BufferDesc.Height = lpcp->Height;
				SCDesc.BufferDesc.RefreshRate.Numerator = 60;
				SCDesc.BufferDesc.RefreshRate.Denominator = 1;
				SCDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//TranslateFormat(lpcp->FrameBufferFormat);
				if (lpcp->NumMultiSamples == 0)
					SCDesc.SampleDesc.Count = 1;
				else
					SCDesc.SampleDesc.Count = lpcp->NumMultiSamples;
				SCDesc.SampleDesc.Quality = 0;
				SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				SCDesc.BufferCount = 1;
				SCDesc.OutputWindow = m_Window;
				SCDesc.Windowed = !lpcp->bFullScreen;
				SCDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			
		
				// Create the swapchain
				hResult = m_lpDXGIFactory->CreateSwapChain(m_lpD3DDevice, &SCDesc, &m_lpSwapChain);
				if (hResult != S_OK)   
				{
					lynxStrCpy(Str, DXGetErrorString(hResult));
					lynxSprintf(Buffer, _T("::Error : Failed to create swap chain(D3D Error :  %s\r\n"), Str);
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
					return LYNX_FALSE;
				}
			#endif

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
			m_lpBackBuffer->SetName(_T("SystemBackBuffer"));						
			m_lpBackBuffer->vSetSize(lpcp->Width, lpcp->Height);
			ID3D11Resource* pBackBuffer = NULL;
			hResult = m_lpSwapChain->GetBuffer(0, __uuidof(*pBackBuffer), (LPVOID*)&pBackBuffer);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("::Error : Failed to create system back buffer(D3D Error :  %s\r\n"), Str);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
				return LYNX_FALSE;
			}
			((CD3D11Buffer*)((CBuffer*)m_lpBackBuffer))->SetlpResource(pBackBuffer);

			// Create the render target view
			ID3D11RenderTargetView* pRTV = NULL;
			hResult = m_lpD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("::Error : Failed to create render target view for system back buffer(D3D Error :  %s\r\n"), Str);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
				return LYNX_FALSE;
			}
			((CD3D11Buffer*)((CBuffer*)m_lpBackBuffer))->SetlpRTView(pRTV);

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);						
			CreateDepthBuffer(m_lpDepthBuffer, lpcp->Width, lpcp->Height, lpcp->DepthBufferFormat, SCDesc.SampleDesc.Count);			
			m_lpDepthBuffer->SetName(_T("SystemDepthBuffer") );

			m_lpOffscreenDepthBuffer = m_lpDepthBuffer;

			SetRenderBuffer(0, m_lpBackBuffer);
			SetDepthBuffer(m_lpDepthBuffer);
			
			GetlpDeviceContext()->VSSetShader(NULL, NULL, 0);
			GetlpDeviceContext()->GSSetShader(NULL, NULL, 0);
			GetlpDeviceContext()->HSSetShader(NULL, NULL, 0);
			GetlpDeviceContext()->DSSetShader(NULL, NULL, 0);
			GetlpDeviceContext()->PSSetShader(NULL, NULL, 0);
			
			SetViewportRect(0, 0, lpcp->Width, lpcp->Height);
			SetViewportRect(0, 0, lpcp->Width, lpcp->Height);			
			SetScissorRect(0, 0, lpcp->Width, lpcp->Height);
			SetScissorRect(0, 0, lpcp->Width, lpcp->Height);

			// Create default system font 	
			/*#if	defined(_UNICODE) || defined(UNICODE)
				D3DXCreateFont(m_lpD3DDevice,
								20, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, 
								OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
								_T("Arial Unicode MS"), &m_lpDefaultFont);
			#else
				D3DXCreateFont(m_lpD3DDevice,
								16, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, 
								OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
								_T("Arial"), &m_lpDefaultFont);
			#endif		*/

			m_Vendor = "Microsoft";
			m_Renderer = "D3D11";

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::Close()
		{
			CRenderer::Close();

			LYNXDEL m_lpBackBuffer;
			LYNXDEL m_lpDepthBuffer;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Restore FPCW...\r\n")));			
			lynxRestoreFPCW();							
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::SetScissorRect(int x1, int y1, int x2, int y2)
		{	
			D3D11_RECT Rect;

			Rect.left	= x1;
			Rect.right	= x2;
			Rect.top	= y1;
			Rect.bottom	= y2;

			m_lpImmediateContext->RSSetScissorRects(1, &Rect);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::SetViewportRect(int x1, int y1, int x2, int y2) 
		{
			CRenderer::SetViewportRect(x1, y1, x2, y2);

			static D3D11_VIEWPORT	D3DViewport;

			D3DViewport.TopLeftX = (DWORD)x1;
			D3DViewport.TopLeftY = (DWORD)y1;
			D3DViewport.Width = (DWORD)(x2 - x1);
			D3DViewport.Height = (DWORD)(y2 - y1);
			D3DViewport.MinDepth = 0.0f;
			D3DViewport.MaxDepth = 1.0f;

			m_lpImmediateContext->RSSetViewports(1, &D3DViewport);	
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) 
		{
			START_COUNTER 
				
			m_lpImmediateContext->IASetPrimitiveTopology(TranslatePrimitiveType(type));
			m_lpImmediateContext->DrawIndexed(GetVertexCount(type, primitive_count), base_index, base_vert);

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, primitive_count)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert) 
		{
			START_COUNTER 
			
			m_lpImmediateContext->IASetPrimitiveTopology(TranslatePrimitiveType(lpp->Type));
			m_lpImmediateContext->DrawIndexed(GetVertexCount(lpp->Type, lpp->NumPrimitives), lpp->BaseIndex, lpp->BaseVertex);

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, lpp->NumPrimitives)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert) 
		{
			START_COUNTER 
				
			m_lpImmediateContext->IASetPrimitiveTopology(TranslatePrimitiveType(type));
			m_lpImmediateContext->Draw(GetVertexCount(type, primitive_count), base_vert);
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1)  
			INC_COUNTER(m_PrimitiveCounter, primitive_count)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Renderer::BltBuffer(CBuffer* lpdest, LYNXRECT* drect, CBuffer* lpsrc, LYNXRECT* srect)
		{
			if (lpdest == lpsrc)
				return;

			SetRenderBuffer(0, lpdest);
			SetViewportRect(0, 0, lpdest->vGetWidth(), lpdest->vGetHeight());
			m_lpGraphicsSystem->SetupScreenSpaceQuadUVOffset(&(m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
			m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			CD3D11PixelShader* lpPS = (CD3D11PixelShader*)(m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetPS()->GetlpShader());
			if (lpsrc->GetlpTexture())
				lpPS->vSetTexture(0, lpsrc->GetlpTexture());
			m_lpGraphicsSystem->DrawScreenSpaceQuad();			

			//m_lpImmediateContext->CopyResource(((CD3D11Buffer*)lpdest)->GetlpResource(), ((CD3D11Buffer*)lpsrc)->GetlpResource());
		}
	}
}

#endif