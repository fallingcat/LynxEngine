#include <LynxEngine_PCH.h>
#import <GameKit/GameKit.h>

#include <VoxelGameEngine.h>
#include <VoxelGame.h>
#import <iOS/VoxelGameDelegate.h>
#include <iOS/VoxelGameKernel.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>	
#include <LynxZipStream.h>	
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxEngine.h>
#include <LynxResource.h>
#include <LynxDebugMemAllocator.h>
#include <LynxReleaseMemAllocator.h>
#include <LynxSmallObjAllocator.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxBuffer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <LynxScreenOutputDevice.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Shader/LynxGaussinBlurPS.h>
#import <JSON.h>
#import <ASIHTTPRequest.h>
#import <ASIFormDataRequest.h>
#import <Reachability.h>
#import <UIKit/UIApplication.h>
#import <TestFlight.h>
#import <Flurry.h>
//#import <MobileAppTracker/MobileAppTracker.h>
#import <Foundation/NSString.h>
#import <Foundation/NSFileHandle.h>
//#import <JSONKit.h>

namespace LynxEngine 
{		
	namespace SocialSystem 
	{		
		namespace iOS      
		{
			//-------------------------------------------------------------------------------------------------------
			//			
			//-------------------------------------------------------------------------------------------------------
			CFacebook::CFacebook(CEngine* const lpe)
			: LynxEngine::SocialSystem::CFacebook(lpe)
			{				
			}
			//-------------------------------------------------------------------------------------------------------
			//			
			//-------------------------------------------------------------------------------------------------------
			CFacebook::~CFacebook()			
			{				
			}
		}
	}
}
