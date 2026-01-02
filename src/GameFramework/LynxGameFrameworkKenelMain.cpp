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

LynxEngine::CLogger*						glpLogger = NULL;
LynxEngine::CLogger*						glpNetworkLogger = NULL;
LynxEngine::CLogger*						glpNetClientLogger = NULL;
LynxEngine::COutputDevice*					glpOutputDevice = NULL;
LynxEngine::COutputDevice*					glpNWOutputDevice = NULL;
LynxEngine::COutputDevice*					glpNetClientOutputDevice = NULL;
#if (defined(__CONTROLPANEL__))	
	LynxEngine::ControlPanel::CControlPanel*	gControlPanelWnd = NULL;
#endif


#if (!defined(__TOOL__))
	LynxEngine::GameFramework::CKernel*	glpKernel = NULL;
	LynxEngine::GameFramework::CKernel* lynxCreateKernel();
	void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk);	
#endif

#if defined(__WXWIDGET__)
	// Define a new application type, each program should derive a class from wxApp
	class DummyApp : public wxApp
	{
	public:		
		//bool Initialize(int& argc, wxChar **argv) {return true; };
		// this one is called on application startup and is a good place for the app
		// initialization (doing it here and not in the ctor allows to have an error
		// return: if OnInit() returns false, the application terminates)
		bool OnInit() {return true; };
	};
	IMPLEMENT_APP_NO_MAIN(DummyApp)		
	/*
	const wxChar* wxEmptyString = "";
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int wxString::DoPrintfWchar(char const *,...)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void wxAppConsoleBase::OnAssertFailure(char const *,int,char const *,char const *,char const *)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void wxAppConsoleBase::OnAssert(char const *,int,char const *,char const *)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	char const *wxFormatString::AsChar(void)
	{
		return NULL;
	}
	*/
	#if (__VISUALC__) && defined(_MT) && !defined(_DLL)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void wxStringData::Free(void)
		{
			::delete (this);
		}
	#endif
#endif

#if (!defined(__TOOL__))
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void lynxSetMemoryHackingDetectionFlag(LYNXBOOL b)
	{
		glpKernel->SetMemoryHackingDetectionFlag(b);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void lynxInit(LYNXBOOL blog, LYNXBOOL bcontrol_panel)
	{
		lynxInitCore();

		glpKernel = lynxCreateKernel();
	#ifdef __LYNX_MEM_MANGER__
		glpKernel->vCreateMemAllocator();
	#endif

	#ifdef __DESKTOP__
		if (blog)
		{
			glpLogger = ::new LynxEngine::CLogger();
			glpNetworkLogger = ::new LynxEngine::CLogger();
			glpNetClientLogger = ::new LynxEngine::CLogger();
		}
	#else
		#if !SHIPPING
			glpLogger = ::new LynxEngine::CLogger();	
			glpNetworkLogger = ::new LynxEngine::CLogger();	
			glpNetClientLogger = ::new LynxEngine::CLogger();
		#endif
	#endif

	#if (defined(__CONTROLPANEL__))			
		if (bcontrol_panel)
		{
			wxSetInstance((HINSTANCE)GetWindowLongPtr(glpKernel->GetWindow(), GWLP_HINSTANCE));
			wxInitialize();

			RECT Rect;
			GetWindowRect(glpKernel->GetWindow(), &Rect);
			gControlPanelWnd = ::new LynxEngine::ControlPanel::CControlPanel(glpKernel->GetlpEngine(), _T("wxControlPanel"), wxPoint(Rect.right, Rect.top), wxSize(640, (Rect.bottom - Rect.top)));
			gControlPanelWnd->Show(true);
			glpKernel->m_lpControlPanel = gControlPanelWnd;
		}
	#endif		

	#ifdef __WIN32__
		#if (defined(__CONTROLPANEL__))	
			if (gControlPanelWnd)
				glpOutputDevice = gControlPanelWnd;
			else
				glpOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#else
			glpOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#endif
	#elif defined __iOS__
		glpOutputDevice = ::new LynxEngine::iOS::CConsoleOutputDevice();
	#elif defined __ANDROID__
		glpOutputDevice = ::new LynxEngine::Android::CLogCatOutputDevice();
	#else
		glpOutputDevice = ::new LynxEngine::CFileOutputDevice();
	#endif
		if (glpLogger)
		{
			glpLogger->vCreate(glpOutputDevice, LynxEngine::CString("LynxEngine.log"));
			glpLogger->vClearLog();
		}
		
	#ifdef __WIN32__
		#if (defined(__CONTROLPANEL__))	
			if (gControlPanelWnd && gControlPanelWnd->GetNetworkLogOutputDevice())
				glpNWOutputDevice = gControlPanelWnd->GetNetworkLogOutputDevice();
			else
				glpNWOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#else
			glpNWOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#endif
	#elif defined __iOS__				
		glpNWOutputDevice = ::new LynxEngine::CFileOutputDevice();
	#elif defined __ANDROID__
		glpNWOutputDevice = ::new LynxEngine::Android::CLogCatOutputDevice();
		// glpNWOutputDevice = ::new LynxEngine::CFileOutputDevice();
	#else
	#endif
		if (glpNetworkLogger)
		{
			glpNetworkLogger->vCreate(glpNWOutputDevice, LynxEngine::CString("LynxEngine_Network.log"));
			glpNetworkLogger->vClearLog();
		}

	#ifdef __WIN32__
		#if (defined(__CONTROLPANEL__))	
			if (gControlPanelWnd && gControlPanelWnd->GetNetClientLogOutputDevice())
				glpNetClientOutputDevice = gControlPanelWnd->GetNetClientLogOutputDevice();
			else
				glpNetClientOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#else
		glpNetClientOutputDevice = ::new LynxEngine::CFileOutputDevice();
		#endif
	#elif defined __iOS__				
		glpNetClientOutputDevice = ::new LynxEngine::CFileOutputDevice();
	#elif defined __ANDROID__
		glpNetClientOutputDevice = ::new LynxEngine::Android::CLogCatOutputDevice();
		// glpNWOutputDevice = ::new LynxEngine::CFileOutputDevice();
	#else
	#endif
		if (glpNetClientLogger)
		{
			glpNetClientLogger->vCreate(glpNetClientOutputDevice, LynxEngine::CString("LynxEngine_NetClient.log"));
			glpNetClientLogger->vClearLog();
		}

		if (glpLogger)
		{
			LYNX_LOG(glpLogger, _T("    __                    ____            __\n"));
			LYNX_LOG(glpLogger, _T("   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____\n"));
			LYNX_LOG(glpLogger, _T("  / /__/ // /  _ \\ \\/ / / __/  _ \\/ __ \\/ /  _ \\/  _/\n"));
			LYNX_LOG(glpLogger, _T("  \\___/__, /__//_/_/\\_\\ \\__/__//_/\\__, /_/__//_/\\__/\n"));
			LYNX_LOG(glpLogger, _T("      /___/                       /___/ \n"));
			LYNX_LOG(glpLogger, _T(" \n"));
			LYNX_LOG(glpLogger, _T("************************LynxEngine Log******************************\n\n"));
		}

		if (glpNetworkLogger)
		{
			LYNX_LOG(glpNetworkLogger, _T("    __                    ____            __\n"));
			LYNX_LOG(glpNetworkLogger, _T("   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____\n"));
			LYNX_LOG(glpNetworkLogger, _T("  / /__/ // /  _ \\ \\/ / / __/  _ \\/ __ \\/ /  _ \\/  _/\n"));
			LYNX_LOG(glpNetworkLogger, _T("  \\___/__, /__//_/_/\\_\\ \\__/__//_/\\__, /_/__//_/\\__/\n"));
			LYNX_LOG(glpNetworkLogger, _T("      /___/                       /___/ \n"));
			LYNX_LOG(glpNetworkLogger, _T(" \n"));
			LYNX_LOG(glpNetworkLogger, _T("************************LynxEngine Network Log******************************\n\n"));
		}

		if (glpNetClientLogger)
		{
			LYNX_LOG(glpNetClientLogger, _T("    __                    ____            __\n"));
			LYNX_LOG(glpNetClientLogger, _T("   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____\n"));
			LYNX_LOG(glpNetClientLogger, _T("  / /__/ // /  _ \\ \\/ / / __/  _ \\/ __ \\/ /  _ \\/  _/\n"));
			LYNX_LOG(glpNetClientLogger, _T("  \\___/__, /__//_/_/\\_\\ \\__/__//_/\\__, /_/__//_/\\__/\n"));
			LYNX_LOG(glpNetClientLogger, _T("      /___/                       /___/ \n"));
			LYNX_LOG(glpNetClientLogger, _T(" \n"));
			LYNX_LOG(glpNetClientLogger, _T("************************LynxEngine NetClient Log******************************\n\n"));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void lynxRelease()
	{
		lynxReleaseKernel(glpKernel);
		glpKernel = NULL;
		lynxReleaseCore();

		LYNX_LOG(glpLogger, _T("************************End of LynxEngine Log******************************\n\n"));							

		LYNX_LOG(glpNetworkLogger, _T("************************End of LynxEngine Network Log******************************\n\n"));							
				
		if (glpLogger)
			::delete glpLogger;
		glpLogger = NULL;

		if (glpOutputDevice)
			::delete glpOutputDevice;
		glpOutputDevice = NULL;		

		if (glpNetworkLogger)
			::delete glpNetworkLogger;
		glpNetworkLogger = NULL;

		#if (defined(__CONTROLPANEL__))	
			if (!gControlPanelWnd)
			{
				if (glpOutputDevice)
					::delete glpOutputDevice;
				glpOutputDevice = NULL;

				if (glpNWOutputDevice)
					::delete glpNWOutputDevice;
				glpNWOutputDevice = NULL;

				if (glpNetClientOutputDevice)
					::delete glpNetClientOutputDevice;
				glpNetClientOutputDevice = NULL;
			}
		#else
			if (glpOutputDevice)
				::delete glpOutputDevice;
			glpOutputDevice = NULL;

			if (glpNWOutputDevice)
				::delete glpNWOutputDevice;
			glpNWOutputDevice = NULL;

			if (glpNetClientOutputDevice)
				::delete glpNetClientOutputDevice;
			glpNetClientOutputDevice = NULL;			
		#endif

		/*
		#if (defined(__CONTROLPANEL__))
			if (gControlPanelWnd)
				::delete gControlPanelWnd;
			wxUninitialize();	
		#endif       
		*/        
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int	lynxMain(const int argc, const char **argv)
	{	
		glpKernel->vInit(argc, argv);

		#if (defined(__CONTROLPANEL__))
			if (glpKernel->m_lpControlPanel)
			{
				glpKernel->m_lpControlPanel->SetlpEngine(glpKernel->GetlpEngine());

				RECT Rect;
				GetWindowRect(glpKernel->GetWindow(), &Rect);			
				gControlPanelWnd->SetSize(Rect.right, Rect.top, 640, (Rect.bottom-Rect.top));
				gControlPanelWnd->Show(true);
				gControlPanelWnd->SetFocus();
				gControlPanelWnd->Update();
			}
		#endif				

		while (glpKernel->vLoop()); 				

		glpKernel->vRelease();		
		
		return 0;	
	}
#endif