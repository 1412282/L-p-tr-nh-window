#pragma once

#include "resource.h"
#include "shapes.h"
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);	
void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
void SetShape();