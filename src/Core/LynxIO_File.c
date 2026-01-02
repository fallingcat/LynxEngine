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

#include <LynxIO.h>
#include <LynxMem.h>

#ifdef __XBOX__
	static LYNXCHAR					gCurrentDriverLetter[4] = _T("D:");
#elif defined __PS2__
	static LYNXCHAR					gCurrentDriverLetter[16] = _T("host:");
#else
	static LYNXCHAR					gCurrentDriverLetter[4] = _T("");
#endif

#ifdef __FILE_MANAGER__	
	static LYNXFILEMANAGER				gFileManager;
#endif
static LYNXCHAR						gBaseDirectory[128] = _T("");
static LYNXBOOL						gbUseZipFile = FALSE;
static LPLYNXFILE					glpCurrentZipFile = NULL;

#define	FILE_CACHE_SIZE				4000000

#ifndef MAX_PATH
    #ifdef __WIN32__
        #include <sys/limits.h>
        #define MAX_PATH            PATH_MAX
    #else
        #define MAX_PATH            512
    #endif
#endif

#if defined __iOS__
    void lynxiOSDeleteFile(const LYNXCHAR *name);
	void lynxiOSRenameFile(const LYNXCHAR *new_name, const LYNXCHAR *old_name);
    void lynxiOSFindFiles(const LYNXCHAR *name, LPLYNXFINDDATA lpd);
	void lynxiOSCreateFolder(const LYNXCHAR *folder);
#elif defined __ANDROID__
	#include <android/asset_manager.h>
	#include <dirent.h>
	LYNXCHAR* gAndroidInternalPath;
#endif
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO  
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */ 
//-------------------------------------------------------------------------------------------------------
LYNXBOOL ilynxFileBlockCollideProc(const void *elem)
{
	LPLYNXFILEBLOCK			lpBlock;
	
	lpBlock = (LPLYNXFILEBLOCK)elem;

	if (lpBlock->Flag == LYNX_FREE_ENTRY)
		return LYNX_FALSE;
	else
		return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO  
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */ 
//-------------------------------------------------------------------------------------------------------
int ilynxFileBlockSearchCollideProc(const void *elem1, const void *elem2)
{
	LPLYNXFILE			*lpKey;
	LPLYNXFILEBLOCK		lpBlock;

	lpKey = (LPLYNXFILE *)elem1;	
	lpBlock = (LPLYNXFILEBLOCK)elem2;

	if (lpBlock->Flag == LYNX_FREE_ENTRY)
		return -1;
	else
	{
		if (lpBlock->lpFile == (*lpKey))
			return 1;
		else
			return 0;	
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetBaseDirectory(const LYNXCHAR *dir)
{
	lynxSprintf(gBaseDirectory, _T("%s"), dir);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetBaseDirectory(LYNXCHAR *dir)
{
	lynxStrCpy(dir, gBaseDirectory);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetCurrentDriverLetter(const LYNXCHAR *drv)
{
	lynxSprintf(gCurrentDriverLetter, _T("%s:"), drv);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetCurrentDriverLetter(LYNXCHAR *drv)
{
	lynxStrCpy(drv, gCurrentDriverLetter);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetDefaultDriverLetter(void)
{
	#ifdef _XBOX
		lynxSprintf(gCurrentDriverLetter, _T("D:"));
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxInitFileIOSystem(void)
{
	int								i;

	#ifdef __FILE_MANAGER__	
		for (i=0; i<LYNX_MAX_FILE_BLOCK; i++)
		{
			gFileManager.lpFileBlock[i].Flag = LYNX_FREE_ENTRY;
			lynxStrCpy(gFileManager.lpFileBlock[i].FileName, "");
			gFileManager.lpFileBlock[i].lpFile = NULL;		
		}
	#endif
	
	#ifdef _XBOX
		//return XSetFileCacheSize(FILE_CACHE_SIZE);
		return TRUE;
	#elif defined __NUCLEUS__
		#ifdef __SIS_MOD__
			//#ifndef __LIB_IMAGE__
			return MMP_fsinit();
			//#endif
		#endif
	#else
		return TRUE;
	#endif
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxParseFileName(LYNXCHAR *parsedname, const LYNXCHAR *filename)
{	
	LYNXCHAR			FullName[MAX_PATH];
	int					i, ci;	
	
	#ifdef _XBOX
		HANDLE			hFile;

		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];
		}				
		FullName[ci++] = lynxToUnicode(0x00);			
	#elif defined __PS2__
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynStrCat(FullName, gBaseDirectory);
		for (i=0, ci=lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('\\'))
			{
				FullName[ci++] = _T('/');								
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];				
		}				
		FullName[ci++] = lynxToUnicode(0x00);			
	#else
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=(int)lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];
		}				
		FullName[ci++] = lynxToUnicode(0x00);						
	#endif
	lynxStrLower(FullName);	
	lynxStrCpy(parsedname, FullName);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxInitMemStr(LPLYNXMEMSTR lpm)
{
	lpm->Buffer = NULL;
	lpm->lpC = lpm->Buffer;
	lpm->Len = 0;
	lpm->Counter = 0;
	
	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCreateMemStr(LPLYNXMEMSTR lpm, DWORD size)
{
	lpm->Buffer = NULL;
	lpm->Buffer = LYNX_MALLOC(size);
	if (lpm->Buffer == NULL)
		return FALSE;
	lpm->lpC = lpm->Buffer;
	lpm->Len = size;
	lpm->Counter = 0;
	
	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCreateMemStrByArray(LPLYNXMEMSTR lpm, LYNXUCHAR *lpc, DWORD size)
{
	lpm->Buffer = (void *)lpc;	
	lpm->lpC = lpm->Buffer;
	lpm->Len = size;
	lpm->Counter = 0;
	
	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxFreeMemStr(LPLYNXMEMSTR lpm)
{
	if (lpm->Buffer)
		LYNX_FREE(lpm->Buffer);
	lpm->Buffer = lpm->lpC = NULL;
	lpm->Len = 0;
	lpm->Counter = 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxReadFileToMemStrByPointer(LPLYNXFILE lpf, LYNXUCHAR *lpc, DWORD size)
{		
	lpf->Type = LYNX_MEMORY_STR;
	lynxInitMemStr(&lpf->MS);		
	if (!lynxCreateMemStrByArray(&lpf->MS, lpc, size))			
		goto ReadError;	

	return TRUE;

ReadError :		
	return FALSE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadFileToMemStr(LPLYNXMEMSTR lpm, const LYNXCHAR *filename)
{		
	DWORD				Len, RLen;	
	int					i, ci;	
	LYNXFILESTR         fp;

	lynxInitMemStr(lpm);		
	if (!LYNX_OPEN_FILESTR(&fp, filename))
		return FALSE;		
	
	Len = lynxGetFileStrSize(&fp); 
	if (!lynxCreateMemStr(lpm, Len))			
		goto ReadError;	
	lynxSeekFileStr(&fp, 0, SEEK_SET);
				
	LYNX_READ_FILESTR(lpm->Buffer, 1, Len, &fp); 

	LYNX_CLOSE_FILESTR(&fp);    

	return TRUE;

ReadError :
	LYNX_CLOSE_FILESTR(&fp);    
	if (lpm->Buffer)
		LYNX_FREE(lpm->Buffer);

	return FALSE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadMemoryToMemStr(LPLYNXMEMSTR lpm, void *lpc, DWORD size)
{		
	int					i, ci;	
	LYNXFILESTR         fp;

	lynxInitMemStr(lpm);			
	if (!lynxCreateMemStr(lpm, size))			
		return FALSE;
	lynxMemCpy(lpm->Buffer, lpc, size); 	

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxWriteMemStrToFile(const LYNXCHAR *filename, LPLYNXMEMSTR lpm)
{
	FILE				*fp;    
	LYNXCHAR			FullName[MAX_PATH];
	
	#ifdef __iOS__
		return LYNX_FALSE;
	#else
		lynxSprintf(FullName, _T("./%s"), filename);		
		fp = _tfopen(FullName, _T("wb"));	
		if(fp == NULL)
		{
			return FALSE;
		}		
		fwrite(lpm->Buffer, 1, lpm->Len, fp); 

		if (fp)
			fclose(fp);    
	#endif

	return TRUE;	
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxWriteMemStrToFileLowerCase(const LYNXCHAR *filename, LPLYNXMEMSTR lpm)
{
	FILE				*fp;    
	LYNXCHAR				FullName[128];
	
	#ifdef __iOS__
		return LYNX_FALSE;
	#else
		lynxSprintf(FullName, _T(".//%s"), filename);
		lynxStrLower(FullName);
		fp = _tfopen(FullName, _T("wb"));	
		if(fp == NULL)
		{
			return FALSE;
		}		
		fwrite(lpm->Buffer, 1, lpm->Len, fp); 

		if (fp)
			fclose(fp);    
	#endif

	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxReadMemStr(void *lpdst, int size, int len, LPLYNXMEMSTR lpm)
{
	int					Len;

	Len = len*size;
	if (lpm->Len == lpm->Counter)
	{
		Len = 0;		
	}
	else if ((lpm->Len -lpm->Counter) < Len)
	{
		Len = (lpm->Len -lpm->Counter);
		lynxMemCpy(lpdst, lpm->lpC, Len);
		lpm->Counter += Len;
		lpm->lpC += Len;
	}
	else
	{
		lynxMemCpy(lpdst, lpm->lpC, Len);
		lpm->Counter += Len;
		lpm->lpC += Len;
	}
	return len;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxWriteMemStr(const void *lpsrc, int size, int len, LPLYNXMEMSTR lpm)
{
	int					Len;

	Len = len*size;
	lynxMemCpy(lpm->lpC, lpsrc, Len);
	lpm->Counter += Len;
	lpm->lpC += Len;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxSeekMemStr(LPLYNXMEMSTR lpm, long len, int flag)
{
	switch (flag)
	{
		case SEEK_SET:
			lpm->Counter = len;
			lpm->lpC = lpm->Buffer + lpm->Counter;
			break;

		case SEEK_END:
			lpm->Counter = (lpm->Len - len);
			lpm->lpC = lpm->Buffer + lpm->Counter;
			break;

		case SEEK_CUR:
			lpm->lpC += len;
			lpm->Counter += len;
			break;
	}	
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONG lynxTellMemStr(LPLYNXMEMSTR lpm)
{
	return lpm->Counter;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXCHAR *lynxMemStrGetStr(LYNXCHAR *str, int len, LPLYNXMEMSTR lpm)
{	
	LYNXCHAR					Char;
	int						CI, RealSize;

	CI = 0;

	while (1)
	{
		if (lynxReadMemStr(&Char, 1, 1, lpm) == 0)						
			return NULL;
		if (Char != 0xA && Char != 0xD)
		{
			str[CI++] = Char;
		}
		else
		{	
			lynxReadMemStr(&Char, 1, 1, lpm);
			if (Char != 0xA && Char != 0xD)
			{
				lynxSeekMemStr(lpm, -1, SEEK_CUR);					
				continue;
			}
			else
			{
				str[CI++] = lynxToUnicode(0x00);
				return str;
			}
		}
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int lynxMemStrPutStr(const LYNXCHAR *str, LPLYNXMEMSTR lpm)
{
	LYNXCHAR					Char;
	int						CI, RealSize;

	CI = 0;

	while (1)
	{
		if (str[CI])
		{
			lynxWriteMemStr((const void *)(&str[CI++]), 1, 1, lpm);
		}
		else
		{				
			lynxWriteMemStr((const void *)(&str[CI++]), 1, 1, lpm);
			return CI;			
		}
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxInitFileStr(LPLYNXFILESTR lpf)
{
	lpf->fp = NULL;
	
	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxCreateFileStr(LPLYNXFILESTR lpf, DWORD size)
{
	return TRUE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxFreeFileStr(LPLYNXFILESTR lpf)
{
	#ifdef _XBOX	
		if (lpf->fp)
			CloseHandle(lpf->fp);
	#elif defined __PS2__
		if (lpf->fp)
			sceClose(lpf->fp);
	#else
		if (lpf->fp)
			fclose(lpf->fp);			
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadFileToFileStr(LPLYNXFILESTR lpf, const LYNXCHAR *filename)
{	
	LYNXCHAR				FullName[MAX_PATH];
	int					i, ci;	

	lynxParseFileName(FullName, filename);
	#ifdef _XBOX
		HANDLE			hFile;

		lpf->fp = CreateFile(FullName, GENERIC_READ, FILE_SHARE_READ, NULL,
							 OPEN_EXISTING, FILE_ATTRIBUTE_READONLY|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if(lpf->fp == INVALID_HANDLE_VALUE)
		{		
			return FALSE;
		}			
		//SetFilePointer(lpf->fp, 0, NULL, FILE_BEGIN);
		return TRUE;		
	#elif defined __PS2__		
		lpf->fp = sceOpen(FullName, SCE_RDONLY);
		if(lpf->fp < 0)
		{		
			return FALSE;
		}			
		//sceLseek(lpf->fp, 0, SCE_SEEK_SET);
		return TRUE;		
	#elif defined __iOS__		
		return LYNX_FALSE;		
	#else		
		lpf->fp = _tfopen(FullName, _T("rb"));			
		if(lpf->fp == NULL)
		{		
			return FALSE;
		}			
		fseek(lpf->fp, 0, SEEK_SET);	

		return TRUE;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxOpenFileStrForWrite(LPLYNXFILESTR lpf, const LYNXCHAR *filename)
{
	LYNXCHAR				FullName[128];
	int					i, ci;	

	lynxParseFileName(FullName, filename);
	#ifdef _XBOX		
		return TRUE;		
	#elif defined __PS2__				
		return TRUE;	
	#elif defined __iOS__		
		return LYNX_FALSE;		
	#else		
		lpf->fp = _tfopen(FullName, _T("wb"));	
		if(lpf->fp == NULL)
		{				
			return FALSE;
		}			
		fseek(lpf->fp, 0, SEEK_SET);	
		return TRUE;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxOpenTextFileStrForWrite(LPLYNXFILESTR lpf, const LYNXCHAR *filename)
{
	LYNXCHAR				FullName[128];
	int					i, ci;	

	lynxParseFileName(FullName, filename);
	#ifdef _XBOX		
		return TRUE;		
	#elif defined __PS2__				
		return TRUE;	
	#elif defined __iOS__		
		return LYNX_FALSE;		
	#else		
		lpf->fp = _tfopen(FullName, _T("wt"));	
		if(lpf->fp == NULL)
		{				
			return FALSE;
		}			
		fseek(lpf->fp, 0, SEEK_SET);	
		return TRUE;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadTextFileToFileStr(LPLYNXFILESTR lpf, const LYNXCHAR *filename)
{	
	LYNXCHAR				FullName[128];
	int					i, ci;

	#ifdef _XBOX
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{				
			}
			*/
			else
				FullName[ci++] = filename[i];
		}
		FullName[ci++] = lynxToUnicode(0x00);
		lynxStrLower(FullName);

		lpf->fp = CreateFile(FullName, GENERIC_READ, FILE_SHARE_READ, NULL,
							 OPEN_EXISTING, FILE_ATTRIBUTE_READONLY|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if(lpf->fp == INVALID_HANDLE_VALUE)
		{		
			return FALSE;
		}			
		SetFilePointer(lpf->fp, 0, NULL, FILE_BEGIN);				
	#elif defined __PS2__		
	#elif defined __iOS__		
		return LYNX_FALSE;		
	#else
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=(int)lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];
		}				
		FullName[ci++] = lynxToUnicode(0x00);				
		lynxStrLower(FullName);	

		#if defined(_UNICODE) || defined (UNICODE)
			lpf->fp = _tfopen(FullName, _T("rt,ccs=UNICODE"));				
		#else
			lpf->fp = _tfopen(FullName, _T("rt"));	
		#endif
		if(lpf->fp == NULL)
		{
			return FALSE;
		}			
		fseek(lpf->fp, 0, SEEK_SET);		
	#endif	
	
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxReadFileStr(void *lpdst, int size, int len, LPLYNXFILESTR lpf)
{
	#ifdef _XBOX
		int					RealSize;

		ReadFile(lpf->fp, lpdst, size*len, &RealSize, NULL);
		return RealSize;
	#elif defined __PS2__
		int					RealSize;

		RealSize = sceRead(lpf->fp, lpdst, size*len);
		return RealSize;
	#else
        #ifdef __OS_X__
            return (int)fread(lpdst, size, len, lpf->fp);
        #else 
            return (int)fread(lpdst, size, len, lpf->fp);
        #endif	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxWriteFileStr(const void *lpsrc, int size, int len, LPLYNXFILESTR lpf)
{
	#ifdef _XBOX
		int					RealSize;

		WriteFile(lpf->fp, lpsrc, size*len, &RealSize, NULL);
		return RealSize;
	#elif defined __PS2__
		return 0;		
	#else
		return (int)fwrite(lpsrc, size, len, lpf->fp);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxSeekFileStr(LPLYNXFILESTR lpf, long len, int flag)
{
	#ifdef _XBOX
		return SetFilePointer(lpf->fp, len, NULL, flag);
	#elif defined __PS2__		
		return sceLseek(lpf->fp, len, flag);
	#else
		return fseek(lpf->fp, len, flag);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONG lynxTellFileStr(LPLYNXFILESTR lpf)
{
	#ifdef _XBOX
	#elif defined __PS2__		
	#else
		return ftell(lpf->fp);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
DWORD lynxGetFileStrSize(LPLYNXFILESTR lpf)
{
	#ifdef _XBOX	
		DWORD						Len;
		
		Len = GetFileSize(lpf->fp, &Len); 

		return Len;
	#elif defined __PS2__
		DWORD						Len1, Len2;
		
		Len1 = lynxSeekFileStr(lpf, 0, SCE_SEEK_SET);
		Len2 = lynxSeekFileStr(lpf, 0, SCE_SEEK_END);
		lynxSeekFileStr(lpf, 0, SCE_SEEK_SET);
		
		return (Len2-Len1);
	#else		
		DWORD						Len, Offset;

		Offset = ftell(lpf->fp); 
		fseek(lpf->fp, 0, SEEK_END);
		Len = ftell(lpf->fp); 
		fseek(lpf->fp, Offset, SEEK_SET);

		return Len;
		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXCHAR *lynxFileStrGetStr(LYNXCHAR *str, int len, LPLYNXFILESTR lpf)
{
	#ifdef _XBOX
		LYNXCHAR				Char;
		int						CI, RealSize;

		CI = 0;

		while (1)
		{
			ReadFile(lpf->fp, &Char, 1, &RealSize, NULL);
			if (RealSize == 0)
				return NULL;
			if (Char != 0xA && Char != 0xD)
			{
				str[CI++] = Char;
			}
			else
			{	
				ReadFile(lpf->fp, &Char, 1, &RealSize, NULL);
				if (Char != 0xA && Char != 0xD)					
					SetFilePointer(lpf->fp, -1, NULL, FILE_CURRENT);					
				str[CI++] = lynxToUnicode(0x00);
				return str;
			}
			if (CI >= len-1)			
				return NULL;
		}
	#elif defined __PS2__
		return NULL;				
	#else
		return _fgetts(str, len, lpf->fp);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int lynxFileStrPutStr(const LYNXCHAR *str, LPLYNXFILESTR lpf)
{
	#ifdef _XBOX		
		return 0;				
	#elif defined __PS2__
		return 0;				
	#else
		return _fputts(str, lpf->fp);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetCurrentZipFile(LPLYNXFILE lpf)
{	
	glpCurrentZipFile = lpf;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LPLYNXFILE	lynxGetCurrentZipFile(void)
{
	return glpCurrentZipFile;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetUseZipFile(LYNXBOOL b)
{
	gbUseZipFile = b;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxGetUseZipFile(void)
{
	return gbUseZipFile;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitZipFileStr(LPLYNXZIPFILESTR lpzfs)
{
	lynxInitMemStr(&lpzfs->MS);
	lpzfs->Zipfp = NULL;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxOpenZipFileStr(LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *filename)
{
	LYNXCHAR			FullName[128];
	char				AnsiFullName[128];
	int					i, ci;		

	#ifdef _XBOX	
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];
		}				
		FullName[ci++] = lynxToUnicode(0x00);
	#else
		lynxStrCpy(FullName, gCurrentDriverLetter);
		lynxStrCat(FullName, gBaseDirectory);
		for (i=0, ci=(int)lynxStrLen(FullName); i<lynxStrLen(filename); i++)
		{
			if (filename[i] == _T('/'))
			{
				if (filename[i+1] == _T('/'))
				{
					FullName[ci++] = _T('\\');				
					i++;
				}
				else
				{
					FullName[ci++] = _T('\\');				
				}
			}
			/*
			else if (filename[i] == '.' && (i==0 || i==1))
			{								
			}			
			*/
			else
				FullName[ci++] = filename[i];
		}				
		FullName[ci++] = lynxToUnicode(0x00);				
	#endif

	lynxStrLower(FullName);	
	lynxAnsiStr(AnsiFullName, FullName);
    lpzfs->Zipfp = unzOpen(AnsiFullName);
    if (!lpzfs->Zipfp)
		return FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxOpenZipFileStrByPointer(LPLYNXZIPFILESTR lpzfs, LYNXUCHAR *lpd, DWORD size)
{
	/*char				AnsiName[128];
	int					i, ci;		
	
	lynxAnsiStr(AnsiName, lpd);
    lpzfs->Zipfp = unzOpenArray(AnsiName, size);
    if (!lpzfs->Zipfp)
		return FALSE;*/
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxCloseZipFileStr(LPLYNXZIPFILESTR lpzfs)
{
    if (lpzfs->Zipfp)
		unzClose(lpzfs->Zipfp);
    lpzfs->Zipfp = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxCloseZipFileStrCreatedByPointer(LPLYNXZIPFILESTR lpzfs)
{
    //if (lpzfs->Zipfp)
	//	unzCloseArray(lpzfs->Zipfp);
    lpzfs->Zipfp = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadFileToMemStrFromZipFileStr(LPLYNXMEMSTR lpms, LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *name)
{
	DWORD					Len;
	unz_file_info			info;
	LYNXCHAR				FullName[128];	
	char					AnsiStr[128];
	int						i, ci;

    lynxStrCpy(FullName, name);
	for (i=0, ci=0; i<lynxStrLen(name); i++)
	{
		if (name[i] == _T('\\'))
		{
			if (name[i+1] == _T('\\'))
			{
				FullName[ci++] = _T('/');				
				i++;
			}
			else
			{
				FullName[ci++] = _T('/');				
			}
		}
		else if (name[i] == _T('/') && name[i+1] == _T('/'))
		{
			FullName[ci++] = _T('/');							
			i++;
		}
		/*
		else if (name[i] == '.' && (i==0 || i==1))
		{								
		}			
		*/
		else
			FullName[ci++] = name[i];
	}				
	FullName[ci++] = lynxToUnicode(0x00);	
	lynxStrLower(FullName);	
	
	if (FullName[0] == _T('/'))
	{
		lynxAnsiStr(AnsiStr, &FullName[1]);
		if (unzLocateFile(lpzfs->Zipfp, AnsiStr, 2) != UNZ_OK)
			return FALSE;
	}
	else
	{
		lynxAnsiStr(AnsiStr, FullName);
		if (unzLocateFile(lpzfs->Zipfp, AnsiStr, 2) != UNZ_OK)
			return FALSE;
	}	
	
	if (unzOpenCurrentFile(lpzfs->Zipfp) != UNZ_OK)
		return FALSE;   

    if (unzGetCurrentFileInfo(lpzfs->Zipfp, &info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK)
		return FALSE;
		
	Len = info.uncompressed_size;	

	if (!lynxCreateMemStr(lpms, Len))			
		return FALSE;	
	
	Len = unzReadCurrentFile(lpzfs->Zipfp, lpms->Buffer, Len);						

	unzCloseCurrentFile(lpzfs->Zipfp);	

	return TRUE;    
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReadFileToZipFileStr(LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *name)
{
	return lynxReadFileToMemStrFromZipFileStr(&lpzfs->MS, &glpCurrentZipFile->ZFS, name);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxFreeZipFileStr(LPLYNXZIPFILESTR lpzfs)
{
	lynxFreeMemStr(&lpzfs->MS);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitFile(LPLYNXFILE lpf)
{
	lynxInitFileStr(&lpf->FS);
	lynxInitMemStr(&lpf->MS);
	lynxInitMemStr(&lpf->ZFS.MS);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxOpenFile(LPLYNXFILE lpf, const LYNXCHAR *name, int opentype, LYNXFILESTRTYPE type)
{
	LYNXBOOL				b;
	int						i;

	b = FALSE;
	lpf->Type = type;
	if (opentype & LYNX_FOT_READ)
	{
		if (opentype & LYNX_FOT_BINARY)
		{
			switch (type)
			{
				case LYNX_FILE_STR:
					b = lynxReadFileToFileStr(&lpf->FS, name);
					break;

				case LYNX_MEMORY_STR:
					b = lynxReadFileToMemStr(&lpf->MS, name);
					break;

				case LYNX_ZIPFILE_STR:
					b = lynxReadFileToZipFileStr(&lpf->ZFS, name);
					break;
			}	
		}
		else if (opentype & LYNX_FOT_TEXT)
		{
			switch (type)
			{
				case LYNX_FILE_STR:
					b = lynxReadTextFileToFileStr(&lpf->FS, name);
					break;

				case LYNX_MEMORY_STR:					
					break;

				case LYNX_ZIPFILE_STR:					
					break;
			}			
		}
	}
	else if (opentype & LYNX_FOT_WRITE)
	{
		if (opentype & LYNX_FOT_BINARY)
		{	
			switch (type)
			{				
				case LYNX_FILE_STR:
					b = lynxOpenFileStrForWrite(&lpf->FS, name);
					break;

				case LYNX_MEMORY_STR:
					//return lynxReadFileToMemStr(&lpf->MS, name);
					break;

				case LYNX_ZIPFILE_STR:
					//return lynxReadFileToZipFileStr(&lpf->ZFS, name);
					break;
			}
		}
		else if (opentype & LYNX_FOT_TEXT)
		{
			switch (type)
			{				
				case LYNX_FILE_STR:
					b = lynxOpenTextFileStrForWrite(&lpf->FS, name);
					break;

				case LYNX_MEMORY_STR:
					//return lynxReadFileToMemStr(&lpf->MS, name);
					break;

				case LYNX_ZIPFILE_STR:
					//return lynxReadFileToZipFileStr(&lpf->ZFS, name);
					break;
			}
		}
	}
	#ifdef __FILE_MANAGER__	
		if (b)
		{
			i = lynxSimpleHash((long)lpf, gFileManager.lpFileBlock, LYNX_MAX_FILE_BLOCK, sizeof(LYNXFILEBLOCK), ilynxFileBlockCollideProc);		
			if (i >= 0)
			{
				gFileManager.lpFileBlock[i].lpFile = lpf;
				lynxStrCpy(gFileManager.lpFileBlock[i].FileName, name);
				gFileManager.lpFileBlock[i].Flag = LYNX_USED_ENTRY;			
			}
			else
			{
				lynxSetLogf("Warning ", "File manager can't find any free block...\n");			
			}
		}
	#endif
	return b;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void		lynxCloseFile(LPLYNXFILE lpf)
{
	int								i;

	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			lynxFreeFileStr(&lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			lynxFreeMemStr(&lpf->MS);
			break;

		case LYNX_ZIPFILE_STR:
			lynxFreeZipFileStr(&lpf->ZFS);
			break;
	}

	#ifdef __FILE_MANAGER__		
		i = lynxSimpleHashSerach((long)lpf, gFileManager.lpFileBlock, LYNX_MAX_FILE_BLOCK, sizeof(LYNXFILEBLOCK), ilynxFileBlockSearchCollideProc);		
		if (i >=0)
		{
			gFileManager.lpFileBlock[i].lpFile = NULL;
			lynxStrCpy(gFileManager.lpFileBlock[i].FileName, "");
			gFileManager.lpFileBlock[i].Flag = LYNX_FREE_ENTRY;		
		}
		else
		{
			lynxSetLogf("Warning ", "File manager can't find opened file(%0x)...\n", lpf);			
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL		lynxIsValidFile(LPLYNXFILE lpf)
{
	int								i;

	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			if (lpf->FS.fp)
				return LYNX_TRUE;
			else
				return LYNX_FALSE;
			break;

		case LYNX_MEMORY_STR:
			if (lpf->MS.Buffer)
				return LYNX_TRUE;
			else
				return LYNX_FALSE;
			break;

		case LYNX_ZIPFILE_STR:
			if (lpf->ZFS.MS.Buffer)
				return LYNX_TRUE;
			else
				return LYNX_FALSE;
			break;
	}

	return LYNX_FALSE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxReadFile(void *lpdst, int size, int len, LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxReadFileStr(lpdst, size, len, &lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			return lynxReadMemStr(lpdst, size, len, &lpf->MS);
			break;

		case LYNX_ZIPFILE_STR:			
			return lynxReadMemStr(lpdst, size, len, &lpf->ZFS.MS);
			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
int lynxWriteFile(const void *lpsrc, int size, int len, LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxWriteFileStr(lpsrc, size, len, &lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			//return lynxReadMemStr(lpsrc, size, len, &lpf->MS);
			break;

		case LYNX_ZIPFILE_STR:			
			//return lynxReadMemStr(lpsrc, size, len, &lpf->ZFS.MS);
			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 DWORD		lynxGetFileSize(LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxGetFileStrSize(&lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			return (lpf->MS.Len);
			break;

		case LYNX_ZIPFILE_STR:			
			return (lpf->ZFS.MS.Len);
			break;
	}	
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxSeekFile(LPLYNXFILE lpf, long len, int flag)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxSeekFileStr(&lpf->FS, len, flag);
			break;

		case LYNX_MEMORY_STR:
			return lynxSeekMemStr(&lpf->MS, len, flag);
			break;

		case LYNX_ZIPFILE_STR:
			return lynxSeekMemStr(&lpf->ZFS.MS, len, flag);
			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONG lynxTellFile(LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxTellFileStr(&lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			return lynxTellMemStr(&lpf->MS);
			break;

		case LYNX_ZIPFILE_STR:
			return lynxTellMemStr(&lpf->ZFS.MS);
			break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXCHAR *lynxFileGetStr(LYNXCHAR *str, int len, LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxFileStrGetStr(str, len, &lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			return lynxMemStrGetStr(str, len, &lpf->MS);
			break;

		case LYNX_ZIPFILE_STR:
			return lynxMemStrGetStr(str, len, &lpf->ZFS.MS);
			break;

		default:
			return NULL;
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 int lynxFilePutStr(const LYNXCHAR *str, LPLYNXFILE lpf)
{
	switch (lpf->Type)
	{
		case LYNX_FILE_STR:
			return lynxFileStrPutStr(str, &lpf->FS);
			break;

		case LYNX_MEMORY_STR:
			return lynxMemStrPutStr(str, &lpf->MS);			
			break;

		case LYNX_ZIPFILE_STR:
			//return lynxMemStrPutStr(str, &lpf->ZFS.MS);
			return 0;
			break;

		default:
			return 0;
			break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxClearBadFileBlock(void)
{
	#ifdef __FILE_MANAGER__	
		int							i;

		for (i=0; i<LYNX_MAX_FILE_BLOCK; i++)
		{
			if (gFileManager.lpFileBlock[i].Flag == LYNX_USED_ENTRY || gFileManager.lpFileBlock[i].lpFile != NULL)
			{
				lynxSetLogf("Warning!! ", "File - (%s) didn't close...\n", gFileManager.lpFileBlock[i].FileName);					
				//lynxCloseFile(gFileManager.lpFileBlock[i].lpFile);
			}
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
LYNXLONG lynxFileModifyTime(LYNXCHAR* filename)
{
	#if defined (__iOS__)
		return 0;
	#elif defined (__ANDROID__)
		return 0;
	#else
		struct _stat	Buffer;
		int				Result;

		// Get data associated with "crt_stat.c": 
		Result = _tstat(filename, &Buffer);

		return Buffer.st_mtime;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxDeleteFile(const LYNXCHAR *file)
{
	#ifdef _XBOX
		DeleteFile(file);
	#elif defined __WINCE__		
	#elif defined (__iOS__)
        lynxiOSDeleteFile(file);
	#elif defined (__ANDROID__)
		char f[256];
		char* sl = "/";
		strncpy(f, gAndroidInternalPath, sizeof(f));
		strncat(f, sl, sizeof(f));

		if (file[0] == _T('.') && file[1] == _T('.') && file[2] == _T('/'))
			strncat(f, &file[3], sizeof(f));
		else if (file[0] == _T('.') && file[1] == _T('/'))
			strncat(f, &file[2], sizeof(f));
		else
			strncat(f, file, sizeof(f));

		remove(f);
	#else
		_tunlink(file);
	#endif
	/*
	#ifdef __WIN32__
		DeleteFile(file);
	#elif defined __LINUX__	
	#endif	
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRenameFile(const LYNXCHAR *new_file, const LYNXCHAR *old_file)
{
	#ifdef _XBOX		
	#elif defined __WINCE__		
	#elif defined (__iOS__)
        lynxiOSRenameFile(new_file, old_file);
	#elif defined (__ANDROID__)
		char n[256], o[256];
		char* sl = "/";
		strncpy(n, gAndroidInternalPath, sizeof(n));
		strncpy(o, gAndroidInternalPath, sizeof(o));
		strncat(n, sl, sizeof(n));
		strncat(o, sl, sizeof(n));

		if (new_file[0] == _T('.') && new_file[1] == _T('.') && new_file[2] == _T('/'))
			strncat(n, &new_file[3], sizeof(n));
		else if (new_file[0] == _T('.') && new_file[1] == _T('/'))
			strncat(n, &new_file[2], sizeof(n));
		else
			strncat(n, new_file, sizeof(n));

		if (old_file[0] == _T('.') && old_file[1] == _T('.') && old_file[2] == _T('/'))
			strncat(o, &old_file[3], sizeof(o));
		else if (old_file[0] == _T('.') && old_file[1] == _T('/'))
			strncat(o, &old_file[2], sizeof(o));
		else
			strncat(o, old_file, sizeof(n));
		rename(o, n);
	#else
		_trename(old_file, new_file);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitFindData(LPLYNXFINDDATA lpd)
{
	lpd->NumFounds = 0;	
	lpd->lpFounds = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL lynxFindFile(const LYNXCHAR *name)
 {
	 LYNXFINDDATA FindData;
	 LYNXBOOL bFound;

	 lynxInitFindData(&FindData);
	 lynxFindFiles(name, &FindData);
	 bFound = (FindData.NumFounds > 0);
	 lynxFreeFounds(&FindData);

	 return bFound;
 }
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
 void lynxFindFiles(const LYNXCHAR *name, LPLYNXFINDDATA lpd)
{	
	#ifdef __WIN32__
		struct _tfinddata_t		Fold;
		long					hFold;
		int						Len, i, ci;
		LYNXCHAR				Buffer[MAX_PATH], *lpStr, Name[32];	
		int						err;
		char					szCurrentFileName[MAX_PATH];
		LYNXCHAR				TCurrentFileName[MAX_PATH];

		if (gbUseZipFile)
		{
			lynxStrCpy(Buffer, name);
			for (i=0, ci=0; i<lynxStrLen(name); i++)
			{
				if (name[i] == _T('\\'))
				{
					if (name[i+1] == _T('\\'))
					{
						Buffer[ci++] = _T('/');				
						i++;
					}
					else
					{
						Buffer[ci++] = _T('/');				
					}
				}
				else if (name[i] == _T('/') && name[i+1] == _T('/'))
				{
					Buffer[ci++] = _T('/');							
					i++;
				}
				/*
				else if (name[i] == '.' && (i==0 || i==1))
				{								
				}							
				*/
				else
					Buffer[ci++] = name[i];
			}				
			Buffer[ci++] = lynxToUnicode(0x00);
		}
		else
		{
			#ifdef _XBOX
				lynxStrCpy(Buffer, gCurrentDriverLetter);
				lynxStrCat(Buffer, gBaseDirectory);
				for (i=0, ci=lynxStrLen(Buffer); i<lynxStrLen(name); i++)
				{
					if (name[i] == _T('/'))
					{
						if (name[i+1] == _T('/'))
						{
							Buffer[ci++] = _T('\\');				
							i++;
						}
						else
						{
							Buffer[ci++] = _T('\\');				
						}
					}
					/*
					else if (name[i] == '.' && (i==0 || i==1))
					{				
					}
					*/
					else
						Buffer[ci++] = name[i];
				}
				Buffer[ci++] = lynxToUnicode(0x00);
			#else
				lynxStrCpy(Buffer, name);			
			#endif
		}
		lynxStrLower(Buffer);
		if (Buffer[0] == _T('/'))
			lpStr = &Buffer[1];
		else
			lpStr = Buffer;

		if (gbUseZipFile)
		{
			lpd->NumFounds = 0;
			if (lpd->lpFounds)
				lynxFreeFounds(lpd);

			err = unzGoToFirstFile(glpCurrentZipFile->ZFS.Zipfp);
			while (err == UNZ_OK)
			{				
				unzGetCurrentFileInfo(glpCurrentZipFile->ZFS.Zipfp, NULL, szCurrentFileName, 127, NULL, 0, NULL, 0);				
				lynxFromAnsiStr(TCurrentFileName, szCurrentFileName);
				if (lynxFindStrWild(TCurrentFileName, lpStr))
				{
					lpd->NumFounds++;
				}
				err = unzGoToNextFile(glpCurrentZipFile->ZFS.Zipfp);
			}
			lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
			lpd->NumFounds = 0;
			err = unzGoToFirstFile(glpCurrentZipFile->ZFS.Zipfp);
			while (err == UNZ_OK)
			{				
				unzGetCurrentFileInfo(glpCurrentZipFile->ZFS.Zipfp, NULL, szCurrentFileName, 127, NULL, 0, NULL, 0);				
				lynxFromAnsiStr(TCurrentFileName, szCurrentFileName);
				if (lynxFindStrWild(TCurrentFileName, lpStr))
				{
					lynxGetNameByPath(Name, TCurrentFileName);
					Len = (int)lynxStrLen(Name);
					lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(Len+1);
					lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Name);
					lpd->NumFounds++;
				}
				err = unzGoToNextFile(glpCurrentZipFile->ZFS.Zipfp);
			}	
		}
		else
		{
			lpd->NumFounds = 0;
			if (lpd->lpFounds)
				lynxFreeFounds(lpd);
			hFold = _tfindfirst(Buffer, &Fold);
			if (hFold >= 0)
			{
				if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))		
				{
					if (!(Fold.attrib & _A_SUBDIR))										
					{
						lpd->NumFounds++;
					}
				}
				while( _tfindnext(hFold, &Fold ) == 0)//晹ぃ
				{		
					if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
					{
						if (!(Fold.attrib & _A_SUBDIR))										
						{
							lpd->NumFounds++;
						}
					}
				}			
				_findclose(hFold);

				lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
				lpd->NumFounds = 0;
				hFold = _tfindfirst(Buffer, &Fold);
				if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
				{
					if (!(Fold.attrib & _A_SUBDIR))										
					{
						Len = (int)lynxStrLen(Fold.name);
						lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
						lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Fold.name);
						lpd->NumFounds++;
					}
				}
				while ( _tfindnext(hFold, &Fold ) == 0)//晹ぃ
				{		
					if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
					{
						if (!(Fold.attrib & _A_SUBDIR))										
						{							
							Len = (int)lynxStrLen(Fold.name);
							lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
							lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Fold.name);
							lpd->NumFounds++;
						}
					}
				}
				_findclose(hFold);
			}
		}
	#elif defined __LINUX__
		glob_t			Result;
		int				i, j, Len;
		LYNXCHAR		Buffer[128];	

		lynxStrCpy(Buffer, name);
		lynxStrLower(Buffer);
		glob(Buffer, 0, NULL, &Result);
		lpd->NumFounds = Result.gl_pathc;
		lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
		for (i=0; i<Result.gl_pathc; i++)
		{       			
			Len = lynxStrLen(Result.gl_pathv[i]);
			for (j=Len-1; j >=0; j--)
			{
				if (Result.gl_pathv[i][j] == _T('/'))
					break;
			}
			lpd->lpFounds[i] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
			//lynxStrCpy(lpd->lpFounds[i], Result.gl_pathv[i]);
			lynxStrCpy(lpd->lpFounds[i], &Result.gl_pathv[i][j+1]);
		}		
		globfree(&Result);
    #elif defined __iOS__
        lynxiOSFindFiles(name, lpd);
	#elif defined __ANDROID__
		/*LYNXCHAR Path[256], Name[128], Ext[8], DataPath[256];
		lynxSeparatePathFilenameExtByFullPath(Path, Name, Ext, name);

		DIR *dp;
		struct dirent *ep;

		dp = opendir ("./");
		if (dp != NULL)
		{
			while (ep = readdir (dp))
				puts (ep->d_name);
			(void) closedir (dp);
		}*/
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxCreateFolder(const LYNXCHAR *folder)
{
	#if defined (__iOS__)
		lynxiOSCreateFolder(folder);
	#elif defined (__ANDROID__)
	#else
		LYNXCHAR *Folder, CurrentPath[256], FullPath[256];

		lynxStrCpy(CurrentPath, _T(""));
		lynxStrCpy(FullPath, folder);
		Folder = lynxStrTok(FullPath, _T("/"));				
		while (Folder)
		{
			lynxStrCat(CurrentPath, Folder);
			_tmkdir(CurrentPath);					
			lynxStrCat(CurrentPath, _T("/"));
			Folder = lynxStrTok(NULL, _T("/"));
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxFindFolders(const LYNXCHAR *name, LPLYNXFINDDATA lpd)
{	
	#ifdef __WIN32__
		struct _tfinddata_t		Fold;
		long					hFold;
		int						Len, i, ci;
		LYNXCHAR				Buffer[MAX_PATH], *lpStr, Name[MAX_PATH];
		int						err;
		char					szCurrentFileName[MAX_PATH];
		LYNXCHAR				TCurrentFileName[MAX_PATH];

		if (gbUseZipFile)
		{
			lynxStrCpy(Buffer, name);
			for (i=0, ci=0; i<lynxStrLen(name); i++)
			{
				if (name[i] == _T('\\'))
				{
					if (name[i+1] == _T('\\'))
					{
						Buffer[ci++] = _T('/');				
						i++;
					}
					else
					{
						Buffer[ci++] = _T('/');				
					}
				}
				else if (name[i] == _T('/') && name[i+1] == _T('/'))
				{
					Buffer[ci++] = _T('/');							
					i++;
				}
				/*
				else if (name[i] == '.' && (i==0 || i==1))
				{								
				}							
				*/
				else
					Buffer[ci++] = name[i];
			}				
			Buffer[ci++] = lynxToUnicode(0x00);
		}
		else
		{
			#ifdef _XBOX
				lynxStrCpy(Buffer, gCurrentDriverLetter);
				lynxStrCat(Buffer, gBaseDirectory);
				for (i=0, ci=lynxStrLen(Buffer); i<lynxStrLen(name); i++)
				{
					if (name[i] == _T('/'))
					{
						if (name[i+1] == _T('/'))
						{
							Buffer[ci++] = _T('\\');				
							i++;
						}
						else
						{
							Buffer[ci++] = _T('\\');				
						}
					}
					/*
					else if (name[i] == '.' && (i==0 || i==1))
					{				
					}
					*/
					else
						Buffer[ci++] = name[i];
				}
				Buffer[ci++] = lynxToUnicode(0x00);
			#else
				lynxStrCpy(Buffer, name);			
			#endif
		}
		lynxStrLower(Buffer);
		if (Buffer[0] == _T('/'))
			lpStr = &Buffer[1];
		else
			lpStr = Buffer;

		if (gbUseZipFile)
		{
			lpd->NumFounds = 0;
			if (lpd->lpFounds)
				lynxFreeFounds(lpd);

			err = unzGoToFirstFile(glpCurrentZipFile->ZFS.Zipfp);
			while (err == UNZ_OK)
			{				
				unzGetCurrentFileInfo(glpCurrentZipFile->ZFS.Zipfp, NULL, szCurrentFileName, 127, NULL, 0, NULL, 0);				
				lynxFromAnsiStr(TCurrentFileName, szCurrentFileName);
				if (lynxFindStrWild(TCurrentFileName, lpStr))
				{
					lpd->NumFounds++;
				}
				err = unzGoToNextFile(glpCurrentZipFile->ZFS.Zipfp);
			}
			lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
			lpd->NumFounds = 0;
			err = unzGoToFirstFile(glpCurrentZipFile->ZFS.Zipfp);
			while (err == UNZ_OK)
			{				
				unzGetCurrentFileInfo(glpCurrentZipFile->ZFS.Zipfp, NULL, szCurrentFileName, 127, NULL, 0, NULL, 0);				
				lynxFromAnsiStr(TCurrentFileName, szCurrentFileName);
				if (lynxFindStrWild(TCurrentFileName, lpStr))
				{
					lynxGetNameByPath(Name, TCurrentFileName);
					Len = (int)lynxStrLen(Name);
					lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(Len+1);
					lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Name);
					lpd->NumFounds++;
				}
				err = unzGoToNextFile(glpCurrentZipFile->ZFS.Zipfp);
			}	
		}
		else
		{
			lpd->NumFounds = 0;
			if (lpd->lpFounds)
				lynxFreeFounds(lpd);
			hFold = _tfindfirst(Buffer, &Fold);
			if (hFold >= 0)
			{
				if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))		
				{
					if (Fold.attrib & _A_SUBDIR)										
					{
						lpd->NumFounds++;
					}
				}
				while( _tfindnext(hFold, &Fold ) == 0)//晹ぃ
				{		
					if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
					{
						if (Fold.attrib & _A_SUBDIR)										
						{
							lpd->NumFounds++;
						}
					}
				}		
				_findclose(hFold);

				lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
				lpd->NumFounds = 0;
				hFold = _tfindfirst(Buffer, &Fold);
				if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
				{
					if (Fold.attrib & _A_SUBDIR)															
					{
						Len = (int)lynxStrLen(Fold.name);
						lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
						lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Fold.name);
						lpd->NumFounds++;
					}
				}
				while ( _tfindnext(hFold, &Fold ) == 0)//晹ぃ
				{		
					if (lynxStrCmp(Fold.name, _T(".")) && lynxStrCmp(Fold.name, _T("..")))
					{
						if (Fold.attrib & _A_SUBDIR)										
						{
							Len = (int)lynxStrLen(Fold.name);
							lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
							lynxStrCpy(lpd->lpFounds[lpd->NumFounds], Fold.name);
							lpd->NumFounds++;
						}
					}
				}
				_findclose(hFold);
			}
		}
	#elif defined __LINUX__
		glob_t			Result;
		int				i, j, Len;
		LYNXCHAR		Buffer[128];	

		lynxStrCpy(Buffer, name);
		lynxStrLower(Buffer);
		glob(Buffer, 0, NULL, &Result);
		lpd->NumFounds = Result.gl_pathc;
		lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
		for (i=0; i<Result.gl_pathc; i++)
		{       			
			Len = lynxStrLen(Result.gl_pathv[i]);
			for (j=Len-1; j >=0; j--)
			{
				if (Result.gl_pathv[i][j] == _T('/'))
					break;
			}
			lpd->lpFounds[i] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
			//lynxStrCpy(lpd->lpFounds[i], Result.gl_pathv[i]);
			lynxStrCpy(lpd->lpFounds[i], &Result.gl_pathv[i][j+1]);
		}		
		globfree(&Result);
    #endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxFreeFounds(LPLYNXFINDDATA lpd)
{	
	int							i;

	for (i=0; i<lpd->NumFounds; i++)
	{
		LYNX_FREE(lpd->lpFounds[i]);		
	}
	LYNX_FREE(lpd->lpFounds);		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief C
 *
 *	@param C
 *	@param C
 *	@param C
 *	@returnC 
 */
//-------------------------------------------------------------------------------------------------------
void lynxCopyFile(const LYNXCHAR *dest, const LYNXCHAR *src)
{
	LYNXMEMSTR MemStr;

	lynxInitMemStr(&MemStr);
	lynxReadFileToMemStr(&MemStr, src);
	lynxWriteMemStrToFile(dest, &MemStr);
	lynxFreeMemStr(&MemStr);
}

