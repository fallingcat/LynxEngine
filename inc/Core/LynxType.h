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
//  說明:
//
//      此檔案負責在不同API間隱藏中間介面及資料結構及運作方式,使得Lynx System
//		的使用者不會發覺到API的不同 
//
//###########################################################################

#ifndef __LYNXTYPE_H__
#define __LYNXTYPE_H__

#include <LynxConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>	

#define NOMINMAX

#ifdef WINAPI_FAMILY_APP
	#if (WINAPI_FAMILY==WINAPI_FAMILY_APP)
		#ifndef __WINAPP__
		#define __WINAPP__
		#endif
	#elif (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
		#ifndef __WPAPP__
		#define __WPAPP__
		#endif
	#endif
#endif

#if (defined(_DEBUG) || defined(DEBUG))					
	#if (defined (__WIN32__) && defined(__VC_MEM_LEAK_CHECK__))		
		#define _CRTDBG_MAP_ALLOC 
		#include <stdlib.h> 
		#include <crtdbg.h>			
	#endif
#endif

#ifdef __VLD__
	#include <vld.h> 
#endif

#ifdef __cplusplus
	#ifdef __MFC__	
		#include <afx.h>
		#include <afxwin.h>         // MFC core and standard components
	#endif
#endif

#ifdef __LINUX__
	#include <stdarg.h>		
	#include <glob.h>
	#include <unistd.h>
	#include <dlfcn.h>
#endif

#ifdef _MSC_VER 		
	#pragma warning ( disable : 4244 )   // "conversion from 'const int' to 'float', possible loss of data"
	//#pragma warning ( disable : 4101 )   // "unreferenced local variable"
	//#pragma warning ( disable : 4018 )   // "'<' : signed/unsigned mismatch"
	#pragma warning ( disable : 4311 )   // "pointer truncation from 'LPVOID' to 'long"	
	//#pragma warning ( disable : 4273 )   // "inconsistent dll linkage"	
	#pragma warning ( error : 4013 )   // "function undefined; assuming extern returning int"
	#pragma warning ( disable : 4150 )	// deletion of pointer to incomplete type 'LynxEngine::MaterialSystem::CMaterial'; no destructor called
	
	#pragma warning ( disable : 4244 )   // "conversion from 'const int' to 'float', possible loss of data"
	#pragma warning ( disable : 4101 )   // "unreferenced local variable"
	#pragma warning ( disable : 4018 )   // "'<' : signed/unsigned mismatch"
	#pragma warning ( disable : 4311 )   // "pointer truncation from 'LPVOID' to 'long"	
	#pragma warning ( disable : 4819 )   // "The file contains a character that cannot be represented in the current code page (950). Save the file in Unicode format to prevent data loss"	
	#pragma warning ( error : 4013 )   // "The file contains a character that cannot be represented in the current code page (950). Save the file in Unicode format to prevent data loss"	

	#if (_MSC_VER) >= 1400
		#pragma warning ( disable : 4996 )   // "This function or variable may be unsafe"
	#endif
	
	#define SPTINTF					sprintf_s
	#define sprintf					sprintf_s
	#define STRCPY(a, b)			strcpy_s(a, b)
	#define FOPEN(a, b, c)			_tfopen_s(&(a), b, c)
#else
    #define SPTINTF					sprintf
    #define STRCPY(a, b)			strcpy(a, b)
    #define FOPEN(a, b, c)			a = _tfopen(b, c)
#endif

#ifdef __WIN32__	
	#if _WINDOWS || _CONSOLE || ((_LIB||_USRDLL) && !_XBOX)
		#if (WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) 
			#define WINVER					_WIN32_WINNT_WIN7
			#define _WIN32_WINNT			_WIN32_WINNT_WIN7
			//#include <windows.h>
			#include <winsock2.h>
			#include <process.h>
			#include <objbase.h>
			#include <io.h>			
			#include <Winuser.h>
			#include <Psapi.h>
		#elif (WINAPI_FAMILY==WINAPI_FAMILY_APP || WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
			#include <windows.h>
			#include <io.h>		
		#endif		
	#elif defined _XBOX
		#include <xtl.h>
		#include <xgraphics.h>
		#include <io.h>		
		#include <xmv.h>
		//#include <wmvxmo.h>
		#include <xfont.h>
	#endif	
#elif defined __WINCE__ 		
	#include <windows.h>
	#include <windowsx.h>		
	typedef	HANDLE									LYNXTHREADHANDLE;
#elif defined __iOS__
    #include <objc/objc.h>
    #include <CoreFoundation/CoreFoundation.h>
    #include <pthread.h>
    #include <mach/mach.h>
    #include <mach/mach_host.h>  
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <TargetConditionals.h>
    #include <string.h>
#elif __ANDROID__
	#include <android/log.h>	
	#include <pthread.h>
#endif

#ifdef __EGL__
	#include <EGL/egl.h>
#endif

#ifdef __DIRECTINPUT__
	#ifndef DIRECTINPUT_VERSION
		#define DIRECTINPUT_VERSION								0x800 	
	#endif
	#define IS_DIRECTINPUT_VERSION8							(DIRECTINPUT_VERSION == 0x0800) 	
	#include <dinput.h>
#endif

#include <LynxAssert.h>
#include <unzip.h>
#include <zip.h>

#ifdef __WIN32__
	typedef	__int64										LYNXLONGLONG;			
	typedef	unsigned __int64							LYNXULONGLONG;			
	typedef	__int16										LYNXINT16;			
	typedef	__int32										LYNXINT32;			
	typedef	__int64										LYNXINT64;
	typedef	unsigned __int16							LYNXUINT16;			
	typedef	unsigned __int32							LYNXUINT32;			
	typedef	unsigned __int64							LYNXUINT64;
#else
	typedef	long long									LYNXLONGLONG;			
	typedef	unsigned long long							LYNXULONGLONG;			
	typedef	int16_t										LYNXINT16;			
	typedef	int32_t										LYNXINT32;			
	typedef	int64_t										LYNXINT64;
	typedef	uint16_t									LYNXUINT16;			
	typedef	uint32_t									LYNXUINT32;			
	typedef	uint64_t									LYNXUINT64;
#endif		

typedef	unsigned char								LYNXBOOL;
typedef	unsigned char								LYNXBYTE;
typedef	LYNXUINT16									LYNXWORD;
#ifdef __WIN32__
	typedef	LYNXUINT32								LYNXDWORD;
#else
	typedef	LYNXUINT32		     					LYNXDWORD;
#endif
typedef	short										LYNXSHORT;
typedef	unsigned short								LYNXUSHORT;

typedef	int											LYNXINT;
typedef	unsigned int								LYNXUINT;
typedef	long										LYNXLONG;
typedef	unsigned long								LYNXULONG;

/*
typedef	LYNXINT32									LYNXINT;
typedef	LYNXUINT32									LYNXUINT;
typedef	LYNXINT64									LYNXLONG;
typedef	LYNXUINT64									LYNXULONG;
*/

typedef	unsigned									LYNXBITFIELD;	
typedef	float										LYNXFLOAT;
typedef	double										LYNXDOUBLE;
typedef	LYNXINT32									LYNXFIXED;
typedef	void										LYNXVOID;	
typedef	void										*LYNXLPVOID;	
typedef	int											LYNXERRORNO;
#define LYNX_BITFIELD_BOOL(v)						LYNXBITFIELD v:1;
#define LYNX_BITFIELD(v, b)							LYNXBITFIELD v:b;

typedef	char										LYNXANSICHAR;
#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
	//typedef	LYNXWORD									LYNXUNICHAR;
    typedef	wchar_t										LYNXUNICHAR;
#else
	#ifdef __GNUC__
		typedef	wchar_t										LYNXUNICHAR;	
	#else
		typedef	wchar_t										LYNXUNICHAR;	
	#endif
#endif
#ifdef _UNICODE
	typedef	LYNXUNICHAR									LYNXCHAR;
	typedef	LYNXUNICHAR									LYNXUCHAR;	
#else
	typedef	char										LYNXCHAR;
	typedef	unsigned char								LYNXUCHAR;	
#endif

/**
 *  @ingroup LynxMath 
 *	@struct LYNXREAL
 *  @brief 實數。  	
 *
 *	依據需求不同，實數有不同定義。
 *  <div class="fragment">
 *	<pre>	
 *	#ifdef __SINGLE__
 *		typedef		LYNXFLOAT		LYNXREAL;
 *	#elif defined __DOUBLE__
 *		typedef		LYNXDOUBLE		LYNXREAL;
 *	#elif defined __FIXED_POINT__
 *		typedef		LYNXFIXED		LYNXREAL;
 *	#else
 *		typedef		LYNXFLOAT		LYNXREAL;
 *	#endif
 *	</pre>
 *  </div>
 */
#ifdef __SINGLE__
	typedef	LYNXFLOAT									LYNXREAL;
#elif defined __DOUBLE__
	typedef	LYNXDOUBLE									LYNXREAL;
#elif defined __FIXED_POINT__
	typedef	LYNXFIXED									LYNXREAL;
#else
	typedef	LYNXFLOAT									LYNXREAL;
#endif

#define	LYNX_FALSE									(const LYNXBOOL)0
#define	LYNX_TRUE									(const LYNXBOOL)1

#define LYNX_MAXBYTE								0xff
#define	LYNX_MAXWORD								0xffffU    
#define	LYNX_MAXDWORD								0xffffffffU
#define	LYNX_MAXSBYTE								0x7f       
#define	LYNX_MAXSWORD								0x7fff     
#define	LYNX_MAXINT									0x7fffffff 
#define	LYNX_INDEX_NONE								-1         
#define	LYNX_UNICODE_BOM							0xfeff     

#ifndef FALSE
	#define		FALSE										0
#endif
#ifndef TRUE
	#define		TRUE										1
#endif

#define LYNX_CACHE_SIZE										4	//4 Bytes		
#ifdef __PS2__
	#define LYNXALIGNED(b)									__attribute__ ((aligned(b)))
	#define	LYNX_CACHE_ALIGN								
#elif defined __WIN32__
	#define LYNXALIGNED(b)									
	#define	LYNX_CACHE_ALIGN								__declspec(align(LYNX_CACHE_SIZE))
#else
	#define LYNXALIGNED(b)									
	#define	LYNX_CACHE_ALIGN								
#endif

#include <string.h>
#include <time.h>
#ifdef __WIN32__ 		
	#if _WINDOWS || _CONSOLE || ((_LIB||_USRDLL) && !_XBOX)		
		typedef	HANDLE									LYNXTHREADHANDLE;		
		typedef HANDLE									LYNXMUTEX;

		#ifdef __EGL__
			typedef	NativeWindowType						LYNXWINDOW;			
		#else
			typedef	HWND									LYNXWINDOW;
		#endif		
		typedef	HDC										LYNXDC;
	#elif defined _XBOX
		#define XFONT_TRUETYPE 	
		typedef	VOID (* TIMERPROC)(HWND, UINT, UINT, DWORD);		
		#define BI_RGB        								0L
		#define BI_RLE8       								1L
		#define BI_RLE4       								2L
		#define BI_BITFIELDS  								3L 	
		typedef struct tagRGBQUAD {
   				BYTE	    rgbBlue;
				BYTE		rgbGreen;
				BYTE		rgbRed;
				BYTE		rgbReserved;
		}RGBQUAD;
		typedef struct tagBITMAPINFOHEADER{
        		DWORD		biSize;
				LONG		biWidth;
				LONG		biHeight;
				WORD		biPlanes;
				WORD		biBitCount;
				DWORD		biCompression;
				DWORD		biSizeImage;
				LONG		biXPelsPerMeter;
				LONG		biYPelsPerMeter;
				DWORD		biClrUsed;
				DWORD		biClrImportant;
		}BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
		typedef struct vdITMAPFILEHEADER {
				WORD		bfType;
				DWORD		bfSize;
				WORD		bfReserved1;
				WORD		bfReserved2;
				DWORD		bfOffBits;
		}BITMAPFILEHEADER, *LPBITMAPFILEHEADER;
		typedef struct tagBITMAPINFO {
				BITMAPINFOHEADER    bmiHeader;
				RGBQUAD             bmiColors[1];
		}BITMAPINFO, *LPBITMAPINFO;
		typedef	DWORD									HMMIO;	
		typedef	DWORD									MMIOINFO;
		typedef	DWORD									MMCKINFO;
		typedef	HANDLE									LYNXTHREADHANDLE;		
		typedef	HWND									LYNXWINDOW;
	#endif	
	#define		LYNXEXPORT							__declspec(dllexport)
	#define		LYNXIMPORT							__declspec(dllimport)	
#elif defined __WINCE__ 		
	typedef	HANDLE									LYNXTHREADHANDLE;
	#ifdef __EGL__
		typedef	NativeWindowType						LYNXWINDOW;			
	#else
		typedef	HWND									LYNXWINDOW;
	#endif	
	#define		LYNXEXPORT							__declspec(dllexport)
	#define		LYNXIMPORT							__declspec(dllimport)
#elif defined __iOS__
	//typedef	signed char									BOOL;
	typedef	unsigned char								BYTE;
	typedef	LYNXWORD									WORD;
	typedef	LYNXDWORD									DWORD;	
	typedef	float										FLOAT;
	#ifndef __NUCLEUS__
		typedef	void										VOID;			
	#endif
	typedef	void										*LPVOID;	
	typedef	unsigned int								UINT;
	typedef	long										LONG;
			
	#define	CALLBACK										
	typedef	DWORD										HINSTANCE;
	#ifdef __EGL__
		typedef	NativeWindowType						LYNXWINDOW;			
	#else
		#ifdef __XWIN__		
			typedef	Window										LYNXWINDOW;
		#elif defined __OS_X__
			typedef	WindowRef									LYNXWINDOW;		
		#else
			typedef	unsigned int								LYNXWINDOW;			
		#endif
	#endif	
	#define	WINAPI				
	typedef	DWORD										LYNXDC;
	typedef	LPVOID										HMODULE;
	#define	DECLARE_HANDLE(h)				
	typedef	DWORD										HPBUFFERARB;			
	typedef struct {
			BYTE					peRed;
			BYTE					peGreen;
			BYTE					peBlue;
			BYTE					peFlags ;
	} PALETTEENTRY, *LPPALETTEENTRY;
	#define BI_RGB        									0L
	#define BI_RLE8       									1L
	#define BI_RLE4       									2L
	#define BI_BITFIELDS  									3L 	
	typedef struct tagRGBQUAD {
   			BYTE	    rgbBlue;
			BYTE		rgbGreen;
			BYTE		rgbRed;
			BYTE		rgbReserved;
	} RGBQUAD;
	typedef struct tagBITMAPINFOHEADER{
        	DWORD		biSize;
			LONG		biWidth;
			LONG		biHeight;
			WORD		biPlanes;
			WORD		biBitCount;
			DWORD		biCompression;
			DWORD		biSizeImage;
			LONG		biXPelsPerMeter;
			LONG		biYPelsPerMeter;
			DWORD		biClrUsed;
			DWORD		biClrImportant;
	} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
	typedef struct tagBITMAPFILEHEADER {
			WORD		bfType;
			DWORD		bfSize;
			WORD		bfReserved1;
			WORD		bfReserved2;
			DWORD		bfOffBits;
	} BITMAPFILEHEADER, *LPBITMAPFILEHEADER;
	typedef struct tagBITMAPINFO {
			BITMAPINFOHEADER    bmiHeader;
			RGBQUAD             bmiColors[1];
	} BITMAPINFO, *LPBITMAPINFO;
	typedef	DWORD										HMMIO;	
	typedef	DWORD										MMIOINFO;
	typedef	DWORD										MMCKINFO;
	typedef	int											HRESULT;	
	typedef	VOID (* TIMERPROC)(LYNXWINDOW, UINT, UINT, DWORD);
	typedef	LPVOID										FARPROC;
	typedef	pthread_t                                   LYNXTHREADHANDLE;
	typedef	pthread_mutex_t								LYNXMUTEX;
	#define	LYNXEXPORT									
	#define	LYNXIMPORT									
#elif defined __ANDROID__
	typedef	int											BOOL;
	typedef	unsigned char								BYTE;
	typedef	LYNXWORD									WORD;
	typedef	LYNXDWORD									DWORD;
	typedef	float										FLOAT;
	#ifndef __NUCLEUS__
		typedef	void										VOID;			
	#endif
	typedef	void										*LPVOID;	
	typedef	unsigned int								UINT;
	typedef	long										LONG;
			
	#define	CALLBACK										
	typedef	DWORD										HINSTANCE;
	#ifdef __EGL__
		typedef	NativeWindowType						LYNXWINDOW;			
	#else
		typedef	unsigned int								LYNXWINDOW;					
	#endif	
	#define	WINAPI				
	typedef	DWORD										LYNXDC;
	typedef	LPVOID										HMODULE;
	#define	DECLARE_HANDLE(h)				
	typedef	DWORD										HPBUFFERARB;			
	typedef struct {
			BYTE					peRed;
			BYTE					peGreen;
			BYTE					peBlue;
			BYTE					peFlags ;
	} PALETTEENTRY, *LPPALETTEENTRY;
	#define BI_RGB        									0L
	#define BI_RLE8       									1L
	#define BI_RLE4       									2L
	#define BI_BITFIELDS  									3L 	
	typedef struct tagRGBQUAD {
   			BYTE	    rgbBlue;
			BYTE		rgbGreen;
			BYTE		rgbRed;
			BYTE		rgbReserved;
	} RGBQUAD;
	typedef struct tagBITMAPINFOHEADER{
        	DWORD		biSize;
			LONG		biWidth;
			LONG		biHeight;
			WORD		biPlanes;
			WORD		biBitCount;
			DWORD		biCompression;
			DWORD		biSizeImage;
			LONG		biXPelsPerMeter;
			LONG		biYPelsPerMeter;
			DWORD		biClrUsed;
			DWORD		biClrImportant;
	} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
	typedef struct tagBITMAPFILEHEADER {
			WORD		bfType;
			DWORD		bfSize;
			WORD		bfReserved1;
			WORD		bfReserved2;
			DWORD		bfOffBits;
	} BITMAPFILEHEADER, *LPBITMAPFILEHEADER;
	typedef struct tagBITMAPINFO {
			BITMAPINFOHEADER    bmiHeader;
			RGBQUAD             bmiColors[1];
	} BITMAPINFO, *LPBITMAPINFO;
	typedef	DWORD										HMMIO;	
	typedef	DWORD										MMIOINFO;
	typedef	DWORD										MMCKINFO;
	typedef	int											HRESULT;	
	typedef	VOID (* TIMERPROC)(LYNXWINDOW, UINT, UINT, DWORD);
	typedef	LPVOID										FARPROC;
	typedef	pthread_t                                   LYNXTHREADHANDLE;
	typedef	pthread_mutex_t								LYNXMUTEX;
	#define	LYNXEXPORT									
	#define	LYNXIMPORT									
#else
	typedef	int											BOOL;
	typedef	unsigned char								BYTE;
	typedef	unsigned short								WORD;
	#ifdef __PS2__
		typedef	unsigned __int32							DWORD;
		//typedef		unsigned int								DWORD;
	#else
		//typedef		unsigned __int32							DWORD;
		typedef	unsigned long								DWORD;
	#endif
	typedef	float										FLOAT;
	#ifndef __NUCLEUS__
		typedef	void										VOID;			
	#endif
	typedef	void										*LPVOID;	
	typedef	unsigned int								UINT;
	typedef	long										LONG;
			
	#define	CALLBACK										
	typedef	DWORD										HINSTANCE;
	#ifdef __EGL__
		typedef	NativeWindowType						LYNXWINDOW;			
	#else
		#ifdef __XWIN__		
			typedef	Window										LYNXWINDOW;
		#elif defined __OS_X__
			typedef	WindowRef									LYNXWINDOW;		
		#else
			typedef	unsigned int								LYNXWINDOW;			
		#endif
	#endif	
	#define	WINAPI				
	typedef	DWORD										LYNXDC;
	typedef	LPVOID										HMODULE;
	#define	DECLARE_HANDLE(h)				
	typedef	DWORD										HPBUFFERARB;			
	typedef struct {
			BYTE					peRed;
			BYTE					peGreen;
			BYTE					peBlue;
			BYTE					peFlags ;
	} PALETTEENTRY, *LPPALETTEENTRY;
	#define BI_RGB        									0L
	#define BI_RLE8       									1L
	#define BI_RLE4       									2L
	#define BI_BITFIELDS  									3L 	
	typedef struct tagRGBQUAD {
   			BYTE	    rgbBlue;
			BYTE		rgbGreen;
			BYTE		rgbRed;
			BYTE		rgbReserved;
	} RGBQUAD;
	typedef struct tagBITMAPINFOHEADER{
        	DWORD		biSize;
			LONG		biWidth;
			LONG		biHeight;
			WORD		biPlanes;
			WORD		biBitCount;
			DWORD		biCompression;
			DWORD		biSizeImage;
			LONG		biXPelsPerMeter;
			LONG		biYPelsPerMeter;
			DWORD		biClrUsed;
			DWORD		biClrImportant;
	} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
	typedef struct tagBITMAPFILEHEADER {
			WORD		bfType;
			DWORD		bfSize;
			WORD		bfReserved1;
			WORD		bfReserved2;
			DWORD		bfOffBits;
	} BITMAPFILEHEADER, *LPBITMAPFILEHEADER;
	typedef struct tagBITMAPINFO {
			BITMAPINFOHEADER    bmiHeader;
			RGBQUAD             bmiColors[1];
	} BITMAPINFO, *LPBITMAPINFO;
	typedef	DWORD										HMMIO;	
	typedef	DWORD										MMIOINFO;
	typedef	DWORD										MMCKINFO;
	typedef	int											HRESULT;	
	typedef	VOID (* TIMERPROC)(LYNXWINDOW, UINT, UINT, DWORD);
	typedef	LPVOID										FARPROC;
	typedef	DWORD										LYNXTHREADHANDLE;
	#define	LYNXEXPORT									
	#define	LYNXIMPORT									
#endif

#if defined(_MSC_VER)
	#ifdef 	__ANDROID__
		//#define		LYNXINLINE												__inline__	__attribute__((gnu_inline))
		#define		LYNXINLINE												__inline__	__attribute__((always_inline)) 			
		#define		LYNXFORCEINLINE											__inline__	__attribute__((always_inline)) 			
	#else
		#define		LYNXINLINE												__inline 		
		#define		LYNXFORCEINLINE											__forceinline	 			
	#endif
#elif defined (__GNUC__)
	#ifdef 	__ANDROID__
		//#define		LYNXINLINE												__inline__	__attribute__((gnu_inline))
		#define		LYNXINLINE												__inline__	__attribute__((always_inline)) 			
		#define		LYNXFORCEINLINE											__inline__	__attribute__((always_inline)) 			
	#else
		//#define		LYNXINLINE												__inline__	__attribute__((gnu_inline))
        #define		LYNXINLINE												__inline__	__attribute__((always_inline))
		#define		LYNXFORCEINLINE											__inline__	__attribute__((always_inline)) 			
	#endif	
#elif defined __PS2__
	#define		LYNXINLINE												inline	
	#define		LYNXFORCEINLINE											inline	 		
#else
	#define		LYNXINLINE												inline	
	#define		LYNXFORCEINLINE											inline	 			
#endif

#ifdef __cplusplus
	#define		LYNXEXTERN													extern	"C"
#else
	#define		LYNXEXTERN													extern
#endif

#ifdef __LYNXCORE_LIB__
	#define LYNXCOREAPI
#elif defined __LYNXDLL__
	#define	LYNXCOREAPI									LYNXEXPORT	
#else
	#define LYNXCOREAPI									LYNXIMPORT	
#endif

#ifdef __LYNXGRAPHICS_LIB__
	#define LYNXGRAPHICSAPI
#elif defined __LYNXGRAPHICS_DLL__
	#define	LYNXGRAPHICSAPI								LYNXEXPORT	
#else
	#define LYNXGRAPHICSAPI								LYNXIMPORT	
#endif

#ifdef __LYNXSDK_LIB__
	#define LYNXSDKAPI
#elif defined __LYNXSDK_DLL__
	#define	LYNXSDKAPI									LYNXEXPORT	
#else
	#define LYNXSDKAPI									LYNXIMPORT	
#endif

typedef	time_t												LYNXTIMESTAMP;			

#define LYNX_INHERIT(p, n)									p	n

#define LYNX_NULL_TEXTURE									0x00	
#define LYNX_SYSTEM_MEMORY									0x10
#define LYNX_VIDEO_MEMORY									0x11
#define LYNX_PURE_SYSTEM_MEMORY								0x12	

#define	LYNX_ABOVE											0x4
#define	LYNX_BELOW											0x8
#define	LYNX_LEFT											0x1
#define	LYNX_RIGHT											0x2

#define	LYNX_FIXPOINT_SHIFT									10

#define LYNX_ARRAY_SIZE(a)								(sizeof((a))/sizeof((a)[0]))
#define LYNX_STRUCTURE_OFFSET(s, member)				((DWORD)&((s*)NULL)->member)

typedef enum{
	LYNX_FREE_ENTRY,
	LYNX_USED_ENTRY,
	LYNX_NUM_ENTRY_ID,
}LYNXENTRYID;

typedef enum{	
	LYNX_VER_NON_COMMERCIAL,	
	LYNX_VER_COMMERCIAL,			
}LYNXLICENSEVERSIONCODE;

/**
 *  @ingroup LynxGraphic 
 *  @brief 
 *  
 * 
 */
typedef union {
	void*			lpVOID;	
	LYNXBYTE*		lpBYTE;
	LYNXWORD*		lpWORD;
	LYNXDWORD*		lpDWORD;
	LYNXLONGLONG*	lpLONG64;
	LYNXSHORT*		lpShort;
	LYNXINT*		lpInt;	
	LYNXREAL*		lpReal;
	LYNXFLOAT*		lpFloat;
	LYNXDOUBLE*		lpDouble;
}LYNXUNIONPTR, *LPLYNXUNIONPTR;
typedef const LYNXUNIONPTR* LPCLYNXUNIONPTR;

/**
 *  @ingroup LynxGraphic 
 *	@struct LPLYNXCOLORRGB
 *  @brief R,G,B 顏色資料指標。 
 *
 *  @see LYNXCOLORRGB
 */
/**
 *  @ingroup LynxGraphic 
 *  @brief R,G,B 顏色資料。
 * 
 *	R,G,B 顏色資料。
 *
 *  @see LPLYNXCOLORRGB
 */
typedef struct {    
	BYTE					Blue;				/*!<  blue component of color */
	BYTE					Green;				/*!<  green component of color */
	BYTE					Red;				/*!<  red component of color */	
}LYNXCOLORRGB, *LPLYNXCOLORRGB;
typedef const LYNXCOLORRGB* LPCLYNXCOLORRGB;

/**
 *  @ingroup LynxGraphic 
 *	@struct LPLYNXCOLORRGBA
 *  @brief R,G,B,A 顏色資料指標。 
 *
 *  @see LYNXCOLORRGBA
 */
/**
 *  @ingroup LynxGraphic 
 *  @brief R,G,B,A 顏色資料。
 * 
 *	<div class="fragment">
 *	<pre>	
 *	LYNX_CACHE_ALIGN typedef struct {
 *		union {
 *			struct{
 *				BYTE					Red;				
 *				BYTE					Green;				
 *				BYTE					Blue;				
 *				BYTE					Alpha;				
 *			}s;
 *			DWORD						PackedColor;
 *		};
 *	}LYNXCOLORRGBA, *LPLYNXCOLORRGBA;
 *	</pre>
 *  </div> 
 *
 *  @see LPLYNXCOLORRGBA
 */
typedef struct _LYNXCOLORRGBA{
	union {
		struct {			
			BYTE					Blue;				/*!< blue component of color */						
			BYTE					Green;				/*!< green component of color */			
			BYTE					Red;				/*!< red component of color */			
			BYTE					Alpha;				/*!< alpha component of color */			
		};
		DWORD					PackedColor;	
	};	
}LYNXCOLORRGBA, *LPLYNXCOLORRGBA;
typedef const LYNXCOLORRGBA* LPCLYNXCOLORRGBA;

LYNX_CACHE_ALIGN typedef struct {
    BYTE  Red;				// red component of color
    BYTE  Green;			// green component of color
    BYTE  Blue;				// blue component of color
	BYTE  Alpha;
	BYTE  Flag;	
}LYNXPALETTEENTRY, *LPLYNXPALETTEENTRY;
typedef const LYNXPALETTEENTRY* LPCLYNXPALETTEENTRY;

LYNX_CACHE_ALIGN typedef struct {
	LPVOID							lpObject;
	int 							ChildIndex;	
}LYNXLINKDATA,* LPLYNXLINKDATA; 
typedef const LYNXLINKDATA* LPCLYNXLINKDATA;
			
/**
 *  @ingroup LynxGraphic 
 *	@struct LPLYNXPOINT2D
 *  @brief 2D座標資料指標。 
 *
 *  @see LYNXPOINT2D
 */
/**
 *  @ingroup LynxGraphic 
 *  @brief 2D座標資料。
 * 
 *	2D座標資料。
 *
 *  @see LPLYNXPOINT2D
 */
LYNX_CACHE_ALIGN typedef struct { 
	int						x;
	int						y;
}LYNXPOINT2D,* LPLYNXPOINT2D;
typedef const LYNXPOINT2D* LPCLYNXPOINT2D;


/**
 *  @ingroup LynxGraphic 
 *	@struct LPLYNXUVPOINT
 *  @brief 貼圖軸座標資料指標。 
 *
 *  @see LYNXUVPOINT
 */
/**
 *  @ingroup LynxGraphic 
 *  @brief 貼圖軸座標資料。
 * 
 *	貼圖軸座標資料。
 *
 *  @see LPLYNXUVPOINT
 */
typedef	struct {
	LYNXREAL				u, v;  //Texture coordination
}LYNXUVPOINT,* LPLYNXUVPOINT;
typedef const LYNXUVPOINT* LPCLYNXUVPOINT;

/**
 *  @ingroup LynxGraphic 
 *	@struct LPLYNXRECT
 *  @brief 2D矩形座標資料指標。 
 *
 *  @see LYNXRECT
 */
/**
 *  @ingroup LynxGraphic 
 *  @brief 2D矩形座標資料。
 * 
 *	2D矩形座標資料。
 *
 *  @see LPLYNXRECT
 */
LYNX_CACHE_ALIGN typedef struct {
	int					x1, y1;
	int					x2, y2;
}LYNXRECT,* LPLYNXRECT;
typedef const LYNXRECT* LPCLYNXRECT;

typedef enum{
	LYNX_PM_ONCE = 0,
	LYNX_PM_LOOP,
	LYNX_PM_BOUNCE,
	LYNX_NUM_PLAY_MODE,
}LYNXPLAYMODE;

#endif