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

#ifdef __ANDROID__

#include <Version.info>
#include <LynxIniReader.h>
#include <LynxFileStream.h>
#include <LynxMemoryStream.h>
#include <LynxLogger.h>
#include <LynxDebugMemAllocator.h>
#include <LynxScreenOutputDevice.h>
#include <Font/LynxFont.h>
#include <Font/LynxFontString.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxSamplerState.h>
#include <GraphicsSystem/LynxToneMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>
#include <Platform/Android/LynxAndroidFileSystem.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxAtmosphere.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxScnRenderer.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#include <ScriptSystem/LynxScript.h>
#include <MaterialSystem/LynxTextureAnimation.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxTextureAnimation.h>

#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <InputSystem/LynxInputSystem.h>
#include <SoundSystem/LynxSoundSystem.h>
#include <CollisionSystem/LynxCollisionSystem.h>
#include <CollisionSystem/Bullet/LynxBtCollisionSystem.h>
#include <NetworkSystem/LynxNetworkSystem.h>

#include <Platform/Android/LynxAndroidEngine.h>
#include <Platform/Android/LynxAndroidGraphicsSystem.h>
//#include <Platform/Android/LynxAndroidVideo.h>
//#include <Platform/Android/LynxAndroidAccelerometer.h>

// Containers
#include <Container/LynxContainer.h>
#include <Container/LynxReferenceContainer.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxGeometryContainer.h>
#include <Container/LynxTerrainContainer.h>
#include <Container/LynxMeshContainer.h>
#include <Container/LynxBoneContainer.h>
#include <Container/LynxModelContainer.h>
#include <Container/LynxPEmitterContainer.h>
#include <Container/LynxSpriteContainer.h>
#include <Container/LynxSprite3DContainer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSkyDomeContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxCrossSpriteGrassContainer.h>
#include <Container/LynxSkyLightMap.h>
#include <Container/LynxVolumetricCloudContainer.h>

//Shaders
#include <Shader/LynxCustomVS.h>
#include <Shader/LynxCustomPS.h>
#include <Shader/LynxFontStringVS.h>
#include <Shader/LynxColorPS.h>
#include <Shader/LynxConstantColorPS.h>
#include <Shader/LynxTintPS.h>
#include <Shader/LynxBltPS.h>
#include <Shader/LynxDiffuseMapPS.h>
#include <Shader/LynxAnimationDiffuseMapPS.h>
#include <Shader/LynxDiffuseMapXColorPS.h>
#include <Shader/LynxDiffuseMapXAlphaPS.h>
#include <Shader/LynxDiffuseMapXColorBlendPS.h>
#include <Shader/LynxDiffuseMapXDiffuseColorPS.h>
#include <Shader/LynxOneChannelBltPS.h>
#include <Shader/LynxAddTwoChannelPS.h>
#include <Shader/LynxBoxBlurPS.h>
#include <Shader/LynxBoxBlurWithEdgeCheckPS.h>
#include <Shader/LynxGaussinBlurPS.h>
#include <Shader/LynxRadialBlurPS.h>
#include <Shader/LynxBilateralBlurPS.h>
#include <Shader/LynxDistortionPS.h>
#include <Shader/LynxGPassPS.h>
#include <Shader/LynxSimplePassPS.h>
#include <Shader/LynxShadowMapPS.h>
#include <Shader/LynxShadowMap_AlphaTestPS.h>
#include <Shader/LynxAmbientLightingPS.h>
#include <Shader/LynxSpotLightingPS.h>
#include <Shader/LynxPointLightingPS.h>
#include <Shader/LynxDirectionalLightingPS.h>
#include <Shader/LynxSimpleLightingPS.h>
#include <Shader/LynxOneLightingPS.h>
#include <Shader/LynxOneLightingNoShadowPS.h>
#include <Shader/LynxDiffuseMapXShadowPS.h>
#include <Shader/LynxGeometryVS.h>
#include <Shader/LynxProxyVS.h>
#include <Shader/LynxProxyPS.h>
#include <Shader/LynxGeometryDiffuseVS.h>
#include <Shader/LynxGeometryLightmapVS.h>
#include <Shader/LynxGeometryLightingVS.h>
#include <Shader/LynxGeometryOneLightingVS.h>
#include <Shader/LynxGeometryZPassVS.h>
#include <Shader/LynxGeometryGPassVS.h>
#include <Shader/LynxGeometryShadowMapVS.h>
#include <Shader/LynxGeometrySimpleLightingVS.h>
#include <Shader/LynxRigidModelVS.h>
#include <Shader/LynxRigidModelZPassVS.h>
#include <Shader/LynxRigidModelGPassVS.h>
#include <Shader/LynxRigidModelShadowMapVS.h>
#include <Shader/LynxRigidModelLightingVS.h>
#include <Shader/LynxRigidModelOneLightingVS.h>
#include <Shader/LynxSkinningModelVS.h>
#include <Shader/LynxSkinningModelZPassVS.h>
#include <Shader/LynxSkinningModelGPassVS.h>
#include <Shader/LynxSkinningModelShadowMapVS.h>
#include <Shader/LynxSkinningModelSimpleLightingVS.h>
#include <Shader/LynxSkinningModelLightingVS.h>
#include <Shader/LynxSkinningModelOneLightingVS.h>
#include <Shader/LynxSpriteVS.h>
#include <Shader/LynxSprite3DVS.h>
#include <Shader/LynxSprite3DZPassVS.h>
#include <Shader/LynxSprite3DShadowMapVS.h>
#include <Shader/LynxSprite3DClusterVS.h>
#include <Shader/LynxSprite3DClusterZPassVS.h>
#include <Shader/LynxSprite3DClusterGPassVS.h>
#include <Shader/LynxSprite3DClusterGPassPS.h>
#include <Shader/LynxSprite3DClusterShadowMapVS.h>
#include <Shader/LynxSpriteClusterVS.h>
#include <Shader/LynxSpriteGrassVS.h>
#include <Shader/LynxSpriteGrassZPassVS.h>
#include <Shader/LynxSpriteGrassGPassVS.h>
#include <Shader/LynxSpriteGrassGPassPS.h>
#include <Shader/LynxSpriteGrassShadowMapVS.h>
#include <Shader/LynxSpriteGrassFadeOutAmbientLightingPS.h>
#include <Shader/LynxSpriteGrassFadeOutDirectionalLightingPS.h>
#include <Shader/LynxCrossSpriteGrassVS.h>
#include <Shader/LynxCrossSpriteGrassZPassVS.h>
#include <Shader/LynxCrossSpriteGrassGPassVS.h>
#include <Shader/LynxCrossSpriteGrassShadowMapVS.h>
#include <Shader/LynxCrossSpriteGrassFadeOutVS.h>
#include <Shader/LynxTerrainGPassPS.h>
#include <Shader/LynxTerrainGPassVS.h>
#include <Shader/LynxTerrainShadowMapVS.h>
#include <Shader/LynxTerrainSimpleLightingVS.h>
#include <Shader/LynxTerrainSimpleLightingPS.h>
#include <Shader/LynxTerrainSimplePS.h>
#include <Shader/LynxTerrainVS.h>
#include <Shader/LynxTerrainZPassVS.h>
#include <Shader/LynxZPass_Velocity_AlphaTestPS.h>
#include <Shader/LynxZPass_VelocityPS.h>
#include <Shader/LynxScreenQuadVS.h>
#include <Shader/LynxScreenQuadUVOffsetVS.h>
#include <Shader/LynxScreenQuadBlurTexCoordVS.h>
#include <Shader/LynxScreenDepthQuadVS.h>
#include <Shader/LynxSkyDomeVS.h>
#include <Shader/LynxSkyDomePS.h>
#include <Shader/LynxSkyBoxVS.h>
#include <Shader/LynxSkyLightPS.h>
#include <Shader/LynxDeferredAmbientLightingPS.h>
#include <Shader/LynxDeferredDirectionalLightingPS.h>
#include <Shader/LynxDeferredSpotLightingPS.h>
#include <Shader/LynxDeferredPointLightingPS.h>
#include <Shader/LynxPostAdaptLuminancePS.h>
#include <Shader/LynxPostAverageLuminancePS.h>
#include <Shader/LynxPostBrightPassPS.h>
#include <Shader/LynxPostComputeLuminancePS.h>
#include <Shader/LynxPostGetLuminancePS.h>
#include <Shader/LynxPostToneMappingPS.h>
#include <Shader/LynxPostMotionBlurPS.h>
#include <Shader/LynxPostDOFPS.h>
#include <Shader/LynxPostSSAOPS.h>
#include <Shader/LynxPostSSAOLQPS.h>
#include <Shader/LynxTextureAnimationPreviewVS.h>
#include <Shader/LynxTextureAnimationPreviewPS.h>
#include <Shader/LynxVoxelClusterVS.h>
#include <Shader/LynxVoxelClusterOneLightingVS.h>
#include <Shader/LynxVoxelColorPS.h>
#include <Shader/LynxVoxelClusterShadowMapVS.h>
#include <Shader/LynxVoxelSpriteVS.h>
#include <Shader/LynxVoxelSpriteOneLightingVS.h>

#ifdef __LUA__
	#include <LynxLuaScriptSystem.h>
#endif

#ifdef __MONO__
	#include <LynxMonoScriptSystem.h>
#endif

#if (defined(__WIN32__) && defined(__PHYSX__))
	#include <LynxNxPhysicsSystem.h>
#endif

#ifdef __BULLET__
	#include <LynxBtPhysicsSystem.h>
#endif

#ifdef __BOX2D__
	#include <LynxB2PhysicsSystem.h>
#endif

#ifdef __OPENAL__
	#include <LynxOALSoundSystem.h>
#endif

#ifdef __OPENSL__
	#include <LynxOSLSoundSystem.h>
#endif

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CContainer* CreateReference(CEngine* e);
	CContainer*	CreateCGeometry(CEngine* e);
	CContainer*	CreateTerrain(CEngine* e);	
	CContainer*	CreateCamera(CEngine* e);
	CContainer*	CreateModel(CEngine* e);
	CContainer*	CreateSprite(CEngine* e);
	CContainer*	CreateSprite3D(CEngine* e);
	CContainer*	CreateSprite3DCluster(CEngine* e);
    CContainer*	CreateSpriteCluster(CEngine* e);
	CContainer*	CreateClusterSub(CEngine* e);
    CContainer*	CreateVoxelCluster(CEngine* e);
	CContainer*	CreatePEmitter(CEngine* e);
	CContainer*	CreateMesh(CEngine* e);
	CContainer*	CreateBone(CEngine* e);			
	CContainer*	CreateGrass(CEngine* e);			
	CContainer*	CreateCrossGrass(CEngine* e);		
	CContainer*	CreateSkyDome(CEngine* e);		
	CContainer*	CreateVolumetricCloud(CEngine* e);
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	DECLARE_SHADERCLASS(CustomVS)
	DECLARE_SHADERCLASS(CustomPS)
    
	DECLARE_SHADERCLASS(FontStringVS)
    
	DECLARE_SHADERCLASS(BltPS)
	DECLARE_SHADERCLASS(ColorPS)
	DECLARE_SHADERCLASS(ConstantColorPS)
    DECLARE_SHADERCLASS(TintPS)
	DECLARE_SHADERCLASS(GrayBltPS)
	DECLARE_SHADERCLASS(DiffuseMapPS)
	DECLARE_SHADERCLASS(AnimationDiffuseMapPS)
	DECLARE_SHADERCLASS(DiffuseMapXColorPS)	
	DECLARE_SHADERCLASS(DiffuseMapXAlphaPS)
	DECLARE_SHADERCLASS(DiffuseMapXDiffuseColorPS)
	DECLARE_SHADERCLASS(OneChannelBltPS)
	DECLARE_SHADERCLASS(AddTwoChannelPS)
	DECLARE_SHADERCLASS(DistortionPS)
	DECLARE_SHADERCLASS(SimpleDistortionPS)
	
	DECLARE_SHADERCLASS(BoxBlurPS)
	DECLARE_SHADERCLASS(BoxBlurWithEdgeCheckPS)
	DECLARE_SHADERCLASS(GaussinBlurPS)
	DECLARE_SHADERCLASS(RadialBlurPS)
	DECLARE_SHADERCLASS(HaloRadialBlurPS)
	DECLARE_SHADERCLASS(DirectionalBlurPS)
	DECLARE_SHADERCLASS(VelocityMapBlurPS)
	DECLARE_SHADERCLASS(BilateralBlurPS)
	DECLARE_SHADERCLASS(ScreenQuadVS)
	DECLARE_SHADERCLASS(ScreenQuadUVOffsetVS)
	DECLARE_SHADERCLASS(ScreenQuadBlurTexCoordVS)
	DECLARE_SHADERCLASS(ScreenDepthQuadVS)
	
	DECLARE_SHADERCLASS(AmbientLightingPS)
	DECLARE_SHADERCLASS(SpotLightingPS)
	DECLARE_SHADERCLASS(PointLightingPS)
	DECLARE_SHADERCLASS(DirectionalLightingPS)
	DECLARE_SHADERCLASS(SimpleLightingPS)
	DECLARE_SHADERCLASS(OneLightingPS)
	DECLARE_SHADERCLASS(OneLightingNoShadowPS)
	DECLARE_SHADERCLASS(DiffuseMapXShadowPS)
    
	DECLARE_SHADERCLASS(DeferredAmbientLightingPS)
	DECLARE_SHADERCLASS(DeferredDirectionalLightingPS)
	DECLARE_SHADERCLASS(DeferredSpotLightingPS)
	DECLARE_SHADERCLASS(DeferredPointLightingPS)
	DECLARE_SHADERCLASS(GPassPS)
    
	DECLARE_SHADERCLASS(SimplePassPS)
    
	DECLARE_SHADERCLASS(PostAdaptLuminancePS)
	DECLARE_SHADERCLASS(PostAverageLuminancePS)
	DECLARE_SHADERCLASS(PostBrightPassPS)
	DECLARE_SHADERCLASS(PostComputeLuminancePS)
	DECLARE_SHADERCLASS(PostGetLuminancePS)
	DECLARE_SHADERCLASS(PostToneMappingPS)
	DECLARE_SHADERCLASS(PostMotionBlurPS)
	DECLARE_SHADERCLASS(PostDOFPS)
	DECLARE_SHADERCLASS(PostSSAOPS)
	DECLARE_SHADERCLASS(PostSSAOLQPS)
	DECLARE_SHADERCLASS(PostScanLineNoisePS)
    
	DECLARE_SHADERCLASS(ProxyVS)
	DECLARE_SHADERCLASS(ProxyPS)
    
	DECLARE_SHADERCLASS(ShadowMapPS)
	DECLARE_SHADERCLASS(ShadowMap_AlphaTestPS)
	DECLARE_SHADERCLASS(ZPass_VelocityPS)
	DECLARE_SHADERCLASS(ZPass_Velocity_AlphaTestPS)
    
	// Containers
	DECLARE_SHADERCLASS(GeometryVS)
	DECLARE_SHADERCLASS(GeometryWithViewDirVS)
	DECLARE_SHADERCLASS(GeometryDiffuseVS)
	DECLARE_SHADERCLASS(GeometryLightmapVS)
	DECLARE_SHADERCLASS(GeometryZPassVS)
	DECLARE_SHADERCLASS(GeometryGPassVS)
	DECLARE_SHADERCLASS(GeometryShadowMapVS)
	DECLARE_SHADERCLASS(GeometrySimpleLightingVS)
	DECLARE_SHADERCLASS(GeometryLightingVS)
	DECLARE_SHADERCLASS(GeometryOneLightingVS)
    
	DECLARE_SHADERCLASS(RigidModelVS)
	DECLARE_SHADERCLASS(RigidModelWithViewDirVS)
	DECLARE_SHADERCLASS(RigidModelZPassVS)
	DECLARE_SHADERCLASS(RigidModelGPassVS)
	DECLARE_SHADERCLASS(RigidModelShadowMapVS)
	DECLARE_SHADERCLASS(RigidModelLightingVS)
	DECLARE_SHADERCLASS(RigidModelOneLightingVS)
    
	DECLARE_SHADERCLASS(SkinningModelVS)
	DECLARE_SHADERCLASS(SkinningModelZPassVS)
	DECLARE_SHADERCLASS(SkinningModelGPassVS)
	DECLARE_SHADERCLASS(SkinningModelShadowMapVS)
	DECLARE_SHADERCLASS(SkinningModelSimpleLightingVS)
	DECLARE_SHADERCLASS(SkinningModelLightingVS)
    DECLARE_SHADERCLASS(SkinningModelOneLightingVS)
    
	DECLARE_SHADERCLASS(SpriteVS)
    
	DECLARE_SHADERCLASS(Sprite3DVS)
	DECLARE_SHADERCLASS(Sprite3DZPassVS)
	DECLARE_SHADERCLASS(Sprite3DShadowMapVS)
    
	DECLARE_SHADERCLASS(Sprite3DClusterVS)
	DECLARE_SHADERCLASS(Sprite3DClusterZPassVS)
	DECLARE_SHADERCLASS(Sprite3DClusterGPassVS)
	DECLARE_SHADERCLASS(Sprite3DClusterGPassPS)
	DECLARE_SHADERCLASS(Sprite3DClusterShadowMapVS)
    
	DECLARE_SHADERCLASS(SpriteClusterVS)
    
	DECLARE_SHADERCLASS(VoxelClusterVS)
    DECLARE_SHADERCLASS(VoxelClusterShadowMapVS)
    DECLARE_SHADERCLASS(VoxelSpriteVS)
    DECLARE_SHADERCLASS(VoxelSpriteOneLightingVS)
    /*
	DECLARE_SHADERCLASS(VoxelClusterOneLightingVS)
	DECLARE_SHADERCLASS(VoxelClusterOneLightingSpecularVS)
	DECLARE_SHADERCLASS(VoxelClusterOneLightingFakeAOVS)
	DECLARE_SHADERCLASS(VoxelClusterOneLightingFakeAOSpecularVS)
	DECLARE_SHADERCLASS(VoxelClusterOneLightingFakeAOLevel2SpecularVS)
	DECLARE_SHADERCLASS(VoxelColorPS)
	DECLARE_SHADERCLASS(VoxelColorFakeAOPS)
	DECLARE_SHADERCLASS(VoxelColorFakeAOLevel2PS)		
    */
    
	DECLARE_SHADERCLASS(SpriteGrassVS)
	DECLARE_SHADERCLASS(SpriteGrassZPassVS)
	DECLARE_SHADERCLASS(SpriteGrassGPassVS)
	DECLARE_SHADERCLASS(SpriteGrassGPassPS)
	DECLARE_SHADERCLASS(SpriteGrassShadowMapVS)
	DECLARE_SHADERCLASS(SpriteGrassFadeOutAmbLightingPS)
	DECLARE_SHADERCLASS(SpriteGrassFadeOutDirLightingPS)
    
	DECLARE_SHADERCLASS(CrossSpriteGrassVS)
	DECLARE_SHADERCLASS(CrossSpriteGrassZPassVS)
	DECLARE_SHADERCLASS(CrossSpriteGrassGPassVS)
	DECLARE_SHADERCLASS(CrossSpriteGrassShadowMapVS)
	DECLARE_SHADERCLASS(CrossSpriteGrassFadeOutVS)
    
	DECLARE_SHADERCLASS(TerrainVS)
	DECLARE_SHADERCLASS(TerrainSimplePS)
	DECLARE_SHADERCLASS(TerrainZPassVS)
	DECLARE_SHADERCLASS(TerrainGPassVS)
	DECLARE_SHADERCLASS(TerrainGPassPS)
	DECLARE_SHADERCLASS(TerrainShadowMapVS)
	DECLARE_SHADERCLASS(TerrainSimpleLightingVS)
	DECLARE_SHADERCLASS(TerrainSimpleLightingPS)
	
	DECLARE_SHADERCLASS(SkyDomeVS)
	DECLARE_SHADERCLASS(SkyDomePS)
	DECLARE_SHADERCLASS(SkyLightPS)
    
	DECLARE_SHADERCLASS(SkyBoxVS)		

	namespace Android
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CEngine::CEngine(void)
		{
			//m_lpSystemTouch = NULL;
			///m_lpSystemAccelerometer = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CEngine::CEngine(GameFramework::CKernel* lpk)
		: LynxEngine::CEngine(lpk)
		{	
			//m_lpSystemTouch = NULL;
			//m_lpSystemAccelerometer = NULL;
			//m_lpSystemMotionInput = NULL;
			m_ConfigFilename = _T("../config/android_engine.ini");		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CEngine::~CEngine(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CEngine::vCreateSubSystems(GameFramework::CKernel::INIT_DESC* desc)	
		{
            // Create file system
            LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create file systenm\r\n")));
			m_lpFileSystem = LYNXNEW FileSystem::Android::CFileSystem(this);
            if (m_lpFileSystem)
            {
                if (!m_lpFileSystem->vCreate())
                    return LYNX_FALSE;
                
                //m_lpFileSystem->OpenPackage(_T("../engineresources.zip"));	
            }
			// Create script system
            // Script system must be loaded first so the following systems can use script
            // 腳本系統必須第一個被載入, 如此後續載入的系統才可以使用腳本.
            //LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load script subsystem <"))+CString(ScriptDLL)+CString(_T(">\r\n")));
            LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create script systenm\r\n")));
            m_lpScriptSystem = (ScriptSystem::CScriptSystem*)LoadSubSystem(_T("Script"));
            if (m_lpScriptSystem)
            {
                if (!m_lpScriptSystem->vCreate())
                    return LYNX_FALSE;
                m_lpScriptSystem->vInit();						
            }
			
            //Create GUI system		
            LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create GUI systenm\r\n")));
            m_lpGUISystem = LYNXNEW GUISystem::CGUISystem(this);			
            if (!m_lpGUISystem->vCreate())
                return LYNX_FALSE;
            
			// Create graphics system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Graphics systenm\r\n")));
			m_lpGraphicsSystem = LYNXNEW GraphicsSystem::Android::CGraphicsSystem(this); 
			if (desc)
			{
				if (!m_lpGraphicsSystem->vCreate(desc->GraphicParam.RendererType))
					return LYNX_FALSE;	
			}
			else 
			{
				if (!m_lpGraphicsSystem->vCreate())
					return LYNX_FALSE;						
			}	
			
			// Create sound system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create sound systenm\r\n")));
			#ifdef __OPENAL__				
				//m_lpSoundSystem = LYNXNEW LynxSound::AVFoundation::CSoundSystem(this);
				m_lpSoundSystem = LYNXNEW LynxSound::OpenAL::CSoundSystem(this);
			#elif __OPENSL__
				m_lpSoundSystem = LYNXNEW LynxSound::OpenSL::CSoundSystem(this);
			#endif
			if (!m_lpSoundSystem->vCreate())
				return LYNX_FALSE;			

			//Create scene system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create scene systenm\r\n")));
			m_lpSceneSystem = LYNXNEW SceneSystem::CSceneSystem(this);			
			if (!m_lpSceneSystem->vCreate())
				return LYNX_FALSE;						

			//Create collision system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create collision systenm\r\n")));
			m_lpCollisionSystem = LYNXNEW CollisionSystem::CBtCollisionSystem(this);			
			if (!m_lpCollisionSystem->vCreate())
				return LYNX_FALSE;						
		
			// Create physics system
            //LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load physics subsystem <"))+CString(PhysicsDLL)+CString(_T(">\r\n")));
            LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create physics systenm\r\n")));
            m_lpPhysicsSystem = (PhysicsSystem::CPhysicsSystem*)LoadSubSystem(_T("Physics"));//PhysicsDLL);		
            if (m_lpPhysicsSystem)
            {
                if (!m_lpPhysicsSystem->vCreate())
                    return LYNX_FALSE;
            }
			
			// Create game console system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create game console systenm\r\n")));
			m_lpGameConsoleSystem = LYNXNEW GameConsoleSystem::CGameConsoleSystem(this);	
			if (m_lpGameConsoleSystem)
			{
				if (!m_lpGameConsoleSystem->vCreate())
					return LYNX_FALSE;
			}

			// Create input system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create input systenm\r\n")));
			m_lpInputSystem = LYNXNEW InputSystem::CInputSystem(this);					
			if (m_lpInputSystem)
			{
				if (!m_lpInputSystem->vCreate())
					return LYNX_FALSE;
			}

			// Create network system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create network systenm\r\n")));
			m_lpNetworkSystem = LYNXNEW NetworkSystem::CNetworkSystem(this);					
			if (!m_lpNetworkSystem->vCreate())
				return LYNX_FALSE;		

			/*
			// Create Ad system
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Ad systenm\r\n")));
			m_lpAdSystem = LYNXNEW AdSystem::CAdSystem(this);					
			if (!m_lpAdSystem->vCreate())
				return LYNX_FALSE;		
				*/

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CEngine::vCreate(GameFramework::CKernel::INIT_DESC* desc)	
		{
			LYNX_GUARD(CEngine::vCreate)				

			LYNXTIMEDATA Time;		
			LYNXCHAR LogStr[256];
			
			m_ConfigFilename = _T("../config/android_engine.ini");		

			lynxSprintf(LogStr, _T("LynxEngine Version: %d.%d.%d\r\n"), GetMajorVersion(), GetMinorVersion(), GetBuildVersion());									
			LYNX_LOG(glpLogger, CString(LogStr));

			lynxOSGetTime(&Time);
			lynxSprintf(LogStr, _T("%s::Start at %d.%02d.%02d %02d:%02d:%02d\r\n"), CString(GetClassName()).c_str(), Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);									
			CString Log(LogStr);
			LYNX_LOG(glpLogger, Log);
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Munge FPCW\r\n")));
			lynxMungeFPCW();		

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init Math System\r\n")));	
			lynxInitMathSystem();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init I/O System\r\n")));	
			lynxInitIOSystem();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init File System\r\n")));	
			lynxInitFileIOSystem();	

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init Memory System\r\n")));		
			lynxInitMemSystem();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init CRC table\r\n")));		
			lynxInitCRC();
			
			vRegisterDefaultResources();
			vRegisterDefaultContainers();		
			vRegisterDefaultShaderClasses();
			
			// Create all sub systems
			vCreateSubSystems(desc);
			
			// Create system font
			m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_L] = CFontPtr(LYNXNEW CTextureFont(this));
            m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_L]->SetName(SYSTEM_DEFAULT_TEXTURE_FONT_L);
			m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT] = CFontPtr(LYNXNEW CTextureFont(this));
            m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT]->SetName(SYSTEM_DEFAULT_TEXTURE_FONT);
			m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_S] = CFontPtr(LYNXNEW CTextureFont(this));
            m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_S]->SetName(SYSTEM_DEFAULT_TEXTURE_FONT_S);
            
            m_lpSystemFontString = LYNXNEW CTextureFontString(this);
            m_lpSystemFontString->SetName(_T("SystemFontString"));
            m_lpSystemTTFontString = LYNXNEW CTrueTypeFontString(this);
            m_lpSystemTTFontString->SetName(_T("SystemTTFontString"));
			
			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (LynxEngine::CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::vRegisterDefaultResources()
		{
			LynxEngine::CEngine::vRegisterDefaultResources();
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::vRegisterDefaultContainers()
		{
			REGISTER_CONTAINER(CContainer::REFERENCE,			LynxEngine::CreateReference);
			REGISTER_CONTAINER(CContainer::GEOMETRY,			LynxEngine::CreateCGeometry);
			REGISTER_CONTAINER(CContainer::CAMERA,				LynxEngine::CreateCamera);
			REGISTER_CONTAINER(CContainer::MODEL,				LynxEngine::CreateModel);
			REGISTER_CONTAINER(CContainer::SPRITE,				LynxEngine::CreateSprite);
			REGISTER_CONTAINER(CContainer::SPRITE3D,			LynxEngine::CreateSprite3D);
			REGISTER_CONTAINER(CContainer::SPRITE3D_CLUSTER,	LynxEngine::CreateSprite3DCluster);
			REGISTER_CONTAINER(CContainer::SPRITE_CLUSTER,		LynxEngine::CreateSpriteCluster);
			REGISTER_CONTAINER(CContainer::VOXEL_CLUSTER,		LynxEngine::CreateVoxelCluster);
			REGISTER_CONTAINER(CContainer::CLUSTER_SUB,         LynxEngine::CreateClusterSub);
			REGISTER_CONTAINER(CContainer::PEMITTER,			LynxEngine::CreatePEmitter);
			REGISTER_CONTAINER(CContainer::MESH,				LynxEngine::CreateMesh);
			REGISTER_CONTAINER(CContainer::BONE,				LynxEngine::CreateBone);
			REGISTER_CONTAINER(CContainer::TERRAIN,				LynxEngine::CreateTerrain);
			REGISTER_CONTAINER(CContainer::SKYDOME,				LynxEngine::CreateSkyDome);
			REGISTER_CONTAINER(CContainer::GRASS,				LynxEngine::CreateGrass);
			REGISTER_CONTAINER(CContainer::CROSS_GRASS,			LynxEngine::CreateCrossGrass);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::vRegisterDefaultVertexLayouts()
		{
			LynxEngine::CEngine::vRegisterDefaultVertexLayouts();				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::vRegisterDefaultShaderClasses()
		{
			//Custom shader
            REGISTER_SHADERCLASS(CustomVS)
            REGISTER_SHADERCLASS(CustomPS)
            
            //Font string
            REGISTER_SHADERCLASS(FontStringVS)
            
            REGISTER_SHADERCLASS(ScreenQuadVS)
            REGISTER_SHADERCLASS(ScreenQuadUVOffsetVS)
            REGISTER_SHADERCLASS(ScreenQuadBlurTexCoordVS)
            REGISTER_SHADERCLASS(ScreenDepthQuadVS)
            REGISTER_SHADERCLASS(BltPS)
            REGISTER_SHADERCLASS(DiffuseMapPS)
            REGISTER_SHADERCLASS(AnimationDiffuseMapPS)
            REGISTER_SHADERCLASS(ColorPS)
            REGISTER_SHADERCLASS(ConstantColorPS)
            REGISTER_SHADERCLASS(TintPS)
			REGISTER_SHADERCLASS(GrayBltPS)
            REGISTER_SHADERCLASS(DiffuseMapXColorPS)			
			REGISTER_SHADERCLASS(DiffuseMapXAlphaPS)
            REGISTER_SHADERCLASS(DiffuseMapXDiffuseColorPS)
            REGISTER_SHADERCLASS(OneChannelBltPS)
            REGISTER_SHADERCLASS(AddTwoChannelPS)
            REGISTER_SHADERCLASS(BoxBlurPS)
            REGISTER_SHADERCLASS(BoxBlurWithEdgeCheckPS)
            REGISTER_SHADERCLASS(GaussinBlurPS)
			REGISTER_SHADERCLASS(RadialBlurPS)
			REGISTER_SHADERCLASS(HaloRadialBlurPS)
			REGISTER_SHADERCLASS(DirectionalBlurPS)
			REGISTER_SHADERCLASS(VelocityMapBlurPS)
            REGISTER_SHADERCLASS(BilateralBlurPS)
            REGISTER_SHADERCLASS(DistortionPS)
			REGISTER_SHADERCLASS(SimpleDistortionPS)
            
			REGISTER_DIFFUSEMAPXCOLORBLENDPS(CAnsiString("DiffuseMapXLinearDodgePS"), TRUE)
			REGISTER_DIFFUSEMAPXCOLORBLENDPS(CAnsiString("DiffuseMapXColorDodgePS"), FALSE)

            // Simple Pass
            REGISTER_SHADERCLASS(SimplePassPS)
            
            // forward shading shader classes
            REGISTER_SHADERCLASS(AmbientLightingPS)
            REGISTER_SHADERCLASS(SpotLightingPS)
            REGISTER_SHADERCLASS(PointLightingPS)
            REGISTER_SHADERCLASS(DirectionalLightingPS)
            REGISTER_SHADERCLASS(SimpleLightingPS)
            REGISTER_SHADERCLASS(OneLightingPS)
			REGISTER_SHADERCLASS(OneLightingNoShadowPS)
			REGISTER_SHADERCLASS(DiffuseMapXShadowPS)
            
            // Deferred shading shader class
            REGISTER_SHADERCLASS(DeferredAmbientLightingPS)
            REGISTER_SHADERCLASS(DeferredDirectionalLightingPS)
            REGISTER_SHADERCLASS(DeferredSpotLightingPS)
            REGISTER_SHADERCLASS(DeferredPointLightingPS)
            REGISTER_SHADERCLASS(GPassPS)
            
            // Post processing shader class
            REGISTER_SHADERCLASS(PostAdaptLuminancePS)
            REGISTER_SHADERCLASS(PostAverageLuminancePS)
            REGISTER_SHADERCLASS(PostBrightPassPS)
            REGISTER_SHADERCLASS(PostComputeLuminancePS)
            REGISTER_SHADERCLASS(PostGetLuminancePS)
            REGISTER_SHADERCLASS(PostToneMappingPS)
            REGISTER_SHADERCLASS(PostMotionBlurPS)
            REGISTER_SHADERCLASS(PostDOFPS)
            REGISTER_SHADERCLASS(PostSSAOPS)
            REGISTER_SHADERCLASS(PostSSAOLQPS)
			REGISTER_SHADERCLASS(PostScanLineNoisePS)
            
            REGISTER_SHADERCLASS(ProxyVS)
            REGISTER_SHADERCLASS(ProxyPS)
            
            //
            REGISTER_SHADERCLASS(ShadowMapPS)
            REGISTER_SHADERCLASS(ShadowMap_AlphaTestPS)
            REGISTER_SHADERCLASS(ZPass_VelocityPS)
            REGISTER_SHADERCLASS(ZPass_Velocity_AlphaTestPS)
            
            // Containers 
            REGISTER_SHADERCLASS(GeometryVS)
			REGISTER_SHADERCLASS(GeometryWithViewDirVS)
            REGISTER_SHADERCLASS(GeometryDiffuseVS)
            REGISTER_SHADERCLASS(GeometryLightmapVS)
            REGISTER_SHADERCLASS(GeometryZPassVS)
            REGISTER_SHADERCLASS(GeometryGPassVS)
            REGISTER_SHADERCLASS(GeometryShadowMapVS)
            REGISTER_SHADERCLASS(GeometrySimpleLightingVS)
            REGISTER_SHADERCLASS(GeometryLightingVS)
            REGISTER_SHADERCLASS(GeometryOneLightingVS)
            
            REGISTER_SHADERCLASS(RigidModelVS)
			REGISTER_SHADERCLASS(RigidModelWithViewDirVS)
            REGISTER_SHADERCLASS(RigidModelZPassVS)
            REGISTER_SHADERCLASS(RigidModelGPassVS)
            REGISTER_SHADERCLASS(RigidModelShadowMapVS)
            REGISTER_SHADERCLASS(RigidModelLightingVS)
            REGISTER_SHADERCLASS(RigidModelOneLightingVS)
            
            REGISTER_SHADERCLASS(SkinningModelVS)
            REGISTER_SHADERCLASS(SkinningModelZPassVS)
            REGISTER_SHADERCLASS(SkinningModelGPassVS)
            REGISTER_SHADERCLASS(SkinningModelShadowMapVS)
            REGISTER_SHADERCLASS(SkinningModelSimpleLightingVS)
            REGISTER_SHADERCLASS(SkinningModelLightingVS)
            REGISTER_SHADERCLASS(SkinningModelOneLightingVS)
            
            REGISTER_SHADERCLASS(SpriteVS)
            
            REGISTER_SHADERCLASS(Sprite3DVS)
            REGISTER_SHADERCLASS(Sprite3DZPassVS)
            REGISTER_SHADERCLASS(Sprite3DShadowMapVS)
            
            REGISTER_SHADERCLASS(Sprite3DClusterVS)
            REGISTER_SHADERCLASS(Sprite3DClusterZPassVS)
            REGISTER_SHADERCLASS(Sprite3DClusterGPassVS)
            REGISTER_SHADERCLASS(Sprite3DClusterGPassPS)
            REGISTER_SHADERCLASS(Sprite3DClusterShadowMapVS)
            
            REGISTER_SHADERCLASS(SpriteClusterVS)
            
            REGISTER_SHADERCLASS(VoxelClusterVS)
			REGISTER_SHADERCLASS(VoxelClusterShadowMapVS)
            REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingVS"), FALSE, FALSE, FALSE)
            REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingSpecularVS"), FALSE, FALSE, TRUE)
            REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOVS"), TRUE, FALSE, FALSE)
            REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOSpecularVS"), TRUE, FALSE, TRUE)
            REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOLevel2SpecularVS"), TRUE, TRUE, TRUE)
            REGISTER_SHADERCLASS(VoxelSpriteVS)
            REGISTER_SHADERCLASS(VoxelSpriteOneLightingVS)
            REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorPS"), FALSE, FALSE)
            REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorFakeAOPS"), TRUE, FALSE)
            REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorFakeAOLevel2PS"), TRUE, TRUE)
            /*
            REGISTER_SHADERCLASS(VoxelClusterVS)
            REGISTER_SHADERCLASS(VoxelClusterOneLightingVS)
            REGISTER_SHADERCLASS(VoxelClusterOneLightingSpecularVS)
            REGISTER_SHADERCLASS(VoxelClusterOneLightingFakeAOVS)
            REGISTER_SHADERCLASS(VoxelClusterOneLightingFakeAOSpecularVS)
            REGISTER_SHADERCLASS(VoxelClusterOneLightingFakeAOLevel2SpecularVS)		
            REGISTER_SHADERCLASS(VoxelColorPS)
            REGISTER_SHADERCLASS(VoxelColorFakeAOPS)
            REGISTER_SHADERCLASS(VoxelColorFakeAOLevel2PS)	
            */
            
            REGISTER_SHADERCLASS(SpriteGrassVS)
            REGISTER_SHADERCLASS(SpriteGrassZPassVS)
            REGISTER_SHADERCLASS(SpriteGrassGPassVS)
            REGISTER_SHADERCLASS(SpriteGrassGPassPS)
            REGISTER_SHADERCLASS(SpriteGrassShadowMapVS)
            REGISTER_SHADERCLASS(SpriteGrassFadeOutAmbLightingPS)
            REGISTER_SHADERCLASS(SpriteGrassFadeOutDirLightingPS)
            
            REGISTER_SHADERCLASS(CrossSpriteGrassVS)
            REGISTER_SHADERCLASS(CrossSpriteGrassZPassVS)
            REGISTER_SHADERCLASS(CrossSpriteGrassGPassVS)
            REGISTER_SHADERCLASS(CrossSpriteGrassShadowMapVS)
            REGISTER_SHADERCLASS(CrossSpriteGrassFadeOutVS)
            
            REGISTER_SHADERCLASS(TerrainVS)
            REGISTER_SHADERCLASS(TerrainSimplePS)
            REGISTER_SHADERCLASS(TerrainZPassVS)
            REGISTER_SHADERCLASS(TerrainGPassVS)
            REGISTER_SHADERCLASS(TerrainGPassPS)
            REGISTER_SHADERCLASS(TerrainShadowMapVS)
            REGISTER_SHADERCLASS(TerrainSimpleLightingVS)
            REGISTER_SHADERCLASS(TerrainSimpleLightingPS)
            
            REGISTER_SHADERCLASS(SkyDomeVS)
            REGISTER_SHADERCLASS(SkyDomePS)
            REGISTER_SHADERCLASS(SkyLightPS)
            
            REGISTER_SHADERCLASS(SkyBoxVS)			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::vInitSystemFont()
		{
			return;

			/*
			 int	CharacterStart[] = {0, 58};    
			int NumCharacter[] = {58, 36};		
			int	OffsetX[] = {0, 0};					
			int	OffsetY[] = {0, 32}; 
			int	CharacterWidth[][128] = {                                                     
				{12,11,19,18,22,19,6,10,11,13,14,8,12,9,10,20,17,19,19,18,20,18,19,18,18,11,9,15,14,13,15,21,21,19,20,18,18,17,22,19,10,18,19,16,21,20,22,19,21,18,17,21,21,21,24,19,20,18}, 
				{12,10,11,13,17,9,18,15,14,16,15,11,15,15,7,10,14,6,20,15,15,15,15,11,12,12,15,14,16,14,13,12,9,8,9,12}
			};
			*/
			
			int	CharacterStart[] = {0, 55};    
			int NumCharacter[] = {55, 39};		
			int	OffsetX[] = {0, 0};					
			int	OffsetY[] = {0, 32}; 
			int	CharacterWidth[][128] = {      
				{11,13,21,19,24,20,8,11,13,14,16,10,13,10,12,21,17,22,19,21,20,20,20,20,22,10,11,16,16,16,18,   23,23/*A*/,       19,22,20,19,19,23,21,12,19,22,17,22,21,24,20,22,20,19,22,23,23,26}, 
				{20,22,19,11,13,11,15,18,12,18,17,15,18,16,13,17,16,8,13,15,8,22,17,17,16,17,13,14,14,16,15,18,15,16,13,11,8,12,14}
			};		
			
			static_cast<CTextureFont*>((CFont*)m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT])->LoadVar(_T("../texture/iphone_font_with_shadow.png"), 2, Math::CReal(1.0f)*((float)(m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth())/480.0f), CharacterStart, NumCharacter, OffsetX, OffsetY, CharacterWidth, 32);
			static_cast<CTextureFont*>((CFont*)m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_S])->LoadVar(_T("../texture/iphone_font_with_shadow.png"), 2, Math::CReal(0.6f)*((float)(m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth()) / 480.0f), CharacterStart, NumCharacter, OffsetX, OffsetY, CharacterWidth, 32);
			m_lpSystemFontString->SetFont(m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT]);
			m_lpSystemFontString->Create(512, LYNX_TRUE);
			static_cast<CTextureFont*>((CFont*)m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT])->GetlpMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
			m_lpSystemTTFontString->Create(16);
			
			//m_lpSystemSmallFontString->SetFont(m_lpSystemSmallFont);		
			//m_lpSystemSmallFontString->Create(512, LYNX_TRUE);	
			//m_lpSystemSmallFont->GetlpMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CEngine::vInit(LYNXWINDOW &win, const int w, const int h)
		{			
			CObj::vInit();
		
			lynxSrand(lynxOSGetPerformanceCounter());

			if (m_lpPerfSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm...\r\n")));
				if (!m_lpPerfSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm OK\r\n")));
			}

			if (m_lpNetworkSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm...\r\n")));
				if (!m_lpNetworkSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm OK\r\n")));
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm...\r\n")));
			if (m_lpGUISystem)
				m_lpGUISystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm OK\r\n")));

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init graphics systenm...\r\n")));
			if (!m_lpGraphicsSystem->vInit(win, w, h))
				return LYNX_FALSE;				
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init graphics systenm OK\r\n")));

			if (m_lpPhysicsSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm...\r\n")));
				if (!m_lpPhysicsSystem->vInit())
				{
					UnloadSubSystem(m_lpPhysicsSystem);
					m_lpPhysicsSystem = NULL;
				}			
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm OK\r\n")));
			}	

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm...\r\n")));
			if (m_lpGameConsoleSystem)
				m_lpGameConsoleSystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm OK\r\n")));

			if (m_lpInputSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm...\r\n")));
				m_lpInputSystem->vInit();			
				#ifdef __DESKTOP__
					m_lpSystemKeyboard	= (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);				
					m_lpSystemMouse		= (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);	
					m_lpSystemKeyboard->SetName(_T("System Keyboard"));
					m_lpSystemMouse->SetName(_T("System Mouse"));
				#elif defined(__MOBILE__)
					m_lpSystemTouch		= (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);					
					m_lpSystemTouch->SetName(_T("System Touch"));
				#endif
				m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
				if (m_lpSystemInputRecord)
					m_lpSystemInputRecord->SetName(_T("System Input Record"));
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm OK\r\n")));
			}

			if (m_lpSoundSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm...\r\n")));
				m_lpSoundSystem->vInit();						
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm OK\r\n")));
			}
		
			vInitSystemFont();		
			//vInitLogoSprite();
			//vInitGameScripts();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file...\r\n")));
			LoadConfig(m_ConfigFilename);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file OK\r\n")));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CEngine::vInit(LYNXWINDOW &win, GraphicsSystem::CRenderer::INITGRAPHICPARAM* lpcp)
		{		
			CObj::vInit();

			lynxSrand(lynxOSGetPerformanceCounter());

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm...\r\n")));
			if (m_lpGUISystem)
				m_lpGUISystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm OK\r\n")));

			if (!m_lpGraphicsSystem->vInit(win, lpcp))
				return LYNX_FALSE;

			if (m_lpPerfSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm...\r\n")));
				if (!m_lpPerfSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm OK\r\n")));
			}
		
			if (m_lpNetworkSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm...\r\n")));
				if (!m_lpNetworkSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm OK\r\n")));
			}

			if (m_lpPhysicsSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm...\r\n")));
				if (!m_lpPhysicsSystem->vInit())
				{
					UnloadSubSystem(m_lpPhysicsSystem);
					m_lpPhysicsSystem = NULL;
				}			
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm OK\r\n")));
			}		

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm...\r\n")));
			if (m_lpGameConsoleSystem)
				m_lpGameConsoleSystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm OK\r\n")));

			if (m_lpInputSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm...\r\n")));
				m_lpInputSystem->vInit();			
				#ifdef __DESKTOP__
					m_lpSystemKeyboard	= (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);				
					m_lpSystemMouse		= (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);	
					m_lpSystemKeyboard->SetName(_T("System Keyboard"));
					m_lpSystemMouse->SetName(_T("System Mouse"));
				#elif defined(__MOBILE__)
					m_lpSystemTouch		= (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);					
					m_lpSystemTouch->SetName(_T("System Touch"));
				#endif
				m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
				if (m_lpSystemInputRecord)
					m_lpSystemInputRecord->SetName(_T("System Input Record"));
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm OK\r\n")));
			}

			if (m_lpSoundSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm...\r\n")));
				m_lpSoundSystem->vInit();						
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm OK\r\n")));
			}
		
			vInitSystemFont();		
			//vInitLogoSprite();
			//vInitGameScripts();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file...\r\n")));
			LoadConfig(m_ConfigFilename);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file OK\r\n")));
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CEngine::vInit(LYNXWINDOW &win, INIT_DESC& desc)
		{
			GraphicsSystem::CRenderer::INITGRAPHICPARAM GraphicParam;				
			char							Buffer[128];

			CObj::vInit();

			lynxSrand(lynxOSGetPerformanceCounter());

			GraphicParam.Width = desc.Width;
			GraphicParam.Height = desc.Height;
			GraphicParam.DepthBufferFormat = desc.ZBufferFormat;
			GraphicParam.bHardware = desc.bHardware;
			GraphicParam.bFullScreen = desc.bFullScreen;
			GraphicParam.ShaderDebuggingFlag = desc.ShaderDebuggingFlag;
			GraphicParam.NumMultiSamples = desc.NumMultiSamples;
		
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm...\r\n")));
			if (m_lpGUISystem)
				m_lpGUISystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init GUI systenm OK\r\n")));

			if (!m_lpGraphicsSystem->vInit(win, &GraphicParam))
				return LYNX_FALSE;

			if (m_lpPerfSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm...\r\n")));
				if (!m_lpPerfSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init perf systenm OK\r\n")));
			}
		
			if (m_lpNetworkSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm...\r\n")));
				if (!m_lpNetworkSystem->vInit())
					return LYNX_FALSE;		
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init network systenm OK\r\n")));
			}

			if (m_lpPhysicsSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm...\r\n")));
				if (!m_lpPhysicsSystem->vInit())
				{
					UnloadSubSystem(m_lpPhysicsSystem);
					m_lpPhysicsSystem = NULL;
				}			
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init physics systenm OK\r\n")));
			}		

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm...\r\n")));
			if (m_lpGameConsoleSystem)
				m_lpGameConsoleSystem->vInit();			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init console systenm OK\r\n")));

			if (m_lpInputSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm...\r\n")));
				m_lpInputSystem->vInit();			
				#ifdef __DESKTOP__
					m_lpSystemKeyboard	= (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);				
					m_lpSystemMouse		= (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);	
					m_lpSystemKeyboard->SetName(_T("System Keyboard"));
					m_lpSystemMouse->SetName(_T("System Mouse"));
				#elif defined(__MOBILE__)
					m_lpSystemTouch		= (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);					
					m_lpSystemTouch->SetName(_T("System Touch"));
				#endif
				m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
				if (m_lpSystemInputRecord)
					m_lpSystemInputRecord->SetName(_T("System Input Record"));
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init input systenm OK\r\n")));
			}

			if (m_lpSoundSystem)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm...\r\n")));
				m_lpSoundSystem->vInit();						
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Init sound systenm OK\r\n")));
			}
		
			vInitSystemFont();				
			//vInitGameScripts();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file...\r\n")));
			LoadConfig(m_ConfigFilename);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load confog file OK\r\n")));

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LynxEngine::CVideo* CEngine::vCreateVideo(const LynxEngine::CCreationDesc* desc) 
		{
			//LynxEngine::CVideo* lpVideo = (LYNXNEW iOS::CVideo(this)); 
			//lpVideo->vCreate(desc);
			
			//return lpVideo;
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CEngine::vReleaseVideo(const LynxEngine::CVideo* video) 
		{
			LYNXDEL video; 
		}
	}
}

#endif