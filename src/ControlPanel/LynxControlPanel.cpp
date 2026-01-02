#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxEngine.h>
#include <ControlPanel/LynxControlPanel.h>
#include <ControlPanel/LynxResourcePage.h>
#include <ControlPanel/LynxMemoryPage.h>
#include <ControlPanel/LynxSmallObjAllocatorPage.h>
#include <ControlPanel/LynxShaderClassPage.h>
#include <ControlPanel/LynxLogPage.h>

namespace LynxEngine
{
	namespace ControlPanel
	{
		BEGIN_EVENT_TABLE(CControlPanel, wxFrame)
		EVT_SIZE(CControlPanel::OnSize)
		END_EVENT_TABLE()
		// ----------------------------------------------------------------------------
		// main frame
		// ----------------------------------------------------------------------------
		// frame constructor
		CControlPanel::CControlPanel(CEngine* lpe, const wxString& title, const wxPoint& pos, const wxSize& size)
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
			m_LogTexWnd = new wxTextCtrl(Panel, 1, wxEmptyString, wxDefaultPosition, size, wxTE_MULTILINE | wxTE_DONTWRAP);
			//Panel->SetCursor(wxCursor(wxCURSOR_HAND));
			m_Book->AddPage(Panel, _T("Log"), true);

			m_NetworkLogOutputDevice = new CLogPage(this, _T("Network Log"), wxDefaultPosition, size);
			m_NetClientLogOutputDevice = new CLogPage(this, _T("NetClient Log"), wxDefaultPosition, size);

			m_Book->AddPage(static_cast<CLogPage*>(m_NetworkLogOutputDevice), _T("Network Log"), true);
			m_Book->AddPage(static_cast<CLogPage*>(m_NetClientLogOutputDevice), _T("NetClient Log"), true);
			m_Book->AddPage(new CResourcePage(this, _T("Resource"), wxDefaultPosition, size), _T("Resource"), true);
			m_Book->AddPage(new CShaderClassPage(this, _T("Shade Class"), wxDefaultPosition, size), _T("Shader Class"), true);
			m_Book->AddPage(new CMemoryPage(this, _T("Memory"), wxDefaultPosition, size), _T("Memory"), true);
			m_Book->AddPage(new CSmallObjAllocatorPage(this, _T("Small Object"), wxDefaultPosition, size), _T("Small Object"), true);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::vOpen(const CString& name)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::vClose()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::vClear()
		{
			m_LogTexWnd->Clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::vAppend(const CString& str)
		{
			m_LogTexWnd->AppendText(str.c_str());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::vOutput(const CString& str)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::OnSize(wxSizeEvent& e)
		{
			wxRect ClientRect = GetClientRect();
			ClientRect.SetWidth(ClientRect.GetWidth() - 20);
			ClientRect.SetHeight(ClientRect.GetHeight() - 20);
			m_Book->SetSize(ClientRect);
			m_LogTexWnd->SetSize(ClientRect);

			e.Skip();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CControlPanel::SelectPage(const wxString& name)
		{
			int PageIndex = 0;
			const wxNotebookPageInfoList& InfoList = m_Book->GetPageInfos();
			for (wxNotebookPageInfoList::const_iterator CIter = InfoList.begin(); CIter != InfoList.end(); ++CIter, ++PageIndex)
			{
				if (name == (*CIter)->GetText())
					break;
			}

			m_Book->SetSelection(PageIndex);
		}
	}
}

#endif