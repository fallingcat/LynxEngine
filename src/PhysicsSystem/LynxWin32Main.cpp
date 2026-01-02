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
#include "../proj/Win32/resource.h"
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <ControlPanel/LynxControlPanel.h>
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxDebugMemAllocator.h>
#include <LynxSmallObjAllocator.h>
#include <LynxReleaseMemAllocator.h>

//Todo : change glpLogger and glpKernel to Singleton?
extern  LynxEngine::CLogger* glpLogger;

#if (!defined(__TOOL__))
	extern LynxEngine::GameFramework::CKernel*	glpKernel;

	LynxEngine::GameFramework::CKernel* lynxCreateKernel();
	void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk);
#endif

#if (!defined(__TOOL__) && !defined(__LYNXENGINE_DLL__))
	#ifdef __WIN32__
		#ifdef __WINAPP__
			/*
			#include <LynxView.h>
			#include <CoreWindowEvents.h>

			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			ApplicationModel::Infrastructure::IViewProvider^ LynxViewFactory::CreateViewProvider()
			{					
				return ref new LynxView();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void LynxView::Initialize(_In_ LYNXWINDOW window, _In_ ApplicationModel::Core::CoreApplicationView^ applicationView)
			{
				lynxInit();
				glpKernel->vInit();

				#if (defined(__CONTROLPANEL__))
					if (glpKernel->m_lpControlPanel)
						glpKernel->m_lpControlPanel->SetlpEngine(glpKernel->GetlpEngine());

					RECT Rect;
					GetWindowRect(glpKernel->GetWindow(), &Rect);			
					gControlPanelWnd->SetSize(Rect.right, Rect.top, 640, (Rect.bottom-Rect.top));
					gControlPanelWnd->Update();
				#endif		
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void LynxView::Load(_In_ Platform::String^ entryPoint)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void LynxView::Run()
			{
				glpKernel->vLoop(); 		
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void LynxView::Uninitialize()
			{
				glpKernel->vRelease();					
				lynxRelease();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			[Platform::MTAThread]
			int main(array<Platform::String^>^)
			{
				auto ViewProviderFactory = ref new LynxViewFactory();
				ApplicationModel::Core::CoreApplication::Run(ViewProviderFactory);				
			}
			*/
		#else
			#include <commctrl.h>

			#define TIMER_ONE_TICK                               0x11

			HINSTANCE			ghInstance;	
			LYNXCHAR			gClassName[] = _T("Lynx Framework");		
			int					lynxMain(const int argc, const char **argv);		

			//------------------------------------------------------------------
			// 
			// Function     : WndProc()
			//
			// Purpose      : Windows procedure to handle messages
			//
			//------------------------------------------------------------------
			LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				glpKernel->m_KBD[VK_MENU] = (BYTE)GetAsyncKeyState(VK_MENU);  
				glpKernel->m_KBD[VK_LMENU] = (BYTE)GetAsyncKeyState(VK_LMENU);  
				glpKernel->m_KBD[VK_RMENU] = (BYTE)GetAsyncKeyState(VK_RMENU);  
				
				switch( message ) 
				{
					case WM_CREATE:
						/*
						if (!InitApplication())
						{
							DestroyWindow(hWnd);		
						}
						*/
						break;

					case WM_TIMER:
						switch (wParam)
						{
							case TIMER_ONE_TICK:
								break;
						}
						break;
		        
					case WM_ENTERMENULOOP:
						//glpGameSystem->Pause(TRUE);
						return 0;            

					case WM_EXITMENULOOP:
						//glpGameSystem->Pause(FALSE);
						return 0;

					/*case WM_ACTIVATEAPP:
						// **This if up to application**
						glpGameSystem->OnActivate(wParam,lParam);
						return 0;*/

					case WM_CHAR:				
						glpKernel->m_CharPressed = wParam; 			
						return 0;

					case WM_KEYDOWN:			
						glpKernel->vOnSysKeyDown(wParam);						
						return 0;

					case WM_KEYUP:
						glpKernel->vOnSysKeyUp(wParam);						
						return 0;

					case WM_LBUTTONDOWN:
						glpKernel->m_KBD[VK_LBUTTON] = TRUE;           					
						return 0;

					case WM_LBUTTONUP:
						glpKernel->m_KBD[VK_LBUTTON] = FALSE;           					
						return 0;			

					case WM_RBUTTONDOWN:					
						glpKernel->m_KBD[VK_RBUTTON] = TRUE;           
						return 0;

					case WM_RBUTTONUP:
						glpKernel->m_KBD[VK_RBUTTON] = FALSE;           					
						return 0;
						
					case WM_MOUSEMOVE :
						//glpGameSystem->MouseData.CurrentPos.x = LOWORD(lParam);
						//glpGameSystem->MouseData.CurrentPos.y = HIWORD(lParam);
						return 0;
		        
					case WM_SYSCOMMAND:	
						if (!glpKernel->IsMultiTask())		
							return 0;					
						break;  

					//case WM_GRAPHNOTIFY:
						//HandleGraphEvent();
						//break;		
		        
					case WM_MOVE :
						if (glpKernel && glpKernel->GetlpEngine() && ((LynxEngine::CEngine*)glpKernel->GetlpEngine())->GetlpGraphicsSystem() && ((LynxEngine::CEngine*)glpKernel->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer())
						{							
							((LynxEngine::CEngine*)glpKernel->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer()->SetSystemOrigin((int) LOWORD(lParam), (int) HIWORD(lParam));
						}
						return 0;
						
					case WM_CLOSE:         					
						glpKernel->SetRunning(LYNX_FALSE);
						CoUninitialize();
						DestroyWindow(hWnd);		
						return 0;
		    
					case WM_DESTROY:
						ShowCursor(TRUE);
						PostQuitMessage(0);
						return 0;       		
				}
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			//------------------------------------------------------------------
			// 
			// Function     : InitClass()
			//
			// Purpose      : Initialises and registers window class
			//
			//------------------------------------------------------------------
			LYNXBOOL InitClass(HINSTANCE hInst, LYNXCHAR* lpIcon)
			{
				WNDCLASS    wndClass;

				// Fill out WNDCLASS info
				wndClass.style              = CS_HREDRAW | CS_VREDRAW;
				wndClass.lpfnWndProc        = WndProc;
				wndClass.cbClsExtra         = 0;
				wndClass.cbWndExtra         = 0;
				wndClass.hInstance          = hInst;
				wndClass.hIcon              = LoadIcon(hInst, lpIcon);
				wndClass.hCursor            = LoadCursor(NULL, IDC_ARROW);
				wndClass.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
				wndClass.lpszMenuName       = NULL;
				wndClass.lpszClassName      = gClassName;
		
				if( !RegisterClass(&wndClass) ) return FALSE;
		
				// Everything's perfect
				return TRUE;
			}

			//------------------------------------------------------------------
			// 
			// Function     : InitWindow()
			//
			// Purpose      : Initialises and creates the main window
			//
			//------------------------------------------------------------------
			LYNXBOOL InitWindow(HINSTANCE hInst)
			{
				ghInstance = hInst;	
				LYNXWINDOW	Window;

				// Create a window
				#ifdef __DIRECT3D__	
					Window = CreateWindowEx(WS_EX_APPWINDOW,
											gClassName, 
											_T("Lynx Engine"),
											WS_OVERLAPPEDWINDOW | WS_VISIBLE | 
	  										WS_POPUP | WS_SYSMENU, 
											0, 0,
											320, 200,
											NULL,
											NULL,
											hInst,
											NULL);
				#else		
					Window = CreateWindowEx(WS_EX_APPWINDOW,
											gClassName, 
											_T("Lynx Engine"),										
											WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
											0, 0,
											320, 200,
											NULL,
											NULL,
											hInst,
											NULL);
				#endif   
				
				// Return false if window creation failed
				if( !Window ) return FALSE;

				glpKernel->SetWindow(Window);
		
				// Show the window
				ShowWindow(Window, SW_SHOWNORMAL);

				// Update the window
				UpdateWindow(Window); 

				//SetTimer(gWindow, 3/*SERVER_EVENT*/,  30,  NULL);
		
				// Everything's perfect
				return TRUE;
			}

			//------------------------------------------------------------------
			// 
			// Function     : WinMain()
			//
			// Purpose      : Entry point to application
			//
			//------------------------------------------------------------------
			int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
			{
				char				*Arg;  
				char				*CmdLine;
				int					argc, i;
				char				**argv; 
				LYNXCHAR			IconName[64];						
				int					Len;

				Len = (int)strlen(lpCmdLine);

				if (Len)
					CmdLine = (char *)malloc(Len+1);
				else
					CmdLine = (char *)malloc(4);

				strcpy(CmdLine, lpCmdLine);

				argc = 1;	
				Arg = strtok(lpCmdLine, " ");	
				while(Arg != NULL ) 
				{
					argc++;
					Arg = strtok(NULL, " ");		
				}
				argv = (char **)malloc(sizeof(char *) * argc);

				argv[0] = NULL;
				argc = 1;
				Arg = strtok(CmdLine, " ");	
				while(Arg != NULL ) 
				{		
					argv[argc] = (char *)malloc(sizeof(char) * strlen(Arg) + 1);
					strcpy(argv[argc], Arg);
					argc++;
					Arg = strtok(NULL, " ");
				}
				free(CmdLine);

				#if ((defined(DEBUG) || defined(_DEBUG)) && defined(__MEMORYLEAK_CHECK__))
					_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
					//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
				#endif

				//if (hInst)
				//{
				//	LoadString(hInst, 2, IconName, sizeof(IconName));
				//}			
				//     
				LYNXBOOL bLog = LYNX_FALSE;
				LYNXBOOL bControlPanel = LYNX_FALSE;

				for (int i=0; i<argc; i++)
				{
					if (argv[i])
					{
						LynxEngine::CString sArg = argv[i];
						if (sArg == _T("-log"))
						{
							bLog = LYNX_TRUE;
						}
						else if (sArg == _T("-cp"))
						{
							bControlPanel = LYNX_TRUE;
						}
					}
				}
				lynxInit(bLog, bControlPanel);

				// Initialise window class
				if( !InitClass(hInst, MAKEINTRESOURCE(IDI_APPLICATION)) ) 
					return 1;

				// Initialise window
				if( !InitWindow(hInst) ) 
					return 1;

				// Initialise COM object
				if (FAILED(CoInitialize(NULL)))
					return 1;

				// Main function
				lynxMain(argc, (const char**)argv);		

				lynxRelease();	

				for (i=0; i<argc; i++)
				{
					if (argv[i])
						free(argv[i]);
				}
				free(argv);						

				#if ((defined(DEBUG) || defined(_DEBUG)) && defined(__MEMORYLEAK_CHECK__))
					_CrtDumpMemoryLeaks();
				#endif

				// Exit WinMain and terminate the app....
				return 1;			
			}
		#endif  //WINAPT_FAMILY >= 2
	#endif	
#endif