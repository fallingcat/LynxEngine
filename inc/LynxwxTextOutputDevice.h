#if defined(__WXWIDGET__)

#ifndef __LYNXWXTEXTOUTPUTDEVICE_H__
#define __LYNXWXTEXTOUTPUTDEVICE_H__

#include <LynxEngineClasses.h>
#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	// Define a new frame type: this is going to be our main frame
	class LYNXENGCLASS CwxTextOutputDevice : public wxFrame, public COutputDevice
	{
		//LYNX_DECLARE_CLASS(CHTMLOutputDevice);
	protected:
		CEngine*					m_lpEngine;
		wxBookCtrl*					m_Book;
		wxTextCtrl*					m_LogTexWnd;
		wxListCtrl*					m_ResourceListWnd;
		wxButton*					m_Refresh;
	public:
		// ctor(s)
		CwxTextOutputDevice(CEngine* lpe, const wxString& title, const wxPoint& pos, const wxSize& size);

		LYNXINLINE void SetlpEngine(CEngine* lpe) {m_lpEngine = lpe; };

		void vOpen(const CString& name); 
		void vClose();
		void vClear();
		void vAppend(const CString& str); 
		void vOutput(const CString& str);		

		void RefreshResource();

		void OnSize(wxSizeEvent& event);		
		void OnRefresh(wxCommandEvent &event);
	protected:
		 DECLARE_EVENT_TABLE()
	private:
		// any class wishing to process wxWidgets events must use this macro
		//DECLARE_EVENT_TABLE()
	};
}
#endif

#endif
