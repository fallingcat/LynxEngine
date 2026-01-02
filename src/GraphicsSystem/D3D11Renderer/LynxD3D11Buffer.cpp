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
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Buffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Buffer::CD3D11Buffer(CGraphicsSystem *pgs)
		: CBuffer(pgs)
		{	
			m_lpResource	= NULL;
			m_lpSRView		= NULL;	
			m_lpRTView		= NULL;
			m_lpDSView		= NULL;

			m_Width = m_Height = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Buffer::~CD3D11Buffer(void)
		{
			if (m_lpResource)
				m_lpResource->Release();
			if (m_lpSRView)
				m_lpSRView->Release();
			if (m_lpRTView)
				m_lpRTView->Release();
			if (m_lpDSView)
				m_lpDSView->Release();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Buffer::vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable)
		{
			LYNXCHAR Str[512], Buffer[512];
			D3D11_TEXTURE2D_DESC TexDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC DescDSV;
			D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
			HRESULT hResult;

			m_Format = format; 
			switch (format)
			{
				case CRenderer::FORMAT_D16_LOCKABLE:       
				case CRenderer::FORMAT_D32:
				case CRenderer::FORMAT_D15S1:
				case CRenderer::FORMAT_D24S8:
				case CRenderer::FORMAT_D24X8:
				case CRenderer::FORMAT_D24X4S4:
				case CRenderer::FORMAT_D16:      
				case CRenderer::FORMAT_D32F_LOCKABLE:
				case CRenderer::FORMAT_D24FS8:
					// Create depth stencil texture					
					TexDesc.Width = w;
					TexDesc.Height = h;
					TexDesc.MipLevels = 1;
					TexDesc.ArraySize = 1;
					TexDesc.Format = CD3D11Renderer::GetCorrespondingTextureFormat(CD3D11Renderer::TranslateFormat(format));
					TexDesc.SampleDesc.Count = num_ms;
					TexDesc.SampleDesc.Quality = 0;
					TexDesc.Usage = D3D11_USAGE_DEFAULT;
					TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
					TexDesc.CPUAccessFlags = 0;
					TexDesc.MiscFlags = 0;
					hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateTexture2D(&TexDesc, NULL, (ID3D11Texture2D**)&m_lpResource);
					if (hResult != S_OK)   
					{
						lynxStrCpy(Str, DXGetErrorString(hResult));
						lynxSprintf(Buffer, _T("Error : Fail to create Buffer(D3D Error :  %s\r\n"), Str);
						LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
						return LYNX_FALSE;
					}
					// Create the depth stencil view					
					DescDSV.Format = CD3D11Renderer::TranslateFormat(format);
					if (TexDesc.SampleDesc.Count > 1)
						DescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
					else
						DescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					DescDSV.Texture2D.MipSlice = 0;
					DescDSV.Flags  = 0;
					hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateDepthStencilView(m_lpResource, &DescDSV, &m_lpDSView);
					if (hResult != S_OK)   
					{
						lynxStrCpy(Str, DXGetErrorString(hResult));
						lynxSprintf(Buffer, _T("Error : Fail to create Buffer view(D3D Error :  %s\r\n"), Str);
						LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));																
						return LYNX_FALSE;
					}		
					DescSRV.Format = CD3D11Renderer::GetCorrespondingShaderResourceViewFormat(CD3D11Renderer::TranslateFormat(format));
					DescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					DescSRV.Texture2D.MostDetailedMip = 0;
					DescSRV.Texture2D.MipLevels = 1;
					hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateShaderResourceView(m_lpResource, &DescSRV, &m_lpSRView);
					if (hResult != S_OK)   
					{
						lynxStrCpy(Str, DXGetErrorString(hResult));
						lynxSprintf(Buffer, _T("Error : Fail to create Buffer resource view(D3D Error :  %s\r\n"), Str);
						LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));																				
						return LYNX_FALSE;
					}
					m_Width = w;
					m_Height = h;
					m_VideoMemoryUsage = m_Width * m_Height * CRenderer::GetFormatBytes(format) * num_ms;
					return LYNX_TRUE; 
					break;

				default:
					if (wnd)		
						return LYNX_FALSE; 
					else
					{
						return LYNX_FALSE;
					}
					break;
			}	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Buffer::vCreate(int type, int size, int len, void* data, int flag)
		{
			LYNXCHAR Str[512], Buffer[512];
			D3D11_BUFFER_DESC Desc;
			D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
			HRESULT hResult;

			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT; 
			if (flag & CRenderer::STATIC)
				Usage = D3D11_USAGE_IMMUTABLE;
			else if (flag & CRenderer::DYNAMIC)
				Usage = D3D11_USAGE_DYNAMIC;
			else if (flag & CRenderer::STAGING)
				Usage = D3D11_USAGE_STAGING;
			else
				Usage = D3D11_USAGE_DEFAULT;
			Desc.Usage = Usage;

			int Flag = 0;
			if (flag & CRenderer::WRITEONLY)
				Flag = 0;
			if (flag & CRenderer::CPU_WRITE)
				Flag |= D3D11_CPU_ACCESS_WRITE;
			if (flag & CRenderer::CPU_READ)
				Flag |= D3D11_CPU_ACCESS_READ;
			Desc.CPUAccessFlags = Flag;

			int BindFlag = D3D11_BIND_SHADER_RESOURCE;
			if (type & CRenderer::CONSTANT_BUFFER)
				BindFlag = D3D11_BIND_CONSTANT_BUFFER;
			else 
			{
				if (type & CRenderer::RENDER_BUFFER)
					BindFlag |= D3D11_BIND_RENDER_TARGET;
				if (type & CRenderer::RENDER_BUFFER)
					BindFlag |= D3D11_BIND_RENDER_TARGET;
				if (type & CRenderer::DEPTH_BUFFER || type & CRenderer::STENCIL_BUFFER)
					BindFlag |= D3D11_BIND_DEPTH_STENCIL;
				if (type & CRenderer::STREAM_BUFFER)
					BindFlag |= D3D11_BIND_STREAM_OUTPUT;
				if (type & CRenderer::UNORDERED_BUFFER)
					BindFlag |= D3D11_BIND_UNORDERED_ACCESS;
			}
			Desc.BindFlags = BindFlag;
			
			Desc.ByteWidth = len * size;
			Desc.StructureByteStride = size;
			Desc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA* lpInitData = NULL;
			D3D11_SUBRESOURCE_DATA InitData;
			if (data)
			{
				lpInitData = &InitData;					
				InitData.pSysMem = data;
			}
			hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->CreateBuffer(&Desc, lpInitData, (ID3D11Buffer**)&m_lpResource);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("Error : Fail to create Buffer(D3D Error :  %s\r\n"), Str);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
				return LYNX_FALSE;
			}

			if (BindFlag & D3D11_BIND_SHADER_RESOURCE)
			{
				ID3D11ShaderResourceView* pSRView = NULL;
				D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
				ZeroMemory(&SRVDesc, sizeof(SRVDesc));
				SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				SRVDesc.Buffer.NumElements = len;
				SRVDesc.Buffer.ElementWidth = size;
				SRVDesc.Buffer.ElementOffset = 0;
				SRVDesc.Buffer.FirstElement = 0;
				((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateShaderResourceView(m_lpResource, &SRVDesc, &m_lpSRView);
			}

			m_VideoMemoryUsage = len * size;

			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Buffer::vSetAsRenderTarget(int index) 
		{ 
			((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->OMSetRenderTargets(1, &m_lpRTView, ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetCacahedDSView());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D11Buffer::vMap(int flag, LPLYNXRECT lprect) 
		{
			LYNXCHAR Str[512], Buffer[512];
			HRESULT hResult;

			if (m_lpBuffer)
				return m_lpBuffer;


			D3D11_MAPPED_SUBRESOURCE MappedSubResource;
			hResult = ((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Map(m_lpResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);
			if (hResult != S_OK)   
			{
				lynxStrCpy(Str, DXGetErrorString(hResult));
				lynxSprintf(Buffer, _T("Error : Fail to map Buffer(D3D Error :  %s\r\n"), Str);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(Buffer));												
				return NULL;
			}

			m_lpBuffer = MappedSubResource.pData;

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_BufferMapCounter, 1)

			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Buffer::vUnmap() 
		{
			LYNX_ASSERT(m_lpResource != NULL);

			if (m_lpBuffer)
			{
				((CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpDeviceContext()->Unmap(m_lpResource, 0);	
				m_lpBuffer = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Buffer::vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...) 
		{
			LYNXCHAR				Buffer[512];
			LYNXCHAR				*Ch;	
			va_list					argptr;

			va_start(argptr,fmt);		
			lynxVsnPrintf(Buffer, 510, fmt, argptr);
			va_end(argptr);

			m_lpEngine->GetSystemFontString()->SetAlignmentType(CSpriteContainer::H_ALIGN_LEFT|CSpriteContainer::V_ALIGN_TOP);
			m_lpEngine->GetSystemFontString()->SetOriginType(CSpriteContainer::OT_UPPER_LEFT);

			m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());
			m_lpEngine->GetSystemFontString()->Printf(x, y, c, Buffer);				
			m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemFont());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Buffer::vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...) 
		{
			LYNXRECT	Rect;		
			LYNXCHAR	Buffer[512];
			LYNXCHAR*   Ch;	
			va_list		argptr;

			va_start(argptr,fmt);		
			lynxVsnPrintf(Buffer, 510, fmt, argptr);
			va_end(argptr);

			if (lprect)
			{
				Rect.x1 = lprect->x1;
				Rect.y1 = lprect->y1;
				Rect.x2 = lprect->x2;
				Rect.y2 = lprect->y2;
			}
			else
			{
				Rect.x1 = 0;
				Rect.y1 = 0;
				Rect.x2 = vGetWidth();
				Rect.y2 = vGetHeight();
			}
			//vPrintf(&Rect, x+1, y+1, sc, Buffer);
			vPrintf(&Rect, x, y, c, Buffer);						
		}
	}
}

#endif