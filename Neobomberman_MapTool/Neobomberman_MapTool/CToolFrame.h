#pragma once
#include<Windows.h>
#include"Define.h"
#include"tinyxml.h"

class CToolFrame {
public:
	HWND hWnd;
	RECT rtWinRect;
	HDC hBackbuffer;

	MOUSE_STATE MouseState;

	int nMouseClickXPos;
	int nMouseClickYPos;

	HDC CreateBmpDC(const char* filename);
	void MouseProcess(UINT message, LPARAM lParam);
	/*void PaintRECT(HDC hdc);*/

	CToolFrame(HWND hWnd);
	~CToolFrame();
};