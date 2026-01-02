
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
#include <LynxLogger.h>
#include <LynxIniReader.h>
#include <LynxScreenOutputDevice.h>
#ifdef __USED_RENDERER_ONLY__
	#if defined (__D3D9__)
		#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
	#elif  defined(__D3D11__)
		#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
	#elif  defined(__OGLES__)
		#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
	#elif  defined(__OGLES2__)
		#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
	#endif
#else
	#ifdef __WIN32__
		#ifdef __WINAPP__
			#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>		
		#else
			#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
			#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>		
			#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>	
		#endif
	#endif
	#if (defined(__iOS__) || defined(__ANDROID__))
		#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
		#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>	
	#endif	
#endif

#include <GraphicsSystem/NullRenderer/LynxNullRenderer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <GraphicsSystem/LynxVertexShader.h>
#include <GraphicsSystem/LynxPixelShader.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxGlowMap.h>
#include <GraphicsSystem/LynxToneMap.h>
#include <GraphicsSystem/LynxLuminanceMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>
#include <GraphicsSystem/LynxVelocityMap.h>
#include <GraphicsSystem/LynxGBufferMap.h>
#include <GraphicsSystem/LynxScreenQuad.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxScreenQuadUVOffsetVS.h>
#include <Shader/LynxScreenQuadBlurTexCoordVS.h>
#include <Shader/LynxScreenDepthQuadVS.h>
#include <Shader/LynxGaussinBlurPS.h>
#include <Shader/LynxBoxBlurWithEdgeCheckPS.h>
#include <Shader/LynxBilateralBlurPS.h>
#include <Shader/LynxRadialBlurPS.h>
#include <Shader/LynxHaloRadialBlurPS.h>
#include <Shader/LynxDirectionalBlurPS.h>
#include <Shader/LynxVelocityMapBlurPS.h>
#include <Shader/LynxPostScanLineNoisePS.h>
#include <Shader/LynxGrayBltPS.h>
#include <Shader/LynxTintPS.h>

#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif


namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::VERTEXTYPE e)
	{
		CString				VertexType;

		switch (e)
		{
			case GraphicsSystem::VT_FIXED:
				VertexType = _T("VT_FIXED");
				break;
			case GraphicsSystem::VT_VECTOR3:
				VertexType = _T("VT_VECTOR3");
				break;
			case GraphicsSystem::VT_SCREEN_QUAD:
				VertexType = _T("VT_SCREEN_QUAD");
				break;
			case GraphicsSystem::VT_SPRITE:
				VertexType = _T("VT_SPRITE");
				break;
			case GraphicsSystem::VT_SPRITE3D:
				VertexType = _T("VT_SPRITE3D");
				break;
			case GraphicsSystem::VT_SPRITE3D_CLUSTER:
				VertexType = _T("VT_SPRITE3D_CLUSTER");
				break;
			case GraphicsSystem::VT_VOXEL_CLUSTER:
				VertexType = _T("VT_VOXEL_CLUSTER");
				break;
			case GraphicsSystem::VT_STATIC:
				VertexType = _T("VT_STATIC");
				break;
			case GraphicsSystem::VT_RIGID:
				VertexType = _T("VT_RIGID");
				break;
			case GraphicsSystem::VT_BLEND:
				VertexType = _T("VT_BLEND");
				break;
			case GraphicsSystem::VT_FONT:
				VertexType = _T("VT_FONT");
				break;			
			case GraphicsSystem::VT_PROXY:
				VertexType = _T("VT_PROXY");
				break;
		}
		return VertexType;
	}
	namespace GraphicsSystem 
	{						
		IMPLEMENT_CLASSSCRIPT(CGraphicsSystem, CSystem)

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::InitMembers(void)
		{
			m_lpMaterialSystem = NULL;			

			m_SystemTextures.resize(NUM_SYSTEMTEXTURETYPE);
			m_VertexLayouts.resize(NUM_VERTEXTYPE);
			m_PostProcessMaterails.resize(NUM_POSTPROCESSTYPE);				
			for (int i=0; i<NUM_MAPTYPE; i++)
			{
				m_lpMaps[i] = NULL;
			}
			m_bNeedToRecompileShader = LYNX_TRUE;
			m_ConstantColor.PackedColor = 0xffffffff;
			m_bOverwriteMaterialColor = LYNX_FALSE;
            m_ViewportRect.x1 = 0;
            m_ViewportRect.y1 = 0;
            m_ViewportRect.x2 = 0;
            m_ViewportRect.y2 = 0;

			m_Config.GraphicParam.Width = 640;
			m_Config.GraphicParam.Height = 480;
			m_Config.GraphicParam.FrameBufferFormat = CRenderer::FORMAT_A16B16G16R16F;
			m_Config.GraphicParam.DepthBufferFormat = CRenderer::FORMAT_D24X8;
			m_Config.GraphicParam.bHardware = TRUE;
			m_Config.GraphicParam.bFullScreen = LYNX_FALSE;
			m_Config.GraphicParam.NumMultiSamples = 1;
			m_Config.GraphicParam.ShaderDebuggingFlag = CRenderer::SHADER_DEBUG_NONE;//CRenderer::SHADER_DEBUG_NONE;// SHADER_DEBUG_VS | SHADER_DEBUG_PS;	
			m_Config.GraphicParam.RenderDeviceType = CRenderer::RDT_DEFAULT;
			m_Config.bCreateVoxelSpriteSource = LYNX_FALSE;
			m_Config.bOverwriteVoxelSpriteRender = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGraphicsSystem::CGraphicsSystem(CEngine* const lpengine, CRenderer* ren)
		: CSystem(lpengine),
		  m_lpScreenOutputDevice(NULL),
		  m_lpMaterialSystem(NULL),
		  m_lpRenderer(NULL),
		  m_lpScreenQuad(NULL)
		{				
			m_lpRenderer = ren;
			InitMembers();

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGraphicsSystem::~CGraphicsSystem(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			
			CVoxelClusterContainer::ReleaseSharedStaticResource();
			LYNXDEL(m_lpScreenOutputDevice);
			LYNXDEL m_lpScreenQuad;

			if (CVoxelClusterContainer::m_SharedVertexArray)
				CVoxelClusterContainer::m_SharedVertexArray.Reset();
						
			if (CVoxelClusterContainer::m_SharedIndexArray)
				CVoxelClusterContainer::m_SharedIndexArray.Reset();

			if (CVoxelClusterContainer::m_SpriteVertexLayout)
				CVoxelClusterContainer::m_SpriteVertexLayout.Reset();

			if (CVoxelClusterContainer::m_SharedSpriteVertexArray)
				CVoxelClusterContainer::m_SharedSpriteVertexArray.Reset();

			if (CVoxelClusterContainer::m_SharedSpriteIndexArray)
				CVoxelClusterContainer::m_SharedSpriteIndexArray.Reset();


			for (int i=0; i<NUM_MAPTYPE; i++)
			{
				if (m_lpMaps[i])
					LYNXDEL m_lpMaps[i];
			}
			m_lpEngine->ClearInvalidResource();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Release system textures----------------------------------------------------------------------------------------------\n")));						
			m_SystemTextures.clear();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Release postprocess materials----------------------------------------------------------------------------------------------\n")));						
			m_PostProcessMaterails.clear();
			// Explicitly release material before the destruction of renderer, otherwise, the CMaterialPtr will be destroyed after
			// renderer has been destroyed and cause crash.
			m_ConsoleMaterail.Reset();			

			LYNXDEL m_lpMaterialSystem;
			m_lpMaterialSystem = NULL;						
			m_lpEngine->ClearInvalidResource();
			m_lpEngine->ClearInvalidShaderClass();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Release vertex layout----------------------------------------------------------------------------------------------\n")));	
			m_VertexLayouts.clear();
			m_lpEngine->ClearInvalidVertexLayout();

			m_lpRenderer->Close();
			m_lpEngine->ClearInvalidResource();
			m_lpEngine->ClearInvalidVertexLayout();
			m_lpEngine->ClearInvalidShaderClass();

			LYNXDEL m_lpRenderer;
			m_lpEngine->ClearInvalidResource();			
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::PushClipPlaneStates()
		{
			for (int i=0; i<6; i++)
			{
				GetClipPlane(i, &m_ClipPlanes[i]);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::PopClipPlaneStates()
		{
			for (int i=0; i<6; i++)
			{
				SetClipPlane(i, &m_ClipPlanes[i]);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::ConvertScreenSpaceToScissorSpace(LYNXRECT* r, float x1, float y1, float x2, float y2)
		{
			r->x1 = LYNX_MAX((x1+1.0)*  0.5f * m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth(), 0);
			r->y1 = LYNX_MAX((y2-1.0)* -0.5f * m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight(), 0);
			r->x2 = LYNX_MIN((x2+1.0)*  0.5f * m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth(), m_lpRenderer->GetSystemBackBuffer()->vGetWidth());
			r->y2 = LYNX_MIN((y1-1.0)* -0.5f * m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight(), m_lpRenderer->GetSystemBackBuffer()->vGetHeight());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::vLoadGraphicParam(const LYNXCHAR* filename)
		{
			CIniReader IniReader(filename);

			m_Config.GraphicParam.Width = 640;
			m_Config.GraphicParam.Height = 480;
			m_Config.GraphicParam.FrameBufferFormat = CRenderer::FORMAT_A16B16G16R16F;
			m_Config.GraphicParam.DepthBufferFormat = CRenderer::FORMAT_D24X8;
			m_Config.GraphicParam.bHardware = TRUE;
			m_Config.GraphicParam.bFullScreen = LYNX_FALSE;
			m_Config.GraphicParam.NumMultiSamples = 1;
			m_Config.GraphicParam.ShaderDebuggingFlag = CRenderer::SHADER_DEBUG_NONE;//CRenderer::SHADER_DEBUG_NONE;// SHADER_DEBUG_VS | SHADER_DEBUG_PS;	
			m_Config.GraphicParam.RenderDeviceType = CRenderer::RDT_DEFAULT;
			m_Config.bCreateVoxelSpriteSource = LYNX_FALSE;
			m_Config.bOverwriteVoxelSpriteRender = LYNX_FALSE;
			
			CString Resolution = IniReader.ReadString(_T("Graphics"), _T("Resolution"), _T("640x480"));
			LYNXCHAR Res[128];
			lynxStrCpy(Res, Resolution.c_str());
			LYNXCHAR* Tok = lynxStrTok(Res, _T("x"));
			if (Tok)
				m_Config.GraphicParam.Width = lynxAtoi(Tok);
			Tok = lynxStrTok(NULL, _T("x"));
			if (Tok)
				m_Config.GraphicParam.Height = lynxAtoi(Tok);
		
			CString Format = IniReader.ReadString(_T("Graphics"), _T("BackBufferFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_X8R8G8B8).c_str());
			m_Config.GraphicParam.FrameBufferFormat = LynxEngine::GetFormat(Format);
			
			CString DepthFormat = IniReader.ReadString(_T("Graphics"), _T("DepthBufferFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_D24X8).c_str());
			m_Config.GraphicParam.DepthBufferFormat = LynxEngine::GetFormat(DepthFormat);
			
			m_Config.GraphicParam.bFullScreen = IniReader.ReadBool(_T("Graphics"), _T("Fullscreen"), LYNX_FALSE);			
			m_Config.GraphicParam.bWaitVSync = IniReader.ReadBool(_T("Graphics"), _T("WaitVSync"), LYNX_FALSE);	
			m_Config.GraphicParam.NumMultiSamples = IniReader.ReadInt(_T("Graphics"), _T("MultiSample"), 0);				
			if (m_Config.GraphicParam.NumMultiSamples <= 1)
				m_Config.GraphicParam.NumMultiSamples = 1;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::vLoadConfig(const LYNXCHAR* filename)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load config file...\r\n")));

			CIniReader IniReader(filename);

			m_Config.bGammaCorrection = IniReader.ReadBool(_T("Graphics"), _T("GammaCorrection"), LYNX_TRUE);

			m_lpRenderer->UseCachedState(IniReader.ReadBool(_T("Graphics"), _T("UseCachedState"), LYNX_TRUE));
            m_lpRenderer->SharedShaderGroup(IniReader.ReadBool(_T("Graphics"), _T("SharedShaderGroup"), LYNX_TRUE));
			m_lpRenderer->SharedShaderClass(IniReader.ReadBool(_T("Graphics"), _T("SharedShaderClass"), LYNX_TRUE));
			m_lpRenderer->CacheShaderGroup(IniReader.ReadBool(_T("Graphics"), _T("CacheShaderGroup"), LYNX_TRUE));			

			CRenderer::FORMAT SSAOFormats[] = { CRenderer::FORMAT_L8, 
												CRenderer::FORMAT_L16, 
												CRenderer::FORMAT_R16F, 
												CRenderer::FORMAT_X8R8G8B8 };

			m_Config.ShadowMapFormat = CRenderer::FORMAT_R32F;
			m_Config.CubeShadowMapFormat = CRenderer::FORMAT_R16F;			
			m_Config.ShadowMapDepthFormat = CRenderer::FORMAT_D24X8;
			
			m_Config.DepthMapFormat = CRenderer::FORMAT_R32F;			
			m_Config.GBufferFormat = CRenderer::FORMAT_A2R10G10B10;
			for (int i= 0; i<LYNX_ARRAY_SIZE(SSAOFormats); i++)
			{
				m_Config.SSAOMapFormat = SSAOFormats[i];
				if (m_lpRenderer->CheckRTFormat(m_Config.SSAOMapFormat))
					break;
			}
			m_Config.RNRMapSize = 512;
			m_Config.ShadowMapSize = 512;
			m_Config.PostprocessQuality = 1;
			
			m_Config.bCompileShaderOnTheFly = IniReader.ReadBool(_T("Graphics"), _T("CompileShaderOnTheFly"), LYNX_TRUE);
			m_Config.bAlwaysRecompileShader = IniReader.ReadBool(_T("Graphics"), _T("AlwaysRecompileShader"), LYNX_FALSE);

		#ifdef __WINAPP__
			CString ShaderTarget = IniReader.ReadString(_T("Graphics"), _T("ShaderTarget"), LynxEngine::GetEnumStr(CRenderer::SHADER_TARGET_40).c_str());
		#else
			CString ShaderTarget = IniReader.ReadString(_T("Graphics"), _T("ShaderTarget"), LynxEngine::GetEnumStr(CRenderer::SHADER_TARGET_30).c_str());
		#endif
			for (int i=CRenderer::SHADER_TARGET_11; i<CRenderer::NUM_SHADERTARGET; i++)
			{
				if (LynxEngine::GetEnumStr((CRenderer::SHADERTARGET)i) == ShaderTarget)
					m_Config.ShaderTarget = (CRenderer::SHADERTARGET)i;
			}
			m_Config.PostprocessQuality = LYNX_CLAMP(lynxAtoi(IniReader.ReadString(_T("Graphics"), _T("PostprocessQuality"), CString("3"))), 1, 3);
			m_Config.SSAOQuality		= LYNX_CLAMP(lynxAtoi(IniReader.ReadString(_T("Graphics"), _T("SSAOQuality"), CString("3"))), 1, 3);			
			m_Config.bDeferredShading = IniReader.ReadBool(_T("Graphics"), _T("DeferredShading"), LYNX_FALSE);	

			CString Format;

			Format = IniReader.ReadString(_T("Graphics"), _T("DepthMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R16F).c_str());
			m_Config.DepthMapFormat = LynxEngine::GetFormat(Format);

			m_Config.bHDRRendering = IniReader.ReadBool(_T("Graphics"), _T("HDRRendering"), LYNX_TRUE);	
			if (m_Config.bHDRRendering)
			{
				Format = IniReader.ReadString(_T("Graphics"), _T("HDRFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_A16B16G16R16F).c_str());
				m_Config.LightingFormat = LynxEngine::GetFormat(Format);

				Format = IniReader.ReadString(_T("Graphics"), _T("ToneMappingFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R16F).c_str());
				m_Config.ToneMappingFormat = LynxEngine::GetFormat(Format);
			}
			else
			{
				m_Config.LightingFormat = CRenderer::FORMAT_X8R8G8B8;
			}

			m_Config.bSharedShadowMap = IniReader.ReadBool(_T("Graphics"), _T("SharedShadowMap"), LYNX_FALSE);	
			m_Config.ShadowMapSize = IniReader.ReadInt(_T("Graphics"), _T("ShadowMapSize"), 512);
			m_Config.CubeShadowMapSize = IniReader.ReadInt(_T("Graphics"), _T("CubeShadowMapSize"), 256);
			m_Config.MaxSplittedShadowMap = IniReader.ReadInt(_T("Graphics"), _T("MaxSplittedShadowMap"), 4);

			Format = IniReader.ReadString(_T("Graphics"), _T("ShadowMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R32F).c_str());
			m_Config.ShadowMapFormat = LynxEngine::GetFormat(Format);

			Format = IniReader.ReadString(_T("Graphics"), _T("CubeShadowMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R32F).c_str());
			m_Config.CubeShadowMapFormat = LynxEngine::GetFormat(Format);			
			
			if (!m_lpRenderer->CheckRTFormat(m_Config.GBufferFormat))
				m_Config.GBufferFormat = CRenderer::FORMAT_X8R8G8B8;						
			
			Format = IniReader.ReadString(_T("Graphics"), _T("ShadowMapDepthFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_D24X8).c_str());
			m_Config.ShadowMapDepthFormat = LynxEngine::GetFormat(Format);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DefaultSetting(void)
		{			
			SetSystemBackBufferAsRenderBuffer(0);
			SetSystemDepthBufferAsDepthBuffer();			
			//m_lpRenderer->SetViewportRect(0, 0, 1280, 720);

			m_lpRenderer->Clipping(LYNX_FALSE);
            m_lpRenderer->ResetTextureSearchPathList();
            m_lpRenderer->AddTextureSearchPath(_T("../texture/"));
            m_lpRenderer->AddTextureSearchPath(_T(""));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGraphicsSystem::vInit(LYNXWINDOW wnd, CRenderer::INITGRAPHICPARAM* lpcp)
		{	
			LYNX_GUARD(CGraphicsSystem::vInit)

			lynxMemCpy(&m_Config.GraphicParam, lpcp, sizeof(CRenderer::INITGRAPHICPARAM));

			if (!CSystem::vInit())
				return LYNX_TRUE;

			m_lpRenderer->Open(wnd, lpcp);
            m_ViewportRect.x1 = 0;
            m_ViewportRect.y1 = 0;
            m_ViewportRect.x2 = lpcp->Width;
            m_ViewportRect.y2 = lpcp->Height;
			DefaultSetting();			
			vLoadConfig(m_lpEngine->GetConfigFilename());			
			vInitDefalutResources();

			if (m_lpEngine->GetlpGUISystem())
			{
				m_lpEngine->GetlpGUISystem()->ComputeLayoutFactor(LYNX_TRUE);
				LYNXRECT ClipRect;
				ClipRect.x1 = 0; ClipRect.y1 = 0; ClipRect.x2 = lpcp->Width; ClipRect.y2 = lpcp->Height;
				m_lpEngine->GetlpGUISystem()->SetClipRect(ClipRect);
			}

			if (!m_lpMaterialSystem->vInit())
				return LYNX_FALSE;			

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGraphicsSystem::vInit(LYNXWINDOW wnd, const int w, const int h, const LYNXBOOL bfull)
		{	
			LYNX_GUARD(CGraphicsSystem::vInit)

			vLoadGraphicParam(m_lpEngine->GetConfigFilename());				

			if (w > 0 && h > 0)
			{
				m_Config.GraphicParam.Width = w;
				m_Config.GraphicParam.Height = h;
			}
			m_Config.GraphicParam.bFullScreen = bfull;

			if (!CSystem::vInit())
				return LYNX_TRUE;

			m_lpRenderer->Open(wnd, &m_Config.GraphicParam);		
            m_ViewportRect.x1 = 0;
            m_ViewportRect.y1 = 0;
            m_ViewportRect.x2 = m_Config.GraphicParam.Width;
            m_ViewportRect.y2 = m_Config.GraphicParam.Height;
			DefaultSetting();	
			vLoadConfig(m_lpEngine->GetConfigFilename());
			vInitDefalutResources();

			if (m_lpEngine->GetlpGUISystem())
			{
				m_lpEngine->GetlpGUISystem()->ComputeLayoutFactor(LYNX_TRUE);
				LYNXRECT ClipRect;
				ClipRect.x1 = 0; ClipRect.y1 = 0; ClipRect.x2 = m_Config.GraphicParam.Width; ClipRect.y2 = m_Config.GraphicParam.Height;
				m_lpEngine->GetlpGUISystem()->SetClipRect(ClipRect);
			}

			if (!m_lpMaterialSystem->vInit())
				return LYNX_FALSE;			

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CGraphicsSystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "Print",				LynxEngine::GraphicsSystem::CGraphicsSystem, void,	Print,					(int, int, LYNXCHAR const *))		
			REGISTER_SCRIPT_METHOD(lps, "PrintWithColor",		LynxEngine::GraphicsSystem::CGraphicsSystem, void,	PrintWithColor,			(int, int, LYNXCHAR const *, const LYNXCOLORRGBA&))		
			REGISTER_SCRIPT_METHOD(lps, "GetBackbufferWidth",	LynxEngine::GraphicsSystem::CGraphicsSystem, int,	GetBackbufferWidth,		())		
			REGISTER_SCRIPT_METHOD(lps, "GetBackbufferHeight",	LynxEngine::GraphicsSystem::CGraphicsSystem, int,	GetBackbufferHeight,	())		
			REGISTER_SCRIPT_METHOD(lps, "ClearRenderBuffer",	LynxEngine::GraphicsSystem::CGraphicsSystem, void,	ClearRenderBuffer,		(int, LYNXCOLORRGBA&, LYNXREAL, int))		
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGraphicsSystem::vInitDefalutResources(void)
		{	
			LYNX_GUARD(CGraphicsSystem::InitDefalutResources)
			
			CRenderer::DEPTHSTENCILSTATEDESC Desc;			
			CRenderer::BLENDSTATEDESC		 BDesc;
			CRenderer::RASTERIZERSTATEDESC	 RDesc;
			CRenderer::SAMPLERSTATEDESC		 SamDesc;

			Desc.Default();			
			BDesc.Default();
			RDesc.Default();
			SamDesc.Default();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Graphics Subsystem...\r\n")));

			m_lpRenderer->SetShaderTarget(m_Config.ShaderTarget);			
			if (m_Config.bCompileShaderOnTheFly)
			{
				m_lpRenderer->ComputeCachedCompiledShaderPath();
				m_bNeedToRecompileShader = (m_lpRenderer->NeedToRecompileShader() | m_Config.bAlwaysRecompileShader);				
			}									
			
            m_lpRenderer->SetTexturePath(_T("../texture/"));
            
			CRenderer::TEXTUREDESC TexDesc;
			TexDesc.Default();
			TexDesc.bSRGB = LYNX_FALSE;
			TexDesc.MipMapLevel = 1;			
			TexDesc.bDelayLoading = LYNX_TRUE;
			m_lpRenderer->LoadTexture(m_SystemTextures[LOGO_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("logo.png"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[DISTORTIONCHECKER_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("DistortionChecker.dds"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[WHITE_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("white.png"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[BLACK_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("black.png"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[DEPTHCOMPLEX_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("depthcomplex.png"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[DOT3_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("dot3.png"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[NOISE_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("noise.dds"), &TexDesc);
			m_lpRenderer->LoadTexture(m_SystemTextures[DETAIL_TEXTURE], *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("perlin_noise_nm.dds"), &TexDesc);
			
			SamDesc.AddressU = CRenderer::AM_CLAMP;
			SamDesc.AddressV = CRenderer::AM_CLAMP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			if (m_SystemTextures[LOGO_TEXTURE])
				m_SystemTextures[LOGO_TEXTURE]->CreateSamplerState(SamDesc);

			SamDesc.AddressU = CRenderer::AM_WRAP;
			SamDesc.AddressV = CRenderer::AM_WRAP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			if (m_SystemTextures[WHITE_TEXTURE])
				m_SystemTextures[WHITE_TEXTURE]->CreateSamplerState(SamDesc);
			if (m_SystemTextures[BLACK_TEXTURE])
				m_SystemTextures[BLACK_TEXTURE]->CreateSamplerState(SamDesc);
			if (m_SystemTextures[DEPTHCOMPLEX_TEXTURE])
				m_SystemTextures[DEPTHCOMPLEX_TEXTURE]->CreateSamplerState(SamDesc);
			if (m_SystemTextures[DOT3_TEXTURE])
				m_SystemTextures[DOT3_TEXTURE]->CreateSamplerState(SamDesc);
			if (m_SystemTextures[NOISE_TEXTURE])
				m_SystemTextures[NOISE_TEXTURE]->CreateSamplerState(SamDesc);
			if (m_SystemTextures[DETAIL_TEXTURE])
				m_SystemTextures[DETAIL_TEXTURE]->CreateSamplerState(SamDesc);

			m_lpEngine->vRegisterDefaultVertexLayouts();

			// Create all built-in vertex layouts
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_FIXED],				GetEnumStr((VERTEXTYPE)VT_FIXED));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_VECTOR3],				GetEnumStr((VERTEXTYPE)VT_VECTOR3));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SCREEN_QUAD],			GetEnumStr((VERTEXTYPE)VT_SCREEN_QUAD));			
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE],				GetEnumStr((VERTEXTYPE)VT_SPRITE));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE3D],				GetEnumStr((VERTEXTYPE)VT_SPRITE3D));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE3D_CLUSTER],		GetEnumStr((VERTEXTYPE)VT_SPRITE3D_CLUSTER));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_STATIC],				GetEnumStr((VERTEXTYPE)VT_STATIC));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_RIGID],				GetEnumStr((VERTEXTYPE)VT_RIGID));
			m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_BLEND],				GetEnumStr((VERTEXTYPE)VT_BLEND));
			
			//Create the ScreenQuad obj
			m_lpScreenQuad = LYNXNEW CScreenQuad(this);
			if (m_lpRenderer->GetRendererType() == CRenderer::OGLES2_RENDERER || m_lpRenderer->GetRendererType() == CRenderer::OGLES_RENDERER) 
				m_lpScreenQuad->Create(LYNX_TRUE);
			else
				m_lpScreenQuad->Create();

			// Create all essential maps																	 
			vResizeMaps(m_lpRenderer->GetSystemBackBuffer()->vGetWidth(), m_lpRenderer->GetSystemBackBuffer()->vGetHeight());			
			
			// Load all built-in materials
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BLT], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[BLT]->SetName(_T("BLT"));
			m_PostProcessMaterails[BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[BLT]->m_SpPower = 0;
			m_PostProcessMaterails[BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			MaterialSystem::CTechnique* Tech = &m_PostProcessMaterails[BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
			Tech->vCreatePass(1);
			Tech->SetName(_T("BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");
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

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ANIMATION_BLT], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[ANIMATION_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[ANIMATION_BLT]->SetName(_T("ANIMATION BLT"));
			m_PostProcessMaterails[ANIMATION_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[ANIMATION_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[ANIMATION_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[ANIMATION_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
			Tech->vCreatePass(1);
			Tech->SetName(_T("ANIMATION BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("ANIMATION BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "AnimationDiffuseMapPS");
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
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ONECHANNEL_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[ONECHANNEL_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[ONECHANNEL_BLT]->SetName(_T("ONE CHANNEL BLT"));
			m_PostProcessMaterails[ONECHANNEL_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[ONECHANNEL_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[ONECHANNEL_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[ONECHANNEL_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
			Tech->vCreatePass(1);
			Tech->SetName(_T("ONE CHANNEL BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("ONE CHANNEL BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "OneChannelBltPS");						
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
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[FILL], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[FILL]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[FILL]->SetName(_T("FILL"));
			m_PostProcessMaterails[FILL]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[FILL]->m_SpPower = 0;
			m_PostProcessMaterails[FILL]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[FILL]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
			Tech->vCreatePass(1);
			Tech->SetName(_T("FILL.Tech0"));
			Tech->GetPass(0).SetName(_T("FILL.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "ConstantColorPS");						
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);		
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[COLOR_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[COLOR_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[COLOR_BLT]->SetName(_T("COLOR BLT"));
			m_PostProcessMaterails[COLOR_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[COLOR_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[COLOR_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[COLOR_BLT]->GetTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech->vCreatePass(1);
			Tech->SetName(_T("COLOR BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("COLOR BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "DiffuseMapXColorPS");			
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);			

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ACC_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[ACC_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[ACC_BLT]->SetName(_T("ACC BLT"));
			m_PostProcessMaterails[ACC_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[ACC_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[ACC_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[ACC_BLT]->GetTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech->vCreatePass(1);
			Tech->SetName(_T("ACC BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("ACC BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");			
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);			

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ALPHA_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[ALPHA_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[ALPHA_BLT]->SetName(_T("ALPHA BLT"));
			m_PostProcessMaterails[ALPHA_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[ALPHA_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[ALPHA_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[ALPHA_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("ALPHA BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("ALPHA BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");			
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);			

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[TINT_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[TINT_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[TINT_BLT]->SetName(_T("TINT BLT"));
			m_PostProcessMaterails[TINT_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[TINT_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[TINT_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[TINT_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("TINT BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("TINT BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "TintPS");			
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);		

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[GRAY_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[GRAY_BLT]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[GRAY_BLT]->SetName(_T("GRAY BLT"));
			m_PostProcessMaterails[GRAY_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[GRAY_BLT]->m_SpPower = 0;
			m_PostProcessMaterails[GRAY_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[GRAY_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("GRAY BLT.Tech0"));
			Tech->GetPass(0).SetName(_T("GRAY BLT.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "GrayBltPS");			
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);			

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_ConsoleMaterail, MaterialSystem::CMaterial::POSTPROCESSING);
            m_ConsoleMaterail->SetResourceFlag(SYSTEM_DEFINED);
            m_ConsoleMaterail->SetName(_T("CONSOLE MATERIAL"));
            m_ConsoleMaterail->CreateVertexLayout(GetEnumStr(VT_FIXED));
            m_ConsoleMaterail->m_SpPower = 0;
            m_ConsoleMaterail->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
            Tech = &m_ConsoleMaterail->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
            Tech->vCreatePass(1);
            Tech->SetName(_T("CONSOLE MATERIAL.Tech0"));
            Tech->GetPass(0).SetName(_T("CONSOLE MATERIAL.Tech0.P0"));
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
            Tech->GetPass(0).vSetNumTextureAnimation(0);

            #if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
                m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ADD_TWO_CHANNEL], MaterialSystem::CMaterial::POSTPROCESSING);
                m_PostProcessMaterails[ADD_TWO_CHANNEL]->SetResourceFlag(SYSTEM_DEFINED);
                m_PostProcessMaterails[ADD_TWO_CHANNEL]->SetName(_T("ADD TWO CHANNEL"));
                m_PostProcessMaterails[ADD_TWO_CHANNEL]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
                m_PostProcessMaterails[ADD_TWO_CHANNEL]->m_SpPower = 0;
                m_PostProcessMaterails[ADD_TWO_CHANNEL]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
                Tech = &m_PostProcessMaterails[ADD_TWO_CHANNEL]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
                Tech->vCreatePass(1);
                Tech->SetName(_T("ADD TWO CHANNEL.Tech0"));
                Tech->GetPass(0).SetName(_T("ADD TWO CHANNEL.Tech0.P0"));
                Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "AddTwoChannelPS");			
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
                Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
                Tech->GetPass(0).CreateRasterizerState(RDesc);
            
                m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BILATERAL_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);
                m_PostProcessMaterails[BILATERAL_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
                m_PostProcessMaterails[BILATERAL_BLUR]->SetName(_T("BILTATERAL BLUR"));
                m_PostProcessMaterails[BILATERAL_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
                m_PostProcessMaterails[BILATERAL_BLUR]->m_SpPower = 0;
                m_PostProcessMaterails[BILATERAL_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
                Tech = &m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
                Tech->vCreatePass(1);
                Tech->SetName(_T("BILTATERAL BLUR.Tech0"));
                Tech->GetPass(0).SetName(_T("BILTATERAL BLUR.Tech0.P0"));
                Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BilateralBlurPS");
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
                Tech->GetPass(0).vSetNumTextureAnimation(0);
                
                m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[PROXY_RENDERING], MaterialSystem::CMaterial::POSTPROCESSING);
                m_PostProcessMaterails[PROXY_RENDERING]->SetResourceFlag(SYSTEM_DEFINED);
                m_PostProcessMaterails[PROXY_RENDERING]->SetName(_T("PROXY RENDERING"));
                m_PostProcessMaterails[PROXY_RENDERING]->CreateVertexLayout(GetEnumStr(VT_VECTOR3));
                m_PostProcessMaterails[PROXY_RENDERING]->m_SpPower = 0;
                m_PostProcessMaterails[PROXY_RENDERING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
                Tech = &m_PostProcessMaterails[PROXY_RENDERING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
                Tech->vCreatePass(1);
                Tech->SetName(_T("PROXY RENDERING.Tech0"));
                Tech->GetPass(0).SetName(_T("PROXY RENDERING.Tech0.P0"));
                Tech->GetPass(0).vCreateShaderClasses("ProxyVS", "ProxyPS");
                BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
                BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
                BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ZERO;
                BDesc.RTBlendParam[0].BlendOp		= CRenderer::BO_ADD;
                BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ZERO_CHANNEL;
                Tech->GetPass(0).CreateBlendState(BDesc);
                Desc.DepthMode				= CRenderer::DB_C;
                Desc.DepthFunc				= CRenderer::TF_GEQUAL;
                Desc.StencilEnable			= LYNX_FALSE;
                Tech->GetPass(0).CreateDepthStencilState(Desc);
                RDesc.FillMode = CRenderer::FM_SOLID;
                RDesc.CullMode = CRenderer::CULL_CW;
                Tech->GetPass(0).CreateRasterizerState(RDesc);
                Tech->GetPass(0).vSetNumTextureAnimation(0);
                Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
                
                m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[LIGHT_PROXY_RENDERING], MaterialSystem::CMaterial::POSTPROCESSING);
                m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->SetResourceFlag(SYSTEM_DEFINED);
                m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->SetName(_T("LIGHT PROXY RENDERING"));
                m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->CreateVertexLayout(GetEnumStr(VT_VECTOR3));
                m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->m_SpPower = 0;
                m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
                Tech = &m_PostProcessMaterails[LIGHT_PROXY_RENDERING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
                Tech->vCreatePass(1);
                Tech->SetName(_T("LIGHT PROXY RENDERING.Tech0"));
                Tech->GetPass(0).SetName(_T("LIGHT PROXY RENDERING.Tech0.P0"));
                Tech->GetPass(0).vCreateShaderClasses("ProxyVS", "ProxyPS");
                BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
                BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
                BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ZERO;
                BDesc.RTBlendParam[0].BlendOp		= CRenderer::BO_ADD;
                BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ZERO_CHANNEL;
                Tech->GetPass(0).CreateBlendState(BDesc);
                Desc.DepthMode				= CRenderer::DB_C;
                Desc.DepthFunc				= CRenderer::TF_GEQUAL;
                Desc.StencilEnable			= LYNX_TRUE;
                Desc.StencilRef				= 0x0;
                Desc.StencilReadMask		= 0xf;
                Desc.StencilWriteMask		= 0xf;
                Desc.StencilFunc			= CRenderer::TF_ALWAYS;
                Desc.StencilFailOp			= CRenderer::SO_KEEP;
                Desc.StencilDepthFailOp		= CRenderer::SO_KEEP;
                Desc.StencilPassOp			= CRenderer::SO_INCR;
                Tech->GetPass(0).CreateDepthStencilState(Desc);
                RDesc.FillMode = CRenderer::FM_SOLID;
                RDesc.CullMode = CRenderer::CULL_CW;
                Tech->GetPass(0).CreateRasterizerState(RDesc);
                Tech->GetPass(0).vSetNumTextureAnimation(0);
                Tech->GetPass(0).m_bUseFog = LYNX_FALSE;

				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->SetName(_T("BOX BLUR EDGE CHECK"));
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->m_SpPower = 0;
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("BOX BLUR EDGE CHECK.Tech0"));
				Tech->GetPass(0).SetName(_T("BOX BLUR EDGE CHECK.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurWithEdgeCheckPS");
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
				Tech->GetPass(0).vSetNumTextureAnimation(0);
			
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BOX_MAX], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[BOX_MAX]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[BOX_MAX]->SetName(_T("BOX MAX"));
				m_PostProcessMaterails[BOX_MAX]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[BOX_MAX]->m_SpPower = 0;
				m_PostProcessMaterails[BOX_MAX]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[BOX_MAX]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("BOX MAX.Tech0"));
				Tech->GetPass(0).SetName(_T("BOX MAX.Tech0.P0"));			
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurPS");			
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
				Tech->GetPass(0).vSetNumTextureAnimation(0);
                
                if (m_Config.bDeferredShading)
                {
                }             
            #endif
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[DOWN_SAMPLING], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[DOWN_SAMPLING]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[DOWN_SAMPLING]->SetName(_T("DOWN SAMPLING"));
			m_PostProcessMaterails[DOWN_SAMPLING]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[DOWN_SAMPLING]->m_SpPower = 0;
			m_PostProcessMaterails[DOWN_SAMPLING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DOWN SAMPLING.Tech0"));
			Tech->GetPass(0).SetName(_T("DOWN SAMPLING.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurPS");						
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
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).vSetNumTextureAnimation(0);
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BOX_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[BOX_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[BOX_BLUR]->SetName(_T("BOX BLUR"));
			m_PostProcessMaterails[BOX_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[BOX_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[BOX_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[BOX_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("BOX BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("BOX BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurPS");
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
			Tech->GetPass(0).vSetNumTextureAnimation(0);			
						
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[GAUSSIN_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[GAUSSIN_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[GAUSSIN_BLUR]->SetName(_T("GAUSSIN BLUR"));
			m_PostProcessMaterails[GAUSSIN_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[GAUSSIN_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[GAUSSIN_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("GAUSSIN_BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("GAUSSIN_BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "GaussinBlurPS");						
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
			Tech->GetPass(0).vSetNumTextureAnimation(0);

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[RADIAL_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[RADIAL_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[RADIAL_BLUR]->SetName(_T("RADIAL BLUR"));
			m_PostProcessMaterails[RADIAL_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[RADIAL_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[RADIAL_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[RADIAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("RADIAL_BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("RADIAL_BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "RadialBlurPS");						
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
			Tech->GetPass(0).vSetNumTextureAnimation(0);
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[HALO_RADIAL_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
			m_PostProcessMaterails[HALO_RADIAL_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[HALO_RADIAL_BLUR]->SetName(_T("HALO RADIAL BLUR"));
			m_PostProcessMaterails[HALO_RADIAL_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[HALO_RADIAL_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[HALO_RADIAL_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[HALO_RADIAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("HALO_RADIAL_BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("HALO_RADIAL_BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "HaloRadialBlurPS");						
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
			Tech->GetPass(0).vSetNumTextureAnimation(0);

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[DIRECTIONAL_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->SetName(_T("DIRECTIONAL BLUR"));
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("DIRECTIONAL_BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("DIRECTIONAL_BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "DirectionalBlurPS");
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend = CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp = CRenderer::BO_ADD;			
			BDesc.RTBlendParam[0].RTWriteMask = CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);
			Desc.StencilEnable = LYNX_FALSE;
			Desc.DepthMode = CRenderer::DB_NONE;
			Desc.DepthFunc = CRenderer::TF_ALWAYS;
			Tech->GetPass(0).CreateDepthStencilState(Desc);
			RDesc.FillMode = CRenderer::FM_SOLID;
			RDesc.CullMode = CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).vSetNumTextureAnimation(0);

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[VELOCITY_MAP_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->SetName(_T("VELOCITY_MAP_BLUR"));
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->m_SpPower = 0;
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[VELOCITY_MAP_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("VELOCITY_MAP_BLUR.Tech0"));
			Tech->GetPass(0).SetName(_T("VELOCITY_MAP_BLUR.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "VelocityMapBlurPS");
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend = CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp = CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);
			Desc.StencilEnable = LYNX_FALSE;
			Desc.DepthMode = CRenderer::DB_NONE;
			Desc.DepthFunc = CRenderer::TF_ALWAYS;
			Tech->GetPass(0).CreateDepthStencilState(Desc);
			RDesc.FillMode = CRenderer::FM_SOLID;
			RDesc.CullMode = CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
			Tech->GetPass(0).vSetNumTextureAnimation(0);

			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[SCANLINE_NOISE], MaterialSystem::CMaterial::POSTPROCESSING);
			m_PostProcessMaterails[SCANLINE_NOISE]->SetResourceFlag(SYSTEM_DEFINED);
			m_PostProcessMaterails[SCANLINE_NOISE]->SetName(_T("SCANLINE_NOISE"));
			m_PostProcessMaterails[SCANLINE_NOISE]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
			m_PostProcessMaterails[SCANLINE_NOISE]->m_SpPower = 0;
			m_PostProcessMaterails[SCANLINE_NOISE]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			Tech = &m_PostProcessMaterails[SCANLINE_NOISE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
			Tech->vCreatePass(1);
			Tech->SetName(_T("SCANLINE_NOISE.Tech0"));
			Tech->GetPass(0).SetName(_T("SCANLINE_NOISE.Tech0.P0"));
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "PostScanLineNoisePS");			
			BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend		= CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend		= CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp		= CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ALL_CHANNELS;
			Tech->GetPass(0).CreateBlendState(BDesc);
			Desc.DepthMode				= CRenderer::DB_NONE;
			Desc.DepthFunc				= CRenderer::TF_ALWAYS;			
			Desc.StencilEnable			= LYNX_TRUE;
			Desc.StencilRef				= 0x0;
			Desc.StencilReadMask		= 0xff;
			Desc.StencilWriteMask		= 0xff;
			Desc.StencilFunc			= CRenderer::TF_ALWAYS;
			Desc.StencilFailOp			= CRenderer::SO_KEEP;
			Desc.StencilDepthFailOp		= CRenderer::SO_KEEP;
			Desc.StencilPassOp			= CRenderer::SO_INCR;			
			Tech->GetPass(0).CreateDepthStencilState(Desc);			
			RDesc.FillMode = CRenderer::FM_SOLID;
			RDesc.CullMode = CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).vSetNumTextureAnimation(0);			
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;

			m_lpScreenOutputDevice = LYNXNEW CScreenOutputDevice;
			m_lpScreenOutputDevice->SetlpGraphicsSystem(this);
			m_lpScreenOutputDevice->SetupViewport(0, 0, GetSystemBackBuffer()->vGetWidth(), GetSystemBackBuffer()->vGetHeight());
			m_lpScreenOutputDevice->SetupNumLinesPerPage(15);
			m_lpScreenOutputDevice->m_Origin.x = 30;
			m_lpScreenOutputDevice->m_Origin.y = 25;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Graphics Subsystem OK\r\n")));

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGraphicsSystem::vCreate(CRenderer::RENDERERTYPE rt)
		{	
			#ifdef __USED_RENDERER_ONLY__
				#if defined (__D3D11__)
					m_Config.GraphicParam.RendererType = CRenderer::D3D11_RENDERER;
					m_lpRenderer = LYNXNEW GraphicsSystem::CD3D11Renderer(this);
				#elif  defined(__D3D9__)
					m_Config.GraphicParam.RendererType = CRenderer::D3D9_RENDERER;
					m_lpRenderer = LYNXNEW GraphicsSystem::CD3D9Renderer(this);
				//#elif  defined(__OGLES__)
				//	m_Config.GraphicParam.RendererType = CRenderer::OGLES_RENDERER;
				//	m_lpRenderer = LYNXNEW GraphicsSystem::COGLESRenderer(this);
				#elif  defined(__OGLES2__)
					m_Config.GraphicParam.RendererType = CRenderer::OGLES2_RENDERER;
					m_lpRenderer = LYNXNEW GraphicsSystem::COGLES2Renderer(this);
				#else
					m_Config.GraphicParam.RendererType = CRenderer::NULL_RENDERER;
					m_lpRenderer = LYNXNEW GraphicsSystem::CNullRenderer(this);
				#endif
			#else
				if (rt == CRenderer::VOID_RENDERER)
				{
					CIniReader IniReader(m_lpEngine->GetConfigFilename());
					CString Renderer = IniReader.ReadString(_T("Graphics"), _T("Renderer"), LynxEngine::GetEnumStr(CRenderer::NULL_RENDERER).c_str());
					for (int i=0; i<CRenderer::NUM_RENDERERTYPE; i++)
					{
						if (LynxEngine::GetEnumStr((CRenderer::RENDERERTYPE)i) == Renderer)
							m_Config.GraphicParam.RendererType = (CRenderer::RENDERERTYPE)i;
					}
				}
				else 
				{
					m_Config.GraphicParam.RendererType = rt;
				}

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create "))+LynxEngine::GetEnumStr(m_Config.GraphicParam.RendererType)+CString(_T(" renderer\r\n")));

				switch (m_Config.GraphicParam.RendererType)
				{
				#ifdef __WIN32__
					#ifdef __WINAPP__
						m_lpRenderer = LYNXNEW GraphicsSystem::CD3D11Renderer(this);
					#else
						case CRenderer::D3D11_RENDERER:			
							m_lpRenderer = LYNXNEW GraphicsSystem::CD3D11Renderer(this);
							break;

						case CRenderer::D3D9_RENDERER:
							m_lpRenderer = LYNXNEW GraphicsSystem::CD3D9Renderer(this);
							break;

						case CRenderer::OGLES2_RENDERER:
						m_lpRenderer = LYNXNEW GraphicsSystem::COGLES2Renderer(this);						
							break;
					#endif
				#endif

				#if (defined(__iOS__) || defined(__ANDROID__))
					//case CRenderer::OGLES_RENDERER:
					//	m_lpRenderer = LYNXNEW GraphicsSystem::COGLESRenderer(this);
					//	break;

					case CRenderer::OGLES2_RENDERER:
						m_lpRenderer = LYNXNEW GraphicsSystem::COGLES2Renderer(this);						
						break;
				#endif						

					default:
					case CRenderer::NULL_RENDERER:			
						m_lpRenderer = LYNXNEW GraphicsSystem::CNullRenderer(this);										
						break;
				}				
			#endif
					
			m_lpMaterialSystem = LYNXNEW MaterialSystem::CMaterialSystem((CEngine*)(GetlpEngine()));
			if (!m_lpMaterialSystem)
				return LYNX_FALSE;
			m_lpMaterialSystem->vCreate();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::Resize(int cx, int cy)
		{
			if (m_lpRenderer->IsValid())
			{
				m_lpRenderer->Resize(cx, cy);
				vResizeMaps(cx, cy);
				if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene() && m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer())
					m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->vResize(cx, cy);
			}
			if (m_lpEngine->GetlpGUISystem())
			{
				m_lpEngine->GetlpGUISystem()->ComputeLayoutFactor(LYNX_TRUE);
				LYNXRECT ClipRect;
				ClipRect.x1 = 0; ClipRect.y1 = 0; ClipRect.x2 = cx; ClipRect.y2 =cy;
				m_lpEngine->GetlpGUISystem()->SetClipRect(ClipRect);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGraphicsSystem::vResizeMaps(int cx, int cy)
		{				
			#if defined(__OGLES2__)
			// Resize maps
				if (m_lpMaps[OFFSCREEN])
					LYNXDEL(m_lpMaps[OFFSCREEN]);
				m_lpMaps[OFFSCREEN] = LYNXNEW COffscreen(this);			

				if (m_lpMaps[UI_TRANSITION])
					LYNXDEL(m_lpMaps[UI_TRANSITION]);
				m_lpMaps[UI_TRANSITION] = LYNXNEW COffscreen(this);	

				/*
				if (m_lpMaps[GLOWMAP])
					LYNXDEL(m_lpMaps[GLOWMAP]);
                 m_lpMaps[GLOWMAP] = LYNXNEW CGlowMap(this);
				 */
				
				m_lpEngine->ClearInvalidResource();		

				if (m_lpMaps[OFFSCREEN])
				{
					int OffscreenShift;
					LYNXBOOL bSRGB;
					if (m_Config.bHDRRendering)
						bSRGB = LYNX_FALSE;
					else
						bSRGB = (m_Config.bGammaCorrection) ? LYNX_TRUE : LYNX_FALSE;

                    switch (m_lpEngine->GetlpKernel()->GetDeviceType())
                    {
                        case DT_IPOD_1:
                        case DT_IPOD_2:
                        case DT_IPOD_3:
                        case DT_IPOD_4:
                        case DT_IPHONE_1:
                        case DT_IPHONE_2:
                        case DT_IPHONE_3:
                        case DT_IPAD_1:
                            OffscreenShift = LYNX_CLAMP((4-m_Config.PostprocessQuality), 2, 5);
							//if (!m_lpMaps[OFFSCREEN]->Create(CRenderer::FORMAT_D24S8, 1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                            if (!((COffscreen*)m_lpMaps[OFFSCREEN])->Create(1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                                return LYNX_FALSE;
                            break;
                         
                        default:
                            OffscreenShift = LYNX_CLAMP((3-m_Config.PostprocessQuality), 0, 4);
							//if (!m_lpMaps[OFFSCREEN]->Create(CRenderer::FORMAT_D24S8, 1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                            if (!((COffscreen*)m_lpMaps[OFFSCREEN])->Create(1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                                return LYNX_FALSE;
                            break;
                    }
					//m_lpMaps[OFFSCREEN]->SetlpDepthBuffer(GetSystemDepthBuffer());
					m_lpMaps[OFFSCREEN]->SetlpDepthBuffer(m_lpRenderer->GetSystemDepthBuffer());				
				}						

				if (m_lpMaps[UI_TRANSITION])
				{
					LYNXBOOL bSRGB;
					if (m_Config.bHDRRendering)
						bSRGB = LYNX_FALSE;
					else
						bSRGB = (m_Config.bGammaCorrection) ? LYNX_TRUE : LYNX_FALSE;

					if (!((COffscreen*)m_lpMaps[UI_TRANSITION])->Create(1, _T("UI Transition"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 1, 1, bSRGB))				
						return LYNX_FALSE;				

					//m_lpMaps[UI_TRANSITION]->SetlpDepthBuffer(GetSystemDepthBuffer());
					m_lpMaps[UI_TRANSITION]->SetlpDepthBuffer(m_lpMaps[OFFSCREEN]->GetlpDepthBuffer());
				}

				/*
				switch (m_lpEngine->GetlpKernel()->GetDeviceType())
                {
                    default:
                    case DT_IPHONE_7:
                    case DT_IPHONE_6:
					case DT_IPHONE_5:
					case DT_IPAD_4:
                    case DT_IPAD_5:
					{
                        if (m_lpMaps[GLOWMAP])
                        {
                            int GlowMapShift = 2;//LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 3, 4);
							if (!((GraphicsSystem::CGlowMap*)m_lpMaps[GLOWMAP])->Create(1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;
                            m_lpMaps[GLOWMAP]->SetlpDepthBuffer(m_lpMaps[OFFSCREEN]->GetlpDepthBuffer());
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
                        if (m_lpMaps[GLOWMAP])
                        {
                            int GlowMapShift = 3;//LYNX_CLAMP((3-lpGfxSystem->m_Config.PostprocessQuality), 1, 4);
                            if (!((GraphicsSystem::CGlowMap*)m_lpMaps[GLOWMAP])->Create(1, _T("Glow Map"), cx, cy, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8, 2, GlowMapShift))
                                return LYNX_FALSE;
                            m_lpMaps[GLOWMAP]->SetlpDepthBuffer(m_lpMaps[OFFSCREEN]->GetlpDepthBuffer());
                        }
                        break;
                    }
                }    
				*/
			#else
				// Resize maps
				if (m_lpMaps[OFFSCREEN])
					LYNXDEL(m_lpMaps[OFFSCREEN]);
				m_lpMaps[OFFSCREEN] = LYNXNEW COffscreen(this);			

				if (m_lpMaps[UI_TRANSITION])
					LYNXDEL(m_lpMaps[UI_TRANSITION]);
				m_lpMaps[UI_TRANSITION] = LYNXNEW COffscreen(this);						
			
				m_lpEngine->ClearInvalidResource();		

				if (m_lpMaps[OFFSCREEN])
				{
					int OffscreenShift = 2;//LYNX_CLAMP((3-m_Config.PostprocessQuality), 0, 1);				
					LYNXBOOL bSRGB;
					if (m_Config.bHDRRendering)
						bSRGB = LYNX_FALSE;
					else
						bSRGB = (m_Config.bGammaCorrection) ? LYNX_TRUE : LYNX_FALSE;

					if (!((COffscreen*)m_lpMaps[OFFSCREEN])->Create(1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))				
						return LYNX_FALSE;
					m_lpMaps[OFFSCREEN]->SetlpDepthBuffer(GetSystemDepthBuffer());
				}						

				if (m_lpMaps[UI_TRANSITION])
				{
					int OffscreenShift = 2;//LYNX_CLAMP((3-m_Config.PostprocessQuality), 0, 1);				
					LYNXBOOL bSRGB;
					if (m_Config.bHDRRendering)
						bSRGB = LYNX_FALSE;
					else
						bSRGB = (m_Config.bGammaCorrection) ? LYNX_TRUE : LYNX_FALSE;

					if (!((COffscreen*)m_lpMaps[UI_TRANSITION])->Create(1, _T("UI Transition"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 0, 0, bSRGB))				
						return LYNX_FALSE;				
				}
			#endif
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetMapAsRenderBuffer(CBuffer** lplastbuffers, int index, CMap* lpmap)
		{
			CBuffer *lpRT, *lpDB;
		
			lpRT = m_lpRenderer->SetRenderBuffer(index, lpmap->GetlpColorBuffer());		
			//lpmap->ResetCurrent();
			if (lpmap->GetlpDepthBuffer())
			{
				lpDB = m_lpRenderer->SetDepthBuffer(lpmap->GetlpDepthBuffer());
			}
			else
			{
				lpDB = m_lpRenderer->GetCachedDepthBuffer();
				if (lpDB)
					lpDB->vSetAsDepthBuffer();
			}

			LYNXBOOL bSRGB = (m_Config.bGammaCorrection && lpmap->IsSRGB());
			m_lpRenderer->SetSRGBWrite(bSRGB);
		
			if (lplastbuffers)
			{
				lplastbuffers[0] = lpRT;
				lplastbuffers[1] = lpDB;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetMapAsRenderBuffer(CBuffer** lplastbuffers, int index, CMap* lpmap, int face)
		{
			//lpmap->ReleaseTextureBuffer();
			lpmap->GetLevel(face, 0);
			SetMapAsRenderBuffer(lplastbuffers, index, lpmap); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetupScreenSpaceQuadUVOffset(MaterialSystem::CPass* pass, LYNXUVPOINT* lpuv)
		{
			Shader::CScreenQuadUVOffsetVS* lpVS = (Shader::CScreenQuadUVOffsetVS*)(&(*(pass->GetVS())));

			if (lpuv)
			{
				lpVS->m_UVOffset[0].u = lpuv[0].u;   lpVS->m_UVOffset[0].v = lpuv[0].v;   
				lpVS->m_UVOffset[1].u = lpuv[1].u;   lpVS->m_UVOffset[1].v = lpuv[1].v;   
				lpVS->m_UVOffset[2].u = lpuv[2].u;   lpVS->m_UVOffset[2].v = lpuv[2].v;   
				lpVS->m_UVOffset[3].u = lpuv[3].u;   lpVS->m_UVOffset[3].v = lpuv[3].v;  
			}
			else
			{
				LYNXUVPOINT	SubPixelOffset;
				m_lpRenderer->GetSubPixelOffset(&SubPixelOffset);
				lpVS->m_UVOffset[0].u = SubPixelOffset.u;   lpVS->m_UVOffset[0].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[1].u = SubPixelOffset.u;   lpVS->m_UVOffset[1].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[2].u = SubPixelOffset.u;   lpVS->m_UVOffset[2].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[3].u = SubPixelOffset.u;   lpVS->m_UVOffset[3].v = SubPixelOffset.v;  
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetupScreenSpaceQuadBlurTexcoord_UVOffset(MaterialSystem::CPass* pass, LYNXUVPOINT* bt, LYNXUVPOINT* lpuv)
		{
			Shader::CScreenQuadBlurTexCoordVS* lpVS = (Shader::CScreenQuadBlurTexCoordVS*)(&(*(pass->GetVS())));

			lynxMemCpy(lpVS->m_BlurUVOffset, bt, sizeof(LYNXUVPOINT)*4);

			if (lpuv)
			{
				lpVS->m_UVOffset[0].u = lpuv[0].u;   lpVS->m_UVOffset[0].v = lpuv[0].v;   
				lpVS->m_UVOffset[1].u = lpuv[1].u;   lpVS->m_UVOffset[1].v = lpuv[1].v;   
				lpVS->m_UVOffset[2].u = lpuv[2].u;   lpVS->m_UVOffset[2].v = lpuv[2].v;   
				lpVS->m_UVOffset[3].u = lpuv[3].u;   lpVS->m_UVOffset[3].v = lpuv[3].v;  
			}
			else
			{
				LYNXUVPOINT	SubPixelOffset;
				m_lpRenderer->GetSubPixelOffset(&SubPixelOffset);
				lpVS->m_UVOffset[0].u = SubPixelOffset.u;   lpVS->m_UVOffset[0].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[1].u = SubPixelOffset.u;   lpVS->m_UVOffset[1].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[2].u = SubPixelOffset.u;   lpVS->m_UVOffset[2].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[3].u = SubPixelOffset.u;   lpVS->m_UVOffset[3].v = SubPixelOffset.v;   
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetupScreenSpaceDepthQuadFarCornerDir_UVOffset(MaterialSystem::CPass* pass, CCameraContainer& cam, LYNXUVPOINT* lpuv)
		{
			Shader::CScreenDepthQuadVS* lpVS = (Shader::CScreenDepthQuadVS*)(&(*(pass->GetVS())));

			lpVS->m_FarCornerDir[0].x = cam.GetFarCornerDir(0).x;
			lpVS->m_FarCornerDir[0].y = cam.GetFarCornerDir(0).y;
			lpVS->m_FarCornerDir[0].z = cam.GetFarCornerDir(0).z;

			lpVS->m_FarCornerDir[1].x = cam.GetFarCornerDir(1).x;
			lpVS->m_FarCornerDir[1].y = cam.GetFarCornerDir(1).y;
			lpVS->m_FarCornerDir[1].z = cam.GetFarCornerDir(1).z;

			lpVS->m_FarCornerDir[2].x = cam.GetFarCornerDir(2).x;
			lpVS->m_FarCornerDir[2].y = cam.GetFarCornerDir(2).y;
			lpVS->m_FarCornerDir[2].z = cam.GetFarCornerDir(2).z;

			lpVS->m_FarCornerDir[3].x = cam.GetFarCornerDir(3).x;
			lpVS->m_FarCornerDir[3].y = cam.GetFarCornerDir(3).y;
			lpVS->m_FarCornerDir[3].z = cam.GetFarCornerDir(3).z;

			if (lpuv)
			{
				lpVS->m_UVOffset[0].u = lpuv[0].u;   lpVS->m_UVOffset[0].v = lpuv[0].v;   
				lpVS->m_UVOffset[1].u = lpuv[1].u;   lpVS->m_UVOffset[1].v = lpuv[1].v;   
				lpVS->m_UVOffset[2].u = lpuv[2].u;   lpVS->m_UVOffset[2].v = lpuv[2].v;   
				lpVS->m_UVOffset[3].u = lpuv[3].u;   lpVS->m_UVOffset[3].v = lpuv[3].v;  
			}
			else
			{
				LYNXUVPOINT	SubPixelOffset;
				m_lpRenderer->GetSubPixelOffset(&SubPixelOffset);
				lpVS->m_UVOffset[0].u = SubPixelOffset.u;   lpVS->m_UVOffset[0].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[1].u = SubPixelOffset.u;   lpVS->m_UVOffset[1].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[2].u = SubPixelOffset.u;   lpVS->m_UVOffset[2].v = SubPixelOffset.v;   
				lpVS->m_UVOffset[3].u = SubPixelOffset.u;   lpVS->m_UVOffset[3].v = SubPixelOffset.v;  
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DownSampled(CTexturePtr& dest, CTexturePtr& src)
		{
            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetTextureAsRenderBuffer(0, dest);							
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);	

			float SubPixelU = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth()));
			float SubPixelV = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight()));
			LYNXUVPOINT Offset[4] = { { SubPixelU,  SubPixelV}, 
									  {-SubPixelU, -SubPixelV},
									  {-SubPixelU,  SubPixelV}, 
									  { SubPixelU, -SubPixelV} };
			
			m_lpRenderer->SetViewportRect(0, 0, dest->vGetWidth(), dest->vGetHeight());
			SetupScreenSpaceQuadBlurTexcoord_UVOffset(&(m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique(0).GetPass(0)), Offset);
			m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);						
			m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", src);
			DrawScreenSpaceQuad();
            SetDepthBuffer(OldDepthBuffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawGrayQuad(CTexturePtr& t1, const float time)
		{
			float				SubPixelU, SubPixelV;			
            
			Shader::CGrayBltPS* lpPS = ((Shader::CGrayBltPS*)&(*(m_PostProcessMaterails[GRAY_BLT]->GetTechnique(0).GetPass(0).GetPS())));
			lpPS->m_Time = time;
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[GRAY_BLT]->GetTechnique(0).GetPass(0)));
			m_PostProcessMaterails[GRAY_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
			m_lpRenderer->GetCachedPixelShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), t1);			
			DrawScreenSpaceQuad();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawTintQuad(CTexturePtr& t1, const float time, const LYNXCOLORRGBA& color)
		{
			Shader::CTintPS* lpPS = ((Shader::CTintPS*)&(*(m_PostProcessMaterails[TINT_BLT]->GetTechnique(0).GetPass(0).GetPS())));
			lpPS->m_Time = time;
			lpPS->m_TintColor  = color;
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[TINT_BLT]->GetTechnique(0).GetPass(0)));
			m_PostProcessMaterails[TINT_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
			m_lpRenderer->GetCachedPixelShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), t1);			
			DrawScreenSpaceQuad();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DownSampled(CTexturePtr& dest, LPLYNXRECT rect, CTexturePtr& src)
		{
            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetTextureAsRenderBuffer(0, dest);							
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			float SubPixelU = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth()));
			float SubPixelV = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight()));
			LYNXUVPOINT Offset[4] = { { SubPixelU,  SubPixelV}, 
									  {-SubPixelU, -SubPixelV},
									  {-SubPixelU,  SubPixelV}, 
									  { SubPixelU, -SubPixelV} };
			
			m_lpRenderer->SetViewportRect(rect->x1, rect->y1, rect->x2, rect->y2);
			SetupScreenSpaceQuadBlurTexcoord_UVOffset(&(m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique(0).GetPass(0)), Offset);
			m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);						
			m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", src);
			DrawScreenSpaceQuad();
            SetDepthBuffer(OldDepthBuffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawScanLineNoiseQuad(CTexturePtr& t1, CTexturePtr& t2, float scale)
		{
			float				SubPixelU, SubPixelV;			

            //CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			//SetRenderBuffer(1, NULL);
			//SetRenderBuffer(2, NULL);
			//SetRenderBuffer(3, NULL);
			
			Shader::CPostScanLineNoisePS* lpPS = (Shader::CPostScanLineNoisePS*)(&(*(m_PostProcessMaterails[SCANLINE_NOISE]->GetTechnique(0).GetPass(0).GetPS())));
			lpPS->m_Scale = scale;
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[SCANLINE_NOISE]->GetTechnique(0).GetPass(0)));
			m_PostProcessMaterails[SCANLINE_NOISE]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
			m_lpRenderer->GetCachedPixelShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), t1);
			m_lpRenderer->GetCachedPixelShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::REFRACTIONMAP).c_str(), t2);
			DrawScreenSpaceQuad();

			//SetDepthBuffer(OldDepthBuffer);            
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::MaxPass(CTexturePtr& t1, CTexturePtr& t2)
		{
			CTexture			*Src, *Dest, *Temp;
			float				SubPixelU, SubPixelV;
			LYNXUVPOINT			Offset[4];
			int					Size;

            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);
			
			Size = t2->vGetWidth();

			SubPixelU = 0.5f/((float)(Size));
			SubPixelV = 0.5f/((float)(Size));
			Offset[0].u =  SubPixelU; Offset[0].v =  SubPixelV;
			Offset[1].u = -SubPixelU; Offset[1].v = -SubPixelV;
			Offset[2].u = -SubPixelU; Offset[2].v =  SubPixelV;
			Offset[3].u =  SubPixelU; Offset[3].v = -SubPixelV;
			
			Src = t1;
			Dest = t2;			
			while (Size >= 1)
			{				
				SetTextureAsRenderBuffer(0, Dest);			
				m_lpRenderer->SetViewportRect(0, 0, Size, Size);
				SetupScreenSpaceQuadBlurTexcoord_UVOffset(&(m_PostProcessMaterails[BOX_MAX]->GetTechnique(0).GetPass(0)), Offset);
				m_PostProcessMaterails[BOX_MAX]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
				m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", Src);
				DrawScreenSpaceQuad();

				Temp = Src;
				Src = Dest;
				Dest = Temp;				
				Size >>= 1;
			}
            SetDepthBuffer(OldDepthBuffer);
			return Src;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::BoxBlur(CTexturePtr& t1, CTexturePtr& t2, int times, float offset)
		{
			CTexture			*Src, *Dest, *Temp;
			float				SubPixelU, SubPixelV;
			LYNXUVPOINT			Offset[4];

            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			SubPixelU = 0.5f/((float)(t2->vGetWidth()))*offset;
			SubPixelV = 0.5f/((float)(t2->vGetHeight()))*offset;
			Offset[0].u =  SubPixelU; Offset[0].v =  SubPixelV;
			Offset[1].u = -SubPixelU; Offset[1].v = -SubPixelV;
			Offset[2].u = -SubPixelU; Offset[2].v =  SubPixelV;
			Offset[3].u =  SubPixelU; Offset[3].v = -SubPixelV;
			
			Src = t1;
			Dest = t2;			
			for (int i=0; i<times; ++i)
			{
				SetTextureAsRenderBuffer(0, Dest);		
				m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());
				SetupScreenSpaceQuadBlurTexcoord_UVOffset(&(m_PostProcessMaterails[BOX_BLUR]->GetTechnique(0).GetPass(0)), Offset);
				m_PostProcessMaterails[BOX_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
				m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", Src);
				DrawScreenSpaceQuad();
				
				m_lpRenderer->DiscardRenderBuffer(0);
				m_lpRenderer->DiscardDepthBuffer();

				Temp = Src;
				Src = Dest;
				Dest = Temp;				
			}            
            SetDepthBuffer(OldDepthBuffer);
            
			return Src;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::BoxBlurEdgeCheck(CTexturePtr& t1, CTexturePtr& t2, int times, float sharpness, float offset)
		{
			CTexture			*Src, *Dest, *Temp;
			float				SubPixelU, SubPixelV;
			LYNXUVPOINT			Offset[4];

			CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
            SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CBoxBlurWithEdgeCheckPS* lpPS = ((Shader::CBoxBlurWithEdgeCheckPS*)&(*(m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->GetTechnique(0).GetPass(0).GetPS())));
			if (sharpness <= 0)
				lpPS->m_DepthThresholdValue = 100.0f;
			else
				lpPS->m_DepthThresholdValue = 1.0f/(sharpness*10.0f);

			SubPixelU = 0.5f/((float)(t2->vGetWidth()))*offset;
			SubPixelV = 0.5f/((float)(t2->vGetHeight()))*offset;
			Offset[0].u =  SubPixelU; Offset[0].v =  SubPixelV;
			Offset[1].u = -SubPixelU; Offset[1].v = -SubPixelV;
			Offset[2].u = -SubPixelU; Offset[2].v =  SubPixelV;
			Offset[3].u =  SubPixelU; Offset[3].v = -SubPixelV;

			Src = t1;
			Dest = t2;			
			for (int i=0; i<times; ++i)
			{
				SetTextureAsRenderBuffer(0, Dest);				
				m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());
				SetupScreenSpaceQuadBlurTexcoord_UVOffset(&(m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->GetTechnique(0).GetPass(0)), Offset);
				m_PostProcessMaterails[BOX_BLUR_EDGE_CHECK]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);			
				m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", Src);
				m_lpRenderer->GetCachedPixelShader()->vSetTexture("DepthMap", m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::DEPTHMAP)->GetTexture());				
				DrawScreenSpaceQuad();

				m_lpRenderer->DiscardRenderBuffer(0);
				m_lpRenderer->DiscardDepthBuffer();

				Temp = Src;
				Src = Dest;
				Dest = Temp;				
			}
            SetDepthBuffer(OldDepthBuffer);
            
			return Src;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::GaussinBlur(CTexturePtr& t1, CTexturePtr& t2, float radius, float offset)
		{
			CTexture			*Src, *Dest, *Temp;
			
            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CGaussinBlurPS* lpPS = ((Shader::CGaussinBlurPS*)&(*(m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_Radius = radius;

			Src = t1;
			Dest = t2;			
			SetTextureAsRenderBuffer(0, Dest);		
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_UVOffsetDir.u = offset/(float)Src->vGetWidth();
			lpPS->m_UVOffsetDir.v = 0.0f;

			m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);															
			lpPS->GetlpShader()->vSetTexture("DiffuseMap", Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			Temp = Src;
			Src = Dest;
			Dest = Temp;

			SetTextureAsRenderBuffer(0, Dest);
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());			

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_UVOffsetDir.u = 0.0f;
			lpPS->m_UVOffsetDir.v = offset/(float)Src->vGetHeight();
			
			m_PostProcessMaterails[GAUSSIN_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);											
			lpPS->GetlpShader()->vSetTexture("DiffuseMap", Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();
			
            SetDepthBuffer(OldDepthBuffer);
            
			return Dest;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::RadialBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& center, const float dist, const float weight)
		{
			CTexture			*Src, *Dest, *Temp;
			
            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CRadialBlurPS* lpPS = ((Shader::CRadialBlurPS*)&(*(m_PostProcessMaterails[RADIAL_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_Distance = dist;
			lpPS->m_Weight = weight;
			
			Src = t1;
			Dest = t2;			
			SetTextureAsRenderBuffer(0, Dest);		
			SetDepthBuffer(NULL);
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[RADIAL_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_RadialCenter.u = center.x;
			lpPS->m_RadialCenter.v = center.y;

			m_PostProcessMaterails[RADIAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);															
			lpPS->GetlpShader()->vSetTexture("DiffuseMap", Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			SetDepthBuffer(OldDepthBuffer);
            
			return Dest;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::HaloRadialBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& center, const float halo_radius, const float dist, const float weight)
		{
			CTexture			*Src, *Dest, *Temp;
			
            CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CHaloRadialBlurPS* lpPS = ((Shader::CHaloRadialBlurPS*)&(*(m_PostProcessMaterails[HALO_RADIAL_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_Distance = dist;
			lpPS->m_Weight = weight;
			lpPS->m_HaloRadius = halo_radius;
			
			Src = t1;
			Dest = t2;			
			SetTextureAsRenderBuffer(0, Dest);		
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[HALO_RADIAL_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_RadialCenter.u = center.x;
			lpPS->m_RadialCenter.v = center.y;

			m_PostProcessMaterails[HALO_RADIAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);															
			lpPS->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			SetDepthBuffer(OldDepthBuffer);
            
			return Dest;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::DirectionalBlur(CTexturePtr& t1, CTexturePtr& t2, const CVector2& dir, const float dist, const float weight)
		{
			CTexture			*Src, *Dest, *Temp;

			CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CDirectionalBlurPS* lpPS = ((Shader::CDirectionalBlurPS*)&(*(m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_BlurDir = dir;
			lpPS->m_Distance = dist;
			lpPS->m_Weight = weight;

			Src = t1;
			Dest = t2;
			SetTextureAsRenderBuffer(0, Dest);
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique(0).GetPass(0)));			
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			lpPS->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			SetDepthBuffer(OldDepthBuffer);

			return Dest;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DirectionalBlur(CTexturePtr& t1, CBuffer* buffer, const CVector2& dir, const float dist, const float weight)
		{
			CTexture			*Src, *Temp;

			CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CDirectionalBlurPS* lpPS = ((Shader::CDirectionalBlurPS*)&(*(m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_BlurDir = dir;
			lpPS->m_Distance = dist;
			lpPS->m_Weight = weight;

			Src = t1;			
			SetRenderBuffer(0, buffer);
			m_lpRenderer->SetViewportRect(0, 0, buffer->vGetWidth(), buffer->vGetHeight());
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique(0).GetPass(0)));
			m_PostProcessMaterails[DIRECTIONAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			lpPS->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			SetDepthBuffer(OldDepthBuffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::VelocityMapBlur(CTexturePtr& t1, CTexturePtr& t2, CTexturePtr& vel, const float dist, const float weight)
		{
			CTexture			*Src, *Dest, *Temp;

			CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);

			Shader::CVelocityMapBlurPS* lpPS = ((Shader::CVelocityMapBlurPS*)&(*(m_PostProcessMaterails[VELOCITY_MAP_BLUR]->GetTechnique(0).GetPass(0).GetPS())));

			lpPS->m_Distance = dist;
			lpPS->m_Weight = weight;

			Src = t1;
			Dest = t2;
			SetTextureAsRenderBuffer(0, Dest);
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());
			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[VELOCITY_MAP_BLUR]->GetTechnique(0).GetPass(0)));
			m_PostProcessMaterails[VELOCITY_MAP_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			lpPS->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str(), Src);
			lpPS->GetlpShader()->vSetTexture(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str(), vel);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			SetDepthBuffer(OldDepthBuffer);

			return Dest;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CGraphicsSystem::BilateralBlur(CTexturePtr& t1, CTexturePtr& t2, float radius, float sharpness, float offset)
		{
			CTexture			*Src, *Dest, *Temp;
            
			CBuffer* OldDepthBuffer = SetDepthBuffer(NULL);
			SetRenderBuffer(1, NULL);
			SetRenderBuffer(2, NULL);
			SetRenderBuffer(3, NULL);
						
			Shader::CBilateralBlurPS* lpPS = ((Shader::CBilateralBlurPS*)&(*(m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique(0).GetPass(0).GetPS())));
			float Sigma = (radius+1) / 2;
			float Falloff = 1.0f / (2.0f*Sigma*Sigma);
			lpPS->SetBlurRadius(radius);
			lpPS->SetBlurFalloff(Falloff);
			lpPS->SetBlurSharpness(sharpness);
			
			Src = t1;
			Dest = t2;			
			SetTextureAsRenderBuffer(0, Dest);		
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_UVOffsetDir.u = offset/(float)Dest->vGetWidth();
			lpPS->m_UVOffsetDir.v = 0.0f;

			m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);		
			lpPS->GetlpShader()->vSetTexture("DiffuseMap", Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();

			Temp = Src;
			Src = Dest;
			Dest = Temp;	

			SetTextureAsRenderBuffer(0, Dest);		
			m_lpRenderer->SetViewportRect(0, 0, Dest->vGetWidth(), Dest->vGetHeight());

			SetupScreenSpaceQuadUVOffset(&(m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique(0).GetPass(0)));
			lpPS->m_UVOffsetDir.u = 0.0f;
			lpPS->m_UVOffsetDir.v = offset/(float)Dest->vGetHeight();			

			m_PostProcessMaterails[BILATERAL_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);											
			lpPS->GetlpShader()->vSetTexture("DiffuseMap", Src);
			DrawScreenSpaceQuad();

			m_lpRenderer->DiscardRenderBuffer(0);
			m_lpRenderer->DiscardDepthBuffer();
            
            SetDepthBuffer(OldDepthBuffer);
			
			return Dest;
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawScreenSpaceQuad()
		{
			m_lpScreenQuad->Render();			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawScreenSpaceQuad(CTexture*t, int x, int y, int w, int h)
		{
			float sx, sy, sw, sh;

			sx = ((x/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth())))-0.5f)*2.0f;
			sy = ((y/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight())))-0.5f)*-2.0f;
			sw = w*2.0f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth()));
			sh = h*2.0f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight()));
			SCREENQUADVERTEX V[4] = {
				sx,		sy,		0.0f,	0.0f,	0,
				sx+sw,	sy,		1.0f,	0.0f,	1,		
				sx,		sy-sh,	0.0f,	1.0f,	2,
				sx+sw,	sy-sh,	1.0f,	1.0f,	3,
			};			

			float SubPixelU = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth()));
			float SubPixelV = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight()));

			V[0].u += SubPixelU; V[0].v += SubPixelV;
			V[1].u += SubPixelU; V[1].v += SubPixelV;
			V[2].u += SubPixelU; V[2].v += SubPixelV;
			V[3].u += SubPixelU; V[3].v += SubPixelV;						
			
			if (t)
				m_lpRenderer->GetCachedPixelShader()->vSetTexture("DiffuseMap", t);

			m_lpRenderer->DrawScreenQuad(V, sizeof(SCREENQUADVERTEX));		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawScreenSpaceQuad(float x1, float y1, float x2, float y2)
		{
			float u0 = x1*0.5f+0.5f;
			float u1 = x2*0.5f+0.5f;
			float v0 = 1.0f-(y1*0.5f+0.5f);
			float v1 = 1.0f-(y2*0.5f+0.5f);
			SCREENQUADVERTEX V[4] = {
				x1,		y1,		u0,		v0,		0,
				x2,		y1,		u1,		v0,		1,		
				x1,		y2,		u0,		v1,		2,
				x2,		y2,		u1,		v1,		3,
			};			

			float SubPixelU = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetWidth()));
			float SubPixelV = 0.5f/((float)(m_lpRenderer->GetCachedRenderBuffer(0)->vGetHeight()));

			V[0].u += SubPixelU; V[0].v += SubPixelV;
			V[1].u += SubPixelU; V[1].v += SubPixelV;
			V[2].u += SubPixelU; V[2].v += SubPixelV;
			V[3].u += SubPixelU; V[3].v += SubPixelV;									
			
			m_lpRenderer->DrawScreenQuad(V, sizeof(SCREENQUADVERTEX));		
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetClipPlane(int index, LPLYNXPLANE lpp, CCameraContainer& camera)
		{
			LYNXMATRIX WVPM, TM;
			LYNXPLANE  TPlane;

			lynxMatrixXMatrix(&WVPM, &camera.GetViewMatrix(), &camera.GetProjectionMatrix());
			lynxInverseMatrix(&TM, &WVPM);			
			lynxTransposeMatrix(&TM);

			TPlane.Normal.x = lpp->Normal.x * TM.m11 + lpp->Normal.y * TM.m21 + lpp->Normal.z * TM.m31 + (-lpp->Distance) * TM.m41;
			TPlane.Normal.y = lpp->Normal.x * TM.m12 + lpp->Normal.y * TM.m22 + lpp->Normal.z * TM.m32 + (-lpp->Distance) * TM.m42;
			TPlane.Normal.z = lpp->Normal.x * TM.m13 + lpp->Normal.y * TM.m23 + lpp->Normal.z * TM.m33 + (-lpp->Distance) * TM.m43;
			TPlane.Distance   = -(lpp->Normal.x * TM.m14 + lpp->Normal.y * TM.m24 + lpp->Normal.z * TM.m34 + (-lpp->Distance) * TM.m44);

			SetClipPlane(index, &TPlane);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::SetClipPlane(int index, LPLYNXPLANE lpp, CMatrix4& view, CMatrix4& proj)
		{
			LYNXMATRIX WVPM, TM;
			LYNXPLANE  TPlane;

			lynxMatrixXMatrix(&WVPM, &view, &proj);
			lynxInverseMatrix(&TM, &WVPM);			
			lynxTransposeMatrix(&TM);

			TPlane.Normal.x = lpp->Normal.x * TM.m11 + lpp->Normal.y * TM.m21 + lpp->Normal.z * TM.m31 + (-lpp->Distance) * TM.m41;
			TPlane.Normal.y = lpp->Normal.x * TM.m12 + lpp->Normal.y * TM.m22 + lpp->Normal.z * TM.m32 + (-lpp->Distance) * TM.m42;
			TPlane.Normal.z = lpp->Normal.x * TM.m13 + lpp->Normal.y * TM.m23 + lpp->Normal.z * TM.m33 + (-lpp->Distance) * TM.m43;
			TPlane.Distance   = -(lpp->Normal.x * TM.m14 + lpp->Normal.y * TM.m24 + lpp->Normal.z * TM.m34 + (-lpp->Distance) * TM.m44);

			SetClipPlane(index, &TPlane);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGraphicsSystem::DrawRectWireframe(LYNXRECT&rect, LYNXCOLORRGBA& color)
		{
			LYNXCOLORRGBA Color = { 0, 0, 0, 255 };
			CMatrix4 M, PM, CM, ICM, TM, TM1, SM;
			CVector3 NoScale(1.0f), Scale(1.0f);
			CVector2 P[8];			
			
			LYNXRECT Rect = GetViewportRect();

			P[0].x = rect.x1 + Rect.x1;
			P[0].y = rect.y1 + Rect.y1;
			
			P[1].x = rect.x2 + Rect.x1;
			P[1].y = rect.y1 + Rect.y1;
			
			P[2].x = rect.x2 + Rect.x1;
			P[2].y = rect.y2 + Rect.y1;
			
			P[3].x = rect.x1 + Rect.x1;
			P[3].y = rect.y2 + Rect.y1;
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(P[0], P[1], color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(P[1], P[2], color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(P[2], P[3], color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(P[3], P[0], color);
		}
	}
}