#ifndef __LYNXDIALOG_H__
#define __LYNXDIALOG_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxWindow.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CDialog : public CWindow
		{
			LYNX_DECLARE_CLASS(CDialog);	
		protected:		
		public:
			CDialog(CGUISystem* lpsys);
			virtual ~CDialog();	

			DECLARE_CLASSSCRIPT

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);			
			void						vLoop(float step);
			void						vRender();					
			
			virtual void				vDismiss();
		};
	}
}
#endif

