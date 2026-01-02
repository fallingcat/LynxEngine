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
#include <Version.info>
#include <json/json.h>
#include <LynxIniReader.h>
#include <LynxEngine.h>
#include <LynxFileStream.h>
#include <LynxMemoryStream.h>
#include <LynxLogger.h>
#include <LynxDebugMemAllocator.h>
#include <LynxScreenOutputDevice.h>
#include <Font/LynxTextureFont.h>
#include <Font/LynxTextureFontString.h>
#include <Font/LynxTrueTypeFont.h>
#include <Font/LynxTrueTypeFontString.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxSamplerState.h>
#include <GraphicsSystem/LynxToneMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>

#include <PerfSystem/LynxPerfSystem.h>
#include <FileSystem/LynxFileSystem.h>
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
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <InputSystem/LynxGamePad.h>
#include <SoundSystem/LynxSoundSystem.h>
#include <CollisionSystem/LynxCollisionSystem.h>
#include <CollisionSystem/Bullet/LynxBtCollisionSystem.h>
#include <NetworkSystem/LynxNetworkSystem.h>

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
#include <Shader/LynxGrayBltPS.h>
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
#include <Shader/LynxHaloRadialBlurPS.h>
#include <Shader/LynxDirectionalBlurPS.h>
#include <Shader/LynxVelocityMapBlurPS.h>
#include <Shader/LynxBilateralBlurPS.h>
#include <Shader/LynxDistortionPS.h>
#include <Shader/LynxSimpleDistortionPS.h>
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
#include <Shader/LynxGeometryWithViewDirVS.h>
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
#include <Shader/LynxRigidModelWithViewDirVS.h>
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
#include <Shader/LynxPostScanLineNoisePS.h>
#include <Shader/LynxTextureAnimationPreviewVS.h>
#include <Shader/LynxTextureAnimationPreviewPS.h>
#include <Shader/LynxVoxelClusterVS.h>
#include <Shader/LynxVoxelClusterOneLightingVS.h>
#include <Shader/LynxVoxelSpriteVS.h>
#include <Shader/LynxVoxelSpriteOneLightingVS.h>
#include <Shader/LynxVoxelColorPS.h>
#include <Shader/LynxVoxelClusterShadowMapVS.h>

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

/*
#ifdef __TOOL__
void* LynxEngine_Alloc(size_t s) {return lynxMemAlloc(s); };
void* LynxEngine_Alloc(size_t s, const char *file, const char *func, int line) {return lynxMemAlloc_d(s, file, func, line); };
void LynxEngine_Free(void* p) {return lynxMemFree_d(p); };
void LynxEngine_Free(void* p, size_t s) {return lynxMemFree_d(p); };
void LynxEngine_Free(void* p, const char *file, const char *func, int line) {return lynxMemFree_d(p); };
#else
//void* LynxEngine_Alloc(size_t s) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vAlloc(s):lynxMemAlloc(s); };
//void* LynxEngine_Alloc(size_t s, const char *file, const char *func, int line) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vAlloc(s, file, func, line):lynxMemAlloc_d(s, file, func, line); };

void* LynxEngine_Alloc(size_t s) {return lynxMemAlloc(s); };
void* LynxEngine_Alloc(size_t s, const char *file, const char *func, int line) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vAlloc(s, file, func, line):lynxMemAlloc(s); };
void LynxEngine_Free(void* p) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vFree(p):lynxMemFree(p); };
void LynxEngine_Free(void* p, size_t s) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vFree(p, s):lynxMemFree(p); };
void LynxEngine_Free(void* p, const char *file, const char *func, int line) {return (glpKernel->GetMemAllocator())?glpKernel->GetMemAllocator()->vFree(p):lynxMemFree_d(p); };
void LynxEngine_RemoveRecord(void* p) { if (glpKernel->GetMemAllocator()) glpKernel->GetMemAllocator()->vRemoveRecord(p); };
#endif
*/

namespace LynxEngine
{
#ifdef __CALL_STACK__
	CStack<CAnsiString>						gCallStack;
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------v
	LYNXENGCLASS void AddToCallStack(const LYNXANSICHAR* s) { gCallStack.push(CAnsiString(s)); }
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------v
	LYNXENGCLASS CString GetCallStackString()
	{
		CAnsiString Str = "";

		while (!gCallStack.empty())
		{
			Str += " -> ";
			Str += (gCallStack.top());
			Str += "()";
			gCallStack.pop();
		}
		return Str;
	}
#endif				
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------v
	CResource* _CreateBuffer(CEngine* e)			{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateBuffer(); }
	CResource* _CreateVertexLayout(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout(); }
	CResource* _CreateVertexArray(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexArray(); }
	CResource* _CreateIndexArray(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateIndexArray(); }
	CResource* _CreateVertexShader(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexShader(); }
	CResource* _CreatePixelShader(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreatePixelShader(); }
	CResource* _CreateShaderGroup(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateShaderGroup(); }
	CResource* _CreateTexture(CEngine* e)			{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateTexture(); }
	CResource* _CreateDepthStencilState(CEngine* e)	{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateDepthStencilState(); }
	CResource* _CreateBlendState(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateBlendState(); }
	CResource* _CreateRasterizerState(CEngine* e)	{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateRasterizerState(); }
	CResource* _CreateSamplerState(CEngine* e)		{ return e->GetlpGraphicsSystem()->GetlpRenderer()->CreateSamplerState(); }
	CResource* _CreateScript(CEngine* e)			{ return e->GetlpScriptSystem()->GetlpVMachine()->vCreateScript(); }
	CResource* _CreateMusic(CEngine* e)				{ return e->GetlpSoundSystem()->vCreateMusic(); }
	CResource* _CreateSound(CEngine* e)				{ return e->GetlpSoundSystem()->vCreateSound(); }
	CResource* _CreateTextureAnimation(CEngine* e)	{ return LYNXNEW MaterialSystem::CTextureAnimation(e->GetlpGraphicsSystem()); }
	CResource* _CreateMaterial(CEngine* e)			{ return LYNXNEW MaterialSystem::CMaterial(e->GetlpGraphicsSystem()->GetlpMaterialSystem()); }
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CContainer* CreateReference(CEngine* e)			{ return LYNXNEW CReferenceContainer(e); }
	CContainer*	CreateCGeometry(CEngine* e)			{ return LYNXNEW CGeometryContainer(e); }
	CContainer*	CreateTerrain(CEngine* e)			{ return LYNXNEW CTerrainContainer(e); }
	CContainer*	CreateCamera(CEngine* e)			{ return LYNXNEW CCameraContainer(e); }
	CContainer*	CreateModel(CEngine* e)				{ return LYNXNEW CModelContainer(e); }
	CContainer*	CreateSprite(CEngine* e)			{ return LYNXNEW CSpriteContainer(e); }
	CContainer*	CreateSprite3D(CEngine* e)			{ return LYNXNEW CSprite3DContainer(e); }
	CContainer*	CreateSprite3DCluster(CEngine* e)	{ return LYNXNEW CSprite3DClusterContainer(e); }
	CContainer*	CreateSpriteCluster(CEngine* e)		{ return LYNXNEW CSpriteClusterContainer(e); }
	CContainer*	CreateVoxelCluster(CEngine* e)		{ return LYNXNEW CVoxelClusterContainer(e); }
	CContainer*	CreateClusterSub(CEngine* e)		{ return LYNXNEW CClusterSubContainer(e); }
	CContainer*	CreatePEmitter(CEngine* e)			{ return LYNXNEW CPEmitterContainer(e); }
	CContainer*	CreateMesh(CEngine* e)				{ return LYNXNEW CMeshContainer(e); }
	CContainer*	CreateBone(CEngine* e)				{ return LYNXNEW CBoneContainer(e); }
	CContainer*	CreateGrass(CEngine* e)				{ return LYNXNEW CSpriteGrassContainer(e); }
	CContainer*	CreateCrossGrass(CEngine* e)		{ return LYNXNEW CCrossSpriteGrassContainer(e); }
	CContainer*	CreateSkyDome(CEngine* e)			{ return LYNXNEW CSkyDomeContainer(e); }
	CContainer*	CreateVolumetricCloud(CEngine* e)	{ return LYNXNEW CVolumetricCloudContainer(e); }

#if (defined(__WIN32__) && defined(__SPEEDTREE__))
	//temp code
	CContainer*	CreateSpeedTree(CEngine* e)			{ return LYNXNEW CSpeedTreeContainer(e); }
#endif 
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	GraphicsSystem::CVertexLayout* CreateFixedVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[12];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space 
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_NORMAL;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Normal in object space		
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_UBYTE;	Slots[2].Number = 4;  // Diffuse color 
		Slots[3].Stream = 0;	Slots[3].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[3].TypeIndex = 1;	Slots[3].Format = GraphicsSystem::CRenderer::VSF_UBYTE;	Slots[3].Number = 4;  // Specular color 
		Slots[4].Stream = 0;	Slots[4].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[4].TypeIndex = 0;	Slots[4].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[4].Number = 2;  // Texture coordinate 0 
		Slots[5].Stream = 0;	Slots[5].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[5].TypeIndex = 1;	Slots[5].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[5].Number = 2;  // Texture coordinate 1 
		Slots[6].Stream = 0;	Slots[6].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[6].TypeIndex = 2;	Slots[6].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[6].Number = 2;  // Texture coordinate 2 
		Slots[7].Stream = 0;	Slots[7].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[7].TypeIndex = 3;	Slots[7].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[7].Number = 2;  // Texture coordinate 3 
		Slots[8].Stream = 0;	Slots[8].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[8].TypeIndex = 4;	Slots[8].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[8].Number = 2;  // Texture coordinate 4 
		Slots[9].Stream = 0;	Slots[9].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[9].TypeIndex = 5;	Slots[9].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[9].Number = 2;  // Texture coordinate 5 
		Slots[10].Stream = 0;	Slots[10].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[10].TypeIndex = 6;	Slots[10].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[10].Number = 2;  // Texture coordinate 6 
		Slots[11].Stream = 0;	Slots[11].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[11].TypeIndex = 7;	Slots[11].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[11].Number = 2;  // Texture coordinate 7 
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 12, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_FIXED));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateVector3VertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[1];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 1, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_PROXY));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateScreenQuadVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[3];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Position in object space
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 2;  // Texture coordinate 0		
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_BLEND_INDICES;	Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 1;  // Vertex index		
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SCREEN_QUAD));

		return v;
	}
	/*
	GraphicsSystem::CVertexLayout* CreateScreenDepthQuadVertexLayout(CEngine* e)
	{
	GraphicsSystem::CRenderer::VERTEXSLOT Slots[4];

	Slots[0].Stream	= 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Position in object space
	Slots[1].Stream	= 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 2;  // Texture coordinate 0
	Slots[2].Stream	= 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[2].TypeIndex = 1;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  /// Far Plane Corner Dir
	Slots[3].Stream	= 0;	Slots[3].Type = GraphicsSystem::CRenderer::VST_BLEND_INDICES;	Slots[3].TypeIndex = 0;	Slots[3].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[3].Number = 1;  // Vertex index
	GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
	v->vCreate(Slots, 4);

	return v;
	}
	*/

	GraphicsSystem::CVertexLayout* CreateFontVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[2];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;	Slots[0].TypeIndex = 0; Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT; Slots[0].Number = 2;
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;	Slots[1].TypeIndex = 0; Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT; Slots[1].Number = 1;
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 2, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_FONT));

		return v;
	}

	GraphicsSystem::CVertexLayout* CreateSpriteVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[3];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space, w component is the rotation degree
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 4;  // Diffuse color 
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 2;  // Texture coordinate 0 
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateSprite3DVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[3];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space, w component is the rotation degree
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_UBYTE;	Slots[1].Number = 4;  // Diffuse color 
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 2;  // Texture coordinate 0 		
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE3D));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateSprite3DClusterVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[2];

		//Slots[0].Stream	= 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Texture coordinate 0
		//Slots[1].Stream	= 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 1;  // Position Index
		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Texture coordinate 0
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 1, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE3D_CLUSTER));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateVoxelClusterVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[6];

#if __FAKE_VOXEL_AO__
		//Slots[0].Stream	= 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Texture coordinate 				
		//Slots[1].Stream	= 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[1].TypeIndex = 1;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 1;  // FaceID
		//Slots[2].Stream	= 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[2].TypeIndex = 2;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 1;  // Instance
		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 4;  // Texture coordinate, FaceID, Instance 				
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Local Position
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_NORMAL;			Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  // Normal				
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_VOXEL_CLUSTER));
#else
		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 2;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 1;  // Instance
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Local Position
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_NORMAL;			Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  // Normal		
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_VOXEL_CLUSTER));
#endif

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateStaticVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[5];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TANGENT;			Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Tangent in object space		
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_NORMAL;			Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  // Normal in object space		
		Slots[3].Stream = 0;	Slots[3].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[3].TypeIndex = 0;	Slots[3].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[3].Number = 4;  // Diffuse color 
		Slots[4].Stream = 0;	Slots[4].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[4].TypeIndex = 0;	Slots[4].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[4].Number = 2;  // Texture coordinate 0		
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 5, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_STATIC));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateRigidVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[5];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space 
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TANGENT;			Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Tangent in object space 
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_NORMAL;			Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  // Normal in object space 
		Slots[3].Stream = 0;	Slots[3].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[3].TypeIndex = 0;	Slots[3].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[3].Number = 4;  // Diffuse color 
		Slots[4].Stream = 0;	Slots[4].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[4].TypeIndex = 0;	Slots[4].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[4].Number = 2;  // Texture coordinate 0 
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 5, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_RIGID));

		return v;
	}
	GraphicsSystem::CVertexLayout* CreateBlendVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[10];

		Slots[0].Stream = 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 3;  // Position in object space
		Slots[1].Stream = 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[1].TypeIndex = 1;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 3;  // Position in object space
		Slots[2].Stream = 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[2].TypeIndex = 2;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 3;  // Position in object space
		Slots[3].Stream = 0;	Slots[3].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[3].TypeIndex = 3;	Slots[3].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[3].Number = 3;  // Position in object space
		Slots[4].Stream = 0;	Slots[4].Type = GraphicsSystem::CRenderer::VST_TANGENT;			Slots[4].TypeIndex = 0;	Slots[4].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[4].Number = 3;  // Tangent in object space
		Slots[5].Stream = 0;	Slots[5].Type = GraphicsSystem::CRenderer::VST_NORMAL;			Slots[5].TypeIndex = 0;	Slots[5].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[5].Number = 3;  // Normal in object space
		Slots[6].Stream = 0;	Slots[6].Type = GraphicsSystem::CRenderer::VST_COLOR;			Slots[6].TypeIndex = 0;	Slots[6].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[6].Number = 4;  // Diffuse color
		Slots[7].Stream = 0;	Slots[7].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[7].TypeIndex = 0;	Slots[7].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[7].Number = 2;  // Texture coordinate 0
		Slots[8].Stream = 0;	Slots[8].Type = GraphicsSystem::CRenderer::VST_BLEND_INDICES;	Slots[8].TypeIndex = 0;	Slots[8].Format = GraphicsSystem::CRenderer::VSF_SHORT;	Slots[8].Number = 4;  // Bone index
		Slots[9].Stream = 0;	Slots[9].Type = GraphicsSystem::CRenderer::VST_BLEND_WEIGHT;	Slots[9].TypeIndex = 0;	Slots[9].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[9].Number = 4;  // Weight		
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 10, GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_BLEND));

		return v;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	IMPLEMENT_SHADERCLASS(CustomVS)
		IMPLEMENT_SHADERCLASS(CustomPS)

		//IMPLEMENT_SHADERCLASS(FontStringVS)

		//IMPLEMENT_SHADERCLASS(BltPS)
		//IMPLEMENT_SHADERCLASS(ColorPS)
		IMPLEMENT_SHADERCLASS(ConstantColorPS)
		IMPLEMENT_SHADERCLASS(TintPS)
		IMPLEMENT_SHADERCLASS(DiffuseMapPS)
		IMPLEMENT_SHADERCLASS(AnimationDiffuseMapPS)
		IMPLEMENT_SHADERCLASS(DiffuseMapXColorPS)
		IMPLEMENT_SHADERCLASS(DiffuseMapXAlphaPS)
		IMPLEMENT_SHADERCLASS(DiffuseMapXDiffuseColorPS)
		IMPLEMENT_SHADERCLASS(OneChannelBltPS)
		IMPLEMENT_SHADERCLASS(AddTwoChannelPS)
		IMPLEMENT_SHADERCLASS(DistortionPS)
		IMPLEMENT_SHADERCLASS(SimpleDistortionPS)

		IMPLEMENT_SHADERCLASS(BoxBlurPS)
		IMPLEMENT_SHADERCLASS(BoxBlurWithEdgeCheckPS)
		IMPLEMENT_SHADERCLASS(GaussinBlurPS)
		IMPLEMENT_SHADERCLASS(RadialBlurPS)
		IMPLEMENT_SHADERCLASS(HaloRadialBlurPS)
		IMPLEMENT_SHADERCLASS(DirectionalBlurPS)
		IMPLEMENT_SHADERCLASS(VelocityMapBlurPS)
		IMPLEMENT_SHADERCLASS(BilateralBlurPS)
		IMPLEMENT_SHADERCLASS(ScreenQuadVS)
		IMPLEMENT_SHADERCLASS(ScreenQuadUVOffsetVS)
		IMPLEMENT_SHADERCLASS(ScreenQuadBlurTexCoordVS)
		IMPLEMENT_SHADERCLASS(ScreenDepthQuadVS)

		IMPLEMENT_SHADERCLASS(AmbientLightingPS)
		IMPLEMENT_SHADERCLASS(SpotLightingPS)
		IMPLEMENT_SHADERCLASS(PointLightingPS)
		IMPLEMENT_SHADERCLASS(DirectionalLightingPS)
		IMPLEMENT_SHADERCLASS(SimpleLightingPS)
		IMPLEMENT_SHADERCLASS(OneLightingPS)
		IMPLEMENT_SHADERCLASS(OneLightingNoShadowPS)
		IMPLEMENT_SHADERCLASS(DiffuseMapXShadowPS)

		IMPLEMENT_SHADERCLASS(DeferredAmbientLightingPS)
		IMPLEMENT_SHADERCLASS(DeferredDirectionalLightingPS)
		IMPLEMENT_SHADERCLASS(DeferredSpotLightingPS)
		IMPLEMENT_SHADERCLASS(DeferredPointLightingPS)
		IMPLEMENT_SHADERCLASS(GPassPS)

		IMPLEMENT_SHADERCLASS(SimplePassPS)

		IMPLEMENT_SHADERCLASS(PostAdaptLuminancePS)
		IMPLEMENT_SHADERCLASS(PostAverageLuminancePS)
		IMPLEMENT_SHADERCLASS(PostBrightPassPS)
		IMPLEMENT_SHADERCLASS(PostComputeLuminancePS)
		IMPLEMENT_SHADERCLASS(PostGetLuminancePS)
		IMPLEMENT_SHADERCLASS(PostToneMappingPS)
		IMPLEMENT_SHADERCLASS(PostMotionBlurPS)
		IMPLEMENT_SHADERCLASS(PostDOFPS)
		IMPLEMENT_SHADERCLASS(PostSSAOPS)
		IMPLEMENT_SHADERCLASS(PostSSAOLQPS)
		IMPLEMENT_SHADERCLASS(PostScanLineNoisePS)

		IMPLEMENT_SHADERCLASS(ProxyVS)
		IMPLEMENT_SHADERCLASS(ProxyPS)

		IMPLEMENT_SHADERCLASS(ShadowMapPS)
		IMPLEMENT_SHADERCLASS(ShadowMap_AlphaTestPS)
		IMPLEMENT_SHADERCLASS(ZPass_VelocityPS)
		IMPLEMENT_SHADERCLASS(ZPass_Velocity_AlphaTestPS)

		// Containers
		IMPLEMENT_SHADERCLASS(GeometryVS)
		IMPLEMENT_SHADERCLASS(GeometryWithViewDirVS)
		IMPLEMENT_SHADERCLASS(GeometryDiffuseVS)
		IMPLEMENT_SHADERCLASS(GeometryLightmapVS)
		IMPLEMENT_SHADERCLASS(GeometryZPassVS)
		IMPLEMENT_SHADERCLASS(GeometryGPassVS)
		IMPLEMENT_SHADERCLASS(GeometryShadowMapVS)
		IMPLEMENT_SHADERCLASS(GeometrySimpleLightingVS)
		IMPLEMENT_SHADERCLASS(GeometryLightingVS)
		IMPLEMENT_SHADERCLASS(GeometryOneLightingVS)

		IMPLEMENT_SHADERCLASS(RigidModelVS)
		IMPLEMENT_SHADERCLASS(RigidModelWithViewDirVS)
		IMPLEMENT_SHADERCLASS(RigidModelZPassVS)
		IMPLEMENT_SHADERCLASS(RigidModelGPassVS)
		IMPLEMENT_SHADERCLASS(RigidModelShadowMapVS)
		IMPLEMENT_SHADERCLASS(RigidModelLightingVS)
		IMPLEMENT_SHADERCLASS(RigidModelOneLightingVS)

		IMPLEMENT_SHADERCLASS(SkinningModelVS)
		IMPLEMENT_SHADERCLASS(SkinningModelZPassVS)
		IMPLEMENT_SHADERCLASS(SkinningModelGPassVS)
		IMPLEMENT_SHADERCLASS(SkinningModelShadowMapVS)
		IMPLEMENT_SHADERCLASS(SkinningModelSimpleLightingVS)
		IMPLEMENT_SHADERCLASS(SkinningModelLightingVS)
		IMPLEMENT_SHADERCLASS(SkinningModelOneLightingVS)

		IMPLEMENT_SHADERCLASS(SpriteVS)

		IMPLEMENT_SHADERCLASS(Sprite3DVS)
		IMPLEMENT_SHADERCLASS(Sprite3DZPassVS)
		IMPLEMENT_SHADERCLASS(Sprite3DShadowMapVS)

		IMPLEMENT_SHADERCLASS(Sprite3DClusterVS)
		IMPLEMENT_SHADERCLASS(Sprite3DClusterZPassVS)
		IMPLEMENT_SHADERCLASS(Sprite3DClusterGPassVS)
		IMPLEMENT_SHADERCLASS(Sprite3DClusterGPassPS)
		IMPLEMENT_SHADERCLASS(Sprite3DClusterShadowMapVS)

		IMPLEMENT_SHADERCLASS(SpriteClusterVS)

		IMPLEMENT_SHADERCLASS(VoxelClusterVS)
		IMPLEMENT_SHADERCLASS(VoxelClusterShadowMapVS)
		IMPLEMENT_SHADERCLASS(VoxelSpriteVS)
		IMPLEMENT_SHADERCLASS(VoxelSpriteOneLightingVS)
		/*
		IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingVS)
		IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingSpecularVS)
		IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOVS)
		IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOSpecularVS)
		IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOLevel2SpecularVS)
		IMPLEMENT_SHADERCLASS(VoxelColorPS)
		IMPLEMENT_SHADERCLASS(VoxelColorFakeAOPS)
		IMPLEMENT_SHADERCLASS(VoxelColorFakeAOLevel2PS)
		*/

		IMPLEMENT_SHADERCLASS(SpriteGrassVS)
		IMPLEMENT_SHADERCLASS(SpriteGrassZPassVS)
		IMPLEMENT_SHADERCLASS(SpriteGrassGPassVS)
		IMPLEMENT_SHADERCLASS(SpriteGrassGPassPS)
		IMPLEMENT_SHADERCLASS(SpriteGrassShadowMapVS)
		IMPLEMENT_SHADERCLASS(SpriteGrassFadeOutAmbLightingPS)
		IMPLEMENT_SHADERCLASS(SpriteGrassFadeOutDirLightingPS)

		IMPLEMENT_SHADERCLASS(CrossSpriteGrassVS)
		IMPLEMENT_SHADERCLASS(CrossSpriteGrassZPassVS)
		IMPLEMENT_SHADERCLASS(CrossSpriteGrassGPassVS)
		IMPLEMENT_SHADERCLASS(CrossSpriteGrassShadowMapVS)
		IMPLEMENT_SHADERCLASS(CrossSpriteGrassFadeOutVS)

		IMPLEMENT_SHADERCLASS(TerrainVS)
		IMPLEMENT_SHADERCLASS(TerrainSimplePS)
		IMPLEMENT_SHADERCLASS(TerrainZPassVS)
		IMPLEMENT_SHADERCLASS(TerrainGPassVS)
		IMPLEMENT_SHADERCLASS(TerrainGPassPS)
		IMPLEMENT_SHADERCLASS(TerrainShadowMapVS)
		IMPLEMENT_SHADERCLASS(TerrainSimpleLightingVS)
		IMPLEMENT_SHADERCLASS(TerrainSimpleLightingPS)

		IMPLEMENT_SHADERCLASS(SkyDomeVS)
		IMPLEMENT_SHADERCLASS(SkyDomePS)
		IMPLEMENT_SHADERCLASS(SkyLightPS)

		IMPLEMENT_SHADERCLASS(SkyBoxVS)

#if (defined(__WIN32__) && defined(__SPEEDTREE__))
		GraphicsSystem::CShaderClass* CreateSpeedTreeBranchVS(CEngine* e)			{ return LYNXNEW Shader::CSpeedTreeBranchVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeBranchZPassVS(CEngine* e)		{ return LYNXNEW Shader::CSpeedTreeBranchZPassVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeBranchGPassVS(CEngine* e)		{ return LYNXNEW Shader::CSpeedTreeBranchGPassVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeBranchShadowMapVS(CEngine* e)	{ return LYNXNEW Shader::CSpeedTreeBranchShadowMapVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeBranchSimpleLightingVS(CEngine* e)	{ return LYNXNEW Shader::CSpeedTreeBranchSimpleLightingVS(e->GetlpGraphicsSystem()); }

	GraphicsSystem::CShaderClass* CreateSpeedTreeLeafVS(CEngine* e)				{ return LYNXNEW Shader::CSpeedTreeLeafVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeLeafZPassVS(CEngine* e)		{ return LYNXNEW Shader::CSpeedTreeLeafZPassVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeLeafGPassVS(CEngine* e)		{ return LYNXNEW Shader::CSpeedTreeLeafGPassVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeLeafShadowMapVS(CEngine* e)	{ return LYNXNEW Shader::CSpeedTreeLeafShadowMapVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateSpeedTreeLeafSimpleLightingVS(CEngine* e) { return LYNXNEW Shader::CSpeedTreeLeafSimpleLightingVS(e->GetlpGraphicsSystem()); }
#endif

	GraphicsSystem::CShaderClass* CreateTextureAnimationPreviewVS(CEngine* e)	{ return LYNXNEW Shader::CTextureAnimationPreviewVS(e->GetlpGraphicsSystem()); }
	GraphicsSystem::CShaderClass* CreateTextureAnimationPreviewPS(CEngine* e)	{ return LYNXNEW Shader::CTextureAnimationPreviewPS(e->GetlpGraphicsSystem()); }


	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::RENDERERTYPE e)
	{
		CString				RendererType;

		switch (e)
		{
		default:
		case GraphicsSystem::CRenderer::NULL_RENDERER:
			RendererType = _T("NULL");
			break;
		case GraphicsSystem::CRenderer::D3D9_RENDERER:
			RendererType = _T("D3D9");
			break;
		case GraphicsSystem::CRenderer::D3D11_RENDERER:
			RendererType = _T("D3D11");
			break;
		case GraphicsSystem::CRenderer::OGL_RENDERER:
			RendererType = _T("OpenGL");
			break;
		case GraphicsSystem::CRenderer::OGLES_RENDERER:
			RendererType = _T("OpenGL_ES");
			break;
		case GraphicsSystem::CRenderer::OGLES2_RENDERER:
			RendererType = _T("OpenGL_ES2");
			break;
		}
		return RendererType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(GraphicsSystem::CRenderer::RENDERDEVICETYPE e)
	{
		CString				RenderDeviceType;

		switch (e)
		{
		case GraphicsSystem::CRenderer::RDT_NULL:
			RenderDeviceType = _T("Null");
			break;
		case GraphicsSystem::CRenderer::RDT_REFERNCE:
			RenderDeviceType = _T("Reference");
			break;
		case GraphicsSystem::CRenderer::RDT_SOFTWARE:
			RenderDeviceType = _T("Software");
			break;
		case GraphicsSystem::CRenderer::RDT_HARDWARE:
			RenderDeviceType = _T("Hardware");
			break;
		case GraphicsSystem::CRenderer::RDT_DEFAULT:
			RenderDeviceType = _T("Default");
			break;
		}
		return RenderDeviceType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(GraphicsSystem::CRenderer::TESTFUNC e)
	{
		CString				TestFunc;

		switch (e)
		{
		case GraphicsSystem::CRenderer::TF_NEVER:
			TestFunc = _T("Never");
			break;
		case GraphicsSystem::CRenderer::TF_LESS:
			TestFunc = _T("Less");
			break;
		case GraphicsSystem::CRenderer::TF_EQUAL:
			TestFunc = _T("Equal");
			break;
		case GraphicsSystem::CRenderer::TF_LEQUAL:
			TestFunc = _T("Less Equal");
			break;
		case GraphicsSystem::CRenderer::TF_GREATER:
			TestFunc = _T("Greater");
			break;
		case GraphicsSystem::CRenderer::TF_NOTEQUAL:
			TestFunc = _T("Not equal");
			break;
		case GraphicsSystem::CRenderer::TF_GEQUAL:
			TestFunc = _T("Greater Equal");
			break;
		case GraphicsSystem::CRenderer::TF_ALWAYS:
			TestFunc = _T("Always");
			break;
		}
		return TestFunc;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(GraphicsSystem::CRenderer::LOCKMODE e)
	{
		CString				LockMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::LOCK_TO_READ:
			LockMode = _T("Lock to read");
			break;
		case GraphicsSystem::CRenderer::LOCK_TO_WRITE:
			LockMode = _T("Lock to write");
			break;
		case GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD:
			LockMode = _T("Lock to write discard old");
			break;
		case GraphicsSystem::CRenderer::LOCK_TO_WRITE_NO_OVERWRITE:
			LockMode = _T("Lock to write no overwrite");
			break;
		case GraphicsSystem::CRenderer::LOCK_TO_BOTH:
			LockMode = _T("Lock to both");
			break;
		}
		return LockMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::SHADERTARGET e)
	{
		CString				Target;

		switch (e)
		{
		case GraphicsSystem::CRenderer::SHADER_TARGET_11:
			Target = _T("1_1");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_20:
			Target = _T("2_0");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_2X:
			Target = _T("2_X");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_30:
			Target = _T("3_0");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_40:
			Target = _T("4_0");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_41:
			Target = _T("4_1");
			break;
		case GraphicsSystem::CRenderer::SHADER_TARGET_50:
			Target = _T("5_0");
			break;
		default:
			Target = _T("3_0");
			break;
		}
		return Target;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::TEXTURETYPE e)
	{
		CString				Type;

		switch (e)
		{
		case GraphicsSystem::CRenderer::TT_2D_TEXTURE:
			Type = _T("2D_TEXTURE");
			break;
		case GraphicsSystem::CRenderer::TT_3D_TEXTURE:
			Type = _T("3D_TEXTURE");
			break;
		case GraphicsSystem::CRenderer::TT_CUBE_TEXTURE:
			Type = _T("CUBE_TEXTURE");
			break;
		case GraphicsSystem::CRenderer::TT_2D_TEXTURE_ARRAY:
			Type = _T("2D_TEXTURE_ARRAY");
			break;
		case GraphicsSystem::CRenderer::TT_3D_TEXTURE_ARRAY:
			Type = _T("3D_TEXTURE_ARRAY");
			break;
		case GraphicsSystem::CRenderer::TT_CUBE_TEXTURE_ARRAY:
			Type = _T("CUBE_TEXTURE_ARRAY");
			break;
		}
		return Type;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::FORMAT e)
	{
		CString				Format;

		switch (e)
		{
		case GraphicsSystem::CRenderer::FORMAT_R8G8B8:
			Format = _T("R8G8B8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8R8G8B8:
			Format = _T("A8R8G8B8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_X8R8G8B8:
			Format = _T("X8R8G8B8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_R5G6B5:
			Format = _T("R5G6B5");
			break;
		case GraphicsSystem::CRenderer::FORMAT_X1R5G5B5:
			Format = _T("X1R5G5B5");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A1R5G5B5:
			Format = _T("A1R5G5B5");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A4R4G4B4:
			Format = _T("A4R4G4B4");
			break;
		case GraphicsSystem::CRenderer::FORMAT_R3G3B2:
			Format = _T("R3G3B2");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8:
			Format = _T("A8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8R3G3B2:
			Format = _T("A8R3G3B2");
			break;
		case GraphicsSystem::CRenderer::FORMAT_X4R4G4B4:
			Format = _T("X4R4G4B4");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A2B10G10R10:
			Format = _T("A2B10G10R10");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8B8G8R8:
			Format = _T("A8B8G8R8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_X8B8G8R8:
			Format = _T("X8B8G8R8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_G16R16:
			Format = _T("G16R16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A2R10G10B10:
			Format = _T("A2R10G10B10");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A16B16G16R16:
			Format = _T("A16B16G16R16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8P8:
			Format = _T("A8P8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_P8:
			Format = _T("P8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_L8:
			Format = _T("L8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A8L8:
			Format = _T("A8L8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A4L4:
			Format = _T("A4L4");
			break;
		case GraphicsSystem::CRenderer::FORMAT_V8U8:
			Format = _T("V8U8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_L6V5U5:
			Format = _T("L6V5U5");
			break;
		case GraphicsSystem::CRenderer::FORMAT_X8L8V8U8:
			Format = _T("X8L8V8U8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_Q8W8V8U8:
			Format = _T("Q8W8V8U8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_V16U16:
			Format = _T("V16U16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A2W10V10U10:
			Format = _T("A2W10V10U10");
			break;
		case GraphicsSystem::CRenderer::FORMAT_UYVY:
			Format = _T("UYVY");
			break;
		case GraphicsSystem::CRenderer::FORMAT_R8G8_B8G8:
			Format = _T("R8G8_B8G8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_YUY2:
			Format = _T("YUY2");
			break;
		case GraphicsSystem::CRenderer::FORMAT_G8R8_G8B8:
			Format = _T("G8R8_G8B8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_DXT1:
			Format = _T("DXT1");
			break;
		case GraphicsSystem::CRenderer::FORMAT_DXT2:
			Format = _T("DXT2");
			break;
		case GraphicsSystem::CRenderer::FORMAT_DXT3:
			Format = _T("DXT3");
			break;
		case GraphicsSystem::CRenderer::FORMAT_DXT4:
			Format = _T("DXT4");
			break;
		case GraphicsSystem::CRenderer::FORMAT_DXT5:
			Format = _T("DXT5");
			break;
		case GraphicsSystem::CRenderer::FORMAT_ATI1N:
			Format = _T("ATI1N");
			break;
		case GraphicsSystem::CRenderer::FORMAT_ATI2N:
			Format = _T("ATI2N");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D16_LOCKABLE:
			Format = _T("D16_LOCKABLE");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D32:
			Format = _T("D32");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D15S1:
			Format = _T("D15S1");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D24S8:
			Format = _T("D24S8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D24X8:
			Format = _T("D24X8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D24X4S4:
			Format = _T("D24X4S4");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D16:
			Format = _T("D16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D32F_LOCKABLE:
			Format = _T("D32F_LOCKABLE");
			break;
		case GraphicsSystem::CRenderer::FORMAT_D24FS8:
			Format = _T("D24FS8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_L16:
			Format = _T("L16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_VERTEXDATA:
			Format = _T("VERTEXDATA");
			break;
		case GraphicsSystem::CRenderer::FORMAT_INDEX16:
			Format = _T("INDEX16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_INDEX32:
			Format = _T("INDEX32");
			break;
		case GraphicsSystem::CRenderer::FORMAT_Q16W16V16U16:
			Format = _T("Q16W16V16U16");
			break;
		case GraphicsSystem::CRenderer::FORMAT_MULTI2_ARGB8:
			Format = _T("MULTI2_ARGB8");
			break;
		case GraphicsSystem::CRenderer::FORMAT_R16F:
			Format = _T("R16F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_G16R16F:
			Format = _T("G16R16F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A16B16G16R16F:
			Format = _T("A16B16G16R16F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_R32F:
			Format = _T("R32F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_G32R32F:
			Format = _T("G32R32F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_A32B32G32R32F:
			Format = _T("A32B32G32R32F");
			break;
		case GraphicsSystem::CRenderer::FORMAT_CxV8U8:
			Format = _T("CxV8U8");
			break;
		default:
			Format = _T("Unknown");
			break;
		}
		return Format;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::BLENDSTATEDESC TranslateBlendType(GraphicsSystem::CRenderer::BLENDTYPE e)
	{
		GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
		BDesc.Default();

		switch (e)
		{
		default:
		case GraphicsSystem::CRenderer::BLT_ALPHA:
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_SRC_ALPHA;
			BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
			BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = GraphicsSystem::CRenderer::ALL_CHANNELS;
			break;

		case GraphicsSystem::CRenderer::BLT_ADD:
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = GraphicsSystem::CRenderer::ALL_CHANNELS;
			break;

		case GraphicsSystem::CRenderer::BLT_SUB:
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_COLOR;
			BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = GraphicsSystem::CRenderer::ALL_CHANNELS;
			break;

		case GraphicsSystem::CRenderer::BLT_COLOR:
			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_DEST_COLOR;
			BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = GraphicsSystem::CRenderer::ALL_CHANNELS;
			break;
		}
		return BDesc;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::BLENDTYPE GetBlendType(const GraphicsSystem::CBlendState& b)
	{

		GraphicsSystem::CRenderer::BLENDTYPE BlendType = GraphicsSystem::CRenderer::BLT_ALPHA;
		if (GraphicsSystem::CRenderer::BF_SRC_ALPHA == b.GetRTBlendParam(0).SrcBlend && GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA == b.GetRTBlendParam(0).DestBlend)
		{
			BlendType = GraphicsSystem::CRenderer::BLT_ALPHA;
		}
		else if (GraphicsSystem::CRenderer::BF_ONE == b.GetRTBlendParam(0).SrcBlend && GraphicsSystem::CRenderer::BF_ONE == b.GetRTBlendParam(0).DestBlend)
		{
			BlendType = GraphicsSystem::CRenderer::BLT_ADD;
		}
		else if (GraphicsSystem::CRenderer::BF_ZERO == b.GetRTBlendParam(0).SrcBlend && GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_COLOR == b.GetRTBlendParam(0).DestBlend)
		{
			BlendType = GraphicsSystem::CRenderer::BLT_SUB;
		}
		else if (GraphicsSystem::CRenderer::BF_DEST_COLOR == b.GetRTBlendParam(0).SrcBlend && GraphicsSystem::CRenderer::BF_ZERO == b.GetRTBlendParam(0).DestBlend)
		{
			BlendType = GraphicsSystem::CRenderer::BLT_COLOR;
		}

		return BlendType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::FORMAT GetFormat(const CString& s)
	{
		for (int i = 0; i<GraphicsSystem::CRenderer::NUM_FORMAT; i++)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::FORMAT)i) == s)
				return (GraphicsSystem::CRenderer::FORMAT)i;
		}
		return GraphicsSystem::CRenderer::FORMAT_NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(GraphicsSystem::CRenderer::BLENDTYPE e)
	{
		CString				Type;

		switch (e)
		{
		case GraphicsSystem::CRenderer::BLT_ALPHA:
			Type = _T("Alpha");
			break;
		case GraphicsSystem::CRenderer::BLT_ADD:
			Type = _T("Add");
			break;
		case GraphicsSystem::CRenderer::BLT_COLOR:
			Type = _T("Color");
			break;
		case GraphicsSystem::CRenderer::BLT_SUB:
			Type = _T("Sub");
			break;
		}
		return Type;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::BLENDTYPE GetBlendType(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_BLENDTYPES; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::BLENDTYPE)i) == s)
				return (GraphicsSystem::CRenderer::BLENDTYPE)i;
		}
		return GraphicsSystem::CRenderer::BLT_ALPHA;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::BLENDFUNC GetBlendFunc(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_BLENDFUNC; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::BLENDFUNC)i) == s)
				return (GraphicsSystem::CRenderer::BLENDFUNC)i;
		}
		return GraphicsSystem::CRenderer::BF_ONE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::BLENDOP GetBlendOp(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_BLENDOP; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::BLENDOP)i) == s)
				return (GraphicsSystem::CRenderer::BLENDOP)i;
		}
		return GraphicsSystem::CRenderer::BO_ADD;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::ADDRESSMODE e)
	{
		CString				AddressMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::AM_WRAP:
			AddressMode = _T("Wrap");
			break;
		case GraphicsSystem::CRenderer::AM_MIRROR:
			AddressMode = _T("Mirror");
			break;
		case GraphicsSystem::CRenderer::AM_CLAMP:
			AddressMode = _T("Clamp");
			break;
		case GraphicsSystem::CRenderer::AM_BORDER:
			AddressMode = _T("Border");
			break;
		case GraphicsSystem::CRenderer::AM_MIRROR_ONCE:
			AddressMode = _T("Mirror Once");
			break;
		}
		return AddressMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::FILTERMODE e)
	{
		CString				FilterMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_POINT:
			FilterMode = _T("MIN_MAG_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_MAG_POINT_MIP_LINEAR:
			FilterMode = _T("MIN_MAG_POINT_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_POINT_MAG_LINEAR_MIP_POINT:
			FilterMode = _T("MIN_POINT_MAG_LINEAR_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_POINT_MAG_MIP_LINEAR:
			FilterMode = _T("MIN_POINT_MAG_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_LINEAR_MAG_MIP_POINT:
			FilterMode = _T("MIN_LINEAR_MAG_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			FilterMode = _T("MIN_LINEAR_MAG_POINT_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_MAG_LINEAR_MIP_POINT:
			FilterMode = _T("MIN_MAG_LINEAR_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_LINEAR:
			FilterMode = _T("MIN_MAG_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_ANISOTROPIC:
			FilterMode = _T("ANISOTROPIC");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_MAG_MIP_POINT:
			FilterMode = _T("COMPARISON_MIN_MAG_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
			FilterMode = _T("COMPARISON_MIN_MAG_POINT_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
			FilterMode = _T("COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
			FilterMode = _T("COMPARISON_MIN_POINT_MAG_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
			FilterMode = _T("COMPARISON_MIN_LINEAR_MAG_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			FilterMode = _T("COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
			FilterMode = _T("COMPARISON_MIN_MAG_LINEAR_MIP_POINT");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_MIN_MAG_MIP_LINEAR:
			FilterMode = _T("COMPARISON_MIN_MAG_MIP_LINEAR");
			break;
		case GraphicsSystem::CRenderer::FM_COMPARISON_ANISOTROPIC:
			FilterMode = _T("COMPARISON_ANISOTROPIC");
			break;
		}
		return FilterMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::BLENDFUNC e)
	{
		CString				BlendFunc;

		switch (e)
		{
		case GraphicsSystem::CRenderer::BF_ZERO:
			BlendFunc = _T("Zero");
			break;
		case GraphicsSystem::CRenderer::BF_ONE:
			BlendFunc = _T("One");
			break;
		case GraphicsSystem::CRenderer::BF_SRC_COLOR:
			BlendFunc = _T("Src Color");
			break;
		case GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_COLOR:
			BlendFunc = _T("1-Src Color");
			break;
		case GraphicsSystem::CRenderer::BF_SRC_ALPHA:
			BlendFunc = _T("Src Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA:
			BlendFunc = _T("1-Src Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_DEST_ALPHA:
			BlendFunc = _T("Dest Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_ONE_MINUS_DEST_ALPHA:
			BlendFunc = _T("1-Dest Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_DEST_COLOR:
			BlendFunc = _T("Dest Color");
			break;
		case GraphicsSystem::CRenderer::BF_ONE_MINUS_DEST_COLOR:
			BlendFunc = _T("1-Dest Color");
			break;
		case GraphicsSystem::CRenderer::BF_SRC_ALPHA_SAT:
			BlendFunc = _T("Src Alpha Sat");
			break;
		case GraphicsSystem::CRenderer::BF_BOTH_SRC_ALPHA:
			BlendFunc = _T("Both Src Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_BOTH_ONE_MINUS_SRC_ALPHA:
			BlendFunc = _T("1-Both Src Alpha");
			break;
		case GraphicsSystem::CRenderer::BF_BLEND_FACTOR:
			BlendFunc = _T("Blens Factor");
			break;
		case GraphicsSystem::CRenderer::BF_ONE_MINUS_BLEND_FACTOR:
			BlendFunc = _T("1-Blend Factor");
			break;
		}
		return BlendFunc;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::BLENDOP e)
	{
		CString				BlendOp;

		switch (e)
		{
		case GraphicsSystem::CRenderer::BO_ADD:
			BlendOp = _T("Add");
			break;
		case GraphicsSystem::CRenderer::BO_SUBTRACT:
			BlendOp = _T("Subtract");
			break;
		case GraphicsSystem::CRenderer::BO_REVSUBTRACT:
			BlendOp = _T("RevSubtract");
			break;
		case GraphicsSystem::CRenderer::BO_MIN:
			BlendOp = _T("Min");
			break;
		case GraphicsSystem::CRenderer::BO_MAX:
			BlendOp = _T("Max");
			break;
		}
		return BlendOp;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::DEPTHBUFFERMODE e)
	{
		CString				DBMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::DB_ALWAYS:
			DBMode = _T("Always");
			break;
		case GraphicsSystem::CRenderer::DB_NONE:
			DBMode = _T("None");
			break;
		case GraphicsSystem::CRenderer::DB_CW:
			DBMode = _T("Compare Write");
			break;
		case GraphicsSystem::CRenderer::DB_W:
			DBMode = _T("Write");
			break;
		case GraphicsSystem::CRenderer::DB_C:
			DBMode = _T("Compare");
			break;
		}
		return DBMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::FILLMODE e)
	{
		CString				FillMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::FM_WIREFRAME:
			FillMode = _T("Wireframe");
			break;
		case GraphicsSystem::CRenderer::FM_SOLID:
			FillMode = _T("Solid");
			break;
		}
		return FillMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::FILLMODE GetFillMode(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_FILLMODE; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::FILLMODE)i) == s)
				return (GraphicsSystem::CRenderer::FILLMODE)i;
		}
		return GraphicsSystem::CRenderer::FM_SOLID;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::CULLMODE e)
	{
		CString				CullMode;

		switch (e)
		{
		case GraphicsSystem::CRenderer::CULL_NONE:
			CullMode = _T("None");
			break;
		case GraphicsSystem::CRenderer::CULL_CCW:
			CullMode = _T("CCW");
			break;
		case GraphicsSystem::CRenderer::CULL_CW:
			CullMode = _T("CW");
			break;
		}
		return CullMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::CULLMODE GetCullMode(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_CULLMODE; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::CULLMODE)i) == s)
				return (GraphicsSystem::CRenderer::CULLMODE)i;
		}
		return GraphicsSystem::CRenderer::CULL_CCW;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::DEPTHBUFFERMODE GetDepthBufferMode(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_DEPTHBUFFERMODE; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::DEPTHBUFFERMODE)i) == s)
				return (GraphicsSystem::CRenderer::DEPTHBUFFERMODE)i;
		}
		return GraphicsSystem::CRenderer::DB_ALWAYS;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::STENCILOP GetStencilOp(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_STENCILOP; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::STENCILOP)i) == s)
				return (GraphicsSystem::CRenderer::STENCILOP)i;
		}
		return GraphicsSystem::CRenderer::SO_KEEP;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CRenderer::TESTFUNC GetTestFunc(const CString& s)
	{
		for (int i = 0; i < GraphicsSystem::CRenderer::NUM_TESTFUNC; ++i)
		{
			if (LynxEngine::GetEnumStr((GraphicsSystem::CRenderer::TESTFUNC)i) == s)
				return (GraphicsSystem::CRenderer::TESTFUNC)i;
		}
		return GraphicsSystem::CRenderer::TF_ALWAYS;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::PRIMITIVETYPE e)
	{
		CString				PType;

		switch (e)
		{
		case GraphicsSystem::CRenderer::POINT_LIST:
			PType = _T("Point List");
			break;
		case GraphicsSystem::CRenderer::LINE_LIST:
			PType = _T("Line List");
			break;
		case GraphicsSystem::CRenderer::LINE_STRIP:
			PType = _T("Line Strip");
			break;
		case GraphicsSystem::CRenderer::LINE_LOOP:
			PType = _T("Line Loop");
			break;
		case GraphicsSystem::CRenderer::TRIANGLE_LIST:
			PType = _T("Triangle List");
			break;
		case GraphicsSystem::CRenderer::TRIANGLE_STRIP:
			PType = _T("Triangle Strip");
			break;
		case GraphicsSystem::CRenderer::QUAD_LIST:
			PType = _T("Quad List");
			break;
		case GraphicsSystem::CRenderer::QUAD_STRIP:
			PType = _T("Quad Strip");
			break;
		case GraphicsSystem::CRenderer::POLYGON:
			PType = _T("Polygon");
			break;
		}
		return PType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::STENCILOP e)
	{
		CString				StencilOp;

		switch (e)
		{
		case GraphicsSystem::CRenderer::SO_KEEP:
			StencilOp = _T("Keep");
			break;
		case GraphicsSystem::CRenderer::SO_ZERO:
			StencilOp = _T("Zero");
			break;
		case GraphicsSystem::CRenderer::SO_REPLACE:
			StencilOp = _T("Replace");
			break;
		case GraphicsSystem::CRenderer::SO_INCR:
			StencilOp = _T("Incr");
			break;
		case GraphicsSystem::CRenderer::SO_DECR:
			StencilOp = _T("Decr");
			break;
		case GraphicsSystem::CRenderer::SO_INVERT:
			StencilOp = _T("Invert");
			break;
		}
		return StencilOp;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  隤芣?:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(GraphicsSystem::CRenderer::VERTEXSLOTTYPE e)
	{
		CString				VertexLayoutType;

		switch (e)
		{
		default:
		case GraphicsSystem::CRenderer::VST_NULL:
			VertexLayoutType = _T("NULL");
			break;
		case GraphicsSystem::CRenderer::VST_POSITION:
			VertexLayoutType = _T("POSITION");
			break;
		case GraphicsSystem::CRenderer::VST_TANGENT:
			VertexLayoutType = _T("TANGENT");
			break;
		case GraphicsSystem::CRenderer::VST_NORMAL:
			VertexLayoutType = _T("NORMAL");
			break;
		case GraphicsSystem::CRenderer::VST_BINORMAL:
			VertexLayoutType = _T("BINORMAL");
			break;
		case GraphicsSystem::CRenderer::VST_TEX_COORD:
			VertexLayoutType = _T("TEXCOORD");
			break;
		case GraphicsSystem::CRenderer::VST_COLOR:
			VertexLayoutType = _T("COLOR");
			break;
		case GraphicsSystem::CRenderer::VST_BLEND_WEIGHT:
			VertexLayoutType = _T("BLENDWEIGHT");
			break;
		case GraphicsSystem::CRenderer::VST_BLEND_INDICES:
			VertexLayoutType = _T("BLENDINDICES");
			break;
		case GraphicsSystem::CRenderer::VST_DEPTH:
			VertexLayoutType = _T("DEPTH");
			break;
		case GraphicsSystem::CRenderer::VST_POINT_SIZE:
			VertexLayoutType = _T("POINTSIZE");
			break;
		}
		return VertexLayoutType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(PLAYMODE e)
	{
		CString				PlayMode;

		switch (e)
		{
		case PM_ONCE:
			PlayMode = _T("Once");
			break;
		case PM_LOOP:
			PlayMode = _T("Loop");
			break;
		case PM_BOUNCE:
			PlayMode = _T("Bounce");
			break;
		}
		return PlayMode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	PLAYMODE GetPlayMode(const CString& s)
	{
		for (int i = 0; i<NUM_PLAYMODE; i++)
		{
			if (LynxEngine::GetEnumStr((PLAYMODE)i) == s)
				return (PLAYMODE)i;
		}
		return PM_LOOP;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(TRANSPARENTTYPE e)
	{
		CString				TransparentType;

		switch (e)
		{
		case TT_OPAQUE:
			TransparentType = _T("Opaque");
			break;
		case TT_TRANSPARENT:
			TransparentType = _T("Transparent");
			break;
		case TT_TRANSLUCENT:
			TransparentType = _T("Translucent");
			break;
		case TT_OPAQUE_TRANSLUCENT:
			TransparentType = _T("Opaque_Translucent");
			break;
		}
		return TransparentType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	TRANSPARENTTYPE GetTransparentType(const CString& s)
	{
		for (int i = 0; i<NUM_TRANSPARENTTYPE; i++)
		{
			if (LynxEngine::GetEnumStr((TRANSPARENTTYPE)i) == s)
				return (TRANSPARENTTYPE)i;
		}
		return TT_OPAQUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(RESOURCETYPE e)
	{
		CString				ResourceType;

		switch (e)
		{
		case TEXTURE:
			ResourceType = _T("Texture");
			break;
		case STREAMING_TEXTURE:
			ResourceType = _T("Streaming Texture");
			break;
		case TEXTURE_ANIMATION:
			ResourceType = _T("Texture Animation");
			break;
		case DEPTHSTENCIL_STATE:
			ResourceType = _T("DepthStencil State");
			break;
		case BLEND_STATE:
			ResourceType = _T("Blend State");
			break;
		case RASTERIZER_STATE:
			ResourceType = _T("Rasterizer State");
			break;
		case SAMPLER_STATE:
			ResourceType = _T("Sampler State");
			break;
		case VERTEX_SHADER:
			ResourceType = _T("Vertex Shader");
			break;
		case GEOMETRY_SHADER:
			ResourceType = _T("Geometry Shader");
			break;
		case PIXEL_SHADER:
			ResourceType = _T("Pixel Shader");
			break;
		case HULL_SHADER:
			ResourceType = _T("Hull Shader");
			break;
		case DOMAIN_SHADER:
			ResourceType = _T("Domain Shader");
			break;
		case COMPUTE_SHADER:
			ResourceType = _T("Compute Shader");
			break;
		case BLEND_SHADER:
			ResourceType = _T("Blend Shader");
			break;
		case TEXTURE_SHADER:
			ResourceType = _T("Texture Shader");
			break;
		case SHADER_GROUP:
			ResourceType = _T("Shader Group");
			break;
		case VERTEX_LAYOUT:
			ResourceType = _T("Vertex Layout");
			break;
		case VERTEX_ARRAY:
			ResourceType = _T("Vertex Array");
			break;
		case INDEX_ARRAY:
			ResourceType = _T("Index Array");
			break;
		case QUERY:
			ResourceType = _T("Query");
			break;
		case STREAMOUT_BUFFER:
			ResourceType = _T("Streamout Buffer");
			break;
		case BUFFER:
			ResourceType = _T("Buffer");
			break;
		case MATERIAL:
			ResourceType = _T("Material");
			break;
		case CONTAINER:
			ResourceType = _T("Container");
			break;
		case SCRIPT:
			ResourceType = _T("Script");
			break;
		case GAME_OBJECT:
			ResourceType = _T("Game Object");
			break;
		case SOUND:
			ResourceType = _T("Sound");
			break;
		case MUSIC:
			ResourceType = _T("Music");
			break;
		default:
			ResourceType = _T("Unknown resource type");
			break;

		}
		return ResourceType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(DEVICETYPE e)
	{
		CString	DeviceType;

		switch (e)
		{
		case DT_PC:
			DeviceType = _T("PC");
			break;
		case DT_IPOD_1:
			DeviceType = _T("iPod 1");
			break;
		case DT_IPOD_2:
			DeviceType = _T("iPod 2");
			break;
		case DT_IPOD_3:
			DeviceType = _T("iPod 3");
			break;
		case DT_IPOD_4:
			DeviceType = _T("iPod 4");
			break;
		case DT_IPOD_5:
			DeviceType = _T("iPod 5");
			break;
		case DT_IPHONE_1:
			DeviceType = _T("iPhone 1");
			break;
		case DT_IPHONE_2:
			DeviceType = _T("iPhone 2");
			break;
		case DT_IPHONE_3:
			DeviceType = _T("iPhone 3");
			break;
		case DT_IPHONE_4:
			DeviceType = _T("iPhone 4");
			break;
		case DT_IPHONE_5:
			DeviceType = _T("iPhone 5");
			break;
		case DT_IPHONE_6:
			DeviceType = _T("iPhone 6");
			break;
		case DT_IPHONE_7:
			DeviceType = _T("iPhone 7");
			break;
		case DT_IPAD_1:
			DeviceType = _T("iPad 1");
			break;
		case DT_IPAD_2:
			DeviceType = _T("iPad 2");
			break;
		case DT_IPAD_3:
			DeviceType = _T("iPad 3");
			break;
		case DT_IPAD_4:
			DeviceType = _T("iPad 4");
			break;
		case DT_WINPHONE_7:
			DeviceType = _T("WinPhone 7");
			break;
		case DT_WINPHONE_8:
			DeviceType = _T("WinPhone 8");
			break;
		case DT_ANDROID:
			DeviceType = _T("Android");
			break;
		case DT_PS_VITA:
			DeviceType = _T("PS Vita");
			break;
		case DT_N3DS:
			DeviceType = _T("N3DS");
			break;
		case DT_WINDOWS8:
			DeviceType = _T("Windows 8");
			break;
		case DT_UNKNOWN:
		default:
			DeviceType = _T("Unknown");
			break;
		}
		return DeviceType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(DEVICECLASS e)
	{
		CString	DeviceClass;

		switch (e)
		{
		case DC_LOW:
			DeviceClass = _T("Low");
			break;
		case DC_MEDIUM:
			DeviceClass = _T("Medium");
			break;
		case DC_HIGH:
			DeviceClass = _T("High");
			break;
		case DC_ULTRA:
			DeviceClass = _T("Ultra");
			break;
		case DC_UNKNOWN:
		default:
			DeviceClass = _T("Unknown");
			break;
		}
		return DeviceClass;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(PEMITTERTYPE e)
	{
		CString	Type;

		switch (e)
		{
		default:
		case PE_POINT:
			Type = _T("Point");
			break;
		case PE_PLANE:
			Type = _T("Plane");
			break;
		case PE_CIRCLE:
			Type = _T("Circle");
			break;
		case PE_MESH:
			Type = _T("Mesh");
			break;
		case PE_CIRCLE_PLANE:
			Type = _T("Circle Plane");
			break;
		case PE_RING_PLANE:
			Type = _T("Ring Plane");
			break;
		}
		return Type;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	PEMITTERTYPE GetPEmitterType(const CString& s)
	{
		for (int i = 0; i < NUM_PETYPE; ++i)
		{
			if (GetEnumStr((PEMITTERTYPE)i) == s)
				return (PEMITTERTYPE)i;
		}
		return PE_POINT;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	CSpriteContainer::ORIGINTYPE GetOriginType(const CString& s)
	{
		if (s == CString(_T("UpperLeft")))
		{
			return CSpriteContainer::OT_UPPER_LEFT;
		}
		else if (s == CString(_T("UpperRight")))
		{
			return CSpriteContainer::OT_UPPER_RIGHT;
		}
		else if (s == CString(_T("Center")))
		{
			return CSpriteContainer::OT_CENTER;
		}
		else
		{
			return CSpriteContainer::OT_CENTER;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	int	GetAligmentType(const CString& s)
	{
		if (s == CString(_T("H-Align-Left")))
		{
			return CSpriteContainer::H_ALIGN_LEFT;
		}
		else if (s == CString(_T("H-Align-Center")))
		{
			return CSpriteContainer::H_ALIGN_CENTER;
		}
		else if (s == CString(_T("H-Align-Right")))
		{
			return CSpriteContainer::H_ALIGN_RIGHT;
		}
		else if (s == CString(_T("V-Align-Top")))
		{
			return CSpriteContainer::V_ALIGN_TOP;
		}
		else if (s == CString(_T("V-Align-Center")))
		{
			return CSpriteContainer::V_ALIGN_CENTER;
		}
		else if (s == CString(_T("V-Align-Bottom")))
		{
			return CSpriteContainer::V_ALIGN_BOTTOM;
		}
		else
		{
			return CSpriteContainer::H_ALIGN_CENTER;
		}
	}

	IMPLEMENT_CLASSSCRIPT(CEngine, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CEngine::GetFileNameFromPath(LYNXCHAR* name, const LYNXCHAR* path)
	{
		LYNXCHAR* Token;

		lynxStrCpy(name, path);
		Token = (LYNXCHAR *)lynxStrTok(name, _T("\\/"));
		while (Token)
		{
			Token = (LYNXCHAR *)lynxStrTok(NULL, _T("\\/"));
			if (Token)
				lynxStrCpy(name, Token);
		}
		lynxStrTok(name, _T("."));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GetFileNameFromPathAnsi(char* name, const char* path)
	{
		char* Token;

		strcpy(name, path);
		Token = (char *)strtok(name, "\\/");
		while (Token)
		{
			Token = (char *)strtok(NULL, "\\/");
			if (Token)
				strcpy(name, Token);
		}
		strtok(name, ".");
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GetRelativePath(LYNXCHAR *path, const LYNXCHAR* work, const LYNXCHAR *fname)
	{
		int i;
		for (i = 0; i<lynxStrLen(fname); i++)
		{
			if (work[i] != fname[i])
				break;
		}
		if (i > 0)
			lynxStrCpy(path, _T("../"));
		else
			lynxStrCpy(path, _T(""));
		lynxStrCat(path, &fname[i]);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GetLowerRelativePath(LYNXCHAR *path, const LYNXCHAR* work, const LYNXCHAR *fname)
	{
		int i;
		for (i = 0; i<lynxStrLen(fname); i++)
		{
			if (work[i] != fname[i])
				break;
		}
		if (i > 0)
			lynxStrCpy(path, _T("../"));
		else
			lynxStrCpy(path, _T(""));
		lynxStrCat(path, &fname[i]);
		lynxStrLower(path);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::FilenameBackslashToSlash(CString& name)
	{
		for (CString::iterator i = name.begin(); i != name.end(); i++)
		{
			if ((*i) == _T('\\'))
				(*i) = _T('/');
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GetTexturePathFromFilename(LYNXCHAR *path, const LYNXCHAR *fname)
	{
		LYNXCHAR Path[256], Name[256], Ext[256];
		lynxSeparatePathFilenameExtByFullPath(Path, Name, Ext, fname);

		if (Path[0] == _T('.') && Path[1] == _T('.') && Path[2] == _T('/'))
		{
			lynxStrCpy(path, _T("../texture/"));
			lynxStrCat(path, &Path[3 + 6]);
		}
		else if (Path[0] == _T('.') && Path[1] == _T('/'))
		{
			lynxStrCpy(path, _T("./texture/"));
			lynxStrCat(path, &Path[2 + 6]);
		}
		else
		{
			lynxStrCpy(path, _T("../texture/"));
			lynxStrCat(path, &Path[CString(Path).find(_T("QubotWar")) + 9 + 6]);
		}
		lynxStrCat(path, _T("/"));
		lynxDebugOutputPrintf(path);
	}
	//-------------------------------------------------------------------------------------------------------
	// This C++ program uses zlib to read then decompress a gzipped file in memory.
	// Author: Andrew Lim Chong Liang
	// http://windrealm.org	
	//-------------------------------------------------------------------------------------------------------v	
	LYNXBOOL CEngine::GZipInflate(LynxEngine::CAnsiString& uncompressedBytes, const LynxEngine::CAnsiString& compressedBytes)
	{
		if (compressedBytes.size() == 0)
		{
			uncompressedBytes = compressedBytes;
			return true;
		}
		uncompressedBytes.clear();

		unsigned full_length = compressedBytes.size();
		unsigned half_length = compressedBytes.size() / 2;

		unsigned uncompLength = full_length;
		char* uncomp = (char*)LYNXALLOC(uncompLength);

		z_stream strm;
		strm.next_in = (Bytef *)compressedBytes.c_str();
		strm.avail_in = compressedBytes.size();
		strm.total_out = 0;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;

		LYNXBOOL done = LYNX_FALSE;

		if (inflateInit2(&strm, (16 + MAX_WBITS)) != Z_OK)
		{
			LYNXFREE(uncomp);
			return LYNX_FALSE;
		}

		while (!done)
		{
			// If our output buffer is too small
			if (strm.total_out >= uncompLength)
			{
				// Increase size of output buffer
				char* uncomp2 = (char*)LYNXALLOC(uncompLength + half_length);
				memcpy(uncomp2, uncomp, uncompLength);
				uncompLength += half_length;
				LYNXFREE(uncomp);
				uncomp = uncomp2;
			}
			strm.next_out = (Bytef *)(uncomp + strm.total_out);
			strm.avail_out = uncompLength - strm.total_out;

			// Inflate another chunk.
			int err = inflate(&strm, Z_SYNC_FLUSH);
			if (err == Z_STREAM_END)
			{
				done = LYNX_TRUE;
			}
			else if (err != Z_OK)
			{
				break;
			}
		}

		if (inflateEnd(&strm) != Z_OK)
		{
			LYNXFREE(uncomp);
			return LYNX_FALSE;
		}

		if (strm.total_out == 0)
		{
			LYNXFREE(uncomp);
			return LYNX_FALSE;
		}

		uncompressedBytes.assign(uncomp, strm.total_out);
		LYNXFREE(uncomp);

		/*
		while (!done)
		{
		// If our output buffer is too small
		if (strm.total_out >= uncompLength)
		{
		// Increase size of output buffer
		char* uncomp2 = LYNXNEW char[uncompLength + half_length];
		memcpy(uncomp2, uncomp, uncompLength);
		uncompLength += half_length ;
		LYNXDEL_ARRAY(uncomp);
		uncomp = uncomp2 ;
		}
		strm.next_out = (Bytef *) (uncomp + strm.total_out);
		strm.avail_out = uncompLength - strm.total_out;

		// Inflate another chunk.
		int err = inflate (&strm, Z_SYNC_FLUSH);
		if (err == Z_STREAM_END)
		{
		done = LYNX_TRUE;
		}
		else if (err != Z_OK)
		{
		break;
		}
		}

		if (inflateEnd (&strm) != Z_OK)
		{
		LYNXDEL_ARRAY(uncomp);
		return LYNX_FALSE;
		}

		if (strm.total_out == 0)
		{
		LYNXDEL_ARRAY(uncomp);
		return LYNX_FALSE;
		}

		uncomp[strm.total_out] = 0x00;
		uncompressedBytes = uncomp;
		LYNXDEL_ARRAY(uncomp);
		*/

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::InitMembers(void)
	{
		m_lpEngine = this;
		m_NumResources = 0;
		m_ResourceManager.SetlpEngine(this);
		m_ContainerFactory.SetParam(this);
		m_ShaderClassManager.SetlpEngine(this);
		m_VertexLayoutManager.SetlpEngine(this);

		m_lpFileSystem = NULL;
		m_lpInputSystem = NULL;
		m_lpGameConsoleSystem = NULL;
		m_lpNetworkSystem = NULL;
		m_lpSoundSystem = NULL;
		m_lpScriptSystem = NULL;
		m_lpPhysicsSystem = NULL;
		m_PhysicsFrameTime = NULL;
		m_lpGUISystem = NULL;
		m_lpGraphicsSystem = NULL;
		m_lpSceneSystem = NULL;
		m_SceneFrameTime = NULL;
		m_lpCollisionSystem = NULL;
		m_lpPerfSystem = NULL;
		m_lpAdSystem = NULL;

		m_bLockFrameRate = LYNX_FALSE;
		m_UpdateRate = 60.0f;
		m_Step = 1.0f;
		m_StepMultiplier = 1.0f;
		m_FineStepData.bFineStepMode = LYNX_FALSE;
		m_FineStepData.MinStep = 1.0f;
		m_FineStepData.MaxCountOfLoop = 6;
		m_lpSystemKeyboard = NULL;
		m_lpSystemMouse = NULL;
		m_SystemGamePadArray.clear();
		m_lpSystemInputRecord = NULL;
		m_lpSystemTouch = NULL;
		m_lpSystemAccelerometer = NULL;
		m_lpSystemGyrometer = NULL;
		m_lpSystemFontString = NULL;
		//m_lpSystemLargeTTFont = NULL;
		//m_lpSystemTTFont = NULL;
		//m_lpSystemSmallTTFont = NULL;
		m_lpSystemTTFontString = NULL;
		//m_lpSystemLargeFont = NULL;
		//m_lpSystemFont = NULL;
		//m_lpSystemSmallFont = NULL;		
		m_FontMap.clear();
		m_lpLogoSprite = NULL;
		m_SceneFrameTime = 0;
		m_PhysicsFrameTime = 0;

#ifdef __WIN32__
#ifdef __WINAPP__
#else
		m_FreeTypeLibrary = NULL;
#endif
#endif

		m_ShowMode = SHOW_NONE;

		m_bRegularGC = LYNX_FALSE;

		m_ConfigFilename = _T("../config/engine.ini");
		m_ConfigIniReader = NULL;

		m_bUseTouchDevice = LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CEngine::CEngine(void)
	{
		InitMembers();

		INIT_CLASSSCRIPT
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CEngine::CEngine(GameFramework::CKernel* lpk)
	{
		InitMembers();
		m_lpKernel = lpk;

		INIT_CLASSSCRIPT
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CEngine::~CEngine(void)
	{
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Destroy(")) + GetName() + CString(_T(")\r\n")));

		vDestroy();

		//Release the script before ScriptSystem is released
		if (m_lpClassScript)
			m_lpClassScript.Reset();

		if (m_lpLogoSprite)
		{
			DestroyContainer(m_lpLogoSprite);
			m_lpLogoSprite = NULL;
		}

		if (m_lpSystemFontString)
		{
			LYNXDEL m_lpSystemFontString;
			m_lpSystemFontString = NULL;
		}

		if (m_lpSystemTTFontString)
		{
			LYNXDEL m_lpSystemTTFontString;
			m_lpSystemTTFontString = NULL;
		}

		/*
		for (CMap<CAnsiString, CFontPtr&>::CIterator Font = m_FontMap.begin(); Font != m_FontMap.end(); Font++)
		{
		LYNXDEL ((*Font).second);
		}
		*/
		m_FontMap.clear();

		if (m_lpSceneSystem)
		{
			LYNXDEL m_lpSceneSystem;
			m_lpSceneSystem = NULL;
		}

		if (m_lpGraphicsSystem)
		{
			LYNXDEL m_lpGraphicsSystem;
			m_lpGraphicsSystem = NULL;
		}

		if (m_lpInputSystem)
		{
			LYNXDEL m_lpInputSystem;
			m_lpInputSystem = NULL;
		}

		if (m_lpFileSystem)
		{
			LYNXDEL m_lpFileSystem;
			m_lpFileSystem = NULL;
		}

		if (m_lpGameConsoleSystem)
		{
			LYNXDEL m_lpGameConsoleSystem;
			m_lpGameConsoleSystem = NULL;
		}

		if (m_lpNetworkSystem)
		{
			LYNXDEL m_lpNetworkSystem;
			m_lpNetworkSystem = NULL;
		}

		if (m_lpGUISystem)
		{
			LYNXDEL m_lpGUISystem;
			m_lpGUISystem = NULL;
		}

		if (m_lpCollisionSystem)
		{
			LYNXDEL m_lpCollisionSystem;
			m_lpCollisionSystem = NULL;
		}

		if (m_lpPerfSystem)
		{
			LYNXDEL m_lpPerfSystem;
			m_lpPerfSystem = NULL;
		}

		if (m_lpAdSystem)
		{
			LYNXDEL m_lpAdSystem;
			m_lpAdSystem = NULL;
		}

		// Unload dynamic subsystems
		UnloadSubSystem(m_lpPhysicsSystem);
		m_lpPhysicsSystem = NULL;

		UnloadSubSystem(m_lpSoundSystem);
		m_lpSoundSystem = NULL;

		UnloadSubSystem(m_lpScriptSystem);
		m_lpScriptSystem = NULL;

#ifdef __WIN32__
#ifdef __WINAPP__
#else
		// Release free type system
		if (m_FreeTypeLibrary)
			FT_Done_FreeType(m_FreeTypeLibrary);
#endif
#endif

		GC();

		if (m_ConfigIniReader)
		{
			LYNXDEL(m_ConfigIniReader);
			m_ConfigIniReader = NULL;
		}

		LYNX_SET_LOG(CString(GetClassName()), _T("Restore FPCW...\r\n"));
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Restore FPCW...\r\n")));
		lynxRestoreFPCW();

		LogUnreleasedResourceList();

		//Check unreleased resources
		//CList<CResourcePtr>::CIterator Res;
		//CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();
		//for (Res = ResList.begin(); Res != ResList.end(); ++Res)
		//{				
		//	if ((*Res).GetCount() != 1)
		//	{
		//		LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::!!!Resource ("))+(*Res)->GetName()+CString(_T(") is not correctly released!!!\r\n")));
		//		//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::!!!Resource <"))+CString(_T("> is not correctly released!!!\r\n")));
		//	}
		//}

		if (lynxGetAllocMemSize() > 0)
		{
			lynxClearBadMemBlock();
			//MessageBeep(MB_ICONEXCLAMATION);    
			//MessageBox(lpsdk->Renderer.SystemData.MainWindow, "記憶體未完全釋放", "警告", MB_OK | MB_ICONERROR);
		}
		lynxClearBadFileBlock();

		glpKernel->LogUnreleasedMemoryBlocks();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::LogResourceList(void)
	{
		ClearInvalidResource();
		LYNXCHAR Number[32];
		CList<CResourcePtr>::CIterator Res;
		CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

		LYNX_LOG(glpLogger, CString(_T("\r\n")));
		LYNX_LOG(glpLogger, CString(_T("------------------------------- Resource List -------------------------------\r\n")));
		for (Res = ResList.begin(); Res != ResList.end(); ++Res)
		{
			if ((*Res))
			{
				lynxItoA((*Res).GetCount(), Number, 10);
				LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::")) + GetEnumStr((*Res)->GetResourceType()) + CString(_T("(")) + (*Res)->GetName() + CString(_T("):")) + CString(Number) + CString(_T("\r\n")));
			}
		}
		LYNX_LOG(glpLogger, CString(_T("------------------------------- Resource List -------------------------------\r\n")));
		LYNX_LOG(glpLogger, CString(_T("\r\n")));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::LogUnreleasedResourceList(void)
	{
		ClearInvalidResource();
		LYNXCHAR Number[32];
		CList<CResourcePtr>::CIterator Res;
		CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

		LYNX_LOG(glpLogger, CString(_T("\r\n")));
		LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased Resource List -------------------------------\r\n")));
		lynxItoA(ResList.size(), Number, 10);
		LYNX_LOG(glpLogger, CString(Number) + CString(_T(" unreleased resources!!\r\n")));
		for (Res = ResList.begin(); Res != ResList.end(); ++Res)
		{
			if ((*Res))
			{
				lynxItoA((*Res).GetCount(), Number, 10);
				LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::!!! ")) + GetEnumStr((*Res)->GetResourceType()) + CString(_T("(")) + (*Res)->GetName() + CString(_T("):")) + CString(Number) + CString(_T(" is not correctly released !!!\r\n")));
			}
		}
		LYNX_LOG(glpLogger, CString(_T("------------------------------- Unreleased Resource List -------------------------------\r\n")));
		LYNX_LOG(glpLogger, CString(_T("\r\n")));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::VerifyResources()
	{
		CList<CResourcePtr>::CIterator Res, Res2;
		CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

		for (Res = ResList.begin(); Res != ResList.end(); ++Res)
		{
			for (Res2 = Res; Res2 != ResList.end(); ++Res2)
			{
				if (Res != Res2)
				{
					if (!(*Res) || !(*Res2))
						return LYNX_FALSE;

					if ((*Res)->GetCRCCode() == (*Res2)->GetCRCCode() &&
						(*Res)->GetName() != (*Res2)->GetName())
						return LYNX_FALSE;

					if ((*Res)->GetCRCCode() != (*Res2)->GetCRCCode() &&
						(*Res)->GetName() == (*Res2)->GetName())
						return LYNX_FALSE;

					if ((*Res)->GetResourceType() >= DEPTHSTENCIL_STATE && (*Res)->GetResourceType() <= SAMPLER_STATE &&
						(*Res2)->GetResourceType() >= DEPTHSTENCIL_STATE && (*Res2)->GetResourceType() <= SAMPLER_STATE)
					{
						if ((*Res)->GetCRCCode() == (*Res2)->GetCRCCode())
							return LYNX_FALSE;
					}
				}
			}
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	int CEngine::GetMajorVersion() { return LYNXENGINE_MAJORVERSION; }
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	int CEngine::GetMinorVersion() { return LYNXENGINE_MINORVERSION; }
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	int	CEngine::GetBuildVersion() { return LYNXENGINE_BUILDNUMBER; }
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyResource(CResourcePtr& r)
	{
		//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy resource ("))+lpr->GetName()+CString(_T(")\r\n")));
		return m_ResourceManager.DestroyObject(r);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyResource(const CString& name)
	{
		//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy resource ("))+name+CString(_T(")\r\n")));
		return m_ResourceManager.DestroyObject(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CResourcePtr& CEngine::FindResource(const CString& name)
	{
		//return m_ResourceManager.Find(name);

		LYNXCRC CRC = lynxComputeCRCFast((BYTE*)name.c_str(), name.size()*sizeof(LYNXCHAR));
		return m_ResourceManager.Find(CRC);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CResourcePtr& CEngine::FindResource(const LYNXCRC& crc)
	{
		return m_ResourceManager.Find(crc);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::ClearInvalidResource(void)
	{
		CList<CResourcePtr>::CIterator Res;
		CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

		Res = ResList.begin();
		while (Res != ResList.end())
		{
			if ((*Res))
			{
				// Remove unrefered materials, sounds, musics
				if (m_lpGraphicsSystem && (*Res)->GetResourceType() == MATERIAL && (*Res).GetCount() <= 2)
				{
					auto toDelete = (*Res)->GetName();
					if (m_lpGraphicsSystem->GetlpMaterialSystem())
						m_lpGraphicsSystem->GetlpMaterialSystem()->DeleteMaterial(toDelete);
					else
						(*Res).Reset();

					Res++;
				}
				else if (m_lpSoundSystem && (*Res)->GetResourceType() == SOUND && (*Res).GetCount() <= 2)
				{
					auto toDelete = &(**Res);
					m_lpSoundSystem->DeleteSound(dynamic_cast<SoundSystem::CSound*>(toDelete));

					//(*Res).Reset();
					Res++;
				}
				else if (m_lpSoundSystem && (*Res)->GetResourceType() == MUSIC && (*Res).GetCount() <= 2)
				{
					auto toDelete = &(**Res);
					m_lpSoundSystem->DeleteMusic(dynamic_cast<SoundSystem::CMusic*>(toDelete));

					//(*Res).Reset();
					Res++;
				}
				else if (m_lpSoundSystem && (*Res)->GetResourceType() == SCRIPT && (*Res).GetCount() <= 2)
				{
					auto toDelete = (*Res)->GetName();
					m_lpScriptSystem->RemoveScript(toDelete);

					//(*Res).Reset();
					Res++;
				}
				else
					Res++;
			}
			else
				Res++;
		}

		int OriginalNumResources = m_ResourceManager.GetObjectList().size();
		m_ResourceManager.RemoveInvalidPtr();

		LYNXCHAR Buffer[256];
		lynxSprintf(Buffer, _T("::Remove invalid resources(%d -> %d)\r\n"), OriginalNumResources, m_ResourceManager.GetObjectList().size());

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(Buffer));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GC(LYNXBOOL bforce)
	{
		if (bforce || m_ResourceManager.GetObjectList().size() > (m_NumResources*1.2f))
		{
			ClearInvalidShaderClass();
			ClearInvalidVertexLayout();
			ClearInvalidResource();
			if (m_lpFileSystem && m_lpFileSystem->GetPackage())
			{
				m_lpFileSystem->GetPackage()->ClearInvalidStream();
				m_lpFileSystem->GetPackage()->ClearClosedStream();
			}
			m_NumResources = m_ResourceManager.GetObjectList().size();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CContainer* CEngine::CreateContainer(CContainer::TYPE type)
	{
		return m_ContainerFactory.CreateObject(type);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CContainer* CEngine::_CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, PEMITTERTYPE petype, int type, int cluster_size, float life)
	{
		CContainer::TYPE ParticleType = LynxEngine::CContainer::ScriptType2EngineType(type);
		return CreatePEmitterContainer(name, filename, petype, ParticleType, cluster_size, life);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CContainer* CEngine::CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, PEMITTERTYPE petype, const CContainer::TYPE& ptype, int cluster_size, float life)
	{
		CContainer* lpC = CreateContainer(CContainer::PEMITTER);

		CPEmitterCreationDesc PEDesc;
		CSpriteCreationDesc SPDesc;
		CSprite3DCreationDesc SP3DDesc;
		CSprite3DClusterCreationDesc PDesc;
		CSpriteClusterCreationDesc SDesc;
		CVoxelClusterCreationDesc VDesc;

		PEDesc.m_ParticleType = ptype;
		PEDesc.m_Name = name;
		PEDesc.m_FileName = filename;

		if (PEDesc.m_ParticleType == CContainer::SPRITE3D_CLUSTER)
		{
			PDesc.Default();
			PDesc.m_bUseBufferObject = LYNX_TRUE;
			PDesc.m_ClusterSize = cluster_size;
			PDesc.m_Number = 1;
			PDesc.m_FileName = filename;
			PDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();
			PEDesc.m_ParticleDesc = &PDesc;
		}
		else if (PEDesc.m_ParticleType == CContainer::SPRITE3D)
		{
			PDesc.Default();
			PDesc.m_bUseBufferObject = LYNX_TRUE;
			PDesc.m_ClusterSize = cluster_size;
			PDesc.m_Number = 1;
			PDesc.m_FileName = filename;
			PDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();
			PEDesc.m_ParticleDesc = &PDesc;
		}
		else if (PEDesc.m_ParticleType == CContainer::SPRITE_CLUSTER)
		{
			SDesc.Default();
			SDesc.m_bUseBufferObject = LYNX_TRUE;
			SDesc.m_ClusterSize = cluster_size;
			SDesc.m_Number = 1;
			SDesc.m_FileName = filename;
			SDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();
			PEDesc.m_ParticleDesc = &SDesc;
		}
		else if (PEDesc.m_ParticleType == CContainer::VOXEL_CLUSTER)
		{
			VDesc.Default();
			VDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();
			VDesc.m_ClusterSize = cluster_size;
			PEDesc.m_ParticleDesc = &VDesc;
		}
		else if (PEDesc.m_ParticleType == CContainer::SPRITE)
		{
			SPDesc.Default();
			SPDesc.m_bUseBufferObject = LYNX_TRUE;
			SPDesc.m_Number = 1;
			SPDesc.m_FileName = filename;
			SPDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();
			PEDesc.m_ParticleDesc = &SPDesc;
		}

		PEDesc.m_MaxParticle = cluster_size*0.5f;
		PEDesc.m_Density = 1;
		PEDesc.m_LifeTimeLimit = life;
		PEDesc.m_Range = 3;
		PEDesc.m_Type = petype;
		PEDesc.m_NumStoppingParticles = -1;
		//PEDesc.m_lpStream = GetlpFileSystem()->GetPackage()->GetStream();

		lpC->vCreate(&PEDesc);
		//((CPEmitterContainer*)lpC)->vCreateDefaultResource();			

		return lpC;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyContainer(CContainer* const lpc)
	{
		m_ContainerFactory.DestroyObject(lpc);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::CreateVertexLayout(GraphicsSystem::CVertexLayoutPtr& v, const CString& type)
	{
		v = FindVertexLayout(type);
		if (!v)
		{
			m_VertexLayoutManager.CreateObject(v, type);
			if (!v)
				return LYNX_FALSE;
			v->SetName(type);
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyVertexLayout(GraphicsSystem::CVertexLayoutPtr& v)
	{
		return m_VertexLayoutManager.DestroyObject(v);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyVertexLayout(const CString& name)
	{
		return m_VertexLayoutManager.DestroyObject(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	GraphicsSystem::CVertexLayoutPtr& CEngine::FindVertexLayout(const CString& name)
	{
		return m_VertexLayoutManager.Find(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::ClearInvalidVertexLayout(void)
	{
		m_VertexLayoutManager.RemoveInvalidPtr();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::CreateShaderClass(GraphicsSystem::CShaderClassPtr& s, const CAnsiString& type)
	{
		m_ShaderClassManager.CreateObject(s, type);
		if (!s)
			return LYNX_FALSE;
		s->SetName(type);
		s->ComputeCRCCode();
		if (!s->vCreate())
		{
			//DestroyShaderClass(s);
			s.Reset();
			return LYNX_FALSE;
		}
		return LYNX_TRUE;

		// Because shader constant location depends on program so it's impossible to share different stage shader 
		// between shader group in OGL/ES. 
		// GL_EXT_separate_shader_objects may be able to resolve this problem.
		if (GetlpGraphicsSystem()->GetlpRenderer()->IsSharedShaderClass())
		{
			s = FindShaderClass(type);
		}

		if (!GetlpGraphicsSystem()->GetlpRenderer()->IsSharedShaderClass() || !s)
		{
			//m_ShaderClassManager.CreateObject(s, type);
			m_ShaderClassManager.CreateObject(s, type);
			if (!s)
				return LYNX_FALSE;
			s->SetName(type);
			s->ComputeCRCCode();
			if (!s->vCreate())
			{
				//DestroyShaderClass(s);
				s.Reset();
				return LYNX_FALSE;
			}
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::CreateCustomShaderClass(GraphicsSystem::CShaderClassPtr& s, const CAnsiString& type, const CString& filename)
	{
		m_ShaderClassManager.CreateObject(s, type);
		if (!s)
			return LYNX_FALSE;

		s->vCreate(filename);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyShaderClass(GraphicsSystem::CShaderClassPtr& lps)
	{
		return m_ShaderClassManager.DestroyObject(lps);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DestroyShaderClass(const CAnsiString& name)
	{
		return m_ShaderClassManager.DestroyObject(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	GraphicsSystem::CShaderClassPtr& CEngine::FindShaderClass(const CAnsiString& name)
	{
		return m_ShaderClassManager.Find(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::ClearInvalidShaderClass(void)
	{
		m_ShaderClassManager.RemoveInvalidPtr();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSystem* CEngine::LoadSubSystem(const LYNXCHAR *name)
	{
		CSystem *lpSubSystem = NULL;

		if (!m_ConfigIniReader)
			m_ConfigIniReader = LYNXNEW CIniReader(m_ConfigFilename);

		CString SubSystem = m_ConfigIniReader->ReadString(name, _T("SubSystem"), _T(""));

		if (lynxStrStr(name, _T("Script")))
		{
#ifdef __LUA__
#ifdef __WINAPP__
			SubSystem = _T("Lua");
#endif
			if (!lynxStriCmp(SubSystem.c_str(), _T("Lua")))
				lpSubSystem = LYNXNEW LynxScript::Lua::CScriptSystem(this);
			//else
			//  lpSubSystem = LYNXNEW LynxScript::Lua::CScriptSystem(this);                    
#endif

#ifdef __MONO__
#ifdef __WINAPP__
			SubSystem = _T("Mono");
#endif
			if (!lynxStriCmp(SubSystem.c_str(), _T("Mono")))
				lpSubSystem = LYNXNEW LynxScript::Mono::CScriptSystem(this);
#endif
		}
		else if (lynxStrStr(name, _T("Physics")))
		{
			if (!lynxStriCmp(SubSystem.c_str(), _T("PhysX")))
			{
#if (defined (__WIN32__) && defined (__PHYSX__))					
				lpSubSystem = LYNXNEW LynxPhysics::Novodex::CPhysicsSystem(this);
#endif
			}
			else if (!lynxStriCmp(SubSystem.c_str(), _T("Bullet")))
			{
#ifdef __BULLET__	
				lpSubSystem = LYNXNEW LynxPhysics::Bullet::CPhysicsSystem(this);
#endif
			}
			else if (!lynxStriCmp(SubSystem.c_str(), _T("Box2D")))
			{
#ifdef __BOX2D__	
				lpSubSystem = LYNXNEW LynxPhysics::Box2D::CPhysicsSystem(this);
#endif
			}
		}
		else if (lynxStrStr(name, _T("Sound")))
		{
			if (!lynxStriCmp(SubSystem.c_str(), _T("OpenAL")))
			{
#ifdef __OPENAL__
				lpSubSystem = LYNXNEW LynxSound::OpenAL::CSoundSystem(this);
#endif
			}
			else if (!lynxStriCmp(SubSystem.c_str(), _T("OpenSL")))
			{
#ifdef __OPENSL__
				lpSubSystem = LYNXNEW LynxSound::OpenSL::CSoundSystem(this);
#endif
			}
		}

		return lpSubSystem;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::UnloadSubSystem(CSystem* lpss)
	{
		if (lpss)
			LYNXDEL lpss;

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::RegisterEvent(CEvent::TYPE t, CGameObj* obj)
	{
		m_EventRegisteredObjList[t].push_back(obj);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::BroadcastEvent(CEvent& e)
	{
		CList<CGameObj*>::CIterator Obj;

		switch (e.GetType())
		{
		case CEvent::SYS_KEY_DOWN:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].end(); ++Obj)
			{
				(*Obj)->vOnSysKeyDown(e.m_Param);
			}
			break;


		case CEvent::SYS_KEY_UP:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_KEY_DOWN].end(); ++Obj)
			{
				(*Obj)->vOnSysKeyUp(e.m_Param);
			}
			break;

		case CEvent::SYS_LBUTTON_DOWN:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_LBUTTON_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_LBUTTON_DOWN].end(); ++Obj)
			{
				(*Obj)->vOnSysLButtonDown();
			}
			break;

		case CEvent::SYS_LBUTTON_UP:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_LBUTTON_UP].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_LBUTTON_UP].end(); ++Obj)
			{
				(*Obj)->vOnSysLButtonUp();
			}
			break;

		case CEvent::SYS_RBUTTON_DOWN:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_RBUTTON_DOWN].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_RBUTTON_DOWN].end(); ++Obj)
			{
				(*Obj)->vOnSysRButtonDown();
			}
			break;

		case CEvent::SYS_RBUTTON_UP:
			for (Obj = m_EventRegisteredObjList[CEvent::SYS_RBUTTON_UP].begin(); Obj != m_EventRegisteredObjList[CEvent::SYS_RBUTTON_UP].end(); ++Obj)
			{
				(*Obj)->vOnSysRButtonUp();
			}
			break;

		default:
			for (Obj = m_EventRegisteredObjList[e.GetType()].begin(); Obj != m_EventRegisteredObjList[e.GetType()].end(); ++Obj)
			{
				(*Obj)->vOnEvent(e);
			}
			break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::vCreateSubSystems(GameFramework::CKernel::INIT_DESC* desc)
	{
		// Create file system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create file systenm\r\n")));
		m_lpFileSystem = LYNXNEW FileSystem::CFileSystem(this);
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
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create script systenm\r\n")));
		m_lpScriptSystem = (ScriptSystem::CScriptSystem*)LoadSubSystem(_T("Script"));
		if (m_lpScriptSystem)
		{
			if (!m_lpScriptSystem->vCreate())
				return LYNX_FALSE;
			m_lpScriptSystem->vInit();
		}

		//Create GUI system		
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create GUI systenm\r\n")));
		m_lpGUISystem = LYNXNEW GUISystem::CGUISystem(this);
		if (m_lpGUISystem)
		{
			if (!m_lpGUISystem->vCreate())
				return LYNX_FALSE;
		}
		// Create graphics system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create graphics systenm\r\n")));
		m_lpGraphicsSystem = LYNXNEW GraphicsSystem::CGraphicsSystem(this);
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

		//Create scene system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create scene systenm\r\n")));
		m_lpSceneSystem = LYNXNEW SceneSystem::CSceneSystem(this);
		if (!m_lpSceneSystem->vCreate())
			return LYNX_FALSE;

		//Create collision system		
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create collision systenm\r\n")));
		m_lpCollisionSystem = LYNXNEW CollisionSystem::CBtCollisionSystem(this);
		if (m_lpCollisionSystem && !m_lpCollisionSystem->vCreate())
			return LYNX_FALSE;

		// Create game console system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create game console systenm\r\n")));
		m_lpGameConsoleSystem = LYNXNEW GameConsoleSystem::CGameConsoleSystem(this);
		if (m_lpGameConsoleSystem)
		{
			if (!m_lpGameConsoleSystem->vCreate())
				return LYNX_FALSE;
		}

		// Create input system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create input systenm\r\n")));
		m_lpInputSystem = LYNXNEW InputSystem::CInputSystem(this);
		if (m_lpInputSystem)
		{
			if (!m_lpInputSystem->vCreate())
				return LYNX_FALSE;
		}

		// Create network system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create network systenm\r\n")));
		m_lpNetworkSystem = LYNXNEW NetworkSystem::CNetworkSystem(this);
		if (!m_lpNetworkSystem->vCreate())
			return LYNX_FALSE;

		// Create perf system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create perf systenm\r\n")));
		m_lpPerfSystem = LYNXNEW PerfSystem::CPerfSystem(this);
		if (!m_lpPerfSystem->vCreate())
			return LYNX_FALSE;

		// Create physics system
		//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load physics subsystem <"))+CString(PhysicsDLL)+CString(_T(">\r\n")));
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create physics systenm\r\n")));
		m_lpPhysicsSystem = (PhysicsSystem::CPhysicsSystem*)LoadSubSystem(_T("Physics"));//PhysicsDLL);		
		if (m_lpPhysicsSystem)
		{
			if (!m_lpPhysicsSystem->vCreate())
				return LYNX_FALSE;
		}

		// Create sound system
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Create sound systenm\r\n")));
		m_lpSoundSystem = (SoundSystem::CSoundSystem*)LoadSubSystem(_T("Sound"));
		if (m_lpSoundSystem)
		{
			if (!m_lpSoundSystem->vCreate())
				return LYNX_FALSE;
		}

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

		m_ConfigFilename = _T("../config/engine.ini");

		lynxSprintf(LogStr, _T("LynxEngine Version: %d.%d.%d\r\n"), GetMajorVersion(), GetMinorVersion(), GetBuildVersion());
		LYNX_LOG(glpLogger, CString(LogStr));

		lynxOSGetTime(&Time);
		lynxSprintf(LogStr, _T("%s::Start at %d.%02d.%02d %02d:%02d:%02d\r\n"), CString(GetClassName()).c_str(), Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);
		CString Log(LogStr);
		LYNX_LOG(glpLogger, Log);

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Munge FPCW\r\n")));
		lynxMungeFPCW();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init Math System\r\n")));
		lynxInitMathSystem();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init I/O System\r\n")));
		lynxInitIOSystem();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init File System\r\n")));
		lynxInitFileIOSystem();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init Memory System\r\n")));
		lynxInitMemSystem();
		lynxSprintf(LogStr, _T("Available physical memory size %.2f MB\r\n"), (float)lynxGetAvailablePhysicalMem() / (1024.0f*1024.0f));

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init CRC table\r\n")));
		lynxInitCRC();

		vRegisterDefaultResources();
		vRegisterDefaultContainers();
		vRegisterDefaultShaderClasses();

		// Create all sub systems
		vCreateSubSystems(desc);

		// Init script class here since script system was just created
		INIT_CLASSSCRIPT

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
		REGISTER_RESOURCE(BUFFER, _CreateBuffer);
		REGISTER_RESOURCE(VERTEX_LAYOUT, _CreateVertexLayout);
		REGISTER_RESOURCE(VERTEX_ARRAY, _CreateVertexArray);
		REGISTER_RESOURCE(INDEX_ARRAY, _CreateIndexArray);
		REGISTER_RESOURCE(TEXTURE_ANIMATION, _CreateTextureAnimation);
		REGISTER_RESOURCE(VERTEX_SHADER, _CreateVertexShader);
		REGISTER_RESOURCE(PIXEL_SHADER, _CreatePixelShader);
		REGISTER_RESOURCE(SHADER_GROUP, _CreateShaderGroup);
		REGISTER_RESOURCE(TEXTURE, _CreateTexture);
		REGISTER_RESOURCE(DEPTHSTENCIL_STATE, _CreateDepthStencilState);
		REGISTER_RESOURCE(BLEND_STATE, _CreateBlendState);
		REGISTER_RESOURCE(RASTERIZER_STATE, _CreateRasterizerState);
		REGISTER_RESOURCE(SAMPLER_STATE, _CreateSamplerState);
		REGISTER_RESOURCE(MATERIAL, _CreateMaterial);
		REGISTER_RESOURCE(SCRIPT, _CreateScript);
		REGISTER_RESOURCE(MUSIC, _CreateMusic);
		REGISTER_RESOURCE(SOUND, _CreateSound);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vRegisterDefaultContainers()
	{
		REGISTER_CONTAINER(CContainer::REFERENCE, CreateReference);
		REGISTER_CONTAINER(CContainer::GEOMETRY, CreateCGeometry);
		REGISTER_CONTAINER(CContainer::CAMERA, CreateCamera);
		REGISTER_CONTAINER(CContainer::MODEL, CreateModel);
		REGISTER_CONTAINER(CContainer::SPRITE, CreateSprite);
		REGISTER_CONTAINER(CContainer::SPRITE3D, CreateSprite3D);
		REGISTER_CONTAINER(CContainer::SPRITE3D_CLUSTER, CreateSprite3DCluster);
		REGISTER_CONTAINER(CContainer::SPRITE_CLUSTER, CreateSpriteCluster);
		REGISTER_CONTAINER(CContainer::VOXEL_CLUSTER, CreateVoxelCluster);
		REGISTER_CONTAINER(CContainer::CLUSTER_SUB, CreateClusterSub);
		REGISTER_CONTAINER(CContainer::PEMITTER, CreatePEmitter);
		REGISTER_CONTAINER(CContainer::MESH, CreateMesh);
		REGISTER_CONTAINER(CContainer::BONE, CreateBone);
		REGISTER_CONTAINER(CContainer::TERRAIN, CreateTerrain);
		REGISTER_CONTAINER(CContainer::SKYDOME, CreateSkyDome);
		REGISTER_CONTAINER(CContainer::GRASS, CreateGrass);
		REGISTER_CONTAINER(CContainer::CROSS_GRASS, CreateCrossGrass);
		REGISTER_CONTAINER(CContainer::VOLUMETRIC_CLOUD, CreateVolumetricCloud);

#if (defined(__WIN32__) && defined(__SPEEDTREE__))
		//temp code
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Register SpeedTree container\r\n")));
		m_ContainerFactory.Register(static_cast<CContainer::TYPE>(CSpeedTreeContainer::SPEEDTREE), CreateSpeedTree);
		//temp code
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vRegisterDefaultVertexLayouts()
	{
		REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_FIXED), CreateFixedVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_VECTOR3), CreateVector3VertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SCREEN_QUAD), CreateScreenQuadVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_FONT), CreateFontVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE), CreateSpriteVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE3D), CreateSprite3DVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_SPRITE3D_CLUSTER), CreateSprite3DClusterVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_VOXEL_CLUSTER), CreateVoxelClusterVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_STATIC), CreateStaticVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_RIGID), CreateRigidVertexLayout)
			REGISTER_VERTEXLAYOUT(GetEnumStr((GraphicsSystem::VERTEXTYPE)GraphicsSystem::VT_BLEND), CreateBlendVertexLayout)

			REGISTER_VERTEXLAYOUT(CString(_T("SkyDome Vertex Layout")), CSkyDomeContainer::CreateVertexLayout)
			REGISTER_VERTEXLAYOUT(CString(_T("Terrain Vertex Layout")), CTerrainContainer::CreateVertexLayout)
			REGISTER_VERTEXLAYOUT(CString(_T("SpriteGrass Vertex Layout")), CSpriteGrassContainer::CreateVertexLayout)
			REGISTER_VERTEXLAYOUT(CString(_T("CrossSpriteGrass Vertex Layout")), CCrossSpriteGrassContainer::CreateVertexLayout)
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
			REGISTER_SHADERCLASS(TintPS)
			REGISTER_SHADERCLASS(GrayBltPS)
			REGISTER_SHADERCLASS(ConstantColorPS)
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
			REGISTER_SHADERCLASS(VoxelSpriteVS)
			REGISTER_SHADERCLASS(VoxelSpriteOneLightingVS)
			REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingVS"), FALSE, FALSE, FALSE)
			REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingSpecularVS"), FALSE, FALSE, TRUE)
			REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOVS"), TRUE, FALSE, FALSE)
			REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOSpecularVS"), TRUE, FALSE, TRUE)
			REGISTER_VOXELCLUSTERVS(CAnsiString("VoxelClusterOneLightingFakeAOLevel2SpecularVS"), TRUE, TRUE, TRUE)

			REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorPS"), FALSE, FALSE)
			REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorFakeAOPS"), TRUE, FALSE)
			REGISTER_VOXELCOLORPS(CAnsiString("VoxelColorFakeAOLevel2PS"), TRUE, TRUE)
			/*
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

#if (defined(__WIN32__) && defined(__SPEEDTREE__))
			REGISTER_SHADERCLASS(SpeedTreeBranchVS)
			REGISTER_SHADERCLASS(SpeedTreeBranchZPassVS)
			REGISTER_SHADERCLASS(SpeedTreeBranchGPassVS)
			REGISTER_SHADERCLASS(SpeedTreeBranchShadowMapVS)
			REGISTER_SHADERCLASS(SpeedTreeBranchSimpleLightingVS)
			REGISTER_SHADERCLASS(SpeedTreeLeafVS)
			REGISTER_SHADERCLASS(SpeedTreeLeafZPassVS)
			REGISTER_SHADERCLASS(SpeedTreeLeafGPassVS)
			REGISTER_SHADERCLASS(SpeedTreeLeafShadowMapVS)
			REGISTER_SHADERCLASS(SpeedTreeLeafSimpleLightingVS)
#endif

			//Tool
			REGISTER_SHADERCLASS(TextureAnimationPreviewVS)
			REGISTER_SHADERCLASS(TextureAnimationPreviewPS)
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vInitSystemFont()
	{
		/*
		int Width = 28;
		int Height = 56;
		Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpGUISystem->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);

		int LargeFontWidth = LYNX_ROUND((float)Width * 2.3f * ScaleFactor.x);
		int LargeFontHeight = LYNX_ROUND((float)Height * 2.4f * ScaleFactor.y);

		int FontWidth = LargeFontWidth;
		int FontHeight = LargeFontHeight;
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 0+0x21,	15, 0, 0,		 Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 15+0x21,	10, 0, Height,	 Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 25+0x21,	7,	0, Height*2, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 32+0x21,	16, 0, Height*3, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 48+0x21,	10, 0, Height*4, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 58+0x21,	6,  0, Height*5, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 64+0x21,	16, 0, Height*6, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 80+0x21,	10, 0, Height*7, Width, Height-2);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 90+0x21,	4,  0, Height*8, Width, Height);

		FontWidth = LargeFontWidth * 0.5f;
		FontHeight = LargeFontHeight * 0.5f;
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 0+0x21,	15, 0, 0,		 Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 15+0x21,	10, 0, Height,	 Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 25+0x21,	7,	0, Height*2, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 32+0x21,	16, 0, Height*3, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 48+0x21,	10, 0, Height*4, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 58+0x21,	6,  0, Height*5, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 64+0x21,	16, 0, Height*6, Width, Height);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 80+0x21,	10, 0, Height*7, Width, Height-2);
		m_lpSystemFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 90+0x21,	4,  0, Height*8, Width, Height);

		FontWidth = LargeFontWidth * 0.25f;
		FontHeight = LargeFontHeight * 0.25f;
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 0+0x21,  15, 0, 0,	    Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 15+0x21, 10, 0, Height,   Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 25+0x21, 7,  0, Height*2, Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 32+0x21, 16, 0, Height*3, Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 48+0x21, 10, 0, Height*4, Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 58+0x21, 6,  0, Height*5, Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 64+0x21, 16, 0, Height*6, Width, Height);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 80+0x21, 10, 0, Height*7, Width, Height-2);
		m_lpSystemSmallFont->LoadFixed(_T("../texture/system_font_simhei.png"), FontWidth, FontHeight, 90+0x21, 4,  0, Height*8, Width, Height);
		*/


		/*
		LynxEngine::CString FontPath = LynxEngine::CString(_T("../font/eng/fonts.fnt"));
		LynxEngine::CTrueTypeFont::FontCreationDesc Desc;
		float Scale = 1.1f;
		Scale *= (96.0f / 64.0f);

		m_lpSystemLargeFont->LoadFnt(FontPath, Scale);
		m_lpSystemFont->LoadFnt(FontPath, Scale*0.5f);
		m_lpSystemSmallFont->LoadFnt(FontPath, Scale*0.25f);

		m_lpSystemFontString->SetFont(m_lpSystemFont);
		m_lpSystemFontString->Create(512, LYNX_TRUE);

		m_lpSystemFontString->SetFont(m_lpSystemFont);
		m_lpSystemFontString->Create(512, LYNX_TRUE);
		m_lpSystemFont->GetlpMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
		m_lpSystemFont->SetCharacterOffset(Math::CVector2(-6.0f, 0.0f));
		m_lpSystemSmallFont->SetCharacterOffset(Math::CVector2(-3.0f, 0.0f));
		m_lpSystemTTFontString->Create(32);
		*/

#ifdef __WIN32__
#ifdef __WINAPP__
#else
		// Init true type font system
		//FT_Face	 face;
		int error = FT_Init_FreeType(&m_FreeTypeLibrary);
		if (error)
		{
			OutputDebugStringA("FT_Init_FreeType fail\n");
		}
		//error = FT_New_Face(m_FreeTypeLibrary, "../font/msgothic.ttc", 0, &face);
		//if (error == FT_Err_Unknown_File_Format)
		//{    
		//}
		//else if (error)
		//{    
		//}  

		//error = FT_Set_Char_Size(
		//	face,    /* handle to face object           */
		//	0,       /* char_width in 1/64th of points  */
		//	16*64,   /* char_height in 1/64th of points */
		//	300,     /* horizontal device resolution    */
		//	300 );   /* vertical device resolution      */

		//error = FT_Set_Pixel_Sizes(
		//	face,   /* handle to face object */
		//	0,      /* pixel_width           */
		//	16 );   /* pixel_height          */

		// FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
#endif
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vInitLogoSprite()
	{
		CPlatformFileStream File;

		m_lpLogoSprite = Container_Cast<CSpriteContainer*>(CreateContainer(CContainer::SPRITE));
		LynxEngine::CSpriteCreationDesc Desc;
		m_lpGraphicsSystem->GetlpRenderer()->SetTexturePath(_T(""));
		Desc.Default();
		Desc.m_TextureDesc.MipMapLevel = 1;
		Desc.m_TextureDesc.bSRGB = LYNX_FALSE;
		Desc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
		Desc.m_lpStream = &File;
		Desc.m_FileName = _T("../texture/logo.png");
		Desc.m_bUseBufferObject = LYNX_TRUE;
		Desc.m_Number = 0;
		Desc.m_bInvertTexCoordV = LYNX_FALSE;
		Desc.m_Name = CString(_T("LogoSpriteContainer"));
		Desc.m_MaterialName = CString(_T("LogoSpriteContainer.Material"));
		m_lpLogoSprite->vCreate(&Desc);
		float Size = LYNX_MIN(m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth(), m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight());
		Size *= 0.7f;
		m_lpLogoSprite->vSetSize(Size, Size);
		m_lpLogoSprite->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
		LYNXRECT Rect;
		Rect.x1 = 0; Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth(); Rect.y1 = 0; Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();
		m_lpLogoSprite->SetAlignmentRect(&Rect);
		m_lpLogoSprite->SetOriginType(CSpriteContainer::OT_CENTER);
		m_lpLogoSprite->GetlpCurrentMaterial()->SetResourceFlag(LynxEngine::SYSTEM_DEFINED);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::LoadConfig(const LYNXCHAR* filename)
	{
		// Read init data from config file
		if (!m_ConfigIniReader)
			m_ConfigIniReader = LYNXNEW CIniReader(m_ConfigFilename);

		m_bLockFrameRate = m_ConfigIniReader->ReadBool(_T("Engine"), _T("LockFrameRate"), LYNX_FALSE);
		m_UpdateRate = (float)m_ConfigIniReader->ReadInt(_T("Engine"), _T("UpdateRate"), 60);

		if (m_lpPhysicsSystem)
		{
			if (!m_lpPhysicsSystem->vInit())
			{
				UnloadSubSystem(m_lpPhysicsSystem);
				m_lpPhysicsSystem = NULL;
			}
			m_lpPhysicsSystem->m_UpdateRate = (float)m_ConfigIniReader->ReadInt(_T("Physics"), _T("UpdateRate"), 30);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::vInit(LYNXWINDOW &win, const int w, const int h, const LYNXBOOL bfull)
	{
		CObj::vInit();

		lynxSrand(lynxOSGetPerformanceCounter());

		if (m_lpPerfSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm...\r\n")));
			if (!m_lpPerfSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm OK\r\n")));
		}

		if (m_lpNetworkSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm...\r\n")));
			if (!m_lpNetworkSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm OK\r\n")));
		}

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm...\r\n")));
		if (m_lpGUISystem)
			m_lpGUISystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm OK\r\n")));

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init graphics systenm...\r\n")));
		if (!m_lpGraphicsSystem->vInit(win, w, h, bfull))
			return LYNX_FALSE;
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init graphics systenm OK\r\n")));

		if (m_lpPhysicsSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm...\r\n")));
			if (!m_lpPhysicsSystem->vInit())
			{
				UnloadSubSystem(m_lpPhysicsSystem);
				m_lpPhysicsSystem = NULL;
			}
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm OK\r\n")));
		}

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm...\r\n")));
		if (m_lpGameConsoleSystem)
			m_lpGameConsoleSystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm OK\r\n")));

		if (m_lpInputSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm...\r\n")));
			m_lpInputSystem->vInit();
#ifdef __DESKTOP__
			m_lpSystemKeyboard = (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);
			if (m_lpSystemKeyboard)
			{
				m_lpSystemKeyboard->SetName(_T("System Keyboard"));
				#if (defined(__CONTROLPANEL__))	
					if (gControlPanelWnd)
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_FALSE;
					}
					else
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
					}
				#else
					m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
				#endif
			}

			m_lpSystemMouse = (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);
			if (m_lpSystemMouse)
				m_lpSystemMouse->SetName(_T("System Mouse"));

			if (m_bUseTouchDevice)
			{
				lynxOSRegisterTouchWindow(win, LYNX_TRUE);

				if (lynxGetMaxTouches() > 0)
				{
					m_lpSystemTouch = (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);
					if (m_lpSystemTouch)
						m_lpSystemTouch->SetName(_T("System Touch"));
				}
			}

			m_SystemGamePadArray.push_back((LynxEngine::InputSystem::CGamePad *)m_lpInputSystem->CreateDevice(InputSystem::GAMEPAD));
			if (m_SystemGamePadArray[0])
			{
				m_SystemGamePadArray[0]->SetName(_T("System Game Pad"));
				m_SystemGamePadArray[0]->SetControllerIndex(0);
			}
#elif defined(__MOBILE__)
			m_lpSystemTouch = (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);
			m_lpSystemTouch->SetName(_T("System Touch"));
#endif
			m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
			if (m_lpSystemInputRecord)
				m_lpSystemInputRecord->SetName(_T("System Input Record"));

			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm OK\r\n")));
		}

		if (m_lpSoundSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm...\r\n")));
			m_lpSoundSystem->vInit();
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm OK\r\n")));
		}

		vInitSystemFont();
		//vInitLogoSprite();
		vInitGameScripts();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file...\r\n")));
		LoadConfig(m_ConfigFilename);
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file OK\r\n")));

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

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm...\r\n")));
		if (m_lpGUISystem)
			m_lpGUISystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm OK\r\n")));

		if (!m_lpGraphicsSystem->vInit(win, lpcp))
			return LYNX_FALSE;

		if (m_lpPerfSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm...\r\n")));
			if (!m_lpPerfSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm OK\r\n")));
		}

		if (m_lpNetworkSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm...\r\n")));
			if (!m_lpNetworkSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm OK\r\n")));
		}

		if (m_lpPhysicsSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm...\r\n")));
			if (!m_lpPhysicsSystem->vInit())
			{
				UnloadSubSystem(m_lpPhysicsSystem);
				m_lpPhysicsSystem = NULL;
			}
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm OK\r\n")));
		}

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm...\r\n")));
		if (m_lpGameConsoleSystem)
			m_lpGameConsoleSystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm OK\r\n")));

		if (m_lpInputSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm...\r\n")));
			m_lpInputSystem->vInit();
#ifdef __DESKTOP__
			m_lpSystemKeyboard = (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);
			if (m_lpSystemKeyboard)
			{
				m_lpSystemKeyboard->SetName(_T("System Keyboard"));
				#if (defined(__CONTROLPANEL__))	
					if (gControlPanelWnd)
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_FALSE;
					}
					else
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
					}
				#else
					m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
				#endif
			}
			m_lpSystemMouse = (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);
			m_lpSystemMouse->SetName(_T("System Mouse"));
#elif defined(__MOBILE__)
			m_lpSystemTouch = (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);
			m_lpSystemTouch->SetName(_T("System Touch"));
#endif
			m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
			if (m_lpSystemInputRecord)
				m_lpSystemInputRecord->SetName(_T("System Input Record"));
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm OK\r\n")));
		}

		if (m_lpSoundSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm...\r\n")));
			m_lpSoundSystem->vInit();
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm OK\r\n")));
		}

		vInitSystemFont();
		//vInitLogoSprite();
		vInitGameScripts();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file...\r\n")));
		LoadConfig(m_ConfigFilename);
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file OK\r\n")));

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

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm...\r\n")));
		if (m_lpGUISystem)
			m_lpGUISystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init GUI systenm OK\r\n")));

		if (!m_lpGraphicsSystem->vInit(win, &GraphicParam))
			return LYNX_FALSE;

		if (m_lpPerfSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm...\r\n")));
			if (!m_lpPerfSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init perf systenm OK\r\n")));
		}

		if (m_lpNetworkSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm...\r\n")));
			if (!m_lpNetworkSystem->vInit())
				return LYNX_FALSE;
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init network systenm OK\r\n")));
		}

		if (m_lpPhysicsSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm...\r\n")));
			if (!m_lpPhysicsSystem->vInit())
			{
				UnloadSubSystem(m_lpPhysicsSystem);
				m_lpPhysicsSystem = NULL;
			}
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init physics systenm OK\r\n")));
		}

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm...\r\n")));
		if (m_lpGameConsoleSystem)
			m_lpGameConsoleSystem->vInit();
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init console systenm OK\r\n")));

		if (m_lpInputSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm...\r\n")));
			m_lpInputSystem->vInit();
#ifdef __DESKTOP__
			m_lpSystemKeyboard = (LynxEngine::InputSystem::CKeyboard *)m_lpInputSystem->CreateDevice(InputSystem::KEYBOARD);
			if (m_lpSystemKeyboard)
			{
				m_lpSystemKeyboard->SetName(_T("System Keyboard"));
				#if (defined(__CONTROLPANEL__))	
					if (gControlPanelWnd)
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_FALSE;
					}
					else
					{
						m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
					}
				#else
					m_lpSystemKeyboard->m_bCheckingFocus = LYNX_TRUE;
				#endif
			}
			m_lpSystemMouse = (LynxEngine::InputSystem::CMouse *)m_lpInputSystem->CreateDevice(InputSystem::MOUSE);
			m_lpSystemMouse->SetName(_T("System Mouse"));
#elif defined(__MOBILE__)
			m_lpSystemTouch = (LynxEngine::InputSystem::CTouch *)m_lpInputSystem->CreateDevice(InputSystem::TOUCH);
			m_lpSystemTouch->SetName(_T("System Touch"));
#endif
			m_lpSystemInputRecord = (LynxEngine::InputSystem::CInputRecord *)m_lpInputSystem->CreateDevice(InputSystem::INPUT_RECORD);
			if (m_lpSystemInputRecord)
				m_lpSystemInputRecord->SetName(_T("System Input Record"));
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init input systenm OK\r\n")));
		}

		if (m_lpSoundSystem)
		{
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm...\r\n")));
			m_lpSoundSystem->vInit();
			LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Init sound systenm OK\r\n")));
		}

		vInitSystemFont();
		//vInitLogoSprite();
		vInitGameScripts();

		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file...\r\n")));
		LoadConfig(m_ConfigFilename);
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Load confog file OK\r\n")));

		return LYNX_TRUE;
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	PhysicsSystem::CPhysicsObj* CEngine::vCreatePhysicsObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->vCreateObj(t, g);
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vDestroyPhysicsObj(PhysicsSystem::CPhysicsObj* o)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->vDeleteObj(o);
		}
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	PhysicsSystem::CPhysicsShape* CEngine::vCreatePhysicsShape(PhysicsSystem::CPhysicsShape::TYPE type, const CCreationDesc *desc, PhysicsSystem::CPhysicsObj* lpo)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->GetlpSpace(0)->vCreateShape(type, desc, lpo);
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vDestroyPhysicsShape(PhysicsSystem::CPhysicsShape* s)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->GetlpSpace(0)->vDeleteShape(s);
		}
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	PhysicsSystem::CPhysicsJoint* CEngine::vCreatePhysicsJoint(PhysicsSystem::CPhysicsJoint::TYPE type, const CCreationDesc* desc)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->vCreateJoint(type, desc);
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vDestroyPhysicsJoint(PhysicsSystem::CPhysicsJoint* j)
	{
		if (m_lpPhysicsSystem)
		{
			return m_lpPhysicsSystem->GetlpPhysicsWorld(0)->vDeleteJoint(j);
		}
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::InitShowingData()
	{
		LYNXCHAR Buffer[256];
		LYNXCOLORRGBA TitleColor = { 255, 255, 0, 255 };
		LYNXCOLORRGBA Color = { 255, 255, 255, 255 }, ShadowColor = { 0, 0, 0, 255 };

		m_lpGraphicsSystem->GetlpScreenOutputDevice()->vClear();

		switch (m_ShowMode)
		{
		case SHOW_RENDERPROFILE:
			break;

		case SHOW_RESOURCEDATA:
		{
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_ResourceManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				lynxSprintf(Buffer, _T("%s, Type: %s, Count: %d"), (*Res)->GetName().c_str(), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str(), (*Res).GetCount());
				m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
				Num++;
			}
		}
		break;

		case SHOW_BUFFERDATA:
		{
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_ResourceManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				if ((*Res)->GetResourceType() == BUFFER)
				{
					lynxSprintf(Buffer, _T("%s, Count: %d"), (*Res)->GetName().c_str(), (*Res).GetCount());
					m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
					Num++;
				}
			}
		}
		break;

		case SHOW_TEXTUREDATA:
		{
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_ResourceManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				if ((*Res)->GetResourceType() == TEXTURE)
				{
					lynxSprintf(Buffer, _T("%s, Count: %d"), (*Res)->GetName().c_str(), (*Res).GetCount());
					m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
					Num++;
				}
			}
		}
		break;

		case SHOW_SHADERDATA:
		{
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_ResourceManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				if ((*Res)->GetResourceType() == VERTEX_SHADER || (*Res)->GetResourceType() == PIXEL_SHADER)
				{
					lynxSprintf(Buffer, _T("%s, Count: %d"), (*Res)->GetName().c_str(), (*Res).GetCount());
					m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
					Num++;
				}
			}
		}
		break;

		case SHOW_SHADERCLASSDATA:
		{
			CList<GraphicsSystem::CShaderClassPtr>::CIterator Res;
			CList<GraphicsSystem::CShaderClassPtr>& ResList = m_ShaderClassManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				lynxSprintf(Buffer, _T("%s, Count: %d"), (*Res)->GetName().c_str(), (*Res).GetCount());
				m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
				Num++;
			}
		}
		break;

		case SHOW_MATERIALDATA:
		{
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_ResourceManager.GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				if ((*Res)->GetResourceType() == MATERIAL)
				{
					lynxSprintf(Buffer, _T("%s, Count: %d"), (*Res)->GetName().c_str(), (*Res).GetCount());
					m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10 + 4 * 5, CString(Buffer), &Color, &ShadowColor);
					Num++;
				}
			}
		}
		break;

		case SHOW_MAPS:
			break;
		}
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::ShowMap(GraphicsSystem::CMap* map)
	{
		if (dynamic_cast<GraphicsSystem::CToneMap*>(map))
			m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetUtilTexture(0));
		else if (dynamic_cast<GraphicsSystem::CCubeShadowMap*>(map))
		{
			//map->GetLevel(0, 0);
			//m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetTexture());			

			m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			LYNXRECT Rect;
			int CX = 10;
			for (int i = 0; i<6; i++)
			{
				map->GetLevel(i, 0);
				//m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetTexture(), CX, 10, 100, 100);				
				Rect.x1 = CX;
				Rect.y1 = 10;
				Rect.x2 = Rect.x1 + 100;
				Rect.y2 = Rect.y1 + 100;
				m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetTexture(), &Rect);
				CX += 110;
			}
		}
		else if (dynamic_cast<GraphicsSystem::CShadowMap*>(map))
		{
			//m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetUtilTexture(0));			
			m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			int CX = 10;
			for (int i = 0; i<GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap; i++)
			{
				if (i == GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.NumSplittedShadowMap - 1)
					m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetTexture(), CX, 10, 100, 100);
				else
					m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetUtilTexture(i), CX, 10, 100, 100);

				CX += 110;
			}
		}
		else
		{
			//m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetTexture());		
			//m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::ALPHA_BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			m_lpGraphicsSystem->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
			m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetTexture(), 10, 10, 200, 200);
			//m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetTexture(), 10, 768-250, 320, 240);
			//m_lpGraphicsSystem->DrawScreenSpaceQuad(map->GetCurrent(), 350, 768-250, 320, 240);
		}
		//m_lpGraphicsSystem->BltTextureToBackBuffer(map->GetCurrent());		
	}
	//------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::ShowData()
	{
		LYNXRECT Rect;
		LYNXCOLORRGBA TitleColor = { 255, 255, 0, 255 };
		LYNXCOLORRGBA Color = { 255, 255, 255, 255 }, ShadowColor = { 0, 0, 0, 255 };

		Rect.x1 = 0;
		Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();
		Rect.y1 = 0;
		Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

		switch (m_ShowMode)
		{
		case SHOW_RESOURCEDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("Resource Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_BUFFERDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("Buffer Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_TEXTUREDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("Texture Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_SHADERDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("Shader Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_SHADERCLASSDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("ShaderClass Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_MATERIALDATA:
			m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, _T("Material Data-----------------------------------------------------------------------"));
			m_lpGraphicsSystem->GetlpScreenOutputDevice()->Show();
			break;

		case SHOW_RENDERPROFILE:
		{
			int Y = 10;
#ifdef __PROFILE__
			DWORD DrawCall = m_lpGraphicsSystem->GetlpRenderer()->GetDrawCallCounter();
			DWORD DrawTime = m_lpGraphicsSystem->GetlpRenderer()->GetDrawTimeCounter();
			DWORD Primitives = m_lpGraphicsSystem->GetlpRenderer()->GetPrimitiveCounter();
			DWORD SetRS = m_lpGraphicsSystem->GetlpRenderer()->GetSetRSCounter();
			DWORD SetTexture = m_lpGraphicsSystem->GetlpRenderer()->GetSetTextureCounter();
			DWORD SetShader = m_lpGraphicsSystem->GetlpRenderer()->GetSetShaderCounter();
			DWORD SetShaderConst = m_lpGraphicsSystem->GetlpRenderer()->GetSetShaderConstantCounter();
			DWORD BufferMap = m_lpGraphicsSystem->GetlpRenderer()->GetBufferMapCounter();
			m_lpGraphicsSystem->Printf(&Rect, 10, Y, &TitleColor, &ShadowColor, _T("Render Profile: %s --------------------------------------------------------------------"), GetlpGraphicsSystem()->GetlpRenderer()->GetDriverDesc());
			Y += 18;
			m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("FPS: %d, Step: %2.2f"), LYNX_ROUND(1000.0f / GetlpKernel()->GetTotalFrameElapsedTime()), m_Step);
			Y += 18;
			m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("Draw Call: %d(%d), Primitives: %d"), DrawCall, DrawTime, Primitives);
			Y += 18;
			m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("Set RS: %d, Texture: %d, Shader: %d, Shader Constant: %d, Buffer Map: %d"), SetRS, SetTexture, SetShader, SetShaderConst, BufferMap);
			Y += 18;

			DWORD VideoMemeoryUsage = 0;
			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				if ((*Res))
				{
					VideoMemeoryUsage += (*Res)->GetVideoMemoryUsage();
				}
			}
			m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("VMEM : %.2fM"), (float)VideoMemeoryUsage / (1024.0f*1024.0f));
			Y += 18;

			if (m_lpSceneSystem->GetlpCurrentScene())
			{
				DWORD NumVisObj = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetProfile().NumVisObj;
				DWORD NumVisContainer = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetProfile().NumVisContainer;
				DWORD NumRenderedContainer = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetProfile().NumRenderedContainer;
				DWORD NumVisLights = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetProfile().NumVisLight;
				DWORD NumRenderedLights = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetProfile().NumRenderedLight;

				m_lpGraphicsSystem->Printf(&Rect, 10, Y, &TitleColor, &ShadowColor, _T("Scene Profile: %s --------------------------------------------------------------------"), m_lpSceneSystem->GetlpCurrentScene()->GetName().c_str());
				Y += 18;
				m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("VisObj: %d, Vis/Ren Container: %d/%d"), NumVisObj, NumVisContainer, NumRenderedContainer);
				Y += 18;
				m_lpGraphicsSystem->Printf(&Rect, 10 + 4 * 5, Y, &Color, &ShadowColor, _T("Vis/Ren Light: %d/%d"), NumVisLights, NumRenderedLights);
				Y += 18;
			}
#endif
		}
		break;

		case SHOW_MAPS:
			ShowMap(m_ShowedMap);
			break;

		case SHOW_FPS:
		{
			// Show fps
			LYNXCOLORRGBA FontColor;

			FontColor.Red = 204;
			FontColor.Green = 152;
			FontColor.Blue = 49;
			FontColor.Alpha = 255;

			GetSystemFontString()->SetFont(GetlpEngine()->GetSystemSmallFont());
			GetSystemFontString()->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			GetSystemFontString()->SetOriginType(CSpriteContainer::OT_CENTER);

			int FPS = LYNX_ROUND(1000.0f / GetlpKernel()->GetTotalFrameElapsedTime());
#ifdef __DESKTOP__
			if (FPS < 60)
			{
				FontColor.Red = 255;
				FontColor.Green = 0;
				FontColor.Blue = 0;
				FontColor.Alpha = 255;
			}
#endif

#ifdef __MOBILE__
			if (FPS < 20)
			{
				FontColor.Red = 255;
				FontColor.Green = 0;
				FontColor.Blue = 0;
				FontColor.Alpha = 255;
			}
#endif
			GetlpEngine()->GetSystemFontString()->Printf(0, 0, &FontColor, _T("FPS:%03d, Step:%01.1f, Device:%s"), FPS, m_Step, LynxEngine::GetEnumStr(m_lpKernel->GetDeviceType()).c_str());
			// Show fps
			break;
		}

		case SHOW_NONE:
		default:
			break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vBeginRendering(CCameraContainer& camera)
	{
		//GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->SetlpCurrentLightingPassTarget(NULL);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::Render()
	{
		CCameraContainer* lpCameraContainer = NULL;

		if (GetlpSceneSystem()->GetlpCurrentScene())
		{
			SceneSystem::CCamera* lpCamera = GetlpSceneSystem()->GetlpCurrentScene()->GetlpCurrentCamera();
			if (lpCamera)
			{
				lpCameraContainer = (CCameraContainer*)lpCamera->GetlpContainer();
			}

			LYNX_ASSERT(lpCameraContainer != NULL);

			vBeginRendering(*lpCameraContainer);
			GetlpSceneSystem()->GetlpCurrentScene()->Render(*lpCameraContainer);
			vFinishRendering(*lpCameraContainer);
		}
		else
		{
			Math::CColor Color(62.0f / 255.0f, 60.0f / 255.0f, 58.0f / 255.0f, 255.0f / 255.0f);
			m_lpGraphicsSystem->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER | GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);
			m_lpLogoSprite->vRender(NULL);
		}
		//ShowData();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vRender(CCameraContainer& camera)
	{
		if (GetlpSceneSystem()->GetlpCurrentScene())
		{
			GetlpSceneSystem()->GetlpCurrentScene()->Render(camera);
		}
		else
		{
			Math::CColor Color(62.0f / 255.0f, 60.0f / 255.0f, 58.0f / 255.0f, 255.0f / 255.0f);
			m_lpGraphicsSystem->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::RENDER_BUFFER | GraphicsSystem::CRenderer::DEPTH_BUFFER, Color, 1.0f, 0);
			m_lpLogoSprite->vRender(&camera);
		}
		//ShowData();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vFinishRendering(CCameraContainer& camera)
	{
		static float IntoConsoleTime = 0.0f;

		if (GetlpSceneSystem()->GetlpCurrentScene())
		{
			if (!GetlpGameConsoleSystem()->IsRunning())
			{
				IntoConsoleTime = 0;

				if (GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget() && GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpTargetBuffer())
				{
					//GetlpGraphicsSystem()->GetlpRenderer()->Finish();
					GetlpGraphicsSystem()->GetlpRenderer()->Flush();

					if (camera.GetViewportRect().x2 == camera.GetViewportRect().x1)  // that means the viewport is full screen, so just blt texture  
						GetlpGraphicsSystem()->BltTextureToBackBuffer(GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget()->GetTexture());
					else
					{
						GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);
						GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
						camera.SetViewportRect();
						GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
						GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetPS()->GetlpShader()->vSetTexture("DiffuseMap", GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget()->GetTexture());
						GetlpGraphicsSystem()->DrawScreenSpaceQuad();
					}
				}
			}
			else
			{
				GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
				GetlpGraphicsSystem()->GetlpOffscreen()->ResetCurrent();
				GetlpGraphicsSystem()->GetlpOffscreen()->DownSampled();
				int NumBlur = LYNX_CLAMP(LYNX_ROUND(IntoConsoleTime), 1, 8);
				//GetlpGraphicsSystem()->GetlpOffscreen()->BoxBlur(NumBlur, 2.5f);									
				//GetlpGraphicsSystem()->GetlpOffscreen()->Gaussin4x4Blur(NumBlur, 2.0f);									
				GetlpGraphicsSystem()->GetlpOffscreen()->GaussinBlur(NumBlur, 2.0f);

				if (GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget())
					GetlpGraphicsSystem()->BltTextureToBackBuffer(GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget()->GetCurrent());

				IntoConsoleTime += 1.0f;//*m_Step;
				if (IntoConsoleTime > 8)
					IntoConsoleTime = 8;
			}
			/*
			if (GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->GetlpCurrentLightingPassTarget())
			{
			GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);
			GetlpGraphicsSystem()->SetViewportRect();
			}
			*/
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::SwapBackBuffer(void)
	{
		m_lpGraphicsSystem->GetlpRenderer()->SwapBackBuffer();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::SetStepMultiplier(const float sf)
	{
		m_StepMultiplier = sf;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	float CEngine::GetGUIStep() const
	{
		return m_lpKernel->m_Step;
		//return ((m_StepMultiplier == 0.0f) ? 0.0f : (m_Step/m_StepMultiplier)); 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::RecursiveLoop(float step)
	{
		if (m_lpInputSystem)
		{
			m_lpInputSystem->vLoop(step);
		}

		if (m_lpGameConsoleSystem->IsRunning())
		{
			m_lpGameConsoleSystem->vLoop(step);
		}
		else
		{
			if (m_bLockFrameRate)
			{
				m_SceneFrameTime += (float)m_lpKernel->GetFrameElapsedTime();
				if (m_SceneFrameTime >= (1000.0f / m_lpEngine->m_UpdateRate))
				{
					m_Step = ((float)m_SceneFrameTime) / (1000.0f / m_lpEngine->m_UpdateRate) * m_StepMultiplier;
					if (m_Step > 5.0f)
						m_Step = 5.0f;

					if (GetlpSceneSystem()->GetlpCurrentScene())
					{
						GetlpSceneSystem()->GetlpCurrentScene()->vLoop(m_Step);
					}
					m_SceneFrameTime = 0.0f;
				}
			}
			else
			{
				m_Step = step * m_StepMultiplier;
				if (m_Step > 5.0f)
					m_Step = 5.0f;
				if (GetlpSceneSystem()->GetlpCurrentScene())
				{
					GetlpSceneSystem()->GetlpCurrentScene()->vLoop(m_Step);
				}
			}

			if (m_lpPhysicsSystem)
			{
				// Update physics based on update rate of physics system
				m_PhysicsFrameTime += (float)m_lpKernel->GetFrameElapsedTime();
				if (m_PhysicsFrameTime >= (1000.0f / m_lpPhysicsSystem->m_UpdateRate))
				{
					float Step = ((float)m_PhysicsFrameTime) / 1000.0f;
					m_lpPhysicsSystem->vLoop(Step);
					m_PhysicsFrameTime = 0.0f;
				}
			}

			if (m_lpSoundSystem)
			{
				m_lpSoundSystem->vLoop(step);
			}			
		}
		if (m_bRegularGC)
			GC(LYNX_FALSE);
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說�?:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::vLoop(float step)
	{
		int Counter = 1;
		float Step = step;

		if (m_FineStepData.bFineStepMode)
		{
			ComputeNewStep(&Step, &Counter, step, m_FineStepData.MinStep, m_FineStepData.MaxCountOfLoop);
		}
		for (int i = 0; i < Counter; i++)
		{
			RecursiveLoop(Step);
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::OpenScene(const LYNXCHAR* filename)
	{
		GetlpSceneSystem()->RemoveAllScenes();

		SceneSystem::CScene* lpScn = GetlpSceneSystem()->AddScene();
		GetlpSceneSystem()->SetCurrentScene(lpScn);
		m_lpGraphicsSystem->GetlpRenderer()->SetTexturePath(_T(""));

		CPlatformFileStream FileStream;
		GetlpSceneSystem()->GetlpCurrentScene()->Load(FileStream, filename);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::OpenScene(const CCreationDesc* desc)
	{
		GetlpSceneSystem()->RemoveAllScenes();

		SceneSystem::CScene* lpScn = GetlpSceneSystem()->AddScene(desc);
		GetlpSceneSystem()->SetCurrentScene(lpScn);
		m_lpGraphicsSystem->GetlpRenderer()->SetTexturePath(_T(""));

		CPlatformFileStream FileStream;
		GetlpSceneSystem()->GetlpCurrentScene()->Load(FileStream, desc->m_FileName);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vOnRenderMode(int num, const LYNXCHAR arg[][64])
	{
		if (num)
		{
			if (!lynxStriCmp(arg[0], _T("simple")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_SIMPLE;
			else if (!lynxStriCmp(arg[0], _T("simplelighting")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_SIMPLELIGHTING;
			else if (!lynxStriCmp(arg[0], _T("normal")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_NORMAL;
			else if (!lynxStriCmp(arg[0], _T("wireframe")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_WIREFRAME;
			else if (!lynxStriCmp(arg[0], _T("lightingonly")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_LIGHTINGONLY;
			else if (!lynxStriCmp(arg[0], _T("depthcomplex")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_DEPTHCOMPLEX;
			else if (!lynxStriCmp(arg[0], _T("ssaoonly")))
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_SSAOONLY;
			else if (!lynxStriCmp(arg[0], _T("ssao")))
			{
				if (!lynxStriCmp(arg[1], _T("on")))
					GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bSSAOPass = LYNX_TRUE;
				else
					GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bSSAOPass = LYNX_FALSE;
			}
			else if (!lynxStriCmp(arg[0], _T("glow")))
			{
				if (!lynxStriCmp(arg[1], _T("on")))
					GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bGlowPass = LYNX_TRUE;
				else
					GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bGlowPass = LYNX_FALSE;
			}
			else
			{
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bSSAOPass = LYNX_TRUE;
				GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_NORMAL;
			}
		}
		else
		{
			GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.bSSAOPass = LYNX_TRUE;
			GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_RenderingOptions.RenderMode = SceneSystem::CRenderer::RENDER_NORMAL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vOnOpenScene(const LYNXCHAR* filename)
	{
		OpenScene(filename);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vOnExit()
	{
		LYNX_LOG(glpLogger, CString(_T("---------------------------------------------------------------------------------------------\r\n")));
		LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::vOnExit()\r\n")));
		//m_lpKernel->GetCurrentUIPage()->SetQuit();		
		m_lpKernel->GetCurrentUIPage()->vDestroy();
		m_lpKernel->Quit();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vOnShow(int num, const LYNXCHAR arg[][64])
	{
		if (num)
		{
			if (!lynxStriCmp(arg[0], _T("renderprofile")))
			{
				m_ShowMode = SHOW_RENDERPROFILE;
			}
			else if (!lynxStriCmp(arg[0], _T("resourcedata")))
			{
				m_ShowMode = SHOW_RESOURCEDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("bufferdata")))
			{
				m_ShowMode = SHOW_BUFFERDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("texturedata")))
			{
				m_ShowMode = SHOW_TEXTUREDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("shaderdata")))
			{
				m_ShowMode = SHOW_SHADERDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("shaderclassdata")))
			{
				m_ShowMode = SHOW_SHADERCLASSDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("materialdata")))
			{
				m_ShowMode = SHOW_MATERIALDATA;
			}
			else if (!lynxStriCmp(arg[0], _T("bloomingmap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::GLOWMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("velocitymap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::VELOCITYMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("depthmap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::DEPTHMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("lightingtarget")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::LIGHTINGPASS_TARGET);
				}
			}
			/*
			else if (!lynxStriCmp(arg[0], _T("diffusemap")))
			{
			m_ShowMode = SHOW_MAPS;
			m_ShowedMap = m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::GBUFFER_DIFFUSE);
			}
			else if (!lynxStriCmp(arg[0], _T("specularmap")))
			{
			m_ShowMode = SHOW_MAPS;
			m_ShowedMap = m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::GBUFFER_SPECULAR);
			}
			else if (!lynxStriCmp(arg[0], _T("normalmap")))
			{
			m_ShowMode = SHOW_MAPS;
			m_ShowedMap = m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::GBUFFER_NORMAL);
			}
			else if (!lynxStriCmp(arg[0], _T("glowmap")))
			{
			m_ShowMode = SHOW_MAPS;
			m_ShowedMap = m_lpGraphicsSystem->GetlpMap(GraphicsSystem::CGraphicsSystem::GBUFFER_GLOW);
			}
			*/
			else if (!lynxStriCmp(arg[0], _T("shadowmap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::SHARED_SHADOWMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("cubeshadowmap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::SHARED_CUBESHADOWMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("tonemap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::TONEMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("ssaomap")))
			{
				if (m_lpSceneSystem->GetlpCurrentScene() && m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer())
				{
					m_ShowMode = SHOW_MAPS;
					m_ShowedMap = m_lpSceneSystem->GetlpCurrentScene()->GetlpRenderer()->GetlpMap(SceneSystem::CRenderer::SSAOMAP);
				}
			}
			else if (!lynxStriCmp(arg[0], _T("skylightmap")))
			{
				m_ShowMode = SHOW_MAPS;
				if (GetlpSceneSystem()->GetlpScene(0)->GetlpAtmosphere()->GetlpSky())
				{
					m_ShowedMap = &((CSkyDomeContainer*)(&(*(GetlpSceneSystem()->GetlpScene(0)->GetlpAtmosphere()->GetlpSky()->GetlpContainer()))))->GetSkyLightMap();
				}
			}
			else
			{
				m_ShowMode = SHOW_NONE;
			}
			InitShowingData();
		}
		else
		{
			m_ShowMode = SHOW_NONE;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::vOnResize(const LYNXCHAR arg1[64], const LYNXCHAR arg2[64])
	{
		int X = lynxAtoi(arg1);
		int Y = lynxAtoi(arg2);
		m_lpGraphicsSystem->Resize(X, Y);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	PhysicsSystem::CPhysicsWorld* CEngine::CreatePhysicsWorld()
	{
		if (m_lpPhysicsSystem)
			return m_lpPhysicsSystem->vCreateWorld();
		else
			return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "OpenPackage", LynxEngine::CEngine, LYNXBOOL, OpenPackage, (const LYNXCHAR *))
			REGISTER_SCRIPT_METHOD(lps, "ClosePackage", LynxEngine::CEngine, void, ClosePackage, ())

			REGISTER_SCRIPT_METHOD(lps, "CreateUIPage", LynxEngine::CEngine, GameFramework::CUIPage*, CreateUIPage, (const LYNXCHAR *, const LYNXCHAR *, const LYNXCHAR *))
			REGISTER_SCRIPT_METHOD(lps, "CreateNativeUIPage", LynxEngine::CEngine, GameFramework::CUIPage*, CreateUIPage, (const LYNXCHAR *, const LYNXCHAR *))

			REGISTER_SCRIPT_METHOD(lps, "DeleteLastUIPage", LynxEngine::CEngine, void, DeleteLastUIPage, (void))
			REGISTER_SCRIPT_METHOD(lps, "SetCurrentUIPage", LynxEngine::CEngine, void, SetCurrentUIPage, (void*))
			REGISTER_SCRIPT_METHOD(lps, "FindUIPage", LynxEngine::CEngine, void*, FindUIPage, (const LYNXCHAR*))
			REGISTER_SCRIPT_METHOD(lps, "SetSwipeSpeed", LynxEngine::CEngine, void, SetSwipeSpeed, (float))
			REGISTER_SCRIPT_METHOD(lps, "SetFadeOutSpeed", LynxEngine::CEngine, void, SetFadeOutSpeed, (float))
			REGISTER_SCRIPT_METHOD(lps, "SetUITransitionMode", LynxEngine::CEngine, void, SetUITransitionMode, (int))

			REGISTER_SCRIPT_METHOD(lps, "CreatePEmitterContainer", LynxEngine::CEngine, void*, _CreatePEmitterContainer, (const LYNXCHAR*, const LYNXCHAR*, int, int, int, float))
			REGISTER_SCRIPT_METHOD(lps, "DestroyContainer", LynxEngine::CEngine, void, DestroyContainer, (void*))

			REGISTER_SCRIPT_METHOD(lps, "CreatePhysicsWorld", LynxEngine::CEngine, void*, CreatePhysicsWorld, (void))

			REGISTER_SCRIPT_METHOD(lps, "OpenScene", LynxEngine::CEngine, void, OpenScene, (const LYNXCHAR*))
			REGISTER_SCRIPT_METHOD(lps, "Render", LynxEngine::CEngine, void, Render, (void))

			REGISTER_SCRIPT_METHOD(lps, "ShowMode", LynxEngine::CEngine, void, ShowMode, (int))

			return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	GameFramework::CUIPage* CEngine::CreateUIPage(const LYNXCHAR* name, const LYNXCHAR* classname)
	{
		GameFramework::CUIPage* lpPage = m_lpKernel->CreateUIPage(classname);
		if (lpPage)
		{
			lpPage->SetName(name);
			lpPage->vCreate();
			m_lpKernel->AddUIPage(lpPage);
		}
		return lpPage;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	GameFramework::CUIPage* CEngine::CreateUIPage(const LYNXCHAR* name, const LYNXCHAR* scriptfile, const LYNXCHAR* scriptname)
	{
		GameFramework::CUIPage* lpPage = LYNXNEW GameFramework::CSimpleUIPage(m_lpKernel);
		lpPage->SetName(name);
		lpPage->OverloadClassScript(scriptfile, scriptname);
		lpPage->vCreate();
		m_lpKernel->AddUIPage(lpPage);

		return lpPage;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::DeleteLastUIPage()
	{
		if (m_lpKernel->GetLastUIPage())
			LYNXDEL m_lpKernel->GetLastUIPage();

		m_lpKernel->RemoveUIPage(m_lpKernel->GetLastUIPage());
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::SetCurrentUIPage(GameFramework::CUIPage* page)
	{
		m_lpKernel->SetCurrentUIPage(page);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	GameFramework::CUIPage* CEngine::FindUIPage(const LYNXCHAR* name)
	{
		return m_lpKernel->FindUIPage(CString(name));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::SetUITransitionMode(UITRASITIONMODE mode)
	{
		m_lpKernel->SetUITransitionMode(mode);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::SetFadeOutSpeed(float speed)
	{
		m_lpKernel->SetFadeOutSpeed(speed);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CEngine::SetSwipeSpeed(float speed)
	{
		m_lpKernel->SetSwipeSpeed(speed);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CEngine::OpenPackage(const LYNXCHAR* name)
	{
		return m_lpFileSystem->OpenPackage(name);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	void CEngine::ClosePackage()
	{
		m_lpFileSystem->ClosePackage();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	void CEngine::vReleaseVideo(const CVideo* video)
	{
		LYNXDEL video;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::GetJSONValues(Json::Value& root, const CAnsiString& doc)
	{
		Json::Reader Reader;

		return (Reader.parse(doc, root));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::GetJSONValuesFromFile(Json::Value& root, const CString& filename)
	{
		CPlatformFileStream File;
		CString Line;
		CString JsonString;

		if (File.vOpen(filename, CStream::Read | CStream::Text | CStream::UTF8))
		{
			while (File.vRead(Line))
			{
				JsonString += Line;
			}
			File.vClose();
		}
		return CEngine::GetJSONValues(root, JsonString);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::MakeJSON(CAnsiString& doc, const Json::Value& root)
	{
		Json::StyledWriter Writer;

		// Make a new JSON document for the configuration. Preserve original comments.
		doc = Writer.write(root);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::LoadJSON(Json::Value& root, const CString& filename)
	{
		CPlatformFileStream File;
		CString Line;
		CString JsonString;

		JsonString.clear();

		if (File.vOpen(filename, CStream::Read | CStream::Text | CStream::UTF8))
		{
			while (File.vRead(Line))
			{
				JsonString += Line;
			}
			File.vClose();
		}

		GetJSONValues(root, JsonString);
		JsonString.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::SaveJSON(const Json::Value& root, const CString& filename)
	{
		CPlatformFileStream File;
		CString Line;
		CString JsonString;

		JsonString.clear();
		MakeJSON(JsonString, root);

		if (File.vOpen(filename, CStream::Write | CStream::Text | CStream::UTF8))
		{
			File.vWrite(JsonString);
			File.vClose();
		}

		JsonString.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::EncryptFile2Zip(const CAnsiString& file_name, const CAnsiString& password)
	{
		CAnsiString TempFileName = file_name + CAnsiString(_T(".tmp"));

		zipFile Zip = CZipStream::CreateZipFile(TempFileName);

		CAnsiString Name;
		if (file_name.c_str()[0] == _T('.') && file_name.c_str()[1] == _T('.') && (file_name.c_str()[2] == _T('/') || file_name.c_str()[2] == _T('\\')))
			Name = &(file_name.c_str()[3]);
		else
			Name = file_name;
		CZipStream::AddFile2ZipFile(Zip, CString(file_name), Name, password);
		CZipStream::CloseZipFile(Zip);
		lynxDeleteFile(CString(file_name));
		lynxRenameFile(CString(file_name), CString(TempFileName));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CEngine::DecryptFileFromZip(CPlatformFileStream& out, const CAnsiString& file_name, const CAnsiString& password, int flag)
	{
		CZipStream Zip;

		Zip.SetPassword(password);
		if (!Zip.OpenZipFile(file_name, LYNX_FALSE))
			return LYNX_FALSE;

		CString TempPath = glpKernel->GetlpEngine()->GetlpFileSystem()->GetFullSearchingPath(0, CString(_T("../temp/output.dec")));
		lynxDeleteFile(TempPath.c_str());
		Zip.vOpen(file_name, CStream::Binary | CStream::Read);
		out.vOpen(CString(_T("../temp/output.dec")), CStream::Binary | CStream::Write);
		Zip.CopyToFile(out);
		out.vClose();
		Zip.vClose();
		Zip.CloseZipFile();

		return out.vOpen(CString(_T("../temp/output.dec")), flag);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GetScreenPosition(CVector2& spos, const CVector3& pos, const CCameraContainer& cam, const LYNXRECT& viewport)
	{
		Math::CVector4 Pos4, ScreenPos;
		Math::CMatrix4 ViewM, ProjM;
		Math::CMatrix4 WVPM;
		float HalfWidth, HalfHeight;

		ViewM = cam.GetViewMatrix();
		ProjM = cam.GetProjectionMatrix();
		lynxMatrixXMatrix(WVPM, ViewM, ProjM);

		HalfWidth = float(viewport.x2 - viewport.x1) * 0.5f;
		HalfHeight = float(viewport.y2 - viewport.y1) * 0.5f;

		Pos4 = CVector4(pos, 1);
		ScreenPos = Pos4 * WVPM;
		ScreenPos.x /= ScreenPos.w;
		ScreenPos.y /= ScreenPos.w;

		spos.x = ScreenPos.x * HalfWidth + HalfWidth;
		spos.y = HalfHeight - ScreenPos.y * HalfHeight;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GenerateUDID()
	{
		LYNXGUID GUID;

		lynxOSMakeGUID(&GUID);
		return CString(lynxOSGUIDToString(&GUID));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GetLocalDateString(const LYNXLONGLONG& t)
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		Time = localtime((const time_t*)(&t));

		lynxSprintf(Buffer, _T("%d/%02d/%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday);

		return CString(Buffer);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	const CEngine::DATEDATA& CEngine::GetLocalDateWithData(const LYNXLONGLONG& t)
	{
		static DATEDATA DateData;

		struct tm* Time;
		Time = localtime((const time_t*)(&t));
		DateData.Year = Time->tm_year + 1900;
		DateData.Month = Time->tm_mon + 1;
		DateData.Day = Time->tm_mday;
		DateData.Hour = Time->tm_hour;
		DateData.Minute = Time->tm_min;
		DateData.Second = Time->tm_sec;

		return DateData;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	CString	CEngine::GetLocalDateStringWithFormat(const LYNXLONGLONG& t, const CString format)
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		Time = localtime((const time_t*)(&t));

		if (Time)
		{
			if (format == "YYYY")
			{
				lynxSprintf(Buffer, _T("%d"), Time->tm_year + 1900);
			}
			else if (format == "YYYY-MM")
			{
				lynxSprintf(Buffer, _T("%d%02d"), Time->tm_year + 1900, Time->tm_mon + 1);
			}
			else if (format == "YYYY-MM-DD")
			{
				lynxSprintf(Buffer, _T("%d%02d%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday);
			}
			else if (format == "MM-DD-HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d/%02d %02d:%02d"), Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min);
			}
			else if (format == "HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d:%02d"), Time->tm_hour, Time->tm_min);
			}
		}
		else
		{
			if (format == "YYYY")
			{
				lynxSprintf(Buffer, _T("%d"), 1900);
			}
			else if (format == "YYYY-MM")
			{
				lynxSprintf(Buffer, _T("%d%02d"), 1900, 1);
			}
			else if (format == "YYYY-MM-DD")
			{
				lynxSprintf(Buffer, _T("%d%02d%02d"), 1900, 1, 1);
			}
			else if (format == "MM-DD-HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d/%02d %02d:%02d"), 1, 1, 0, 0);
			}
			else if (format == "HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d:%02d"), Time->tm_hour, Time->tm_min);
			}
		}

		return CString(Buffer);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GetGMTDateStringWithFormat(const LYNXLONGLONG& t, const CString format)
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		Time = gmtime((const time_t*)(&t));

		if (Time)
		{
			if (format == "YYYY")
			{
				lynxSprintf(Buffer, _T("%d"), Time->tm_year + 1900);
			}
			else if (format == "YYYY-MM")
			{
				lynxSprintf(Buffer, _T("%d%02d"), Time->tm_year + 1900, Time->tm_mon + 1);
			}
			else if (format == "YYYY-MM-DD")
			{
				lynxSprintf(Buffer, _T("%d%02d%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday);
			}
			else if (format == "MM-DD-HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d/%02d %02d:%02d"), Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min);
			}
			else if (format == "HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d:%02d"), Time->tm_hour, Time->tm_min);
			}
		}
		else
		{
			if (format == "YYYY")
			{
				lynxSprintf(Buffer, _T("%d"), 1900);
			}
			else if (format == "YYYY-MM")
			{
				lynxSprintf(Buffer, _T("%d%02d"), 1900, 1);
			}
			else if (format == "YYYY-MM-DD")
			{
				lynxSprintf(Buffer, _T("%d%02d%02d"), 1900, 1, 1);
			}
			else if (format == "MM-DD-HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d/%02d %02d:%02d"), 1, 1, 0, 0);
			}
			else if (format == "HH-MM")
			{
				lynxSprintf(Buffer, _T("%02d:%02d"), Time->tm_hour, Time->tm_min);
			}
		}

		return CString(Buffer);	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GetLocalTimeStringWithoutSecond(const LYNXLONGLONG& t)
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		Time = localtime((const time_t*)(&t));

		lynxSprintf(Buffer, _T("%d/%02d/%02d %02d:%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min);

		return CString(Buffer);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GetFullLocalTimeString(const LYNXLONGLONG& t)
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		Time = localtime((const time_t*)(&t));

		lynxSprintf(Buffer, _T("%d/%02d/%02d %02d:%02d:%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min, Time->tm_sec);

		return CString(Buffer);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CString CEngine::GetCurrentLocalTimeStringForFilename()
	{
		struct tm* Time;
		LYNXCHAR Buffer[256];

		time_t t;

		time(&t);
		Time = localtime((const time_t*)(&t));

		lynxSprintf(Buffer, _T("%d_%02d_%02d_%02d_%02d_%02d"), Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min, Time->tm_sec);

		return CString(Buffer);
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CString CEngine::ProfanityFilterString(const CString& inputString, const CArray<CString>& ProfanityStringTable, const LYNXCHAR replaceChar)
	{
		LYNXCHAR Buf[256];
		static CString ReplacementString;
		CString RetValue(inputString);
		typedef CArray<CString>::const_iterator STRINGARRAY_CITER;
		for (STRINGARRAY_CITER CIter = ProfanityStringTable.begin(); CIter != ProfanityStringTable.end(); ++CIter)
		{
			const LYNXCHAR* Word = CIter->c_str();
			ReplacementString.assign(lynxUTF8StrLen(Word), replaceChar);
            
#if defined(__WIN32__)
			// since GCC 4.8 hasn't implement std::regex_replace.
			using namespace std::regex_constants;
			lynxSprintf(Buf, _T("(%s)+"), Word);
			std::regex ProfanityFilterRegex(Buf, ECMAScript | icase);
			RetValue = std::regex_replace(RetValue, ProfanityFilterRegex, ReplacementString);
#else
            static std::function<void(CString&, const CString&, const CString&)> regex_replace =
            [=](CString& src, const CString& from, const CString& to)->void
            {
                size_t StartPos = 0;
                while ((StartPos = src.find(from, StartPos)) != CString::npos)
                {
                    src = src.replace(StartPos, from.length(), to);
                    StartPos += to.length();
                }
            };
            
            regex_replace(RetValue, Word, ReplacementString);
#endif
		}

		// return std::move(RetValue);
		return RetValue;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------	
	CString CEngine::ReplaceFormatString(const LYNXCHAR* inputCString, ...)
	{
		CString formatString(inputCString);
		const LYNXCHAR* Token = inputCString;
		
		va_list va;
		va_start(va, inputCString);
		
		do {
			Token = va_arg(va, char*);
			if (!Token)
				break;

			LYNXCHAR* Value = va_arg(va, char*);
			if (!Value)
				break;
		
			size_t StartPos = formatString.find(Token);
			if (StartPos != CString::npos) {
				formatString = formatString.replace(StartPos, lynxStrLen(Token), Value);
			}			
		} while (Token);

		va_end(va);	

		return formatString;
	}
	//-------------------------------------------------------------------------------------------------------
	/*
	*	@brief C
	*
	*	@param C
	*	@param C
	*	@return C
	*/
	//-------------------------------------------------------------------------------------------------------
	void CEngine::ComputeNewStep(float* new_step, int* count, const float step, const float min, const int max_count)
	{
		(*new_step) = step;
		(*count) = 1;
		if (step > min)
		{
			(*count) = LYNX_FLOOR(step / min);
			if ((*count) > max_count)
				(*count) = max_count;
			(*new_step) /= float((*count));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCRC CEngine::ComputeFileCRC(const CString& path)
	{
		LYNXBYTE Buffer[64000];
		int Len;
		LYNXCRC Remainder;
		CPlatformFileStream File;

		if (File.vOpen(path))
		{
			Len = File.vRead(Buffer, sizeof(LYNXBYTE), 64000);
			Remainder = INITIAL_REMAINDER;
			while (Len)
			{
				Remainder = lynxComputeCRCSlowWithRemainder(Remainder, Buffer, Len);
				Len = File.vRead(Buffer, sizeof(LYNXBYTE), 64000);
			}
			File.vClose();
		}
		return Remainder;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CEngine::GeneratePVRTextures(const CString& dest, const CString& src)
	{
		#ifdef __DESKTOP__
			//using namespace pvrtexture;

			// Open and reads a pvr texture from the file location specified by filePath 
			//CPVRTexture cTexture(src.c_str());

			// Convert the image to a Normal Map with a scale of 5.0, and y/z/x channel order 
			//GenerateNormalMap(cTexture, 5.0, "yzx"); 

			// Generate MIP-map chain 
			//if (!GenerateMIPMaps(cTexture, eResizeLinear))
				//return;

			// Compress to PVRTC 4bpp. 
			//if (!Transcode(cTexture, ePVRTPF_PVRTCI_4bpp_RGBA, ePVRTVarTypeUnsignedByteNorm, ePVRTCSpacelRGB))
				//return;

			// Save the file 
			//cTexture.saveFile("out.pvr"); 		
			//cTexture.saveFileLegacyPVR("e:/out.pvr", eOGLES2);


			//"PVRTexToolCLI -m –f a1r5g5b5 –i Example.bmp"

			
			ShellExecute(NULL, "open", "cmd", " /K PVRTexToolCLI.exe -o test.pvr -i map_0012_01_02.png -m -mfilter cubic -f PVRTC1_4_RGB -legacypvr", "./", SW_HIDE);			

			//system("PVRTexToolCLI.exe -o test.pvr -m -f PVRTC1_4_RGB -legacypvr -i map_0012_01_02.png");
		#endif
	}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowProfileData()
	//{	
	//	#ifdef __PROFILE__
	//		LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//		LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//		LYNXRECT Rect;

	//		Rect.x1 = 0;
	//		Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//		Rect.y1 = 0;
	//		Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//		DWORD DrawCall = m_lpGraphicsSystem->GetlpRenderer()->GetDrawCallCounter();
	//		DWORD DrawTime = m_lpGraphicsSystem->GetlpRenderer()->GetDrawTimeCounter();
	//		DWORD Primitives = m_lpGraphicsSystem->GetlpRenderer()->GetPrimitiveCounter();
	//		DWORD SetRS = m_lpGraphicsSystem->GetlpRenderer()->GetSetRSCounter();
	//		DWORD SetTexture = m_lpGraphicsSystem->GetlpRenderer()->GetSetTextureCounter();
	//		DWORD SetShader = m_lpGraphicsSystem->GetlpRenderer()->GetSetShaderCounter();
	//		DWORD SetShaderConst = m_lpGraphicsSystem->GetlpRenderer()->GetSetShaderConstantCounter();
	//		lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10, 10, &TitleColor, &ShadowColor, "Render Profile : %s --------------------------------------------------------------------", ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer()->GetDeviceName());
	//		lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25, &Color, &ShadowColor, "FPS : %d, Step : %2.2f", (1000/(int)((CEngine*)m_lpEngine)->GetlpKernel()->GetFrameTime()), ((CEngine*)m_lpEngine)->m_Step);				
	//		lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 40, &Color, &ShadowColor, "Draw Call : %d(%d), Primitives : %d", DrawCall, DrawTime, Primitives);	
	//		lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 55, &Color, &ShadowColor, "Set RS : %d, Texture : %d, Shader : %d, Shader Constant : %d", SetRS, SetTexture, SetShader, SetShaderConst);	
	//	#endif
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowMaterialData()
	//{	
	//	LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//	LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//	LYNXRECT Rect;

	//	Rect.x1 = 0;
	//	Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//	Rect.y1 = 0;
	//	Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//	CList<MaterialSystem::CMaterialPtr>::CIterator Mat;
	//	CList<MaterialSystem::CMaterialPtr>& MatList = m_lpMaterialSystem->GetMaterialList();

	//	lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10, 10, &TitleColor, &ShadowColor, "Material Data(%d)-----------------------------------------------------------------------", MatList.size());				
	//	int i;
	//	for (Mat = MatList.begin(), i=0; Mat != MatList.end(); ++Mat, ++i)
	//	{				
	//		lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25+i*15, &Color, &ShadowColor, "%s, Count: %d", (*Mat)->GetName().c_str(), (*Mat).GetCount());				
	//	}
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowBufferData()
	//{	
	//	LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//	LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//	LYNXRECT Rect;

	//	Rect.x1 = 0;
	//	Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//	Rect.y1 = 0;
	//	Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//	CList<CResourcePtr>::CIterator Res;
	//	CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

	//	int Num = 0;
	//	for (Res = ResList.begin(); Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == BUFFER)
	//			Num++;
	//	}

	//	lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10, 10, &TitleColor, &ShadowColor, "Buffer Data(%d)-----------------------------------------------------------------------", Num);				
	//	int i;
	//	for (Res = ResList.begin(), i=0; Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == BUFFER)
	//			lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25+(i++)*15, &Color, &ShadowColor, "%s, Count: %d", (*Res)->GetName().c_str(), (*Res).GetCount());				
	//	}
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowResourceData()
	//{	
	//	LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//	LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//	LYNXRECT Rect;

	//	Rect.x1 = 0;
	//	Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//	Rect.y1 = 0;
	//	Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//	CList<CResourcePtr>::CIterator Res;
	//	CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

	//	int Num = 0;
	//	for (Res = ResList.begin(); Res != ResList.end(); ++Res)
	//	{				
	//		Num++;
	//	}

	//	LYNXCHAR Buffer[256];
	//	m_lpGraphicsSystem->Printf(&Rect, 10, 10, &TitleColor, &ShadowColor, "Resource Data(%d)-----------------------------------------------------------------------", Num);				
	//	int i;
	//	for (Res = ResList.begin(), i=0; Res != ResList.end(); ++Res)
	//	{				
	//		m_lpGraphicsSystem->Printf(&Rect, 10+4*5, 25+(i++)*15, &Color, &ShadowColor, "%s, Type: %s, Count: %d", (*Res)->GetName().c_str(), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str(), (*Res).GetCount());				
	//		sprintf(Buffer, "%s, Type: %s, Count: %d", (*Res)->GetName().c_str(), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str(), (*Res).GetCount());
	//		m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10+4*5, CString(Buffer), &Color, &ShadowColor);
	//	}
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowTextureData()
	//{	
	//	LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//	LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//	LYNXRECT Rect;

	//	Rect.x1 = 0;
	//	Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//	Rect.y1 = 0;
	//	Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//	CList<CResourcePtr>::CIterator Res;
	//	CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

	//	int Num = 0;
	//	for (Res = ResList.begin(); Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == TEXTURE)
	//			Num++;
	//	}

	//	lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10, 10, &TitleColor, &ShadowColor, "Texture Data(%d)-----------------------------------------------------------------------", Num);				
	//	int i;
	//	for (Res = ResList.begin(), i=0; Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == TEXTURE)
	//		{
	//			GraphicsSystem::CTexture* lpTex = (GraphicsSystem::CTexture*)(&(*(*Res)));
	//			if (lpTex->vIsValid())
	//				lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25+(i++)*15, &Color, &ShadowColor, "%s<%dX%d>, Count: %d", (*Res)->GetName().c_str(), (lpTex)->vGetWidth(), (lpTex)->vGetHeight(), (*Res).GetCount());				
	//			else
	//				lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25+(i++)*15, &Color, &ShadowColor, "%s<%dX%d>, Count: %d", (*Res)->GetName().c_str(), 1, 1, (*Res).GetCount());				
	//		}
	//	}
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowShaderData()
	//{	
	//	LYNXCOLORRGBA TitleColor = {255, 255, 0, 255};
	//	LYNXCOLORRGBA Color = {255, 255, 255, 255}, ShadowColor = {0, 0, 0, 255};
	//	LYNXRECT Rect;

	//	Rect.x1 = 0;
	//	Rect.x2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetWidth();				
	//	Rect.y1 = 0;
	//	Rect.y2 = m_lpGraphicsSystem->GetSystemBackBuffer()->vGetHeight();

	//	CList<CResourcePtr>::CIterator Res;
	//	CList<CResourcePtr>& ResList = GetResourceManager().GetObjectList();

	//	int Num = 0;
	//	for (Res = ResList.begin(); Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == VERTEX_SHADER || (*Res)->GetResourceType() == PIXEL_SHADER)
	//			Num++;
	//	}

	//	lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10, 10, &TitleColor, &ShadowColor, "Shader Data(%d)-----------------------------------------------------------------------", Num);				
	//	int i;
	//	for (Res = ResList.begin(), i=0; Res != ResList.end(); ++Res)
	//	{				
	//		if ((*Res)->GetResourceType() == VERTEX_SHADER || (*Res)->GetResourceType() == PIXEL_SHADER)
	//			lynxPrintfWithShadow(&lynxGetCurrentRenderer()->SystemData.BackBuffer, &Rect, 10+4*5, 25+(i++)*15, &Color, &ShadowColor, "%s, Count: %d", (*Res)->GetName().c_str(), (*Res).GetCount());				
	//	}
	//}
	////-------------------------------------------------------------------------------------------------------
	////
	////  說明:   
	////-------------------------------------------------------------------------------------------------------
	//void CScene::ShowMaps(GraphicsSystem::CGraphicsSystem::MAPTYPE map)
	//{
	//	m_lpGraphicsSystem->BltTextureToBackBuffer(m_lpGraphicsSystem->GetlpMap(map)->GetCurrent());
	//}
}
