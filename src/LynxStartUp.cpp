#include <LynxSDK.h>
#include <commctrl.h>
#include <GameFramework/LynxGameFrameworkKernel.h>

LYNXWINDOW			gWindow;
LPLYNXSDK			glpSDK;
HINSTANCE			ghInstance;	
char				gClassName[] = "Lynx Frame";
LYNXEXTERN int		main(int argc, char **argv);		

//------------------------------------------------------------------
// 
// Function     : WndProc()
//
// Purpose      : Windows procedure to handle messages
//
//------------------------------------------------------------------
LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (glpSDK)
	{
		glpSDK->KBD[VK_MENU] = (BYTE)GetAsyncKeyState(VK_MENU);  
		glpSDK->KBD[VK_LMENU] = (BYTE)GetAsyncKeyState(VK_LMENU);  
		glpSDK->KBD[VK_RMENU] = (BYTE)GetAsyncKeyState(VK_RMENU);  
	}
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
			if (glpSDK)
				glpSDK->CharPressed = wParam; 			
			return 0;

		case WM_KEYDOWN:
			if (glpSDK)
			{
				glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_DOWN;
				glpSDK->KeyPressed = (BYTE)wParam; 			
				glpSDK->KBD[(BYTE)wParam] = TRUE;
				//if (glpEvent->Key ==  VK_ESCAPE)
				//	glpSDK->bRunning = FALSE;
				//  DestroyWindow(hWnd);
			}
			return 0;

		case WM_KEYUP:
			if (glpSDK)
			{
				glpSDK->Event.ID = LYNX_SYS_EVENT_KEY_UP;
				//glpGameSystem->KeyPressed = -1;
				//glpGameSystem->CharPressed = -1;
				glpSDK->KBD[(BYTE)wParam] = FALSE;           
			}
			return 0;

		case WM_LBUTTONDOWN:
			if (glpSDK)
				glpSDK->KBD[VK_LBUTTON] = TRUE;           
			//glpGameSystem->MouseData.bLeftButtonDown = TRUE;
			return 0;

		case WM_LBUTTONUP:
			if (glpSDK)
				glpSDK->KBD[VK_LBUTTON] = FALSE;           
			//glpGameSystem->MouseData.bLeftButtonDown = FALSE;
			return 0;			

		case WM_RBUTTONDOWN:
			if (glpSDK)
				glpSDK->KBD[VK_RBUTTON] = TRUE;           
			//glpGameSystem->MouseData.bRightButtonDown = TRUE;
			return 0;

		case WM_RBUTTONUP:
			if (glpSDK)
				glpSDK->KBD[VK_RBUTTON] = FALSE;           
			//glpGameSystem->MouseData.bRightButtonDown = FALSE;
			return 0;
			
		case WM_MOUSEMOVE :
			//glpGameSystem->MouseData.CurrentPos.x = LOWORD(lParam);
			//glpGameSystem->MouseData.CurrentPos.y = HIWORD(lParam);
			return 0;
    
		case WM_SYSCOMMAND:	
			if (glpSDK)
			{
				if (glpSDK->bDisableMultiTask)		
					return 0;
					//glpGameSystem->KBD[(char)VK_MENU] = TRUE;
			}
			break;  

		//case WM_GRAPHNOTIFY:
			//HandleGraphEvent();
			//break;		
    
		case WM_MOVE :
			if (glpSDK)
			{
				glpSDK->Renderer.SystemData.Origin.x = (int) LOWORD(lParam);    // horizontal position 
				glpSDK->Renderer.SystemData.Origin.y = (int) HIWORD(lParam);    // vertical position 
			}
			return 0;
			
		case WM_CLOSE:         
			if (glpSDK)					
				glpSDK->bRunning = FALSE;
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
LYNXBOOL InitClass(HINSTANCE hInst, LPSTR lpIcon)
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

	// Create a window
	#ifdef __DIRECT3D__	
		gWindow =  CreateWindowEx(WS_EX_APPWINDOW,
									gClassName, 
									"Lynx Engine",
									WS_OVERLAPPEDWINDOW | WS_VISIBLE | 
		  							WS_POPUP | WS_SYSMENU, 
									0, 0,
									320, 200,
									NULL,
									NULL,
									hInst,
									NULL);
	#else		
		gWindow =  CreateWindowEx(WS_EX_APPWINDOW,
									gClassName, 
									"Lynx Engine",
									//WS_OVERLAPPEDWINDOW | WS_VISIBLE | 
		  							//WS_POPUP | WS_SYSMENU, 
									WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
									0, 0,
									320, 200,
									NULL,
									NULL,
									hInst,
									NULL);
	#endif

	// Return false if window creation failed
	if( !gWindow ) return FALSE;

	// Show the window
	ShowWindow(gWindow, SW_SHOWNORMAL);

	// Update the window
	UpdateWindow(gWindow); 

	#ifdef __NUCLEUS__
		#ifdef __SIS_MOD__				
			mmpGetScreenDisplay((MMP_UINT32*)&gWindow);
		#endif
	#endif

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
	char				IconName[64];						
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

	//if (hInst)
	//{
	//	LoadString(hInst, 2, IconName, sizeof(IconName));
	//}			
//     
	// Initialise window class
	if( !InitClass(hInst, IconName) ) 
		return 1;

	// Initialise window
	if( !InitWindow(hInst) ) 
		return 1;

	// Initialise COM object
	if (FAILED(CoInitialize(NULL)))
		return 1;
	//
	///*
	//if (!SetTimer(glpGameSystem->hWndMain, TIMER_ONE_TICK, 30, NULL))
	//{        
	//	glpGameSystem->vReleaseGame();
	//	glpGameSystem->Release();
	//	PostMessage(glpGameSystem->hWndMain, WM_CLOSE, 0L, 0L);
	//	return 1;
	//} 
	//*/
	//
	main(argc, argv);				

	for (i=0; i<argc; i++)
	{
		if (argv[i])
			free(argv[i]);
	}
	free(argv);						

	////// Initialise window class
	//if( !InitClass(hInst, IconName) ) 
	//	return 1;

	////// Initialise window
	//if( !InitWindow(hInst) ) 
	//	return 1;

	////// Initialise COM object
	//if (FAILED(CoInitialize(NULL)))
	//	return 1;

	//main(nCmdShow, &lpCmdLine);
	
	// Exit WinMain and terminate the app....
	return 1;
	//return gMsg.wParam;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void lynxStartUp(LynxEngine::GameFramework::CKernel& kel)
{
	kel.SetWindow(gWindow);
	glpSDK = &kel.GetEngine().GetSDK();
}
