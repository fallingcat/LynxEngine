#ifndef __LINXENGINEDEFINES_H__
#define __LINXENGINEDEFINES_H__

#ifdef __LYNXENGINE_LIB__
	#define	LYNXENGCLASS						
#elif __LYNXENGINE_DLL__
	#define	LYNXENGCLASS						LYNXEXPORT
#else
	#define LYNXENGCLASS						LYNXIMPORT
#endif

#ifdef __LYNXENGINE_LIB__
	#define	LYNXENGTEMPLATE						
#elif __LYNXENGINE_DLL__
	#define	LYNXENGTEMPLATE						LYNXEXPORT
#else
	#define	LYNXENGTEMPLATE						LYNXEXPORT
#endif

#define LYNX_SORT_DEC							LYNX_TRUE
#define LYNX_SORT_ASC							LYNX_FALSE

LYNXENGCLASS void* LynxEngine_Alloc(size_t s);
LYNXENGCLASS void* LynxEngine_Alloc(size_t s, const char *file, const char *func, int line);
LYNXENGCLASS void LynxEngine_Free(void* p);
LYNXENGCLASS void LynxEngine_Free(void* p, size_t s);
LYNXENGCLASS void LynxEngine_Free(void* p, const char *file, const char *func, int line);
LYNXENGCLASS void LynxEngine_RemoveRecord(void* p);
LYNXENGCLASS void LynxEngine_AddRecord(void* p, size_t s, const char *file, const char *func, int line);

#if 0
//LYNXENGCLASS void* operator new(size_t s);
//LYNXENGCLASS void* operator new[](size_t s);
LYNXENGCLASS void* operator new(size_t s, const char *file, const char *func, int line);
LYNXENGCLASS void* operator new[](size_t s, const char *file, const char *func, int line);
LYNXENGCLASS void operator delete(void* p);
LYNXENGCLASS void operator delete[](void* p);
LYNXENGCLASS void operator delete(void* p, size_t s);
LYNXENGCLASS void operator delete[](void* p, size_t s);
LYNXENGCLASS void operator delete(void* p, const char *file, const char *func, int line);
LYNXENGCLASS void operator delete[](void* p, const char *file, const char *func, int line);	
#endif


#ifdef __TOOL__
	#define	LYNXNEW							::new
	#define	LYNXDEL							::delete	

	#define	LYNXNEW							::new
	#define	LYNXDEL(x)						::delete x
	#define	LYNXDEL_ARRAY(x)				::delete [] x
#else
	#if (defined(_DEBUG) || defined(DEBUG))				
		#if (defined (__WIN32__) && defined(__VC_MEM_LEAK_CHECK__))		
			#define	LYNXNEW						::new( _CLIENT_BLOCK , __FILE__ , __LINE__ )
			#define	LYNXDEL						::delete				
			#define	LYNXDEL_ARRAY(x)			::delete [] x

			#define	LYNXGLOBAL_NEW				::new( _CLIENT_BLOCK , __FILE__ , __LINE__ )
			#define	LYNXGLOBAL_DEL				::delete				
			#define	LYNXGLOBAL_DEL_ARRAY(x)		::delete [] x

			#define LYNXALLOC(_s)				LYNXNEW LYNXBYTE[_s]
			#define LYNXALLOC_ARRAY(_c, _s)		LYNXNEW _c[_s]
			#define LYNXFREE(x)					LYNXDEL x
			#define LYNXFREE_ARRAY(x)			LYNXDEL_ARRAY(x)
		#elif (defined (__WIN32__) && defined(__VLD__))
			#define	LYNXNEW						::new
			#define	LYNXDEL						::delete				
			#define	LYNXDEL_ARRAY(x)			::delete [] x

			#define	LYNXGLOBAL_NEW				::new	
			#define	LYNXGLOBAL_DEL				::delete				
			#define	LYNXGLOBAL_DEL_ARRAY(x)		::delete [] x

			#define LYNXALLOC(_s)				LYNXNEW LYNXBYTE[_s]
			#define LYNXALLOC_ARRAY(_c, _s)		LYNXNEW _c[_s]
			#define LYNXFREE(x)					LYNXDEL x
			#define LYNXFREE_ARRAY(x)			LYNXDEL_ARRAY(x)
		#elif (defined (__WIN32__) &&  defined (__LYNX_DEBUG_MEM_ALLOCATOR__))
			#define	LYNXNEW						new(__FILE__, __FUNCTION__, __LINE__)		
			#define	LYNXDEL						delete				
			#define	LYNXDEL_ARRAY(x)			delete [] x

			#define	LYNXGLOBAL_NEW				::new
			#define	LYNXGLOBAL_DEL				::delete				
			#define	LYNXGLOBAL_DEL_ARRAY(x)		::delete [] x

			#define LYNXALLOC(_s)				LYNXGLOBAL_NEW LYNXBYTE[_s]
			#define LYNXALLOC_ARRAY(_c, _s)		LYNXGLOBAL_NEW _c[_s]
			#define LYNXFREE(x)					LYNXGLOBAL_DEL x
			#define LYNXFREE_ARRAY(x)			LYNXGLOBAL_DEL_ARRAY(x)
		#else
			#define	LYNXNEW						new
			#define	LYNXDEL						delete
			#define	LYNXDEL_ARRAY(x)			delete [] x

			#define	LYNXGLOBAL_NEW				::new
			#define	LYNXGLOBAL_DEL				::delete				
			#define	LYNXGLOBAL_DEL_ARRAY(x)		::delete [] x

			#define LYNXALLOC(_s)				lynxMemAlloc(_s);
			#define LYNXALLOC_ARRAY(_c, _s)		(_c*)lynxMemAlloc(sizeof(_c)*_s)
			#define LYNXFREE(x)					lynxMemFree(x)	
			#define LYNXFREE_ARRAY(x)			lynxMemFree(x)
		#endif
	#else		
		#define	LYNXNEW						new
		#define	LYNXDEL						delete				
		#define	LYNXDEL_ARRAY(x)			delete [] x

		#define	LYNXGLOBAL_NEW				::new	
		#define	LYNXGLOBAL_DEL				::delete				
		#define	LYNXGLOBAL_DEL_ARRAY(x)		::delete [] x
		
		#define LYNXALLOC(_s)				lynxMemAlloc(_s)
		#define LYNXALLOC_ARRAY(_c, _s)		lynxMemAlloc(sizeof(_c)*_s)
		#define LYNXFREE(x)					lynxMemFree(x)
		#define LYNXFREE_ARRAY(x)			lynxMemFree(x)
	#endif	
	
#endif

#include <LynxString.h>

#ifdef __iOS__
    //#import <Foundation/NSString.h>
    #ifdef _UNICODE
        #define iOS_STRING_ENCODING                      NSUTF32LittleEndianStringEncoding
    #else
        #define iOS_STRING_ENCODING                      NSUTF8StringEncoding//NSASCIIStringEncoding
    #endif
#endif

#ifdef _MSC_VER
	#if defined (xx_CPPRTTI)
		#include <typeinfo.h>	
		#define LYNX_CLASSNAME(n)						(_T(typeid(n).name()))
		#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return (typeid(*this).name()); };	
	#else
		#define LYNX_CLASSNAME(n)						(#n)
		#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return (LYNX_CLASSNAME(n)); };	
	#endif
#elif defined(__GNUC__)
	#if defined (_CPPRTTI)		
		// Starting with version 4.3.2, gcc defines __GXX_RTTI iff RTTI is enabled.
		#if GTEST_GCC_VER_ >= 40302
			#ifdef __GXX_RTTI
				#include <typeinfo>	
				#define LYNX_CLASSNAME(n)						(_T(typeid(n).name()))
				#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return LynxEngine::CAnsiString(typeid(*this).name()); };	
			#else
				#define LYNX_CLASSNAME(n)						(#n)
				#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return LynxEngine::CAnsiString(LYNX_CLASSNAME(n)); };	
			#endif  // __GXX_RTTI
		#else
			// For gcc versions smaller than 4.3.2, we assume RTTI is enabled.
			#include <typeinfo>	
			#define LYNX_CLASSNAME(n)						(_T(typeid(n).name()))
			#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return LynxEngine::CAnsiString(typeid(*this).name()); };	
		#endif  // GTEST_GCC_VER >= 40302
	#else
		#define LYNX_CLASSNAME(n)						(#n)
		#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return LynxEngine::CAnsiString(LYNX_CLASSNAME(n)); };	
	#endif
#else
	#include <typeinfo.h>	
	#define LYNX_CLASSNAME(n)						(_T(typeid(n).name()))
	#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const LynxEngine::CAnsiString GetClassName()  {return LynxEngine::CAnsiString(typeid(*this).name()); };	
#endif

#ifdef __OBJC__
    #define  LYNX_FORWARD_DECLARATION(cls)          @class cls;
#else
    #define  LYNX_FORWARD_DECLARATION(cls)          class cls;
#endif

#define LYNX_DECLARE_CLASS(TClass)\
	public:\
		LYNX_DECLARE_GETCLASSNAME(TClass)

#define FUNC_NAME(func)							static char __FUNC_NAME__[] = (#func);

#define CHECK_SUM_STRING							_T("Com.Qubit.Games.Qubot")
#define CHECK_SUM_STRING2							_T("{0425D5E6-5D84-487C-A951-B89208F04819}")

#define OPEN_Z_FILE(stream, filename) \
		LynxEngine::CMemoryStream OutStream;\
		LynxEngine::CMemoryStream Stream;\
		uLongf Size;\
		if (Stream.vOpen(filename+LynxEngine::CString(".z"), LynxEngine::CStream::Binary|LynxEngine::CStream::Read))\
		{\
			LYNXDWORD UnzipSize;\
			Stream.vRead(&UnzipSize, sizeof(LYNXDWORD), 1);\
			Size = UnzipSize;\
			OutStream.Alloc(Size);\
			BYTE* Dest = (BYTE*)OutStream.GetCurrentPtr();\
			BYTE* Src = (BYTE*)Stream.GetCurrentPtr();\
			uncompress(Dest, &Size, (const Bytef *)Src, Stream.vSize()-sizeof(LYNXDWORD));\
			Stream.vClose();\
			stream = &OutStream;\
		}\

#define PACKAGE_OPEN_FILE(_stream, _filename, _flag) \
	LynxEngine::CMemoryStream _Stream; \
	static LynxEngine::CMemoryStream _OutStream; \
	LYNXBOOL bResult;\
	if (m_lpEngine->GetlpFileSystem()->GetPackage()->OpenFile(&_Stream, _filename + CString(".z"), _flag))\
	{\
		uLongf Size; \
		LYNXDWORD UnzipSize;\
		_Stream.vRead(&UnzipSize, sizeof(LYNXDWORD), 1);\
		Size = UnzipSize;\
		_OutStream.Alloc(Size);\
		BYTE* Dest = (BYTE*)_OutStream.GetCurrentPtr();\
		BYTE* Src = (BYTE*)_Stream.GetCurrentPtr();\
		uncompress(Dest, &Size, (const Bytef *)Src, _Stream.vSize() - sizeof(LYNXDWORD));\
		_Stream.vClose();\
		_stream = &_OutStream;\
		bResult = LYNX_TRUE;\
	}\
	else\
	{\
		bResult = m_lpEngine->GetlpFileSystem()->GetPackage()->OpenFile(_stream, _filename, _flag);\
	}\

#define SYSTEM_DEFAULT_TEXTURE_FONT_L					"DefaultTextureFont_L"
#define SYSTEM_DEFAULT_TEXTURE_FONT						"DefaultTextureFont"
#define SYSTEM_DEFAULT_TEXTURE_FONT_S					"DefaultTextureFont_S"
#define SYSTEM_DEFAULT_TRUETYPE_FONT_L					"DefaultTrueTypeFont_L"
#define SYSTEM_DEFAULT_TRUETYPE_FONT					"DefaultTrueTypeFont"
#define SYSTEM_DEFAULT_TRUETYPE_FONT_S					"DefaultTrueTypeFont_S"
#define NO_TEXT_FOUND									_T("!!No Text Found!!")

#endif