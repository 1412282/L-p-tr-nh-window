// Quan_ly_chi_tieu_Midterm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Quan_ly_chi_tieu_Midterm.h"
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#define MAX_LOADSTRING 100
#define _CRT_SECURE_NO_WARNINGS
// Global Variables:
HWND hWnd;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
vector<DANHSACH> danhsach;
vector<CRectangle> rectangles;
vector<CRectangle> rectchart;
vector<HBRUSH> hbrushs;
UINT selected;
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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUAN_LY_CHI_TIEU_MIDTERM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUAN_LY_CHI_TIEU_MIDTERM));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		ds_selected = CheckSelectedDS();
		if (ds_selected != ds_selected_old)
		{
			ds_selected_old = ds_selected;
			LoadList(); 
		}
		CheckSelectedItem();
        if (!IsDialogMessage(hWnd, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUAN_LY_CHI_TIEU_MIDTERM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   300, 10, 600, 700, nullptr, nullptr, hInstance, nullptr);

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
HWND temphwnd;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	temphwnd = hWnd;
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		
    default:

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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

void OnDestroy(HWND hWnd)
{
	UpdateDataToFile();
	PostQuitMessage(0);
}
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	for (int i = 0; i < rectangles.size(); i++)
	{
		rectangles[i].Draw(hdc, hbrushs[i]);
		rectchart[i].Draw(hdc, hbrushs[i]);
	}


	EndPaint(hWnd, &ps);

}


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	HWND hwnd;
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);


	
	cmb_danhsach = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS | WS_OVERLAPPED | WS_TABSTOP, 30, 45, 130, 200, hWnd, NULL, hInst, NULL);
	SendMessage(cmb_danhsach, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"BUTTON", L"Thêm danh sách", WS_CHILD | WS_VISIBLE | BS_CENTER | BS_CENTER, 45, 75, 100, 30, hWnd, HMENU(IDC_BUTTONADDDS), hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"BUTTON", L"Chi tiết", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 25, 430, 460, 170, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"BUTTON", L"Danh sách", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 10, 150, 100, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"BUTTON", L"Thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 330, 540, 280, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"BUTTON", L"Toàn bộ danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 125, 540, 200, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	CreateBrush(temphwnd);
	
	hwnd = CreateWindowEx(0, L"BUTTON", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 180, 10, 380, 100, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"STATIC", L"Loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_CENTER, 190, 25, 70, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	loaiCT = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS | WS_OVERLAPPED | WS_TABSTOP, 190, 45, 100, 200, hWnd, NULL, hInst, NULL);
	SendMessage(loaiCT, WM_SETFONT, WPARAM(hFont), TRUE);
	WCHAR *list_loai[6] = { L"1. Ăn uống", L"2. Di chuyển", L"3. Nhà cửa", L"4. Xe cộ", L"5. Nhu yếu phẩm", L"6. Dịch vụ" };
	for (int i = 0; i < 6; i++) {
		SendMessage(loaiCT, CB_ADDSTRING, 0, (LPARAM)(list_loai[i]));
		hwnd = CreateWindowEx(0, L"STATIC", list_loai[i], WS_CHILD | WS_VISIBLE | BS_CENTER, 30, 450 + i* 25, 90, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	}
	hwnd = CreateWindowEx(0, L"STATIC", L"Nội dung", WS_CHILD | WS_VISIBLE | BS_CENTER, 310, 25, 70, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	noidungCT = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER | WS_TABSTOP, 310, 45, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(noidungCT, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"STATIC", L"Số tiền", WS_CHILD | WS_VISIBLE | BS_CENTER, 430, 25, 70, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	sotienCT = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER | WS_TABSTOP, 430, 45, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(sotienCT, WM_SETFONT, WPARAM(hFont), TRUE);
	button_add = CreateWindowEx(0, L"BUTTON", L"Thêm chi tiêu", WS_CHILD | WS_VISIBLE | BS_CENTER | BS_CENTER | WS_TABSTOP | BS_DEFPUSHBUTTON, 300, 75, 130, 30, hWnd, HMENU(IDC_BUTTONADD), hInst, NULL);
	SendMessage(button_add, WM_SETFONT, WPARAM(hFont), TRUE);
	
	ds_listview = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT  , 25, 150, 530, 160, hWnd, NULL, hInst, NULL);
	SendMessage(ds_listview, WM_SETFONT, WPARAM(hFont), TRUE);
	
	ShowWindow(button_save, SW_HIDE);
	
	ShowWindow(button_cancel, SW_HIDE);
	hwnd = CreateWindowEx(0, L"STATIC", L"Tổng cộng", WS_CHILD | WS_VISIBLE | BS_CENTER, 150, 350, 70, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Đơn vị: Đồng", WS_CHILD | WS_VISIBLE | BS_CENTER, 350, 350, 70, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	tongcong = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER, 210, 350, 130, 20, hWnd, NULL, hInst, NULL);
	hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, FW_BOLD,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	SendMessage(tongcong, WM_SETFONT, WPARAM(hFont), TRUE);
	
	//tao columns
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	//loai
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 150;
	lvCol.pszText = _T("Loại chi tiêu");
	ListView_InsertColumn(ds_listview, 0, &lvCol);
	//noidung
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 250;
	lvCol.pszText = _T("Nội dung");
	ListView_InsertColumn(ds_listview, 1, &lvCol);
	//sotien
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 100;
	lvCol.pszText = _T("Số tiền");
	ListView_InsertColumn(ds_listview, 2, &lvCol);
	SendMessage(ds_listview, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	LoadDataFromFile(hWnd);
	LoadDSList();
	return 1;
}
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{

	switch (id)
	{

	
	case IDM_EXIT:
		UpdateDataToFile();
		DestroyWindow(hWnd);
		break;
	case IDC_BUTTONADD:
	{
		WCHAR *buffer = new WCHAR[255];
		GetWindowText(cmb_danhsach, buffer, sizeof(buffer));
		if (ds_selected != -1)
		{
			if (CheckInput()) {
				AddItem(GetInput(), &danhsach[ds_selected]);
				LoadList();
			}
			return;
		}
		else MessageBox(hWnd, L"Vui lòng chọn danh sách", L"Thông báo!", MB_OK);
		break;
	}
	
	case IDC_BUTTONADDDS: {
		WCHAR* buffer = new WCHAR[255];
		GetWindowText(cmb_danhsach, buffer, 254);
		for (int i = 0; i < danhsach.size(); i++)
			if (wcscmp(danhsach[i].name, buffer) == 0)
			{
				MessageBox(hWnd, L"Danh sách đã tồn tại!Vui lòng tạo danh sách mới!", L"Rất tiếc!", MB_OK);
				return;
			}
		DANHSACH tempDS;
		wsprintf(tempDS.name, L"%s\0", buffer);
		danhsach.push_back(tempDS);
		LoadDSList();
		SetWindowText(cmb_danhsach, buffer);
		WCHAR* buff = new WCHAR[255];
		wsprintf(buff, L"Danh sách [%s] được tạo thành công!", buffer);
		MessageBox(hWnd, buff, L"Chúc mừng!", MB_OK);
		break;
	}

						  
	}
}

CHITIEU GetInput()
{
	WCHAR *buffer = new WCHAR[255];
	CHITIEU ct;
	GetWindowText(loaiCT, ct.loai, 40);
	GetWindowText(noidungCT, ct.noidung, 255);
	GetWindowText(sotienCT, buffer, 255);
	ct.sotien = _wtof(buffer);
	return ct;
}

bool LoadDataFromFile(HWND hWnd)
{
	const int BUFFERSIZE = 260;
	WCHAR *buffer = new WCHAR[BUFFERSIZE];

	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	buffer = L"%s\\quanlichitieu.dat";
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, buffer, curPath);
	hFile = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		CloseHandle(hFile);
		return 0;
	}

	DWORD fileSize = GetFileSize(hFile, 0);
	DWORD BytesRead = 0;
	WCHAR* buff = new WCHAR[255];

	//check empty file
	if (0 == fileSize) {
		CloseHandle(hFile);
		return 0;
	}
	int numberOfDS = 0;
	unsigned long i = 0;
	unsigned long begin = 0;
	buff = new WCHAR[fileSize];
	int t = 0;
	if (ReadFile(hFile, buff, fileSize, &BytesRead, NULL)) {
		buffer = new WCHAR[255];
		while (buff[i] != L'\n') i++;
		t = 0;
		for (unsigned long j = begin; j <= i; j++)
		{
			buffer[t++] = buff[j];
		}
		numberOfDS = _wtoi(buffer);
		i++;
		for (int index_DS = 0; index_DS < numberOfDS; index_DS++)
		{
			DANHSACH *tempDS = new DANHSACH;
			begin = i;
			while (buff[i] != L'\n') i++;
			tempDS->name = new WCHAR[i - begin];
			int sizeName = 0;
			for (unsigned long j = begin; j < i; j++)
			{
				tempDS->name[sizeName++] = buff[j];
			}
			tempDS->name[sizeName] = L'\0';
			i++;
			begin = i;
			buffer = new WCHAR[255];
			while (buff[i] != L'\n') i++;
			t = 0;
			for (unsigned long j = begin; j <= i; j++)
			{
				buffer[t++] = buff[j];
			}
			int amountOfCT = _wtoi(buffer);
			i++;
				for (int index_CT = 0; index_CT < amountOfCT; index_CT++)
				{
					CHITIEU tempCT;
					begin = i;
					while (buff[i] != L'|') i++;
					tempCT.loai = new WCHAR[i - begin];
					int sizeName = 0;
					for (unsigned long j = begin; j < i; j++)
					{
						tempCT.loai[sizeName++] = buff[j];
					}
					tempCT.loai[sizeName] = L'\0';
					i++;
					begin = i;
					while (buff[i] != L'|') i++;
					tempCT.noidung = new WCHAR[i - begin];
					sizeName = 0;
					for (unsigned long j = begin; j < i; j++)
					{
						tempCT.noidung[sizeName++] = buff[j];
					}
					tempCT.noidung[sizeName] = L'\0';
					i++;
					begin = i;
					while (buff[i] != L'\n') i++;
					buffer = new WCHAR[i - begin];
					sizeName = 0;
					for (unsigned long j = begin; j < i; j++)
					{
						buffer[sizeName++] = buff[j];
					}
					buffer[sizeName] = L'\0';
					tempCT.sotien = _wtof(buffer);
					AddItem(tempCT, tempDS);
					i++;
				}
 				danhsach.push_back(*tempDS);
				if (0 == amountOfCT) i++;
		}

	}
	CloseHandle(hFile);
	return 1;
}
void UpdateDataToFile()
{
	const int BUFFERSIZE = 260;
	WCHAR *buffer = new WCHAR[BUFFERSIZE];

	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	buffer = L"%s\\quanlichitieu.dat";
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, buffer, curPath);
	hFile = CreateFileW(filePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	{
		if (0 == danhsach.size()) return;
		DWORD NumberOfBytesWriten = 0;
		buffer = new WCHAR[255];
		swprintf(buffer, 255, L"%d\n", (int)danhsach.size());
		WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
		for (int i = 0; i < danhsach.size(); i++)
		{
			buffer = new WCHAR[255];
			wcscpy_s(buffer, 254, danhsach[i].name);
			wcscat_s(buffer, 255, L"\n");
			WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
			buffer = new WCHAR[255];
			swprintf(buffer, 255, L"%d\n", (int)danhsach[i].ctieu.size());
			WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
			for (int j = 0; j < danhsach[i].ctieu.size(); j++)
			{
				buffer = new WCHAR[255];
				wcscpy_s(buffer, 254, danhsach[i].ctieu[j].loai);
				wcscat_s(buffer, 255, L"|");
				WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);

				buffer = new WCHAR[255];
				wcscpy_s(buffer, 254, danhsach[i].ctieu[j].noidung);
				wcscat_s(buffer, 255, L"|");
				WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
				
				buffer = new WCHAR[255];
				swprintf(buffer, 255, L"%.0f\n", danhsach[i].ctieu[j].sotien);
				WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
			}
		}
		CloseHandle(hFile);
	}
}


void LoadDSList()
{
	SendMessage(cmb_danhsach, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < danhsach.size(); i++)
	{
		SendMessage(cmb_danhsach, CB_ADDSTRING, 0, (LPARAM)(danhsach[i].name));
	}
}
bool CheckInput()
{
	CHITIEU tempCT = GetInput();
	if (wcscmp(tempCT.loai, L"") == 0)
	{
		MessageBox(hWnd, L"box 'Loại' không được bỏ trống!", L"Thông báo!!", MB_OK);
		return 0;
	}
	if (tempCT.sotien <= 0.0)
	{
		MessageBox(hWnd, L"'Số tiền' không đúng, vui lòng nhập lại!", L"Thông báo!!", MB_OK);
		return 0;
	}
	WCHAR *list_loai[6] = { L"1. Ăn uống", L"2. Di chuyển", L"3. Nhà cửa", L"4. Xe cộ", L"5. Nhu yếu phẩm", L"6. Dịch vụ" };
	int i = 0;
	for(; i < 6; i++)
		if (wcscmp(tempCT.loai, list_loai[i]) == 0)
		{
			break;
		}
	if (6 == i) {
		MessageBox(hWnd, L"Vui lòng chọn loại chi tiêu !!", L"Thông báo!!", MB_OK);
		return 0;
	}
	return 1;
}
void AddItem(CHITIEU ct, DANHSACH *DShientai)
{
	DShientai->ctieu.push_back(ct);
}

void CreateBrush(HWND hWnd)
{

	HBRUSH tempBrush;
	tempBrush = CreateSolidBrush(RGB(255, 0, 0));
		hbrushs.push_back(tempBrush);
	tempBrush = CreateSolidBrush(RGB(5, 229, 21));
		hbrushs.push_back(tempBrush);
	tempBrush = CreateSolidBrush(RGB(6, 6, 240));
		hbrushs.push_back(tempBrush);
	tempBrush = CreateSolidBrush(RGB(222, 219, 5));
		hbrushs.push_back(tempBrush);
	tempBrush = CreateSolidBrush(RGB(235, 6, 178));
		hbrushs.push_back(tempBrush);
	tempBrush = CreateSolidBrush(RGB(6, 255, 223));
		hbrushs.push_back(tempBrush);

		
		CRectangle tempRect;
		rectangles.resize(0);
		rectchart.resize(0);
		int cao1 = 390, cao2 = 415;
		int *r = new int[7];
		for (int i = 0; i < 7; i++) 
		{ 
			r[i] = 2; 
		};
		r[0] = 40;
		for (int i = 1; i < 7; i++)
		{
			tempRect.SetData(180, cao1 + i * 25 + 38, r[i] / 2 + 180, cao2 + i * 25 + 28);
			rectchart.push_back(tempRect);
		}
		for (int i = 1; i < 7; i++)
		{
			r[i] += r[i - 1];
			tempRect.SetData(r[i - 1], cao1, r[i], cao2);
			rectangles.push_back(tempRect);
		}
		InvalidateRect(temphwnd, NULL, TRUE);
}

void CreateChart()
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	int cao1 = 390, cao2 = 415;
	int *r = new int[7];
	double *tien = new double[7];
	for (int i = 0; i < 7; i++) 
	{ 
		r[i] = 2; 
		tien[i] = 0;
	};

	for (int i = 0; i < danhsach[ds_selected].ctieu.size(); i++)
	{
		WCHAR *temp = new WCHAR[2];
		temp[0] = danhsach[ds_selected].ctieu[i].loai[0];
		temp[1] = L'\0';
		int stt = _wtoi(temp);
		r[stt] += (danhsach[ds_selected].ctieu[i].sotien * 490 / danhsach[ds_selected].tongcong );
		tien[stt] += danhsach[ds_selected].ctieu[i].sotien;
	}
	r[0] = 40;

	rectangles.resize(0);
	rectchart.resize(0);
	CRectangle tempRect;
	WCHAR *buffer = new WCHAR[255];
	HWND hwnd;
	for (int i = 1; i < 7; i++)
	{
		tempRect.SetData(220, cao1 + i * 25 + 38, r[i] / 2 + 220, cao2 + i * 25 + 28);
		rectchart.push_back(tempRect);
		swprintf(buffer, 255, L"%.1f", tien[i]);
		hwnd = CreateWindowEx(0, L"STATIC", buffer, WS_CHILD | WS_VISIBLE | BS_CENTER, 130, 425 + i * 25, 35, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		if (0 != tien[i])
			swprintf(buffer, 255, L"%.1f%%", tien[i] *100 / danhsach[ds_selected].tongcong);
		else wcscpy_s(buffer, 254, L"0.0%");
		hwnd = CreateWindowEx(0, L"STATIC", buffer, WS_CHILD | WS_VISIBLE | BS_CENTER, 170, 425 + i * 25, 35, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	}
	for (int i = 1; i < 7; i++)
	{

		r[i] += r[i - 1];
		tempRect.SetData(r[i - 1], cao1, r[i], cao2);
		rectangles.push_back(tempRect);

	}

	InvalidateRect(temphwnd, NULL, TRUE);
}
void LoadList()
{
	if (ds_selected == -1) return;
	SetWindowText(loaiCT, L"");
	SetWindowText(noidungCT, L"");
	SetWindowText(sotienCT, L"");
	SendMessage(ds_listview, LVM_DELETEALLITEMS, 0, 0);
	WCHAR *buffer = new WCHAR[255];
	LVITEM lv;
	lv.pszText = LPSTR_TEXTCALLBACK; 
	lv.mask = LVIF_TEXT;
	lv.iSubItem = 0;
	lv.iItem = 0;
	for (unsigned int i = 0; i < (unsigned int)danhsach[ds_selected].ctieu.size(); i++)
	{
		lv.pszText = danhsach[ds_selected].ctieu[i].loai;
		ListView_InsertItem(ds_listview, &lv);
		ListView_SetItemText(ds_listview, 0, 1, danhsach[ds_selected].ctieu[i].noidung);
		WCHAR *buffer = new WCHAR[255];
		
		swprintf(buffer, 255, L"%.0f", danhsach[ds_selected].ctieu[i].sotien);
		ListView_SetItemText(ds_listview, 0, 2, buffer);
	}
	danhsach[ds_selected].tongcong = 0;
	for (int i = 0; i < danhsach[ds_selected].ctieu.size(); i++)
	{
		danhsach[ds_selected].tongcong += danhsach[ds_selected].ctieu[i].sotien;
	}
	buffer = new WCHAR[255];
	swprintf(buffer, 255, L"%.0f", danhsach[ds_selected].tongcong);
	SetWindowText(tongcong, buffer);
	CreateChart();
}

int CheckSelectedDS()
{
	WCHAR *buffer = new WCHAR[255];
	GetWindowText(cmb_danhsach, buffer, 254);
	{
		for (int i = 0; i < danhsach.size(); i++)
		{
			if (wcscmp(danhsach[i].name, buffer) == 0) {
				return i;
			}
		}
	}
	return -1;
	
}

void CheckSelectedItem() {

	item_selected = SendMessage(ds_listview, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (item_selected != -1) {
		EnableWindow(button_del, 1);
		EnableWindow(button_change, 1);
	}
	else
	{
		EnableWindow(button_del, 0);
		EnableWindow(button_change, 0);
	}
}

