#if defined(__WXWIDGET__)

#ifndef __LYNXCONTROLPANEL_H__
#define __LYNXCONTROLPANEL_H__

#include <LynxOutputDevice.h>

namespace LynxEngine
{
	namespace ControlPanel
	{
		// Define a new frame type: this is going to be our main frame
		class LYNXENGCLASS CControlPanel : public wxFrame, public COutputDevice
		{
			LYNX_DECLARE_CLASS(CControlPanel);
		protected:
			CEngine*					m_lpEngine;
			wxBookCtrl*					m_Book;
			wxTextCtrl*					m_LogTexWnd;
			COutputDevice*				m_NetworkLogOutputDevice;
			COutputDevice*				m_NetClientLogOutputDevice;
		public:
			// ctor(s)
			CControlPanel(CEngine* lpe, const wxString& title, const wxPoint& pos, const wxSize& size);

			LYNXINLINE CEngine* GetlpEngine() { return m_lpEngine; };
			LYNXINLINE void SetlpEngine(CEngine* lpe) { m_lpEngine = lpe; };
			LYNXINLINE wxBookCtrl* GetlpBookCtrl() { return m_Book; };
			LYNXINLINE COutputDevice* GetNetworkLogOutputDevice() { return m_NetworkLogOutputDevice; };
			LYNXINLINE COutputDevice* GetNetClientLogOutputDevice() { return m_NetClientLogOutputDevice; };

			void AddPage(wxWindow *page, const wxString& text, bool bSelect = false, int imageId = -1) { m_Book->AddPage(page, text, bSelect, imageId); };
			void SelectPage(const wxString& name);

			void vOpen(const CString& name);
			void vClose();
			void vClear();
			void vAppend(const CString& str);
			void vOutput(const CString& str);

			void OnSize(wxSizeEvent& event);
		private:
			// any class wishing to process wxWidgets events must use this macro
			DECLARE_EVENT_TABLE()
		};
	}
}
#endif

#endif