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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXSTRING_H__
#define __LYNXSTRING_H__

#include <LynxCore.h>

#ifdef __USE_STL__
	#include <string>	
#endif
#include <algorithm>
#include <string>

namespace LynxEngine 
{
	template <class T>
	class CTString : public std::basic_string<T>		
	{
	public:		
		CTString() {};					
		CTString(const T *str):std::basic_string<T>(str) {};			
		CTString(std::basic_string<T>& str):std::basic_string<T>(str) {};			
		virtual ~CTString() {};

		LYNXINLINE							operator const T*() const { return std::basic_string<T>::c_str(); };									
		LYNXINLINE void						ToLower()
		{
			std::transform(this->begin(), this->end(), this->begin(), ::towlower);
		}
	};
	
	class CAnsiString : public std::basic_string<char>		
	{
	public:		
		CAnsiString() {};
		CAnsiString(int i) {};
		CAnsiString(const char *str):std::basic_string<char>(str) {};	
		CAnsiString(const std::basic_string<char> &str):std::basic_string<char>(str) {}; 
		~CAnsiString() {}; 
		#if defined (_UNICODE) || defined (UNICODE)
			CAnsiString(const LYNXUNICHAR* str)
			{
				static char	AnsiStr[2048];
				char*		pAnsiStr;
                int Len = (int)_tcslen(str);
				if (Len != 0)
				{
					if (Len <= 2046)
					{
						lynxAnsiStr(AnsiStr, str);
						AnsiStr[Len] = 0x00;
						*this = AnsiStr;
					}
					else
					{
						pAnsiStr = LYNXNEW char[Len+2];
						lynxAnsiStr(pAnsiStr, str);
						pAnsiStr[Len] = 0x00;
						*this = pAnsiStr;
						LYNXDEL_ARRAY(pAnsiStr);
					}
				}
			}
		#endif				

		LYNXINLINE							operator const char*() const { return (c_str()); };															
		LYNXINLINE	CAnsiString&			Trim() 
		{ 
			this->erase(0, this->find_first_not_of(' '));
			std::string::size_type i = 0;
			while (i < this->length()) 
			{
				i = this->find('\n', i);
				if (i == std::string::npos) 
				{
					break;
				}
				this->erase(i);
			}
            i = 0;
			while (i < this->length())
			{
				i = this->find('\r', i);
				if (i == std::string::npos)
				{
					break;
				}
				this->erase(i);
			}
			return (*this);
		}
        CAnsiString&                        ReplaceBackslashWithSlash()
        {
          	unsigned int i = 0;
			while (i < this->length())
			{
				if (at(i) == '\\')
                    at(i) = '/';
                i++;
			}
			return (*this);
        }
		LYNXINLINE void						ToLower()
		{
			std::transform(begin(), end(), begin(), ::towlower);
		}
		LYNXINLINE void						ToValue(LYNXUINT& v)
		{
			v = lynxAtoi(this->c_str());			
		}
		LYNXINLINE void						ToValue(LYNXINT& v)
		{
			v = lynxAtoi(this->c_str());			
		}
		LYNXINLINE void						ToValue(unsigned long& v)
		{
			v = lynxAtoul(this->c_str());			
		}
		LYNXINLINE void						ToValue(long& v)
		{
			v = lynxAtol(this->c_str());			
		}
		LYNXINLINE void						ToValue(LYNXULONGLONG& v)
		{
			v = lynxAtoull(this->c_str());			
		}
		LYNXINLINE void						ToValue(float& v)
		{
			v = lynxAtof(this->c_str());			
		}
		LYNXINLINE void						ToValue(CAnsiString& v)
		{
			v = (*this);			
		}
	};

	class CUniString : public std::basic_string<LYNXUNICHAR>		
	{
	public:		
		CUniString() {};
		CUniString(int i) {};
		CUniString(const LYNXUNICHAR *str):std::basic_string<LYNXUNICHAR>(str) {};	
		CUniString(const std::basic_string<LYNXUNICHAR> &str):std::basic_string<LYNXUNICHAR>(str) {}; 
		~CUniString() {};
		CUniString(const char* str)
		{
			LYNXUNICHAR* pUniStr;
			static LYNXUNICHAR	UniStr[2048];
			int Len = (int)strlen(str);
			if (Len != 0)
			{
				if (Len < 2046)
				{
					int i = 0;
					for (i=0; i<Len; i++)
						UniStr[i] = lynxToUnicode(str[i]);
					UniStr[i] = lynxToUnicode(0x00);
					*this = UniStr;				
				}
				else
				{
					pUniStr = LYNXGLOBAL_NEW LYNXUNICHAR[Len+2];
					int i = 0;
					for (i=0; i<Len; i++)
						pUniStr[i] = lynxToUnicode(str[i]);
					pUniStr[i] = lynxToUnicode(0x00);
					*this = pUniStr;	
					LYNXGLOBAL_DEL_ARRAY(pUniStr);
				}				
			}
		}		
		LYNXINLINE							operator const LYNXUNICHAR*() const { return c_str(); };
		LYNXINLINE	CUniString&				Trim() 
		{ 
			this->erase(0, this->find_first_not_of(_T(' ')));
			std::string::size_type i = 0;
			while (i < this->length()) 
			{
				i = this->find(_T('\n'), i);
				if (i == std::string::npos) 
				{
					break;
				}
				this->erase(i);
			}
            i = 0;
			while (i < this->length())
			{
				i = this->find(_T('\r'), i);
				if (i == std::string::npos)
				{
					break;
				}
				this->erase(i);
			}

			return (*this);
		}
        CUniString&                        ReplaceBackslashWithSlash()
        {
          	unsigned int i = 0;
			while (i < this->length())
			{
				if (at(i) == _T('\\'))
                    at(i) = _T('/');
                i++;
			}
			return (*this);
        }
		LYNXINLINE void						ToLower()
		{
			std::transform(begin(), end(), begin(), ::towlower);
		}		
		/*
		LYNXINLINE void						ToValue(LYNXUINT& v)
		{
			v = lynxAtoi(CAnsiString(this->c_str()));			
		}
		LYNXINLINE void						ToValue(LYNXINT& v)
		{
			v = lynxAtoi(this->c_str());			
		}
		LYNXINLINE void						ToValue(unsigned long& v)
		{
			v = lynxAtoul(this->c_str());			
		}
		LYNXINLINE void						ToValue(long& v)
		{
			v = lynxAtol(this->c_str());			
		}
		LYNXINLINE void						ToValue(LYNXULONGLONG& v)
		{
			v = lynxAtoull(this->c_str());			
		}
		LYNXINLINE void						ToValue(float& v)
		{
			v = lynxAtof(this->c_str());			
		}
		LYNXINLINE void						ToValue(CUniString& v)
		{
			v = (*this);			
		}
		*/
	};
	
#ifdef _UNICODE
	typedef CUniString						CString;
	/*
	class CString : public std::basic_string<LYNXCHAR>		
	{
	public:		
		CString() {};
		CString(int i) {};
		CString(const LYNXCHAR *str):std::basic_string<LYNXCHAR>(str) {};			
		CString(const CAnsiString &str) {*this = str.c_str(); }; 
		CString(const std::basic_string<LYNXCHAR> &str):std::basic_string<LYNXCHAR>(str) {}; 
		#if defined (_UNICODE) || defined (UNICODE)
			CString(const LYNXANSICHAR *str) 
			{
				LYNXCHAR* pUniStr;
				static LYNXCHAR	UniStr[2048];
				int Len = (int)strlen(str);
				if (Len != 0)
				{
					if (Len < 2046)
					{
						lynxFromAnsiStr(UniStr, str);
						UniStr[Len] = 0x00;
						*this = UniStr;
					}
					else
					{
						pUniStr = LYNXNEW LYNXCHAR[Len+2];
						lynxFromAnsiStr(pUniStr, str);
						pUniStr[Len] = 0x00;
						*this = pUniStr;
						LYNXDEL_ARRAY(pUniStr);
					}				
				}
			}
		#endif				
		CString(std::basic_string<LYNXCHAR>& str):std::basic_string<LYNXCHAR>(str) {};			
		//virtual ~CString() {};

		LYNXINLINE							operator const LYNXCHAR*() const { return c_str(); };															
		LYNXINLINE	CString&				Trim() 
		{ 
			this->erase(0, this->find_first_not_of(_T(' ')));
			std::string::size_type i = 0;
			while (i < this->length()) 
			{
				i = this->find(_T('\n'), i);
				if (i == std::string::npos) 
				{
					break;
				}
				this->erase(i);
			}
            i = 0;
			while (i < this->length())
			{
				i = this->find(_T('\r'), i);
				if (i == std::string::npos)
				{
					break;
				}
				this->erase(i);
			}
			return (*this);
		}
	};
	*/
#else
	typedef CAnsiString						CString;
#endif
}

#endif