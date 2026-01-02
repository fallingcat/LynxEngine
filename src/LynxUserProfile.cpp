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
#include <LynxUserProfile.h>

namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CUserProfile::CUserProfile(const CString& filename)
	{	
		m_Filename = filename;
		m_CheckSumString = CHECK_SUM_STRING;
		m_CheckSumString2 = CHECK_SUM_STRING2;
		m_bUseCheckSum = LYNX_TRUE;
		m_Doc = LYNXGLOBAL_NEW CXMLDoc();		
		WriteDefaultElements();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CUserProfile::~CUserProfile() 
	{
		if (m_Doc)
		{
			LYNXGLOBAL_DEL(m_Doc);
			m_Doc = NULL;
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CUserProfile::Load() 
	{
		m_Doc->Clear();
		
		CString Path, CheckSumPath;
		if (!m_Password.empty())
		{
			CPlatformFileStream File;
			CEngine::DecryptFileFromZip(File, m_Filename, m_Password, CStream::Binary|CStream::Read);			
			File.vClose();
			Path = _T("../temp/output.dec");
		}
		else
		{
			Path = m_Filename;
			CheckSumPath = Path + CString(_T(".cks"));
		}

		LYNXANSICHAR* Data;
		CStream* Stream = glpKernel->GetlpEngine()->GetlpFileSystem()->GetPackage()->GetStream();
		if (!Stream->vOpen(Path.c_str(), CStream::Binary|CStream::Read))
		{
			return LYNX_FALSE;
		}

		int Size = Stream->vSize();
		Data = LYNXGLOBAL_NEW LYNXANSICHAR[Size + m_CheckSumString.size() + m_CheckSumString2.size()];
		Stream->vRead(&Data[m_CheckSumString.size()], 1, Size);		
		Stream->vClose();

		if (m_Password.empty())
		{
			CPlatformFileStream CKSStream;
			LYNXANSICHAR Result[64];
			LYNXCRC SourceCRC, CRC;
			lynxMemCpy(&Data[0], m_CheckSumString.c_str(), m_CheckSumString.size());
			lynxMemCpy(&Data[Size + m_CheckSumString.size()], m_CheckSumString2.c_str(), m_CheckSumString2.size());
			//lynxMD5(Result, Data);
			CRC = lynxComputeCRCFast((const unsigned char*)Data, Size + m_CheckSumString.size() + m_CheckSumString2.size());
			Data[Size + m_CheckSumString.size()] = 0x00;

			#if SHIPPING
				if (m_bUseCheckSum)
				{
					if (!Stream->vOpen(CheckSumPath.c_str(), CStream::Binary|CStream::Read))
					{
						glpKernel->SetMemoryHackingDetectionFlag(LYNX_TRUE);
						//LYNXGLOBAL_DEL_ARRAY(Data);												
						//return LYNX_FALSE;
					}
					else
					{
						Stream->vRead(&SourceCRC, 1, sizeof(LYNXCRC));
						Stream->vClose();
						if (SourceCRC != CRC)
						{
							glpKernel->SetMemoryHackingDetectionFlag(LYNX_TRUE);
							//LYNXGLOBAL_DEL_ARRAY(Data);							
							//return LYNX_FALSE;
						}
					}
				}
			#else
				#ifdef __DESKTOP__
					if (CKSStream.vOpen(CheckSumPath.c_str(), CStream::Binary|CStream::Write))
					{
						CKSStream.vWrite(&CRC, 1, sizeof(LYNXCRC));
						CKSStream.vClose();
					}
				#endif
			#endif
		}
		if (m_Doc->Parse(&Data[m_CheckSumString.size()], Size) != tinyxml2::XML_SUCCESS)
		{
			LYNXGLOBAL_DEL_ARRAY(Data);
			WriteDefaultElements();
			return LYNX_FALSE;
		}
		LYNXGLOBAL_DEL_ARRAY(Data);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CUserProfile::GenerateCKS()
	{
		m_Doc->Clear();

		CString Path, CheckSumPath;
		if (!m_Password.empty())
		{
			CPlatformFileStream File;
			CEngine::DecryptFileFromZip(File, m_Filename, m_Password, CStream::Binary | CStream::Read);
			File.vClose();
			Path = _T("../temp/output.dec");
		}
		else
		{
			Path = m_Filename;
			CheckSumPath = Path + CString(_T(".cks"));
		}

		LYNXANSICHAR* Data;
		CStream* Stream = glpKernel->GetlpEngine()->GetlpFileSystem()->GetPackage()->GetStream();
		if (!Stream->vOpen(Path.c_str(), CStream::Binary | CStream::Read))
		{
			return LYNX_FALSE;
		}

		int Size = Stream->vSize();
		Data = LYNXGLOBAL_NEW LYNXANSICHAR[Size + m_CheckSumString.size() + m_CheckSumString2.size()];
		Stream->vRead(&Data[m_CheckSumString.size()], 1, Size);
		Stream->vClose();

		if (m_Password.empty())
		{
			CPlatformFileStream CKSStream;
			CPlatformFileStream CKSTextStream;
			LYNXANSICHAR Result[64];
			LYNXCRC SourceCRC, CRC;
			lynxMemCpy(&Data[0], m_CheckSumString.c_str(), m_CheckSumString.size());
			lynxMemCpy(&Data[Size + m_CheckSumString.size()], m_CheckSumString2.c_str(), m_CheckSumString2.size());
			//lynxMD5(Result, Data);
			CRC = lynxComputeCRCFast((const unsigned char*)Data, Size + m_CheckSumString.size() + m_CheckSumString2.size());
			Data[Size + m_CheckSumString.size()] = 0x00;			
						
			if (CKSStream.vOpen(CheckSumPath.c_str(), CStream::Binary | CStream::Write))
			{
				CKSStream.vWrite(&CRC, 1, sizeof(LYNXCRC));
				CKSStream.vClose();
			}						
		}
		LYNXGLOBAL_DEL_ARRAY(Data);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::SetVersion(const CString& v) 
	{	
		CAnsiString Key = "Version";

		CXMLElement* Element = m_Doc->RootElement()->FirstChildElement(Key);
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Text->SetValue(v.c_str());
				}
			}			
		}
		else
		{
			CXMLElement* Element = m_Doc->NewElement(Key);		
			m_Root->InsertEndChild(Element);
			CXMLText* Text = m_Doc->NewText(v.c_str());
			Element->InsertFirstChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString CUserProfile::GetVersion() 
	{			
		CString Version = _T("0.0");

		Read("Version", Version);		

		return Version;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::WriteDefaultElements() 
	{	
		m_Doc->InsertFirstChild(m_Doc->NewDeclaration());
		if (!m_Doc->RootElement())
		{
			m_Root = m_Doc->NewElement("Profile");
			m_Doc->InsertEndChild(m_Root);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Save() 
	{	
		CString Path = glpKernel->GetlpEngine()->GetlpFileSystem()->GetFullSearchingPath(0, m_Filename);		
		m_Doc->SaveFile(Path.c_str());		

		CPlatformFileStream Stream, CKSStream;
		LYNXANSICHAR* Data;
		if (Stream.vOpen(m_Filename.c_str(), CStream::Binary|CStream::Read))
		{
			int Size = Stream.vSize();
			Data = LYNXGLOBAL_NEW LYNXANSICHAR[Size + m_CheckSumString.size() + m_CheckSumString2.size()];
			Stream.vRead(&Data[m_CheckSumString.size()], 1, Size);
			Stream.vClose();

			LYNXANSICHAR Result[64];
			LYNXCRC SourceCRC, CRC;
			lynxMemCpy(&Data[0], m_CheckSumString.c_str(), m_CheckSumString.size());
			lynxMemCpy(&Data[Size + m_CheckSumString.size()], m_CheckSumString2.c_str(), m_CheckSumString2.size());
			//lynxMD5(Result, Data);
			CRC = lynxComputeCRCFast((const unsigned char*)Data, Size + m_CheckSumString.size() + m_CheckSumString2.size());			

			LYNXGLOBAL_DEL_ARRAY(Data);						

			CString CheckSumPath = m_Filename + CString(_T(".cks"));
			lynxDeleteFile(CheckSumPath);

			if (CKSStream.vOpen(CheckSumPath.c_str(), CStream::Binary|CStream::Write))
			{
				CKSStream.vWrite(&CRC, 1, sizeof(LYNXCRC));
				CKSStream.vClose();
			}
		}
		
		if (!m_Password.empty())
		{
			CEngine::EncryptFile2Zip(m_Filename, m_Password);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	CUserProfile::GetValue(CXMLElement* node)
	{
		CXMLNode* TNode = node->FirstChild(); 
		if (TNode)
		{
			CXMLText* Text = TNode->ToText();
			if (Text)
			{			
				return CString(Text->Value());				
			}
		}

		return CString(_T(""));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CUserProfile::Read(const CAnsiString& key, const int default_value, CXMLElement* root)
	{
		int Result = default_value;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = lynxAtoi(Text->Value());
				}
			}
		}		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const int value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root;
		
		if (root)
			Root = root;
		else
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%d"), value);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d"), value);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);				
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	unsigned int CUserProfile::Read(const CAnsiString& key, const unsigned int default_value, CXMLElement* root)
	{
		unsigned int Result = default_value;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = lynxAtoul(Text->Value());
				}
			}
		}		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const unsigned int value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root;
		
		if (root)
			Root = root;
		else
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%u"), value);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%u"), value);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);				
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	unsigned long CUserProfile::Read(const CAnsiString& key, const unsigned long default_value, CXMLElement* root)
	{
		unsigned long Result = default_value;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = lynxAtoul(Text->Value());
				}
			}
		}		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const unsigned long value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%lu"), value);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}			
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%lu"), value);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXULONGLONG CUserProfile::Read(const CAnsiString& key, const LYNXULONGLONG default_value, CXMLElement* root)
	{
		LYNXULONGLONG Result = default_value;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = lynxAtoul(Text->Value());
				}
			}
		}		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXULONGLONG value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%lu"), value);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}			
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%lu"), value);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	float CUserProfile::Read(const CAnsiString& key, const float default_value, CXMLElement* root)
	{
		float Result = default_value;	
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();
		
		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = lynxAtof(Text->Value());
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const float value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%.6f"), value);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Text->SetValue(Buffer);				
				}
			}
			else
			{				
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%.6f"), value);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCOLORRGBA CUserProfile::Read(const CAnsiString& key, const LYNXCOLORRGBA& default_value, CXMLElement* root)
	{
		LYNXCOLORRGBA Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
					Result.Red = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.Green = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.Blue = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.Alpha = lynxAtof(Tok);
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXCOLORRGBA& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.Red, value.Green, value.Blue, value.Alpha);

			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}				
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.Red, value.Green, value.Blue, value.Alpha);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXCOLORRGB CUserProfile::Read(const CAnsiString& key, const LYNXCOLORRGB& default_value, CXMLElement* root)
	{
		LYNXCOLORRGB Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
					Result.Red = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.Green = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.Blue = lynxAtof(Tok);					
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXCOLORRGB& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%d, %d, %d"), value.Red, value.Green, value.Blue);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d, %d, %d"), value.Red, value.Green, value.Blue);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXQUATERNION CUserProfile::Read(const CAnsiString& key, const LYNXQUATERNION& default_value, CXMLElement* root)
	{
		LYNXQUATERNION Result = default_value;
		CXMLElement* Root = root;

		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild();
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));
					Result.w = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));
					Result.x = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));
					Result.y = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));
					Result.z = lynxAtof(Tok);
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXQUATERNION& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;

		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.w, value.x, value.y, value.z);
			CXMLNode* Node = Element->FirstChild();
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.w, value.x, value.y, value.z);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXRECT CUserProfile::Read(const CAnsiString& key, const LYNXRECT& default_value, CXMLElement* root)
	{
		LYNXRECT Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
					Result.x1 = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.y1 = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					Result.x2 = lynxAtof(Tok);					
					Tok = lynxStrTok(NULL, _T(","));					
					Result.y2 = lynxAtof(Tok);					
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXRECT& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.x1, value.y1, value.x2, value.y2);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d, %d, %d, %d"), value.x1, value.y1, value.x2, value.y2);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CVector2 CUserProfile::Read(const CAnsiString& key, const CVector2& default_value, CXMLElement* root)
	{
		CVector2 Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
					Result.x = Result.y = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					if (Tok)
						Result.y = lynxAtof(Tok);					
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CVector2& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%.6f, %.6f"), value.x, value.y);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%.6f, %.6f"), value.x, value.y);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CVector3 CUserProfile::Read(const CAnsiString& key, const CVector3& default_value, CXMLElement* root)
	{
		CVector3 Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					LYNXCHAR Buffer[256];
					lynxStrCpy(Buffer, Text->Value());
					LYNXCHAR* Tok = lynxStrTok(Buffer, _T(","));					
					Result.x = Result.y = Result.z = lynxAtof(Tok);
					Tok = lynxStrTok(NULL, _T(","));					
					if (Tok)
					{
						Result.y = lynxAtof(Tok);
						Tok = lynxStrTok(NULL, _T(","));
						Result.z = lynxAtof(Tok);
					}
				}
			}
		}
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CVector3& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[128];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%.6f, %.6f, %.6f"), value.x, value.y, value.z);
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%.6f, %.6f, %.6f"), value.x, value.y, value.z);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CMatrix4& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[2048];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();
	
		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			lynxSprintf(Buffer, _T("%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f"),
				value.m11, value.m12, value.m13, value.m14,
				value.m21, value.m22, value.m23, value.m24,
				value.m31, value.m32, value.m33, value.m34,
				value.m41, value.m42, value.m43, value.m44);

			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f"), 
						value.m11, value.m12, value.m13, value.m14,
						value.m21, value.m22, value.m23, value.m24,
						value.m31, value.m32, value.m33, value.m34,
						value.m41, value.m42, value.m43, value.m44);
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CUserProfile::Read(const CAnsiString& key, const LYNXBOOL default_value, CXMLElement* root)
	{
		LYNXBOOL Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					if (CString(Text->Value()) == CString("TRUE"))
						Result = LYNX_TRUE;
					else
						Result = LYNX_FALSE;
				}
			}
		}			
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const LYNXBOOL value, CXMLElement* root)
	{
		LYNXCHAR Buffer[32];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			if (value)
			{
				lynxSprintf(Buffer, _T("%s"), "TRUE");
			}
			else
			{
				lynxSprintf(Buffer, _T("%s"), "FALSE");
			}
			CXMLNode* Node = Element->FirstChild(); 
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{					
					Text->SetValue(Buffer);				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(Buffer);
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			if (value)
			{
				lynxSprintf(Buffer, _T("%s"), "TRUE");
			}
			else
			{
				lynxSprintf(Buffer, _T("%s"), "FALSE");
			}
			CXMLText* Text = m_Doc->NewText(Buffer);
			Element->InsertEndChild(Text);	
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString CUserProfile::Read(const CAnsiString& key, const CString& default_value, CXMLElement* root)
	{
		CString Result = default_value;		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild();
            if (Node)
            {
                CXMLText* Text = Node->ToText();
                if (Text)
                {
                    Result = Text->Value();
                }
            }
		}
		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CString& value, CXMLElement* root)
	{
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild();
            if (Node)
            {
                CXMLText* Text = Node->ToText();
                if (Text)
				{			
					Text->SetValue(value.c_str());				
				}
			}
			else
			{
				CXMLText* Text = m_Doc->NewText(value.c_str());
				Element->InsertEndChild(Text);
			}
		}
		else
		{
			CXMLElement* Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			CXMLText* Text = m_Doc->NewText(value.c_str());
			Element->InsertEndChild(Text);	
		}
	}
	/*
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CArray<int> CUserProfile::Read(const CAnsiString& key, const CArray<int>& default_value, CXMLElement* root)
	{
		CArray<int> Result = default_value;
		int Size = 0;
		
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			Size = Element->IntAttribute("Size");
			Result.clear();
			Result.resize(Size);

			CXMLElement* Value = Element->FirstChildElement("Value");
			for (CArray<int>::CIterator Pos=Result.begin(); Pos!=Result.end(); Pos++)
			{					
				CXMLNode* Node = Value->FirstChild();
				if (Node)
				{
					CXMLText* Text = Node->ToText();
					if (Text)
					{				
						*Pos = lynxAtoi(Text->Value());				
					}
				}
				Value = Value->NextSiblingElement();
			}			
		}	
		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CArray<int>& value)
	{
		LYNXCHAR Buffer[64];

		CXMLElement* Element = m_Doc->RootElement()->FirstChildElement(key.c_str());
		if (Element)
		{
			Element->DeleteChildren();
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CArray<int>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%d"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			m_Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CArray<int>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%d"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
	}
	*/
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CList<int> CUserProfile::Read(const CAnsiString& key, const CList<int>& default_value, CXMLElement* root)
	{
		CList<int> Result = default_value;
		int Size = 0;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			Size = Element->IntAttribute("Size");
			Result.clear();
			Result.resize(Size);

			CXMLElement* Value = Element->FirstChildElement("Value");
			for (CList<int>::CIterator Pos=Result.begin(); Pos!=Result.end(); Pos++)
			{					
				CXMLNode* Node = Value->FirstChild();
				if (Node)
				{
					CXMLText* Text = Node->ToText();
					if (Text)
					{				
						*Pos = lynxAtoi(Text->Value());				
					}
				}
				Value = Value->NextSiblingElement();
			}			
		}	
		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CList<int>& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			Element->DeleteChildren();
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CList<int>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%d"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
		else
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CList<int>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%d"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CList<unsigned long> CUserProfile::Read(const CAnsiString& key, const CList<unsigned long>& default_value, CXMLElement* root)
	{
		CList<unsigned long> Result = default_value;
		int Size = 0;
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			Size = Element->IntAttribute("Size");
			Result.clear();
			Result.resize(Size);

			CXMLElement* Value = Element->FirstChildElement("Value");
			for (CList<unsigned long>::CIterator Pos=Result.begin(); Pos!=Result.end(); Pos++)
			{					
				CXMLNode* Node = Value->FirstChild();
				if (Node)
				{
					CXMLText* Text = Node->ToText();
					if (Text)
					{				
						*Pos = lynxAtoul(Text->Value());				
					}
				}
				Value = Value->NextSiblingElement();
			}			
		}	
		
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CUserProfile::Write(const CAnsiString& key, const CList<unsigned long>& value, CXMLElement* root)
	{
		LYNXCHAR Buffer[64];
		CXMLElement* Root = root;
		
		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			Element->DeleteChildren();
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CList<unsigned long>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%lu"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
		else	
		{
			Element = m_Doc->NewElement(key.c_str());		
			Root->InsertEndChild(Element);
			lynxSprintf(Buffer, _T("%d"), value.size());
			Element->SetAttribute("Size", Buffer);
			for (CList<unsigned long>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
			{
				CXMLElement* Value = m_Doc->NewElement("Value");		
				Element->InsertEndChild(Value);
				lynxSprintf(Buffer, _T("%lu"), *Pos);
				CXMLText* Text = m_Doc->NewText(Buffer);
				Value->InsertEndChild(Text);				
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CUserProfile::Find(const CAnsiString& key, CXMLElement* root)
	{
		CXMLElement* Root = root;

		LYNXBOOL Result = LYNX_FALSE;

		if (!Root)
			Root = m_Doc->RootElement();

		CXMLElement* Element = Root->FirstChildElement(key.c_str());
		if (Element)
		{
			CXMLNode* Node = Element->FirstChild();
			if (Node)
			{
				CXMLText* Text = Node->ToText();
				if (Text)
				{
					Result = LYNX_TRUE;					
				}
			}
		}
		return Result;
	}
}
