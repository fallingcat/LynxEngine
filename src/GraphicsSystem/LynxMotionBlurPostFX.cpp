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
#include <GraphicsSystem/LynxMotionBlurPostFX.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMotionBlurPostFX::CMotionBlurPostFX(CGraphicsSystem *pgs)
		: CPostProcessing(pgs)
		{					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMotionBlurPostFX::~CMotionBlurPostFX(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMotionBlurPostFX::vCreate(CMap& output)
		{
			CString Name = CString(_T("MOTIONBLUR_POSTFX"));

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
				m_lpGraphicsSystem->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::POSTPROCESSING, CString(_T("MOTIONBLUR_POSTFX")));
				m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);			
				m_lpMaterial->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_lpMaterial->m_SpPower = 0;
				m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				MaterialSystem::CTechnique* Tech = &m_lpMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(2);
				Tech->SetName(_T("MOTIONBLUR_POSTFX.Tech0"));
				Tech->GetPass(0).SetName(_T("MOTIONBLUR_POSTFX.Tech0.BrightPass"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostMtionBlurPS");

				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ONE_MINUS_SRC_ALPHA;
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
			m_OutputMap = &output;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMotionBlurPostFX::vAddSourceMap(CMap& src)
		{
			m_InputMapList.push_back(&src);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMotionBlurPostFX::vRender(CCameraContainer* cam)
		{
		}
	}
}