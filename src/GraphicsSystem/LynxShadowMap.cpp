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
		CShadowMap::CShadowMap(void)
		{
			m_bDepthTexture = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShadowMap::CShadowMap(CGraphicsSystem *pgs)
		: CMap(pgs)
		{
			m_bDepthTexture = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShadowMap::~CShadowMap(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CShadowMap::Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift)
		{			
			int				W, H;
			LYNXCHAR		Name[128];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ShadowMap...\r\n")));

			W = w;
			H = h;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, BUFFER);
			m_lpDepthBuffer->SetName(CString(name)+CString(_T(".DepthBuffer")));
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreateRT(&m_lpDepthBuffer, zformat, name, W, H, mipmaplevel, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0))
				return LYNX_FALSE;
			m_Current = m_Texture;

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
					m_bDepthTexture = LYNX_TRUE;		
					lynxSprintf(Name, _T("%s.DummyTexture"), name);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_DummyTexture, TEXTURE);
					if (!m_DummyTexture->vCreateRT(NULL, GraphicsSystem::CRenderer::FORMAT_NULL, Name, W, H, mipmaplevel, GraphicsSystem::CRenderer::TT_2D_TEXTURE, GraphicsSystem::CRenderer::FORMAT_R16F, 0))
						return LYNX_FALSE;					
					break;
				default:
					m_bDepthTexture = LYNX_FALSE;
					break;
			}

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

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ShadowMap OK.\r\n")));

			return LYNX_TRUE;
		}					
	}
}