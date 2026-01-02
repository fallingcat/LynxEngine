#if defined(__WXWIDGET__)

#ifndef __LYNXMEMORYPAGE_H__
#define __LYNXMEMORYPAGE_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{	
		class LYNXENGCLASS CMemoryPage : public wxPanel
		{
			LYNX_DECLARE_CLASS(CMemoryPage);
		protected:
			CControlPanel*				m_lpControlPanel;
			wxListCtrl*					m_MemoryBlockListWnd;
			wxButton*					m_Refresh;
		public:
			// ctor(s)
			CMemoryPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size);
			void Refresh();

			void OnSize(wxSizeEvent& event);		
			void OnRefresh(wxCommandEvent &event);
		private:
			// any class wishing to process wxWidgets events must use this macro
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

#endif
