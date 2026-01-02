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

#include <SceneSystem/LynxScnDeferredRenderer.h>
#include <Shader/LynxConstantColorPS.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredRenderer::CDeferredRenderer(CScene* s)
		: CRenderer(s)
		{				
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDeferredRenderer::~CDeferredRenderer(void)
		{
			for (int i=0; i<m_GBufferArray.size(); i++)
				LYNXDEL (m_GBufferArray[i]);		
			m_GBufferArray.clear();

			for (int i=0; i<m_DeferredLightingMaterialArray.size(); i++)
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->DeleteMaterial(m_DeferredLightingMaterialArray[i]);
			m_DeferredLightingMaterialArray.clear();

			m_lpEngine->ClearInvalidShaderClass();
			m_lpEngine->ClearInvalidResource();
			
			
			//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy ("))+GetName()+CString(_T(") ----------------------------------------------------------------------------------\n")));
			//Release();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDeferredRenderer::vCreate()
		{
			if (!CRenderer::vCreate())
				return LYNX_FALSE;

			GraphicsSystem::CGraphicsSystem* lpGfxSystem = m_lpScene->GetlpEngine()->GetlpGraphicsSystem();

			m_GBufferArray.resize(NUM_GBUFFERTYPE);

			if (!CDeferredRenderer::vResize(lpGfxSystem->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetSystemBackBuffer()->vGetHeight()))
				return LYNX_FALSE;

			if (m_GBufferArray[DIFFUSE])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[DIFFUSE])->Create(_T("Diffuse"), lpGfxSystem->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetSystemBackBuffer()->vGetHeight(), GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[DIFFUSE]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[NORMAL])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[NORMAL])->Create(_T("Normal"), lpGfxSystem->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetSystemBackBuffer()->vGetHeight(), lpGfxSystem->GetConfig().GBufferFormat, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[NORMAL]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[SPECULAR])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[SPECULAR])->Create(_T("Specular"), lpGfxSystem->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetSystemBackBuffer()->vGetHeight(), GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[SPECULAR]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[GLOW])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[GLOW])->Create(_T("Glow"), lpGfxSystem->GetSystemBackBuffer()->vGetWidth(), lpGfxSystem->GetSystemBackBuffer()->vGetHeight(), GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, 0))								
				//if (!((CGBufferMap*)m_GBufferArray[GLOW])->Create(_T("Glow"), cx, cy, CRenderer::FORMAT_A16B16G16R16F, 2, 0))								
					return LYNX_FALSE;
				m_GBufferArray[GLOW]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			m_DeferredLightingMaterialArray.resize(NUM_MATERIALTYPE);			

			GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC Desc;			
			GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
			GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
			GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

			Desc.Default();			
			BDesc.Default();
			RDesc.Default();
			SamDesc.Default();

			MaterialSystem::CTechnique* Tech;

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_DeferredLightingMaterialArray[AMBIENT_LIGHTING], MaterialSystem::CMaterial::POSTPROCESSING);
			m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->SetResourceFlag(SYSTEM_DEFINED);
			m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->SetName(_T("DEFERRED AMBIENT LIGHTING"));
			m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
			m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->m_SpPower = 0;
			m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_DeferredLightingMaterialArray[AMBIENT_LIGHTING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DEFERRED AMBIENT LIGHTING.Tech0"));
			Tech->GetPass(0).SetName(_T("DEFERRED AMBIENT LIGHTING.Tech0.P0"));				
			Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "DeferredAmbientLightingPS");
			Desc.StencilEnable	= LYNX_FALSE;		
			Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
			Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
			Tech->GetPass(0).CreateDepthStencilState(Desc);
			BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_DEST_COLOR;
			BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);								
			RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;				

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING], MaterialSystem::CMaterial::POSTPROCESSING);
			m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->SetResourceFlag(SYSTEM_DEFINED);
			m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->SetName(_T("DEFERRED DIRECTIONAL LIGHTING"));
			m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
			m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->m_SpPower = 0;
			m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_DeferredLightingMaterialArray[DIRECTIONAL_LIGHTING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DEFERRED DIRECTIONAL LIGHTING.Tech0"));
			Tech->GetPass(0).SetName(_T("DEFERRED DIRECTIONAL LIGHTING.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "DeferredDirectionalLightingPS");					
			Desc.StencilEnable	= LYNX_FALSE;		
			Desc.DepthMode		= GraphicsSystem::CRenderer::DB_NONE;
			Desc.DepthFunc		= GraphicsSystem::CRenderer::TF_ALWAYS;			
			Tech->GetPass(0).CreateDepthStencilState(Desc);				
			BDesc.bIndependentBlend = LYNX_FALSE;
			BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);				
			RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;				
							
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_DeferredLightingMaterialArray[SPOT_LIGHTING], MaterialSystem::CMaterial::POSTPROCESSING);
			m_DeferredLightingMaterialArray[SPOT_LIGHTING]->SetResourceFlag(SYSTEM_DEFINED);
			m_DeferredLightingMaterialArray[SPOT_LIGHTING]->SetName(_T("DEFERRED SPOT LIGHTING"));
			m_DeferredLightingMaterialArray[SPOT_LIGHTING]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
			m_DeferredLightingMaterialArray[SPOT_LIGHTING]->m_SpPower = 0;
			m_DeferredLightingMaterialArray[SPOT_LIGHTING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_DeferredLightingMaterialArray[SPOT_LIGHTING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DEFERRED SPOT LIGHTING.Tech0"));
			Tech->GetPass(0).SetName(_T("DEFERRED SPOT LIGHTING.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "DeferredSpotLightingPS");									
			Desc.DepthMode				= GraphicsSystem::CRenderer::DB_NONE;
			Desc.DepthFunc				= GraphicsSystem::CRenderer::TF_ALWAYS;			
			Desc.StencilEnable			= LYNX_TRUE;
			Desc.StencilRef				= 0x0;
			Desc.StencilWriteMask		= 0xf;
			Desc.StencilReadMask		= 0xf;
			Desc.StencilFunc			= GraphicsSystem::CRenderer::TF_LESS;
			Desc.StencilFailOp			= GraphicsSystem::CRenderer::SO_KEEP;
			Desc.StencilDepthFailOp		= GraphicsSystem::CRenderer::SO_KEEP;
			Desc.StencilPassOp			= GraphicsSystem::CRenderer::SO_KEEP;				
			Tech->GetPass(0).CreateDepthStencilState(Desc);				
			BDesc.bIndependentBlend = LYNX_FALSE;
			BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);				
			RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_DeferredLightingMaterialArray[POINT_LIGHTING], MaterialSystem::CMaterial::POSTPROCESSING);
			m_DeferredLightingMaterialArray[POINT_LIGHTING]->SetResourceFlag(SYSTEM_DEFINED);
			m_DeferredLightingMaterialArray[POINT_LIGHTING]->SetName(_T("DEFERRED POINT LIGHTING"));
			m_DeferredLightingMaterialArray[POINT_LIGHTING]->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));
			m_DeferredLightingMaterialArray[POINT_LIGHTING]->m_SpPower = 0;
			m_DeferredLightingMaterialArray[POINT_LIGHTING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_DeferredLightingMaterialArray[POINT_LIGHTING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DEFERRED POINT LIGHTING.Tech0"));
			Tech->GetPass(0).SetName(_T("DEFERRED POINT LIGHTING.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenDepthQuadVS", "DeferredPointLightingPS");					
			Desc.DepthMode				= GraphicsSystem::CRenderer::DB_NONE;
			Desc.DepthFunc				= GraphicsSystem::CRenderer::TF_ALWAYS;							
			Desc.StencilEnable			= LYNX_TRUE;
			Desc.StencilRef				= 0x0;
			Desc.StencilWriteMask		= 0xf;
			Desc.StencilReadMask		= 0xf;
			Desc.StencilFunc			= GraphicsSystem::CRenderer::TF_LESS;
			Desc.StencilFailOp			= GraphicsSystem::CRenderer::SO_KEEP;
			Desc.StencilDepthFailOp		= GraphicsSystem::CRenderer::SO_KEEP;
			Desc.StencilPassOp			= GraphicsSystem::CRenderer::SO_KEEP;				
			Tech->GetPass(0).CreateDepthStencilState(Desc);				
			BDesc.bIndependentBlend = LYNX_FALSE;
			BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);				
			RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;					

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDeferredRenderer::vResize(int x, int y)
		{
			if (!CRenderer::vResize(x, y))
				return LYNX_FALSE;

			GraphicsSystem::CGraphicsSystem* lpGfxSystem = m_lpScene->GetlpEngine()->GetlpGraphicsSystem();
			
			if (m_GBufferArray[DIFFUSE])
				LYNXDEL(m_GBufferArray[DIFFUSE]);
			m_GBufferArray[DIFFUSE] = LYNXNEW GraphicsSystem::CGBufferMap(lpGfxSystem);

			if (m_GBufferArray[SPECULAR])
				LYNXDEL(m_GBufferArray[SPECULAR]);
			m_GBufferArray[SPECULAR] = LYNXNEW GraphicsSystem::CGBufferMap(lpGfxSystem);
			
			if (m_GBufferArray[NORMAL])
				LYNXDEL(m_GBufferArray[NORMAL]);
			m_GBufferArray[NORMAL] = LYNXNEW GraphicsSystem::CGBufferMap(lpGfxSystem);
			
			if (m_GBufferArray[GLOW])
				LYNXDEL(m_GBufferArray[GLOW]);
			m_GBufferArray[GLOW] = LYNXNEW GraphicsSystem::CGBufferMap(lpGfxSystem);

			m_lpEngine->ClearInvalidResource();

			if (m_GBufferArray[DIFFUSE])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[DIFFUSE])->Create(_T("Deferred.Diffuse"), x, y, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[DIFFUSE]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[NORMAL])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[NORMAL])->Create(_T("Deferred.Normal"), x, y, lpGfxSystem->GetConfig().GBufferFormat, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[NORMAL]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[SPECULAR])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[SPECULAR])->Create(_T("Deferred.Specular"), x, y, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 0, 0))								
					return LYNX_FALSE;
				m_GBufferArray[SPECULAR]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			if (m_GBufferArray[GLOW])
			{
				//Allocate G-Buffers()
				if (!((GraphicsSystem::CGBufferMap*)m_GBufferArray[GLOW])->Create(_T("Deferred.Glow"), x, y, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, 0))								
				//if (!((CGBufferMap*)m_GBufferArray[GLOW])->Create(_T("Glow"), cx, cy, CRenderer::FORMAT_A16B16G16R16F, 2, 0))								
					return LYNX_FALSE;
				m_GBufferArray[GLOW]->SetlpDepthBuffer(lpGfxSystem->GetSystemDepthBuffer());
			}

			// shaders need to re-bind resources because of viewport resize
			for (int i=0; i<m_DeferredLightingMaterialArray.size(); i++)
			{
				m_DeferredLightingMaterialArray[i]->RebindResources();				
				m_DeferredLightingMaterialArray[i]->SetDirty(LYNX_TRUE);
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredRenderer::GPass(CCameraContainer& camera)
		{
			CList< MaterialSystem::CMaterialPtr >::CIterator Mat;
			CList< MaterialSystem::CMaterialPtr >& MatList = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList();			
			CList<CContainer*>::CIterator Con;							
			static Math::CColor Color(0.0f, 0.0f, 0.0f, 1.0f);

			//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::GREEN_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL|GraphicsSystem::CRenderer::ALPHA_CHANNEL);
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_GBufferArray[DIFFUSE]);								
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 1, m_GBufferArray[SPECULAR]);								
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 2, m_GBufferArray[NORMAL]);						
			if (m_RenderingOptions.bGlowPass)
			{
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 3, m_GBufferArray[GLOW]);								
			}
			else
			{
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			}
			m_lpEngine->GetlpGraphicsSystem()->SetSystemDepthBufferAsDepthBuffer();
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetlpMap(GraphicsSystem::OFFSCREEN)->GetTexture()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetlpMap(GraphicsSystem::OFFSCREEN)->GetTexture()->vGetHeight());

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, Color, 1.0f, 0);

			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				(*Mat)->GetRenderableObjList().clear();				
			}

			for (Con = m_VisOpaqueContainerList.begin(); Con != m_VisOpaqueContainerList.end(); ++Con)
			{				
				if (!(*Con)->IsOccluded())
					(*Con)->AddToCurrentMaterialRenderList();
			}
			for (Mat = MatList.begin(); Mat != MatList.end(); ++Mat)
			{				
				CList<CContainer*>& ConList = (*Mat)->GetRenderableObjList();
				for (Con = ConList.begin(); Con != ConList.end(); ++Con)
				{
					(*Con)->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::GPASS);																			
					(*Con)->vRender(&camera);					
				}
			}					

			if (m_RenderingOptions.bGlowPass)
			{
				m_GBufferArray[GLOW]->DownSampled();
				m_GBufferArray[GLOW]->GaussinBlur(5.0f, 1.0f);			
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//  Description : Render scene objects to <CurrentLightingPassTarget>, <CurrentGlowMap>		
		//-------------------------------------------------------------------------------------------------------
		void CDeferredRenderer::OpaqueLightingPass(CCameraContainer& camera)
		{	
			if (m_lpScene->GetCurrentAmbientLight())
			{	
				m_lpScene->GetCurrentAmbientLight()->vDeferredLighting(&camera);
			}			
			
			// Glow map pass
			if (m_RenderingOptions.bGlowPass)
			{
				static float OutputColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};				
				((Shader::CConstantColorPS*)&(*(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ACC_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())))->SetConstantColor(OutputColor);
				m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ACC_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
				m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ACC_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);
				//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedPixelShader()->vSetConstantF("gOutputColor", OutputColor, 4);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexture(0, m_GBufferArray[GLOW]->GetCurrent());			
				m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();
			}
			
			CList<CLight*>::CIterator Lit;
			for (Lit = m_lpScene->GetVisLightList().begin(); Lit != m_lpScene->GetVisLightList().end(); ++Lit)			
			{
				if ((*Lit)->GetLightType() != CLight::AMBIENT)
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight((*Lit));
					(*Lit)->vComputeVisObjList();
					ShadowmapPass((*Lit), camera);
					(*Lit)->MaskLightedArea(&camera);					
					(*Lit)->vDeferredLighting(&camera);					
					INC_COUNTER(m_Profile.NumRenderedLight, 1);
				}				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDeferredRenderer::TranslucentLightingPass(CCameraContainer& camera)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  Description : Render scene objects to <CurrentLightingPassTarget>, <m_lpCurrentGlowMap>		
		//	Bound texture : <Offscreen>, <DepthMap>, <Normal>, <Specular>
		//-------------------------------------------------------------------------------------------------------
		void CDeferredRenderer::vLightingPass(CCameraContainer& camera)
		{		
			static Math::CColor Color(1.0f, 1.0f, 1.0f, 1.0f);
			static Math::CColor BlackColor(0.0f, 0.0f, 0.0f, 0.0f);
						
			GPass(camera);			

			if (m_RenderingOptions.bSSAOPass)
			{
				SSAOPass(camera);
				//SSAO_SSGI_Pass(camera);
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			}
			else
			{
				m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);	
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, Color, 1.0f, 0);
			}
			
			OpaqueLightingPass(camera);

			if (m_lpScene->GetlpAtmosphere() && m_lpScene->GetlpAtmosphere()->GetlpSky())
			{
				m_lpEngine->GetlpGraphicsSystem()->SetSystemDepthBufferAsDepthBuffer();
				m_lpScene->GetlpAtmosphere()->GetlpSky()->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);																			
				m_lpScene->GetlpAtmosphere()->GetlpSky()->vRender(&camera);
			}

			
		#if 0
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			TranslucentLightingPass(camera);
		#else	
			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentTranslucentLightingPassTarget);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentTranslucentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentTranslucentLightingPassTarget->GetTexture()->vGetHeight());
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER, BlackColor, 1.0f, 0);
			//DeferredTranslucentLightingPass(camera);		
			CRenderer::TranslucentLightingPass(camera);

			m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(NULL, 0, m_lpCurrentLightingPassTarget);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);
			m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpCurrentLightingPassTarget->GetTexture()->vGetWidth(), m_lpCurrentLightingPassTarget->GetTexture()->vGetHeight());
			m_lpEngine->GetlpGraphicsSystem()->SetupScreenSpaceQuadUVOffset(&(m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ALPHA_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0)));			
			m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::ALPHA_BLT)->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).Set(NULL, &camera);			
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexture(0, m_lpCurrentTranslucentLightingPassTarget->GetTexture());									
			m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();
		#endif		
			
			//m_lpEngine->GetlpGraphicsSystem()->BltTexture(m_lpCurrentLightingPassTarget->GetTexture(), m_GBufferArray[NORMAL]->GetTexture());
		}
	}
}