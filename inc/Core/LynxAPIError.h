#ifndef __LYNXAPIERROR_H__
#define __LYNXAPIERROR_H__

#include <LynxType.h>

#ifdef __cplusplus
extern "C" {
#endif
	#ifdef __API_ERRORS__	
		LYNXGRAPHICSAPI void			lynxShowAPIError(long error, char *file, int line, char *call);	
		LYNXGRAPHICSAPI char*			lynxGetAPIErrorStr(void);
		LYNXGRAPHICSAPI void			lynxSetAPIErrorStr(char *str);
		LYNXGRAPHICSAPI char*			lynxGetAPIErrorTitle(void);
		LYNXGRAPHICSAPI void			lynxSetAPIErrorTitle(char *str);
		#define SHOW_API_ERROR(r)   	lynxShowAPIError(r, __FILE__, __LINE__, NULL)				
		#define SHOW_API_ERROR2(r, c)   lynxShowAPIError(r, __FILE__, __LINE__, c)				
	#else
		#define SHOW_API_ERROR(r)                                       				
		#define SHOW_API_ERROR2(r, c)
	#endif

	#ifdef __DIRECT3D__
		#define LYNX_API_TRACE(call)			ghRes = call; if (FAILED(ghRes))  { SHOW_API_ERROR2(ghRes, #call); }			
		#define LYNX_API_TRACE_RETURN(call, r)	ghRes = call; if (FAILED(ghRes))  { SHOW_API_ERROR2(ghRes, #call); return r;}			
	#else
		#define LYNX_API_TRACE(call)			call
		#define LYNX_API_TRACE_RETURN(call, r)	call
	#endif
#ifdef __cplusplus
}
#endif

#endif
