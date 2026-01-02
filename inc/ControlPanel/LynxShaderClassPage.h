#if defined(__WXWIDGET__)

#ifndef __LYNXSHADERCLASSPAGE_H__
#define __LYNXSHADERCLASSPAGE_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{		
		// Define a new frame type: this is going to be our main frame
		class LYNXENGCLASS CShaderClassPage : public wxPanel
		{
			LYNX_DECLARE_CLASS(CShaderClassPage);
		public:
			long						m_SortType;
		protected:
			CControlPanel*				m_lpControlPanel;
			wxListCtrl*					m_ResourceListWnd;
			wxButton*					m_Refresh;			
		public:
			// ctor(s)
			CShaderClassPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size);
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
