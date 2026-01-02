#include <LynxEngineClasses.h>
#include <LynxMathWrap.h>
#include <LynxString.h>
#include <LynxList.h>
#include <LynxArray.h>
#include <LynxStack.h>
#include <LynxTMap.h>
#include <LynxBitSet.h>
#include <LynxSet.h>
#include <LynxWeakPtr.h>
#include <LynxCountedPtr.h>
//#include <LynxBufferingObj.h>
#include <LynxAlgorithm.h>
#include <json/json.h>
#include <tinyxml2.h>
#include <regex>

#if defined(__WXWIDGET__)
	// For compilers that support precompilation, includes "wx/wx.h".
	#include "wx/wx.h"
	#include "wx/wxprec.h"
	#include "wx/spinbutt.h"
	#include "wx/tglbtn.h"
	#include "wx/bookctrl.h"
	#include "wx/imaglist.h"
	#include "wx/artprov.h"
	#include "wx/cshelp.h"
	#include "wx/listctrl.h"
	#include "wx/html/htmlwin.h"
	#include "wx/html/m_templ.h"	
#endif

#ifdef __LUA__		
	#ifdef __ANDROID__
		#ifdef _L
			#undef _L
		#endif
	#endif
	#include <LynxLuaBuiltinTypes.h>		
	#include <LynxLuaScript.h>		
	#include <LynxLuaVMachine.h>
#endif

#include <LynxDebugMemAllocator.h>
#include <LynxReleaseMemAllocator.h>
#include <LynxFixedAllocator.h>
#include <LynxSmallObjAllocator.h>
//#include <LynxDebugSmallObjAllocator.h>
#include <LynxObj.h>
#include <LynxGameObj.h>
#include <LynxRenderableObj.h>
#include <LynxResource.h>
#include <LynxLogger.h>
#include <LynxCreationDesc.h>
#include <LynxAlgorithm.h>
#include <LynxFileStream.h>
#include <LynxMemoryStream.h>
#include <LynxZipStream.h>
#include <LynxURLStream.h>

#include <LynxOutputDevice.h>
#include <LynxFileOutputDevice.h>
#include <LynxScreenOutputDevice.h>

#include <Font/LynxFont.h>
#include <Font/LynxTextureFont.h>
#include <Font/LynxTrueTypeFont.h>
#include <Font/LynxFontLabel.h>
#include <Font/LynxStaticText.h>
#include <Font/LynxTextureFontString.h>
#include <Font/LynxTrueTypeFontString.h>
#include <Font/LynxSpriteTextString.h>

#include <GameFramework/LynxGameFrameworkKernel.h>
#include <GameFramework/LynxUIPage.h>
#include <GameFramework/LynxSimpleUIPage.h>

#include <GameConsoleSystem/LynxGameConsoleSystem.h>

// Animation headers ----------------------------
#include <Animation/LynxAnimation.h>
#include <Animation/LynxVoxelAnimation.h>
#include <Animation/LynxAnimationKeys.h>
#include <Animation/LynxAnimationTree.h>

// Container headers ----------------------------
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
#include <Container/LynxSpriteClusterContainer.h>
#include <Container/LynxSprite3DContainer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxVoxelClusterContainer.h>
#include <Container/LynxClusterSubContainer.h>
#include <Container/LynxSkyDomeContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxSkyLightMap.h>
#include <Container/LynxVolumetricCloudContainer.h>

#include <LynxEngine.h>
#include <LynxGame.h>

// Graphics system headers ----------------------------
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxSamplerState.h>
#include <GraphicsSystem/LynxMap.h>
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxGlowMap.h>
#include <GraphicsSystem/LynxToneMap.h>
#include <GraphicsSystem/LynxLuminanceMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>
#include <GraphicsSystem/LynxVelocityMap.h>
#include <GraphicsSystem/LynxGBufferMap.h>
#include <GraphicsSystem/LynxToneMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>
#include <GraphicsSystem/LynxProjectedMap.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <GraphicsSystem/LynxVertexShader.h>
#include <GraphicsSystem/LynxHullShader.h>
#include <GraphicsSystem/LynxDomainShader.h>
#include <GraphicsSystem/LynxGeometryShader.h>
#include <GraphicsSystem/LynxPixelShader.h>
#include <GraphicsSystem/LynxComputeShader.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <GraphicsSystem/LynxShaderClass.h>
#include <GraphicsSystem/LynxShaderConstant.h>
#include <GraphicsSystem/LynxShaderResource.h>
#include <GraphicsSystem/LynxShaderGroup.h>
#include <GraphicsSystem/LynxShaderConstantGroup.h>
#include <GraphicsSystem/LynxPostProcessing.h>
#include <GraphicsSystem/LynxScreenQuad.h>

#include <GraphicsSystem/NullRenderer/LynxNullBuffer.h>
#include <GraphicsSystem/NullRenderer/LynxNullTexture.h>
#include <GraphicsSystem/NullRenderer/LynxNullQuery.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexLayout.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexShader.h>
#include <GraphicsSystem/NullRenderer/LynxNullPixelShader.h>
#include <GraphicsSystem/NullRenderer/LynxNullVertexArray.h>
#include <GraphicsSystem/NullRenderer/LynxNullIndexArray.h>
#include <GraphicsSystem/NullRenderer/LynxNullDepthStencilState.h>
#include <GraphicsSystem/NullRenderer/LynxNullBlendState.h>
#include <GraphicsSystem/NullRenderer/LynxNullRasterizerState.h>
#include <GraphicsSystem/NullRenderer/LynxNullSamplerState.h>

// GUI system headers ----------------------------
#include <GUISystem/LynxGUISystem.h>
#include <GUISystem/LynxScrollGroup.h>
#include <GUISystem/LynxControl.h>
#include <GUISystem/LynxDummyControl.h>
#include <GUISystem/LynxImage.h>
#include <GUISystem/LynxText.h>
#include <GUISystem/LynxButton.h>
#include <GUISystem/LynxStateButton.h>
#include <GUISystem/LynxSlider.h>
#include <GUISystem/LynxTab.h>
#include <GUISystem/LynxWindow.h>
#include <GUISystem/LynxMessageBox.h>
#include <GUISystem/LynxDialog.h>
#include <GUISystem/LynxGUIContext.h>
#include <GUISystem/LynxHitMask.h>
#include <GUISystem/LynxPEmitter.h>

// Collision system headers ----------------------------
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>
#include <CollisionSystem/Bullet/LynxBtOBB.h>
#include <CollisionSystem/Bullet/LynxBtCapsule.h>
#include <CollisionSystem/Bullet/LynxBtSphere.h>
#include <CollisionSystem/Bullet/LynxBtPlane.h>

// Perf system headers ----------------------------
#include <PerfSystem/LynxPerfSystem.h>
#include <PerfSystem/LynxPerfCounter.h>
#include <PerfSystem/LynxPerfTimer.h>

// AD system headers ----------------------------
#include <AdSystem/LynxAdSystem.h>
#include <AdSystem/LynxAd.h>

// Scene system headers ----------------------------
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxAtmosphere.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <SceneSystem/StaticObj/LynxStaticObj.h>
#include <SceneSystem/StaticObj/LynxStaticMesh.h>
#include <SceneSystem/StaticObj/LynxStaticModel.h>
#include <SceneSystem/DynObj/LynxDynObj.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxScnRenderer.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>

// Script system headers ----------------------------
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>	
	#include <LynxLuaBuiltinTypes.h>
#endif

// Material system headers ----------------------------
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxTextureAnimation.h>

// State machine headers ----------------------------
#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateTranslation.h>
#include <StateMachine/LynxRandTranslation.h>

// Physics system headers ----------------------------
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <PhysicsSystem/LynxPhysicsSpace.h>
#include <PhysicsSystem/LynxPhysicsObj.h>
#include <PhysicsSystem/LynxPhysicsShape.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>

// Sound system headers ----------------------------
#include <SoundSystem/LynxSoundSystem.h>
#include <SoundSystem/LynxMusic.h>
#include <SoundSystem/LynxSound.h>

// Collision system headers ----------------------------
#include <CollisionSystem/LynxCollisionSystem.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>

// File system headers ----------------------------
#include <FileSystem/LynxFileSystem.h>
#include <FileSystem/LynxPackage.h>
#include <FileSystem/LynxUpdate.h>

// Input system headers ----------------------------
#include <InputSystem/LynxInputSystem.h>
#include <InputSystem/LynxInputDevice.h>
#include <InputSystem/LynxTouch.h>
#include <InputSystem/LynxAccelerometer.h>
#include <InputSystem/LynxGyrometer.h>
#include <InputSystem/LynxTeslameter.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <InputSystem/LynxGamePad.h>
#include <InputSystem/LynxInputRecord.h>

// Network system headers ----------------------------
#include <NetworkSystem/LynxNetworkSystem.h>
#include <NetworkSystem/LynxHttpRequest.h>
#include <NetworkSystem/LynxHttpFormDataRequest.h>
#include <NetworkSystem/LynxNetPlayer.h>
#include <NetworkSystem/LynxNetClient.h>
#ifdef __CURL__
	#include <NetworkSystem/LynxcurlHttpRequest.h>
	#include <NetworkSystem/LynxcurlHttpFormDataRequest.h>
#endif
#ifdef __PHOTON__
	#include <NetworkSystem/LynxPhotonNetClient.h>
	#include <NetworkSystem/LynxPhotonNetListener.h>
#endif

#include <flatbuffers/flatbuffers.h>	

#include <GameAnalyticsSystem/LynxGameAnalytics.h>
#include <GameAnalyticsSystem/LynxFirebaseAnalytics.h>
#include <GameAnalyticsSystem/LynxFacebookAnalytics.h>
#include <GameAnalyticsSystem/LynxAppsFlyerAnalytics.h>

#include <LynxIniReader.h>
#include <LynxUserProfile.h>
#include <LynxReadOnlyProfile.h>
#include <LynxPlayerProfile.h>
#include <LynxGameProfile.h>
#include <LynxStoreKit.h>

#ifdef __WIN32__
	#include <Platform/Win32/LynxWin32GameFrameworkKernel.h>
    #include <Platform/Win32/LynxWin32TrueTypeFont.h>
    #include <Platform/Win32/LynxWin32FontLabel.h>
	#include <Platform/Win32/LynxWin32URLStream.h>
	#include <Platform/Win32/LynxWin32BitmapContext.h>
	#include <Platform/Win32/LynxWin32ConsoleOutputDevice.h>
	#include <Platform/Win32/LynxWin32WebView.h>
	//#include <vld.h>
#endif

#ifdef __ANDROID__
    #include <Platform/Android/LynxAndroidFontLabel.h>
	#include <Platform/Android/LynxAndroidBitmapContext.h>
	#include <Platform/Android/LynxAndroidTrueTypeFont.h>
	#include <Platform/Android/LynxAndroidFileStream.h>
	#include <Platform/Android/LynxAndroidURLStream.h>
	#include <Platform/Android/LynxAndroidCrashdump.h>
	#include <Platform/Android/LynxAndroidStackUnwind.h>
#endif

#ifdef __iOS__
    #include <Platform/iOS/LynxiOSFileSystem.h>
    #include <Platform/iOS/LynxiOSFileStream.h>
    #include <Platform/iOS/LynxiOSZipStream.h>
    #include <Platform/iOS/LynxiOSEngine.h>
    #include <Platform/iOS/LynxiOSConsoleOutputDevice.h>
    #include <Platform/iOS/LynxiOSTrueTypeFont.h>
    #include <Platform/iOS/LynxiOSFontLabel.h>
    #include <Platform/iOS/LynxiOSHttpRequest.h>
    #include <Platform/iOS/LynxiOSHttpFormDataRequest.h>
    #include <Platform/iOS/LynxiOSURLStream.h>
	#include <Platform/iOS/LynxiOSBitmapContext.h>
    #ifdef __OBJC__
        #include <Platform/iOS/LynxView.h>
        #include <Platform/iOS/LynxViewController.h>
        #include <Platform/iOS/LynxiOSTouch.h>
        #include <Platform/iOS/LynxiOSAccelerometer.h>
        #include <Platform/iOS/LynxiOSGyrometer.h>
        #include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
        #include <Platform/iOS/LynxiOSConsoleLogger.h>
        #include <Platform/iOS/LynxiOSGraphicsSystem.h>
        #include <Platform/iOS/NSData+Base64.h>
        #include <Platform/iOS/LynxiOSStoreKit.h>
        #include <Platform/iOS/LynxiOSGameAnalytics.h>
		#include <Platform/iOS/LynxiOSFirebaseAnalytics.h>
		#include <Platform/iOS/LynxiOSFacebookAnalytics.h>
        #include <Platform/iOS/LynxiOSAppsFlyerAnalytics.h>
        #include <Platform/iOS/LynxiOSFacebookShareDialogDelegate.h>
        #include <Platform/iOS/LynxiOSFeedbackDialog.h>
        #include <Platform/iOS/LynxiOSCrashdump.h>
    #endif
#endif

#ifdef __ANDROID__
    #include <Platform/Android/LynxAndroidLogCatOutputDevice.h>
    #include <Platform/Android/LynxAndroidFileStream.h>
	#include <Platform/Android/LynxAndroidFileSystem.h>
    #include <Platform/Android/LynxAndroidGameFrameworkKernel.h>
    #include <Platform/Android/LynxAndroidEngine.h>
    #include <Platform/Android/LynxAndroidGraphicsSystem.h>
	#include <Platform/Android/LynxAndroidStoreKit.h>
	#include <Platform/Android/LynxAndroidGameAnalytics.h>
	#include <Platform/Android/LynxAndroidFirebaseAnalytics.h>
	#include <Platform/Android/LynxAndroidFacebookAnalytics.h>
	#include <Platform/Android/LynxAndroidAppsFlyerAnalytics.h>
#endif

LYNX_FORWARD_DECLARATION(NSString)

#if defined __iOS__
    extern "C" void     lynxcString2NSString(NSString** out_str, const LYNXCHAR* str);
    extern "C" void     lynxcString2NSStringUTF16(NSString** out_str, const LYNXCHAR* str);
    extern "C" void     lynxNSString2cString(LYNXCHAR* out_str, const NSString* str);
    extern "C" void     lynxNSString2cStringUTF16(LYNXCHAR* out_str, const NSString* str);
    extern "C" void     lynxcString2NSStringUTF16BigEndian(NSString** out_str, const LYNXCHAR* str);
    extern "C" void     lynxcString2NSStringUTF8(NSString** out_str, const LYNXCHAR* str);
    extern "C" void     lynxcStringUTF162UTF32(LYNXCHAR* out_str, const LYNXCHAR* str);
    extern "C" void     lynxcStringUTF322UTF16(LYNXCHAR* out_str, const LYNXCHAR* str);
    extern "C" int      lynxiOSAtoi(const LYNXCHAR* str);
    extern "C" float    lynxiOSAtof(const LYNXCHAR* str);
    extern "C" void     lynxiOSLog(const LYNXCHAR *str);
#endif
