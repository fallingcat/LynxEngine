#ifndef __LYNXPLAYERPROFILE_H__
#define __LYNXPLAYERPROFILE_H__

#include <LynxEngineClasses.h>
#include <LynxUserProfile.h>

namespace LynxEngine
{
	class LYNXENGCLASS CPlayerProfile : public CUserProfile
	{
	protected:		
		CString					m_Name;		
	public:
		CPlayerProfile(const CString& filename);
		~CPlayerProfile();		

		LYNXFORCEINLINE const CString GetName() const {return m_Name; };
		LYNXFORCEINLINE void SetName(const CString& name) {m_Name = name; };		

		virtual LYNXBOOL		vLoad(); 
		virtual void			vSave();
	};
}
#endif