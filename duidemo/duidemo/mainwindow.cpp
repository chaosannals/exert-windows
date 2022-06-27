#include "mainwindow.h"

void CMainWindowWnd::Notify(DuiLib::TNotifyUI& msg) {
	if (msg.sType == TEXT("click")) {
		if (msg.pSender->GetName() == TEXT("closebtn")) {
			Close();
		}
	}
}

LRESULT CMainWindowWnd::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	using namespace DuiLib;
	if (msg == WM_CREATE) {
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(TEXT("mainwindow.xml", 0, NULL, &m_pm));
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		//CControlUI* pButton = new CButtonUI;
		//pButton->SetName(TEXT("closebtn"));
		//pButton->SetBkColor(0xFFFF0000);
		//m_pm.AttachDialog(pButton);
		//m_pm.AddNotifier(this);
		return 0;
	}
	else if (msg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (msg == WM_NCACTIVATE) {
		if (!IsIconic(m_hWnd)) {
			return wParam == 0 ? TRUE : FALSE;
		}
	}
	else if (msg == WM_NCCALCSIZE) {
		return 0;
	}
	else if (msg == WM_NCPAINT) {
		return 0;
	}

	LRESULT lRes = 0;
	if (m_pm.MessageHandler(msg, wParam, lParam, lRes)) {
		return lRes;
	}
	return CWindowWnd::HandleMessage(msg, wParam, lParam);
}