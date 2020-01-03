#pragma once
#include<Windows.h>
#include<string>
#include"Define.h"
#include"CToolFrame.h"

using namespace std;

struct MapBlock {
	RECT rtBlock;
	TYLE Tyle = NONE_TYLE;
	OBJECT Object = NONE_OBJECT;
	BLOCK Block = NONE_BLOCK;
};

class CMapManager : public CToolFrame {
private:
	HDC hOptionDC;
	HDC hBackgroundDC[4];
	HDC hTyleDC[22];
	HDC hObjectDC[7];
	HDC hBlockDC[3];

	MapBlock mapBlock[143];
	RECT rtOption[33];

	int nBackground;
	TYLE Tyle;
	OBJECT Object;
	BLOCK Block;
public:
	CMapManager(HWND hWnd);
	//~CMapManager();

	//void Load();
	void Save(const char* strFileName);
	void MouseCheck();

	void Update();
	void Render();
};