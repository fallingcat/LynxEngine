//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/07/06
//  Last Update : 
//
//  說明:
//
//
//###########################################################################

#include <LynxEngine_PCH.h>

#if (!defined(__TOOL__) && !defined(__WEB__))
	#include <GameFramework/LynxGameFrameworkKernel.h>	
#endif
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <ControlPanel/LynxControlPanel.h>
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxDebugMemAllocator.h>
#include <LynxSmallObjAllocator.h>
#include <LynxReleaseMemAllocator.h>
#include <Platform/Windows8/LynxView.h>

#ifdef __WINAPP__

	//Todo : change glpLogger and glpKernel to Singleton?
	extern  LynxEngine::CLogger* glpLogger;

	#if (!defined(__TOOL__))
		extern LynxEngine::GameFramework::CKernel*	glpKernel;

		LynxEngine::GameFramework::CKernel* lynxCreateKernel();
		void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk);
	#endif	

	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	ref class LynxEngineAppSource : Windows::ApplicationModel::Core::IFrameworkViewSource
	{
	public:
		Windows::ApplicationModel::Core::IFrameworkView^ CreateView()
		{
			return new LynxView();
		}
	};
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	[Platform::MTAThread]
	int main(Platform::Array<Platform::String^>^)
	{
		auto AppSource = ref new LynxEngineAppSource();
		CoreApplication::Run(AppSource);

		return 0;
	}

#endif
