#ifndef __LYNXREADONLYRPROFILE_H__
#define __LYNXREADONLYRPROFILE_H__

#include <LynxUserProfile.h>

namespace LynxEngine
{
	class LYNXENGCLASS CReadOnlyProfile : public CUserProfile
	{
	protected:				
	public:
		CReadOnlyProfile(const CString& filename);
		~CReadOnlyProfile();
		

		LYNXBOOL				Load(); 
		void					Save() {};

		void					WriteDefaultElements() {};
		LYNXFORCEINLINE void	SetPassword(const CAnsiString& pw) {m_Password = pw;}

		void					SetVersion(const CString& v) {};
		
		void					Write(const CAnsiString& key, const int value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const unsigned long value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXULONGLONG value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const float value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXBOOL value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const CString& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXCOLORRGBA& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXCOLORRGB& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXQUATERNION& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const LYNXRECT& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const CVector2& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const CVector3& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const CList<int>& value, CXMLElement* root = NULL) {};
		void					Write(const CAnsiString& key, const CList<unsigned long>& value, CXMLElement* root = NULL) {};
		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		void WriteC(const CAnsiString& key, const T& value)
		{			
		}		
	};
}
#endif