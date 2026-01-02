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
		CToneMap::CToneMap(void)
		{
		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CToneMap::CToneMap(CGraphicsSystem *pgs)
		: CMap(pgs)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CToneMap::~CToneMap(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CToneMap::Create(const LYNXCHAR *name, CRenderer::FORMAT format, int num)
		{
			int			W, H;
			LYNXCHAR	Name[64];

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ToneMap...\r\n")));

			SetName(name);
			W = 1;
			H = 1;	
			m_UtilTextures.resize(num);			
			for (int i=0; i<m_UtilTextures.size(); ++i)
			{				
				W = 1<<(i*2);
				H = 1<<(i*2);

				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_UtilTextures[i], TEXTURE);
				lynxSprintf(Name, _T("%s_down_sampled_%d"), name, i);
				if (!m_UtilTextures[i]->vCreateRT(NULL, CRenderer::FORMAT_NULL, Name, W, H, 1, CRenderer::TT_2D_TEXTURE, format, 0))
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
			for (int i=0; i<m_UtilTextures.size(); i++)
			{
				m_UtilTextures[i]->CreateSamplerState(SamDesc);
			}

			m_Texture = m_UtilTextures[num-1];
			m_Current = m_Texture;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ToneMap OK.\r\n")));

			return LYNX_TRUE;
		}		
	}
}