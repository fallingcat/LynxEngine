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

#if AVOID_STRCPY_DURING_TOKEN_PROCESS
	static LYNXCHAR* LastTokAddress = NULL;
	static LYNXBOOL bCopy = LYNX_FALSE;
#endif

#ifdef __iOS__
    void            lynxiOSLog(const LYNXCHAR* str);
    int             lynxiOSAtoi(const LYNXCHAR* str);
    long            lynxiOSAtol(const LYNXCHAR* str);
    unsigned long   lynxiOSAtoul(const LYNXCHAR* str);
    float           lynxiOSAtof(const LYNXCHAR* str);
    void            lynxiOSStrLower(LYNXCHAR *str);
#endif
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxDebugOutputPrintf(const LYNXCHAR *fmt, ...)
{   
	#if 1//(defined(_DEBUG) || defined(DEBUG))
		#ifdef __WINCE__
			va_list					argptr;		
			char					Buffer[256];
			wchar_t					Str[256];
				
			va_start(argptr,fmt);
			vsprintf(Buffer,fmt,argptr);
			va_end(argptr);
			mbstowcs(Str, Buffer, 256);
			NKDbgPrintfW(Str);			
		#elif defined __WIN32__	
			va_list					argptr;		
			LYNXCHAR				Buffer[256];
				
			va_start(argptr,fmt);
			lynxVsnPrintf(Buffer,256,fmt,argptr);
			va_end(argptr);

			OutputDebugString(Buffer);
		#elif defined __NUCLEUS__			
			va_list					argptr;		
			char					Buffer[128];
				
			va_start(argptr,fmt);
			vsprintf(Buffer,fmt,argptr);
			va_end(argptr);

			mstd_printf(Buffer);								
		#elif defined __PS2__
			va_list					argptr;		
			char					Buffer[256];
				
			va_start(argptr,fmt);
			vsprintf(Buffer,fmt, argptr);
			va_end(argptr);

			printf(Buffer);
        #elif defined __iOS__
            va_list					argptr;		
            LYNXCHAR				Buffer[256];
    
            va_start(argptr,fmt);
            lynxVsnPrintf(Buffer,256,fmt,argptr);
            va_end(argptr);

            lynxiOSLog(Buffer);
		#elif defined __ANDROID__
			va_list					argptr;		
            LYNXCHAR				Buffer[256];
    
            va_start(argptr,fmt);
            lynxVsnPrintf(Buffer,256,fmt,argptr);
            va_end(argptr);

			__android_log_print(ANDROID_LOG_INFO, "LynxEngine", Buffer);
		#endif	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSprintf(LYNXCHAR *buffer, const LYNXCHAR *fmt, ...)
{   
	va_list					argptr;		

	va_start(argptr,fmt);
	//_vstprintf(buffer,fmt, argptr);
	lynxVsnPrintf(buffer, 256, fmt, argptr);
	va_end(argptr);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxVsnPrintf(LYNXCHAR *buffer, size_t max, const LYNXCHAR *fmt, va_list argptr)
{
	#if defined (__GNUC__) && defined (_UNICODE)
		_vstprintf(buffer, max, fmt, argptr);
	#else
		_vsntprintf(buffer, max, fmt, argptr);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxUniStr(LYNXUNICHAR *lpustr, const LYNXCHAR *lpstr)
{
	DWORD		Len, i;

	Len = (DWORD)_tcslen(lpstr);
	for (i=0; i<Len; i++)
		lpustr[i] = lynxToUnicode(lpstr[i]);
	lpustr[i] = lynxToUnicode(0x00);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxAnsiStr(LYNXANSICHAR *lpastr, const LYNXCHAR *lpstr)
{
	DWORD		Len, i;

	Len = lynxStrLen(lpstr);
	for (i=0; i<Len; i++)
		lpastr[i] = lynxToAnsi(lpstr[i]);
	lpastr[i] = 0x00;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxFromUniStr(LYNXCHAR *lpstr, const LYNXUNICHAR *lpustr)
{	
	DWORD		Len, i;

	#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
		Len = 0;
		while (lpustr[Len] != 0x00)
		{
			Len++;
		}
	#else
		Len = (DWORD)wcslen(lpustr);
	#endif
	for (i=0; i<Len; i++)
		lpstr[i] = lynxFromUnicode(lpustr[i]);
	lpstr[i] = 0x00;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxFromAnsiStr(LYNXCHAR *lpstr, const LYNXANSICHAR *lpastr)
{
	DWORD		Len, i;

	Len = (DWORD)strlen(lpastr);
	for (i=0; i<Len; i++)
		lpstr[i] = lynxFromAnsi(lpastr[i]);
	lpstr[i] = 0x00;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxAtoi(const LYNXCHAR *lpstr)
{
	#if defined (__GNUC__) && defined (_UNICODE)
		return lynxiOSAtoi(lpstr);
	#else
		return _ttoi(lpstr);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
long lynxAtol(const LYNXCHAR *lpstr)
{
    #if defined (__iOS__)
        return lynxiOSAtol(lpstr);
	#elif defined (__ANDROID__)
		return strtol(lpstr, NULL, 0);
    #else
        return _ttol(lpstr);
    #endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
unsigned long lynxAtoul(const LYNXCHAR *lpstr)
{
    #if defined (__iOS__)
        return lynxiOSAtoul(lpstr);
	#elif defined (__ANDROID__)
		return strtoul(lpstr, NULL, 0);
    #else
        return _tcstoul(lpstr, NULL, 10);
    #endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
LYNXULONGLONG lynxAtoull(const LYNXCHAR *lpstr)
{
    #if defined (__iOS__)
        return lynxiOSAtoul(lpstr);
	#elif defined (__ANDROID__)
		return strtoull(lpstr, NULL, 0);
    #else
        return _tcstoui64(lpstr, NULL, 10);
    #endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief Converts a hexadecimal string to integer。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
unsigned int lynxXtoi(const LYNXCHAR* xs)
{
	unsigned int Result = 0;
	char HexString[256];
	int i, xv, fact;
	int szlen;

	lynxAnsiStr(HexString, xs);
	szlen = strlen(HexString);	
	if (szlen > 0)
	{
		// Converting more than 32bit hexadecimal value?
		if (szlen>8) return 2; // exit

		// Begin conversion here
		Result = 0;
		fact = 1;

		// Run until no more character to convert
		for(i=szlen-1; i>=0 ;i--)
		{
			if (isxdigit(*(HexString+i)))
			{
				if (*(HexString+i)>=97)
				{
					xv = ( *(HexString+i) - 97) + 10;
				}
				else if ( *(HexString+i) >= 65)
				{
					xv = (*(HexString+i) - 65) + 10;
				}
				else
				{
					xv = *(HexString+i) - 48;
				}
				Result += (xv * fact);
				fact *= 16;
			}
			else
			{
				// Conversion was abnormally terminated
				// by non hexadecimal digit, hence
				// returning only the converted with
				// an error value 4 (illegal hex character)
				return Result;
			}
		}
	}	
	return Result;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
float lynxAtof(const LYNXCHAR *lpstr)
{
	#if defined (__GNUC__) && defined (_UNICODE)
		return lynxiOSAtof(lpstr);
	#else	
		#ifdef	_UNICODE
			return _wtof(lpstr);
		#else
			return atof(lpstr);
		#endif	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxStriCmp(const LYNXCHAR *str1, const LYNXCHAR *str2)
{
	int					i, L1, L2, Rest;
	
	L1 = (DWORD)_tcslen(str1);
	L2 = (DWORD)_tcslen(str2);	

	#ifdef __WIN32__
		Rest = _tcsicmp(str1, str2);	
	#else
        Rest = 0;
		for (i=0; i<L1; i++)
		{
			if (str1[i] > str2[i])
			{
				if (str1[i] >= 0x41 && str1[i] <= 0x5a)
				{
					if (str1[i]+0x20 == str2[i])
						continue;					
				}
				else if (str1[i] >= 0x61 && str1[i] <= 0x7a)
				{
					if (str1[i]-0x20 == str2[i])
						continue;					
				}
				Rest = 1;
                break;
			}
			else if (str1[i] < str2[i])
			{
				if (str1[i] >= 0x41 && str1[i] <= 0x5a)
				{
					if (str1[i]+0x20 == str2[i])
						continue;					
				}
				else if (str1[i] >= 0x61 && str1[i] <= 0x7a)
				{
					if (str1[i]-0x20 == str2[i])
						continue;					
				}
				Rest = -1;
                break;
			}
		}
	#endif

	if (Rest)
		return Rest;
	else
	{
		if (L1 > L2)
			return 1;
		else if (L2 > L1)
			return -1;
		else
			return 0;
	}
	/*
	char				*S1;//[128];
	char				*S2;//[128];
	int					Go, i;
	
	
	Go = 0;
	i = 0;
	S1 = (char *)LYNX_MALLOC(strlen(str1) + 1);
	S2 = (char *)LYNX_MALLOC(strlen(str2) + 1);
	strcpy(S1, str1);
	strcpy(S2, str2);
	while (S1[i] && S2[i] && !Go)
	{
		if (S1[i] >= 0x41 && S1[i] <= 0x5a) 
			S1[i] += 0x20;
		if (S2[i] >= 0x41 && S2[i] <= 0x5a) 
			S2[i] += 0x20;

		if (S1[i] == S2[i])
			Go = 0;
		else if (S1[i] > S2[i])
			Go = 1;
		else
			Go = -1;

		i++;
	}
	LYNX_FREE(S1);
	LYNX_FREE(S2);

	return Go;
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxStrCmp(const LYNXCHAR *str1, const LYNXCHAR *str2)
{
	int					L1, L2, Rest;
	
	L1 = (DWORD)_tcslen(str1);
	L2 = (DWORD)_tcslen(str2);

	Rest = _tcscmp(str1, str2);

	if (Rest)
		return Rest;
	else
	{
		if (L1 > L2)
			return 1;
		else if (L2 > L1)
			return -1;
		else
			return 0;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int	lynxStrnCmp(const LYNXCHAR *str1, const LYNXCHAR *str2, const int max)
{
	return _tcsncmp(str1, str2, max);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxStrStr(const LYNXCHAR* string, const LYNXCHAR* strCharSet)
{
	return _tcsstr(string, strCharSet);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxStriStr(const LYNXCHAR* string, const LYNXCHAR* strCharSet)
{
	LYNXCHAR	*Str, *Set, *Result;
	int			Len;		

	Len = lynxStrLen(string)*sizeof(LYNXCHAR);
	Str = LYNX_MALLOC(Len + 2);
	lynxStrCpy(Str, string);
	Len = lynxStrLen(strCharSet)*sizeof(LYNXCHAR);
	Set = LYNX_MALLOC(Len + 2);
	lynxStrCpy(Set, strCharSet);

	lynxStrLower(Str);
	lynxStrLower(Set);

	Result = _tcsstr(Str, Set);

	LYNX_FREE(Str);
	LYNX_FREE(Set);

	return Result;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxStrCpy(LYNXCHAR *str1, const LYNXCHAR *str2)
{
	#if AVOID_STRCPY_DURING_TOKEN_PROCESS
		if (str1 == LastTokAddress)
		{
			bCopy = LYNX_TRUE;
		}
	#endif

	return _tcscpy(str1, str2);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxStrCat(LYNXCHAR *str1, const LYNXCHAR *str2)
{
	#if AVOID_STRCPY_DURING_TOKEN_PROCESS
		if (str1 == LastTokAddress)
		{
			bCopy = LYNX_TRUE;
		}
	#endif

	return _tcscat(str1, str2);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXCHAR* lynxStrTok(LYNXCHAR* str, const LYNXCHAR *delimiters)
{
	LYNXCHAR* Status;
    
    static LYNXCHAR Str[512];
    static int StringIndex = 0;
    int CurrentIndex;
	int Index, i;

	#if AVOID_STRCPY_DURING_TOKEN_PROCESS
		if (str)
		{
			LastTokAddress = str;
			bCopy = LYNX_FALSE;
		}
		else
		{
			if (bCopy)
			{
				int xxx = 0;
			}
		}
	#endif

	#if (defined __GNUC__ && defined _UNICODE)
	    if (str)
		{
			StringIndex = 0;
            lynxStrCpy(Str, str);
		}

        CurrentIndex = 0;
        while (Str[StringIndex + CurrentIndex])
        {
			for (i=0; i<lynxStrLen(delimiters); i++)
			{
				if (delimiters[i] == Str[StringIndex + CurrentIndex])
				{
					Str[StringIndex + CurrentIndex] = 0x00;
					Index = StringIndex;
					StringIndex = StringIndex + CurrentIndex + 1;
					return &Str[Index];
				}
			}
            CurrentIndex++;
        }        

		if (str)
			return NULL;
		else
			return &Str[StringIndex];
    	//return _tcstok(str, delimiters, &Status);
	#else	
		return _tcstok(str, delimiters);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxStrLen(const LYNXCHAR *str)
{
	return (int)(_tcslen(str));
}


//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxIO
*  @brief 。計算 UTF8 字串長度.
*
*	@param 。
*	@param 。
*	@param 。
*	@return。
*/
//-------------------------------------------------------------------------------------------------------
int lynxUTF8StrLen(const LYNXCHAR *str)
{
	// 對於UTF-8編碼中的任意位元組B，如果B的第一位為0，則B為ASCII碼，並且B獨立的表示一個字元;
	// 如果B的第一位為1，第二位為0，則B為一個非ASCII字元（該字元由多個位元組表示）中的一個位元組，並且不為字元的第一個位元組編碼;
	// 如果B的前兩位為1，第三位為0，則B為一個非ASCII字元（該字元由多個位元組表示）中的第一個位元組，並且該字元由兩個位元組表示;
	// 如果B的前三位為1，第四位為0，則B為一個非ASCII字元（該字元由多個位元組表示）中的第一個位元組，並且該字元由三個位元組表示;
	// 如果B的前四位為1，第五位為0，則B為一個非ASCII字元（該字元由多個位元組表示）中的第一個位元組，並且該字元由四個位元組表示;

	int Len = lynxStrLen(str);
	int Count = 0;
	const unsigned char Byte4 = 0xF0; //11110000.
	const unsigned char Byte3 = 0xE0; //11100000.
	const unsigned char Byte2 = 0xC0; //11000000.
	const unsigned char Byte1 = 0x80; //10000000.
	int i;
	for (i = 0; i < Len;)
	{
		if ((str[i] & Byte4) == Byte4)
		{
			Count++;
			i += 4;
		}
		else if ((str[i] & Byte3) == Byte3)
		{
			Count++;
			i += 3;
		}
		else if ((str[i] & Byte2) == Byte2)
		{
			Count++;
			i += 2;
		}
		else if ((str[i] & Byte1) == Byte1)
		{
			i++;
		}
		else
		{
			//跳過控製碼.
			if (str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
			{
				i++;
			}
			else
			{
				//英數符.
				Count++;
				i++;
			}
		}
	}

	return Count;
}


//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxStrLower(LYNXCHAR *str)
{
	#if defined __iOS__
        lynxiOSStrLower(str);
	#elif __ANDROID__
		int i = 0;
		while (str[i])
		{
			str[i] = tolower(str[i]);
			i++;
		}
    #else
        _tcslwr(str);
    #endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxFindStrWild(LYNXCHAR *str, LYNXCHAR *find)
{
	int						i, k;
	LYNXCHAR					C;
	LYNXBOOL					bWild, bFind;

	if (_tcslen(str) < _tcslen(find))
		return FALSE;

	k = 0;
	bWild = FALSE;
	bFind = TRUE;	
	for (i=0; i<_tcslen(str); i++)
	{		
		if (bWild)
		{			
			if (str[i] == C)
			{
				k++;
				bWild = FALSE;
				bFind = TRUE;
			}
		}
		else
		{
			if (find[k] == '*')
			{			
				k++;
				if (find[k] == 0x00)
				{
					bFind = TRUE;
					break;
				}
				else
				{
					C = find[k];
					bWild = TRUE;
					bFind = FALSE;
				}
			}
			else
			{
				if (find[k] != str[i])
				{
					bFind = FALSE;
					break;
				}
				k++;
			}
		}
	}
	return bFind;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxIsUnicodeString(const LYNXCHAR *str)
{
	int i;
	DWORD C;
	for (i=0; i<lynxStrLen(str); i++)
	{
		C = str[i];
		if (C > 127)
			return LYNX_TRUE;
	}
	return LYNX_FALSE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetNameByPath(LYNXCHAR *name, const LYNXCHAR *path)
{
	int					i, k = 0; 

	for (i=(int)lynxStrLen(path)-2; i>=0; i--)
	{		
		if (path[i] == _T('/') || path[i] == _T('\\'))
		{
			k = i+1;
			break;
		}
	}
	lynxStrCpy(name, &path[k]);
	if (name[lynxStrLen(name)-1] == _T('/') || name[lynxStrLen(name)-1] == _T('\\'))
		name[lynxStrLen(name)-1] = 0x00;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSeparateFileExtByPath(LYNXCHAR *name, LYNXCHAR *ext, const LYNXCHAR *path)
{
	int				i;
	LYNXCHAR		Buffer[512];

	lynxStrCpy(ext, _T(""));			
	lynxStrCpy(name, path);			

	lynxStrCpy(Buffer, path);
	for (i=(int)lynxStrLen(Buffer); i>=0; i--)
	{
		if (Buffer[i] == _T('.'))
		{
			if (i != 0 && Buffer[i-1] != _T('.'))
			{
				lynxStrCpy(ext, &Buffer[i+1]);			
				Buffer[i] = 0x00;
				lynxStrCpy(name, Buffer);
				break;								
			}
		}		
	}
}

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSeparatePathFilenameExtByFullPath(LYNXCHAR *path, LYNXCHAR *name, LYNXCHAR *ext, const LYNXCHAR *full_path)
{
	int				i;
	LYNXCHAR		Buffer[512];

	lynxStrCpy(ext, _T(""));			
	lynxStrCpy(name, full_path);			
	lynxStrCpy(path, full_path);

	lynxStrCpy(Buffer, full_path);
	for (i=(int)lynxStrLen(Buffer); i>=0; i--)
	{
		if (Buffer[i] == _T('.'))
		{
			if (i != 0 && Buffer[i-1] != _T('.'))
			{
				lynxStrCpy(ext, &Buffer[i+1]);			
				Buffer[i] = 0x0;				
				break;								
			}
		}		
	}

	lynxStrCpy(name, Buffer);
	lynxStrCpy(path, _T(""));

	for (i=(int)lynxStrLen(Buffer); i>=0; i--)
	{
		if (Buffer[i] == _T('/') || Buffer[i] == _T('\\'))
		{
			if (i != 0)
			{
				lynxStrCpy(name, &Buffer[i+1]);			
				Buffer[i] = 0x0;
				lynxStrCpy(path, Buffer);
				break;								
			}			
		}		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetFilenameOnlyByPath(LYNXCHAR *filename, const LYNXCHAR *path)
{
	int i;
	LYNXCHAR FilePath[256];
	LYNXCHAR Ext[16];
	LYNXCHAR Filename[256];

	lynxSeparateFileExtByPath(FilePath, Ext, path);
	lynxStrCpy(filename, FilePath);

	for (i=(int)lynxStrLen(FilePath); i>=0; i--)
	{
		if (FilePath[i] == _T('\\') || FilePath[i] == _T('/'))
		{
			lynxStrCpy(filename, &FilePath[i+1]);
			return;
		}		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRemoveWhiteSpaceAndTab(LYNXCHAR *str)
{
	int Len, i;
	LYNXCHAR* Temp;
	int Start, End, Index;

	Len = lynxStrLen(str);
	Temp = lynxMemAlloc((Len+1)*sizeof(LYNXCHAR));
	lynxStrCpy(Temp, str);
	
	for (i=0; i<Len; i++)
	{
		if (str[i] != L' ')
        {
			Start = i;
			break;
		}
	}

	for (i=Len-1; i>=0; i--)
	{
		if (str[i] != L' ')
        {
			End = i;
			break;
		}
	}

	Index = 0;
	for (i=Start; i<=End; i++)
	{
		if (str[i] != L'\t')
        {
			Temp[Index++] = str[i];
		}
	}
	Temp[Index++] = 0x00;

	lynxStrCpy(str, Temp);

	lynxMemFree(Temp);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
DWORD lynxDecodeUnicode(LYNXCHAR **str_pos, int decoder)
{
    DWORD C[4], Unicode = 0;
	DWORD B0, B1, B2, B3;
    
    switch (decoder)
    {
        case LYNX_UTF8:
            C[0] = (**str_pos) & 0xff;
            if ((C[0] >> 7) == 0)
            {
                Unicode = C[0] & 0x7f;
            }
            else if ((C[0] >> 5) == 0x6)
            {
                (*str_pos)++;
                C[1] = (**str_pos) & 0xff;
                
                B0 = C[0] & 0x1f;
                B1 = C[1] & 0x3f;
                
                Unicode = (B0 << 6) + B1;
            }
            else if ((C[0] >> 4) == 0xe)
            {
                (*str_pos)++;
                C[1] = (**str_pos) & 0xff;
                (*str_pos)++;
                C[2] = (**str_pos) & 0xff;
                
                B0 = C[0] & 0xf;
                B1 = C[1] & 0x3f;
                B2 = C[2] & 0x3f;
                
                Unicode = (B0 << 12) + (B1 << 6) + B2;
            }
            else if ((C[0] >> 3) == 0x1e)
            {
                (*str_pos)++;
                C[1] = (**str_pos) & 0xff;
                (*str_pos)++;
                C[2] = (**str_pos) & 0xff;
                (*str_pos)++;
                C[3] = (**str_pos) & 0xff;
                
                B0 = C[0] & 0x7;
                B1 = C[1] & 0x3f;
                B2 = C[2] & 0x3f;
                B3 = C[3] & 0x3f;
                
                Unicode = (B0 << 18) + (B1 << 12) + (B2 << 6) + B3;
            }
            (*str_pos)++;
            break;
    }
    return Unicode;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCopyUnicodeStrByLen(LYNXCHAR *new_str, const LYNXCHAR *src_str, const int len, const int decoder)
{
    DWORD C[4], Unicode = 0;
	DWORD B0, B1, B2, B3;    
	LYNXCHAR* Dst;
	int i;

	lynxStrCpy(new_str, src_str);
	Dst = new_str;

	if (len < 0)
	{
		return LYNX_FALSE;
	}
	else
	{
		switch (decoder)
		{
			case LYNX_UTF8:
				for (i=0; i<len; i++)
				{
					if ((*Dst) == 0x00)
					{
						return LYNX_FALSE;
					}

					C[0] = (*Dst) & 0xff;
					if ((C[0] >> 7) == 0)
					{					
					}
					else if ((C[0] >> 5) == 0x6)
					{
						Dst++;										
					}
					else if ((C[0] >> 4) == 0xe)
					{
						Dst++;
						Dst++;										
					}
					else if ((C[0] >> 3) == 0x1e)
					{
						Dst++;					
						Dst++;					
						Dst++;										
					}				
					Dst++;					
				}
				(*Dst) = 0x00;			
				break;

		}
	}
	return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxIO
*  @brief 。
*
*	@param 。
*	@param 。
*	@param 。
*	@return。
*/
//-------------------------------------------------------------------------------------------------------
LYNXDWORD lynxUnicodeStrLen(const LYNXCHAR *str, const int decoder)
{
	DWORD C[4], Unicode = 0;
	DWORD B0, B1, B2, B3;
	const LYNXCHAR* Dst;
	LYNXDWORD Len = 0;;

	Dst = str;

	switch (decoder)
	{
	case LYNX_UTF8:
		while ((*Dst) != 0x00)
		{
			C[0] = (*Dst) & 0xff;

			if ((C[0] >> 7) == 0)
			{
			}
			else if ((C[0] >> 5) == 0x6)
			{
				Dst++;
			}
			else if ((C[0] >> 4) == 0xe)
			{
				Dst++;
				Dst++;
			}
			else if ((C[0] >> 3) == 0x1e)
			{
				Dst++;
				Dst++;
				Dst++;
			}
			Dst++;
			Len++;
		}		
		break;
	}
	return Len;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
//-------------------------------------------------------------------------------------------------------
void lynxModifiedUTF8String(LYNXCHAR *new_str, const LYNXCHAR *src_str)
{
	DWORD C[4];	
	LYNXCHAR *Dst;
	const LYNXCHAR *Src;
	
	Dst = new_str;
	Src = src_str;
	while ((*Src) != 0x00)
	{
		C[0] = (*Src) & 0xff;
		if ((C[0] >> 7) == 0)
		{
			(*Dst) = (*Src);
			Src++;
			Dst++;
		}
		else if ((C[0] >> 5) == 0x6)
		{
			(*Dst) = (*Src);
			Src++;
			Dst++;										
			(*Dst) = (*Src);
			Src++;
			Dst++;										
		}
		else if ((C[0] >> 4) == 0xe)
		{
			(*Dst) = (*Src);
			Src++;
			Dst++;										
			(*Dst) = (*Src);
			Src++;
			Dst++;										
			(*Dst) = (*Src);
			Src++;
			Dst++;										
		}
		else if ((C[0] >> 3) == 0x1e)
		{
			Src += 4;													
		}
		else if ((C[0] >> 2) == 0x3e)
		{
			Src += 5;
		}
		else if ((C[0] >> 1) == 0x7e)
		{
			Src += 6;
		}
		else if ((C[0] >> 1) == 0xfe)
		{
			Src += 7;
		}		
	}
	/*
	*Dst = 0xc0;
	Dst++;
	*Dst = 0x80;
	Dst++;	
	*/
	*Dst = 0x00;
	Dst++;	
}




 //#include <stdio.h> 
 //#include <windows.h> 
 //#include <locale.h> 
 //#define BUFF_SIZE 1024 
 // 
 //wchar_t * ANSIToUnicode( const char* str ) 
 //{ 
 //     int textlen ; 
 //     wchar_t * result; 
 //     textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
 //     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
 //     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
 //     MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
 //     return result; 
 //} 
 // 
 //char * UnicodeToANSI( const wchar_t* str ) 
 //{ 
 //     char* result; 
 //     int textlen; 
 //     textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL ); 
 //     result =(char *)malloc((textlen+1)*sizeof(char)); 
 //     memset( result, 0, sizeof(char) * ( textlen + 1 ) ); 
 //     WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL ); 
 //     return result; 
 //} 
 // 
 //wchar_t * UTF8ToUnicode( const char* str ) 
 //{ 
 //     int textlen ; 
 //     wchar_t * result; 
 //     textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
 //     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
 //     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
 //     MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen ); 
 //     return result; 
 //} 
 // 
 //char * UnicodeToUTF8( const wchar_t* str ) 
 //{ 
 //     char* result; 
 //     int textlen; 
 //     textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL ); 
 //     result =(char *)malloc((textlen+1)*sizeof(char)); 
 //     memset(result, 0, sizeof(char) * ( textlen + 1 ) ); 
 //     WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL ); 
 //     return result; 
 //} 
 //char* w2m(const wchar_t* wcs) 
 //{ 
 //      int len; 
 //      char* buf; 
 //      len =wcstombs(NULL,wcs,0); 
 //      if (len == 0) 
 //          return NULL; 
 //      buf = (char *)malloc(sizeof(char)*(len+1)); 
 //      memset(buf, 0, sizeof(char) *(len+1)); 
 //      len =wcstombs(buf,wcs,len+1); 
 //      return buf; 
 //} 
 //wchar_t* m2w(const char* mbs) 
 //{ 
 //      int len; 
 //      wchar_t* buf; 
 //      len =mbstowcs(NULL,mbs,0); 
 //      if (len == 0) 
 //          return NULL; 
 //      buf = (wchar_t *)malloc(sizeof(wchar_t)*(len+1)); 
 //      memset(buf, 0, sizeof(wchar_t) *(len+1)); 
 //      len =mbstowcs(buf,mbs,len+1); 
 //      return buf; 
 //} 
 // 
 //char* ANSIToUTF8(const char* str) 
 //{ 
 //     return UnicodeToUTF8(ANSIToUnicode(str)); 
 //} 
 // 
 //char* UTF8ToANSI(const char* str) 
 //{ 
 //     return UnicodeToANSI(UTF8ToUnicode(str)); 
 //} 
  