#pragma once

#define WM_NOTIMESSAGE WM_USER+1

#define NOTI_EXIT WM_NOTIMESSAGE + 1 // Exit
#define NOTI_SCAN WM_NOTIMESSAGE + 2 // Scan files
#define NOTI_STATICS WM_NOTIMESSAGE + 3 //
BOOL CreateNotifyIcon(HWND hWnd, HINSTANCE hInstance);
void DeleteNotifyIcon(HWND hWnd);
void ShowContextMenu(HWND hWnd);