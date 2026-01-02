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
		CCubeShadowMap::CCubeShadowMap(void)
		{				
			//m_ResultTexIndex = LYNX_NULL_TEXTURE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCubeShadowMap::CCubeShadowMap(CGraphicsSystem *pgs)
		: CShadowMap(pgs)
		{				
			//m_ResultTexIndex = LYNX_NULL_TEXTURE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCubeShadowMap::~CCubeShadowMap(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCubeShadowMap::Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create CubeShadowMap...\r\n")));

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, BUFFER);
			m_lpDepthBuffer->SetName(CString(name)+CString(_T(".DepthBuffer")));
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreateRT(&m_lpDepthBuffer, zformat, name, w, h, mipmaplevel, CRenderer::TT_CUBE_TEXTURE, format, 0))
				return LYNX_FALSE;
			m_Current = m_Texture;

			CRenderer::SAMPLERSTATEDESC SamDesc;
			SamDesc.Default();
			SamDesc.AddressU = CRenderer::AM_CLAMP;
			SamDesc.AddressV = CRenderer::AM_CLAMP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			SamDesc.MinLOD	 = 0;
			SamDesc.MaxLOD	 = 0;
			m_Texture->CreateSamplerState(SamDesc);
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create CubeShadowMap OK.\r\n")));

			return LYNX_TRUE;
		}		
	}
}