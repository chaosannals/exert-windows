#pragma once

#include <Windows.h>
#include <uicore.h>

typedef struct tagMYBITMAPINFO {
	BITMAPINFOHEADER    bmiHeader;
	DWORD				biRedMask;
	DWORD				biGreenMask;
	DWORD				biBlueMask;
} MYBITMAPINFO;


class CUiBlock
{
public:
	CUiBlock(int index, int color_bytes);
	void renderUI(RECT& rect, HDC hDC);

	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnKeyUp(unsigned int key);
private:
	void pointMFC2GuiLite(int& x, int& y);

	int m_color_bytes;
	RECT m_block_rect;
	int m_ui_width;
	int m_ui_height;

	MYBITMAPINFO m_ui_bm_info;
	HDC			m_memDC;
	HBITMAP		m_blockBmp;

	bool m_is_mouse_down;
};
