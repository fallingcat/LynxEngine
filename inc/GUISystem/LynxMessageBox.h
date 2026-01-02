#ifndef __LYNXMESSAGEBOX_H__
#define __LYNXMESSAGEBOX_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxWindow.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CText;
		class CMessageBox : public CWindow
		{
			LYNX_DECLARE_CLASS(CMessageBox);	
		public:
			typedef enum {
				RESULT_NONE = 0,
				RESULT_OK,
				RESULT_CANCEL,
			}RESULT;

			RESULT						m_Result;
		protected:				
			GUISystem::CText*			m_Title;		
			GUISystem::CText*			m_Text;	
			GUISystem::CButton*			m_OKButton;
		public:			
			CMessageBox(CGUISystem* lpsys);
			virtual ~CMessageBox();	

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);			
			void						vLoop(float step);
			void						vRender();	

			void						SetTitle(const CString& text);
			void						SetText(const CString& text);
			void						Display();
			void						Dismiss();
		};
	}
}
#endif

