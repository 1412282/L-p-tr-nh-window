
#pragma once

#include "resource.h"
#include <vector>
#include <commctrl.h>
using namespace std;
//global variable
int item_selected, ds_selected, ds_selected_old;
HWND ds_listview, cmb_danhsach;
HWND button_add, button_change, button_del;
HWND button_save, button_cancel;
HWND loaiCT, noidungCT, sotienCT, tongcong;
HANDLE hFile;

//function
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnDestroy(HWND hWnd);
void OnPaint(HWND hWnd);
void UpdateDataToFile();
bool LoadDataFromFile(HWND hWnd);

int  CheckSelectedDS();
void CheckSelectedItem();
void LoadDSList();
bool CheckInput();


struct CHITIEU{
	WCHAR* loai = new WCHAR[255];
	WCHAR* noidung = new WCHAR[255];
	double sotien = NULL;
};

CHITIEU GetInput();

class DANHSACH{
public:
	vector<CHITIEU> ctieu;
	WCHAR* name = new WCHAR[255];
	double tongcong = 0.0;
public:

	void Them(CHITIEU x)
	{
		this->ctieu.push_back(x);
	}
	void Capnhat(int phantu, CHITIEU x)
	{
		this->ctieu.erase(this->ctieu.begin() + ctieu.size() - phantu - 1);
		this->ctieu.push_back(x);
	}
	void XoaPhanTu(int phantu) 
	{
		//thu tu list vector dao so voi list view
		this->ctieu.erase(this->ctieu.begin() + ctieu.size() - phantu - 1);
	}

};

void LoadList();
void AddItem(CHITIEU ct, DANHSACH *DShientai);


class CRectangle {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc, HBRUSH hbrush) {
		Rectangle(hdc, x1, y1, x2, y2);
		HRGN hRegion = CreateRectRgn(x1, y1, x2, y2);
		FillRgn(hdc, hRegion, hbrush);
	}

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

void CreateBrush(HWND hWnd);
