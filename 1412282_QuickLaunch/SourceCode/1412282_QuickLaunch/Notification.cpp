
#include "stdafx.h"
#include <shellapi.h>
#include "Resource.h"
#include "Notification.h"



BOOL CreateNotifyIcon(HWND hWnd, HINSTANCE hInstance)
{
	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = 100;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_NOTIMESSAGE;
	nid.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MY1412282QUICKLAUNCH), IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	swprintf(nid.szTip, ARRAYSIZE(nid.szTip), L"DoubleClick to Open QuickLaunch");
	// Show the notification.
	return Shell_NotifyIcon(NIM_ADD, &nid);
}

void DeleteNotifyIcon(HWND hWnd)
{
	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = 100;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		InsertMenu(hMenu, -1, MF_BYPOSITION, NOTI_EXIT, _T("Exit"));
		InsertMenu(hMenu, 0, MF_BYPOSITION, NOTI_SCAN, _T("Scan folder"));
		InsertMenu(hMenu, 1, MF_BYPOSITION, NOTI_STATICS, _T("Statistic"));
		SetForegroundWindow(hWnd);
		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}