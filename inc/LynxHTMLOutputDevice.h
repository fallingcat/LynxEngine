#if defined(__WXWIDGET__)

#ifndef __LYNXHTMLOUTPUTDEVICE_H__
#define __LYNXHTMLOUTPUTDEVICE_H__

#include <LynxEngineClasses.h>
#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	// Define a new frame type: this is going to be our main frame
	class LYNXENGCLASS CHTMLOutputDevice : public wxFrame, public COutputDevice
	{
		//LYNX_DECLARE_CLASS(CHTMLOutputDevice);
	protected:
		wxHtmlWindow*					m_HtmlWnd;
	public:
		// ctor(s)
		CHTMLOutputDevice(const wxString& title, const wxPoint& pos, const wxSize& size);

		void vOpen(const CString& name); 
		void vClose();
		void vClear();
		void vAppend(const CString& str); 
		void vOutput(const CString& str);

		// event handlers (these functions should _not_ be virtual)
		void OnQuit(wxCommandEvent& event);
		void OnBack(wxCommandEvent& event);
		void OnForward(wxCommandEvent& event);

	private:
		// any class wishing to process wxWidgets events must use this macro
		DECLARE_EVENT_TABLE()
	};
}
#endif

#endif