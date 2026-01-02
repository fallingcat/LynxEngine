#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxEngine.h>
#include <ControlPanel/LynxControlPanel.h>
#include <ControlPanel/LynxLogPage.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{
		BEGIN_EVENT_TABLE(CLogPage, wxPanel)
			EVT_SIZE    (						CLogPage::OnSize)					
		END_EVENT_TABLE()
		// ----------------------------------------------------------------------------
		// main frame
		// ----------------------------------------------------------------------------
		// frame constructor
		CLogPage::CLogPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxPanel(lpc->GetlpBookCtrl(), pos.x, pos.y, size.GetWidth(), size.GetHeight(), wxTAB_TRAVERSAL, title), m_lpControlPanel(lpc)
		{
			m_LogTexWnd = new wxTextCtrl(this, 1, wxEmptyString, wxDefaultPosition, size, wxTE_MULTILINE | wxTE_DONTWRAP);

			m_LogTexWnd->Show();
		}					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::OnSize(wxSizeEvent& e)
		{
			wxRect ClientRect = GetClientRect();		
			ClientRect.SetWidth(ClientRect.GetWidth()-20);
			ClientRect.SetHeight(ClientRect.GetHeight()-20);
			
			wxRect Rect(ClientRect.GetX(), ClientRect.GetY(), ClientRect.GetWidth(), ClientRect.GetHeight()*0.85f);		
			m_LogTexWnd->SetSize(Rect);
			
			e.Skip();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::vOpen(const CString& name)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::vClose()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::vClear()
		{
			m_LogTexWnd->Clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::vAppend(const CString& str)
		{
			m_LogTexWnd->AppendText(str.c_str());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CLogPage::vOutput(const CString& str)
		{
		}
	}
}

#endif