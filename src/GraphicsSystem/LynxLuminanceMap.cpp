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

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CLuminanceMap::CLuminanceMap(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CLuminanceMap::CLuminanceMap(CGraphicsSystem *pgs)
		: CMap(pgs)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CLuminanceMap::~CLuminanceMap(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CLuminanceMap::Create(const LYNXCHAR *name, CRenderer::FORMAT format)
		{		
			LYNXCHAR	Name[64];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create LuminanceMap...\r\n")));

			m_UtilTextures.resize(2);

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_UtilTextures[0], TEXTURE);
			lynxSprintf(Name, _T("%s_%d"), name, 0);
			if (!m_UtilTextures[0]->vCreateRT(NULL, CRenderer::FORMAT_NULL, Name, 1, 1, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0))			
				return LYNX_FALSE;				

			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_UtilTextures[1], TEXTURE);
			lynxSprintf(Name, _T("%s_%d"), name, 1);
			if (!m_UtilTextures[1]->vCreateRT(NULL, CRenderer::FORMAT_NULL, Name, 1, 1, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0))			
				return LYNX_FALSE;				

			CRenderer::SAMPLERSTATEDESC SamDesc;
			SamDesc.Default();
			SamDesc.AddressU = CRenderer::AM_CLAMP;
			SamDesc.AddressV = CRenderer::AM_CLAMP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			for (int i=0; i<m_UtilTextures.size(); i++)
			{
				m_UtilTextures[i]->CreateSamplerState(SamDesc);
			}

			m_Current = m_UtilTextures[0];
			m_Last = m_UtilTextures[1];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create LuminanceMap OK.\r\n")));

			return LYNX_TRUE;
		}		
	}
}