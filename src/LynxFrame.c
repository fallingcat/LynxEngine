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

#include <LynxCore.h>
#include <LynxSDK.h>
#include <LynxObj3D.h>
#include <LynxCamera.h>
#include <LynxBone.h>
#include <LynxMesh.h>
#include <LynxModel.h>

LYNXWINDOW			gWindow;

//#ifdef __WIN32__
//	#if (defined(_WINDOWS) || (defined(__LYNXSDK_LIB__) && !defined(_XBOX) && !defined(__TOOL__)) )
//		#include <commctrl.h>
//
//		#define TIMER_ONE_TICK                               0x11
//
//		extern LPLYNXSDK	glpSDK;		
//		HINSTANCE			ghInstance;	
//		char				gClassName[] = "Lynx Frame";		
//		LYNXEXTERN int		main(int argc, char **argv);		
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : WndProc()
//		//
//		// Purpose      : Windows procedure to handle messages
//		//
//		//------------------------------------------------------------------
//		LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//		{
//			if (glpSDK)
//			{
//				glpSDK->KBD[VK_MENU] = (BYTE)GetAsyncKeyState(VK_MENU);  
//				glpSDK->KBD[VK_LMENU] = (BYTE)GetAsyncKeyState(VK_LMENU);  
//				glpSDK->KBD[VK_RMENU] = (BYTE)GetAsyncKeyState(VK_RMENU);  
//			}
//			switch( message ) 
//			{
//				case WM_CREATE:
//					/*
//					if (!InitApplication())
//					{
//						DestroyWindow(hWnd);		
//					}
//					*/
//					break;
//
//				case WM_TIMER:
//					switch (wParam)
//					{
//						case TIMER_ONE_TICK:
//							break;
//					}
//					break;
//            
//				case WM_ENTERMENULOOP:
//					//glpGameSystem->Pause(TRUE);
//					return 0;            
//
//				case WM_EXITMENULOOP:
//					//glpGameSystem->Pause(FALSE);
//					return 0;
//
//				/*case WM_ACTIVATEAPP:
//					// **This if up to application**
//					glpGameSystem->OnActivate(wParam,lParam);
//					return 0;*/
//
//				case WM_CHAR:
//					if (glpSDK)
//						glpSDK->CharPressed = wParam; 			
//					return 0;
//
//				case WM_KEYDOWN:
//					if (glpSDK)
//					{
//						glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_DOWN;
//						glpSDK->KeyPressed = (BYTE)wParam; 			
//						glpSDK->KBD[(BYTE)wParam] = TRUE;
//						//if (glpEvent->Key ==  VK_ESCAPE)
//						//	glpSDK->bRunning = FALSE;
//						//  DestroyWindow(hWnd);
//					}
//					return 0;
//
//				case WM_KEYUP:
//					if (glpSDK)
//					{
//						glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_UP;
//						//glpGameSystem->KeyPressed = -1;
//						//glpGameSystem->CharPressed = -1;
//						glpSDK->KBD[(BYTE)wParam] = FALSE;           
//					}
//					return 0;
//
//				case WM_LBUTTONDOWN:
//					if (glpSDK)
//						glpSDK->KBD[VK_LBUTTON] = TRUE;           
//					//glpGameSystem->MouseData.bLeftButtonDown = TRUE;
//					return 0;
//
//				case WM_LBUTTONUP:
//					if (glpSDK)
//						glpSDK->KBD[VK_LBUTTON] = FALSE;           
//					//glpGameSystem->MouseData.bLeftButtonDown = FALSE;
//					return 0;			
//
//				case WM_RBUTTONDOWN:
//					if (glpSDK)
//						glpSDK->KBD[VK_RBUTTON] = TRUE;           
//					//glpGameSystem->MouseData.bRightButtonDown = TRUE;
//					return 0;
//
//				case WM_RBUTTONUP:
//					if (glpSDK)
//						glpSDK->KBD[VK_RBUTTON] = FALSE;           
//					//glpGameSystem->MouseData.bRightButtonDown = FALSE;
//					return 0;
//					
//				case WM_MOUSEMOVE :
//					//glpGameSystem->MouseData.CurrentPos.x = LOWORD(lParam);
//					//glpGameSystem->MouseData.CurrentPos.y = HIWORD(lParam);
//					return 0;
//            
//				case WM_SYSCOMMAND:	
//					if (glpSDK)
//					{
//						if (glpSDK->bDisableMultiTask)		
//							return 0;
//							//glpGameSystem->KBD[(char)VK_MENU] = TRUE;
//					}
//					break;  
//
//				//case WM_GRAPHNOTIFY:
//					//HandleGraphEvent();
//					//break;		
//         
//				case WM_MOVE :
//					if (glpSDK)
//					{
//						glpSDK->Renderer.SystemData.Origin.x = (int) LOWORD(lParam);    // horizontal position 
//						glpSDK->Renderer.SystemData.Origin.y = (int) HIWORD(lParam);    // vertical position 
//					}
//					return 0;
//					
//				case WM_CLOSE:         
//					if (glpSDK)					
//						glpSDK->bRunning = FALSE;
//					CoUninitialize();
//					DestroyWindow(hWnd);		
//					return 0;
//        
//				case WM_DESTROY:
//					ShowCursor(TRUE);
//					PostQuitMessage(0);
//					return 0;       		
//			}
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : InitClass()
//		//
//		// Purpose      : Initialises and registers window class
//		//
//		//------------------------------------------------------------------
//
//		LYNXBOOL    InitClass(HINSTANCE hInst, LPSTR lpIcon)
//		{
//			WNDCLASS    wndClass;
//
//			// Fill out WNDCLASS info
//			wndClass.style              = CS_HREDRAW | CS_VREDRAW;
//			wndClass.lpfnWndProc        = WndProc;
//			wndClass.cbClsExtra         = 0;
//			wndClass.cbWndExtra         = 0;
//			wndClass.hInstance          = hInst;
//			wndClass.hIcon              = LoadIcon(hInst, lpIcon);
//			wndClass.hCursor            = LoadCursor(NULL, IDC_ARROW);
//			wndClass.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
//			wndClass.lpszMenuName       = NULL;
//			wndClass.lpszClassName      = gClassName;
//    
//			if( !RegisterClass(&wndClass) ) return FALSE;
//  
//			// Everything's perfect
//			return TRUE;
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : InitWindow()
//		//
//		// Purpose      : Initialises and creates the main window
//		//
//		//------------------------------------------------------------------
//
//		LYNXBOOL    InitWindow(HINSTANCE hInst)
//		{
//			ghInstance = hInst;	
//
//			// Create a window
//			#ifdef __DIRECT3D__	
//				gWindow =  CreateWindowEx(WS_EX_APPWINDOW,
//										   gClassName, 
//										   "Lynx Engine",
//										   WS_OVERLAPPEDWINDOW | WS_VISIBLE | 
//		  								   WS_POPUP | WS_SYSMENU, 
//										   0, 0,
//										   320, 200,
//										   NULL,
//										   NULL,
//										   hInst,
//										   NULL);
//			#else		
//				gWindow =  CreateWindowEx(WS_EX_APPWINDOW,
//										   gClassName, 
//										   "Lynx Engine",
//										   //WS_OVERLAPPEDWINDOW | WS_VISIBLE | 
//		  								   //WS_POPUP | WS_SYSMENU, 
//										   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
//										   0, 0,
//										   320, 200,
//										   NULL,
//										   NULL,
//										   hInst,
//										   NULL);
//			#endif
//    
//			// Return false if window creation failed
//			if( !gWindow ) return FALSE;
//    
//			// Show the window
//			ShowWindow(gWindow, SW_SHOWNORMAL);
//
//			// Update the window
//			UpdateWindow(gWindow); 
//
//			#ifdef __NUCLEUS__
//				#ifdef __SIS_MOD__				
//					mmpGetScreenDisplay((MMP_UINT32*)&gWindow);
//				#endif
//			#endif
//
//			//SetTimer(gWindow, 3/*SERVER_EVENT*/,  30,  NULL);
//    
//			// Everything's perfect
//			return TRUE;
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : WinMain()
//		//
//		// Purpose      : Entry point to application
//		//
//		//------------------------------------------------------------------
//
//		 int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
//		{
//			char				*Arg;  
//			char				*CmdLine;
//			int					argc, i;
//			char				**argv; 
//			char				IconName[64];						
//			int					Len;
//
//			Len = strlen(lpCmdLine);
//
//			if (Len)
//				CmdLine = (char *)malloc(Len+1);
//			else
//				CmdLine = (char *)malloc(4);
//
//			strcpy(CmdLine, lpCmdLine);
//
//			argc = 1;	
//			Arg = strtok(lpCmdLine, " ");	
//			while(Arg != NULL ) 
//			{
//				argc++;
//				Arg = strtok(NULL, " ");		
//			}
//			argv = (char **)malloc(sizeof(char *) * argc);
//
//			argv[0] = NULL;
//			argc = 1;
//			Arg = strtok(CmdLine, " ");	
//			while(Arg != NULL ) 
//			{		
//				argv[argc] = (char *)malloc(sizeof(char) * strlen(Arg) + 1);
//				strcpy(argv[argc], Arg);
//				argc++;
//				Arg = strtok(NULL, " ");
//			}
//			free(CmdLine);
//
//			//if (hInst)
//			//{
//			//	LoadString(hInst, 2, IconName, sizeof(IconName));
//			//}			
//   //     
//			// Initialise window class
//			if( !InitClass(hInst, IconName) ) 
//				return 1;
//
//			// Initialise window
//			if( !InitWindow(hInst) ) 
//				return 1;
//
//			// Initialise COM object
//			if (FAILED(CoInitialize(NULL)))
//				return 1;
//			//
//			///*
//			//if (!SetTimer(glpGameSystem->hWndMain, TIMER_ONE_TICK, 30, NULL))
//			//{        
//			//	glpGameSystem->vReleaseGame();
//			//	glpGameSystem->Release();
//			//	PostMessage(glpGameSystem->hWndMain, WM_CLOSE, 0L, 0L);
//			//	return 1;
//			//} 
//			//*/
//			//
//			main(argc, argv);				
//
//			for (i=0; i<argc; i++)
//			{
//				if (argv[i])
//					free(argv[i]);
//			}
//			free(argv);						
//
//			////// Initialise window class
//			//if( !InitClass(hInst, IconName) ) 
//			//	return 1;
//
//			////// Initialise window
//			//if( !InitWindow(hInst) ) 
//			//	return 1;
//
//			////// Initialise COM object
//			//if (FAILED(CoInitialize(NULL)))
//			//	return 1;
//
//			//main(nCmdShow, &lpCmdLine);
//			
//			// Exit WinMain and terminate the app....
//			return 1;
//			//return gMsg.wParam;
//		}
//	#else
//	#endif
//#elif __WINCE__
//	#if (defined(_WINDOWS) || (defined(_LIB) && !defined(_XBOX)) )
//		#include <commctrl.h>		
//		#include <sipapi.h>
//		  #ifdef __POCKET_PC__
//			#include <aygshell.h>
//		#endif
//
//		HINSTANCE					ghInstance;	
//		char						gClassName[] = "Lynx Frame";		
//		HWND						ghWndCB;// The command bar handle
//		#ifdef __POCKET_PC__
//			static SHACTIVATEINFO		gSAI;
//		#endif
//
//		LYNXEXTERN int				main(int argc, char **argv);		
//		//------------------------------------------------------------------
//		// 		
//		//------------------------------------------------------------------		
//		HWND CreateRpCommandBar(HWND hwnd)
//		{
//			#ifdef __POCKET_PC__
//				SHMENUBARINFO mbi;
//
//				memset(&mbi, 0, sizeof(SHMENUBARINFO));
//				mbi.cbSize     = sizeof(SHMENUBARINFO);
//				mbi.hwndParent = hwnd;
//				mbi.nToolBarId = 0;//IDM_MENU;
//				mbi.hInstRes   = ghInstance;
//				mbi.nBmpId     = 0;
//				mbi.cBmpImages = 0;
//
//				if (!SHCreateMenuBar(&mbi)) 
//					return NULL;
//
//				return mbi.hwndMB;
//			#else
//				return 0;
//			#endif
//		}		
//		//------------------------------------------------------------------
//		// 
//		// Function     : WndProc()
//		//
//		// Purpose      : Windows procedure to handle messages
//		//
//		//------------------------------------------------------------------
//		LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//		{
//			if (glpSDK)
//			{
//				glpSDK->KBD[VK_MENU] = (BYTE)GetAsyncKeyState(VK_MENU);  
//				glpSDK->KBD[VK_LMENU] = (BYTE)GetAsyncKeyState(VK_LMENU);  
//				glpSDK->KBD[VK_RMENU] = (BYTE)GetAsyncKeyState(VK_RMENU);  
//			}
//			switch( message ) 
//			{
//				case WM_CREATE:
//					#ifdef __POCKET_PC__					
//						ghWndCB = CreateRpCommandBar(hWnd);
//						// Initialize the shell activate info structure					
//						memset (&gSAI, 0, sizeof (gSAI));
//						gSAI.cbSize = sizeof (gSAI);			
//					#endif					
//					break;
//
//				case WM_TIMER:
//					switch (wParam)
//					{
//						case TIMER_ONE_TICK:
//							break;
//					}
//					break;
//            
//				//case WM_ENTERMENULOOP:
//					//glpGameSystem->Pause(TRUE);
//				//	return 0;            
//
//				//case WM_EXITMENULOOP:
//					//glpGameSystem->Pause(FALSE);
//				//	return 0;
//
//				/*case WM_ACTIVATEAPP:
//					// **This if up to application**
//					glpGameSystem->OnActivate(wParam,lParam);
//					return 0;*/
//
//				case WM_CHAR:
//					if (glpSDK)
//						glpSDK->CharPressed = wParam; 			
//					return 0;
//
//				case WM_KEYDOWN:
//					if (glpSDK)
//					{
//						glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_DOWN;
//						glpSDK->KeyPressed = (BYTE)wParam; 			
//						glpSDK->KBD[(BYTE)wParam] = TRUE;
//						//if (glpEvent->Key ==  VK_ESCAPE)
//						//	glpSDK->bRunning = FALSE;
//						//  DestroyWindow(hWnd);
//					}
//					return 0;
//
//				case WM_KEYUP:
//					if (glpSDK)
//					{
//						glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_UP;
//						//glpGameSystem->KeyPressed = -1;
//						//glpGameSystem->CharPressed = -1;
//						glpSDK->KBD[(BYTE)wParam] = FALSE;           
//					}
//					return 0;
//
//				case WM_LBUTTONDOWN:
//					if (glpSDK)
//						glpSDK->KBD[VK_LBUTTON] = TRUE;           
//					//glpGameSystem->MouseData.bLeftButtonDown = TRUE;
//					return 0;
//
//				case WM_LBUTTONUP:
//					if (glpSDK)
//						glpSDK->KBD[VK_LBUTTON] = FALSE;           
//					//glpGameSystem->MouseData.bLeftButtonDown = FALSE;
//					return 0;			
//
//				case WM_RBUTTONDOWN:
//					if (glpSDK)
//						glpSDK->KBD[VK_RBUTTON] = TRUE;           
//					//glpGameSystem->MouseData.bRightButtonDown = TRUE;
//					return 0;
//
//				case WM_RBUTTONUP:
//					if (glpSDK)
//						glpSDK->KBD[VK_RBUTTON] = FALSE;           
//					//glpGameSystem->MouseData.bRightButtonDown = FALSE;
//					return 0;
//					
//				//case WM_MOUSEMOVE :
//					//glpGameSystem->MouseData.CurrentPos.x = LOWORD(lParam);
//					//glpGameSystem->MouseData.CurrentPos.y = HIWORD(lParam);
//				//	return 0;
//            
//				case WM_SYSCOMMAND:	
//					if (glpSDK)
//					{
//						if (glpSDK->bDisableMultiTask)		
//							return 0;
//							//glpGameSystem->KBD[(char)VK_MENU] = TRUE;
//					}
//					break;  
//
//				//case WM_GRAPHNOTIFY:
//					//HandleGraphEvent();
//					//break;		
//         
//				case WM_MOVE :
//					if (glpSDK)
//					{
//						glpSDK->Render.SystemData.Origin.x = (int) LOWORD(lParam);    // horizontal position 
//						glpSDK->Render.SystemData.Origin.y = (int) HIWORD(lParam);    // vertical position 
//					}
//					return 0;
//					
//				case WM_CLOSE:         
//					if (glpSDK)					
//						glpSDK->bRunning = FALSE;					
//					DestroyWindow(hWnd);		
//					return 0;
//        
//				case WM_DESTROY:
//					if (glpSDK)					
//						glpSDK->bRunning = FALSE;					
//					CommandBar_Destroy(ghWndCB);
//					ShowCursor(TRUE);
//					PostQuitMessage(0);
//					return 0;       		
//			}
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : InitClass()
//		//
//		// Purpose      : Initialises and registers window class
//		//
//		//------------------------------------------------------------------
//
//		LYNXBOOL    InitClass(HINSTANCE hInst, LPSTR lpIcon)
//		{
//			WNDCLASS    wndClass;
//
//			// Fill out WNDCLASS info
//			wndClass.style              = 0;//CS_HREDRAW | CS_VREDRAW;
//			wndClass.lpfnWndProc        = WndProc;
//			wndClass.cbClsExtra         = 0;
//			wndClass.cbWndExtra         = 0;
//			wndClass.hInstance          = hInst;
//			wndClass.hIcon              = NULL;//LoadIcon(hInst, lpIcon);
//			wndClass.hCursor            = LoadCursor(NULL, IDC_ARROW);
//			wndClass.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
//			wndClass.lpszMenuName       = NULL;
//			wndClass.lpszClassName      = gClassName;
//    
//			if( !RegisterClass(&wndClass) ) return FALSE;
//  
//			// Everything's perfect
//			return TRUE;
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : InitWindow()
//		//
//		// Purpose      : Initialises and creates the main window
//		//
//		//------------------------------------------------------------------
//
//		LYNXBOOL    InitWindow(HINSTANCE hInst)
//		{
//			RECT			rc;
//			RECT			rcMenuBar;
//
//			ghInstance = hInst;				
//
//			// Create a window						
//			gWindow = CreateWindow(gClassName, 
//									"Lynx Engine",	
//									WS_VISIBLE,
//									CW_USEDEFAULT,
//									CW_USEDEFAULT,
//									GetSystemMetrics(SM_CXSCREEN),
//									GetSystemMetrics(SM_CYSCREEN),
//									NULL,
//									NULL,
//									hInst,
//									NULL);
//    
//			// Return false if window creation failed
//			if( !gWindow ) return FALSE;
//
//			if (ghWndCB)
//			{
//				GetWindowRect(gWindow, &rc);
//				GetWindowRect(ghWndCB, &rcMenuBar);
//				rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
//				
//				MoveWindow(gWindow, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
//			}
//    
//			// Show the window
//			ShowWindow(gWindow, SW_SHOWNORMAL);
//
//			// Update the window
//			UpdateWindow(gWindow); 
//
//			//SetTimer(gWindow, 3/*SERVER_EVENT*/,  30,  NULL);
//    
//			// Everything's perfect
//			return TRUE;
//		}
//
//		//------------------------------------------------------------------
//		// 
//		// Function     : WinMain()
//		//
//		// Purpose      : Entry point to application
//		//
//		//------------------------------------------------------------------
//
//		 int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
//		{
//			char				*Arg;  
//			char				*CmdLine;
//			int					argc, i;
//			char				**argv; 
//			char				IconName[64];						
//
//			CmdLine = (char *)malloc(sizeof(char) * 256);
//			strcpy(CmdLine, lpCmdLine);
//
//			argc = 1;	
//			Arg = strtok(lpCmdLine, " ");	
//			while(Arg != NULL ) 
//			{
//				argc++;
//				Arg = strtok(NULL, " ");		
//			}
//			argv = (char **)malloc(sizeof(char *) * argc);
//
//			argv[0] = NULL;
//			argc = 1;
//			Arg = strtok(CmdLine, " ");	
//			while(Arg != NULL ) 
//			{		
//				argv[argc] = (char *)malloc(sizeof(char) * strlen(Arg) + 1);
//				strcpy(argv[argc], Arg);
//				argc++;
//				Arg = strtok(NULL, " ");
//			}
//			free(CmdLine);
//
//			if (hInst)
//			{
//				LoadString(hInst, 2, IconName, sizeof(IconName));
//			}
//        
//			// Initialise window class
//			if( !InitClass(hInst, IconName) ) 
//				return 1;
//
//			// Initialise window
//			if( !InitWindow(hInst) ) 
//				return 1;
//			
//			
//			/*
//			if (!SetTimer(glpGameSystem->hWndMain, TIMER_ONE_TICK, 30, NULL))
//			{        
//				glpGameSystem->vReleaseGame();
//				glpGameSystem->Release();
//				PostMessage(glpGameSystem->hWndMain, WM_CLOSE, 0L, 0L);
//				return 1;
//			} 
//			*/
//			
//			main(argc, argv);	
//
//			for (i=0; i<argc; i++)
//			{
//				if (argv[i])
//					free(argv[i]);
//			}
//			free(argv);
//
//			/*
//			if (lynxGetAllocMemSize() > 0)
//			{
//				lynxClearBadMemBlock();
//				MessageBeep(MB_ICONEXCLAMATION);    
//				MessageBox(NULL, "記憶體未完全釋放", "警告", MB_OK | MB_ICONERROR);
//			}
//			lynxClearBadFileBlock();									
//			*/
//			
//			// Exit WinMain and terminate the app....
//			return 1;
//			//return gMsg.wParam;
//		}
//	#else
//	#endif
//#elif __NUCLEUS__
//	#ifdef __SIS_MOD__				
//		/* Nucleus Serial Driver interface */
//		#ifdef __LIB_IMAGE__
//			extern NU_MEMORY_POOL			System_Memory;			
//		#else
//			NU_MEMORY_POOL					System_Memory;
//			static NU_TASK					gTask0;
//			#define TASK_STACK_SIZE			10000	
//		#endif
//			
//		extern void					main(int argc, char **argv);		
//		//extern void					lynxMain(int argc, char **argv);		
//		
//		#ifndef __LIB_IMAGE__
//			void MainTask(UNSIGNED argc, VOID* argv)
//			{
//				#ifdef SERIAL
//					mstd_print_init();					
//					mstd_printf("Init serial port\n");										
//				#endif
//					
//				mstd_printf("mmpInitMMP ");
//		    	//mmpInitMMP(0x8000000, 0x800000, 0x50a);//for evb
//		    	mmpInitMMP(MMP_BASE_ADDRESS, MMP_VRAM_ADDRESS_OFFSET, MMP_PLL_NUMERATOR);
//		    	mstd_printf("OK\n");
//		    	
//		    	mmpSetMemoryPool(&System_Memory);
//				//mmpInitializeMemory(0x200000, 0x10000);    	
//				mmpInitializeMemory(MMP_TOTAL_HEAP_SIZE, MMP_MICROP_HEAP_SIZE);		    	
//				mmpGraphicsInitialize();			
//				mstd_printf("mmpGraphicsInitialize OK\n");				
//				mstd_printf("InitialLCDPanel ");
//		    	InitialLCDPanel();				
//				mstd_printf("OK\n");
//				mmpGetScreenDisplay((MMP_UINT32*)&gWindow);						
//				main(argc, argv);						
//				//lynxMain(argc, argv);						
//			}
//
//			void Application_Initialize(void* first_available_memory)
//			{
//				VOID*					Mem;
//				STATUS					Status;
//
//				Status = NU_Create_Memory_Pool(&System_Memory, "SYSMEM", first_available_memory, 24000000/*30000000*/, 50, NU_FIFO);
//
//				if (Status != NU_SUCCESS)
//					ERC_System_Error(Status);
//
//				NU_Allocate_Memory(&System_Memory, &Mem, TASK_STACK_SIZE, NU_NO_SUSPEND);
//
//				Status = NU_Create_Task(&gTask0, "TASK 0", MainTask, 0, NU_NULL, Mem, TASK_STACK_SIZE, 1, 20, NU_PREEMPT, NU_START);
//
//				if (Status != NU_SUCCESS)
//					ERC_System_Error(Status);		
//					
//				mstd_init_alloc_nucleus(&System_Memory, NU_NULL);
//			}
//		#endif
//	#endif	
//#endif
//
