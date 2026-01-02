#ifndef __LYNXHITMASK_H__
#define __LYNXHITMASK_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxButton.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CHitMask : public LynxEngine::CObj
		{
			LYNX_DECLARE_CLASS(CHitMask);	
		protected:				
			int							m_Width, m_Height;
			CArray< CArray<LYNXBOOL> >	m_Array;					
		public:			
			CHitMask(CEngine* lpe);
			virtual ~CHitMask();	
			
			void						CreateFromFile(const CString& filename);
			void						CreateFromArray(const int w, const int h, const LYNXBOOL* data);
			LYNXBOOL					HitTest(int x, int y, int w, int h);
		};
	}
}
#endif

