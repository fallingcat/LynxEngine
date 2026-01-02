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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <Container/LynxSkyLightMap.h>
#include <SceneSystem/Light/LynxLight.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSkyLightMap::CSkyLightMap(void)
	{
		//m_ResultTexIndex = LYNX_NULL_TEXTURE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSkyLightMap::CSkyLightMap(GraphicsSystem::CGraphicsSystem *pgs)
	: CMap(pgs)
	{
		//m_ResultTexIndex = LYNX_NULL_TEXTURE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSkyLightMap::~CSkyLightMap(void)
	{			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSkyLightMap::Create(int mipmaplevel, const LYNXCHAR *name, int w, int h, GraphicsSystem::CRenderer::FORMAT format)
	{			
		int				W, H;
		LYNXCHAR		Name[64];

		LYNX_SET_LOG(CString(GetClassName()), _T("Create Sky Light Map...\n"));	

		m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
		if (!m_Texture->vCreateRT(NULL, GraphicsSystem::CRenderer::FORMAT_NULL, name, w, h, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0))
			return LYNX_FALSE;
		m_Current = m_Texture;		

		if (!CreateUtilityTexture(name, w, h, format, 2, 1))
			return LYNX_FALSE;			

		GraphicsSystem::CRenderer::SAMPLERSTATEDESC SamDesc;
		SamDesc.Default();
		SamDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
		SamDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
		SamDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
		SamDesc.Filter	 = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_LINEAR;
		m_Texture->CreateSamplerState(SamDesc);
		for (int i=0; i<m_UtilTextures.size(); i++)
		{
			m_UtilTextures[i]->CreateSamplerState(SamDesc);
		}

		LYNX_SET_LOG(CString(GetClassName()), _T("Create Sky Light Map OK\n"));	

		return LYNX_TRUE;
	}						
}