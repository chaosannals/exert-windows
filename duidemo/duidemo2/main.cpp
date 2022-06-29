#include <DuiLib/UIlib.h>
#include "resource.h"
#include "mainwindow.h"

LPBYTE g_lpResourceZIPBuffer = NULL;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	using namespace DuiLib;
	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	
	HRSRC hResource = FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(IDR_ZIPRES1), TEXT("ZIPRES"));
	ASSERT(hResource && "Failed to find res.zip");
	HGLOBAL hGlobal = LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
	if (hGlobal == NULL) {
		FreeResource(hResource);
		return -1;
	}
	ASSERT(hGlobal && "Failed to load res.zip");
	DWORD dwSize = SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
	
	if (dwSize == 0) {
		return -2;
	}

	g_lpResourceZIPBuffer = new BYTE[dwSize];
	if (g_lpResourceZIPBuffer != NULL) {
		CopyMemory(g_lpResourceZIPBuffer, LockResource(hGlobal), dwSize);
	}
	FreeResource(hResource);
	CPaintManagerUI::SetResourceZip(g_lpResourceZIPBuffer, dwSize);

	CMainWindowWnd* window = new CMainWindowWnd();
	if (window == NULL) return -1;
	window->Create(NULL, TEXT("²âÊÔ"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	window->ShowWindow(true);

	
	CPaintManagerUI::MessageLoop();
	return 0;
}