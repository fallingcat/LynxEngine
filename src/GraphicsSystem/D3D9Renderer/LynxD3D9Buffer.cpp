//##########################################################################
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
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Buffer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Buffer::CD3D9Buffer(CGraphicsSystem* lpsys)
		: CBuffer(lpsys) 		
		{				
			m_lpSurface = NULL;
			m_lpSwapChain = NULL;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Buffer::~CD3D9Buffer()
		{				
			if (m_lpSurface)
			{
				m_lpSurface->Release();
				m_lpSurface = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   If wnd == NULL means to create renderable buffer
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::SetlpSurface(IDirect3DSurface9* s) 
		{
			m_lpSurface = s; 
			D3DSURFACE_DESC Desc; 
			s->GetDesc(&Desc);

			m_Width = Desc.Width; 
			m_Height = Desc.Height;
			m_NumSamples = Desc.MultiSampleType;
			m_Format = CD3D9Renderer::TranslateFormat(Desc.Format);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   If wnd == NULL means to create renderable buffer
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Buffer::vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable) 
		{
			m_Width = w; 
			m_Height = h;
			m_Format = format; 
			m_NumSamples = num_ms;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
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
					if (!lpR->CheckDepthFormat(format))
					{
						return LYNX_FALSE;
					}		
					if (FAILED(lpR->GetlpD3DDevice()->CreateDepthStencilSurface(w, h, CD3D9Renderer::TranslateFormat(format), (D3DMULTISAMPLE_TYPE)num_ms, 0.5f, TRUE, &m_lpSurface, NULL)))					
						return LYNX_FALSE;

					if (num_ms > 1)
						m_VideoMemoryUsage = w * h * CRenderer::GetFormatBytes(format) * num_ms;
					else
						m_VideoMemoryUsage = w * h * CRenderer::GetFormatBytes(format);
					break;

				default:
					if (wnd)
					{
						D3DPRESENT_PARAMETERS D3DPP;
						lynxMemSet(&D3DPP, 0, sizeof(D3DPP));
						D3DPP.hDeviceWindow = wnd;
						//D3DPP.Flags = D3DPRESENTFLAG_DEVICECLIP;
						D3DPP.BackBufferWidth = w;
						D3DPP.BackBufferHeight = h;
						D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
						D3DPP.BackBufferFormat = CD3D9Renderer::TranslateFormat(format);		
						D3DPP.BackBufferCount = 1;
						D3DPP.EnableAutoDepthStencil = FALSE;
						D3DPP.MultiSampleType = (D3DMULTISAMPLE_TYPE)num_ms;
						D3DPP.Windowed = TRUE;	
						D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

						if (FAILED(lpR->GetlpD3DDevice()->CreateAdditionalSwapChain(&D3DPP, &m_lpSwapChain)))
							return LYNX_FALSE;
						m_lpSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_lpSurface);
					}
					else
					{
						if (!lpR->CheckRTFormat(format))
						{
							return LYNX_FALSE;
						}
						if (FAILED(lpR->GetlpD3DDevice()->CreateRenderTarget(w, h, CD3D9Renderer::TranslateFormat(format), (D3DMULTISAMPLE_TYPE)num_ms, 0.5f, blockable, &m_lpSurface, NULL)))
							return LYNX_FALSE;

						m_VideoMemoryUsage = 0;
					}
					break;
			}					
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vSetAsRenderTarget(int index) 
		{ 
			((CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->SetRenderTarget(index, m_lpSurface);  			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vSetAsDepthBuffer() 
		{ 
			((CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpD3DDevice()->SetDepthStencilSurface(m_lpSurface);  			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vSaveToFile(const LYNXCHAR* name, const LYNXRECT* rect) 
		{ 
			RECT Rect;
			LYNXCHAR Ext[32], Name[256];
			D3DXIMAGE_FILEFORMAT Format;
			
			lynxSeparateFileExtByPath(Name, Ext, name);

			if (!lynxStriCmp(Ext, _T("png")))
				Format = D3DXIFF_PNG;
			else if (!lynxStriCmp(Ext, _T("bmp")))
				Format = D3DXIFF_BMP;

			if (rect)
			{
				Rect.left	= rect->x1;
				Rect.top	= rect->y1;
				Rect.right	= rect->x2;
				Rect.bottom = rect->y2;
				D3DXSaveSurfaceToFile(name, Format, m_lpSurface, NULL, &Rect);
			}
			else
			{
				D3DXSaveSurfaceToFile(name, Format, m_lpSurface, NULL, NULL);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, LYNXCHAR const *fmt, ...) 
		{
			RECT		Rect;		
			LYNXCHAR	Buffer[512];
			LYNXCHAR*   Ch;	
			va_list		argptr;

			va_start(argptr,fmt);		
			lynxVsnPrintf(Buffer, 510, fmt, argptr);
			va_end(argptr);

			if (lprect)
			{
				m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(lprect->x1, lprect->y1, lprect->x2, lprect->y2);		
				Rect.left = lprect->x1+x;
				Rect.top = lprect->y1+y;
				Rect.right = lprect->x2;
				Rect.bottom = lprect->y2;
			}
			else
			{
				m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, vGetWidth(), vGetHeight());	
				Rect.left = x;
				Rect.top = y;
				Rect.right = vGetWidth();
				Rect.bottom = vGetHeight();
			}
			((CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer()))->GetlpSystemFont()->DrawText(NULL, Buffer, lynxStrLen(Buffer), &Rect, DT_LEFT, D3DCOLOR_RGBA(c->Red, c->Green, c->Blue, c->Alpha));		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vPrintf(LPLYNXRECT lprect, int x, int y, const LYNXCOLORRGBA* c, const LYNXCOLORRGBA* sc, LYNXCHAR const *fmt, ...) 
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
			vPrintf(&Rect, x+1, y+1, sc, Buffer);
			vPrintf(&Rect, x, y, c, Buffer);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D9Buffer::vMap(int flag, LPLYNXRECT lprect) 
		{
			RECT				Rect;
			D3DLOCKED_RECT		LockedRect;
			DWORD				Flag = 0;

			switch (flag)
			{
				case CRenderer::LOCK_TO_READ:
					Flag = D3DLOCK_READONLY;
					break;

				case CRenderer::LOCK_TO_WRITE_DISCARD_OLD:
					Flag = D3DLOCK_DISCARD;
					break;

				case CRenderer::LOCK_TO_WRITE_NO_OVERWRITE:
					Flag = D3DLOCK_NOOVERWRITE;
					break;

				case CRenderer::LOCK_TO_WRITE :
					Flag = 0;
					break;

				case CRenderer::LOCK_TO_BOTH :
					Flag = 0;
					break;
			}		

			LYNX_ASSERT(m_lpSurface != NULL);
			HRESULT LockHr = D3D_OK;
			if (lprect)
			{
				Rect.left	= lprect->x1;
				Rect.right	= lprect->x2;
				Rect.top	= lprect->y1;
				Rect.bottom = lprect->y2;
				m_lpSurface->LockRect(&LockedRect, &Rect, Flag);	
			}
			else
			{
				LockHr = m_lpSurface->LockRect(&LockedRect, NULL, Flag);	


			}
			
			m_lpBuffer = LockedRect.pBits;
			m_lPitch = LockedRect.Pitch;

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_BufferMapCounter, 1)

			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Buffer::vUnmap() 
		{
			LYNX_ASSERT(m_lpSurface != NULL);
			m_lpSurface->UnlockRect();	
			m_lpBuffer = NULL;
		}
	}
}

#endif