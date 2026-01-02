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
		CGBufferMap::CGBufferMap(void)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGBufferMap::CGBufferMap(CGraphicsSystem *pgs)
		: CMap(pgs)
		{						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGBufferMap::~CGBufferMap(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGBufferMap::Create(const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift)
		{			
			int			W, H;
			LYNXCHAR	Name[64];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create GBuffer...\r\n")));

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreateRT(NULL, CRenderer::FORMAT_NULL, name, w, h, 1, CRenderer::TT_2D_TEXTURE, format, 0))
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
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_POINT;
			SamDesc.MinLOD	 = 0;
			SamDesc.MaxLOD	 = 0;
			m_Texture->CreateSamplerState(SamDesc);
			for (int i=0; i<m_UtilTextures.size(); i++)
			{
				m_UtilTextures[i]->CreateSamplerState(SamDesc);
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create GBuffer OK.\r\n")));

			return LYNX_TRUE;
		}
	}
}