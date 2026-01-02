#ifndef __LYNXDUMMYCONTROL_H__
#define __LYNXDUMMYCONTROL_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CDummyControl : public CControl
		{
			LYNX_DECLARE_CLASS(CDummyControl);	
		protected:							
		public:			
			CDummyControl(CGUISystem* lpsys);
			virtual ~CDummyControl();	

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			DWORD						vSetAlignmentType(const DWORD type);
			void						vSetAlignmentRect(const LYNXRECT& r);
			void						vGetAlignmentRect(LYNXRECT& r);
			void						vRender();
			void						vOnHit();
            LYNXBOOL                    vHitTest(const LYNXPOINT2D* p);

			void						vSetSize(float w, float h) {};
			void						vSetPosition(const Math::CVector3& pos) {};
			void						vGetPosition(Math::CVector3& pos) {};			
		};
	}
}
#endif

