//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXANDROIDFILESYSTEM_H__
#define __LYNXANDROIDFILESYSTEM_H__

#include <FileSystem/LynxFileSystem.h>

namespace LynxEngine 
{
	namespace FileSystem 
	{
		namespace Android
		{
			class LYNXENGCLASS CFileSystem : public LynxEngine::FileSystem::CFileSystem
			{
				LYNX_DECLARE_CLASS(CFileSystem);
			private:		
			protected:
			public:			
				CFileSystem(CEngine* const lpengine);
				~CFileSystem();			

				LYNXBOOL							vCreate(void);
				virtual CString						vProcessPath(const CString& path);
            };
		}
	}
}

#endif