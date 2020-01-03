﻿// Neobomberman_MapTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "Neobomberman_MapTool.h"
#include <CommDlg.h>
#include"Define.h"
#include"CMapManager.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
static HWND g_hWnd = NULL;
RECT rtWindow = { WIN_LEFT, WIN_TOP, WIN_RIGHT, WIN_BOTTOM };
SIZE szWindow;
CMapManager *MapManager;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_NEOBOMBERMANMAPTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NEOBOMBERMANMAPTOOL));

    MSG msg;

	MapManager = new CMapManager(g_hWnd);

    // 기본 메시지 루프입니다:
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			MapManager->Update();
			MapManager->Render();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_NEOBOMBERMANMAPTOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd;
	   
   g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

int FileSaveDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "저장할 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = "xml";

	if (GetSaveFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;
	static char strFilePath[MAX_PATH], strFileName[100];
	static char strFilter[] = "(*.xml)\0*.xml\0";
	char buffer[100];
	
    switch (message)
    {
		case WM_CREATE:
			AdjustWindowRect(&rtWindow, WS_OVERLAPPEDWINDOW, TRUE);
			szWindow.cx = rtWindow.right - rtWindow.left;
			szWindow.cy = rtWindow.bottom - rtWindow.top;
			break;
		case WM_SIZE:
			GetWindowRect(hWnd, &rtWindow);
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, szWindow.cx, szWindow.cy, TRUE);
			break;
		case WM_MOVE:
			GetWindowRect(hWnd, &rtWindow);
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, szWindow.cx, szWindow.cy, TRUE);
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			MapManager->MouseProcess(message, lParam);
			break;
		}
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// 메뉴 선택을 구문 분석합니다:
				switch (wmId)
				{
				case ID_32771: { // Save
					if (FileSaveDilaog(hWnd, strFilePath, strFileName, strFilter) != -1)
					{	// 저장파일 형식이 한 가지이므로 파일 선택 인덱스를 조사할 필요가 없다.				
						MapManager->Save(strFilePath);
						MessageBox(hWnd, "저장이 완료되었습니다.", "MapTool~", 0);
					}
					break;
				}
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
