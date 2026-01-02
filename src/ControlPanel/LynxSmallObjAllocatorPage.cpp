#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxEngine.h>
#include <ControlPanel/LynxControlPanel.h>
#include <LynxSmallObjAllocator.h>
#include <ControlPanel/LynxSmallObjAllocatorPage.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{
		#define ID_LIST					1
		#define ID_REFRESH				2

		BEGIN_EVENT_TABLE(CSmallObjAllocatorPage, wxPanel)
			EVT_SIZE    (						CSmallObjAllocatorPage::OnSize)		
			EVT_BUTTON  (ID_REFRESH,			CSmallObjAllocatorPage::OnRefresh)
		END_EVENT_TABLE()
		// ----------------------------------------------------------------------------
		// main frame
		// ----------------------------------------------------------------------------
		// frame constructor
		CSmallObjAllocatorPage::CSmallObjAllocatorPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxPanel(lpc->GetlpBookCtrl(), pos.x, pos.y, size.GetWidth(), size.GetHeight(), wxTAB_TRAVERSAL, title), m_lpControlPanel(lpc)
		{
			m_FixedAllocatorListWnd = new wxListCtrl(this, ID_LIST, wxDefaultPosition, size, wxLC_REPORT|wxLC_ALIGN_LEFT|wxLC_SORT_ASCENDING);		
			wxListItem ItemCol;
			ItemCol.SetText(_T("Index"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetImage(-1);		
			m_FixedAllocatorListWnd->InsertColumn(0, ItemCol);

			ItemCol.SetText(_T("Object Size (Bytes)"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetImage(-1);		
			m_FixedAllocatorListWnd->InsertColumn(1, ItemCol);

			ItemCol.SetText(_T("Amount"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);			
			m_FixedAllocatorListWnd->InsertColumn(2, ItemCol);

			ItemCol.SetText(_T("Chunks"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);			
			m_FixedAllocatorListWnd->InsertColumn(3, ItemCol);

			m_FixedAllocatorListWnd->Show();
			m_Refresh = new wxButton(this, ID_REFRESH, _T("Refresh"), wxPoint(180,30), wxSize(140,30) );
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSmallObjAllocatorPage::Refresh()
		{
			m_FixedAllocatorListWnd->DeleteAllItems();

		#ifdef __LYNX_SMALLOBJ_MEM_ALLOCATOR__
			CArray<CFixedAllocator>& Pool = LYNXMEMALLOCATOR::GetPool();
			CArray<CFixedAllocator>::CIterator Fixed;

			int Index = 0;
			for (Fixed = Pool.begin(); Fixed != Pool.end(); ++Fixed)
			{
				wxString buf;
				buf.Printf(_T("%04d"), Index);
				long Tmp = m_FixedAllocatorListWnd->InsertItem(Index, buf, 0);
				m_FixedAllocatorListWnd->SetItemData(Tmp, Index);

				buf.Printf(_T("%03d Bytes"), (*Fixed).GetBlockSize());;
				m_FixedAllocatorListWnd->SetItem(Tmp, 1, buf);

				buf.Printf(_T("%d"), (*Fixed).GetNumAllocatedBlocks());
				m_FixedAllocatorListWnd->SetItem(Tmp, 2, buf);

				buf.Printf(_T("%d"), (*Fixed).GetNumChunks());
				m_FixedAllocatorListWnd->SetItem(Tmp, 3, buf);

				Index++;
			}			
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSmallObjAllocatorPage::OnSize(wxSizeEvent& e)
		{
			wxRect ClientRect = GetClientRect();		
			ClientRect.SetWidth(ClientRect.GetWidth()-20);
			ClientRect.SetHeight(ClientRect.GetHeight()-20);
			
			wxRect Rect(ClientRect.GetX(), ClientRect.GetY(), ClientRect.GetWidth(), ClientRect.GetHeight()*0.85f);		
			m_FixedAllocatorListWnd->SetSize(Rect);				
			m_Refresh->SetSize(ClientRect.GetRight() - 150, Rect.GetBottom() + 10, 140, 30);

			m_FixedAllocatorListWnd->SetColumnWidth(0, Rect.GetWidth()*0.1f);
			m_FixedAllocatorListWnd->SetColumnWidth(1, Rect.GetWidth()*0.5f);
			
			e.Skip();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSmallObjAllocatorPage::OnRefresh(wxCommandEvent& e)
		{
			switch (e.GetId())
			{
				case ID_REFRESH:
					Refresh();
					break;
			}
		}
	}
}

#endif