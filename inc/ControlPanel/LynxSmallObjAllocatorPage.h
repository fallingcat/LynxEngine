#if defined(__WXWIDGET__)

#ifndef __LYNXSMALLOBJALLOCATORPAGE_H__
#define __LYNXSMALLOBJALLOCATORPAGE_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{	
		class LYNXENGCLASS CSmallObjAllocatorPage : public wxPanel
		{
			LYNX_DECLARE_CLASS(CSmallObjAllocatorPage);
		protected:
			CControlPanel*				m_lpControlPanel;
			wxListCtrl*					m_FixedAllocatorListWnd;
			wxButton*					m_Refresh;
		public:
			// ctor(s)
			CSmallObjAllocatorPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size);
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
