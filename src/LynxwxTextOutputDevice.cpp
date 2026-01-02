#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxwxTextOutputDevice.h>

namespace LynxEngine 
{	
	#define ID_REFRESH							2

	BEGIN_EVENT_TABLE(CwxTextOutputDevice, wxFrame)
		EVT_SIZE    (						CwxTextOutputDevice::OnSize)		
		EVT_BUTTON  (ID_REFRESH,			CwxTextOutputDevice::OnRefresh)
	END_EVENT_TABLE()
	// ----------------------------------------------------------------------------
	// main frame
	// ----------------------------------------------------------------------------
	// frame constructor
	CwxTextOutputDevice::CwxTextOutputDevice(CEngine* lpe, const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size), m_lpEngine(lpe)
	{
		// create a menu bar
		//wxMenu *menuFile = new wxMenu;
		//wxMenu *menuNav = new wxMenu;

		//menuFile->Append(Minimal_Quit, _("E&xit"));
		//menuNav->Append(Minimal_Back, _("Go &BACK"));
		//menuNav->Append(Minimal_Forward, _("Go &FORWARD"));

		// now append the freshly created menu to the menu bar...
		//wxMenuBar *menuBar = new wxMenuBar;
		//menuBar->Append(menuFile, _("&File"));
		//menuBar->Append(menuNav, _("&Navigate"));

		// ... and attach this menu bar to the frame
		//SetMenuBar(menuBar);

		//SetIcon(wxIcon(sample_xpm));	    

		m_Book = new wxBookCtrl(this, wxNB_BOTTOM);

		wxPanel* Panel = new wxPanel(m_Book);
		m_LogTexWnd = new wxTextCtrl(Panel, 1, wxEmptyString, wxDefaultPosition, size, wxTE_MULTILINE);		
		//Panel->SetCursor(wxCursor(wxCURSOR_HAND));
		m_Book->AddPage(Panel, _T("Log"), true);

		Panel = new wxPanel(m_Book);
		m_ResourceListWnd = new wxListCtrl(Panel, 1, wxDefaultPosition, size, wxLC_REPORT|wxLC_ALIGN_LEFT|wxLC_SORT_ASCENDING);		
		wxListItem ItemCol;
		ItemCol.SetText(_T("Name"));
		ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
		ItemCol.SetImage(-1);		
		m_ResourceListWnd->InsertColumn(0, ItemCol);
		ItemCol.SetText(_T("Type"));
		ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
		m_ResourceListWnd->InsertColumn(1, ItemCol);
		ItemCol.SetText(_T("Count"));
		ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
		m_ResourceListWnd->InsertColumn(2, ItemCol);
		m_ResourceListWnd->Show();
		m_Refresh = new wxButton(Panel, ID_REFRESH, _T("Refresh"), wxPoint(180,30), wxSize(140,30) );
		m_Book->AddPage(Panel, _T("Resource"), true);		
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::vOpen(const CString& name) 
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::vClose() 
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::vClear()
	{
		m_LogTexWnd->Clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::vAppend(const CString& str) 
	{
		m_LogTexWnd->AppendText(str.c_str());
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::vOutput(const CString& str) 
	{	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::RefreshResource()
	{
		m_ResourceListWnd->DeleteAllItems();
		
		CList<CResourcePtr>::CIterator Res;
		CList<CResourcePtr>& ResList = m_lpEngine->GetResourceManager().GetObjectList();
		int Num = 0;
		for (Res = ResList.begin(); Res != ResList.end(); ++Res)
		{
			wxString buf;
			buf.Printf(_T("%s"), (*Res)->GetName().c_str());
			long Tmp = m_ResourceListWnd->InsertItem(Num, buf, 0);
			m_ResourceListWnd->SetItemData(Tmp, Num);

			buf.Printf(_T("%s"), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str());
			m_ResourceListWnd->SetItem(Tmp, 1, buf);

			buf.Printf(_T("%d"), (*Res).GetCount());
			m_ResourceListWnd->SetItem(Tmp, 2, buf);

			//lynxSprintf(Buffer, _T("%s, Type: %s, Count: %d"), (*Res)->GetName().c_str(), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str(), (*Res).GetCount());
			//m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10+4*5, CString(Buffer), &Color, &ShadowColor);
			Num++;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::OnSize(wxSizeEvent& e)
	{
		wxRect ClientRect = GetClientRect();		
		ClientRect.SetWidth(ClientRect.GetWidth()-20);
		ClientRect.SetHeight(ClientRect.GetHeight()-20);
		m_Book->SetSize(ClientRect);
		m_LogTexWnd->SetSize(ClientRect);

		wxRect Rect(ClientRect.GetX(), ClientRect.GetY(), ClientRect.GetWidth(), ClientRect.GetHeight()*0.85f);		
		m_ResourceListWnd->SetSize(Rect);				
		m_Refresh->SetSize(ClientRect.GetRight() - 150, Rect.GetBottom() + 10, 140, 30);

		m_ResourceListWnd->SetColumnWidth(0, Rect.GetWidth()*0.6f);
		m_ResourceListWnd->SetColumnWidth(1, Rect.GetWidth()*0.3f);
		m_ResourceListWnd->SetColumnWidth(2, Rect.GetWidth()*0.1f);
		
		e.Skip();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CwxTextOutputDevice::OnRefresh(wxCommandEvent& e)
	{
		switch (e.GetId())
		{
			case ID_REFRESH:
				RefreshResource();
				break;
		}
	}
}

#endif