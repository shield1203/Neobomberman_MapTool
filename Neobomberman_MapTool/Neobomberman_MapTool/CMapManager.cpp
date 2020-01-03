#include"CMapManager.h"

CMapManager::CMapManager(HWND hWnd) : CToolFrame(hWnd) {
	hOptionDC = CreateBmpDC("Map\\Option.bmp");
	hBackgroundDC[0] = CreateBmpDC("Map\\Background01.bmp");
	hBackgroundDC[1] = CreateBmpDC("Map\\Background02.bmp");
	hBackgroundDC[2] = CreateBmpDC("Map\\Background03.bmp");
	hBackgroundDC[3] = CreateBmpDC("Map\\Background04.bmp");
	
	// MapBlock RECT 값
	RECT rt = { 192, 128, 256, 192 };
	for (int i = 0; i < 143; i++) {
		mapBlock[i].rtBlock.left = rt.left;
		mapBlock[i].rtBlock.right = rt.right;
		mapBlock[i].rtBlock.top = rt.top;
		mapBlock[i].rtBlock.bottom = rt.bottom;

		if (rt.left == 960) {
			rt.left = 192;
			rt.right = 256;
			rt.top += 64;
			rt.bottom += 64;
		}
		else {
			rt.left += 64;
			rt.right += 64;
		}
	}
	
	// Tyle RECT
	rt = { 1216, 230, 1280, 294 };
	int nj = 0;
	for (int i = 0; i < 23; i++) {
		rtOption[nj].left = rt.left;
		rtOption[nj].right = rt.right;
		rtOption[nj].top = rt.top;
		rtOption[nj].bottom = rt.bottom;

		rt.left += 67;
		rt.right = rt.left + 64;

		if (rt.left == 1618) {
			rt.left = 1216;
			rt.right = rt.left + 64;
			rt.top += 67;
			rt.bottom = rt.top + 64;
		}
		nj++;
	}

	// Object RECT
	rt = { 1216, 590, 1280, 654 };
	for (int i = 0; i < 7; i++) {
		rtOption[nj].left = rt.left;
		rtOption[nj].right = rt.right;
		rtOption[nj].top = rt.top;
		rtOption[nj].bottom = rt.bottom;

		rt.left += 67;
		rt.right = rt.left + 64;

		if (rt.left == 1618) {
			rt.left = 1216;
			rt.right = rt.left + 64;
			rt.top += 91;
			rt.bottom = rt.top + 64;
		}
		nj++;
	}
	// Block RECT
	rt = { 1216, 829, 1280, 893 };
	for (int i = 0; i < 3; i++) {
		rtOption[nj].left = rt.left;
		rtOption[nj].right = rt.right;
		rtOption[nj].top = rt.top;
		rtOption[nj].bottom = rt.bottom;

		rt.left += 67;
		rt.right = rt.left + 64;
		nj++;
	}

	// DC읽어오기 =================================================================================
	for (int i = 0; i < 23; i++) {
		string filename = "Map\\Tyle.bmp";

		if (i+1 < 10) {
			string num = "0";
			num.append(to_string(i+1));
			filename.insert(8, num);
		}
		else {
			filename.insert(8, to_string(i+1));
		}
		
		hTyleDC[i] = CreateBmpDC(filename.c_str());
	}
	
	for (int i = 0; i < 8; i++) {
		string filename = "Map\\Object0.bmp";
		filename.insert(11, to_string(i+1));
		hObjectDC[i] = CreateBmpDC(filename.c_str());
	}

	for (int i = 0; i < 4; i++) {
		string filename = "Map\\Block0.bmp";
		filename.insert(10, to_string(i+1));
		hBlockDC[i] = CreateBmpDC(filename.c_str());
	}

	nBackground = 0;
	Tyle = NONE_TYLE;
	Block = NONE_BLOCK;
	Object = NONE_OBJECT;

	mapBlock[0].rtBlock.left = 192; // 왜 값이 바뀌는지 불명
}

void CMapManager::Save(const char* strFileName) {
	TiXmlDocument doc;
	TiXmlDeclaration* DB = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(DB);

	TiXmlElement* pSAVE = new TiXmlElement("Map");
	doc.LinkEndChild(pSAVE);

	TiXmlElement* pBack = new TiXmlElement("Background");
	pSAVE->LinkEndChild(pBack);
	if (nBackground == 0) {
		pBack->LinkEndChild(new TiXmlText("0"));
	}
	else if(nBackground == 1){
		pBack->LinkEndChild(new TiXmlText("1"));
	}
	else if (nBackground == 2) {
		pBack->LinkEndChild(new TiXmlText("2"));
	}
	else if (nBackground == 3) {
		pBack->LinkEndChild(new TiXmlText("3"));
	}

	for (int i = 0; i < 143; i++) {
		TiXmlElement* pMapBlock = new TiXmlElement("MapBlock");
		pSAVE->LinkEndChild(pMapBlock);

		pMapBlock->SetAttribute("Tyle", mapBlock[i].Tyle);
		pMapBlock->SetAttribute("Object", mapBlock[i].Object);
		pMapBlock->SetAttribute("Block", mapBlock[i].Block);
	}
	

	doc.SaveFile(strFileName);
}

void CMapManager::MouseCheck() {
	switch (MouseState)
	{
	case MOUSE_UP:
		break;
	case MOUSE_DOWN: {
		if (nMouseClickXPos > 1216) { // 맵 옵션 선택
			if (nMouseClickYPos >= 65 && nMouseClickYPos <= 155) { // 배경 설정
				if (nMouseClickXPos <= 1316) {
					nBackground = 0;
				}
				else if (nMouseClickXPos <= 1416) {
					nBackground = 1;
				}
				else if (nMouseClickXPos <= 1516) {
					nBackground = 2;
				}
				else {
					nBackground = 3;
				}
			}
			else {
				for (int i = 0; i < 33; i++) {
					if (rtOption[i].left <= nMouseClickXPos && rtOption[i].right >= nMouseClickXPos &&
						rtOption[i].top <= nMouseClickYPos && rtOption[i].bottom >= nMouseClickYPos) {

						if (i < 23) { // Tyle
							Tyle = (TYLE)(i+1);
							Object = NONE_OBJECT;
							Block = NONE_BLOCK;
						}
						else if (i < 30) { // Object
							Tyle = NONE_TYLE;
							Object = (OBJECT)(i-22);
							Block = NONE_BLOCK;
						}
						else { // Block
							Tyle = NONE_TYLE;
							Object = NONE_OBJECT;
							Block = (BLOCK)(i-29);
						}
					}
				}
			}
		}
		else { // 맵 옵션 적용
			for (int i = 0; i < 143; i++) {
				if (mapBlock[i].rtBlock.left <= nMouseClickXPos && mapBlock[i].rtBlock.right >= nMouseClickXPos &&
					mapBlock[i].rtBlock.top <= nMouseClickYPos && mapBlock[i].rtBlock.bottom >= nMouseClickYPos) {
					
					if (Tyle != NONE_TYLE) {
						mapBlock[i].Tyle = Tyle;
					}
					else if (Object != NONE_OBJECT) {
						mapBlock[i].Object = Object;
						mapBlock[i].Block = NONE_BLOCK;
					}
					else if (Block != NONE_BLOCK) {
						mapBlock[i].Block = Block;
						mapBlock[i].Object = NONE_OBJECT;
					}
				}
			}
		}
		break;
	}
	case MOUSE_DEL: {
		for (int i = 0; i < 143; i++) {
			if (mapBlock[i].rtBlock.left <= nMouseClickXPos && mapBlock[i].rtBlock.right >= nMouseClickXPos &&
				mapBlock[i].rtBlock.top <= nMouseClickYPos && mapBlock[i].rtBlock.bottom >= nMouseClickYPos) {
				if (mapBlock[i].Object != NONE_OBJECT) {
					mapBlock[i].Object = NONE_OBJECT;
				}
				else if (mapBlock[i].Block != NONE_BLOCK) {
					mapBlock[i].Block = NONE_BLOCK;
				}
				else if (mapBlock[i].Tyle != NONE_TYLE) {
					mapBlock[i].Tyle = NONE_TYLE;
				}
				break;
			}
		}
		MouseState = MOUSE_UP;
		break;
	}
	}
}

void CMapManager::Update() {
	MouseCheck();
}

void CMapManager::Render() {
	HDC hdc = GetDC(hWnd); 
	// ===========================================================================================================================
	// Map====================================================================================================
	//PatBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, BLACKNESS); // 검은 배경
	BitBlt(hBackbuffer, 1216, 0, 400, rtWinRect.bottom - rtWinRect.top, hOptionDC, 0, 0, SRCCOPY); // 옵션창(택스트)

	StretchBlt(hBackbuffer, 0, 0, 1216, 896, hBackgroundDC[nBackground], 0, 0, 304, 224, SRCCOPY);
	
	for (int i = 0; i < 143; i++) {
		FrameRect(hBackbuffer, &mapBlock[i].rtBlock, 0);
		if (mapBlock[i].Tyle != NONE_TYLE) {
			StretchBlt(hBackbuffer, mapBlock[i].rtBlock.left, mapBlock[i].rtBlock.top, 64, 64, hTyleDC[mapBlock[i].Tyle - 1], 0, 0, 16, 16, SRCCOPY);
		}

		if (mapBlock[i].Object != NONE_OBJECT) {
			TransparentBlt(hBackbuffer, mapBlock[i].rtBlock.left, mapBlock[i].rtBlock.top - 24, 64, 88, hObjectDC[mapBlock[i].Object - 1], 0, 0, 16, 22, RGB(0, 248, 0));
		}
		else if (mapBlock[i].Block != NONE_BLOCK) {
			TransparentBlt(hBackbuffer, mapBlock[i].rtBlock.left, mapBlock[i].rtBlock.top, 64, 64, hBlockDC[mapBlock[i].Block - 1], 0, 0, 16, 16, RGB(0, 248, 0));
		}
	}
	
	// Option =================================================================================================
	// Background 
	StretchBlt(hBackbuffer, 1216, 65, 100, 90, hBackgroundDC[0], 0, 20, 304, 224, SRCCOPY);
	StretchBlt(hBackbuffer, 1316, 65, 100, 90, hBackgroundDC[1], 0, 20, 304, 224, SRCCOPY);
	StretchBlt(hBackbuffer, 1416, 65, 100, 90, hBackgroundDC[2], 0, 20, 304, 224, SRCCOPY);
	StretchBlt(hBackbuffer, 1516, 65, 100, 90, hBackgroundDC[3], 0, 20, 304, 224, SRCCOPY);

	// Tyle
	int nXPos = 1216;
	int nYPos = 230;
	for (int i = 0; i < 22; i++) {
		StretchBlt(hBackbuffer, nXPos, nYPos, 64, 64, hTyleDC[i], 0, 0, 16, 16, SRCCOPY);
		nXPos += 67;

		if (nXPos == 1618) {
			nXPos = 1216;
			nYPos += 67;
		}
	}

	// Object
	nXPos = 1216;
	nYPos = 590;
	for (int i = 0; i < 7; i++) {
		StretchBlt(hBackbuffer, nXPos, nYPos, 64, 88, hObjectDC[i], 0, 0, 16, 22, SRCCOPY);
		nXPos += 67;

		if (nXPos == 1618) {
			nXPos = 1216;
			nYPos += 91;
		}
	}
	// Block
	nXPos = 1216;
	nYPos = 829;
	for (int i = 0; i < 3; i++) {
		StretchBlt(hBackbuffer, nXPos, nYPos, 64, 64, hBlockDC[i], 0, 0, 16, 16, SRCCOPY);
		nXPos += 67;
	}

	if (Tyle != NONE_TYLE || Object != NONE_OBJECT || Block != NONE_BLOCK) {
		if (Tyle != NONE_TYLE) { // TYLE
			FrameRect(hBackbuffer, &rtOption[Tyle - 1], 0);
		}
		else if (Object != NONE_OBJECT) { // OBJECT
			RECT rt = { rtOption[22 + Object].left, rtOption[22 + Object].top, rtOption[22 + Object].right, rtOption[22 + Object].bottom + 24 };
			FrameRect(hBackbuffer, &rt, 0);
		}
		else { // BLOCK
			FrameRect(hBackbuffer, &rtOption[29 + Block], 0);
		}
		
	}

	// ===========================================================================================================================
	BitBlt(hdc, 0, 0, rtWinRect.right - rtWinRect.left, rtWinRect.bottom - rtWinRect.top, hBackbuffer, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}