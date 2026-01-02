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

#include <LynxCPU.h>
#include <LynxLog.h>

static WORD							gFPUControlWord;
LYNXCHAR							gCPUModuleName[] = _T("\tLynxCPU");
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//
//-------------------------------------------------------------------------------------------------------
int ilynxIsCPUID(void)
{
	#ifdef __WIN32__
		#if (defined _M_IX86)
			__try 
			{
				_asm
				{
					xor eax, eax
					cpuid
				}
			} 
			__except ( EXCEPTION_EXECUTE_HANDLER) 
			{
				return 0;
			}
			return 1;			
		#else
			return 0;			
		#endif
	#else
		return 0;
	#endif

}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//		int _os_support(int feature)
//			- Checks if OS Supports the capablity or not
//
//		Entry:
//			feature: the feature we want to check if OS supports it.
//
//		Exit:
//			Returns 1 if OS support exist and 0 when OS doesn't support it.
//-------------------------------------------------------------------------------------------------------
int ilynxOSSupport(int feature)
{
	#ifdef __WIN32__
		#if (defined _M_IX86)
			__try
			{
				switch (feature)
				{
					case LYNX_CPU_FEATURE_SSE:
						__asm 
						{
							xorps xmm0, xmm0        // __asm _emit 0x0f __asm _emit 0x57 __asm _emit 0xc0
												// executing SSE instruction
						}
						break;

					case LYNX_CPU_FEATURE_SSE2:
						__asm 
						{
							__asm _emit 0x66 __asm _emit 0x0f __asm _emit 0x57 __asm _emit 0xc0
												// xorpd xmm0, xmm0
												// executing WNI instruction
						}
						break;

					case LYNX_CPU_FEATURE_3DNOW:
						__asm 
						{
							__asm _emit 0x0f __asm _emit 0x0f __asm _emit 0xc0 __asm _emit 0x96 
												// pfrcp mm0, mm0
												// executing 3Dnow instruction
						}
						break;

					case LYNX_CPU_FEATURE_MMX:
						__asm 
						{
							pxor mm0, mm0           // executing MMX instruction
						}
						break;
				}
			} 
			__except (EXCEPTION_EXECUTE_HANDLER) 
			{
				if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) 
				{
					return (0);
				}
				return (0);
			}
			return 1;
		#else
			return 0;
		#endif
	#else
		return 0;
	#endif	
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//		void map_mname(int, int, const char *, char *) maps family and model to processor name
//-------------------------------------------------------------------------------------------------------
void ilynxMapMname( int family, int model, const char * v_name, char *m_name)
{
    if (!strncmp("AuthenticAMD", v_name, 12))
    {
        switch (family) // extract family code
        {
			case 4: // Am486/AM5x86
				strcpy (m_name,"AMD Am486");
				break;

			case 5: // K6
				switch (model) // extract model code
				{
					case 0:
					case 1:
					case 2:
					case 3: 
						strcpy (m_name,"AMD K5");
						break;

					case 4:
						break;  // Not really used

					case 5:
						break;  // Not really used

					case 6:
					case 7: 
						strcpy (m_name,"AMD K6");
						break;

					case 8:
						strcpy (m_name,"AMD K6-2");
						break;

					case 9: 
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
						strcpy (m_name,"AMD K6-3");
						break;

					default:
						strcpy (m_name, "Unknown");

				}
				break;

			case 6: // Athlon
				switch(model)  // No model numbers are currently defined
				{
					case 1:
					default:
						strcpy (m_name,"AMD ATHLON");
				}
				break;
        }
    } 
	else if ( !strncmp("GenuineIntel", v_name, 12))
    {
        switch (family) // extract family code
        {
			case 4:
				switch (model) // extract model code
				{
					case 0:
					case 1:
						strcpy (m_name,"INTEL 486DX");
						break;
					case 2:
						strcpy (m_name,"INTEL 486SX");
						break;
					case 3:
						strcpy (m_name,"INTEL 486DX2");
						break;
					case 4:
						strcpy (m_name,"INTEL 486SL");
						break;
					case 5:
						strcpy (m_name,"INTEL 486SX2");
						break;
					case 7:
						strcpy (m_name,"INTEL 486DX2E");
						break;
					case 8:
						strcpy (m_name,"INTEL 486DX4");
						break;
					default:
						strcpy (m_name, "Unknown");
				}
				break;
			case 5:
				switch (model) // extract model code
				{

					case 1:
					case 2:
					case 3:
						strcpy (m_name,"INTEL Pentium");
						break;
					case 4:
						strcpy (m_name,"INTEL Pentium-MMX");
						break;
					default:
						strcpy (m_name, "Unknown");
				}
				break;
			case 6:
				switch (model) // extract model code
				{
					case 1:
						strcpy (m_name,"INTEL Pentium-Pro");
						break;
					case 3:
						strcpy (m_name,"INTEL Pentium-II"); 
						break;
					case 5:
						strcpy (m_name,"INTEL Pentium-II");
						break;  // actual differentiation depends on cache settings
					case 6:
						strcpy (m_name,"INTEL Celeron");
						break;
					case 7:
						strcpy (m_name,"INTEL Pentium-III");
						break;  // actual differentiation depends on cache settings
					default:
						strcpy (m_name, "Unknown");
				}
				break;

			default:
				strcpy (m_name, "Unknown");
				break;
        }
    } 
	else if ( !strncmp("CyrixInstead", v_name,12))
    {
        strcpy (m_name,"Unknown");
    } 
	else if ( !strncmp("CentaurHauls", v_name,12))
    {
        strcpy (m_name,"Unknown");
    } 
	else 
    {
        strcpy (m_name, "Unknown");
    }
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCPU 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//
//  說明:   
//		int _cpuid (_p_info *pinfo)
// 
//		Entry:
//			pinfo: pointer to _p_info.
//
//		Exit:
//			Returns int with capablity bit set even if pinfo = NULL
//
//-------------------------------------------------------------------------------------------------------
 void	lynxGetCPUInfo(LPLYNXCPUINFO lpinfo)
{
    DWORD dwStandard = 0;
    DWORD dwFeature = 0;
    DWORD dwMax = 0;
    DWORD dwExt = 0;
    int feature = 0, os_support = 0;
    union
    {
        char cBuf[12+1];
        struct tag
        {
            DWORD dw0;
            DWORD dw1;
            DWORD dw2;
        };
    } Ident;

    if (!ilynxIsCPUID())
    {
        return;
    }

	#ifdef __WIN32__
		#if (defined _M_IX86)
			_asm
			{
				push ebx
				push ecx
				push edx

				// get the vendor string
				xor eax,eax
				cpuid
				mov dwMax,eax
				mov Ident.dw0,ebx
				mov Ident.dw1,edx
				mov Ident.dw2,ecx

				// get the Standard bits
				mov eax,1
				cpuid
				mov dwStandard,eax
				mov dwFeature,edx

				// get AMD-specials
				mov eax,80000000h
				cpuid
				cmp eax,80000000h
				jc notamd
				mov eax,80000001h
				cpuid
				mov dwExt,edx

		notamd:
				pop ecx
				pop ebx
				pop edx
			}
		#endif

    if (dwFeature & LYNX_MMX_FEATURE_BIT)
    {
        feature |= LYNX_CPU_FEATURE_MMX;
        if (ilynxOSSupport(LYNX_CPU_FEATURE_MMX))
            os_support |= LYNX_CPU_FEATURE_MMX;
    }
    if (dwExt & LYNX_3DNOW_FEATURE_BIT)
    {
        feature |= LYNX_CPU_FEATURE_3DNOW;
        if (ilynxOSSupport(LYNX_CPU_FEATURE_3DNOW))
            os_support |= LYNX_CPU_FEATURE_3DNOW;
    }
    if (dwFeature & LYNX_SSE_FEATURE_BIT)
    {
        feature |= LYNX_CPU_FEATURE_SSE;
        if (ilynxOSSupport(LYNX_CPU_FEATURE_SSE))
            os_support |= LYNX_CPU_FEATURE_SSE;
    }
    if (dwFeature & LYNX_SSE2_FEATURE_BIT)
    {
        feature |= LYNX_CPU_FEATURE_SSE2;
        if (ilynxOSSupport(LYNX_CPU_FEATURE_SSE2))
            os_support |= LYNX_CPU_FEATURE_SSE2;
    }    
	if (lpinfo)
    {
        memset(lpinfo, 0, sizeof(LYNXCPUINFO));
        lpinfo->OSSupport = os_support;
        lpinfo->Feature = feature;
        lpinfo->Family = (dwStandard >> 8)&0xF;  // retriving family
        lpinfo->Model = (dwStandard >> 4)&0xF;   // retriving model
        lpinfo->Stepping = (dwStandard) & 0xF;   // retriving stepping
        Ident.cBuf[12] = 0;
        strcpy(lpinfo->VendorName, Ident.cBuf);
        ilynxMapMname(lpinfo->Family, lpinfo->Model, lpinfo->VendorName, lpinfo->ModelName);
        lpinfo->Checks = LYNX_CPU_FEATURE_MMX |
                         LYNX_CPU_FEATURE_SSE |
                         LYNX_CPU_FEATURE_SSE2 |
                         LYNX_CPU_FEATURE_3DNOW;

		lynxSetLogf(gCPUModuleName, _T("CPU Vendor : %s\n"), lpinfo->VendorName);
		lynxSetLogf(gCPUModuleName, _T("CPU Model Name : %s\n"), lpinfo->ModelName);
    }
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCPU 
 *  @brief 。
 *
 *	This function evaluates whether the floating-point control word is set to single precision/round to nearest/
 *	exceptions disabled. If these conditions don't hold, the function changes the control word to set them and returns
 *	TRUE, putting the old control word value in the passback location pointed to by pwOldCW.
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//  說明:   
//		This function evaluates whether the floating-point control word is set to single precision/round to nearest/
//		exceptions disabled. If these conditions don't hold, the function changes the control word to set them and returns
//		TRUE, putting the old control word value in the passback location pointed to by pwOldCW.
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxMungeFPCW(void)
{    
	LYNXBOOL			Ret = FALSE;    
	WORD				Temp, Save = 0; 

	#ifdef __WIN32__
		#if (defined _M_IX86)
			__asm fstcw Save   
			if (Save & 0x300 ||            // Not single mode
				0x3f != (Save & 0x3f) ||   // Exceptions enabled
				Save & 0xC00)              // Not round to nearest mode    
			{
				__asm        
				{            
					mov ax, Save
					and ax, not 300h    // single mode
					or  ax, 3fh         // disable all exceptions
					and ax, not 0xC00   // round to nearest mode
					mov Temp, ax            
					fldcw   Temp        
				}        
				Ret = TRUE;
			}
		#endif
	#endif
	gFPUControlWord = Save;    

	return Ret;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCPU 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRestoreFPCW(void)
{
	#ifdef __WIN32__
		#if (defined _M_IX86)
			__asm fldcw gFPUControlWord;
		#endif
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCPU 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXINT64	lynxCPUClock(void)
{
	#ifdef __PS2__
		u_long				Clock;
		LYNXINT64			Ret;

		Clock = GetTimerSystemTime();		
		Ret = Clock;

		return Ret;
	#elif defined __WIN32__		
		#if (defined _M_IX86)
			__int64				Time;	
			_asm{
					RDTSC
					mov DWORD PTR Time, eax
					mov DWORD PTR Time+4, edx
				}
			return Time;		
		#endif
	#endif
	return 0;
}