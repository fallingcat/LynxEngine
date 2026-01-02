#if defined(__WXWIDGET__)

#ifndef __LYNXLOGPAGE_H__
#define __LYNXLOGPAGE_H__

#include <LynxEngineClasses.h>
#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{	
		class LYNXENGCLASS CLogPage : public wxPanel, public COutputDevice
		{
			LYNX_DECLARE_CLASS(CLogPage);
		protected:
			CControlPanel*				m_lpControlPanel;
			wxTextCtrl*					m_LogTexWnd;
		public:
			// ctor(s)
			CLogPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size);
			
			void OnSize(wxSizeEvent& event);					

			void vOpen(const CString& name);
			void vClose();
			void vClear();
			void vAppend(const CString& str);
			void vOutput(const CString& str);
		private:
			// any class wishing to process wxWidgets events must use this macro
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

#endif
