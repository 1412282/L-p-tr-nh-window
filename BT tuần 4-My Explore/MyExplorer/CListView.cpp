#include "CListView.h"
#include <tchar.h>
#include "CDrive.h"
#include "CExtractor.h"
#include "CConverter.h"
#include "resource.h"

#include <shlobj.h> //Shell object

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//Vị trí 8 icon cơ bản trong file Shell32.dll
#define IDI_SHELL32_FOLDER 3
#define IDI_SHELL32_UNKNOWN_FILE 0 //Chú ý khác với Tree-View là FOLDER_SELECTED
#define IDI_SHELL32_DESKTOP 34
#define IDI_SHELL32_MYCOMPUTER 15

#define IDI_SHELL32_FLOPPY 6
#define IDI_SHELL32_USB 7
#define IDI_SHELL32_HDD 8
#define IDI_SHELL32_CD  11 

//Vị trí trong image list sau khi add
#define IDI_FOLDER 0
#define IDI_UNKNOWN_FILE 1
#define IDI_DESKTOP 2
#define IDI_MYCOMPUTER 3

#define IDI_FLOPPY 4
#define IDI_USB 5
#define IDI_HDD 6
#define IDI_CD  7 

#define INITIAL_ICON_IN_TREE 8 //Tổng số icon lúc đầu trong tree, mặc định chỉ nạp những gì cần thiết
#define NUMBER_OF_ICON_TO_GROW 65535 //Số icon có thể mở rộng

//Độ dài tối đa đường dẫn
#define MAX_PATH_LEN 10240
/*****************************************************************************************/

CListView::CListView()
{
	m_hInst = NULL;
	m_hParent = NULL;
	m_hListView = NULL;
	m_nID = 0;
}

CListView::~CListView()
{
	DestroyWindow(m_hListView);
}

void CListView::Create(HWND parentWnd, long ID, HINSTANCE hParentInst, 
					   int nWidth, int nHeight, 
					   int x, int y, long lExtStyle, long lStyle)
{
	InitCommonControls();
	CListView::m_hInst = hParentInst;
	CListView::m_hParent = parentWnd;
	CListView::m_hListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"), 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, m_hParent, (HMENU) ID, m_hInst, NULL);
	CListView::m_nID = ID;

	InitImageList();
	Init4Cols();
}

void CListView::Init4Cols()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt=LVCFMT_LEFT;

	lvCol.cx = 130;
	lvCol.pszText = _T("Tên");
	ListView_InsertColumn(m_hListView, 0, &lvCol);

	lvCol.fmt=LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại");
	ListView_InsertColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Kích thước");
	ListView_InsertColumn(m_hListView, 2, &lvCol);

	lvCol.pszText = _T("Mô tả");
	ListView_InsertColumn(m_hListView, 3, &lvCol);
}

void CListView::InitDriveColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_TEXT| LVCF_FMT;

	lvCol.fmt=LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại");
	ListView_SetColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
	lvCol.cx = 80;
	lvCol.pszText = _T("Tổng dung lượng");
	ListView_SetColumn(m_hListView, 2, &lvCol);
	
	lvCol.cx = 80;
	lvCol.pszText = _T("Dung lượng trống");
	ListView_SetColumn(m_hListView, 3, &lvCol);

}
void CListView::InitFolderColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_WIDTH;
	lvCol.cx = 180;
	ListView_SetColumn(m_hListView, 0, &lvCol);

	lvCol.mask = LVCF_TEXT | LVCF_FMT;
	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Kích thước");
	ListView_SetColumn(m_hListView, 1, &lvCol);


	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 130;
	lvCol.pszText = _T("Loại tập tin");
	ListView_SetColumn(m_hListView, 2, &lvCol);
	
	lvCol.pszText = _T("Ngày chỉnh sửa");
	ListView_SetColumn(m_hListView, 3, &lvCol);
}
//Nạp 8 icon cơ bản
void CListView::InitImageList()
{
	HIMAGELIST *hLarge = new HIMAGELIST;
	HIMAGELIST *hSmall = new HIMAGELIST;

	*hSmall = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 
		INITIAL_ICON_IN_TREE, NUMBER_OF_ICON_TO_GROW);
	*hLarge = ImageList_Create(32, 32, ILC_COLOR32 | ILC_MASK, 
		INITIAL_ICON_IN_TREE, NUMBER_OF_ICON_TO_GROW);
	
	HICON hIcon;

	//Các icon cho imagelist nhỏ
	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_FOLDER, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_UNKNOWN_FILE, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_DESKTOP, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_MYCOMPUTER, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_FLOPPY, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_USB, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_HDD, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_CD, 16, 16,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hSmall, hIcon);
	//DestroyIcon(hIcon);

	//Các icon cho image list lớn
	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_FOLDER, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_UNKNOWN_FILE, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_DESKTOP, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_MYCOMPUTER, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_FLOPPY, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_USB, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_HDD, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);

	CExtractor::Get(_T("%SystemRoot%\\System32\\Shell32.dll"), IDI_SHELL32_CD, 32, 32,
		&hIcon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(*hLarge, hIcon);
	//DestroyIcon(hIcon);
	
	ListView_SetImageList(m_hListView, *hSmall, LVSIL_SMALL);
	ListView_SetImageList(m_hListView, *hLarge, LVSIL_NORMAL);
}
/*****************************************************************************************/

void CListView::Size()
{
	RECT tree;
	GetWindowRect(GetDlgItem(m_hParent, IDC_TREEVIEW), &tree);

	RECT main;
	GetWindowRect(m_hParent, &main);

	MoveWindow(m_hListView, tree.right - tree.left, 75, main.right - tree.right, tree.bottom - tree.top, TRUE);
	ListView_Arrange(m_hListView, LVA_ALIGNTOP);
}

HWND CListView::GetHandle()
{
	return m_hListView;
}

LPCWSTR CListView::GetPath(int iItem)
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = iItem;
	lv.iSubItem = 0;
	ListView_GetItem(m_hListView, &lv);
	return (LPCWSTR)lv.lParam;
}

LPCWSTR CListView::GetCurSelPath()
{
	return GetPath(ListView_GetSelectionMark(m_hListView));
}

LPCWSTR CListView::GetCurParentPath()
{
	LPCWSTR path = GetPath(0);
	if (path == NULL)
		return NULL;

	if (!StrCmp(path, L"MyComputer")) //Cha đang là Desktop
		return _T("Desktop");
	else
		if (wcslen(path) == 3) //Cha đang là My Computer
			return _T("MyComputer");
		else
		{	
			TCHAR *parent;
			int nBackSlachPos = (StrRStrI(path, NULL, _T("\\")) - path);
			parent = new TCHAR[nBackSlachPos+2];
			StrNCpy(parent, path, nBackSlachPos+1);
			return parent;
		}
}

void CListView::GetFocus()
{
	SetFocus(m_hListView);
}
/*****************************************************************************************/

void CListView::LoadDesktop()
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, _T("Desktop"));
	DeleteAll();
	InitFolderColumn();

	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lv.iItem = 0;
	lv.iSubItem = 0;
	lv.pszText = _T("My Computer");
	lv.iImage = IDI_MYCOMPUTER;
	lv.lParam = (LPARAM)_T("MyComputer");
	ListView_InsertItem(m_hListView, &lv);

	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 2, (LPARAM)_T("Desktop"));
}
void CListView::LoadMyComputer(CDrive *drive)
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, _T("My Computer"));
	InitDriveColumn();
	DeleteAll();
	LV_ITEM lv;
	
	for (int i = 0; i < drive->GetCount(); ++i)
	{
		//Nạp cột đầu tiên cũng là thông tin chính
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iItem = i;	
		lv.iImage = drive->GetIconIndex(i);

		lv.iSubItem = 0;
		lv.pszText = drive->GetDisplayName(i);
		lv.lParam = (LPARAM) drive->GetDriveName(i);
		ListView_InsertItem(m_hListView, &lv);

		//Nạp các cột còn lại (Type, Size, Free Space)
		lv.mask = LVIF_TEXT;

		//Cột đầu tiên là Type
		lv.iSubItem = 1;
		lv.pszText = drive->GetType(i);
		ListView_SetItem(m_hListView, &lv);
		
		//Cột tiếp theo là Size
		lv.iSubItem = 2;

		if (drive->GetIconIndex(i) != IDI_FLOPPY && drive->GetIconIndex(i) != IDI_CD )
			lv.pszText = drive->GetSizeStr(i);
		else
			lv.pszText = NULL;

		ListView_SetItem(m_hListView, &lv);

		//Cột cuối cùng là Free Space
		lv.iSubItem = 3;

		if (drive->GetIconIndex(i) != IDI_FLOPPY && drive->GetIconIndex(i) != IDI_CD )
			lv.pszText = drive->GetFreeSizeStr(i);
		else
			lv.pszText = NULL;

		ListView_SetItem(m_hListView, &lv);
	}//for
	
	TCHAR *buffer = new TCHAR[34];
	wsprintf(buffer, _T("My Computer có tổng cộng %d ổ đĩa"), drive->GetCount());
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 0, (LPARAM) buffer);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 2, (LPARAM)_T("My Computer"));
} 

void CListView::LoadChild(LPCWSTR path, CDrive *drive)
{
	if (path == NULL)
		return;

	if (!StrCmp(path, _T("Desktop")))
		LoadDesktop();
	else
		if (!StrCmp(path, _T("MyComputer")))
			LoadMyComputer(drive);
		else
			LoadFileAndFolder(path);
}

void CListView::LoadFileAndFolder(LPCWSTR path)
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, path);
	InitFolderColumn();
	DeleteAll();

	TCHAR buffer[10240];
	StrCpy(buffer, path);

	if ( wcslen(path) == 3) //Nếu quét các ổ đĩa
		StrCat(buffer, _T("*"));
	else
		StrCat(buffer, _T("\\*"));

	//Bắt đầu tìm các file và folder trong thư mục
	WIN32_FIND_DATA fd;
	HANDLE hFile;
	BOOL bFound = true;
	LV_ITEM lv;

	TCHAR * folderPath;
	int nItemCount = 0;

	//Chạy lần thứ nhất lấy các thư mục
	hFile = FindFirstFileW(buffer, &fd);
	bFound = TRUE;

	if (hFile == INVALID_HANDLE_VALUE)
		bFound = FALSE;

	while (bFound)
	{
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) &&
			(StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0) )
		{
			folderPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(folderPath, path);

			if (wcslen(path) != 3)
				StrCat(folderPath, _T("\\"));

			StrCat(folderPath, fd.cFileName);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = nItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;	
			lv.lParam = (LPARAM) folderPath;
			lv.iImage = IDI_FOLDER;	
			ListView_InsertItem(m_hListView, &lv);

			//Bỏ qua cột thứ hai là Size (cho giống Explorer, chỉ hiển thị dung lượng
			//của thư mục ở thanh status bar thôi

			//Cột thứ ba là cột Type
			ListView_SetItemText(m_hListView, nItemCount, 2, _T("Thư mục"));

			//Cột thứ tư là cột Date modified
			ListView_SetItemText(m_hListView, nItemCount, 3, _GetDateModified(fd.ftLastWriteTime));
			++nItemCount;
		}//if

		bFound = FindNextFileW(hFile, &fd);
	}//while
	
	DWORD folderCount = nItemCount;
	/*************************************************************************************/
	//Chạy lần thứ hai để lấy các tập tin T_T Cách bưởi chưa từng thấy !!! Nhưng không còn cách nào khác
	TCHAR *filePath;
	DWORD fileSizeCount = 0;
	DWORD fileCount = 0;

	hFile = FindFirstFileW(buffer, &fd);
	bFound = TRUE;

	if (hFile == INVALID_HANDLE_VALUE)
		bFound = FALSE;

	while (bFound) 
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
			 ((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
			  ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) )		
		{
			filePath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(filePath, path);

			if (wcslen(path) != 3)
				StrCat(filePath, _T("\\"));

			StrCat(filePath, fd.cFileName);
			
			//Cột thứ nhất là tên hiển thị của tập tin
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = nItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;	
			lv.lParam = (LPARAM) filePath;
			lv.iImage = _GetImageListIconIndex(filePath, fd); 
			ListView_InsertItem(m_hListView, &lv);	
			
			//Cột thứ hai là Size
			ListView_SetItemText(m_hListView, nItemCount, 1, _GetSize(fd));
			fileSizeCount+= fd.nFileSizeLow;

			//Cột thứ ba là Type
			ListView_SetItemText(m_hListView, nItemCount, 2, _GetType(fd));

			//Cột thứ tư là Date Modified	
			ListView_SetItemText(m_hListView, nItemCount, 3, _GetDateModified(fd.ftLastWriteTime));

			++nItemCount;
			++fileCount;
		}//if

		bFound = FindNextFileW(hFile, &fd);
	}//while

	TVITEMEX tv;
	TCHAR *folder = new TCHAR[512];
	TCHAR *info = new TCHAR[256];

	tv.mask = TVIF_TEXT;
	tv.hItem = TreeView_GetNextItem(GetDlgItem(m_hParent, IDC_TREEVIEW), NULL, TVGN_CARET);
	tv.pszText = folder;
	tv.cchTextMax = 256;
	TreeView_GetItem(GetDlgItem(m_hParent, IDC_TREEVIEW), &tv);

	wsprintf(info, _T(" có %d thư mục con và %d tập tin. (Không tính ẩn và hệ thống)"), folderCount, fileCount);
	StrCat(folder, info);
	
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 0, (LPARAM)tv.pszText);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 1, (LPARAM)CConverter::Convert(fileSizeCount));
}

void CListView::LoadCurSel()
{
	LPCWSTR path = GetCurSelPath();

	WIN32_FIND_DATA fd;
	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(m_hListView);
		LoadFileAndFolder(path);
	}
	else //Nếu là tập tin thì chạy nó
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}
void CListView::DisplayInfoCurSel()
{
	int nCurSelIndex = ListView_GetNextItem(GetDlgItem(m_hParent, IDC_LISTVIEW), -1, LVNI_FOCUSED);
	TCHAR *text = new TCHAR[256];
	LVITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iItem = nCurSelIndex;
	lv.iSubItem = 0;
	lv.pszText = text;
	lv.cchTextMax = 256;
	
	ListView_GetItem(m_hListView, &lv);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 2,(LPARAM) text);

	lv.iSubItem = 2;
	ListView_GetItem(m_hListView, &lv);

	if (!StrCmpI(lv.pszText, _T("Thư mục")))
		SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 1, NULL);
	else
	{
		lv.iSubItem = 1;
		ListView_GetItem(m_hListView, &lv);
		SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 1,(LPARAM) text);
	}

}
/*****************************************************************************************/

void CListView::ChangeViewOption(int nNewStyle)
{
	LONG dNotView = ~(LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_REPORT);

	SetWindowLong(m_hListView, GWL_STYLE, GetWindowLong(m_hListView, GWL_STYLE) & dNotView | nNewStyle);

}
void CListView::DeleteAll()
{
	ListView_DeleteAllItems(m_hListView);
}
void CListView::SelectAll()
{
	int nOld = ListView_GetSelectionMark(m_hListView);
	ListView_SetItemState(m_hListView, nOld, 0, LVIS_FOCUSED | LVIS_SELECTED);

	for (int i = ListView_GetItemCount(m_hListView) - 1; i >= 0; --i)
	{
		ListView_SetItemState(m_hListView, i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		ListView_SetSelectionMark(m_hListView, i);
	}

	GetFocus();
}
void CListView::Up(CDrive *drive)
{
	LPCWSTR path = GetPath(0);
	if (!StrCmp(path, L"MyComputer"))
		return;
	
	if (wcslen(path) == 3) //Ổ đĩa
	{
		DeleteAll();
		LoadDesktop();
		return;
	}

	TCHAR *parent;
	TCHAR *grandPa;
	int nBackSlachPos = (StrRStrI(path, NULL, _T("\\")) - path);
	if (nBackSlachPos == 2)
	{
		DeleteAll();
		LoadMyComputer(drive);
		return;
	}

	parent = new TCHAR[nBackSlachPos+2];
	StrNCpy(parent, path, nBackSlachPos+1);
	nBackSlachPos = (StrRStrI(parent, NULL, _T("\\")) - parent);

	grandPa = new TCHAR[nBackSlachPos  + 2];
	StrNCpy(grandPa, parent, nBackSlachPos+1);
	DeleteAll();
	LoadFileAndFolder(grandPa);
}

/*****************************************************************************************/

LPWSTR CListView::_GetSize(const WIN32_FIND_DATA &fd)
{
	DWORD dwSize = fd.nFileSizeLow;

	return CConverter::Convert(dwSize);
}

#define ENTIRE_STRING NULL

LPWSTR CListView::_GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);
	
	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return _T("Không biết");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi
	
	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;
	
	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszPath, &dwSize)!= ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	return pszPath;
}

LPWSTR CListView::_GetDateModified(const FILETIME &ftLastWrite)
{

	//Chuyển đổi sang local time
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&ftLastWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	
	TCHAR *buffer = new TCHAR[50];
	wsprintf(buffer,_T("%02d/%02d/%04d %02d:%02d %s"), 
					stLocal.wDay, stLocal.wMonth, stLocal.wYear, 
					(stLocal.wHour>12)?(stLocal.wHour/12):(stLocal.wHour),
					stLocal.wMinute,
					(stLocal.wHour>12)?(_T("Chiều")):(_T("Sáng")));

	return buffer;
}

int CListView::_GetImageListIconIndex(LPCWSTR path, const WIN32_FIND_DATA &fd)
{
	unsigned int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	unsigned int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return IDI_UNKNOWN_FILE;

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	unsigned int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi
	
	HICON hicon;
	if (!StrCmpI(szExtension, _T(".exe")))
	{
		CExtractor::Get(path, 0, 16, 16, &hicon, NULL, 1, LR_COLOR);
		ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

		CExtractor::Get(path, 0, 32, 32, &hicon, NULL, 1, LR_COLOR);
		return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
	}
	
	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		CExtractor::Get(_T("%ProgramFiles%\\Internet Explorer\\iexplore.exe"), 0, 16, 16, &hicon, NULL, 1, LR_COLOR);
		ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

		CExtractor::Get(_T("%ProgramFiles%\\Internet Explorer\\iexplore.exe"), 0, 32, 32, &hicon, NULL, 1, LR_COLOR);
		return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
	}

	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return IDI_UNKNOWN_FILE;
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE) pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return IDI_UNKNOWN_FILE;
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR pszOut1[256];
	StrCpy(pszOut1, pszOut);
	StrCat(pszOut1, _T("\\DefaultIcon"));

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut1, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return IDI_UNKNOWN_FILE;
	}//if

	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszPath, &dwSize)!= ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return IDI_UNKNOWN_FILE;
	}

	RegCloseKey(hKey);

	//Có được đường dẫn đến file chứa icon và vị trí icon trong file nhưng phải xử lí
	//Tìm coi có dấu chấm không
	nDotPos = StrRStrI(pszPath, ENTIRE_STRING, _T(".")) - pszPath;
	len = wcslen(pszPath);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không có dấu chấm
		return IDI_UNKNOWN_FILE;

	//Tìm coi có dấu phẩy không
	int nIconIndex;
	unsigned int nCommaPos = StrRStrI(pszPath, ENTIRE_STRING, _T(",")) - pszPath;
	TCHAR *filePath;

	if (nCommaPos < 0 || nCommaPos >= len) //Nếu không tìm thấy dấu phẩy thì mặc định là 0
	{
		nIconIndex = 0;
		filePath = new TCHAR[len];
		StrCpy(filePath, pszPath);
	}
	else
	{
		TCHAR *szIndex = new TCHAR[len - nCommaPos];
		
		++nCommaPos;
		for (i = nCommaPos ; i < len; ++i)
			szIndex[i - nCommaPos] = pszPath[i];
		szIndex[i - nCommaPos] = NULL;

		nIconIndex = _wtoi(szIndex);//Chuyển sang số là xong
		filePath = new TCHAR[nCommaPos];
		StrNCpy(filePath, pszPath, nCommaPos);
	}

	CExtractor::Get(filePath, nIconIndex, 16, 16, &hicon, NULL, 1, LR_COLOR);
	ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

	CExtractor::Get(filePath, nIconIndex, 32, 32, &hicon, NULL, 1, LR_COLOR);
	return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
}	
