#pragma once
#define WM_OPENSEARCHBOX WM_USER + 20

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void doInstallHook(HWND hWnd);
void doRemoveHook(HWND hWnd);
static bool active = TRUE;