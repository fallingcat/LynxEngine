#ifndef __LYNXGAMEPROFILE_H__
#define __LYNXGAMEPROFILE_H__

#include <LynxEngineClasses.h>
#include <LynxUserProfile.h>

namespace LynxEngine
{
	class LYNXENGCLASS CGameProfile : public CUserProfile
	{
	protected:				
	public:
		CGameProfile(const CString& filename);
		~CGameProfile();		
		
		virtual LYNXBOOL		vLoad(); 
		virtual void			vSave();
	};
}
#endif