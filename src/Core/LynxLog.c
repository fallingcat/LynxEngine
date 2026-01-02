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

#include <LynxLog.h>

static LPLYNXLOG								glpLog = NULL;
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitLog(LPLYNXLOG lplog)
{
	lynxInitFile(&lplog->LogFile);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxReleaseLog(LPLYNXLOG lplog)
{
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxSetCurrentLog(LPLYNXLOG lplog)
{
	glpLog = lplog;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL lynxOpenLog(const LYNXCHAR *name)
{
	if (glpLog)
	{
		if (LYNX_OPEN_FILE(&glpLog->LogFile, name, (LYNX_FOT_WRITE|LYNX_FOT_TEXT), LYNX_FILE_STR))
		{
			lynxStrCpy(glpLog->FileName, name);
			lynxCloseLog();
			return LYNX_TRUE;
		}
		return LYNX_FALSE;
	}
	return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxCloseLog(void)
{
	if (glpLog)
		LYNX_CLOSE_FILE(&glpLog->LogFile);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxSetLog(const LYNXCHAR *mod, const LYNXCHAR *log)
{
	#if defined (__iOS__) && defined (__UNICODE__) 
	#else
		if (glpLog && lynxIsValidFile(&glpLog->LogFile))
		{
			FOPEN(glpLog->LogFile.FS.fp, glpLog->FileName, _T("at"));

				if (mod)
				{
					LYNX_FILE_PUTSTR(mod, &glpLog->LogFile); 	
					LYNX_FILE_PUTSTR(_T("::"), &glpLog->LogFile); 	
					LYNX_FILE_PUTSTR(log, &glpLog->LogFile); 	
				}		
				else
					LYNX_FILE_PUTSTR(log, &glpLog->LogFile); 			

			fclose(glpLog->LogFile.FS.fp);
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void    lynxSetLogf(const LYNXCHAR *mod, const LYNXCHAR *fmt, ...)
{   
	#if defined (__iOS__) && defined (__UNICODE__)
	#else
		va_list					argptr;		
				
		if (glpLog && lynxIsValidFile(&glpLog->LogFile))
		{
			FOPEN(glpLog->LogFile.FS.fp, glpLog->FileName, _T("at"));
				
				if (mod)
				{
					LYNX_FILE_PUTSTR(mod, &glpLog->LogFile); 	
					LYNX_FILE_PUTSTR(_T("::"), &glpLog->LogFile); 	
					va_start(argptr, fmt);
					_vftprintf(glpLog->LogFile.FS.fp, fmt, argptr); 			
					va_end(argptr);
				}		
				else
				{
					va_start(argptr, fmt);
					_vftprintf(glpLog->LogFile.FS.fp, fmt, argptr); 			
					va_end(argptr);
				}

			fclose(glpLog->LogFile.FS.fp);
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void    lynxLogDumpTextureManager()
{   
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxLog 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void    lynxLogDumpShaderManager()
{   
}

