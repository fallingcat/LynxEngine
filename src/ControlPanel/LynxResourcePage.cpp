#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxEngine.h>
#include <ControlPanel/LynxControlPanel.h>
#include <ControlPanel/LynxResourcePage.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{
		#define ID_LIST					1
		#define ID_REFRESH				2

		#define ID_COLUMN_NUMBER		0
		#define ID_COLUMN_NAME			ID_COLUMN_NUMBER+1
		#define ID_COLUMN_TYPE			ID_COLUMN_NUMBER+2
		#define ID_COLUMN_COUNT			ID_COLUMN_NUMBER+3

		BEGIN_EVENT_TABLE(CResourcePage, wxPanel)
			EVT_SIZE    (						CResourcePage::OnSize)		
			EVT_BUTTON  (ID_REFRESH,			CResourcePage::OnRefresh)
			EVT_LIST_COL_CLICK(ID_LIST,			CResourcePage::OnSort)			
		END_EVENT_TABLE()
		// ----------------------------------------------------------------------------
		// main frame
		// ----------------------------------------------------------------------------
		// frame constructor
		CResourcePage::CResourcePage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxPanel(lpc->GetlpBookCtrl(), pos.x, pos.y, size.GetWidth(), size.GetHeight(), wxTAB_TRAVERSAL, title), m_lpControlPanel(lpc)
		{
			m_SortType = wxLC_SORT_ASCENDING;
			m_ResourceListWnd = new wxListCtrl(this, ID_LIST, wxDefaultPosition, size, wxLC_REPORT|wxLC_ALIGN_LEFT|wxLC_SORT_ASCENDING);		
			wxListItem ItemCol;
			ItemCol.SetText(_T("#"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetImage(-1);		
			ItemCol.SetColumn(ID_COLUMN_NUMBER);
			m_ResourceListWnd->InsertColumn(ID_COLUMN_NUMBER, ItemCol);

			ItemCol.SetText(_T("Name"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);			
			ItemCol.SetColumn(ID_COLUMN_NAME);
			m_ResourceListWnd->InsertColumn(ID_COLUMN_NAME, ItemCol);

			ItemCol.SetText(_T("Type"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetColumn(ID_COLUMN_TYPE);
			m_ResourceListWnd->InsertColumn(ID_COLUMN_TYPE, ItemCol);

			ItemCol.SetText(_T("Count"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetColumn(ID_COLUMN_COUNT);
			m_ResourceListWnd->InsertColumn(ID_COLUMN_COUNT, ItemCol);

			m_ResourceListWnd->Show();
			m_Refresh = new wxButton(this, ID_REFRESH, _T("Refresh"), wxPoint(180,30), wxSize(140,30) );
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CResourcePage::RefreshResource()
		{
			m_ResourceListWnd->DeleteAllItems();

			CList<CResourcePtr>::CIterator Res;
			CList<CResourcePtr>& ResList = m_lpControlPanel->GetlpEngine()->GetResourceManager().GetObjectList();
			int Num = 0;
			for (Res = ResList.begin(); Res != ResList.end(); ++Res)
			{
				wxString buf;
				buf.Printf(_T("%04d"), Num);
				long Tmp = m_ResourceListWnd->InsertItem(Num, buf, 0);
				m_ResourceListWnd->SetItemData(Tmp, Num);

				buf.Printf(_T("%s"), (*Res)->GetName().c_str());
				//long Tmp = m_ResourceListWnd->InsertItem(Num, buf, 0);
				//m_ResourceListWnd->SetItemData(Tmp, Num);
				m_ResourceListWnd->SetItem(Tmp, 1, buf);

				buf.Printf(_T("%s"), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str());
				m_ResourceListWnd->SetItem(Tmp, 2, buf);

				buf.Printf(_T("%d"), (*Res).GetCount());
				m_ResourceListWnd->SetItem(Tmp, 3, buf);

				//lynxSprintf(Buffer, _T("%s, Type: %s, Count: %d"), (*Res)->GetName().c_str(), LynxEngine::GetEnumStr((*Res)->GetResourceType()).c_str(), (*Res).GetCount());
				//m_lpGraphicsSystem->GetlpScreenOutputDevice()->Append(10+4*5, CString(Buffer), &Color, &ShadowColor);
				Num++;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CResourcePage::OnSize(wxSizeEvent& e)
		{
			wxRect ClientRect = GetClientRect();		
			ClientRect.SetWidth(ClientRect.GetWidth()-20);
			ClientRect.SetHeight(ClientRect.GetHeight()-20);
			
			wxRect Rect(ClientRect.GetX(), ClientRect.GetY(), ClientRect.GetWidth(), ClientRect.GetHeight()*0.85f);		
			m_ResourceListWnd->SetSize(Rect);				
			m_Refresh->SetSize(ClientRect.GetRight() - 150, Rect.GetBottom() + 10, 140, 30);

			m_ResourceListWnd->SetColumnWidth(0, Rect.GetWidth()*0.1f);
			m_ResourceListWnd->SetColumnWidth(1, Rect.GetWidth()*0.5f);
			m_ResourceListWnd->SetColumnWidth(2, Rect.GetWidth()*0.3f);
			m_ResourceListWnd->SetColumnWidth(3, Rect.GetWidth()*0.1f);
			
			e.Skip();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CResourcePage::OnRefresh(wxCommandEvent& e)
		{
			switch (e.GetId())
			{
				case ID_REFRESH:
					RefreshResource();
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int wxCALLBACK wxTypeCompareFunction(long item1, long item2, long sortData)
		{
			CResourcePage* Page = (CResourcePage*)sortData;
			CList<CResourcePtr>& ResList = Page->GetlpControlPanel()->GetlpEngine()->GetResourceManager().GetObjectList();

			if (ResList[item1]->GetResourceType() > ResList[item2]->GetResourceType())
				return 1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else if (ResList[item1]->GetResourceType() < ResList[item2]->GetResourceType())
				return -1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else
				return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int wxCALLBACK wxNameCompareFunction(long item1, long item2, long sortData)
		{
			CResourcePage* Page = (CResourcePage*)sortData;
			CList<CResourcePtr>& ResList = Page->GetlpControlPanel()->GetlpEngine()->GetResourceManager().GetObjectList();

			if (ResList[item1]->GetName() > ResList[item2]->GetName())
				return 1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else if (ResList[item1]->GetName() < ResList[item2]->GetName())
				return -1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else
				return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int wxCALLBACK wxNumberCompareFunction(long item1, long item2, long sortData)
		{
			CResourcePage* Page = (CResourcePage*)sortData;

			if (item1 > item2)
				return 1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else if (item1 < item2)
				return -1 * (Page->m_SortType == wxLC_SORT_ASCENDING?1:-1);
			else
				return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CResourcePage::OnSort(wxListEvent& e)
		{	
			if (m_SortType == wxLC_SORT_ASCENDING)
			{
				m_SortType = wxLC_SORT_DESCENDING;				
			}
			else
			{
				m_SortType = wxLC_SORT_ASCENDING;				
			}
			
			switch (e.GetColumn())
			{
				case ID_COLUMN_TYPE:					
					m_ResourceListWnd->SortItems(wxTypeCompareFunction, (long)this);
					break;

				case ID_COLUMN_NAME:
					m_ResourceListWnd->SortItems(wxNameCompareFunction, (long)this);
					break;

				case ID_COLUMN_NUMBER:
					m_ResourceListWnd->SortItems(wxNumberCompareFunction, (long)this);
					break;
			}
		}
	}
}

#endif