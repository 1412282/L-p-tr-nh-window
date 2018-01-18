#include "stdafx.h"
#include "Hook.h"


HHOOK hHook = NULL;
HINSTANCE hinstLib;
KBDLLHOOKSTRUCT hkey;
HWND current_hWnd = NULL;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {

		hkey = *(KBDLLHOOKSTRUCT*)lParam;

		//check have turn on or turn off ?
		if (hkey.vkCode == VK_SPACE && GetAsyncKeyState(VK_LWIN))
		{
			active = !active;
			MessageBeep(1);
			PostMessage(current_hWnd, WM_OPENSEARCHBOX, 0, 0);
			CallNextHookEx(hHook, nCode, wParam, lParam);
		}

	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void doInstallHook(HWND hWnd)
{
	current_hWnd = hWnd;
	if (hHook != NULL) return;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hinstLib, 0);
	if (hHook == NULL) {
		MessageBox(hWnd, L"Setup keyboard hook Failed", L"Error", MB_OK);
	}
}

void doRemoveHook(HWND hWnd)
{
	if (NULL == hHook) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}

