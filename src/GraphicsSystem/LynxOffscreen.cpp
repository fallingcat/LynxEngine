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

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COffscreen::COffscreen(void)
		{				
			//m_ResultTexIndex = LYNX_NULL_TEXTURE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COffscreen::COffscreen(CGraphicsSystem *pgs)
		: CMap(pgs)
		{	
			//m_ResultTexIndex = LYNX_NULL_TEXTURE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COffscreen::~COffscreen(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COffscreen::Create(int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift, LYNXBOOL bSRGB)
		{
			int			W, H;
			LYNXCHAR	Name[64];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create OffscreenMap...\r\n")));

			m_bSRGB = bSRGB;

			W = w;
			H = h;			
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreateRT(NULL, CRenderer::FORMAT_NULL, name, W, H, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, m_lpGraphicsSystem->GetSystemBackBuffer()->vGetNumMultiSamples()))
				return LYNX_FALSE;
			m_Current = m_Texture;

			if (utilsize)
			{				
				if (!CreateUtilityTexture(name, w, h, format, utilsize, shift))
					return LYNX_FALSE;
			}	

			CRenderer::SAMPLERSTATEDESC SamDesc;
			SamDesc.Default();
			SamDesc.AddressU = CRenderer::AM_CLAMP;
			SamDesc.AddressV = CRenderer::AM_CLAMP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			SamDesc.MinLOD	 = 0;
			SamDesc.MaxLOD	 = 0;
			m_Texture->CreateSamplerState(SamDesc);
			for (int i=0; i<m_UtilTextures.size(); i++)
			{
				m_UtilTextures[i]->CreateSamplerState(SamDesc);
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create OffscreenMap OK.\r\n")));

			return LYNX_TRUE;
		}		
	}
}