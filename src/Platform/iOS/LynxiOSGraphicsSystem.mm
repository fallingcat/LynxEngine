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

#ifdef __iOS__
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <LynxIniReader.h>
#include <LynxScreenOutputDevice.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
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
	#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
	#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#endif
#include <GraphicsSystem/NullRenderer/LynxNullRenderer.h>
#include <Platform/iOS/LynxiOSGraphicsSystem.h>
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

#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{	
		namespace iOS
		{
			//IMPLEMENT_SCRIPTCLASS_HIERARCHY(CGraphicsSystem, LynxEngine::GraphicsSystem::CGraphicsSystem)
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CGraphicsSystem::CGraphicsSystem(CEngine* const lpengine, CRenderer* ren)
			: LynxEngine::GraphicsSystem::CGraphicsSystem(lpengine, ren)
			{			
				//INIT_SCRIPTCLASS
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CGraphicsSystem::~CGraphicsSystem(void)
			{
			}	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CGraphicsSystem::vLoadConfig(const LYNXCHAR* filename)
			{
                LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load config file...\r\n")));
                
                m_Config.bGammaCorrection = LYNX_FALSE;
                m_lpRenderer->UseCachedState(LYNX_TRUE);
                m_Config.DepthMapFormat = CRenderer::FORMAT_D16;
                m_Config.bCompileShaderOnTheFly = LYNX_TRUE;
                m_Config.bAlwaysRecompileShader = LYNX_FALSE;
                m_Config.PostprocessQuality = 1;
                
                //return;
                                                                                                          
                CIniReader IniReader(filename);
                
                m_Config.bGammaCorrection = IniReader.ReadBool(_T("Graphics"), _T("GammaCorrection"), LYNX_TRUE);

				m_lpRenderer->UseCachedState(IniReader.ReadBool(_T("Graphics"), _T("UseCachedState"), LYNX_TRUE));
				m_lpRenderer->SharedShaderGroup(IniReader.ReadBool(_T("Graphics"), _T("SharedShaderGroup"), LYNX_TRUE));
				m_lpRenderer->SharedShaderClass(IniReader.ReadBool(_T("Graphics"), _T("SharedShaderClass"), LYNX_FALSE));
				m_lpRenderer->CacheShaderGroup(IniReader.ReadBool(_T("Graphics"), _T("CacheShaderGroup"), LYNX_TRUE));
                
                CRenderer::FORMAT SSAOFormats[] = { CRenderer::FORMAT_L8,
                    CRenderer::FORMAT_L16,
                    CRenderer::FORMAT_R16F,
                    CRenderer::FORMAT_X8R8G8B8 };
                
                m_Config.PostprocessQuality = 1;
                
                m_Config.bCompileShaderOnTheFly = IniReader.ReadBool(_T("Graphics"), _T("CompileShaderOnTheFly"), LYNX_TRUE);
                m_Config.bAlwaysRecompileShader = IniReader.ReadBool(_T("Graphics"), _T("AlwaysRecompileShader"), LYNX_FALSE);
                m_Config.PostprocessQuality = LYNX_CLAMP(lynxAtoi(IniReader.ReadString(_T("Graphics"), _T("PostprocessQuality"), CString("3"))), 1, 3);
                
                CString Format;
                
                m_Config.ShadowMapSize = 1024;//IniReader.ReadInt(_T("Graphics"), _T("ShadowMapSize"), 512);
				Format = IniReader.ReadString(_T("Graphics"), _T("ShadowMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R32F).c_str());
				m_Config.ShadowMapFormat = CRenderer::FORMAT_A8R8G8B8;//LynxEngine::GetFormat(Format);
								
				Format = IniReader.ReadString(_T("Graphics"), _T("ShadowMapDepthFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_D16).c_str());
				m_Config.ShadowMapDepthFormat = CRenderer::FORMAT_D16;//LynxEngine::GetFormat(Format);

                
				/*
                LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load config file...\r\n")));
				
                CString SectionString;
                
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
                        SectionString = _T("Graphics_iPod");
                        break;
                        
                    case DT_IPHONE_4:
                    case DT_IPHONE_5:
                    case DT_IPHONE_6:
                    case DT_IPOD_5:
                        SectionString = _T("Graphics_iPhone");
                        break;
                        
                    case DT_IPAD_2:
                    case DT_IPAD_3:
                    case DT_IPAD_4:
                        SectionString = _T("Graphics_iPad");
                        break;
                        
                    default:
                        SectionString = _T("Graphics");
                        break;
                }
                
				CIniReader IniReader(filename);
				
				m_Config.bGammaCorrection = IniReader.ReadBool(SectionString, _T("GammaCorrection "), LYNX_TRUE);
				
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
                //if (GetBackbufferWidth() > 480) 
                //    m_Config.PostprocessQuality = 1;
                //else
                //    m_Config.PostprocessQuality = 2;
				
				m_Config.bCompileShaderOnTheFly = IniReader.ReadBool(SectionString, _T("CompileShaderOnTheFly"), LYNX_TRUE);
				m_Config.bAlwaysRecompileShader = IniReader.ReadBool(SectionString, _T("AlwaysRecompileShader"), LYNX_FALSE);
				
				CString ShaderTarget = IniReader.ReadString(SectionString, _T("ShaderTarget"), LynxEngine::GetEnumStr(CRenderer::SHADER_TARGET_30).c_str());
				for (int i=CRenderer::SHADER_TARGET_11; i<CRenderer::NUM_SHADERTARGET; i++)
				{
					if (LynxEngine::GetEnumStr((CRenderer::SHADERTARGET)i) == ShaderTarget)
						m_Config.ShaderTarget = (CRenderer::SHADERTARGET)i;
				}
				//m_Config.PostprocessQuality = LYNX_CLAMP(lynxAtoi(IniReader.ReadString(SectionString, _T("PostprocessQuality"), CString("3"))), 1, 3);
				m_Config.SSAOQuality		= LYNX_CLAMP(lynxAtoi(IniReader.ReadString(SectionString, _T("SSAOQuality"), CString("3"))), 1, 3);			
				m_Config.bDeferredShading = IniReader.ReadBool(SectionString, _T("DeferredShading"), LYNX_FALSE);	
				
				CString Format;
				
				Format = IniReader.ReadString(SectionString, _T("DepthMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R16F).c_str());
				m_Config.DepthMapFormat = LynxEngine::GetFormat(Format);
				
				m_Config.bHDRRendering = IniReader.ReadBool(SectionString, _T("HDRRendering"), LYNX_FALSE);	
				if (m_Config.bHDRRendering)
				{
					Format = IniReader.ReadString(SectionString, _T("HDRFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_A16B16G16R16F).c_str());
					m_Config.LightingFormat = LynxEngine::GetFormat(Format);
					
					Format = IniReader.ReadString(SectionString, _T("ToneMappingFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R16F).c_str());
					m_Config.ToneMappingFormat = LynxEngine::GetFormat(Format);
				}
				else
				{
					m_Config.LightingFormat = CRenderer::FORMAT_X8R8G8B8;
				}
				
				m_Config.bSharedShadowMap = IniReader.ReadBool(SectionString, _T("SharedShadowMap"), LYNX_FALSE);	
				m_Config.ShadowMapSize = IniReader.ReadInt(SectionString, _T("ShadowMapSize"), 512);
				m_Config.CubeShadowMapSize = IniReader.ReadInt(SectionString, _T("CubeShadowMapSize"), 256);
				m_Config.MaxSplittedShadowMap = IniReader.ReadInt(SectionString, _T("MaxSplittedShadowMap"), 4);
				
				Format = IniReader.ReadString(SectionString, _T("ShadowMapFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_R32F).c_str());
				m_Config.ShadowMapFormat = LynxEngine::GetFormat(Format);
				
				if (!m_lpRenderer->CheckRTFormat(m_Config.GBufferFormat))
					m_Config.GBufferFormat = CRenderer::FORMAT_X8R8G8B8;						
				
				Format = IniReader.ReadString(SectionString, _T("ShadowMapDepthFormat"), LynxEngine::GetEnumStr(CRenderer::FORMAT_D24X8).c_str());
				m_Config.ShadowMapDepthFormat = LynxEngine::GetFormat(Format);	
                 */
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
				InitDefalutResources();
                
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

				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
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
				InitDefalutResources();
                
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

				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CGraphicsSystem::InitDefalutResources(void)
			{	
				LYNX_GUARD(CGraphicsSystem::InitDefalutResources)

				//CPlatformFileStream TexFileStream;
				CMemoryStream TexFileStream;
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
					m_bNeedToRecompileShader = m_lpRenderer->NeedToRecompileShader();				
				}			
				
                m_lpRenderer->SetTexturePath(_T("../texture/"));
                
				CRenderer::TEXTUREDESC TexDesc;
				TexDesc.Default();
				TexDesc.bSRGB = LYNX_FALSE;
				TexDesc.MipMapLevel = 1;			
				TexDesc.bDelayLoading = LYNX_TRUE;	
				m_lpRenderer->LoadTexture(m_SystemTextures[WHITE_TEXTURE], TexFileStream, _T("white.png"), &TexDesc);
				m_lpRenderer->LoadTexture(m_SystemTextures[BLACK_TEXTURE], TexFileStream, _T("black.png"), &TexDesc);

                TexDesc.bLoadFromMemory = LYNX_TRUE;
                TexDesc.MipMapLevel = 1;
                TexDesc.TargetFormat = GraphicsSystem::CRenderer::FORMAT_A8R8G8B8;
                TexDesc.Width = static_cast<GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetCameraCaptureWidth();
                TexDesc.Height = static_cast<GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetCameraCaptureHeight();
                TexDesc.lpData = NULL;                
                m_lpRenderer->LoadTexture(m_SystemTextures[CAMERA_CAPTURE_TEXTURE], TexFileStream, _T("CameraCaptureTexture"), &TexDesc);
				
				SamDesc.AddressU = CRenderer::AM_CLAMP;
				SamDesc.AddressV = CRenderer::AM_CLAMP;
				SamDesc.AddressW = CRenderer::AM_CLAMP;
				SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
                if (m_SystemTextures[LOGO_TEXTURE])
                    m_SystemTextures[LOGO_TEXTURE]->CreateSamplerState(SamDesc);
                if (m_SystemTextures[CAMERA_CAPTURE_TEXTURE])
                    m_SystemTextures[CAMERA_CAPTURE_TEXTURE]->CreateSamplerState(SamDesc);
				
                SamDesc.AddressU = CRenderer::AM_WRAP;
				SamDesc.AddressV = CRenderer::AM_WRAP;
				SamDesc.AddressW = CRenderer::AM_CLAMP;
				SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
                if (m_SystemTextures[WHITE_TEXTURE])
                    m_SystemTextures[WHITE_TEXTURE]->CreateSamplerState(SamDesc);
				if (m_SystemTextures[BLACK_TEXTURE])
                    m_SystemTextures[BLACK_TEXTURE]->CreateSamplerState(SamDesc);				
				
				m_lpEngine->vRegisterDefaultVertexLayouts();

				// Create all built-in vertex layouts
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_FIXED],				GetEnumStr((VERTEXTYPE)VT_FIXED));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_VECTOR3],				GetEnumStr((VERTEXTYPE)VT_VECTOR3));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SCREEN_QUAD],			GetEnumStr((VERTEXTYPE)VT_SCREEN_QUAD));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE],				GetEnumStr((VERTEXTYPE)VT_SPRITE));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE3D],			GetEnumStr((VERTEXTYPE)VT_SPRITE3D));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_SPRITE3D_CLUSTER],	GetEnumStr((VERTEXTYPE)VT_SPRITE3D_CLUSTER));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_STATIC],				GetEnumStr((VERTEXTYPE)VT_STATIC));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_RIGID],				GetEnumStr((VERTEXTYPE)VT_RIGID));
				m_lpEngine->CreateVertexLayout(m_VertexLayouts[VT_BLEND],				GetEnumStr((VERTEXTYPE)VT_BLEND));
				
				//Create the ScreenQuad obj
				m_lpScreenQuad = LYNXNEW CScreenQuad(this);
				m_lpScreenQuad->Create(LYNX_TRUE);
				
				vResizeMaps(m_lpRenderer->GetSystemBackBuffer()->vGetWidth(), m_lpRenderer->GetSystemBackBuffer()->vGetHeight());
				
				// Load all built-in materials
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create BLT material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BLT], MaterialSystem::CMaterial::POSTPROCESSING);
				m_PostProcessMaterails[BLT]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[BLT]->SetName(_T("BLT"));
				m_PostProcessMaterails[BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[BLT]->m_SpPower = 0;
				//m_PostProcessMaterails[BLT]->vCreateTechnique(1);
				m_PostProcessMaterails[BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				MaterialSystem::CTechnique* Tech = &m_PostProcessMaterails[BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
				Tech->vCreatePass(1);
				Tech->SetName(_T("BLT.Tech0"));
				Tech->GetPass(0).SetName(_T("BLT.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= CRenderer::DB_NONE;
				Desc.DepthFunc		= CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);			
				RDesc.FillMode = CRenderer::FM_SOLID;
				RDesc.CullMode = CRenderer::CULL_NONE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;			
							
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create FILL material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[FILL], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[FILL]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[FILL]->SetName(_T("FILL"));
				m_PostProcessMaterails[FILL]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[FILL]->m_SpPower = 0;
				//m_PostProcessMaterails[FILL]->vCreateTechnique(1);
				m_PostProcessMaterails[FILL]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[FILL]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
				Tech->vCreatePass(1);
				Tech->SetName(_T("FILL.Tech0"));
				Tech->GetPass(0).SetName(_T("FILL.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "ConstantColorPS");						
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
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

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ACC_BLT material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ACC_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[ACC_BLT]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[ACC_BLT]->SetName(_T("ACC BLT"));
				m_PostProcessMaterails[ACC_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[ACC_BLT]->m_SpPower = 0;
				//m_PostProcessMaterails[ACC_BLT]->vCreateTechnique(1);
				m_PostProcessMaterails[ACC_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[ACC_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("ACC BLT.Tech0"));
				Tech->GetPass(0).SetName(_T("ACC BLT.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "DiffuseMapXColorPS");			
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.StencilEnable	= LYNX_FALSE;		
				Desc.DepthMode		= CRenderer::DB_NONE;
				Desc.DepthFunc		= CRenderer::TF_ALWAYS;			
				Tech->GetPass(0).CreateDepthStencilState(Desc);			
				RDesc.FillMode = CRenderer::FM_SOLID;
				RDesc.CullMode = CRenderer::CULL_NONE;
				Tech->GetPass(0).m_bUseFog = LYNX_FALSE;
				Tech->GetPass(0).CreateRasterizerState(RDesc);			

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create ALPHA_BLT material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[ALPHA_BLT], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[ALPHA_BLT]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[ALPHA_BLT]->SetName(_T("ALPHA BLT"));
				m_PostProcessMaterails[ALPHA_BLT]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[ALPHA_BLT]->m_SpPower = 0;
				//m_PostProcessMaterails[ALPHA_BLT]->vCreateTechnique(1);
				m_PostProcessMaterails[ALPHA_BLT]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[ALPHA_BLT]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("ALPHA BLT.Tech0"));
				Tech->GetPass(0).SetName(_T("ALPHA BLT.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadUVOffsetVS", "BltPS");			
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
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

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create DOWN_SAMPLING material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[DOWN_SAMPLING], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[DOWN_SAMPLING]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[DOWN_SAMPLING]->SetName(_T("DOWN SAMPLING"));
				m_PostProcessMaterails[DOWN_SAMPLING]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[DOWN_SAMPLING]->m_SpPower = 0;
				//m_PostProcessMaterails[DOWN_SAMPLING]->vCreateTechnique(1);
				m_PostProcessMaterails[DOWN_SAMPLING]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[DOWN_SAMPLING]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("DOWN SAMPLING.Tech0"));
				Tech->GetPass(0).SetName(_T("DOWN SAMPLING.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurPS");						
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
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

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create BOX BLUR material\r\n")));
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_PostProcessMaterails[BOX_BLUR], MaterialSystem::CMaterial::POSTPROCESSING);			
				m_PostProcessMaterails[BOX_BLUR]->SetResourceFlag(SYSTEM_DEFINED);
				m_PostProcessMaterails[BOX_BLUR]->SetName(_T("BOX BLUR"));
				m_PostProcessMaterails[BOX_BLUR]->CreateVertexLayout(GetEnumStr(VT_SCREEN_QUAD));
				m_PostProcessMaterails[BOX_BLUR]->m_SpPower = 0;
				//m_PostProcessMaterails[BOX_BLUR]->vCreateTechnique(1);
				m_PostProcessMaterails[BOX_BLUR]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Tech = &m_PostProcessMaterails[BOX_BLUR]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
				Tech->vCreatePass(1);
				Tech->SetName(_T("BOX BLUR.Tech0"));
				Tech->GetPass(0).SetName(_T("BOX BLUR.Tech0.P0"));
				Tech->GetPass(0).vCreateShaderClasses("ScreenQuadBlurTexCoordVS", "BoxBlurPS");
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
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

				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create GAUSSIN BLUR material\r\n")));
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
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= CRenderer::BO_ADD;
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
				
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create RADIAL BLUR material\r\n")));
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
				m_PostProcessMaterails[DIRECTIONAL_BLUR]->SetName(_T("DIRECTIONAL_BLUR"));
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
				BDesc.RTBlendParam[0].RTWriteMask	= CRenderer::ZERO_CHANNEL;
				Tech->GetPass(0).CreateBlendState(BDesc);
				Desc.DepthMode				= CRenderer::DB_NONE;
				Desc.DepthFunc				= CRenderer::TF_ALWAYS;			
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

				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CGraphicsSystem::vCreate(CRenderer::RENDERERTYPE rt)
			{	
				return LynxEngine::GraphicsSystem::CGraphicsSystem::vCreate(rt);
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CGraphicsSystem::vResizeMaps(int cx, int cy)
			{				
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
                        case DT_IPHONE_4:
                        case DT_IPAD_1:
                            OffscreenShift = 3;//LYNX_CLAMP((4-m_Config.PostprocessQuality), 2, 5);
                            if (!((COffscreen*)m_lpMaps[OFFSCREEN])->Create(1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                                return LYNX_FALSE;
                            break;
                         
                        default:
                            OffscreenShift = 2;//LYNX_CLAMP((3-m_Config.PostprocessQuality), 0, 4);
                            if (!((COffscreen*)m_lpMaps[OFFSCREEN])->Create(1, _T("Offscreen"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 2, OffscreenShift, bSRGB))
                                return LYNX_FALSE;
                            break;
                    }
					//m_lpMaps[OFFSCREEN]->SetlpDepthBuffer(GetSystemDepthBuffer());
                    //m_lpMaps[OFFSCREEN]->SetlpDepthBuffer(GetlpRenderer()->GetOffscreenDepthBuffer());
				}						

				if (m_lpMaps[UI_TRANSITION])
				{
					LYNXBOOL bSRGB;
					if (m_Config.bHDRRendering)
						bSRGB = LYNX_FALSE;
					else
						bSRGB = (m_Config.bGammaCorrection) ? LYNX_TRUE : LYNX_FALSE;

					if (!((COffscreen*)m_lpMaps[UI_TRANSITION])->Create(1, _T("UI Transition"), cx, cy, CRenderer::FORMAT_A8R8G8B8, 0, 0, bSRGB))				
						return LYNX_FALSE;
                    
                    m_lpMaps[UI_TRANSITION]->SetlpDepthBuffer(m_lpMaps[OFFSCREEN]->GetlpDepthBuffer());
				}
											
				return LYNX_TRUE;
			}
		}
	}
}

#endif