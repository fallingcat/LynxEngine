#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxHTMLOutputDevice.h>

namespace LynxEngine 
{
	static LYNXCHAR HtmlSrc[] = _T("<html>\
									<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\">\
									<body bgcolor=\"#302C28\" text=\"#CCCCCC\" link=\"#FFCC00\" vlink=\"#FFCC00\" alink=\"#FFCC00\">\
									<p align=\"center\"><font face=\"Arial, Helvetica, sans-serif\"><b><font size=\"5\" color=\"#FFCC00\">LynxEngine Log</font></b></font></p>\
									<hr width=\"100%\" align=\"left\" size=\"1\" color=\"6699ff\" noshade>");							
	// ----------------------------------------------------------------------------
	// constants
	// ----------------------------------------------------------------------------

	// IDs for the controls and the menu commands
	enum
	{
		// menu items
		Minimal_Quit = 1,
		Minimal_Back,
		Minimal_Forward,

		// controls start here (the numbers are, of course, arbitrary)
		Minimal_Text = 1000
	};

	// ----------------------------------------------------------------------------
	// event tables and other macros for wxWidgets
	// ----------------------------------------------------------------------------

	// the event tables connect the wxWidgets events with the functions (event
	// handlers) which process them. It can be also done at run-time, but for the
	// simple menu events like this the static method is much simpler.
	BEGIN_EVENT_TABLE(CHTMLOutputDevice, wxFrame)
		EVT_MENU(Minimal_Quit,  CHTMLOutputDevice::OnQuit)
		EVT_MENU(Minimal_Back, CHTMLOutputDevice::OnBack)
		EVT_MENU(Minimal_Forward, CHTMLOutputDevice::OnForward)
	END_EVENT_TABLE()

	// ============================================================================
	// implementation
	// ============================================================================

	//CHTMLOutputDevice *frame = new CHTMLOutputDevice( _("wxHtmlWindow testing application"),
	//		wxDefaultPosition, wxSize(640, 480) );
	//frame->Show(true);
	//SetTopWindow(frame);

	// ----------------------------------------------------------------------------
	// main frame
	// ----------------------------------------------------------------------------
	// frame constructor
	CHTMLOutputDevice::CHTMLOutputDevice(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size)
	{
		// create a menu bar
		wxMenu *menuFile = new wxMenu;
		wxMenu *menuNav = new wxMenu;

		menuFile->Append(Minimal_Quit, _("E&xit"));
		menuNav->Append(Minimal_Back, _("Go &BACK"));
		menuNav->Append(Minimal_Forward, _("Go &FORWARD"));

		// now append the freshly created menu to the menu bar...
		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuFile, _("&File"));
		menuBar->Append(menuNav, _("&Navigate"));

		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);

		//SetIcon(wxIcon(sample_xpm));
	    
	#if wxUSE_STATUSBAR
		CreateStatusBar(2);
	#endif // wxUSE_STATUSBAR

		m_HtmlWnd = new wxHtmlWindow(this);
		m_HtmlWnd->SetRelatedFrame(this, _("wxHTML Demo: '%s'"));
	#if wxUSE_STATUSBAR
		m_HtmlWnd->SetRelatedStatusBar(1);
	#endif // wxUSE_STATUSBAR
		//m_HtmlWnd->LoadPage("start.htm"));
		m_HtmlWnd->SetPage(HtmlSrc);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	// event handlers
	void CHTMLOutputDevice::OnQuit(wxCommandEvent& WXUNUSED(event))
	{
		// true is to force the frame to close
		Close(true);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::OnBack(wxCommandEvent& WXUNUSED(event))
	{
		if (!m_HtmlWnd-> HistoryBack()) wxMessageBox(_("You reached prehistory era!"));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::OnForward(wxCommandEvent& WXUNUSED(event))
	{
		if (!m_HtmlWnd-> HistoryForward()) wxMessageBox(_("No more items in history!"));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::vOpen(const CString& name) 
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::vClose() 
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::vClear()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::vAppend(const CString& str) 
	{
		m_HtmlWnd->AppendToPage(_T("<p align=\"left\"><font face=\"Verdana, Helvetica, sans-serif\">"));	

		LYNXCHAR Str[256], *Str1;
		lynxStrCpy(Str, str.c_str());
		Str1 = lynxStrTok(Str, _T("("));
		if (Str1)
		{
			m_HtmlWnd->AppendToPage(_T("<font size=\"2\" color=\"#FFCC00\">"));			
			m_HtmlWnd->AppendToPage(Str1);
			m_HtmlWnd->AppendToPage(_T("</font></font></p>\n"));					
		}
		Str1 = lynxStrTok(NULL, _T(")"));
		if (Str1)
		{
			m_HtmlWnd->AppendToPage(_T("<font size=\"2\" color=\"#FFCC00\">"));			
			m_HtmlWnd->AppendToPage(_T("("));
			m_HtmlWnd->AppendToPage(_T("</font></font></p>\n"));					
			m_HtmlWnd->AppendToPage(_T("<font size=\"2\" color=\"#FFFFFF\">"));
			m_HtmlWnd->AppendToPage(Str1);		
			m_HtmlWnd->AppendToPage(_T("</font></font></p>\n"));		
			m_HtmlWnd->AppendToPage(_T("<font size=\"2\" color=\"#FFCC00\">"));			
			m_HtmlWnd->AppendToPage(_T(")"));
			m_HtmlWnd->AppendToPage(_T("</font></font></p>\n"));					
		}
		Str1 = lynxStrTok(NULL, _T("\n"));
		if (Str1)
		{
			m_HtmlWnd->AppendToPage(_T("<font size=\"2\" color=\"#FFCC00\">"));			
			m_HtmlWnd->AppendToPage(Str1);
			m_HtmlWnd->AppendToPage(_T("</font></font></p>\n"));		
		}
		//m_HtmlWnd->AppendToPage("<font size=\"2\" color=\"#FFCC00\">"));
		//m_HtmlWnd->AppendToPage(str.c_str()));
		//m_HtmlWnd->AppendToPage("</font></font></p>\n"));							
		m_HtmlWnd->AppendToPage(_T("</body>"));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CHTMLOutputDevice::vOutput(const CString& str) 
	{
		
	}
}
#endif