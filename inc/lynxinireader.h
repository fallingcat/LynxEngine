#ifndef __LYNXINIREADER_H__
#define __LYNXINIREADER_H__

#include <LynxEngineClasses.h>

namespace LynxEngine
{
	class LYNXENGCLASS CIniReader : public CUnknown
	{
	protected:
		LYNXCHAR				m_FileName[256];
		CMap<CString, CMap<CString, CString> >	m_ParamMap;
		//CMap<CString, CString>  m_ParamMap;
        unsigned int            m_Encoding;
	public:
		CIniReader(const LYNXCHAR* filename, unsigned int encoder = CStream::ANSI);
		~CIniReader();

		void					Prefetch();
		int						ReadInt(const LYNXCHAR* szSection, const LYNXCHAR* szKey, int iDefaultValue);
		float					ReadFloat(const LYNXCHAR* szSection, const LYNXCHAR* szKey, float fltDefaultValue);
		LYNXBOOL				ReadBool(const LYNXCHAR* szSection, const LYNXCHAR* szKey, LYNXBOOL bolDefaultValue);
		CString					ReadString(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCHAR* szDefaultValue);	
		LYNXCOLORRGBA			ReadRGBA(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCOLORRGBA& szDefaultValue);	
		LYNXCOLORRGB			ReadRGB(const LYNXCHAR* szSection, const LYNXCHAR* szKey, const LYNXCOLORRGB& szDefaultValue);
	protected:
		LYNXCHAR*				ReadString(LYNXCHAR* dest, const LYNXCHAR* szSection, const LYNXCHAR* szKey);
	};
}
#endif