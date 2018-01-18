#pragma once
#include "stdafx.h"
#include "ScanApp.h"
#include <shellapi.h>
#include <queue>
#include "shlobj.h"
#include <string>
#include <iostream>
using namespace std;

int get_path_File_list(wstring path, vector<wstring>  &list_file)
{

	list_file.clear();
	HANDLE hFile;
	WIN32_FIND_DATA data;
	queue<WCHAR*> list_Folder;
	WCHAR path_current_folder[255];
	// TODO: them \*
	path += L"\\*";
	WCHAR *root_path = new WCHAR[255];
	wcscpy_s(root_path, 255, path.c_str());
	list_Folder.push(root_path);

	while (!list_Folder.empty())
	{
		hFile = FindFirstFileEx(list_Folder.front(), FindExInfoBasic, &data, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
		wcscpy_s(path_current_folder, 255, list_Folder.front());
		path_current_folder[wcslen(path_current_folder) - 1] = path_current_folder[wcslen(path_current_folder)];
		WCHAR *pfolder = list_Folder.front();
		list_Folder.pop();
		delete pfolder;
		WCHAR *temp_path;
		if (hFile != INVALID_HANDLE_VALUE)
		{
			do {
				if ((wcscmp(data.cFileName, L".") != 0) && (wcscmp(data.cFileName, L"..") != 0))
				{

					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						temp_path = new WCHAR[255];
						swprintf_s(temp_path, 255, L"%s%s\\*", path_current_folder, data.cFileName);
						list_Folder.push(temp_path);
					}

					else if ((data.cFileName[wcslen(data.cFileName) - 3] == 'e') && (data.cFileName[wcslen(data.cFileName) - 2] == 'x') && (data.cFileName[wcslen(data.cFileName) - 1] == 'e'))
					{
						temp_path = new WCHAR[500];
						swprintf_s(temp_path, 500, L"%s%s", path_current_folder, data.cFileName);
						list_file.push_back(temp_path);
						delete temp_path;
					}

				}
			} while (FindNextFile(hFile, &data));
			FindClose(hFile);
		}
		else FindClose(hFile);
	}
	return list_file.size();
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if (uMsg == BFFM_INITIALIZED)
	{
		string tmp = (const char *)lpData;
		cout << "path: " << tmp << endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}


string BrowseFolder(string saved_path)
{
	TCHAR path[MAX_PATH];

	const char * path_param = saved_path.c_str();

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Browse for folder...";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
		wstring tmp_wstr = path;
		string str = string(tmp_wstr.begin(), tmp_wstr.end());
		return  str;
	}

	return "";
}