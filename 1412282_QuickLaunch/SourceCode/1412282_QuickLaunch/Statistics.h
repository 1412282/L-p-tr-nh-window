#pragma once

#include "stdafx.h"
#include "List.h"
#include <commctrl.h>
#include <shellapi.h>
using namespace std;



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

	void SetData(int left, int top, int right, int bottom) {
		x1 = left;
		y1 = top;
		x2 = right;
		y2 = bottom;
	}
};

class Statistics {
private:
	HWND hWnd;
	vector<CRectangle> list_rect;
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	HFONT hFont;
public:
	Statistics()
	{
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&icc);
		// Lấy font hệ thống
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
	}

	void Init(HWND in_hWnd, List_Nodes &list_node, int begin, int bottom, int width)
	{
		hWnd = in_hWnd;
		list_rect.clear();

		vector<int> list_heigh;
		int max_pri = list_node[0].GetPri();
		int per_heigh = (bottom - 30) / max_pri;
		for (int i = begin; i < begin + 10; i++)
		{
			list_heigh.push_back(list_node[i].GetPri() * per_heigh);
		}
		CRectangle temp_rect;

		for (int i = 0; i < 10; i++)
		{
			int left = 140 + i * 90;
			temp_rect.SetData(left, bottom - list_heigh[i], width + left, bottom + 10);
			list_rect.push_back(temp_rect);
		}

	}

	vector<CRectangle>& GetListRect()
	{
		return list_rect;
	}

	void Draw(HDC hdc)
	{
		for (int i = 0; i < (int)list_rect.size(); i++)
		{
			list_rect[i].Draw(hdc, hbrush);
		}
	}

};