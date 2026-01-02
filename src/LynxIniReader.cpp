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

#include <LynxEngine_PCH.h>
#include <LynxIniReader.h>

namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CIniReader::CIniReader(const LYNXCHAR* filename, unsigned int encoding)
	{
		lynxStrCpy(m_FileName, filename);
        if (encoding == 0)
        {
            #ifdef _UNICODE
                m_Encoding = CStream::UTF16;
            #else
                m_Encoding = CStream::ANSI;				
            #endif
        }
        else
        {
			#ifdef __WIN32__
				#ifdef _UNICODE
					m_Encoding = encoding;
				#else
					m_Encoding = CStream::ANSI;
				#endif				
			#else
				m_Encoding = encoding;				
			#endif
        }
		if (m_ParamMap.empty())
			Prefetch();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CIniReader::~CIniReader() 
	{
		for (CMap<CString , CMap<CString, CString> >::CIterator Pos = m_ParamMap.begin(); Pos != m_ParamMap.end(); Pos++)
		{
			(*Pos).second.clear();
		}
		m_ParamMap.clear(); 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CIniReader::Prefetch() 
	{
		for (CMap<CString , CMap<CString, CString> >::CIterator Pos = m_ParamMap.begin(); Pos != m_ParamMap.end(); Pos++)
		{
			(*Pos).second.clear();
		}
		m_ParamMap.clear(); 

		CPlatformFileStream File;
		CString Line, SectionStr;
		LYNXCHAR SectionToken[] = _T("[]");
		LYNXCHAR Value[1024], TrimmedLine[1024], TrimmedKey[1024], TrimmedValue[1024];		
		
        if (!File.vOpen(m_FileName, CStream::Text|CStream::Read|m_Encoding))
			return;

		while (File.vRead(Line))
		{			
			if (Line.c_str()[0] == L'[')
			{
				LYNXCHAR* SecToken = lynxStrTok((LYNXCHAR*)Line.c_str(), SectionToken);
				SectionStr = SecToken;						
			}
			else
			{
				LYNXCHAR* Token = lynxStrTok((LYNXCHAR*)Line.c_str(), _T("="));
				if (Token)
				{
					lynxStrCpy(TrimmedKey, Token);
					lynxRemoveWhiteSpaceAndTab(TrimmedKey);				
					CString Key = TrimmedKey;
					Token = lynxStrTok(NULL, _T("="));
					if (Token)
					{
						lynxStrCpy(TrimmedValue, Token);
						lynxRemoveWhiteSpaceAndTab(TrimmedValue);				

                        int Index = 0;
						for (int i=0; i<lynxStrLen(TrimmedValue); i++)
						{
							if (TrimmedValue[i] == L'\\' && TrimmedValue[i+1] == L'n')
							{
								Value[Index] = L'\n';
                                i++;
								Index++;
							}
                            else if (TrimmedValue[i] != 0xA && TrimmedValue[i] != 0xD)
							{
								Value[Index] = TrimmedValue[i];
								Index++;
							}
						}
						Value[Index] = NULL;
						m_ParamMap[SectionStr][Key] = Value;												
					}							
				}
			}
		}			
		File.vClose();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCHAR* CIniReader::ReadString(LYNXCHAR* dest, const LYNXCHAR* szSection, const LYNXCHAR* szKey)
	{
		if (m_ParamMap.find(CString(szSection)) != m_ParamMap.end())
		{
			CMap<CString , CString>& Map = (m_ParamMap[CString(szSection)]);
			CMap<CString , CString>::CIterator Found = Map.find(CString(szKey));
			if (Found != Map.end())
			{
				lynxStrCpy(dest, Found->second.c_str());
				return dest;
			}		
		}	

		CPlatformFileStream File;
		CString Section = CString(_T("[")) + CString(szSection) + CString(_T("]"));
		CString Line;

        File.vOpen(m_FileName, CStream::Text|CStream::Read|m_Encoding);   
		
		while (File.IsValid() && File.vRead(Line))
		{
			if (lynxStriStr(Line.c_str(), Section))
			{
				while (File.vRead(Line))
				{
					if (Line.c_str()[0] == L'[')
					{
						File.vClose();
						return NULL;		
					}
					else
					{
						LYNXCHAR* Token = lynxStrTok((LYNXCHAR*)Line.c_str(), _T("="));
						if (Token)
						{
                            if (lynxStriStr(Token, szKey))
							{
								Token = lynxStrTok(NULL, _T("="));
								if (Token)
								{
                                    int Start = 0;
                                    for (int i=0; i<lynxStrLen(Token); i++)
									{
										if (Token[i] != L' ' && Token[i] != L'\t')
                                        {
											Start = i;
                                            break;
										}
									}
									int Index = 0;
									for (int i=Start; i<lynxStrLen(Token); i++)
									{
										if (Token[i] == L'\\' && Token[i+1] == 'n')
										{
											dest[Index] = L'\n';
                                            i++;
											Index++;
										}
                                        else if (Token[i] != 0xA && Token[i] != 0xD)
										{
											dest[Index] = Token[i];
											Index++;
										}
									}
									dest[Index] = NULL;
									return dest;
								}
							}
						}
					}
				}
			}
		}
		File.vClose();
		return NULL;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CIniReader::ReadInt(const LYNXCHAR* szSection, const LYNXCHAR* szKey, int iDefaultValue)
	{
		int iResult = iDefaultValue;

		#if 0//defined (__WIN32__) && !defined (__WINAPP__)
			iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_FileName); 			
		#else			
			LYNXCHAR Value[256];

			if (ReadString(Value, szSection, szKey))
			{
				iResult = lynxAtoi(Value);
			}			
		#endif

		return iResult;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	float CIniReader::ReadFloat(const LYNXCHAR* szSection, const LYNXCHAR* szKey, float fltDefaultValue)
	{
		LYNXCHAR		szResult[256];
		LYNXCHAR		szDefault[256];
		LYNXANSICHAR	ValueStr[32];
		float			fltResult;
		
		fltResult = fltDefaultValue;
		
		#if 0//defined (__WIN32__) && !defined (__WINAPP__)
			lynxSprintf(szDefault, _T("%f"), fltDefaultValue);
			GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_FileName); 			
			lynxAnsiStr(ValueStr, szResult);
			fltResult = atof(ValueStr);
		#else			
			if (ReadString(szResult, szSection, szKey))
			{
				lynxAnsiStr(ValueStr, szResult);
				fltResult = atof(ValueStr);
			}			
		#endif

		return fltResult;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CIniReader::ReadBool(const LYNXCHAR* szSection, const LYNXCHAR* szKey, LYNXBOOL bolDefaultValue)
	{
		LYNXCHAR	szResult[256];
		LYNXCHAR	szDefault[256];
		LYNXBOOL	bolResult;

		bolResult = bolDefaultValue;
		
		#if 0//defined (__WIN32__) && !defined (__WINAPP__)
			lynxSprintf(szDefault, _T("%s"), bolDefaultValue? _T("TRUE") : _T("FALSE"));
			GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_FileName); 
			bolResult =  (lynxStriCmp(szResult, _T("FALSE"))) ? LYNX_TRUE : LYNX_FALSE;
		#else			
			if (ReadString(szResult, szSection, szKey))
			{
				bolResult =  (lynxStriCmp(szResult, _T("FALSE"))) ? LYNX_TRUE : LYNX_FALSE;
			}			
		#endif

		return bolResult;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString CIniReader::ReadString(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCHAR* szDefaultValue)
	{
		LYNXCHAR	szResult[512];
	
		lynxStrCpy(szResult, szDefaultValue);
				
		#if 0//defined (__WIN32__) && !defined (__WINAPP__)
			GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_FileName); 			
		#else			
			ReadString(szResult, szSection, szKey);
		#endif

		return CString(szResult);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCOLORRGBA CIniReader::ReadRGBA(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCOLORRGBA& szDefaultValue)
	{
		LYNXCOLORRGBA Color = szDefaultValue;
		LYNXCHAR DefaultString[32];

		lynxSprintf(DefaultString, _T("%d,%d,%d,%d"), Color.Red, Color.Green, Color.Blue, Color.Alpha);
		CString Value = ReadString(szSection, szKey, DefaultString);

		int Start = 0;
		int CurrentChar = 0;
		LYNXCHAR Comp[32];
		for (int i=0; i<4; i++)
		{
			CurrentChar = 0;
			while (Value.c_str()[Start] && Value.c_str()[Start] != _T(','))
			{
				Comp[CurrentChar] = Value.c_str()[Start];
				CurrentChar++;
				Start++;
			}
			Start++;
			Comp[CurrentChar] = NULL;
			switch(i)
			{
				case 0:
					Color.Red = lynxAtoi(Comp);
					break;
				case 1:
					Color.Green = lynxAtoi(Comp);
					break;
				case 2:
					Color.Blue = lynxAtoi(Comp);
					break;
				case 3:
					Color.Alpha = lynxAtoi(Comp);
					break;
			}
		}
		return Color;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCOLORRGB CIniReader::ReadRGB(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCOLORRGB& szDefaultValue)
	{
		LYNXCOLORRGB Color = szDefaultValue;
		LYNXCHAR DefaultString[32];

		lynxSprintf(DefaultString, _T("%d,%d,%d"), Color.Red, Color.Green, Color.Blue);
		CString Value = ReadString(szSection, szKey, DefaultString);

		int Start = 0;
		int CurrentChar = 0;
		LYNXCHAR Comp[32];
		for (int i=0; i<3; i++)
		{
			CurrentChar = 0;
			while (Value.c_str()[Start] && Value.c_str()[Start] != _T(','))
			{
				Comp[CurrentChar] = Value.c_str()[Start];
				CurrentChar++;
				Start++;
			}
			Start++;
			Comp[CurrentChar] = NULL;
			switch(i)
			{
				case 0:
					Color.Red = lynxAtoi(Comp);
					break;
				case 1:
					Color.Green = lynxAtoi(Comp);
					break;
				case 2:
					Color.Blue = lynxAtoi(Comp);
					break;				
			}
		}
		return Color;
	}
}