#include"CToolFrame.h"

CToolFrame::CToolFrame(HWND hWnd) {
	GetWindowRect(hWnd, &rtWinRect);
	this->hWnd = hWnd;

	MouseState = MOUSE_NONE;

	nMouseClickXPos = 0;
	nMouseClickYPos = 0;

	HBITMAP hBitmap;
	HDC hdc;

	hdc = GetDC(hWnd);
	hBackbuffer = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rtWinRect.right - rtWinRect.left, rtWinRect.bottom - rtWinRect.top);
	SelectObject(hBackbuffer, (HBITMAP)hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);
}

CToolFrame::~CToolFrame() {
	DeleteDC(hBackbuffer);
}

HDC CToolFrame::CreateBmpDC(const char* filename) {
	HBITMAP hBitmap;
	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT(filename), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	SelectObject(hMemDC, hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);
	return hMemDC;
}

//void CToolFrame::PaintRECT(HDC hdc) {
//
//}

void CToolFrame::MouseProcess(UINT message, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
	{
		MouseState = MOUSE_DOWN;  // 다운과 업으로 바꿀 필요가 있다.
		nMouseClickXPos = LOWORD(lParam);
		nMouseClickYPos = HIWORD(lParam);
	}
	else if (message == WM_LBUTTONUP) {
		MouseState = MOUSE_UP;
		nMouseClickXPos = LOWORD(lParam);
		nMouseClickYPos = HIWORD(lParam);
	}
	else if (message == WM_RBUTTONUP) {
		MouseState = MOUSE_DEL;
		nMouseClickXPos = LOWORD(lParam);
		nMouseClickYPos = HIWORD(lParam);
	}
	else {
		MouseState = MOUSE_NONE;
	}
}