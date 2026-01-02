#ifndef __LYNXIMAGE_H__
#define __LYNXIMAGE_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxButton.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CImage : public CButton
		{
			LYNX_DECLARE_CLASS(CImage);	
		protected:				
			//CContainer*					m_lpContainer;            
			//Math::CVector2				m_Size;
			//Math::CReal					m_Scale;	
		public:			
			CImage(CGUISystem* lpsys);
			virtual ~CImage();	
			
			DECLARE_CLASSSCRIPT

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			//CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			//DWORD						vSetAlignmentType(const DWORD type);
			//void						vSetAlignmentRect(const LYNXRECT& r);
			//void						vGetAlignmentRect(LYNXRECT& r);
			void						vRender();
			//void						vSetSize(float w, float h);			
			void						vOnHit();
			//LYNXFORCEINLINE void		vSetPosition(const Math::CVector3& pos) {m_lpContainer->vSetPosition(pos); };			
			//LYNXFORCEINLINE void		vGetPosition(Math::CVector3& pos) {m_lpContainer->vGetPosition(pos); };						           
			
			//LYNXFORCEINLINE	float		GetScale() {return m_Scale; };		
			//LYNXFORCEINLINE	void		Scale(const float s) {m_Scale = s; };					
                        			
			//LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; };
		};
	}
}
#endif

