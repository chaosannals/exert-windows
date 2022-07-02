#include <DuiLib/UIlib.h>
#include "mainwindow.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	using namespace DuiLib;
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CMainWindowWnd* window = new CMainWindowWnd();
	if (window == NULL) return -1;
	window->Create(NULL, TEXT("²âÊÔ"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	window->ShowWindow(true);


	CPaintManagerUI::MessageLoop();
	return 0;
}