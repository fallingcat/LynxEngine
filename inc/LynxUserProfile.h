#ifndef __LYNXUSERPROFILE_H__
#define __LYNXUSERPROFILE_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine
{
	class LYNXENGCLASS CUserProfile : public CObj
	{
	public:
		LYNXBOOL				m_bUseCheckSum;
	protected:		
		CXMLDoc*				m_Doc;
		CXMLElement*			m_Root;
		CString					m_Filename;
		CAnsiString				m_Password;
		CString					m_CheckSumString;
		CString					m_CheckSumString2;
	public:
		CUserProfile(const CString& filename);
		~CUserProfile();
		

		LYNXBOOL				Load(); 
		void					Save();

		const CString&			GetFilename() const {return m_Filename;}	
		void					SetFilename(const CString& f) {m_Filename = f;}	
		void					WriteDefaultElements();
		LYNXFORCEINLINE void	SetPassword(const CAnsiString& pw) {m_Password = pw;}

		void					SetVersion(const CString& v);
		CString					GetVersion();

		LYNXBOOL				GenerateCKS();

		LYNXFORCEINLINE CXMLDoc* GetDoc() {return  m_Doc; }		
		LYNXFORCEINLINE CXMLElement* GetRootElement() {return m_Root; }
		LYNXFORCEINLINE CString& SetCheckSumString() {return m_CheckSumString; }

		LYNXBOOL				Find(const CAnsiString& key, CXMLElement* root = NULL);

		CString					GetValue(CXMLElement* node);
		int						Read(const CAnsiString& key, const int default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const int value, CXMLElement* root = NULL);

		unsigned int			Read(const CAnsiString& key, const unsigned int default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const unsigned int value, CXMLElement* root = NULL);

		unsigned long			Read(const CAnsiString& key, const unsigned long default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const unsigned long value, CXMLElement* root = NULL);

		LYNXULONGLONG			Read(const CAnsiString& key, const LYNXULONGLONG default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const LYNXULONGLONG value, CXMLElement* root = NULL);

		float					Read(const CAnsiString& key, const float default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const float value, CXMLElement* root = NULL);

		LYNXBOOL				Read(const CAnsiString& key, const LYNXBOOL default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const LYNXBOOL value, CXMLElement* root = NULL);

		CString					Read(const CAnsiString& key, const CString& default_value, CXMLElement* root = NULL);	
		void					Write(const CAnsiString& key, const CString& value, CXMLElement* root = NULL);	

		LYNXCOLORRGBA			Read(const CAnsiString& key, const LYNXCOLORRGBA& default_value, CXMLElement* root = NULL);		
		void					Write(const CAnsiString& key, const LYNXCOLORRGBA& value, CXMLElement* root = NULL);		

		LYNXCOLORRGB			Read(const CAnsiString& key, const LYNXCOLORRGB& default_value, CXMLElement* root = NULL);		
		void					Write(const CAnsiString& key, const LYNXCOLORRGB& value, CXMLElement* root = NULL);		

		LYNXQUATERNION			Read(const CAnsiString& key, const LYNXQUATERNION& default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const LYNXQUATERNION& value, CXMLElement* root = NULL);

		LYNXRECT				Read(const CAnsiString& key, const LYNXRECT& default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const LYNXRECT& value, CXMLElement* root = NULL);

		CVector2				Read(const CAnsiString& key, const CVector2& default_value, CXMLElement* root = NULL);
		void					Write(const CAnsiString& key, const CVector2& value, CXMLElement* root = NULL);

		CVector3				Read(const CAnsiString& key, const CVector3& default_value, CXMLElement* root = NULL);		
		void					Write(const CAnsiString& key, const CVector3& value, CXMLElement* root = NULL);		
		
		template <class T>
		CArray<T>				Read(const CAnsiString& key, const CArray<T>& default_value, CXMLElement* root = NULL)
		{
			CArray<T> Result = default_value;
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
				for (typename CArray<T>::CIterator Pos=Result.begin(); Pos!=Result.end(); Pos++)
				{					
					CXMLNode* Node = Value->FirstChild();
					if (Node)
					{
						CXMLText* Text = Node->ToText();
						if (Text)
						{				
							T Value;
							CString Str = CString(Text->Value());
							Str.ToValue(Value);
							*Pos = Value;				
						}
					}
					Value = Value->NextSiblingElement();
				}			
			}			
			return Result;
		}		
		template <class T>
		void					Write(const CAnsiString& key, const CArray<T>& value, CXMLElement* root = NULL)
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
				for (typename CArray<T>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
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
				for (typename CArray<T>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Value = m_Doc->NewElement("Value");		
					Element->InsertEndChild(Value);
					lynxSprintf(Buffer, _T("%d"), *Pos);
					CXMLText* Text = m_Doc->NewText(Buffer);
					Value->InsertEndChild(Text);				
				}
			}
		}

		template <class T1, class T2>
		CMap<T1, T2>				Read(const CAnsiString& key, const CMap<T1, T2>& default_value, CXMLElement* root = NULL)
		{
			CMap<T1, T2> Result;
			int Size = 0;
		
			CXMLElement* Root = root;
		
			if (!Root)
				Root = m_Doc->RootElement();

			CXMLElement* Element = Root->FirstChildElement(key.c_str());
			if (Element)
			{
				Size = Element->IntAttribute("Size");
				Result.clear();
				
				CXMLElement* Data = Element->FirstChildElement("Data");
				if  (Data)
				{					
					T1 Key = Data->Attribute("Key");
					T2 Value = Data->IntAttribute("Value");
					Result[Key] = Value;					
					Value = Value->NextSiblingElement();
				}			
			}			
			return Result;
		}

		template <class T1, class T2>
		void					Write(const CAnsiString& key, const CMap<T1, T2>& value, CXMLElement* root = NULL)
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
				for (typename CMap<T1, T2>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Data = m_Doc->NewElement("Data");		
					Element->InsertEndChild(Data);
					Data->SetAttribute("Key", (*Pos).first);
					Data->SetAttribute("Value", (*Pos).second);
				}
			}
			else
			{
				Element = m_Doc->NewElement(key.c_str());		
				Root->InsertEndChild(Element);
				lynxSprintf(Buffer, _T("%d"), value.size());
				Element->SetAttribute("Size", Buffer);
				for (typename CMap<T1, T2>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Data = m_Doc->NewElement("Data");
					Element->InsertEndChild(Data);
					Data->SetAttribute("Key", (*Pos).first);
					Data->SetAttribute("Value", (*Pos).second);
				}
			}
		}

		void					Write(const CAnsiString& key, const CArray<CString>& value, CXMLElement* root = NULL)
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
				for (CArray<CString>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Value = m_Doc->NewElement("Value");		
					Element->InsertEndChild(Value);					
					CXMLText* Text = m_Doc->NewText((*Pos));
					Value->InsertEndChild(Text);				
				}
			}
			else
			{
				Element = m_Doc->NewElement(key.c_str());		
				Root->InsertEndChild(Element);
				lynxSprintf(Buffer, _T("%d"), value.size());
				Element->SetAttribute("Size", Buffer);
				for (CArray<CString>::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Value = m_Doc->NewElement("Value");		
					Element->InsertEndChild(Value);					
					CXMLText* Text = m_Doc->NewText((*Pos));
					Value->InsertEndChild(Text);				
				}
			}
		}

		CList<int>				Read(const CAnsiString& key, const CList<int>& default_value, CXMLElement* root = NULL);		
		void					Write(const CAnsiString& key, const CList<int>& value, CXMLElement* root = NULL);		

		CList<unsigned long>	Read(const CAnsiString& key, const CList<unsigned long>& default_value, CXMLElement* root = NULL);		
		void					Write(const CAnsiString& key, const CList<unsigned long>& value, CXMLElement* root = NULL);	

		void					Write(const CAnsiString& key, const CMatrix4& value, CXMLElement* root = NULL);	

		CXMLText* AllocText(const CString& s)
		{
			return m_Doc->NewText(s.c_str());
		}
		void GetValue(CString& s, const CXMLText* t)
		{
			s = t->Value();				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		void WriteC(const CAnsiString& key, const T& value, CXMLElement* root = NULL)
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
				for (typename T::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Value = m_Doc->NewElement("Value");		
					Element->InsertEndChild(Value);
					Value->InsertEndChild(AllocText((*Pos)));				
				}
			}
			else
			{
				Element = m_Doc->NewElement(key.c_str());		
				m_Root->InsertEndChild(Element);
				lynxSprintf(Buffer, _T("%d"), value.size());
				Element->SetAttribute("Size", Buffer);
				for (typename T::const_iterator Pos = value.begin(); Pos != value.end(); Pos++)
				{
					CXMLElement* Value = m_Doc->NewElement("Value");		
					Element->InsertEndChild(Value);
					Value->InsertEndChild(AllocText((*Pos)));				
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		template <typename T> 
		T ReadC(const CAnsiString& key, const T& default_value, CXMLElement* root = NULL)
		{
			T Result = default_value;
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
				for (typename T::CIterator Pos=Result.begin(); Pos!=Result.end(); Pos++)
				{					
					CXMLNode* Node = Value->FirstChild();
					if (Node)
					{
						CXMLText* Text = Node->ToText();
						if (Text)
						{				
							GetValue(*Pos, Text);
						}
					}
					Value = Value->NextSiblingElement();
				}			
			}	
		
			return Result;
		}
	};
}
#endif