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
#include <Platform/Android/LynxAndroidCrashdump.h>
#include <NetworkSystem/LynxHttpFormDataRequest.h>

#include <client/linux/handler/exception_handler.h>
#include <client/linux/handler/minidump_descriptor.h>

// header for linux kernel
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace LynxEngine
{
	namespace Android
	{		
		extern "C"
		{
			bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded)
			{
				LYNXBOOL succeed = (succeeded) ? LYNX_TRUE : LYNX_FALSE;
				LYNXCHAR FileNameWithoutExt[128];
				lynxGetFilenameOnlyByPath(FileNameWithoutExt, descriptor.path());
				
				glpKernel->vCrashDump(succeed, FileNameWithoutExt + CString(".dmp"));

				// If an exception occurred and the callback returns true, Breakpad will
				// treat the exception as fully-handled, suppressing any other handlers from
				// being notified of the exception.
				return LYNX_FALSE;
			}
		}

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrashDumpSystem::CCrashDumpSystem()			
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrashDumpSystem::~CCrashDumpSystem(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCrashDumpSystem::vInit(const LYNXCHAR* dumpfilepath)
		{						
			LYNX_LOG(glpLogger, CString("[Crashdump] vInit...\r\n"));

			static google_breakpad::MinidumpDescriptor s_Descriptor(dumpfilepath);
			static google_breakpad::ExceptionHandler s_Handler(s_Descriptor, NULL, DumpCallback, NULL, LYNX_TRUE, -1);
						
			return LYNX_TRUE;
		}		

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCrashDumpSystem::ExecDumpLogcat(const LYNXCHAR* dumpfilepath, CString& dumpfilename)
		{
			LYNX_LOG(glpLogger, CString("[Crashdump] start to execute dump logcat ...\r\n"));
				
			// generate file name in datetime format.			
			time_t _t = time(NULL);
			struct tm _tm, *_ptm;
			_ptm = gmtime_r(&_t, &_tm);

				LYNXCHAR FormatDate[64];
				lynxSprintf(FormatDate, "%d-%02d-%02d-%02d:%02d:%02d", 
				_ptm->tm_year + 1900,
				_ptm->tm_mon + 1,
				_ptm->tm_mday,
				(_ptm->tm_hour + 8) % 24, // UTC+8
				_ptm->tm_min,
				_ptm->tm_sec);
			
			dumpfilename = CString(FormatDate) + CString(".log");

			pid_t pid = fork();			
			if (pid == 0)  // child process
			{
				LYNXCHAR Buffer[256];
				lynxSprintf(Buffer, "logcat | grep %d", getppid());				
				CString logcat_file = CString(dumpfilepath) + "/" + dumpfilename;

				LYNX_LOG(glpLogger, CString("[Crashdump] dump logcat file = ") + logcat_file + CString("\r\n"));

				execl("/system/bin/logcat",		
					Buffer,						// logcat command, and filter by PID.
					"-v", "threadtime",			// log format.
					"-d",						// output to screen.
					"-f", logcat_file.c_str(), // file dump location.
					NULL);

				_exit(EXIT_FAILURE);
			}

			if (pid < 0) 
			{
				LYNX_LOG(glpLogger, CString("Launching logcat has failed: ") + CString(strerror(errno)));
				return LYNX_FALSE;
			}

			int status;
			pid_t pid2 = waitpid(pid, &status, 0);
			if (pid2 < 0) 
			{
				return LYNX_FALSE;
			}

			if (WIFSIGNALED(status)) 
			{
				LYNXCHAR Buffer[64];
				lynxSprintf(Buffer, "logcat was killed by signal %d", WTERMSIG(status));
				LYNX_LOG(glpLogger, Buffer);
				return LYNX_FALSE;
			}

			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) 
			{
				LYNX_LOG(glpLogger, CString("logcat has failed."));
				return LYNX_FALSE;
			}

			LYNX_LOG(glpLogger, CString("execute logcat succeed."));

			return LYNX_TRUE;
		}
	}
}