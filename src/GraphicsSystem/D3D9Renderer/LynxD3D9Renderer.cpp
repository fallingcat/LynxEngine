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
//  èªªæ?:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/LynxBuffer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxCrossSpriteGrassContainer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine
{
	namespace GraphicsSystem
	{			
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Include::ComputeCurrentShaderPath(const LYNXCHAR* path)
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
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CD3D9Include::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			CString Path = CString(_T("C:\\Users\\Owen\\Documents\\Visual Studio 11\\Projects\\Direct3DApplication2\\Debug\\Direct3DApplication2\\AppX\\engine\\shader\\d3d\\vs\\common\\")) + CString(pFileName); 
			CPlatformFileStream Stream;

			if (!Stream.vOpen(Path, (CStream::Read|CStream::Binary)))
				return S_FALSE;

			*pBytes = Stream.vSize();
			(*ppData) = LYNXALLOC((*pBytes));
			Stream.vRead((void*)(*ppData), 1, *pBytes);

			Stream.vClose();

			return S_OK;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CD3D9Include::Close(LPCVOID pData)
		{
			LYNXFREE((void*)pData);

			return S_OK;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DQUERYTYPE CD3D9Renderer::TranslateQueryType(CRenderer::QUERYTYPE type)
		{
			switch (type)
			{
				default:
				case QT_OCCLUSION:
					return D3DQUERYTYPE_OCCLUSION;
					break;				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DPRIMITIVETYPE CD3D9Renderer::TranslatePrimitiveType(CRenderer::PRIMITIVETYPE e)
		{
			D3DPRIMITIVETYPE PType;

			switch (e)
			{
				case CRenderer::POINT_LIST:
					PType = D3DPT_POINTLIST;
					break;
				case CRenderer::LINE_LIST:
					PType = D3DPT_LINELIST;
					break;
				case CRenderer::LINE_STRIP:
					PType = D3DPT_LINESTRIP;
					break;			
				case CRenderer::TRIANGLE_LIST:
					PType = D3DPT_TRIANGLELIST;
					break;		
				case CRenderer::TRIANGLE_STRIP:
					PType = D3DPT_TRIANGLESTRIP;
					break;
				case CRenderer::TRIANGLE_FAN:
					PType = D3DPT_TRIANGLEFAN;
					break;	
				default:
					PType = D3DPT_TRIANGLELIST;
					break;	
			}
			return PType;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DFORMAT CD3D9Renderer::TranslateFormat(CRenderer::FORMAT f)
		{
			switch (f)
			{
				default:
				case FORMAT_NULL:
					return D3DFMT_UNKNOWN;
					break;
				case FORMAT_R8G8B8:
					return D3DFMT_R8G8B8;
					break;
				case FORMAT_A8R8G8B8:
					return D3DFMT_A8R8G8B8;          
					break;
				case FORMAT_X8R8G8B8:
					return D3DFMT_X8R8G8B8;           
					break;
				case FORMAT_R5G6B5:
					return D3DFMT_R5G6B5;           
					break;
				case FORMAT_X1R5G5B5:
					return D3DFMT_X1R5G5B5;           
					break;
				case FORMAT_A1R5G5B5:
					return D3DFMT_A1R5G5B5;           
					break;
				case FORMAT_A4R4G4B4:
					return D3DFMT_A4R4G4B4;           
					break;
				case FORMAT_R3G3B2:
					return D3DFMT_R3G3B2;           
					break;
				case FORMAT_A8:
					return D3DFMT_A8;           
					break;
				case FORMAT_A8R3G3B2:
					return D3DFMT_A8R3G3B2;           
					break;
				case FORMAT_X4R4G4B4:
					return D3DFMT_X4R4G4B4;           
					break;
				case FORMAT_A2B10G10R10:
					return D3DFMT_A2B10G10R10;           
					break;
				case FORMAT_A8B8G8R8:
					return D3DFMT_A8B8G8R8;           
					break;
				case FORMAT_X8B8G8R8:
					return D3DFMT_X8B8G8R8;           
					break;
				case FORMAT_G16R16:
					return D3DFMT_G16R16;           
					break;
				case FORMAT_A2R10G10B10:
					return D3DFMT_A2R10G10B10;           
					break;
				case FORMAT_A16B16G16R16:
					return D3DFMT_A16B16G16R16;           
					break;
				case FORMAT_A8P8:
					return D3DFMT_A8P8;           
					break;
				case FORMAT_P8:
					return D3DFMT_P8;           
					break;
				case FORMAT_L8:
					return D3DFMT_L8;           
					break;                 
				case FORMAT_A8L8:
					return D3DFMT_A8L8;           
					break;
				case FORMAT_A4L4:
					return D3DFMT_A4L4;           
					break;
				case FORMAT_V8U8:
					return D3DFMT_V8U8;           
					break;
				case FORMAT_L6V5U5:
					return D3DFMT_L6V5U5;           
					break;
				case FORMAT_X8L8V8U8:
					return D3DFMT_X8L8V8U8;           
					break;
				case FORMAT_Q8W8V8U8:
					return D3DFMT_Q8W8V8U8;           
					break;
				case FORMAT_V16U16:
					return D3DFMT_V16U16;           
					break;
				case FORMAT_A2W10V10U10:
					return D3DFMT_A2W10V10U10;           
					break;
				case FORMAT_UYVY:
					return D3DFMT_UYVY;           
					break;
				case FORMAT_R8G8_B8G8:
					return D3DFMT_R8G8_B8G8;           
					break;
				case FORMAT_YUY2:
					return D3DFMT_YUY2;           
					break;
				case FORMAT_G8R8_G8B8:
					return D3DFMT_G8R8_G8B8;           
					break;
				case FORMAT_DXT1:
					return D3DFMT_DXT1;           
					break;
				case FORMAT_DXT2:
					return D3DFMT_DXT2;           
					break;
				case FORMAT_DXT3:
					return D3DFMT_DXT3;           
					break;
				case FORMAT_DXT4:
					return D3DFMT_DXT4;           
					break;
				case FORMAT_DXT5:
					return D3DFMT_DXT5;           
					break;
				case FORMAT_ATI1N:
					return (D3DFORMAT) MAKEFOURCC('A', 'T', 'I', '1');// ATI1N;           
					break;
				case FORMAT_ATI2N:
					return (D3DFORMAT) MAKEFOURCC('A', 'T', 'I', '2');// ATI2N;           
					break;
				case FORMAT_D16_LOCKABLE:
					return D3DFMT_D16_LOCKABLE;           
					break;
				case FORMAT_D32:
					return D3DFMT_D32;           
					break;
				case FORMAT_D15S1:
					return D3DFMT_D15S1;           
					break;
				case FORMAT_D24S8:
					return D3DFMT_D24S8;           
					break;
				case FORMAT_D24X8:
					return D3DFMT_D24X8;           
					break;
				case FORMAT_D24X4S4:
					return D3DFMT_D24X4S4;           
					break;
				case FORMAT_D16:
					return D3DFMT_D16;           
					break;
				case FORMAT_D32F_LOCKABLE:
					return D3DFMT_D32F_LOCKABLE;           
					break;      
				case FORMAT_D24FS8:
					return D3DFMT_D24FS8;           
					break;
				case FORMAT_L16:
					return D3DFMT_L16;           
					break;
				case FORMAT_VERTEXDATA:
					return D3DFMT_VERTEXDATA;           
					break;
				case FORMAT_INDEX16:
					return D3DFMT_INDEX16;           
					break;
				case FORMAT_INDEX32:
					return D3DFMT_INDEX32;           
					break;
				case FORMAT_Q16W16V16U16:
					return D3DFMT_Q16W16V16U16;           
					break;
				case FORMAT_MULTI2_ARGB8:
					return D3DFMT_MULTI2_ARGB8;           
					break;
				case FORMAT_R16F:
					return D3DFMT_R16F;           
					break;
				case FORMAT_G16R16F:
					return D3DFMT_G16R16F;           
					break;
				case FORMAT_A16B16G16R16F:
					return D3DFMT_A16B16G16R16F;           
					break;
				case FORMAT_R32F:
					return D3DFMT_R32F;           
					break;
				case FORMAT_G32R32F:
					return D3DFMT_G32R32F;           
					break;
				case FORMAT_A32B32G32R32F:
					return D3DFMT_A32B32G32R32F;           
					break;
				case FORMAT_CxV8U8:
					return D3DFMT_CxV8U8;           
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::FORMAT CD3D9Renderer::TranslateFormat(D3DFORMAT f)
		{
			switch (f)
			{
				default:
				case D3DFMT_UNKNOWN:
					return FORMAT_NULL;
					break;
				case D3DFMT_R8G8B8:
					return FORMAT_R8G8B8;
					break;
				case D3DFMT_A8R8G8B8:
					return FORMAT_A8R8G8B8;          
					break;
				case D3DFMT_X8R8G8B8:
					return FORMAT_X8R8G8B8;           
					break;
				case D3DFMT_R5G6B5:
					return FORMAT_R5G6B5;           
					break;
				case D3DFMT_X1R5G5B5:
					return FORMAT_X1R5G5B5;           
					break;
				case D3DFMT_A1R5G5B5:
					return FORMAT_A1R5G5B5;           
					break;
				case D3DFMT_A4R4G4B4:
					return FORMAT_A4R4G4B4;           
					break;
				case D3DFMT_R3G3B2:
					return FORMAT_R3G3B2;           
					break;
				case D3DFMT_A8:
					return FORMAT_A8;           
					break;
				case D3DFMT_A8R3G3B2:
					return FORMAT_A8R3G3B2;           
					break;
				case D3DFMT_X4R4G4B4:
					return FORMAT_X4R4G4B4;           
					break;
				case D3DFMT_A2B10G10R10:
					return FORMAT_A2B10G10R10;           
					break;
				case D3DFMT_A8B8G8R8:
					return FORMAT_A8B8G8R8;           
					break;
				case D3DFMT_X8B8G8R8:
					return FORMAT_X8B8G8R8;           
					break;
				case D3DFMT_G16R16:
					return FORMAT_G16R16;           
					break;
				case D3DFMT_A2R10G10B10:
					return FORMAT_A2R10G10B10;           
					break;
				case D3DFMT_A16B16G16R16:
					return FORMAT_A16B16G16R16;           
					break;
				case D3DFMT_A8P8:
					return FORMAT_A8P8;           
					break;
				case D3DFMT_P8:
					return FORMAT_P8;           
					break;
				case D3DFMT_L8:
					return FORMAT_L8;           
					break;                 
				case D3DFMT_A8L8:
					return FORMAT_A8L8;           
					break;
				case D3DFMT_A4L4:
					return FORMAT_A4L4;           
					break;
				case D3DFMT_V8U8:
					return FORMAT_V8U8;           
					break;
				case D3DFMT_L6V5U5:
					return FORMAT_L6V5U5;           
					break;
				case D3DFMT_X8L8V8U8:
					return FORMAT_X8L8V8U8;           
					break;
				case D3DFMT_Q8W8V8U8:
					return FORMAT_Q8W8V8U8;           
					break;
				case D3DFMT_V16U16:
					return FORMAT_V16U16;           
					break;
				case D3DFMT_A2W10V10U10:
					return FORMAT_A2W10V10U10;           
					break;
				case D3DFMT_UYVY:
					return FORMAT_UYVY;           
					break;
				case D3DFMT_R8G8_B8G8:
					return FORMAT_R8G8_B8G8;           
					break;
				case D3DFMT_YUY2:
					return FORMAT_YUY2;           
					break;
				case D3DFMT_G8R8_G8B8:
					return FORMAT_G8R8_G8B8;           
					break;
				case D3DFMT_DXT1:
					return FORMAT_DXT1;           
					break;
				case D3DFMT_DXT2:
					return FORMAT_DXT2;           
					break;
				case D3DFMT_DXT3:
					return FORMAT_DXT3;           
					break;
				case D3DFMT_DXT4:
					return FORMAT_DXT4;           
					break;
				case D3DFMT_DXT5:
					return FORMAT_DXT5;           
					break;
				case (D3DFORMAT) MAKEFOURCC('A', 'T', 'I', '1'):
					return FORMAT_ATI1N;// ATI1N;           
					break;
				case (D3DFORMAT) MAKEFOURCC('A', 'T', 'I', '2'):
					return FORMAT_ATI2N;// ATI2N;           
					break;
				case D3DFMT_D16_LOCKABLE:
					return FORMAT_D16_LOCKABLE;           
					break;
				case D3DFMT_D32:
					return FORMAT_D32;           
					break;
				case D3DFMT_D15S1:
					return FORMAT_D15S1;           
					break;
				case D3DFMT_D24S8:
					return FORMAT_D24S8;           
					break;
				case D3DFMT_D24X8:
					return FORMAT_D24X8;           
					break;
				case D3DFMT_D24X4S4:
					return FORMAT_D24X4S4;           
					break;
				case D3DFMT_D16:
					return FORMAT_D16;           
					break;
				case D3DFMT_D32F_LOCKABLE:
					return FORMAT_D32F_LOCKABLE;           
					break;      
				case D3DFMT_D24FS8:
					return FORMAT_D24FS8;           
					break;
				case D3DFMT_L16:
					return FORMAT_L16;           
					break;
				case D3DFMT_VERTEXDATA:
					return FORMAT_VERTEXDATA;           
					break;
				case D3DFMT_INDEX16:
					return FORMAT_INDEX16;           
					break;
				case D3DFMT_INDEX32:
					return FORMAT_INDEX32;           
					break;
				case D3DFMT_Q16W16V16U16:
					return FORMAT_Q16W16V16U16;           
					break;
				case D3DFMT_MULTI2_ARGB8:
					return FORMAT_MULTI2_ARGB8;           
					break;
				case D3DFMT_R16F:
					return FORMAT_R16F;           
					break;
				case D3DFMT_G16R16F:
					return FORMAT_G16R16F;           
					break;
				case D3DFMT_A16B16G16R16F:
					return FORMAT_A16B16G16R16F;           
					break;
				case D3DFMT_R32F:
					return FORMAT_R32F;           
					break;
				case D3DFMT_G32R32F:
					return FORMAT_G32R32F;           
					break;
				case D3DFMT_A32B32G32R32F:
					return FORMAT_A32B32G32R32F;           
					break;
				case D3DFMT_CxV8U8:
					return FORMAT_CxV8U8;           
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DBLEND CD3D9Renderer::TranslateBlendFunction(CRenderer::BLENDFUNC e)
		{
			switch (e)
			{
				case BF_ZERO:
					return D3DBLEND_ZERO;
					break;
				default:
				case BF_ONE:
					return D3DBLEND_ONE;
					break;
				case BF_SRC_COLOR:
					return D3DBLEND_SRCCOLOR;
					break;
				case BF_ONE_MINUS_SRC_COLOR:
					return D3DBLEND_INVSRCCOLOR;
					break;
				case BF_SRC_ALPHA:
					return D3DBLEND_SRCALPHA;
					break;
				case BF_ONE_MINUS_SRC_ALPHA:
					return D3DBLEND_INVSRCALPHA;
					break;
				case BF_DEST_ALPHA:
					return D3DBLEND_DESTALPHA;
					break;
				case BF_ONE_MINUS_DEST_ALPHA:
					return D3DBLEND_INVDESTALPHA;
					break;
				case BF_DEST_COLOR:
					return D3DBLEND_DESTCOLOR;
					break;
				case BF_ONE_MINUS_DEST_COLOR:
					return D3DBLEND_INVDESTCOLOR;
					break;
				case BF_SRC_ALPHA_SAT:
					return D3DBLEND_SRCALPHASAT;
					break;
				case BF_BOTH_SRC_ALPHA:
					return D3DBLEND_BOTHSRCALPHA;
					break;
				case BF_BOTH_ONE_MINUS_SRC_ALPHA:
					return D3DBLEND_BOTHINVSRCALPHA;
					break;
				case BF_BLEND_FACTOR:
					return D3DBLEND_BLENDFACTOR;
					break;
				case BF_ONE_MINUS_BLEND_FACTOR:
					return D3DBLEND_INVBLENDFACTOR;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DBLENDOP CD3D9Renderer::TranslateBlendOP(CRenderer::BLENDOP e)
		{
			switch (e)
			{
				default:
				case BO_ADD:
					return D3DBLENDOP_ADD;
					break;
				case BO_SUBTRACT:
					return D3DBLENDOP_SUBTRACT;
					break;
				case BO_REVSUBTRACT:
					return D3DBLENDOP_REVSUBTRACT;
					break;
				case BO_MIN:
					return D3DBLENDOP_MIN;
					break;
				case BO_MAX:
					return D3DBLENDOP_MAX;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DCMPFUNC CD3D9Renderer::TranslateTestFunction(CRenderer::TESTFUNC e)
		{
			switch (e)
			{
				case CRenderer::TF_NEVER:
					return D3DCMP_NEVER;
					break;				
				case CRenderer::TF_LESS:
					return D3DCMP_LESS;
					break;
				case CRenderer::TF_EQUAL:
					return D3DCMP_EQUAL;
					break;
				case CRenderer::TF_LEQUAL:
					return D3DCMP_LESSEQUAL;
					break;				
				case CRenderer::TF_GREATER:
					return D3DCMP_GREATER;
					break;
				case CRenderer::TF_NOTEQUAL:
					return D3DCMP_NOTEQUAL;
					break;
				case TF_GEQUAL:
					return D3DCMP_GREATEREQUAL;
					break;
				default:
				case TF_ALWAYS:
					return D3DCMP_ALWAYS;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DSTENCILOP CD3D9Renderer::TranslateStencilOP(CRenderer::STENCILOP e)
		{
			switch (e)
			{
				default:
				case CRenderer::SO_KEEP:
					return D3DSTENCILOP_KEEP;
					break;				
				case CRenderer::SO_ZERO:
					return D3DSTENCILOP_ZERO;
					break;
				case CRenderer::SO_REPLACE:
					return D3DSTENCILOP_REPLACE;
					break;
				case CRenderer::SO_INCR:
					return D3DSTENCILOP_INCR;
					break;				
				case CRenderer::SO_DECR:
					return D3DSTENCILOP_DECR;
					break;				
				case CRenderer::SO_INVERT:
					return D3DSTENCILOP_INVERT;
					break;				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DFILLMODE	CD3D9Renderer::TranslateFillMode(CRenderer::FILLMODE e)
		{
			switch (e)
			{
				case CRenderer::FM_WIREFRAME:
					return D3DFILL_WIREFRAME;
					break;
				default:
				case CRenderer::FM_SOLID:
					return D3DFILL_SOLID;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DCULL	CD3D9Renderer::TranslateCullMode(CRenderer::CULLMODE e)
		{
			switch (e)
			{
				case CRenderer::CULL_NONE:
					return D3DCULL_NONE;
					break;				
				case CRenderer::CULL_CW:
					return D3DCULL_CW;
					break;
				default:
				case CRenderer::CULL_CCW:
					return D3DCULL_CCW;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DTEXTUREFILTERTYPE CD3D9Renderer::TranslateFilter(CRenderer::FILTERMODE e)
		{
			return D3DTEXF_NONE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DTEXTUREADDRESS CD3D9Renderer::TranslateAddressMode(CRenderer::ADDRESSMODE e)
		{
			switch (e)
			{
				default:
				case AM_WRAP:
					return D3DTADDRESS_WRAP;
					break;
				case AM_MIRROR:
					return D3DTADDRESS_MIRROR;
					break;
				case AM_CLAMP:
					return D3DTADDRESS_CLAMP;
					break;
				case AM_BORDER:
					return D3DTADDRESS_BORDER;
					break;
				case AM_MIRROR_ONCE:
					return D3DTADDRESS_MIRRORONCE;
					break;
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		D3DDECLUSAGE CD3D9Renderer::TranslateVertexSlotType(CRenderer::VERTEXSLOTTYPE e)
		{
			switch (e)
			{
				default:
					return D3DDECLUSAGE_SAMPLE;
					break;
				case VST_POSITION:
					return D3DDECLUSAGE_POSITION;
					break;
				case VST_TANGENT:
					return D3DDECLUSAGE_TANGENT;
					break;
				case VST_NORMAL:
					return D3DDECLUSAGE_NORMAL;
					break;
				case VST_BINORMAL:
					return D3DDECLUSAGE_BINORMAL;
					break;
				case VST_TEX_COORD:
					return D3DDECLUSAGE_TEXCOORD;
					break;
				case VST_COLOR:
					return D3DDECLUSAGE_COLOR;
					break;
				case VST_BLEND_WEIGHT:
					return D3DDECLUSAGE_BLENDWEIGHT;
					break;
				case VST_BLEND_INDICES:
					return D3DDECLUSAGE_BLENDINDICES;
					break;
				case VST_DEPTH:
					return D3DDECLUSAGE_DEPTH;
					break;
				case VST_POINT_SIZE:
					return D3DDECLUSAGE_PSIZE;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Renderer::CD3D9Renderer() 
		{
			m_RendererType = D3D9_RENDERER;

			m_lpD3D = NULL;
			m_lpD3DDevice = NULL;
			m_lpSwapChain = NULL;
			m_CurrentDriver = 0;
			m_lpSysFont = NULL;			
			m_DriverList.clear();

			m_Pixel2TexelOffset.x = -0.5f;
			m_Pixel2TexelOffset.y = -0.5f;

			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_TRUE;
			m_bCacheShaderGroup = LYNX_TRUE;

			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_FALSE;
			m_bCacheShaderGroup = LYNX_TRUE;

			m_bInvertRenderTarget = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Renderer::CD3D9Renderer(CGraphicsSystem* lpgs)
		: CRenderer(lpgs) 
		{
			m_RendererType = D3D9_RENDERER;

			m_lpD3D = NULL;
			m_lpD3DDevice = NULL;
			m_lpSwapChain = NULL;
			m_CurrentDriver = 0;
			m_lpSysFont = NULL;			
			m_DriverList.clear();

			m_Pixel2TexelOffset.x = -0.5f;
			m_Pixel2TexelOffset.y = -0.5f;

			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_TRUE;
			m_bCacheShaderGroup = LYNX_TRUE;

			m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_FALSE;
			m_bCacheShaderGroup = LYNX_TRUE;

			m_bInvertRenderTarget = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Renderer::~CD3D9Renderer() 
		{
			m_lpD3D = NULL;
			m_lpD3DDevice = NULL;
			m_lpSwapChain = NULL;
			m_CurrentDriver = 0;
			m_lpSysFont = NULL;
			m_DriverList.clear();
		}		
		//#include "AMDVLIWDetect.h"
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::FindAllDrivers()
		{
			D3DADAPTER_IDENTIFIER9	ID;
			LYNXCHAR				Buffer[MAX_DEVICE_IDENTIFIER_STRING];	

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all drivers...\r\n")));

			m_DriverList.clear(); 
			m_DriverList.resize(m_lpD3D->GetAdapterCount());		
			for (int i=0; i<m_DriverList.size(); i++)
			{
				m_lpD3D->GetAdapterIdentifier(i, 0, &ID);	

				lynxFromAnsiStr(Buffer, ID.Driver);
				// Kill the all "space" character at the end of the string
				for (int j=lynxStrLen(Buffer)-1; j>=0; j--)
				{
					if (Buffer[j] == L' ')
						Buffer[j] = 0x00;
					else
						break;
				}
				m_DriverList[i].Name = Buffer;

				lynxFromAnsiStr(Buffer, ID.Description);
				// Kill the all "space" character at the end of the string
				for (int j=lynxStrLen(Buffer)-1; j>=0; j--)
				{
					if (Buffer[j] == L' ')
						Buffer[j] = 0x00;
					else
						break;
				}
				m_DriverList[i].Desc = Buffer;

				lynxFromAnsiStr(Buffer, ID.DeviceName);
				// Kill the all "space" character at the end of the string
				for (int j=lynxStrLen(Buffer)-1; j>=0; j--)
				{
					if (Buffer[j] == L' ')
						Buffer[j] = 0x00;
					else
						break;
				}
				m_DriverList[i].DeviceName = Buffer;
								
				m_DriverList[i].VendorId = ID.VendorId;				
				m_DriverList[i].DeviceId = ID.DeviceId;
	
				//bool b = IsAMDVLIW(m_DriverList[i].VendorId, m_DriverList[i].DeviceId);				

				LYNXCHAR Buffer[512];
				lynxSprintf(Buffer, _T("\tDirect3D driver[%d] Description : %s\r\n"), i, m_DriverList[i].Desc.c_str());
				LYNX_LOG(glpLogger, CString(Buffer));
				lynxSprintf(Buffer, _T("\tDirect3D driver[%d] VendorID : %d\r\n"), i, m_DriverList[i].VendorId);
				LYNX_LOG(glpLogger, CString(Buffer));
				lynxSprintf(Buffer, _T("\tDirect3D driver[%d] DeviceID : %d\r\n"), i, m_DriverList[i].DeviceId);
				LYNX_LOG(glpLogger, CString(Buffer));
			}
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all drivers OK.\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::FindAllDisplayModes(FORMAT f, int driver, int output)
		{
			LYNXCHAR Buffer[512];
			D3DDISPLAYMODE D3DDisplayMode;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all display mode...\r\n")));

			UINT NumVideoModes = 0;
			NumVideoModes += m_lpD3D->GetAdapterModeCount(m_CurrentDriver, TranslateFormat(f));			
			m_VideoModeList.resize(NumVideoModes);
			for (int i=0; i<NumVideoModes; i++)
			{
				m_lpD3D->EnumAdapterModes(m_CurrentDriver, TranslateFormat(f), i, &D3DDisplayMode);

				m_VideoModeList[i].RefreshRate = D3DDisplayMode.RefreshRate;
				m_VideoModeList[i].Width = D3DDisplayMode.Width;
				m_VideoModeList[i].Height = D3DDisplayMode.Height;
				m_VideoModeList[i].Format = TranslateFormat(D3DDisplayMode.Format);				
				m_VideoModeList[i].bpp = 32;

				lynxSprintf(Buffer, _T("\tDisplay mode[%d] : %dx%dx%d@%d\n"), i, m_VideoModeList[i].Width, m_VideoModeList[i].Height, m_VideoModeList[i].bpp, m_VideoModeList[i].RefreshRate);
				LYNX_LOG(glpLogger, CString(Buffer));								
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Find all display mode OK.\r\n")));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		DWORD CD3D9Renderer::FindRefreshRate(DWORD w, DWORD h)
		{
			for (int i=0; i<m_VideoModeList.size(); i++)
			{
				if (w == m_VideoModeList[i].Width && h == m_VideoModeList[i].Height)
				{
					return (m_VideoModeList[i].RefreshRate);
				}
			}
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		CString& CD3D9Renderer::ComputeCachedCompiledShaderPath() 
		{
			CString				Target;
			LYNXCHAR			Buffer[16];

			Target = LynxEngine::GetEnumStr(m_ShaderTarget);
			m_CachedCompiledShaderPath = CString(_T("../cached/graphics/")) + CString(GetAPIString()) + CString(_T("/")) + CString(GetDriverDesc()) + CString(_T("/shader/")) + Target + CString(_T("/"));			
			m_ShaderSrcPath.clear();
			m_ShaderSrcPath.push_back(CString(_T("../../engine/shader/d3d/")));			
			m_ShaderSrcPath.push_back(CString(_T("../engine/shader/d3d/")));			
			m_ShaderSrcPath.push_back(CString(_T("../shader/d3d/")));			

			m_UserShaderSrcPath = _T("../shader/ogles/");

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

			// TODO : Adjust the batch size according to the HW
			//CSprite3DClusterContainer::m_BatchSize = 64;
			//CSpriteGrassContainer::m_BatchSize = 64;
			//CCrossSpriteGrassContainer::m_BatchSize = 64;
			//CFontString::m_BatchSize = 16*4;
			
			//lynxSprintf(Buffer, _T("%d"), CFontString::m_BatchSize);
			//AddDefinedValue(CAnsiString("FONTSTRING_BATCH_SIZE"), CAnsiString(Buffer));

			//lynxSprintf(Buffer, _T("%d"), CSprite3DClusterContainer::m_BatchSize);
			//AddDefinedValue(CAnsiString("SPRITE3DCLUSTER_BATCH_SIZE"), CAnsiString(Buffer));
			
			lynxSprintf(Buffer, _T("%d"), CSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("GRASS_BATCH_SIZE"), CAnsiString(Buffer));

			lynxSprintf(Buffer, _T("%d"), CCrossSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("CROSSGRASS_BATCH_SIZE"), CAnsiString(Buffer));			

			lynxSprintf(Buffer, _T("%d"), MAX_NUM_TEXTURE_CHANNEL);
			AddDefinedValue(CAnsiString("MAX_TEXTURE_CHANNELS"), CAnsiString(Buffer));	
						
			//AddDefinedValue(CString("PARALLAX"), CString("1"));
			
			return m_CachedCompiledShaderPath; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Renderer::Open(LYNXWINDOW wnd, INITGRAPHICPARAM* lpcp)
		{
			m_Window = wnd; 

			m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
			FindAllDrivers();
			m_CurrentDriver = 0;
			FindAllDisplayModes(lpcp->FrameBufferFormat, m_CurrentDriver);

			RECT rcClient, rcWind;
			POINT ptDiff;
			GetClientRect(m_Window, &rcClient);
			GetWindowRect(m_Window, &rcWind);
			ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
			ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
			if (ptDiff.x > 0 || ptDiff.y > 0)
				lynxOSMoveWindow(m_Window, 0, 0, lpcp->Width, lpcp->Height, LYNX_TRUE);			
			else
				lynxOSMoveWindow(m_Window, 0, 0, lpcp->Width, lpcp->Height, LYNX_FALSE);			
			
			D3DPRESENT_PARAMETERS		D3DPP; 
			D3DSURFACE_DESC				DESC;
			D3DDEVTYPE					DeviceType;
			DWORD						BehaviorFlags;

			lynxMemSet(&D3DPP, 0, sizeof(D3DPP));
			D3DPP.BackBufferWidth = lpcp->Width;
			D3DPP.BackBufferHeight = lpcp->Height;
			D3DPP.BackBufferFormat = TranslateFormat(lpcp->FrameBufferFormat);		
			D3DPP.BackBufferCount = 1;
			D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;			
			D3DPP.MultiSampleType = (D3DMULTISAMPLE_TYPE)((lpcp->NumMultiSamples>1)?lpcp->NumMultiSamples:0);
			D3DPP.EnableAutoDepthStencil = TRUE;
			if (lpcp->DepthBufferFormat == CRenderer::FORMAT_NULL)
				D3DPP.AutoDepthStencilFormat = TranslateFormat(CRenderer::FORMAT_D24S8);
			else
				D3DPP.AutoDepthStencilFormat = TranslateFormat(lpcp->DepthBufferFormat);
			if (lpcp->bFullScreen)
			{
				D3DPP.FullScreen_RefreshRateInHz = FindRefreshRate(lpcp->Width, lpcp->Height);
				D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
				D3DPP.Windowed = FALSE;	
				m_WindowOrigin.x = 0;
				m_WindowOrigin.y = 0;
			}
			else
			{
				D3DPP.FullScreen_RefreshRateInHz = 0;  
				D3DPP.PresentationInterval = 0; 
				D3DPP.Windowed = TRUE;	
				if (ptDiff.x > 0 || ptDiff.y > 0)
				{
					m_WindowOrigin.x = GetSystemMetrics(SM_CXFRAME);
					m_WindowOrigin.y = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION); 
				}
				else
				{
					m_WindowOrigin.x = 0;
					m_WindowOrigin.y = 0;
				}
			}
			if (lpcp->bWaitVSync)
			{
				D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			}
			else
			{
				D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			}			

			// Create the Direct3D device. Here we are using the default adapter (most
			// systems only have one, unless they have multiple graphics hardware cards
			// installed) and requesting the HAL (which is saying we want the hardware
			// device rather than a software one). Software vertex processing is 
			// specified since we know it will work on all cards. On cards that support 
			// hardware vertex processing, though, we would see a big performance gain 
			// by specifying hardware vertex processing
			switch (lpcp->RenderDeviceType)
			{
				case RDT_NULL:
					DeviceType = D3DDEVTYPE_NULLREF;
					break;
				case RDT_REFERNCE:
					DeviceType = D3DDEVTYPE_REF;
					break;
				case RDT_SOFTWARE:
					DeviceType = D3DDEVTYPE_SW;
					break;
				case RDT_HARDWARE:
					DeviceType = D3DDEVTYPE_HAL;
					break;
				case RDT_DEFAULT:
					DeviceType = D3DDEVTYPE_HAL;
					break;
			}			
			BehaviorFlags =	D3DCREATE_HARDWARE_VERTEXPROCESSING;
			BehaviorFlags |= D3DCREATE_MULTITHREADED;
			if (lpcp->ShaderDebuggingFlag & CRenderer::SHADER_DEBUG_VS)
			{	
				BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
				BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
				BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			}
			if (lpcp->ShaderDebuggingFlag & CRenderer::SHADER_DEBUG_PS)
			{
				DeviceType = D3DDEVTYPE_REF;
			}
			if (FAILED(m_lpD3D->CreateDevice(m_CurrentDriver, DeviceType, m_Window, BehaviorFlags, &D3DPP, &m_lpD3DDevice)))
				return LYNX_FALSE;

			D3DCAPS9 Caps;
			m_lpD3DDevice->GetDeviceCaps(&Caps);
			m_Capbilities.MaxVertexShaderConstants	= Caps.MaxVertexShaderConst;
			m_Capbilities.MaxTextureWidth			= Caps.MaxTextureWidth;
			m_Capbilities.MaxTextureHeight			= Caps.MaxTextureHeight;
			m_Capbilities.MaxTextureChannels		= Caps.MaxSimultaneousTextures;
			m_Capbilities.MaxRenderBuffers			= Caps.NumSimultaneousRTs;

			m_lpD3DDevice->GetSwapChain(0, &m_lpSwapChain);

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
			m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
			IDirect3DSurface9* lpSurface;
			m_lpD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &lpSurface);
			((CD3D9Buffer*)((CBuffer*)m_lpBackBuffer))->SetlpSurface(lpSurface);
			((CD3D9Buffer*)((CBuffer*)m_lpBackBuffer))->SetlplpSwapChain(m_lpSwapChain);
			SetRenderBuffer(0, m_lpBackBuffer);

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
			m_lpDepthBuffer->SetName(_T("SystemDepthBuffer"));
			m_lpD3DDevice->GetDepthStencilSurface(&lpSurface);
			((CD3D9Buffer*)((CBuffer*)m_lpDepthBuffer))->SetlpSurface(lpSurface);
			SetDepthBuffer(m_lpDepthBuffer);

			m_lpOffscreenDepthBuffer = m_lpDepthBuffer;
			
			SetViewportRect(0, 0, lpcp->Width, lpcp->Height);
			EnableScissorRect(LYNX_FALSE);			
			m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 		
			m_lpD3DDevice->SetRenderState(D3DRS_LASTPIXEL, TRUE);		
			UseAlphaBlend(LYNX_TRUE);
			UseAlphaTest(LYNX_TRUE);
			SetAlphaReference(0);
			for (int i=0; i<8; i++)
			{
				m_lpD3DDevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE); 
				m_lpD3DDevice->SetTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_TEXTURE); 
				m_lpD3DDevice->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
			}						

			m_SystemFixedVertexLayout = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX8);

			#if	defined(_UNICODE) || defined(UNICODE)
				if (D3DXCreateFont(m_lpD3DDevice,
								   20, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, 
								   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
								   _T("å¾®è?æ­??é«?"), &m_lpSysFont) != S_OK)
				{
					D3DXCreateFont(m_lpD3DDevice,
								   20, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, 
								   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
								   _T("Arial Unicode MS"), &m_lpSysFont);
				}
			#else
				D3DXCreateFont(m_lpD3DDevice,
							   16, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, 
							   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
							   _T("Arial"), &m_lpSysFont);
			#endif
			
			//m_Extensions = (LYNXANSICHAR*)glGetString(GL_EXTENSIONS);
			//m_Version = (LYNXANSICHAR*)glGetString(GL_VERSION);
			m_Vendor = "Microsoft";
			m_Renderer = "D3D9";

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::Close()
		{
			CRenderer::Close();
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Restore FPCW...\r\n")));			
			lynxRestoreFPCW();							
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::Resize(int cx, int cy)
		{
			if (IsValid() && cx != 0 && cy != 0 && (m_lpBackBuffer->vGetWidth() != cx || m_lpBackBuffer->vGetHeight() != cy))
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpBackBuffer, LynxEngine::BUFFER);
				m_lpBackBuffer->vCreate(m_Window, cx, cy, m_lpGraphicsSystem->GetConfig().GraphicParam.FrameBufferFormat, (m_lpGraphicsSystem->GetConfig().GraphicParam.NumMultiSamples > 1) ? m_lpGraphicsSystem->GetConfig().GraphicParam.NumMultiSamples : 0, LYNX_FALSE);
				SetRenderBuffer(0, m_lpBackBuffer);
				m_lpBackBuffer->SetName(_T("SystemBackBuffer"));
				m_lpSwapChain = ((CD3D9Buffer*)(((CBuffer*)m_lpBackBuffer)))->GetlpSwapChain();
							
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, LynxEngine::BUFFER);
				m_lpDepthBuffer->vCreate(m_Window, cx, cy, m_lpGraphicsSystem->GetConfig().GraphicParam.DepthBufferFormat, (m_lpGraphicsSystem->GetConfig().GraphicParam.NumMultiSamples > 1) ? m_lpGraphicsSystem->GetConfig().GraphicParam.NumMultiSamples : 0, LYNX_FALSE);
				SetDepthBuffer(m_lpDepthBuffer);
				m_lpDepthBuffer->SetName(_T("SystemDepthBuffer") );

				SetViewportRect(0, 0, cx, cy);
				EnableScissorRect(LYNX_FALSE);
				m_lpGraphicsSystem->GetlpEngine()->ClearInvalidResource();

				#ifndef __TOOL__
					lynxOSMoveWindow(m_Window, 0, 0, cx, cy, LYNX_TRUE);
				#endif
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::SetViewportRect(int x1, int y1, int x2, int y2) 
		{
			CRenderer::SetViewportRect(x1, y1, x2, y2);

			D3DVIEWPORT9	D3DViewport;

			D3DViewport.X = (DWORD)x1;
			D3DViewport.Y = (DWORD)y1;
			D3DViewport.Width = (DWORD)(x2 - x1);
			D3DViewport.Height = (DWORD)(y2 - y1);
			D3DViewport.MinZ = 0;
			D3DViewport.MaxZ = 1;
		
			m_lpD3DDevice->SetViewport(&D3DViewport);	
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::SetScissorRect(int x1, int y1, int x2, int y2)
		{			
			RECT Rect;

			Rect.left	= x1;
			Rect.right	= x2;
			Rect.top	= y1;
			Rect.bottom = y2;	
			m_lpD3DDevice->SetScissorRect(&Rect);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::GetScissorRect(LYNXRECT* lprect)
		{			
			RECT Rect;

			m_lpD3DDevice->GetScissorRect(&Rect);

			lprect->x1 = Rect.left;
			lprect->x2 = Rect.right;
			lprect->y1 = Rect.top;
			lprect->y2 = Rect.bottom;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CD3D9Renderer::SetColorWriteMask(LYNXDWORD m)
		{
			DWORD Mask = 0;
			DWORD OldMask = m_CachedColorWriteMask;
			if (m & RED_CHANNEL) 
				Mask |= D3DCOLORWRITEENABLE_RED;
			if (m & GREEN_CHANNEL) 
				Mask |= D3DCOLORWRITEENABLE_GREEN;
			if (m & BLUE_CHANNEL) 
				Mask |= D3DCOLORWRITEENABLE_BLUE;
			if (m & ALPHA_CHANNEL) 
				Mask |= D3DCOLORWRITEENABLE_ALPHA;

			m_lpD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, Mask);
			m_CachedColorWriteMask = m;

			return OldMask;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawStrip(FIXEDVERTEX* lpv, int x, int y) 
		{
			D3DTLVERTEX V[4];

			V[0].sx = lpv[0].x + x; V[0].sy = lpv[0].y + y; V[0].sz = 0.01f; V[0].rhw = 100.0f;
			V[1].sx = lpv[1].x + x; V[1].sy = lpv[1].y + y; V[1].sz = 0.01f; V[1].rhw = 100.0f;
			V[2].sx = lpv[2].x + x; V[2].sy = lpv[2].y + y; V[2].sz = 0.01f; V[2].rhw = 100.0f;
			V[3].sx = lpv[3].x + x; V[3].sy = lpv[3].y + y; V[3].sz = 0.01f; V[3].rhw = 100.0f;
			
			V[0].color = lpv[0].color;
			V[1].color = lpv[1].color;
			V[2].color = lpv[2].color;
			V[3].color = lpv[3].color;	

			UseSpecular(LYNX_FALSE);
			SetVertexShader(NULL);			
			SetPixelShader(NULL);			
			SetFixedVertexLayout((D3DFVF_XYZRHW | D3DFVF_DIFFUSE));						
			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(D3DTLVERTEX));							
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawTriangle(FIXEDVERTEX& v1, FIXEDVERTEX& v2, FIXEDVERTEX& v3) 
		{
			static FIXEDVERTEX	V[3];

			START_COUNTER 

			lynxMemCpy(&V[0], &v1, sizeof(FIXEDVERTEX));
			lynxMemCpy(&V[1], &v2, sizeof(FIXEDVERTEX));
			lynxMemCpy(&V[2], &v3, sizeof(FIXEDVERTEX));
			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, V, sizeof(FIXEDVERTEX));

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawTriangleStrip(FIXEDVERTEX* lpv) 
		{
			START_COUNTER 

			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, lpv, sizeof(FIXEDVERTEX));

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawTriangleFans(FIXEDVERTEX* lpv, int num) 
		{
			START_COUNTER 

			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num-2, (LPVOID)lpv, sizeof(FIXEDVERTEX));  

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawElements(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) 
		{
			START_COUNTER 

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawPrimitive(PRIMITIVE* lpp, DWORD num_vert) 
		{
			START_COUNTER 

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawScreenQuad(void* lpv, DWORD size) 
		{
			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, lpv, size);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawElementsBO(PRIMITIVETYPE type, int base_vert, DWORD num_vert, int base_index, DWORD primitive_count) 
		{
			START_COUNTER 

			m_lpD3DDevice->DrawIndexedPrimitive(TranslatePrimitiveType(type), base_vert, 0, num_vert, base_index, primitive_count);
			
			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, primitive_count) 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawPrimitiveBO(PRIMITIVE* lpp, DWORD num_vert) 
		{
			START_COUNTER 
			
			m_lpD3DDevice->DrawIndexedPrimitive(TranslatePrimitiveType(lpp->Type), lpp->BaseVertex, 0, num_vert, lpp->BaseIndex, lpp->NumPrimitives);	

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, lpp->NumPrimitives) 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawArrayBO(PRIMITIVETYPE type, DWORD primitive_count, int base_vert) 
		{
			START_COUNTER 
				
			m_lpD3DDevice->DrawPrimitive(TranslatePrimitiveType(type), base_vert, primitive_count);

			STOP_COUNTER(m_DrawTimeCounter) 
			INC_COUNTER(m_DrawCallCounter, 1) 
			INC_COUNTER(m_PrimitiveCounter, primitive_count) 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawLine(const Math::CVector2& p1, const Math::CVector2& p2, LYNXCOLORRGBA& c) 
		{
			static D3DTLVERTEX P[2];				

			P[0].sx = p1.x; P[0].sy = p1.y; P[0].sz = 0.01f; P[0].rhw = 100.0f;
			P[1].sx = p2.x; P[1].sy = p2.y; P[1].sz = 0.01f; P[1].rhw = 100.0f;			
			P[0].color = D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha);
			P[1].color = D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha);
			
			CBlendState* OldBlend = m_CachedBlendState;
			CTexture* OldTexture = m_CachedTexture[0];
			CVertexShader* OldVS = m_CachedVS;
			CPixelShader* OldPS = m_CachedPS;

			UseSpecular(LYNX_FALSE);
			SetVertexShader(NULL);
			SetPixelShader(NULL);
			SetTexture(0, NULL);
			m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

			//m_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, LYNX_TRUE);
			//m_lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, CD3D9Renderer::BF_ONE);
			//m_lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, CD3D9Renderer::BF_ZERO);
			//m_lpD3DDevice->SetRenderState(D3DRS_BLENDOP, CD3D9Renderer::BO_ADD);			

			SetFixedVertexLayout((D3DFVF_XYZRHW | D3DFVF_DIFFUSE));						
			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, P, sizeof(D3DTLVERTEX));		

			SetBlendState(OldBlend);
			SetTexture(0, OldTexture);
			SetVertexShader(OldVS);
			SetPixelShader(OldPS);

			/*
			LPD3DXLINE line;
			D3DXCreateLine(m_lpD3DDevice, &line);
			D3DXVECTOR2 lines[] = { D3DXVECTOR2(p1.x, p1.y), D3DXVECTOR2(p2.x, p2.y) };
			line->Begin();
			line->Draw(lines, 2, D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha));
			line->End();
			line->Release();			
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  èªªæ?:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Renderer::DrawLine(const Math::CVector3& p1, const Math::CVector3& p2, LYNXCOLORRGBA& c) 
		{
			static D3DVERTEX P[2];				

			P[0].x = p1.x; P[0].y = p1.y; P[0].z = p1.z;
			P[1].x = p2.x; P[1].y = p2.y; P[1].z = p2.z;			
			P[0].color = D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha);
			P[1].color = D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha);
			
			UseSpecular(LYNX_FALSE);
			SetVertexShader(NULL);
			SetPixelShader(NULL);
			SetTexture(0, NULL);
			m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			SetFixedVertexLayout((D3DFVF_XYZ | D3DFVF_DIFFUSE));
			m_lpD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, P, sizeof(D3DVERTEX));										

			/*
			LPD3DXLINE line;
			D3DXCreateLine(m_lpD3DDevice, &line);
			D3DXVECTOR2 lines[] = { D3DXVECTOR3(p1.x, p1.y, p1.z), D3DXVECTOR3(p2.x, p2.y, p2.z) };
			line->Begin();
			line->Draw(lines, 2, D3DCOLOR_RGBA(c.Red, c.Green, c.Blue, c.Alpha));
			line->End();
			line->Release();
			*/
		}
	}
}

#endif