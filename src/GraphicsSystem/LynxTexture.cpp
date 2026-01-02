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
#include <GraphicsSystem/LynxGraphicsSystem.h>

#if defined(_UNICODE) || defined(UNICODE)		
	#define  FREEIMAGE_LOAD						FreeImage_LoadU
	#define  FREEIMAGE_SAVE						FreeImage_SaveU
	#define  FREEIMAGE_GETFILETYPE				FreeImage_GetFileTypeU
#else
	#define  FREEIMAGE_LOAD						FreeImage_Load
	#define  FREEIMAGE_SAVE						FreeImage_Save
	#define  FREEIMAGE_GETFILETYPE				FreeImage_GetFileType
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
	#ifdef __WIN32__
		#ifdef __FREEIMAGE__
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			unsigned DLL_CALLCONV FreeImage_ReadFileProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
			{
				CStream* lpStream = (CStream*)handle;
				return lpStream->vRead(buffer, size, count);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			unsigned DLL_CALLCONV FreeImage_WriteFileProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
			{
				CStream* lpStream = (CStream*)handle;
				return lpStream->vWrite(buffer, size, count);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int DLL_CALLCONV FreeImage_SeekFileProc(fi_handle handle, long offset, int origin)
			{
				CStream* lpStream = (CStream*)handle;
				return lpStream->vSeek(offset, origin);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			long DLL_CALLCONV FreeImage_TellFileProc(fi_handle handle)
			{
				CStream* lpStream = (CStream*)handle;
				return lpStream->vTell();
			}
		#endif
	#endif
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		png_voidp PNGMalloc(png_structp png_ptr, png_size_t size)
		{
			return LYNXALLOC(size);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		//! PNG free
		void PNGFree(png_structp png_ptr, png_voidp ptr)
		{
			LYNXFREE(ptr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		//! PNG error
		void PNGError(png_structp png_ptr, png_const_charp msg)
		{
			//LYNX_ASSERT(false, msg);
			lynxDebugOutputPrintf(CString(msg).c_str());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		//! PNG warning
		void PNGWarning(png_structp png_ptr, png_const_charp msg)
		{
			//SHOOT_WARNING(false, msg);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		//! PNG read
		void PNGRead(png_structp png_ptr, png_bytep data, png_size_t length)
		{
			CStream* lpStream = (CStream*)png_get_io_ptr(png_ptr);
			lpStream->vRead(data, 1, length);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture::CTexture(CGraphicsSystem *pgs)
		: CResource(pgs->GetlpEngine())
		{	
			m_ResourceType = TEXTURE;
			m_lpGraphicsSystem = pgs;		
			m_MipMapLevel = 0;
			m_LODBias = 0;
			m_bSRGB = LYNX_FALSE;
			m_AlphaReference = 0.0f;

			CRenderer::SAMPLERSTATEDESC SamDesc;
			SamDesc.Default();			
			CreateSamplerState(SamDesc);

			IMPLEMENT_PARAM_INT(m_MipMapLevel, 0)
			IMPLEMENT_PARAM_INT(m_LODBias, 0)
			IMPLEMENT_PARAM_BOOL(m_bSRGB, 0)
			IMPLEMENT_PARAM_FLOAT(m_AlphaReference, 0)			

			m_bDelayLoading = LYNX_FALSE;
			m_bLoaded = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture::~CTexture(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTexture::CreateSamplerState(const CRenderer::SAMPLERSTATEDESC& desc)
		{
			m_lpGraphicsSystem->GetlpRenderer()->CreateSamplerState(m_SamplerState, desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::FORMAT CTexture::GetFormat() 
		{
			return m_lpSrcBuffer->GetFormat(); 
		}
	}
}