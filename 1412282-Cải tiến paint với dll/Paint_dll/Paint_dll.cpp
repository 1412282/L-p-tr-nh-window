// Paint_dll.cpp : Defines the entry point for the application.
//
#pragma once
#include "stdafx.h"
#include "Paint_dll.h"
#include <vector>
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"
#include "windowsx.h"	
#include "Windows.h"
using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.
	CoInitialize(NULL);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINT_DLL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT_DLL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	CoUninitialize();
    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT_DLL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT_DLL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
vector<CSHAPE*> shapes;
int isDrawing = 0;
int a, b, c, d;

HDC memDC, hdc;
PAINTSTRUCT ps;
HBITMAP hdcBM, memBM;
int width, height;
RECT client;
HPEN hPen1 = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
CSHAPE *tempShape;
int type = 1;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_LBUTTONDOWN:
		SetShape();
		isDrawing = 1;
		a = GET_X_LPARAM(lParam);
		b = GET_Y_LPARAM(lParam);
		tempShape->SetData(a, b, 0, 0);

		GetClientRect(hWnd, &client);
		width = client.right - client.left;
		height = client.bottom - client.top;

		hdc = GetDC(hWnd);
		memDC = CreateCompatibleDC(hdc);
		memBM = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(memDC, memBM);

		hdc = BeginPaint(hWnd, &ps);

		
		break;
	case WM_MOUSEMOVE:
		if (isDrawing)
		{ 
			
			c = GET_X_LPARAM(lParam);
			d = GET_Y_LPARAM(lParam);
			tempShape->SetData(a, b,c,d);
			//Draw to memDC

			FillRect(memDC, &client, (HBRUSH)::GetStockObject(WHITE_BRUSH));
			SelectObject(memDC, hPen1);
			for (int i = 0; i < shapes.size(); i++)
				shapes[i]->Draw(memDC);
			
			tempShape->Draw(memDC);
			BitBlt(hdc, client.left, client.top, (int)width, (int)height, memDC, 0, 0, SRCCOPY);
		}
		break;
	case WM_LBUTTONUP:
		if (isDrawing)
		{
			EndPaint(hWnd, &ps);
			isDrawing = 0;
			shapes.push_back(tempShape);
			InvalidateRect(hWnd, NULL, NULL);
		}
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void SetShape()
{
	switch (type) {
	case 1:
		tempShape = new CLINE;
		break;
	case 2:
		tempShape = new CRECTANGLE;
		break;
	case 3:
		tempShape = new CELLIPSE;
		break;
	}
}
// Message handler for about box.
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


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)

{
	return 1;
}

void OnCommand(HWND hWnd, int wmId, HWND hwndCtl, UINT codeNotify)
{
	switch (wmId)
	{
	case ID_SHAPE_LINE:
		type = 1;
		break;
	case ID_SHAPE_RECTANGLE:
		type = 2;
		break;
	case ID_SHAPE_ELLIPSE:
		type = 3;
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnPaint(HWND hWnd)
{


}

void OnDestroy(HWND hWnd)
{
	DestroyFramework();
	PostQuitMessage(0);
}
