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
#include <GraphicsSystem/LynxSSAOPostFX.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSSAOPostFX::CSSAOPostFX(CGraphicsSystem *pgs)
		: CPostProcessing(pgs)
		{
			IMPLEMENT_PARAM_FLOAT(m_SSAOSamplingKernel, 0)
			IMPLEMENT_PARAM_BOOL(m_bSSAOBlur, 0)
			IMPLEMENT_PARAM_FLOAT(m_SSAOBlurKernel, 0)
			IMPLEMENT_PARAM_FLOAT(m_SSAOBlurScale, 0)
			IMPLEMENT_PARAM_FLOAT(m_SSAOBlurSharpness, 0)
			IMPLEMENT_PARAM_FLOAT(m_SSAOThreshold, 0)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSSAOPostFX::~CSSAOPostFX(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSSAOPostFX::vCreate(CMap& output)
		{
			CString Name = CString(_T("SSAO_POSTFX"));

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
				m_lpGraphicsSystem->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::POSTPROCESSING, CString(_T("SSAO_POSTFX")));
				m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);			
				m_lpMaterial->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_lpMaterial->m_SpPower = 0;
				m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				MaterialSystem::CTechnique* Tech = &m_lpMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("SSAO_POSTFX.Tech0"));
				Tech->GetPass(0).SetName(_T("SSAO_POSTFX.Tech0.P0"));
				if (m_SSAOQuality >= 2)
				{
					Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "PostSSAOPS");
				}
				else
				{
					Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "PostSSAO_LQ_PS");
				}			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ZERO;
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
			}
			
			//Shader::CPostSSAOPS* lpSSAOPS = (Shader::CPostSSAOPS*)&(*(m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()));
			//lpSSAOPS->SetParam(static_cast<CCameraContainer*>(m_lpGraphicsSystem->GetlpEngine()->GetlpSceneSystem()->GetlpCurrentScene()->GetlpCurrentCamera()->GetlpContainer()));

			m_OutputMap = &output;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSSAOPostFX::vAddSourceMap(CMap& src)
		{
			m_InputMapList.push_back(&src);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSSAOPostFX::vRender(CCameraContainer* cam)
		{
			m_lpGraphicsSystem->SetMapAsRenderBuffer(NULL, 0, m_OutputMap);		
			m_lpGraphicsSystem->DisableRenderBuffer(1);	
			m_lpGraphicsSystem->DisableRenderBuffer(2);
			m_lpGraphicsSystem->DisableRenderBuffer(3);
			m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, m_OutputMap->GetTexture()->vGetWidth(), m_OutputMap->GetTexture()->vGetHeight());	

			cam->CalculateFarCornerDir(((float)m_OutputMap->GetCurrent()->vGetWidth())/((float)m_OutputMap->GetCurrent()->vGetHeight()));			
			m_lpGraphicsSystem->SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(&(m_lpMaterial->GetTechnique(0).GetPass(0)), *cam);			
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, cam);
			m_lpGraphicsSystem->DrawScreenSpaceQuad();

			if (m_bSSAOBlur)
			{
				// TODO : remove redundant DownSampled() call (2010.11.10)
				m_OutputMap->DownSampled();
				//m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::SSAOMAP)->BoxBlurEdgeCheck(m_SSAOBlurKernel, m_SSAOBlurSharpness, m_SSAOBlurScale);			
				m_OutputMap->BilateralBlur(m_SSAOBlurKernel, m_SSAOBlurSharpness, m_SSAOBlurScale);			
			}

			/*
			m_lpGraphicsSystem->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			m_lpGraphicsSystem->DisableRenderBuffer(1);	
			m_lpGraphicsSystem->DisableRenderBuffer(2);
			m_lpGraphicsSystem->DisableRenderBuffer(3);			
			m_lpGraphicsSystem->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			m_lpGraphicsSystem->SetupScreenSpaceQuadUVOffset(&(m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::ONECHANNEL_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
			m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::ONECHANNEL_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
			m_lpGraphicsSystem->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::SSAOMAP)->GetCurrent());			
			m_lpGraphicsSystem->DrawScreenSpaceQuad();	
			*/
		}
	}
}