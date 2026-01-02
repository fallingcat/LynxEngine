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

#include <LynxRenderableObj.h>
#include <SceneSystem/LynxScnRenderer.h>
#include <SceneSystem/StaticObj/LynxSky.h>
#include <Shader/LynxPostGetLuminancePS.h>
#include <Shader/LynxPostComputeLuminancePS.h>
#include <Shader/LynxPostAverageLuminancePS.h>
#include <Shader/LynxConstantColorPS.h>
#include <Shader/LynxPostSSAOPS.h>
#include <GraphicsSystem/LynxSSAOPostFX.h>
#include <GraphicsSystem/LynxBloomingPostFX.h>

//#define __TEST_POSTFX_CLASS__				1

namespace LynxEngine 
{
	namespace SceneSystem 
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::CRenderer(CScene* s)
		: CObj(s->GetlpEngine())
		{			
			m_lpScene = s;
			m_VisOpaqueContainerList.clear();
            m_NonSortingVisOpaqueContainerList.clear();
			m_VisTranslucentContainerList.clear();
			m_VisDistortionContainerList.clear();
			
			m_lpCurrentRNRData = NULL;
			m_RNRData.clear();

			m_lpCurrentLightingPassTarget = NULL;
			m_lpCurrentTranslucentLightingPassTarget = NULL;
			m_lpCurrentGlowMap = NULL;			
			m_lpTargetBuffer = NULL;

			RESET_ALL_SCENE_COUNTERS

			m_RenderingOptions.RenderMode = RENDER_NORMAL;
			m_RenderingOptions.bUseBackgroundTexture = LYNX_FALSE;
			m_RenderingOptions.ClearColor.Red = 0;
			m_RenderingOptions.ClearColor.Green = 0;
			m_RenderingOptions.ClearColor.Blue = 0;
			m_RenderingOptions.ClearColor.Alpha = 255;
			m_RenderingOptions.bDepthDownSampling = LYNX_FALSE;
			m_RenderingOptions.bSSAOPass = LYNX_TRUE;
			m_RenderingOptions.SSAOSamplingKernel = 0.8f;
			m_RenderingOptions.bSSAOBlur = LYNX_TRUE;
			m_RenderingOptions.SSAOBlurKernel = 5.0f;
			m_RenderingOptions.SSAOBlurScale = 1.0f;
			m_RenderingOptions.SSAOBlurSharpness = 160.0f;
			m_RenderingOptions.SSAOThreshold = 5.0f;
			m_RenderingOptions.bGlowPass = LYNX_FALSE;
			m_RenderingOptions.NumSplittedShadowMap = 0;

			LynxEngine::GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
			BDesc.Default();
			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;		
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateBlendState(m_DepthComplexBlendState, BDesc);

			m_BackgroundUVOffset[0].u = 0.0f;	m_BackgroundUVOffset[0].v = 0.0f;
			m_BackgroundUVOffset[1].u = 0.0f;	m_BackgroundUVOffset[1].v = 0.0f;
			m_BackgroundUVOffset[2].u = 0.0f;	m_BackgroundUVOffset[2].v = 0.0f;
			m_BackgroundUVOffset[3].u = 0.0f;	m_BackgroundUVOffset[3].v = 0.0f;						
			//Create background texture screen quad
			m_lpBackgroundQuad = LYNXNEW GraphicsSystem::CScreenQuad(m_lpScene->GetlpEngine()->GetlpGraphicsSystem());
			m_lpBackgroundQuad->Create();

			//IMPLEMENT_PARAM_ENUM(m_RenderingOptions.RenderMode, 0)
			IMPLEMENT_PARAM_URGBA(m_RenderingOptions.ClearColor, 0)
			IMPLEMENT_PARAM_BOOL(m_RenderingOptions.bUseBackgroundTexture, 0)
			IMPLEMENT_PARAM_COUNTEDPTR(m_RenderingOptions.BackgroundTexture, 0)
			IMPLEMENT_PARAM_BOOL(m_RenderingOptions.bSSAOPass, 0)
			IMPLEMENT_PARAM_FLOAT(m_RenderingOptions.SSAOSamplingKernel, 0)
			IMPLEMENT_PARAM_BOOL(m_RenderingOptions.bSSAOBlur, 0)
			IMPLEMENT_PARAM_FLOAT(m_RenderingOptions.SSAOBlurKernel, 0)
			IMPLEMENT_PARAM_FLOAT(m_RenderingOptions.SSAOBlurScale, 0)
			IMPLEMENT_PARAM_FLOAT(m_RenderingOptions.SSAOBlurSharpness, 0)
			IMPLEMENT_PARAM_FLOAT(m_RenderingOptions.SSAOThreshold, 0)
			IMPLEMENT_PARAM_BOOL(m_RenderingOptions.bGlowPass, 0)
			IMPLEMENT_PARAM_INT(m_RenderingOptions.NumSplittedShadowMap, 0)			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::~CRenderer(void)
		{
			for (int i=0; i<m_MapArray.size(); i++)
				LYNXDEL (m_MapArray[i]);		
			m_MapArray.clear();

			//for (int i=0; i<m_MaterialArray.size(); i++)
				//m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->DeleteMaterial(m_MaterialArray[i]);
			m_MaterialArray.clear();

			if (m_lpBackgroundQuad)
				LYNXDEL m_lpBackgroundQuad;

			//m_lpEngine->ClearInvalidResource();
			//m_lpEngine->ClearInvalidShaderClass();

			m_ShadowMapSplittedDepthArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::vCreate()
		{
			GraphicsSystem::CGraphicsSystem* lpGfxSystem = m_lpScene->GetlpEngine()->GetlpGraphicsSystem();

			m_MapArray.resize(NUM_MAPTYPE);	

			// Create all essential maps																	 
			CRenderer::vResize(lpGfxSystem->GetlpRenderer()->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetlpRenderer()->GetSystemBackBuffer()->vGetHeight());

			#if defined(__DESKTOP__) && !defined(__OGLES2__) && !defined(__MOBILE_SIMULATION__)
				m_MapArray[REFLECTIONMAP] = LYNXNEW GraphicsSystem::CMap(lpGfxSystem);
				m_MapArray[REFRACTIONMAP] = LYNXNEW GraphicsSystem::CMap(lpGfxSystem);
				m_MapArray[RNRGLOWMAP] = LYNXNEW GraphicsSystem::CGlowMap(lpGfxSystem);	
				if (lpGfxSystem->m_Config.bSharedShadowMap)
				{
					m_MapArray[SHARED_SHADOWMAP] = LYNXNEW GraphicsSystem::CShadowMap(lpGfxSystem);
					m_MapArray[SHARED_CUBESHADOWMAP] = LYNXNEW GraphicsSystem::CCubeShadowMap(lpGfxSystem);
				}
				if (lpGfxSystem->m_Config.bHDRRendering)
				{
					if (m_MapArray[TONEMAP])
						LYNXDEL(m_MapArray[TONEMAP]);
					m_MapArray[TONEMAP] = LYNXNEW GraphicsSystem::CToneMap(lpGfxSystem);

					if (m_MapArray[LUMINANCEMAP])
						LYNXDEL(m_MapArray[LUMINANCEMAP]);
					m_MapArray[LUMINANCEMAP] = LYNXNEW GraphicsSystem::CLuminanceMap(lpGfxSystem);				
				}

				if (m_MapArray[REFLECTIONMAP])
				{
					if (!m_MapArray[REFLECTIONMAP]->Create(GraphicsSystem::CRenderer::FORMAT_D24S8, 1, _T("Reflection Map"), lpGfxSystem->m_Config.RNRMapSize, lpGfxSystem->m_Config.RNRMapSize, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))				
						return LYNX_FALSE;				
				}
				if (m_MapArray[REFRACTIONMAP])
				{
					if (!m_MapArray[REFRACTIONMAP]->Create(GraphicsSystem::CRenderer::FORMAT_NULL, 1, _T("Refraction Map"), lpGfxSystem->m_Config.RNRMapSize, lpGfxSystem->m_Config.RNRMapSize, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))				
						return LYNX_FALSE;
					m_MapArray[REFRACTIONMAP]->SetlpDepthBuffer(m_MapArray[REFLECTIONMAP]->GetlpDepthBuffer());
				}
				if (m_MapArray[RNRGLOWMAP])
				{
					if (!((GraphicsSystem::CGlowMap*)m_MapArray[RNRGLOWMAP])->Create(1, _T("RNR Glow Map"), lpGfxSystem->m_Config.RNRMapSize, lpGfxSystem->m_Config.RNRMapSize, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, 2))				
						return LYNX_FALSE;
					m_MapArray[RNRGLOWMAP]->SetlpDepthBuffer(m_MapArray[REFLECTIONMAP]->GetlpDepthBuffer());
				}	
				if (m_MapArray[SHARED_SHADOWMAP])
				{
					if (lpGfxSystem->m_Config.MaxSplittedShadowMap > 1)
					{
						if (!((GraphicsSystem::CShadowMap*)m_MapArray[SHARED_SHADOWMAP])->Create(lpGfxSystem->m_Config.ShadowMapDepthFormat, 1, _T("Shared Shadow Map"), lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapFormat, lpGfxSystem->m_Config.MaxSplittedShadowMap-1, 0))				
							return LYNX_FALSE;
					}
					else
					{
						if (!((GraphicsSystem::CShadowMap*)m_MapArray[SHARED_SHADOWMAP])->Create(lpGfxSystem->m_Config.ShadowMapDepthFormat, 1, _T("Shared Shadow Map"), lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapFormat, 2, 1))				
							return LYNX_FALSE;
					}
					m_MapArray[SHARED_SHADOWMAP]->SetlpDepthBuffer(m_MapArray[SHARED_SHADOWMAP]->GetlpDepthBuffer());
				}
				if (m_MapArray[SHARED_CUBESHADOWMAP])
				{
					if (!((GraphicsSystem::CCubeShadowMap*)m_MapArray[SHARED_CUBESHADOWMAP])->Create(lpGfxSystem->m_Config.ShadowMapDepthFormat, 1, _T("Shared Cube Shadow Map"), lpGfxSystem->m_Config.CubeShadowMapSize, lpGfxSystem->m_Config.CubeShadowMapSize, lpGfxSystem->m_Config.CubeShadowMapFormat))				
						return LYNX_FALSE;
					m_MapArray[SHARED_CUBESHADOWMAP]->SetlpDepthBuffer(m_MapArray[SHARED_CUBESHADOWMAP]->GetlpDepthBuffer());
				}
				if (m_MapArray[TONEMAP])
				{
					if (!((GraphicsSystem::CToneMap*)m_MapArray[TONEMAP])->Create(_T("Tone Map"), lpGfxSystem->m_Config.ToneMappingFormat, 4))				
						return LYNX_FALSE;	
				}

				if (m_MapArray[LUMINANCEMAP])
				{
					if (!((GraphicsSystem::CLuminanceMap*)m_MapArray[LUMINANCEMAP])->Create(_T("Luminance Map"), lpGfxSystem->m_Config.ToneMappingFormat))				
						return LYNX_FALSE;								
				}			
				m_lpEngine->ClearInvalidResource();						

				m_MaterialArray.resize(NUM_MATERIALTYPE);

				GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC Desc;			
				GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
				GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
				GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

				Desc.Default();			
				BDesc.Default();
				RDesc.Default();
				SamDesc.Default();

				MaterialSystem::CTechnique* Tech;

				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[MOTION_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[MOTION_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[MOTION_BLUR]->SetName(_T("MOTION BLUR POSTPROCESS"));
				m_MaterialArray[MOTION_BLUR]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[MOTION_BLUR]->m_SpPower = 0;
				m_MaterialArray[MOTION_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[MOTION_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("MOTION BLUR POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("MOTION BLUR POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostMotionBlurPS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				Tech->GetPass(0).vSetNumTextureAnimation(0);
				
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[GET_LUMINANCE], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[GET_LUMINANCE]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[GET_LUMINANCE]->SetName(_T("GET_LUMINANCE POSTPROCESS"));
				m_MaterialArray[GET_LUMINANCE]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[GET_LUMINANCE]->m_SpPower = 0;
				m_MaterialArray[GET_LUMINANCE]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[GET_LUMINANCE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("GET_LUMINANCE POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("GET_LUMINANCE POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostGetLuminancePS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[COMPUTE_LUMINANCE], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[COMPUTE_LUMINANCE]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[COMPUTE_LUMINANCE]->SetName(_T("COMPUTE_LUMINANCE POSTPROCESS"));
				m_MaterialArray[COMPUTE_LUMINANCE]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[COMPUTE_LUMINANCE]->m_SpPower = 0;
				m_MaterialArray[COMPUTE_LUMINANCE]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[COMPUTE_LUMINANCE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("COMPUTE_LUMINANCE POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("COMPUTE_LUMINANCE POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostComputeLuminancePS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[AVERAGE_LUMINANCE], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[AVERAGE_LUMINANCE]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[AVERAGE_LUMINANCE]->SetName(_T("AVERAGE_LUMINANCE POSTPROCESS"));
				m_MaterialArray[AVERAGE_LUMINANCE]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[AVERAGE_LUMINANCE]->m_SpPower = 0;
				m_MaterialArray[AVERAGE_LUMINANCE]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[AVERAGE_LUMINANCE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("AVERAGE_LUMINANCE POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("AVERAGE_LUMINANCE POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostAverageLuminancePS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;			

				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[TONE_MAPPING], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[TONE_MAPPING]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[TONE_MAPPING]->SetName(_T("TONE_MAPPING POSTPROCESS"));
				m_MaterialArray[TONE_MAPPING]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[TONE_MAPPING]->m_SpPower = 0;
				m_MaterialArray[TONE_MAPPING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[TONE_MAPPING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("TONE_MAPPING POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("TONE_MAPPING POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostToneMappingPS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[ADAPT_LUMINANCE], MaterialSystem::CMaterial::POSTPROCESSING);
				m_MaterialArray[ADAPT_LUMINANCE]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[ADAPT_LUMINANCE]->SetName(_T("ADAPT_LUMINANCE POSTPROCESS"));
				m_MaterialArray[ADAPT_LUMINANCE]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[ADAPT_LUMINANCE]->m_SpPower = 0;
				m_MaterialArray[ADAPT_LUMINANCE]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[ADAPT_LUMINANCE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("ADAPT_LUMINANCE POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("ADAPT_LUMINANCE POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostAdaptLuminancePS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
							
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[DOF], MaterialSystem::CMaterial::POSTPROCESSING);
				m_MaterialArray[DOF]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[DOF]->SetName(_T("DOF POSTPROCESS"));
				m_MaterialArray[DOF]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[DOF]->m_SpPower = 0;
				m_MaterialArray[DOF]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[DOF]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("DOF POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("DOF POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostDOFPS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;//BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;//BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[BRIGHTPASS], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_MaterialArray[BRIGHTPASS]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[BRIGHTPASS]->SetName(_T("BRIGHTPASS POSTPROCESS"));
				m_MaterialArray[BRIGHTPASS]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[BRIGHTPASS]->m_SpPower = 0;
				m_MaterialArray[BRIGHTPASS]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[BRIGHTPASS]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("BRIGHTPASS POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("BRIGHTPASS POSTPROCESS.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostBrightPassPS");			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);			
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				
			#ifndef __TEST_POSTFX_CLASS__			
				lpGfxSystem->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[SSAO], MaterialSystem::CMaterial::POSTPROCESSING);
				m_MaterialArray[SSAO]->SetResourceFlag(SYSTEM_DEFINED);
				m_MaterialArray[SSAO]->SetName(_T("SSAO POSTPROCESS"));
				m_MaterialArray[SSAO]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
				m_MaterialArray[SSAO]->m_SpPower = 0;
				m_MaterialArray[SSAO]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_MaterialArray[SSAO]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("SSAO POSTPROCESS.Tech0"));
				Tech->GetPass(0).SetName(_T("SSAO POSTPROCESS.Tech0.P0"));
				if (lpGfxSystem->m_Config.SSAOQuality >= 2)
				{
					Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "PostSSAOPS");
				}
				else
				{
					Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "PostSSAO_LQ_PS");
				}			
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
				Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;						
			#endif						

				if (m_MapArray[TONEMAP])
				{
					// Set HDR shading
					int ToneMapIndex = m_MapArray[TONEMAP]->GetNumUtilTextures()-1;
					Shader::CPostGetLuminancePS* lpPostGetLuminancePS = ((Shader::CPostGetLuminancePS*)&(*(m_MaterialArray[GET_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())));
					float tU = 1.0f / ( 3.0f * m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetWidth() );
					float tV = 1.0f / ( 3.0f * m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetHeight() );
					int Index = 0;
					for (int x = -1; x <= 1; x++)
					{
						for (int y = -1; y <= 1; y++)
						{
							lpPostGetLuminancePS->m_SampleOffset[Index].u = x * tU;
							lpPostGetLuminancePS->m_SampleOffset[Index].v = y * tV;
							Index++;
						}
					}
					lpPostGetLuminancePS->SetParam();			
				}
            #else
                /*
				m_MapArray[SHARED_SHADOWMAP] = LYNXNEW GraphicsSystem::CShadowMap(lpGfxSystem);
                if (m_MapArray[SHARED_SHADOWMAP])
                {
                    if (!((GraphicsSystem::CShadowMap*)m_MapArray[SHARED_SHADOWMAP])->Create(lpGfxSystem->m_Config.ShadowMapDepthFormat, 1, _T("Shared Shadow Map"), lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapSize, lpGfxSystem->m_Config.ShadowMapFormat, 2, 1))
                        return LYNX_FALSE;
                    m_MapArray[SHARED_SHADOWMAP]->SetlpDepthBuffer(m_MapArray[SHARED_SHADOWMAP]->GetlpDepthBuffer());
                }
				*/
			#endif

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::vResize(int cx, int cy)
		{
			GraphicsSystem::CGraphicsSystem* lpGfxSystem = m_lpScene->GetlpEngine()->GetlpGraphicsSystem();

			// Resize maps

			#if defined(__DESKTOP__) && !defined(__OGLES2__) && !defined(__MOBILE_SIMULATION__)
				//TODO : allocate resources according to render options
				if (m_MapArray[LIGHTINGPASS_TARGET])
					LYNXDEL(m_MapArray[LIGHTINGPASS_TARGET]);
				m_MapArray[LIGHTINGPASS_TARGET] = LYNXNEW GraphicsSystem::CMap(lpGfxSystem);

				if (m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET])
					LYNXDEL(m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET]);
				m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET] = LYNXNEW GraphicsSystem::CMap(lpGfxSystem);			
							
				if (m_MapArray[SSAOMAP])
					LYNXDEL(m_MapArray[SSAOMAP]);
				m_MapArray[SSAOMAP] = LYNXNEW GraphicsSystem::COffscreen(lpGfxSystem);
				
				if (m_MapArray[DEPTHMAP])
					LYNXDEL(m_MapArray[DEPTHMAP]);
				m_MapArray[DEPTHMAP] = LYNXNEW GraphicsSystem::CDepthMap(lpGfxSystem);
				
				if (m_MapArray[GLOWMAP])
					LYNXDEL(m_MapArray[GLOWMAP]);
				m_MapArray[GLOWMAP] = LYNXNEW GraphicsSystem::CGlowMap(lpGfxSystem);
				
				if (m_MapArray[VELOCITYMAP])
					LYNXDEL(m_MapArray[VELOCITYMAP]);
				m_MapArray[VELOCITYMAP] = LYNXNEW GraphicsSystem::CVelocityMap(lpGfxSystem);
				
				m_lpEngine->ClearInvalidResource();		

				if (m_MapArray[LIGHTINGPASS_TARGET])
				{
					if (!m_MapArray[LIGHTINGPASS_TARGET]->Create(GraphicsSystem::CRenderer::FORMAT_NULL, 1, _T("Lighting Pass Target"), cx, cy, lpGfxSystem->m_Config.LightingFormat, 0, 0))				
						return LYNX_FALSE;
					m_MapArray[LIGHTINGPASS_TARGET]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
				}
				if (m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET])
				{
					if (!m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET]->Create(GraphicsSystem::CRenderer::FORMAT_NULL, 1, _T("Translucent Lighting Pass Target"), cx, cy, lpGfxSystem->m_Config.LightingFormat, 0, 0))				
						return LYNX_FALSE;
					m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
				}								
				if (m_MapArray[SSAOMAP])
				{
					int OffscreenShift = LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 0, 1);
					if (!((GraphicsSystem::COffscreen*)m_MapArray[SSAOMAP])->Create(1, _T("SSAO Map"), cx, cy, lpGfxSystem->m_Config.SSAOMapFormat, 2, OffscreenShift))				
						return LYNX_FALSE;				
				}
				if (m_MapArray[DEPTHMAP])
				{
					int DepthMapShift = LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 0, 3);
					if (!((GraphicsSystem::CDepthMap*)m_MapArray[DEPTHMAP])->Create(_T("Depth Map"), cx, cy, lpGfxSystem->m_Config.DepthMapFormat, (DepthMapShift)?1:0, DepthMapShift))
						return LYNX_FALSE;
					m_MapArray[DEPTHMAP]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
				}
				if (m_MapArray[GLOWMAP])
				{
					int GlowMapShift = LYNX_CLAMP((4-lpGfxSystem->m_Config.PostprocessQuality), 1, 3);					
					if (!((GraphicsSystem::CGlowMap*)m_MapArray[GLOWMAP])->Create(1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))				
						return LYNX_FALSE;
					//m_MapArray[GLOWMAP]->SetDepthBuffer(GetSystemDepthBuffer());
				}

				if (m_MapArray[VELOCITYMAP])
				{
					int VelocityMapShift = LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 0, 3);
					if (!((GraphicsSystem::CVelocityMap*)m_MapArray[VELOCITYMAP])->Create(_T("Velocity Map"), cx<<VelocityMapShift, cy<<VelocityMapShift, GraphicsSystem::CRenderer::FORMAT_G16R16F))
						return LYNX_FALSE;				
				}					
				
				// shaders need to re-bind resources because of viewport resize
				for (int i=0; i<m_MaterialArray.size(); i++)
				{
					m_MaterialArray[i]->RebindResources();	
					m_MaterialArray[i]->SetDirty(LYNX_TRUE);
				}

				#if __TEST_POSTFX_CLASS__			
					GraphicsSystem::CPostProcessing* lpPostFX = LYNXNEW GraphicsSystem::CSSAOPostFX(m_lpEngine->GetlpGraphicsSystem());
					m_PostFXList.push_back(lpPostFX);
					lpPostFX->vCreate(*(m_MapArray[SSAOMAP]));
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_bSSAOBlur			= m_RenderingOptions.bSSAOBlur;
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_SSAOBlurKernel		= m_RenderingOptions.SSAOBlurKernel;
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_SSAOBlurScale		= m_RenderingOptions.SSAOBlurScale;
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_SSAOBlurSharpness	= m_RenderingOptions.SSAOBlurSharpness;
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_SSAOSamplingKernel	= m_RenderingOptions.SSAOSamplingKernel;
					((GraphicsSystem::CSSAOPostFX*)lpPostFX)->m_SSAOThreshold		= m_RenderingOptions.SSAOThreshold;	

					lpPostFX = LYNXNEW GraphicsSystem::CBloomingPostFX(m_lpEngine->GetlpGraphicsSystem());
					m_PostFXList.push_back(lpPostFX);
					lpPostFX->vCreate(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());
				#endif			
			#elif defined (__MOBILE__) || defined(__OGLES2__) || defined(__MOBILE_SIMULATION__)
				switch (m_lpEngine->GetlpKernel()->GetDeviceType())
                {
                    default:
                    case DT_IPHONE_7:
                    case DT_IPHONE_6:
					case DT_IPHONE_5:
					case DT_IPAD_4:
                    case DT_IPAD_5:
					{
                        if (m_MapArray[GLOWMAP])
                            LYNXDEL(m_MapArray[GLOWMAP]);
                        m_MapArray[GLOWMAP] = LYNXNEW GraphicsSystem::CGlowMap(lpGfxSystem);
            
                        if (m_MapArray[GLOWMAP])
                        {
							int GlowMapShift = 2;//LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 3, 4);
                            if (!((GraphicsSystem::CGlowMap*)m_MapArray[GLOWMAP])->Create(1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;
                            //m_MapArray[GLOWMAP]->SetlpDepthBuffer(lpGfxSystem->GetlpRenderer()->GetOffscreenDepthBuffer());
							//m_MapArray[GLOWMAP]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
							
							/*
							int GlowMapShift = 2;
                            if (!(m_MapArray[GLOWMAP])->Create(GraphicsSystem::CRenderer::FORMAT_D24S8, 1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;        
							*/
                        }
                        break;
                    }                    
                        
					case DT_IPOD_1:
                    case DT_IPOD_2:
                    case DT_IPOD_3:
                    case DT_IPOD_4:
                    case DT_IPHONE_1:
                    case DT_IPHONE_2:
                    case DT_IPHONE_3:
                    case DT_IPHONE_4:
                    case DT_IPAD_1:
                    {
                        if (m_MapArray[GLOWMAP])
                            LYNXDEL(m_MapArray[GLOWMAP]);
                        m_MapArray[GLOWMAP] = LYNXNEW GraphicsSystem::CGlowMap(lpGfxSystem);
            
                        if (m_MapArray[GLOWMAP])
                        {
                            int GlowMapShift = 3;//LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 1, 4);
                            if (!((GraphicsSystem::CGlowMap*)m_MapArray[GLOWMAP])->Create(1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;
							//m_MapArray[GLOWMAP]->SetlpDepthBuffer(lpGfxSystem->GetlpRenderer()->GetOffscreenDepthBuffer());
                            //m_MapArray[GLOWMAP]->SetlpDepthBuffer(lpGfxSystem->GetlpMap(GraphicsSystem::OFFSCREEN)->GetlpDepthBuffer());
							//m_MapArray[GLOWMAP]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
							
							/*
							int GlowMapShift = 3;
                            if (!(m_MapArray[GLOWMAP])->Create(GraphicsSystem::CRenderer::FORMAT_D24S8, 1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;                            
								*/
                        }
                        break;
                    }
                }    
				// shaders need to re-bind resources because of viewport resize
				for (int i=0; i<m_MaterialArray.size(); i++)
				{
					m_MaterialArray[i]->RebindResources();	
					m_MaterialArray[i]->SetDirty(LYNX_TRUE);
				}							
			#endif

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SetNumSplittedShadowMap(int num)
		{
			m_ShadowMapSplittedDepthArray.clear();

			m_RenderingOptions.NumSplittedShadowMap = num;
			if (m_RenderingOptions	.NumSplittedShadowMap > m_lpEngine->GetlpGraphicsSystem()->m_Config.MaxSplittedShadowMap)
				m_RenderingOptions.NumSplittedShadowMap = m_lpEngine->GetlpGraphicsSystem()->m_Config.MaxSplittedShadowMap;
			m_ShadowMapSplittedDepthArray.resize(m_RenderingOptions.NumSplittedShadowMap+1);
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief 。
		//		adjust the camera planes to contain the visible scene as tightly as possible
		*
		*	@param 。
		*	@param 。  
		*	@return 。 
		*/
		//-------------------------------------------------------------------------------------------------------		
		void CRenderer::AdjustCameraPlanes(CCameraContainer& cam)
		{
			//if (m_VisObjList.size() == 0) 
			//	return;

			// find the nearest and farthest points of given
			// scene objects in camera's view space
			//
			//float fMaxZ = 0;
			//float fMinZ = FLT_MAX;

			//Vector3 vDir = Normalize(m_vTarget - m_vSource);

			//// for each object
			//for (unsigned int i = 0; i < m_VisObjList.size(); i++)
			//{
			//	SceneObject *pObject = VisibleObjects[i];

			//	// for each point in AABB
			//	for(int j = 0; j < 8; j++)
			//	{
			//	  // calculate z-coordinate distance to near plane of camera
			//	  Vector3 vPointToCam = pObject->m_AABB.m_pPoints[j] - m_vSource;
			//	  float fZ = Dot(vPointToCam, vDir);

			//	  // find boundary values
			//	  if(fZ > fMaxZ) fMaxZ = fZ;
			//	  if(fZ < fMinZ) fMinZ = fZ;
			//	}
			//}

			//// use smallest distance as new near plane
			//// and make sure it is not too small
			//m_fNear = Max(fMinZ, m_fNearMin);
			//// use largest distance as new far plane
			//// and make sure it is larger than nearPlane
			//m_fFar = Max(fMaxZ, m_fNear + 1.0f);
			//// update matrices
			//CalculateMatrices();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::RenderModeFilter()
		{
			if (m_RenderingOptions.RenderMode == RENDER_LIGHTINGONLY)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("TerrainLayer0", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("TerrainLayer1", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("TerrainLayer2", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("TerrainLayer3", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));
			}
			else if (m_RenderingOptions.RenderMode == RENDER_DEPTHCOMPLEX)
			{
				for (int i=0; i<8; i++)
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(i, ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::DEPTHCOMPLEX_TEXTURE));
				
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetBlendState(m_DepthComplexBlendState);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ClearVisOpaqueContainerList()
		{
			m_VisOpaqueContainerList.clear();
            m_NonSortingVisOpaqueContainerList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ClearVisTranslucentContainerList()
		{
			m_VisTranslucentContainerList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::AddContainerToVisList(CContainer* con)
		{
			if (con->IsRenderable() && con->GetlpCurrentMaterial())
			{
				LYNX_ASSERT(con->GetlpCurrentMaterial() != NULL);
				
				switch (con->GetlpCurrentMaterial()->GetMaterialType())
				{
					case MaterialSystem::CMaterial::STANDARD:					
						if (con->GetlpCurrentMaterial()->m_TransparentType == TT_OPAQUE || con->GetlpCurrentMaterial()->m_TransparentType == TT_TRANSPARENT || con->GetlpCurrentMaterial()->m_TransparentType == TT_OPAQUE_TRANSLUCENT)
						{
                            if (con->m_bSort)
                                m_VisOpaqueContainerList.push_back(con);
                            else
                                m_NonSortingVisOpaqueContainerList.push_back(con);
						}
						if (con->GetlpCurrentMaterial()->m_TransparentType == TT_TRANSLUCENT || con->GetlpCurrentMaterial()->m_TransparentType == TT_OPAQUE_TRANSLUCENT)
						{
							m_VisTranslucentContainerList.push_back(con);
						}
						break;

					case MaterialSystem::CMaterial::DISTORTION:
						m_VisDistortionContainerList.push_back(con);							
						break;
				}				
								
				INC_COUNTER(m_Profile.NumVisContainer, 1);

				PERF_COUNTER_INC(_T("Scene"), _T("NumVisContainer"), 1)
			}
			for (CList<CContainer*>::CIterator Sub = con->GetSubContainerList().begin(); Sub != con->GetSubContainerList().end(); ++Sub)
			{
				AddContainerToVisList((*Sub));
			}								
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::AddObjToVisList(CRenderableObj* obj)
		{
			RNRDATA Data;

			m_lpScene->GetVisObjList().push_back(obj);
			if (obj->GetlpContainer())
			{
				AddContainerToVisList(obj->GetlpContainer());
				if (obj->IsReflective() || obj->IsRefractive())
				{
					lynxMemCpy(&Data.Plane, &obj->GetRNRPlane(), sizeof(LYNXPLANE));
					Data.lpContainer = obj->GetlpContainer();
					m_RNRData.push_back(Data);
				}
			}
			INC_COUNTER(m_Profile.NumVisObj, 1);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   		
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SetupDepthLayerContainerList()
		{			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			
			for (int i=0; i<CRenderableObj::NUM_DEPTHLAYERTYPE; i++)
			{				
				m_DepthLayerContainerList[i].clear();				
			}
			for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
			{				
				(*Con)->AddToRenderList(m_DepthLayerContainerList[(*Con)->GetlpRenObj()->GetDepthLayer()]);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SortRenderList(CList<CContainer*>& list, SORTTYPE type, SORTMODE mode, CVector3* lpv)
		{
			CList<CContainer*>::CIterator Con;

			if (type == SORT_NONE)
				return;

			switch (mode)
			{
				case SORT_BY_DISTANCE:						
					for (Con = list.begin(); Con != list.end(); ++Con)
					{
						(*Con)->CalculateDist((*lpv));
					}
					switch (type)
					{
						case FRONT_TO_BACK:
							list.sort(CContainer::compare_front2back);
							break;

						case BACK_TO_FRONT:
							list.sort(CContainer::compare_back2front);
							break;
					}
					break;

				case SORT_BY_MATERIAL:
					list.sort(CContainer::compare_material);
					break;

				case SORT_BY_DEPTH_LAYER:
					switch (type)
					{
						case FRONT_TO_BACK:
							list.sort(CContainer::compare_depthlayer_f2b);										
							break;

						case BACK_TO_FRONT:
							list.sort(CContainer::compare_depthlayer_b2f);										
							break;
					}					
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SortVisList(CONTAINERLISTTYPE listtype, SORTTYPE type, SORTMODE mode, CVector3* lpv)
		{
			CList<CContainer*>::CIterator Con;

			switch (listtype)
			{
				case CLT_TRANSLUCENT:
					SortRenderList(m_VisTranslucentContainerList, type, mode, lpv);
					break;

				case CLT_OPAQUE:
					SortRenderList(m_VisOpaqueContainerList, type, mode, lpv);
					break;

				case CLT_DISTORTION:
					SortRenderList(m_VisDistortionContainerList, type, mode, lpv);					
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ZPass(CCameraContainer& camera)
		{			
			CContainer* lpCon;				
			CList<CContainer*>::CIterator Container;
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			float FarPlane[4];

			if (m_lpCurrentRNRData) 
			{
				//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(0x00);
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);								
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);				
			}
			else
			{
				//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::GREEN_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL|GraphicsSystem::CRenderer::ALPHA_CHANNEL);
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_MapArray[DEPTHMAP]);								
				if (camera.m_bMotionBlur)
				{
					m_MapArray[VELOCITYMAP]->GetCurrent()->vSetAsRT(1);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				}
				else
				{
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				}
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::FILL)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				Shader::CConstantColorPS* lpPS = ((Shader::CConstantColorPS*)&(*(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::FILL)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())));
				lpPS->m_ConstantColor[0] = lpPS->m_ConstantColor[1] = lpPS->m_ConstantColor[2] = lpPS->m_ConstantColor[3] = camera.m_Far;
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[DEPTHMAP]->GetTexture()->vGetWidth(), m_MapArray[DEPTHMAP]->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);		
				m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::FILL)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);				
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
			}
			
			CVector3 ViewerPos;
			camera.vGetPosition(ViewerPos);
			SortVisList(CLT_OPAQUE, FRONT_TO_BACK, SORT_BY_DISTANCE, &ViewerPos);

			//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL);
			for (Container = m_VisOpaqueContainerList.begin(); Container != m_VisOpaqueContainerList.end(); ++Container)
			{
				if (m_lpCurrentRNRData && m_lpCurrentRNRData->lpContainer == (*Container)) 
					continue;
				(*Container)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ZPASS);
				(*Container)->OcclusionRender(&camera);				
			}
            
            for (Container = m_NonSortingVisOpaqueContainerList.begin(); Container != m_NonSortingVisOpaqueContainerList.end(); ++Container)
			{
				if (m_lpCurrentRNRData && m_lpCurrentRNRData->lpContainer == (*Container)) 
					continue;
				(*Container)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ZPASS);
				(*Container)->OcclusionRender(&camera);				
			}
			//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::GREEN_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL|GraphicsSystem::CRenderer::ALPHA_CHANNEL);

			if (m_MapArray[DEPTHMAP]->HasDownsampled())
				m_MapArray[DEPTHMAP]->DownSampled();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Dest : <CurrentLightingPassTarget>
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SSAOPass(CCameraContainer& camera)
		{
		#if __TEST_POSTFX_CLASS__			
			(*(m_PostFXList.begin()))->vRender(&camera);
		#else			
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_MapArray[SSAOMAP]);		
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[SSAOMAP]->GetTexture()->vGetWidth(), m_MapArray[SSAOMAP]->GetTexture()->vGetHeight());	

			camera.CalculateFarCornerDir(((float)m_MapArray[SSAOMAP]->GetCurrent()->vGetWidth())/((float)m_MapArray[SSAOMAP]->GetCurrent()->vGetHeight()));			
			m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(&(m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), camera);			
			m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
			m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();				

			if (m_RenderingOptions.bSSAOBlur)
			{
				// TODO : remove redundant DownSampled() call (2010.11.10)
				m_MapArray[SSAOMAP]->DownSampled();
				//m_MapArray[SSAOMAP]->BoxBlurEdgeCheck(m_RenderingOptions.SSAOBlurKernel, m_RenderingOptions.SSAOBlurSharpness, m_RenderingOptions.SSAOBlurScale);			
				m_MapArray[SSAOMAP]->BilateralBlur(m_RenderingOptions.SSAOBlurKernel, m_RenderingOptions.SSAOBlurSharpness, m_RenderingOptions.SSAOBlurScale);			
			}
		#endif

			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);			
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ONECHANNEL_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
			m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ONECHANNEL_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_MapArray[SSAOMAP]->GetCurrent());			
			m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Dest : <CurrentLightingPassTarget>
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SSAO_SSGI_Pass(CCameraContainer& camera)
		{
			//CContainer* lpCon;				
			//CList<CRenderableObj*>::CIterator Obj;
			//static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);			
			//LYNXMATRIX ProjM;
			//static Math::CMatrix4 ScaleM(0.5f,	0.0f,	0.0f,	0.0f,
			//							 0.0f,  -0.5f,  0.0f,   0.0f,
			//							 0.0f,  0.0f,   1.0f,   0.0f,
			//							 0.5f,	0.5f,	0.0f,	1.0f);						

			//m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_MapArray[SSAOMAP]);		
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[SSAOMAP]->GetTexture()->vGetWidth(), m_MapArray[SSAOMAP]->GetTexture()->vGetHeight());
			//lynxMatrixXMatrix(&ProjM, &camera.GetProjectionMatrix(), &ScaleM);							

			//camera.CalculateFarCornerDir(((float)m_MapArray[SSAOMAP]->GetCurrent()->vGetWidth())/((float)m_MapArray[SSAOMAP]->GetCurrent()->vGetHeight()));			
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DEPTHMAP).c_str(), m_MapArray[DEPTHMAP]->GetCurrent());
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetTexture("DiffuseMap", m_lpEngine->GetlpGraphicsSystem()->GetlpMap(GraphicsSystem::OFFSCREEN)->GetTexture());
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetTexture("NoiseMap", m_lpEngine->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE));
			////m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_MapArray[DEPTHMAP]->GetCurrent());
			////m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(2, m_lpEngine->GetlpGraphicsSystem()->GetNoiseTexture());
			//float NoiseTile[2];
			//NoiseTile[0] = m_MapArray[SSAOMAP]->GetTexture()->vGetWidth()/m_lpEngine->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetWidth();
			//NoiseTile[1] = m_MapArray[SSAOMAP]->GetTexture()->vGetHeight()/m_lpEngine->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::NOISE_TEXTURE)->vGetHeight();
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetConstantM("gProjM", &ProjM);				
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetConstantF("gKernel", m_RenderingOptions.SSAOKernel);	
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetConstantF("gNoiseTile", NoiseTile, 2);				
			//m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetConstantF("gThreshold", m_RenderingOptions.SSAOThreshold);
			//if (m_lpEngine->GetlpGraphicsSystem()->m_Config.SSAOQuality <= 1)
			//{
			//	float RTSize[2] = {m_MapArray[SSAOMAP]->GetCurrent()->vGetWidth(), m_MapArray[SSAOMAP]->GetCurrent()->vGetHeight()};
			//	m_MaterialArray[SSAO]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetConstantF("gRTSize", RTSize, 2);					
			//}
			//m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceDepthQuad(camera);				

			//m_MapArray[SSAOMAP]->DownSampled();
			////m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->BoxBlur(16, 2.0f);			
			//m_MapArray[SSAOMAP]->BoxBlurEdgeCheck(4, 2.0f);			
			//
			//m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			//m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);			
			//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			////m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ONECHANNEL_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set();
			//m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
			//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_MapArray[SSAOMAP]->GetCurrent());			
			//m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ShadowmapPass(CLight* lplit, CCameraContainer& camera)
		{
			if (lplit->m_bCastShadow && lplit->IsShadowmapDirty())
			{	
				m_lpEngine->GetlpGraphicsSystem()->Clipping(LYNX_FALSE);
				m_lpEngine->GetlpGraphicsSystem()->PushClipPlaneStates();				
				m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, NULL);
				
				lplit->vGenerateShadowMap(&camera);

				m_lpEngine->GetlpGraphicsSystem()->Clipping(LYNX_TRUE);
				m_lpEngine->GetlpGraphicsSystem()->PopClipPlaneStates();				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Description : Render scene objects to <CurrentLightingPassTarget>, <CurrentGlowMap>		
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::OpaqueLightingPass(CCameraContainer& camera)
		{			
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}
			for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
			{				
				(*Con)->AddToCurrentMaterialRenderList();
			}
            for (Con = m_NonSortingVisOpaqueContainerList.begin(); Con != m_NonSortingVisOpaqueContainerList.end(); ++Con)
			{				
				(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();

				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					if (m_lpCurrentRNRData && m_lpCurrentRNRData->lpContainer == (*Con)) 
						continue;

					if (m_lpScene->GetCurrentAmbientLight())
					{						
						m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(m_lpScene->GetCurrentAmbientLight());
						(*Con)->vLighting(&camera, m_lpScene->GetCurrentAmbientLight());
						INC_COUNTER(m_Profile.NumRenderedLight, 1);
					}

					CList<CGameObj*>& DLitList = (*Con)->GetlpRenObj()->GetVisDynamicLightList();					
					for (Lit = DLitList.begin(); Lit != DLitList.end(); ++Lit)
					{
						CLight* lpLit = static_cast<CLight*>((*Lit)); 
						m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(lpLit);
						ShadowmapPass(lpLit, camera);
						(*Con)->vLighting(&camera, lpLit);				
						INC_COUNTER(m_Profile.NumRenderedLight, 1);
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::TranslucentLightingPass(CCameraContainer& camera)
		{	
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			
			CVector3 ViewerPos;
			camera.vGetPosition(ViewerPos);
			SortVisList(CLT_TRANSLUCENT, BACK_TO_FRONT, SORT_BY_DISTANCE, &ViewerPos);

			for (Con = m_VisTranslucentContainerList.begin(); Con != m_VisTranslucentContainerList.end(); ++Con)
			{
				if (m_lpCurrentRNRData && m_lpCurrentRNRData->lpContainer == (*Con)) 
					continue;

				if (m_lpScene->GetCurrentAmbientLight())
				{						
					(*Con)->SetCurrentLight(*m_lpScene->GetCurrentAmbientLight());
					(*Con)->vTranslucentRender(&camera);
					INC_COUNTER(m_Profile.NumRenderedLight, 1);
				}

				CList<CGameObj*>& SLitList = (*Con)->GetlpRenObj()->GetVisStaticLightList();					
				for (Lit = SLitList.begin(); Lit != SLitList.end(); ++Lit)
				{
					CLight* lpLit = static_cast<CLight*>((*Lit)); 
					(*Con)->SetCurrentLight(*lpLit);
					//ShadowmapPass(lpLit, camera);
					(*Con)->vTranslucentRender(&camera);
					INC_COUNTER(m_Profile.NumRenderedLight, 1);
				}
				CList<CGameObj*>& DLitList = (*Con)->GetlpRenObj()->GetVisDynamicLightList();					
				for (Lit = DLitList.begin(); Lit != DLitList.end(); ++Lit)
				{
					CLight* lpLit = static_cast<CLight*>((*Lit)); 
					(*Con)->SetCurrentLight(*lpLit);
					//ShadowmapPass(lpLit, camera);
					(*Con)->vTranslucentRender(&camera);
					INC_COUNTER(m_Profile.NumRenderedLight, 1);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::DistortionPass(CCameraContainer& camera)
		{			
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			
			if (m_VisDistortionContainerList.size())
			{
				m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_MapArray[REFRACTIONMAP]->GetTexture(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());				

				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);	
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());				

				CVector3 ViewerPos;
				camera.vGetPosition(ViewerPos);
				//SortVisList(CLT_DISTORTION, BACK_TO_FRONT, SORT_BY_DISTANCE, &ViewerPos);

				for (Con = m_VisDistortionContainerList.begin(); Con != m_VisDistortionContainerList.end(); ++Con)
				{
					if (m_lpCurrentRNRData && m_lpCurrentRNRData->lpContainer == (*Con)) 
						continue;

					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
					(*Con)->GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetPS()->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::REFRACTIONMAP).c_str(), m_MapArray[REFRACTIONMAP]->GetTexture());					
					(*Con)->vPreRender(&camera);
					(*Con)->vRender(&camera);
					(*Con)->vPostRender(&camera);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Render scene objects to <CurrentLightingPassTarget>, <m_lpCurrentGlowMap>		
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::vLightingPass(CCameraContainer& camera)
		{			
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);		
			
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			//m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(1, m_lpCurrentGlowMap);			
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());					

			if (m_RenderingOptions.bSSAOPass)
				SSAOPass(camera);			
			else
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, Color, 1.0f, 0);							

			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			//m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(1, m_lpCurrentGlowMap);			
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());			

			OpaqueLightingPass(camera);
			//TranslucentLightingPass(camera);		
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Render reflection to <CurrentLightingPassTarget>, <m_lpCurrentGlowMap>		
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ReflectionPass(CCameraContainer& camera)
		{			
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			Math::CMatrix4	MirrorM;
			LYNXPLANE		ClipPlane;			
			Math::CVector3	Pos;

			if (!m_RNRData.empty())
			{
				if (m_RNRData[0].lpContainer->IsReflective())
				{
					camera.vGetPosition(Pos);
					if (LYNX_DOT_PRODUCT(Pos, m_RNRData[0].Plane.Normal) < m_RNRData[0].Plane.Distance)
					{
						m_RNRData[0].Plane.Normal.x *= -1.0f;
						m_RNRData[0].Plane.Normal.y *= -1.0f;
						m_RNRData[0].Plane.Normal.z *= -1.0f;
						m_RNRData[0].Plane.Distance *= -1.0f;
					}
					m_lpCurrentLightingPassTarget = m_MapArray[REFLECTIONMAP];
					m_lpCurrentGlowMap = m_MapArray[RNRGLOWMAP];
					m_lpCurrentRNRData = &m_RNRData[0];					
					
					lynxGetMatrixReflectToPlane(&MirrorM, &m_RNRData[0].Plane);
					camera.SetMirror(LYNX_TRUE);
					camera.SetMirrorMatrix(MirrorM);

					ClipPlane.Normal.x = -m_RNRData[0].Plane.Normal.x;
					ClipPlane.Normal.y = -m_RNRData[0].Plane.Normal.y;
					ClipPlane.Normal.z = -m_RNRData[0].Plane.Normal.z;
					ClipPlane.Distance = -m_RNRData[0].Plane.Distance;
					m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, &ClipPlane, camera);

					ZPass(camera); 				
					vLightingPass(camera);						

					m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, NULL);				

					BloomingPass(camera);
					camera.SetMirror(LYNX_FALSE);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Render refraction to <CurrentLightingPassTarget>, <m_lpCurrentGlowMap>		
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::RefractionPass(CCameraContainer& camera)
		{			
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			Math::CVector3 Pos;
			LYNXPLANE ClipPlane;			

			if (!m_RNRData.empty())
			{				
				if (m_RNRData[0].lpContainer->IsRefractive())
				{
					camera.vGetPosition(Pos);
					if (LYNX_DOT_PRODUCT(Pos, m_RNRData[0].Plane.Normal) < m_RNRData[0].Plane.Distance)
					{
						m_RNRData[0].Plane.Normal.x *= -1.0f;
						m_RNRData[0].Plane.Normal.y *= -1.0f;
						m_RNRData[0].Plane.Normal.z *= -1.0f;
						m_RNRData[0].Plane.Distance *= -1.0f;
					}
					m_lpCurrentLightingPassTarget = m_MapArray[REFRACTIONMAP];
					m_lpCurrentGlowMap = m_MapArray[RNRGLOWMAP];
					m_lpCurrentRNRData = &m_RNRData[0];								
					
					ClipPlane.Normal.x = -m_RNRData[0].Plane.Normal.x;
					ClipPlane.Normal.y = -m_RNRData[0].Plane.Normal.y;
					ClipPlane.Normal.z = -m_RNRData[0].Plane.Normal.z;
					ClipPlane.Distance = -m_RNRData[0].Plane.Distance;
					m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, &ClipPlane, camera);

					ZPass(camera); 				
					vLightingPass(camera);

					m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, NULL);				

					BloomingPass(camera);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Get current luminance
		//  Dest : <ToneMap.m_UtilTextures[0]>
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::LuminancePass(CCameraContainer& camera)
		{
			if (m_lpEngine->GetlpGraphicsSystem()->m_Config.bHDRRendering)
			{
				int ToneMapIndex = m_MapArray[TONEMAP]->GetNumUtilTextures()-1;
				m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vSetAsRT(0);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetWidth(), m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetHeight());			
				
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[GET_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[GET_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_lpCurrentLightingPassTarget->GetCurrent());			
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();

				ToneMapIndex--;

				Shader::CPostComputeLuminancePS* lpCLPS = NULL;
				while (ToneMapIndex > 0)
				{
					m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vSetAsRT(0);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetWidth(), m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetHeight());			

					lpCLPS = ((Shader::CPostComputeLuminancePS*)&(*(m_MaterialArray[COMPUTE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())));
					float tU = 1.0f / m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex+1)->vGetWidth();
					float tV = 1.0f / m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex+1)->vGetHeight();
					// Sample from the 16 surrounding points. Since the center point will be in
					// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
					// center.
					int Index = 0;
					for (int y = 0; y<4; y++)
					{
						for (int x = 0; x<4; x++)
						{
							lpCLPS->m_SampleOffset[Index].u = ( x - 1.5f ) * tU;
							lpCLPS->m_SampleOffset[Index].v = ( y - 1.5f ) * tV;
							Index++;
						}
					}
					m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[COMPUTE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
					m_MaterialArray[COMPUTE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex+1));			
					m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();
					ToneMapIndex--;
				}
				m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vSetAsRT(0);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetWidth(), m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex)->vGetHeight());			
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[AVERAGE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				Shader::CPostAverageLuminancePS* lpALPS = ((Shader::CPostAverageLuminancePS*)&(*(m_MaterialArray[AVERAGE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())));
				lynxMemCpy(lpALPS->m_SampleOffset, lpCLPS->m_SampleOffset, sizeof(LYNXUVPOINT)*16);
				m_MaterialArray[AVERAGE_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_MapArray[TONEMAP]->GetUtilTexture(ToneMapIndex+1));						
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();

				m_MapArray[LUMINANCEMAP]->GetLast()->vSetAsRT(0);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, 1, 1);
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[ADAPT_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[ADAPT_LUMINANCE]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);				
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_MapArray[LUMINANCEMAP]->GetCurrent());						
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(1, m_MapArray[TONEMAP]->GetUtilTexture(0));						
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();

				m_MapArray[LUMINANCEMAP]->Switch();
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Do tone mapping post process
		//  Src	 : <CurrentLightingPassTarget> 
		//  Dest : <Offscreen>
		//  <Offscreen> =  <CurrentLightingPassTarget> * ToneMapping()
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ToneMappingPass(CCameraContainer& camera)
		{
			if (m_lpEngine->GetlpGraphicsSystem()->m_Config.bHDRRendering)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());						
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[TONE_MAPPING]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[TONE_MAPPING]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_lpCurrentLightingPassTarget->GetCurrent());			
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(1, m_MapArray[LUMINANCEMAP]->GetCurrent());										
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
			}
			else
			{
				if (m_lpEngine->GetlpGraphicsSystem()->m_Config.bGammaCorrection)
				{
					m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);	
					m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);	
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());
					m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
					m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_MapArray[LIGHTINGPASS_TARGET]->GetTexture());
					m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
				}
				else
				{
					m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture(), m_MapArray[LIGHTINGPASS_TARGET]->GetTexture());
				}				
			}						
			DistortionPass(camera);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Do blooming post process
		//	Src	 : <Offscreen>, 
		//	Temp : <GlowMap>
		//  Dest : <Offscreen>
		//  <Offscreen> = <Offscreen> + <GlowMap>
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::BloomingPass(CCameraContainer& camera)
		{
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);			

			if (camera.m_bBlooming && m_lpCurrentGlowMap)
			{		
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentGlowMap);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentGlowMap->GetTexture()->vGetWidth(), m_lpCurrentGlowMap->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, Color, 1.0f, 0);				
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[BRIGHTPASS]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[BRIGHTPASS]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();					

				m_lpCurrentGlowMap->DownSampled();
				//m_lpCurrentGlowMap->BoxBlur(camera.m_BloomingBlurKernel/2, camera.m_BloomingBlurScale);
				m_lpCurrentGlowMap->GaussinBlur(camera.m_BloomingBlurKernel, camera.m_BloomingBlurScale);				

				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());

				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ACC_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ACC_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture("DiffuseMap", m_lpCurrentGlowMap->GetCurrent());
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	
			}	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Description : Do MotionBlur post process
		//  Src  : <Offscreen>, <Velocity Map>
		//	Temp : <GlowMap>
		//  Dest : <Offscreen>
		//  <Offscreen> = <Offscreen> * MotionBlur()
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::MotionBlurPass(CCameraContainer& camera)
		{
			if (camera.m_bMotionBlur)
			{				
				//m_MapArray[VELOCITYMAP]->BoxBlur(4, 2.0f);
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentGlowMap);						
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());
				
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[MOTION_BLUR]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[MOTION_BLUR]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(1, m_MapArray[VELOCITYMAP]->GetCurrent());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(2, m_MapArray[VELOCITYMAP]->GetLast());				
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();												
				m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture(), m_lpCurrentGlowMap->GetTexture());
			}						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Description : Do DOF blur to <GlowMap-downsampled> then add <GlowMap-downsampled> to <Offscreen> 
		//  Src  : <Offscreen>, <DepthMap> 
		//	Temp : <GlowMap>
		//  Dest : <Offscreen>
		//  <Offscreen> = <Offscreen> + DOF
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::DOFPass(CCameraContainer& camera)
		{			
			// Add DOF layer										
			if (camera.m_bDOF)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->DownSampled();
				//m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->BoxBlur(camera.m_DOFBlurKernel/2, camera.m_DOFBlurScale);		
				m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GaussinBlur(camera.m_DOFBlurKernel, camera.m_DOFBlurScale);		
				
				//m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_MapArray[GLOWMAP]);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);	
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());

				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_MaterialArray[DOF]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_MaterialArray[DOF]->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(1, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetCurrent());			
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetTexture(2, m_MapArray[DEPTHMAP]->GetCurrent());			
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();					
				m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture(), m_lpCurrentGlowMap->GetTexture());			
			}						
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::PostProcessingPass(CCameraContainer& camera)
		{	
			m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);	
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());

			ToneMappingPass(camera);
			BloomingPass(camera);
			MotionBlurPass(camera);		
			DOFPass(camera);				
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SimpleLightingPass(CCameraContainer& camera)
		{	
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}

			for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
			{				
				if (!(*Con)->IsOccluded())
					(*Con)->AddToCurrentMaterialRenderList();
			}
            for (Con = m_NonSortingVisOpaqueContainerList.begin(); Con != m_NonSortingVisOpaqueContainerList.end(); ++Con)
			{				
				if (!(*Con)->IsOccluded())
					(*Con)->AddToCurrentMaterialRenderList();
			}
			
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);																			
					(*Con)->vPreRender(&camera);
					(*Con)->vRender(&camera);					
					(*Con)->vPostRender(&camera);
				}
			}	

			if (m_lpScene->GetlpAtmosphere() && m_lpScene->GetlpAtmosphere()->GetlpSky())
			{
				m_lpScene->GetlpAtmosphere()->GetlpSky()->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);																			
				m_lpScene->GetlpAtmosphere()->GetlpSky()->vRender(&camera);
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(((CSky*)m_lpScene->GetlpAtmosphere()->GetlpSky())->GetSunLight());
			}
						
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}
			for (Con = m_VisTranslucentContainerList.begin(); Con != m_VisTranslucentContainerList.end(); ++Con)
			{				
				if (!(*Con)->IsOccluded())
					(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);																			
					(*Con)->vPreRender(&camera);
					(*Con)->vRender(&camera);	
					(*Con)->vPostRender(&camera);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::NormalRender(CCameraContainer& camera)
		{	
			m_lpScene->ComputeVisList(camera);
			m_lpScene->ComputeVisLightList();

			if (!m_lpEngine->GetlpGraphicsSystem()->m_Config.bDeferredShading)
			{
				CList<CLight*>::CIterator Lit;
				for (Lit = m_lpScene->GetVisLightList().begin(); Lit != m_lpScene->GetVisLightList().end(); ++Lit)
				{
					(*Lit)->vComputeVisObjList();				
				}
			}

			/*
			LYNXBOOL bMotionBlur = camera.m_bMotionBlur;
			camera.m_bMotionBlur = LYNX_FALSE;
			m_lpEngine->GetlpGraphicsSystem()->Clipping(LYNX_TRUE);
			ReflectionPass(camera);
			RefractionPass(camera);
			m_lpEngine->GetlpGraphicsSystem()->Clipping(LYNX_FALSE);			
			camera.m_bMotionBlur = bMotionBlur;
			*/

			m_lpCurrentLightingPassTarget = m_MapArray[LIGHTINGPASS_TARGET];
			m_lpCurrentTranslucentLightingPassTarget = m_MapArray[TRANSLUCENTLIGHTINGPASS_TARGET];
			m_lpCurrentGlowMap = m_MapArray[GLOWMAP];

			ZPass(camera);				
			vLightingPass(camera);			
			LuminancePass(camera);			
			PostProcessingPass(camera);

			m_MapArray[VELOCITYMAP]->Switch();	
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SimpleLightingRender(CCameraContainer& camera)
		{				
			m_lpScene->ComputeVisList(camera);
			m_lpScene->ComputeVisLightList();

			static Math::CColor WhiteColor(1.0f, 1.0f, 1.0f, 1.0f);	
			static Math::CColor BlackColor(0.0f, 0.0f, 0.0f, 1.0f);

			m_lpCurrentRNRData = NULL;			
			m_lpCurrentLightingPassTarget = m_MapArray[LIGHTINGPASS_TARGET];
			m_lpCurrentGlowMap = m_MapArray[GLOWMAP];
			ZPass(camera);	
						
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, BlackColor, 1.0f, 0);	
			SimpleLightingPass(camera);											
			LuminancePass(camera);
			PostProcessingPass(camera);
			m_MapArray[VELOCITYMAP]->Switch();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::DepthComplexRender(CCameraContainer& camera)
		{				
			CContainer* lpCon;				
			CList<CContainer*>::CIterator Container;
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);

			m_lpScene->ComputeVisList(camera);

			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen());			
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->SetSystemDepthBufferAsDepthBuffer();						
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture()->vGetHeight());
			//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::GREEN_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL|GraphicsSystem::CRenderer::ALPHA_CHANNEL);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);

			m_lpEngine->GetlpGraphicsSystem()->m_ConstantColor.Red = 255/8;
			m_lpEngine->GetlpGraphicsSystem()->m_ConstantColor.Green = 255/8;
			m_lpEngine->GetlpGraphicsSystem()->m_ConstantColor.Blue= 255/8;
			m_lpEngine->GetlpGraphicsSystem()->m_ConstantColor.Alpha = 255;
			m_lpEngine->GetlpGraphicsSystem()->m_bOverwriteMaterialColor = LYNX_TRUE;
			
			CVector3 ViewerPos;
			camera.vGetPosition(ViewerPos);
			SortVisList(CLT_OPAQUE, CRenderer::FRONT_TO_BACK, CRenderer::SORT_BY_DISTANCE, &ViewerPos);
			for (Container = m_VisOpaqueContainerList.begin(); Container != m_VisOpaqueContainerList.end(); ++Container)
			{
				(*Container)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
				(*Container)->vPreRender(&camera);
				(*Container)->vRender(&camera);
				(*Container)->vPostRender(&camera);
			}
            for (Container = m_NonSortingVisOpaqueContainerList.begin(); Container != m_NonSortingVisOpaqueContainerList.end(); ++Container)
			{
				(*Container)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
				(*Container)->vPreRender(&camera);
				(*Container)->vRender(&camera);
				(*Container)->vPostRender(&camera);
			}
			m_lpEngine->GetlpGraphicsSystem()->m_bOverwriteMaterialColor = LYNX_FALSE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Render SSAO only
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SSAORender(CCameraContainer& camera)
		{			
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			
			m_lpScene->ComputeVisList(camera);
			m_lpScene->ComputeVisLightList();

			CList<CLight*>::CIterator Lit;
			for (Lit = m_lpScene->GetVisLightList().begin(); Lit != m_lpScene->GetVisLightList().end(); ++Lit)
			{
				(*Lit)->vComputeVisObjList();				
			}			

			m_lpCurrentLightingPassTarget = m_MapArray[LIGHTINGPASS_TARGET];
			ZPass(camera);			
			SSAOPass(camera);			
			m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture(), m_MapArray[LIGHTINGPASS_TARGET]->GetCurrent());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SimplePass(CCameraContainer& camera)
		{	
			PERF_TIMER_RESET_START(_T("Scene"), _T("SimplePass"))

			CList<CContainer*>::CIterator Con;							
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			
			if (m_lpScene->GetlpAtmosphere() && m_lpScene->GetlpAtmosphere()->GetlpSky())
			{
				m_lpScene->GetlpAtmosphere()->GetlpSky()->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);																							
				m_lpScene->GetlpAtmosphere()->GetlpSky()->vRender(&camera);				
			}
			
			switch (m_SortMode)
			{
				case SORT_BY_DEPTH_LAYER:
					/*
					SortVisList(CLT_OPAQUE, BACK_TO_FRONT, SORT_BY_DEPTH_LAYER, NULL);
					for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
					{				
						(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);					
						(*Con)->vRender(&camera);					
					}
					*/
					
					SetupDepthLayerContainerList();

					for (int i=CRenderableObj::NUM_DEPTHLAYERTYPE-1; i>=0; i--)
					{					
						for (Con = m_DepthLayerContainerList[i].begin(); Con != m_DepthLayerContainerList[i].end(); ++Con)
						{
							(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);					
							(*Con)->vPreRender(&camera);					
							(*Con)->vRender(&camera);					
							(*Con)->vPostRender(&camera);					
						}
					}
                    break;

				case SORT_BY_MATERIAL:
				default:
					CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
					CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			

					for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
					{				
						(*Mat)->GetRenderableObjList().clear();				
					}

					for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
					{				
						(*Con)->AddToCurrentMaterialRenderList();
					}
					for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
					{				
						CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
						for (Con = ConList.begin(); Con != ConList.end(); ++Con)
						{
							(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);					
							(*Con)->vPreRender(&camera);					
							(*Con)->vRender(&camera);					
							(*Con)->vPostRender(&camera);					
						}
					}
					break;
			}
            // render containers without sorting
            CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
            CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
            
            for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
            {				
                (*Mat)->GetRenderableObjList().clear();
            }
            
            for (Con = m_NonSortingVisOpaqueContainerList.begin(); Con != m_NonSortingVisOpaqueContainerList.end(); ++Con)
            {				
                (*Con)->AddToCurrentMaterialRenderList();
            }
            for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
            {				
                CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
                for (Con = ConList.begin(); Con != ConList.end(); ++Con)
                {
                    (*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);					
                    (*Con)->vPreRender(&camera);					
					(*Con)->vRender(&camera);					
					(*Con)->vPostRender(&camera);					
                }
            }	
            
            for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
            {
                (*Mat)->GetRenderableObjList().clear();
            }
            for (Con = m_VisTranslucentContainerList.begin(); Con != m_VisTranslucentContainerList.end(); ++Con)
            {
                (*Con)->AddToCurrentMaterialRenderList();
            }
            for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
            {
                CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
                for (Con = ConList.begin(); Con != ConList.end(); ++Con)
                {
                    (*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
					(*Con)->vPreRender(&camera);
                    (*Con)->vRender(&camera);
					(*Con)->vPostRender(&camera);
                }
            }
        
			PERF_TIMER_STOP(_T("Scene"), _T("SimplePass"))
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::SimpleRender(CCameraContainer& camera)
		{		
			m_lpScene->ComputeVisList(camera);

            if (m_lpCurrentLightingPassTarget)
			{			
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);			
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			}
			/*
			else
			{
				m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);							
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
			}	
			*/

			if (m_RenderingOptions.bUseBackgroundTexture && m_RenderingOptions.BackgroundTexture)
			{
                #ifdef __iOS__
                    m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, m_RenderingOptions.ClearColor, 1.0f, 0);
                #else
                    m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::DEPTH_BUFFER, m_RenderingOptions.ClearColor, 1.0f, 0);		
                #endif
				if (camera.GetViewportRect().x2 != camera.GetViewportRect().x1)  // that means the viewport is full screen, so just blt texture  
				{	
					camera.SetViewportRect();
				}
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);			
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), m_BackgroundUVOffset);			
				m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, NULL);
                m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetTexture("DiffuseMap", m_RenderingOptions.BackgroundTexture);				
				m_lpBackgroundQuad->Render();				
			}				
			else
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER|GraphicsSystem::CRenderer::DEPTH_BUFFER, m_RenderingOptions.ClearColor, 1.0f, 0);						
			
			SimplePass(camera);								
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::GlowPass(CCameraContainer& camera)
		{
			PERF_TIMER_RESET_START(_T("Scene"), _T("GlowPass"))

			CList<CContainer*>::CIterator Con;
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);	
			
			// render containers without sorting
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();

			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{
				(*Mat)->GetRenderableObjList().clear();
			}
			for (Con = m_NonSortingVisOpaqueContainerList.begin(); Con != m_NonSortingVisOpaqueContainerList.end(); ++Con)
			{
				(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
					(*Con)->vPreRender(&camera);
					(*Con)->vGlowRender(&camera);
					(*Con)->vPostRender(&camera);					
				}
			}

			PERF_TIMER_STOP(_T("Scene"), _T("GlowPass"))
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::GlowRender(CCameraContainer& camera)
		{
			LYNXCOLORRGBA ClearColor = { 0, 0, 0, 255};			

			/*
			if (m_lpCurrentLightingPassTarget)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER | GraphicsSystem::CRenderer::DEPTH_BUFFER, ClearColor, 1.0f, 0);
			}
			*/
			GlowPass(camera);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::OneLightingPass(CCameraContainer& camera)
		{	
			PERF_TIMER_RESET_START(_T("Scene"), _T("OneLightingPass"))

			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			CList<CGameObj*>::CIterator Lit;
			CSpotLight *lpSpotLit;
			CPointLight *lpPointLit;
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
			
			if (m_lpScene->GetlpAtmosphere() && m_lpScene->GetlpAtmosphere()->GetlpSky())
			{
				m_lpScene->GetlpAtmosphere()->GetlpSky()->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);																			
				m_lpScene->GetlpAtmosphere()->GetlpSky()->vRender(&camera);
			}
			
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentAmbientLight((CAmbientLight*)m_lpScene->GetCurrentAmbientLight());
			for (int i=0; i<m_lpScene->GetNumLights(); ++i)
			{	
				if (m_lpScene->GetlpLight(i)->GetLightType() != CLight::AMBIENT)
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(m_lpScene->GetlpLight(i));														
					break;
				}
			}
			
			for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
			{				
				(*Con)->AddToCurrentMaterialRenderList();
			}
            for (Con = m_NonSortingVisOpaqueContainerList.begin(); Con != m_NonSortingVisOpaqueContainerList.end(); ++Con)
			{				
				(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{					
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);
					(*Con)->vPreRender(&camera);					
					(*Con)->vRender(&camera);					
					(*Con)->vPostRender(&camera);					
				}
			}	
			
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}
			for (Con = m_VisTranslucentContainerList.begin(); Con != m_VisTranslucentContainerList.end(); ++Con)
			{				
				(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);																			
					(*Con)->vPreRender(&camera);
					(*Con)->vRender(&camera);
					(*Con)->vPostRender(&camera);
				}
			}	

			PERF_TIMER_STOP(_T("Scene"), _T("OneLightingPass"))
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::OneLightRender(CCameraContainer& camera)
		{		
			m_lpScene->ComputeVisList(camera);

			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);
            if (m_lpCurrentLightingPassTarget)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				camera.SetViewportRect();
			}
			else
            {
                //m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);
				camera.SetViewportRect();
            }
            
            if (m_RenderingOptions.bUseBackgroundTexture && m_RenderingOptions.BackgroundTexture)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::DEPTH_BUFFER, m_RenderingOptions.ClearColor, 1.0f, 0);				
				camera.SetViewportRect();
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)), m_BackgroundUVOffset);			
				m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, NULL);
                m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS()->GetlpShader()->vSetTexture("DiffuseMap", m_RenderingOptions.BackgroundTexture);
				// Using channel to set texture is not supported by OpenGL ES 2.0 renderer yet.
				//m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetPS()->GetlpShader()->vSetTexture(0, m_RenderingOptions.BackgroundTexture);
				m_lpBackgroundQuad->Render();				
			}				
			else if (m_RenderingOptions.ClearColor.Alpha != 0)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::DEPTH_BUFFER|GraphicsSystem::CRenderer::RENDER_BUFFER, m_RenderingOptions.ClearColor, 1.0f, 0);				
			}
			
			OneLightingPass(camera);								
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::Render(CCameraContainer& camera)
		{
			RESET_ALL_SCENE_COUNTERS

			PERF_TIMER_RESET_START(_T("Scene"), _T("Render"))

			switch (m_RenderingOptions.RenderMode)
			{
				case RENDER_SIMPLE:
					SimpleRender(camera);
					break;

				case RENDER_SIMPLELIGHTING:
					SimpleLightingRender(camera);
					break;

				case RENDER_DEPTHCOMPLEX:
					DepthComplexRender(camera);
					break;

				case RENDER_SSAOONLY:
					SSAORender(camera);
					break;

				case RENDER_ONELIGHT:
					OneLightRender(camera);
					break;

				default:
					camera.CalculateSplitPositions(GetShadowMapSplittedDepthArray(), m_RenderingOptions.NumSplittedShadowMap);
					NormalRender(camera);
					break;
			}
			PERF_TIMER_STOP(_T("Scene"), _T("Render"))			
		}
	}
}