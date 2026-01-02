#if defined(__WXWIDGET__)

#ifndef __LYNXRESOURCEPAGE_H__
#define __LYNXRESOURCEPAGE_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{		
		// Define a new frame type: this is going to be our main frame
		class LYNXENGCLASS CResourcePage : public wxPanel
		{
			LYNX_DECLARE_CLASS(CResourcePage);
		public:
			long						m_SortType;
		protected:
			CControlPanel*				m_lpControlPanel;
			wxListCtrl*					m_ResourceListWnd;
			wxButton*					m_Refresh;			
		public:
			// ctor(s)
			CResourcePage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size);
			void RefreshResource();

			CControlPanel* GetlpControlPanel() {return m_lpControlPanel; };
			void OnSize(wxSizeEvent& event);		
			void OnRefresh(wxCommandEvent &event);
			void OnSort(wxListEvent& e);
		private:
			// any class wishing to process wxWidgets events must use this macro
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

#endif
