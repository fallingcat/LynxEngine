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
#include <Shader/LynxPostSSAOPS.h>
#include <GraphicsSystem/LynxBloomingPostFX.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBloomingPostFX::CBloomingPostFX(CGraphicsSystem *pgs)
		: CPostProcessing(pgs)
		{					
			m_BloomingBlurKernel = 5;
			m_BloomingBlurScale = 1;
			m_lpCurrentGlowMap = NULL; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBloomingPostFX::~CBloomingPostFX(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBloomingPostFX::vCreate(CMap& output)
		{
			CString Name = CString(_T("BLOOMING_POSTFX"));

			CRenderer::DEPTHSTENCILSTATEDESC Desc;			
			CRenderer::BLENDSTATEDESC		 BDesc;
			CRenderer::RASTERIZERSTATEDESC	 RDesc;
			CRenderer::SAMPLERSTATEDESC		 SamDesc;

			Desc.Default();			
			BDesc.Default();
			RDesc.Default();
			SamDesc.Default();

			m_lpMaterial = m_lpGraphicsSystem->GetlpMaterialSystem()->FindMaterial(Name);
			if (!m_lpMaterial)
			{
				m_lpGraphicsSystem->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::POSTPROCESSING, CString(_T("BLOOMING_POSTFX")));
				m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);			
				m_lpMaterial->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_lpMaterial->m_SpPower = 0;
				m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				MaterialSystem::CTechnique* Tech = &m_lpMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(2);
				Tech->SetName(_T("BLOOMING_POSTFX.Tech0"));
				Tech->GetPass(0).SetName(_T("BLOOMING_POSTFX.Tech0.BrightPass"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostBrightPassPS");

				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp		= CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= CRenderer::DB_NONE;
				Desc.DepthFunc		= CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = CRenderer::FM_SOLID;
				RDesc.CullMode = CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;

				Tech->GetPass(1).SetName(_T("BLOOMING_POSTFX.Tech0.BloomingPass"));
				Tech->GetPass(1).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");

				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp		= CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ALL_CHANNELS;
				Tech->GetPass(1).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= CRenderer::DB_NONE;
				Desc.DepthFunc		= CRenderer::TF_ALWAYS;			
				Tech->GetPass(1).CreateDepthStencilState(Desc);				
				RDesc.FillMode = CRenderer::FM_SOLID;
				RDesc.CullMode = CRenderer::CULL_NONE;
				Tech->GetPass(1).CreateRasterizerState(RDesc);
				Tech->GetPass(1).m_bUseFog = LYNX_FALSE;
			}
			m_OutputMap = &output;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBloomingPostFX::vAddSourceMap(CMap& src)
		{
			m_InputMapList.push_back(&src);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBloomingPostFX::vRender(CCameraContainer* cam)
		{
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);	

			// First pass, bright pass
			m_lpGraphicsSystem->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentGlowMap);
			m_lpGraphicsSystem->DisableRenderBuffer(1);	
			m_lpGraphicsSystem->DisableRenderBuffer(2);
			m_lpGraphicsSystem->DisableRenderBuffer(3);
			m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentGlowMap->GetTexture()->vGetWidth(), m_lpCurrentGlowMap->GetTexture()->vGetHeight());
			m_lpGraphicsSystem->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, Color, 1.0f, 0);				
			m_lpGraphicsSystem->SetupScreenSpaceQuadUVOffset(&(m_lpMaterial->GetTechnique(0).GetPass(0)));			
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, cam);
			m_lpGraphicsSystem->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, (*(m_InputMapList.begin()))->GetTexture());
			m_lpGraphicsSystem->DrawScreenSpaceQuad();					

			m_lpCurrentGlowMap->DownSampled();
			//m_OutputMap->BoxBlur(m_BloomingBlurKernel/2, m_BloomingBlurScale);
			m_lpCurrentGlowMap->GaussinBlur(m_BloomingBlurKernel, m_BloomingBlurScale);				

			// Second pass, accumulation pass 
			m_lpGraphicsSystem->SetMapAsRenderBuffer(NULL, 0, m_OutputMap);
			m_lpGraphicsSystem->DisableRenderBuffer(1);	
			m_lpGraphicsSystem->DisableRenderBuffer(2);
			m_lpGraphicsSystem->DisableRenderBuffer(3);
			m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, m_OutputMap->GetTexture()->vGetWidth(), m_OutputMap->GetTexture()->vGetHeight());

			m_lpGraphicsSystem->SetupScreenSpaceQuadUVOffset(&(m_lpMaterial->GetTechnique(0).GetPass(1)));			
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(1).Set(NULL, cam);
			m_lpGraphicsSystem->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_lpCurrentGlowMap->GetCurrent());
			m_lpGraphicsSystem->DrawScreenSpaceQuad();	
		}
	}
}