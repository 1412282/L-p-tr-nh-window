#pragma once
#include "stdafx.h"


class CSHAPE {
protected:
	int left;
	int top;
	int right;
	int bottom;
public:
	void SetData(int a, int b, int c, int d)
	{
		left = a;
		top = b;
		right = c;
		bottom = d;
	}
	void virtual Draw(HDC hdc) = 0;
};

class CLINE : public CSHAPE {
public:

	void Draw(HDC hdc)
	{
		MoveToEx(hdc, left, top, NULL);
		LineTo(hdc, right, bottom);
	}
};

class CRECTANGLE : public CSHAPE {
public:
	void Draw(HDC hdc)
	{
		Rectangle(hdc, left, top, right, bottom);
	}
};


class CELLIPSE : public CSHAPE {
public:
	void Draw(HDC hdc)
	{
		Ellipse(hdc, left, top, right, bottom);
	}
};

