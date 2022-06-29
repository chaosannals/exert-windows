#pragma once

#include <DuiLib/UIlib.h>

class CMainWindowWnd : public DuiLib::CWindowWnd, public DuiLib::INotifyUI
{
public:
	DuiLib::CPaintManagerUI m_pm;
	CMainWindowWnd() {}
	LPCTSTR GetWindowClassName()const { return TEXT("UIMainFrame"); }
	UINT GetClassStyle()const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; }
	void OnFinalMessage(HWND) { delete this; }

	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};