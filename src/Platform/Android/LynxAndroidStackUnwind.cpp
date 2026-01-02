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
#include <Platform/Android/LynxAndroidStackUnwind.h>
#include <android/log.h>

#define LOG_TAG    "LynxEngine"
#define ANDROID_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

// header for linux kernel
#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>

namespace LynxEngine
{
	namespace Android
	{		
		extern "C"
		{			
			struct android_backtrace_state
			{
				void **current;
				void **end;
			};
						
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			_Unwind_Reason_Code android_unwind_callback(struct _Unwind_Context* context, void* arg)
			{
				android_backtrace_state* state = (android_backtrace_state *)arg;
				_Unwind_Ptr pc = _Unwind_GetIP(context);
				if (pc)
				{
					if (state->current == state->end)
					{
						return _URC_END_OF_STACK;
					}
					else
					{
						*state->current++ = reinterpret_cast<void*>(pc);
					}
				}
				return _URC_NO_REASON;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CAndroidStackUnwind::DefaultLogCatOutput(const LYNXCHAR* logStr)
			{
				ANDROID_LOGE(logStr);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//
			//-------------------------------------------------------------------------------------------------------
			void CAndroidStackUnwind::DumpStack(OUTPUT_DELEGATE out_delegate)
			{
				out_delegate("Begin of android stack dump.");

				LYNXCHAR Buf[128];
				const int BUFFER_SIZE = 256;				
				void* Buffer[BUFFER_SIZE];

				android_backtrace_state state;
				state.current = Buffer;
				state.end = Buffer + BUFFER_SIZE;

				_Unwind_Backtrace(android_unwind_callback, &state);

				int Count = (int)(state.current - Buffer);

				for (int idx = 0; idx < Count; idx++)
				{
					const void* addr = Buffer[idx];
					const char* symbol = "";

					Dl_info info;
					if (dladdr(addr, &info) && info.dli_sname)
					{
						symbol = info.dli_sname;
					}
					int status = 0;
					char *demangled = __cxxabiv1::__cxa_demangle(symbol, 0, 0, &status);

					lynxSprintf(Buf, "%03d: 0x%p %s", idx, addr, (demangled && 0 == status) ? demangled : symbol);
					out_delegate(Buf);

					if (demangled) {
						free(demangled);
					}
				}

				out_delegate("End of android stack dump.");
			}
		}
	}
}
