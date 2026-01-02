#include <LynxEngine_PCH.h>

#if defined(__WXWIDGET__)

#include <LynxEngine.h>
#include <ControlPanel/LynxControlPanel.h>
#include <ControlPanel/LynxMemoryPage.h>

namespace LynxEngine 
{
	namespace ControlPanel
	{
		#define ID_LIST					1
		#define ID_REFRESH				2

		BEGIN_EVENT_TABLE(CMemoryPage, wxPanel)
			EVT_SIZE    (						CMemoryPage::OnSize)		
			EVT_BUTTON  (ID_REFRESH,			CMemoryPage::OnRefresh)
		END_EVENT_TABLE()
		// ----------------------------------------------------------------------------
		// main frame
		// ----------------------------------------------------------------------------
		// frame constructor
		CMemoryPage::CMemoryPage(CControlPanel* lpc, const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxPanel(lpc->GetlpBookCtrl(), pos.x, pos.y, size.GetWidth(), size.GetHeight(), wxTAB_TRAVERSAL, title), m_lpControlPanel(lpc)
		{
			m_MemoryBlockListWnd = new wxListCtrl(this, ID_LIST, wxDefaultPosition, size, wxLC_REPORT|wxLC_ALIGN_LEFT|wxLC_SORT_ASCENDING);					
			wxListItem ItemCol;
			ItemCol.SetText(_T("Index"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetImage(-1);		
			m_MemoryBlockListWnd->InsertColumn(0, ItemCol);
			ItemCol.SetText(_T("Address"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			ItemCol.SetImage(-1);		
			m_MemoryBlockListWnd->InsertColumn(1, ItemCol);
			ItemCol.SetText(_T("Size"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);			
			m_MemoryBlockListWnd->InsertColumn(2, ItemCol);
			ItemCol.SetText(_T("File"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			m_MemoryBlockListWnd->InsertColumn(3, ItemCol);
			ItemCol.SetText(_T("Function"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			m_MemoryBlockListWnd->InsertColumn(4, ItemCol);
			ItemCol.SetText(_T("Line"));
			ItemCol.SetAlign(wxLIST_FORMAT_LEFT);
			m_MemoryBlockListWnd->InsertColumn(5, ItemCol);
			m_MemoryBlockListWnd->Show();
			m_Refresh = new wxButton(this, ID_REFRESH, _T("Refresh"), wxPoint(180,30), wxSize(140,30) );
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMemoryPage::Refresh()
		{
			m_MemoryBlockListWnd->DeleteAllItems();

		#ifdef LYNXMEMALLOCATOR
			CList<CMemAllocator::MemoryChunk>* lpList = LYNXMEMALLOCATOR::vGetMemoryChunkList();
			CList<CMemAllocator::MemoryChunk>::CIterator Mem;

			if (lpList)
			{
				int Index = 0;
				for (Mem = lpList->begin(); Mem != lpList->end(); ++Mem)
				{
					wxString buf;
					buf.Printf(_T("%04d"), Index);
					long Tmp = m_MemoryBlockListWnd->InsertItem(Index, buf, 0);
					m_MemoryBlockListWnd->SetItemData(Tmp, Index);

					buf.Printf(_T("%0xd"), (*Mem).Address);
					m_MemoryBlockListWnd->SetItem(Tmp, 1, buf);

					buf.Printf(_T("%d"), (*Mem).Size);				
					m_MemoryBlockListWnd->SetItem(Tmp, 2, buf);

					LYNXCHAR File[256], Name[128];				
					lynxFromAnsiStr(File, (*Mem).File);
					CEngine::GetFileNameFromPath(Name, File);
					buf.Printf(_T("%s"), Name);
					m_MemoryBlockListWnd->SetItem(Tmp, 3, buf);

					lynxFromAnsiStr(File, (*Mem).Function);
					buf.Printf(_T("%s"), File);
					m_MemoryBlockListWnd->SetItem(Tmp, 4, buf);

					buf.Printf(_T("%d"), (*Mem).Line);
					m_MemoryBlockListWnd->SetItem(Tmp, 5, buf);		

					Index++;
				}
			}
		#endif

			/*LPLYNXMEMBLOCK lpMem = lynxGetFirstMemBlock();

			int Index = 0;
			while (lpMem)
			{
				wxString buf;
				buf.Printf(_T("%0xd"), lpMem->lpBase);
				long Tmp = m_MemoryBlockListWnd->InsertItem(Index, buf, 0);
				m_MemoryBlockListWnd->SetItemData(Tmp, Index);

				buf.Printf(_T("%d"), lpMem->Size);				
				m_MemoryBlockListWnd->SetItem(Tmp, 1, buf);

				LYNXCHAR File[64];
				lynxFromAnsiStr(File, lpMem->File);
				buf.Printf(_T("%s"), File);
				m_MemoryBlockListWnd->SetItem(Tmp, 2, buf);

				buf.Printf(_T("%d"), lpMem->Line);
				m_MemoryBlockListWnd->SetItem(Tmp, 3, buf);		

				lpMem = lynxGetNextMemBlock();
				Index++;
			}*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMemoryPage::OnSize(wxSizeEvent& e)
		{
			wxRect ClientRect = GetClientRect();		
			ClientRect.SetWidth(ClientRect.GetWidth()-20);
			ClientRect.SetHeight(ClientRect.GetHeight()-20);
			
			wxRect Rect(ClientRect.GetX(), ClientRect.GetY(), ClientRect.GetWidth(), ClientRect.GetHeight()*0.85f);		
			m_MemoryBlockListWnd->SetSize(Rect);				
			m_Refresh->SetSize(ClientRect.GetRight() - 150, Rect.GetBottom() + 10, 140, 30);

			m_MemoryBlockListWnd->SetColumnWidth(0, Rect.GetWidth()*0.1f);
			m_MemoryBlockListWnd->SetColumnWidth(1, Rect.GetWidth()*0.5f);
			m_MemoryBlockListWnd->SetColumnWidth(2, Rect.GetWidth()*0.3f);
			m_MemoryBlockListWnd->SetColumnWidth(3, Rect.GetWidth()*0.1f);
			
			e.Skip();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMemoryPage::OnRefresh(wxCommandEvent& e)
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