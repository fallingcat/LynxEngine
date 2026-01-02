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
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

#include <LynxOS.h>
#include <LynxIO.h>
#include <LynxMem.h>
#ifdef __WIN32__
    #include <crtdbg.h>
#endif

#ifdef __iOS__
    #define LYNX_IOS_PERFORMANCE_FREQUENCY 65536.0f
#endif

#if defined __ANDROID__
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxOSThreadExitHandler(int sig)
 {
	 pthread_exit(0);
 }
#endif
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitOS()
{
	#if defined __ANDROID__
		struct sigaction Actions;
		memset(&Actions, 0, sizeof(Actions)); 
		sigemptyset(&Actions.sa_mask);
		Actions.sa_flags = 0; 
		Actions.sa_handler = lynxOSThreadExitHandler;
		sigaction(SIGUSR1, &Actions, NULL);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSGetTime(LPLYNXTIMEDATA lpt)
{
	#ifdef __WIN32__
		SYSTEMTIME					Time;

		GetLocalTime(&Time);
		lpt->Year = Time.wYear;  
		lpt->Month = Time.wMonth;  
		lpt->DayOfWeek = Time.wDayOfWeek;  
        lpt->Day = Time.wDay;  
		lpt->Hour = Time.wHour;  
		lpt->Minute = Time.wMinute;  
		lpt->Second = Time.wSecond;  
		lpt->Milliseconds = Time.wMilliseconds;
    #else
        time_t rawtime;
        struct tm *timeinfo;
    
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        
        lpt->Year = timeinfo->tm_year + 1900;  
        lpt->Month = timeinfo->tm_mon + 1;  
        lpt->DayOfWeek = timeinfo->tm_wday;  
        lpt->Day = timeinfo->tm_mday;  
        lpt->Hour = timeinfo->tm_hour;  
        lpt->Minute = timeinfo->tm_min;  
        lpt->Second = timeinfo->tm_sec;  
        lpt->Milliseconds = 0;
	#endif
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXULONGLONG lynxOSGetTimeStamp()
{
	time_t rawtime;
    struct tm *timeinfo;
    
    time ( &rawtime );

	return rawtime;    
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
double lynxOSGetTimeDifference(LPLYNXTIMEDATA lpt0, LPLYNXTIMEDATA lpt1) 
{	
	struct tm t0, t1;
	time_t tt0, tt1;
	double Diff;

	t0.tm_year = lpt0->Year - 1900;
	t0.tm_mon	= lpt0->Month - 1;
	t0.tm_mday = lpt0->Day;
	t0.tm_hour = lpt0->Hour;
	t0.tm_min	= lpt0->Minute;
	t0.tm_sec	= lpt0->Second;
    t0.tm_isdst = -1;

	t1.tm_year = lpt1->Year - 1900;
	t1.tm_mon	= lpt1->Month - 1;
	t1.tm_mday = lpt1->Day;
	t1.tm_hour = lpt1->Hour;
	t1.tm_min	= lpt1->Minute;
	t1.tm_sec	= lpt1->Second;
    t1.tm_isdst = -1;
    
	tt0 = mktime(&t0);
	tt1 = mktime(&t1);

	Diff = difftime (tt1, tt0);

	return Diff;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONGLONG lynxOSGetPerformanceCounter()
{
	#ifdef __WIN32__
		LARGE_INTEGER c;
        QueryPerformanceCounter(&c);
		return (LYNXLONGLONG)c.QuadPart;
	#elif defined __iOS__
		CFTimeInterval  Time;
		Time = CFAbsoluteTimeGetCurrent();
		return (LYNXLONGLONG)(Time*LYNX_IOS_PERFORMANCE_FREQUENCY);
	#elif defined __ANDROID__
		/*time_t Time;
		time(&Time);
		return (LYNXLONGLONG)(Time*CLOCKS_PER_SEC);*/
		struct timespec Time;
		clock_gettime(CLOCK_REALTIME, &Time);
		return (LYNXLONGLONG)((Time.tv_sec + (float)Time.tv_nsec / 1e9) * CLOCKS_PER_SEC);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONGLONG lynxOSGetPerformanceFrequency()
{
	#ifdef __WIN32__
		LARGE_INTEGER c;
        QueryPerformanceFrequency(&c);
		return (LYNXLONGLONG)c.QuadPart;
	#elif defined __iOS__
		return (LYNXLONGLONG)(LYNX_IOS_PERFORMANCE_FREQUENCY);
	#elif defined __ANDROID__
		return (LYNXLONGLONG)(CLOCKS_PER_SEC);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSGetOSInfo()
{
	/*
		OSVERSIONINFOEX osvi;
		BOOL bOsVersionInfoEx;

		// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
		// If that fails, try using the OSVERSIONINFO structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
				return FALSE;
		}

		switch (osvi.dwPlatformId)
		{
			// Test for the Windows NT product family.
			case VER_PLATFORM_WIN32_NT:

				// Test for the specific product family.
				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
					printf ("Microsoft Windows Server&nbsp;2003 family, ");

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
					printf ("Microsoft Windows XP ");

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
					printf ("Microsoft Windows 2000 ");

				if ( osvi.dwMajorVersion <= 4 )
					printf("Microsoft Windows NT ");

				// Test for specific product on Windows NT 4.0 SP6 and later.
				if( bOsVersionInfoEx )
				{
					// Test for the workstation type.
					if ( osvi.wProductType == VER_NT_WORKSTATION )
					{
					if( osvi.dwMajorVersion == 4 )
						printf ( "Workstation 4.0 " );
					else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
						printf ( "Home Edition " );
					else
						printf ( "Professional " );
					}
		            
					// Test for the server type.
					else if ( osvi.wProductType == VER_NT_SERVER )
					{
					if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							printf ( "Datacenter Edition " );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ( "Enterprise Edition " );
						else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
							printf ( "Web Edition " );
						else
							printf ( "Standard Edition " );
					}

					else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							printf ( "Datacenter Server " );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ( "Advanced Server " );
						else
							printf ( "Server " );
					}

					else  // Windows NT 4.0 
					{
						if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ("Server 4.0, Enterprise Edition " );
						else
							printf ( "Server 4.0 " );
					}
					}
				}
				else  // Test for specific product on Windows NT 4.0 SP5 and earlier
				{
					HKEY hKey;
					char szProductType[BUFSIZE];
					DWORD dwBufLen=BUFSIZE;
					LONG lRet;

					lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
					0, KEY_QUERY_VALUE, &hKey );
					if( lRet != ERROR_SUCCESS )
					return FALSE;

					lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
					(LPBYTE) szProductType, &dwBufLen);
					if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
					return FALSE;

					RegCloseKey( hKey );

					if ( lstrcmpi( "WINNT", szProductType) == 0 )
					printf( "Workstation " );
					if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
					printf( "Server " );
					if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
					printf( "Advanced Server " );

					printf( "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
				}

			// Display service pack (if any) and build number.

				if( osvi.dwMajorVersion == 4 && 
					lstrcmpi( osvi.szCSDVersion, "Service Pack 6" ) == 0 )
				{
					HKEY hKey;
					LONG lRet;

					// Test for SP6 versus SP6a.
					lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
					0, KEY_QUERY_VALUE, &hKey );
					if( lRet == ERROR_SUCCESS )
					printf( "Service Pack 6a (Build %d)\n", osvi.dwBuildNumber & 0xFFFF );         
					else // Windows NT 4.0 prior to SP6a
					{
					printf( "%s (Build %d)\n",
						osvi.szCSDVersion,
						osvi.dwBuildNumber & 0xFFFF);
					}

					RegCloseKey( hKey );
				}
				else // Windows NT 3.51 and earlier or Windows 2000 and later
				{
					printf( "%s (Build %d)\n",
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);
				}


				break;

			// Test for the Windows 95 product family.
			case VER_PLATFORM_WIN32_WINDOWS:

				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				{
					printf ("Microsoft Windows 95 ");
					if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
						printf("OSR2 " );
				} 

				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				{
					printf ("Microsoft Windows 98 ");
					if ( osvi.szCSDVersion[1] == 'A' )
						printf("SE " );
				} 

				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				{
					printf ("Microsoft Windows Millennium Edition\n");
				} 
				break;

			case VER_PLATFORM_WIN32s:

				printf ("Microsoft Win32s\n");
				break;
		}
	*/

	/*
		SYSTEM_INFO siSysInfo;
	 
		// Copy the hardware information to the SYSTEM_INFO structure. 
		 
		GetSystemInfo(&siSysInfo); 
		 
		// Display the contents of the SYSTEM_INFO structure. 

		printf("Hardware information: \n");  
		printf("  OEM ID: %u\n", siSysInfo.dwOemId);
		printf("  Number of processors: %u\n", 
			siSysInfo.dwNumberOfProcessors); 
		printf("  Page size: %u\n", siSysInfo.dwPageSize); 
		printf("  Processor type: %u\n", siSysInfo.dwProcessorType); 
		printf("  Minimum application address: %lx\n", 
			siSysInfo.lpMinimumApplicationAddress); 
		printf("  Maximum application address: %lx\n", 
			siSysInfo.lpMaximumApplicationAddress); 
		printf("  Active processor mask: %u\n", 
			siSysInfo.dwActiveProcessorMask); 
	*/

	/*
		lpszSystemInfo = tchBuffer; 
	 
		// Get and display the name of the computer. 

		if( GetComputerName(lpszSystemInfo, &cchBuff) ) 
			printf("Computer name:  %s\n", lpszSystemInfo); 

		// Get and display the user name. 

		cchBuff = BUFSIZE; 
		if( GetUserName(lpszSystemInfo, &cchBuff) )
			printf("User name:  %s\n\n", lpszSystemInfo
	*/

}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXTHREADHANDLE	lynxOSCreateThread(LPTHREADPROC lpproc, LPVOID lpdata)
{	
	LYNXTHREADHANDLE					Handle;	

	#ifdef __WIN32__
        Handle = NULL;
		#ifdef __WINAPP__
		#elif _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
			Handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lpproc, lpdata, 0, NULL);        
			SetThreadPriority(Handle, THREAD_PRIORITY_HIGHEST);
		#elif defined _XBOX		
			Handle = CreateThread(NULL, (64<<10), (LPTHREAD_START_ROUTINE)lpproc, lpdata, 0, NULL);        
		#endif
	#elif defined __iOS__ || defined __ANDROID__
		pthread_attr_t  Attr;
		int             ReturnVal;
		int				ThreadError;
	 
        Handle = NULL;
		ReturnVal = pthread_attr_init(&Attr);
		LYNX_ASSERT(!ReturnVal);
		ReturnVal = pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
		LYNX_ASSERT(!ReturnVal);
	 
		ThreadError = pthread_create(&Handle, &Attr, lpproc, lpdata);	 

		ReturnVal = pthread_attr_destroy(&Attr);
		LYNX_ASSERT(!ReturnVal);
	#else
	#endif

	return Handle;
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSSetThreadPriority(LYNXTHREADHANDLE handle, THREADPRIORITY pro)
{	
	#ifdef __WIN32__        
		#ifdef __WINAPP__
		#elif _WINDOWS || _CONSOLE || (_LIB && !_XBOX)			
			switch (pro)
			{
				case TP_LOWEST:
					SetThreadPriority(handle, THREAD_PRIORITY_LOWEST);		
					break;
				case TP_NORMAL:
					SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);		
					break;
				case TP_HIGHEST:
					SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);							
					break;
				case TP_TIME_CRITICAL:					
					SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);		
					break;
			}
		#endif
	#elif defined __iOS__ || defined __ANDROID__    
        struct sched_param sched;    

		switch (pro)
		{
			case TP_LOWEST:
				sched.sched_priority = 1;
				break;
			case TP_NORMAL:
				sched.sched_priority = 50;
				break;
			case TP_HIGHEST:
				sched.sched_priority = 90;
				break;
			case TP_TIME_CRITICAL:					
				sched.sched_priority = 98;
				break;
		}        
        // We use RR to act as the default scheduler
        pthread_setschedparam(handle, SCHED_RR, &sched);
	#else
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSExitThread(DWORD exitcode)
{	
	#ifdef __WIN32__
		#ifdef __WINAPP__
		#elif _WINDOWS || _CONSOLE || (_LIB && !_XBOX)			
			ExitThread(exitcode);        
		#elif defined _XBOX					
			ExitThread(exitcode);        
		#endif
	#elif defined __iOS__ || defined __ANDROID__
        pthread_exit(NULL);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSCloseThread(LYNXTHREADHANDLE th)
{	
	#ifdef __WIN32__
		#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
			CloseHandle(th); 
		#elif defined _XBOX					
			CloseHandle(th);
		#endif
	#elif defined __iOS__// || defined __ANDROID__
        pthread_cancel(th);
	#elif defined __ANDROID__
		pthread_kill(th, SIGUSR1);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSCreateMutex(LYNXMUTEX* m)
{	
	#ifdef __WIN32__
		*m = CreateMutex(NULL,              // default security attributes
						FALSE,             // initially not owned
						NULL);             // unnamed mutex		
	#elif defined __iOS__ || defined __ANDROID__
        //pthread_mutexattr_t Attr;
        //pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_ERRORCHECK);
        pthread_mutex_init(m, NULL);//&Attr);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSLockMutex(LYNXMUTEX* m)
{	
	#ifdef __WIN32__		
		WaitForSingleObject(*m, INFINITE);
	#elif defined __iOS__ || defined __ANDROID__
		pthread_mutex_lock(m);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSUnlockMutex(LYNXMUTEX* m)
{	
	#ifdef __WIN32__		
		ReleaseMutex(*m);
	#elif defined __iOS__ || defined __ANDROID__
		pthread_mutex_unlock(m);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSDestoryMutex(LYNXMUTEX* m)
{	
	#ifdef __WIN32__		
		CloseHandle(*m);
	#elif defined __iOS__ || defined __ANDROID__
		pthread_mutex_destroy(m);
	#endif	
}

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSInitCriticalSection(LPLYNXCRITICALSECTION lpcs)
{	
	#ifdef __WIN32__
		//InitializeCriticalSection(&lpcs->CriticalSection);
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSReleaseCriticalSection(LPLYNXCRITICALSECTION lpcs)
{	
	#ifdef __WIN32__
		//DeleteCriticalSection(&lpcs->CriticalSection);
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSEnterCriticalSection(LPLYNXCRITICALSECTION lpcs)
{	
	#ifdef __WIN32__
		//EnterCriticalSection(&lpcs->CriticalSection);
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSLeaveCriticalSection(LPLYNXCRITICALSECTION lpcs)
{	
	#ifdef __WIN32__
		//LeaveCriticalSection(&lpcs->CriticalSection);
	#elif defined __XWIN__
	#endif	
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSMoveWindow(const LYNXWINDOW w, const int x, const int y, const int width, const int height, LYNXBOOL bclientsize)
{
	int Width, Height;

	#ifdef __WIN32__		
		#ifndef __WINAPP__
			if (bclientsize)
			{
				Height = height + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYEDGE)*2 + GetSystemMetrics(SM_CYFRAME)*2;
				Width = width + GetSystemMetrics(SM_CXEDGE)*2 + GetSystemMetrics(SM_CYFRAME)*2;
				MoveWindow(w, x, y, Width, Height, FALSE);		
			}
			else
				MoveWindow(w, x, y, width, height, FALSE);
		#endif
	#elif defined __XWIN__			
		return;	
	#else
		return;
	#endif
}

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSRegisterTouchWindow(const LYNXWINDOW w, const LYNXBOOL b)
{
	int Width, Height;

	#ifdef __WIN32__		
		#ifndef __WINAPP__
			if (b)
			{
				RegisterTouchWindow(w, 0);
			}
			else
			{
				UnregisterTouchWindow(w);			
			}
		#endif
	#elif defined __XWIN__			
		return;	
	#else
		return;
	#endif
}


//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSSetTitle(const LYNXWINDOW w, const LYNXCHAR *title)
{
	#ifdef __WIN32__
		#ifndef __WINAPP__
			SetWindowText(w, title);
		#endif		
	#elif defined __WINCE__
		SetWindowText(w, (unsigned short*)title);		
	#elif defined __XWIN__	
		XSetStandardProperties(lynxGetlpRenderer()->SystemData.Disp, lynxGetlpRenderer()->lpSystemData->RenderWin,
							     title, title, None, NULL, 0, NULL);
    #elif defined __OS_X__
        SetWindowTitleWithCFString(w, CFStringCreateWithCString(NULL, title, 0));
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxOSGetTitle(const LYNXWINDOW w)
{
	static LYNXCHAR	Buffer[256];

	#ifdef __WIN32__
		#ifndef __WINAPP__
			GetWindowText(w, Buffer, 255);		
			return Buffer;
		#else
			return NULL;
		#endif
	#elif defined __XWIN__			
		return NULL;	
	#else
		return NULL;
	#endif
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCOREAPI void lynxOSSetIcon(DWORD icon)
{

}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSSleep(DWORD t)
{
	#ifdef __WIN32__
		#ifdef __WINAPP__
		#else
			Sleep(t);
		#endif
	#elif defined __NUCLEUS__		
		NU_Sleep(t);
	#elif defined __LINUX__			
		//sleep(t);	
	#elif defined __iOS__			
		usleep(t*1000);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxOSClose(const LYNXWINDOW w)
{
	#ifdef __WIN32__
		#ifndef __WINAPP__
			SendMessage(w, WM_CLOSE, 0, 0);	
		#endif		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
HMODULE	lynxOSLoadLibrary(const LYNXCHAR *name)
{
	#ifdef __WIN32__
		#if (!defined(_XBOX) && !defined(__WINAPP__))
			return LoadLibrary(name);
		#else
			return NULL;
		#endif		
	#elif defined __LINUX__			
		return dlopen(name, 0);
	#else
		return NULL;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
FARPROC	lynxOSGetProc(HMODULE hm, const char *name)
{
	#ifdef __WIN32__
		#if (!defined(_XBOX))
			return GetProcAddress(hm, name);	
		#else
			return NULL;
		#endif
	#elif defined __LINUX__
		return dlsym(hm, name);
	#else
		return NULL;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSMakeGUID(LPLYNXGUID pguid)
{
	#ifdef __WIN32__
		CoCreateGuid((GUID*)pguid);
    #else
        new_uuid_t UUID;
        uuid_create(&UUID);
        lynxMemCpy(pguid, &UUID, sizeof(LYNXGUID));
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
const LYNXCHAR* lynxOSGUIDToString(const LYNXGUID* pguid)
{
	static LYNXCHAR String[64];

	lynxSprintf(String, _T("%08x-%04hx-%04hx-%02hx%02hx-%02hx%02hx%02hx%02hx%02hx%02hx"), pguid->Data1, pguid->Data2, pguid->Data3, pguid->Data4[0], pguid->Data4[1], pguid->Data4[2], pguid->Data4[3], pguid->Data4[4], pguid->Data4[5], pguid->Data4[6], pguid->Data4[7]);

	return String;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
const LYNXGUID lynxOSStringToGUID(const LYNXCHAR* str)
{
	static LYNXGUID Guid;
	LYNXCHAR Part[128];

	lynxStrCpy(Part, &str[0]);
	Part[8] = 0x00;
	Guid.Data1 = lynxXtoi(Part);

	lynxStrCpy(Part, &str[9]);
	Part[4] = 0x00;
	Guid.Data2 = lynxXtoi(Part);

	lynxStrCpy(Part, &str[14]);
	Part[4] = 0x00;
	Guid.Data3 = lynxXtoi(Part);

	lynxStrCpy(Part, &str[19]);
	Part[2] = 0x00;
	Guid.Data4[0] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[21]);
	Part[2] = 0x00;
	Guid.Data4[1] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[24]);
	Part[2] = 0x00;
	Guid.Data4[2] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[26]);
	Part[2] = 0x00;
	Guid.Data4[3] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[28]);
	Part[2] = 0x00;
	Guid.Data4[4] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[30]);
	Part[2] = 0x00;
	Guid.Data4[5] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[32]);
	Part[2] = 0x00;
	Guid.Data4[6] = lynxXtoi(Part);

	lynxStrCpy(Part, &str[34]);
	Part[2] = 0x00;
	Guid.Data4[7] = lynxXtoi(Part);	

	return Guid;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxOSComapreGUID(const LYNXGUID* pguid1, const LYNXGUID* pguid2)
{
	#if defined(__WIN32__) && !defined(__WINAPP__)
		RPC_STATUS Status;
		return UuidCompare((GUID*)pguid1, (GUID*)pguid2, &Status);	
	#else
		int i;

		if (pguid1->Data1 <  pguid2->Data1)
			return -1;
		else if (pguid1->Data1 >  pguid2->Data1)
			return 1;
		else
		{
			if (pguid1->Data2 <  pguid2->Data2)
				return -1;
			else if (pguid1->Data2 >  pguid2->Data2)
				return 1;
			else
			{
				if (pguid1->Data3 <  pguid2->Data3)
					return -1;
				else if (pguid1->Data3 >  pguid2->Data3)
					return 1;
				else
				{
					for (i=0; i<8; i++)
					{
						if (pguid1->Data4[i] <  pguid2->Data4[i])
							return -1;
						else if (pguid1->Data4[i] >  pguid2->Data4[i])
							return 1;
					}
						
				}
			}
		}
		return 0;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSGetCurrentWorkingDirectory(LYNXCHAR* dir, DWORD len)
{
	#ifdef __WIN32__
		#ifndef __WINAPP__
			GetCurrentDirectory(len , dir);
		#endif
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSSetCurrentWorkingDirectory(const LYNXCHAR* dir)
{
	#ifdef __WIN32__
		#ifndef __WINAPP__
			SetCurrentDirectory(dir);
		#endif
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxOS 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxOSDebuggerBreak()
{
	#if defined(DEBUG) || defined(_DEBUG)
		#ifdef __WIN32__
			_CrtDbgBreak();
		#endif
	#endif
}

