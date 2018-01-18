#include "stdafx.h"
#include "Data.h"
#include "List.h"
#include <string>
using namespace std;

DataFile::DataFile()
{
	All_Nodes.SetNameFile(data_file_name);
	All_Nodes.LoadFromFile();
	LoadDataVector();
}

void DataFile::AddListFile(List_Nodes &list_add)
{
	const int BUFFERSIZE = 260;
	WCHAR curPath[BUFFERSIZE];
	WCHAR buffer[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, curPath);

	vector<int> exist_list = CheckExistList(list_add);
	if (exist_list.size() != 0) {
		for (int count_list = 0; count_list < (int)exist_list.size(); count_list++)
		{
			vector_List_File[exist_list[count_list]].Sync_List(list_add);
			wsprintf(buffer, L"%s\\%s.txt", curPath, vector_List_File[exist_list[count_list]].GetFileName().c_str());
			DeleteFile(buffer);

		}
		for (int count_list = 0; count_list < (int)exist_list.size(); count_list++)
		{
			vector_List_File.erase(vector_List_File.begin() + exist_list[count_list] - count_list);
		}
		list_add.ReSortList();
		list_add.SaveToFile(list_add.GetFileName());
		vector_List_File.push_back(list_add);
		All_Nodes.Clear();
		for (int i = 0; i < (int)vector_List_File.size(); i++)
			All_Nodes.Add_List_Files(vector_List_File[i]);
		All_Nodes.ReSortList();
	}
	else {
		All_Nodes.Add_List_Files(list_add);
		vector_List_File.push_back(list_add);
		list_add.SaveToFile(list_add.GetFileName());
	}
	SaveDateNameToFile();
	All_Nodes.SaveToFile(All_Nodes.GetFileName());

}

void DataFile::UpdateList(int index_node)
{
	for (int count_vector = 0; count_vector < (int)vector_List_File.size(); count_vector++)
	{
		if (All_Nodes[index_node].GetPath().find(vector_List_File[count_vector].GetPath()) != wstring::npos)
		{
			for (int count_node = 0; count_node < (int)vector_List_File[count_vector].Size(); count_node++)
			{
				if (All_Nodes[index_node].GetPath().find(vector_List_File[count_vector][count_node].GetPath()) != wstring::npos)
				{
					vector_List_File[count_vector].SetPri(count_node, 1);
					vector_List_File[count_vector].UpdatePri(count_node);
					return;
				}
			}
			return;
		}
	}
}

vector<int> DataFile::CheckExistList(List_Nodes list_node)
{
	vector<int> List;
	for (int count_list = 0; count_list < (int)vector_List_File.size(); count_list++)
	{
		if (vector_List_File[count_list].GetPath().find(list_node.GetPath()) != wstring::npos)
		{
			List.push_back(count_list);
		}
	}
	return List;
}
int DataFile::NumberList()
{
	return (int)vector_List_File.size();
}
List_Nodes& DataFile::GetListNode()
{
	return All_Nodes;
}

int DataFile::LoadDataVector() {
	const int BUFFERSIZE = 260;
	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, L"%s\\%s", curPath, file_name.c_str());
	HANDLE hFile = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		CloseHandle(hFile);
		return 0;
	}
	DWORD fileSize = GetFileSize(hFile, 0);
	DWORD BytesRead = 0;
	//check empty file
	if (0 == fileSize) {
		CloseHandle(hFile);
		return 0;
	}
	WCHAR *reader = new WCHAR[fileSize];
	wstring buffer;
	if (ReadFile(hFile, reader, fileSize, &BytesRead, NULL)) {
		wstring reader_string(reader); // convert WCHAR* to WSTRING
		delete reader;
		unsigned long count = 0;
		//lấy số lượng file
		do
		{
			count++;
		} while (reader_string[count] != L'\n');
		buffer = reader_string.substr(0, count);
		int number_list = _wtoi(buffer.c_str());
		count++;
		// TODO: quet danh sach
		for (int i = 0; i < number_list; i++)
		{
			wstring temp_name;
			while (reader_string[count] != L'\n') temp_name += reader_string[count++];
			List_Nodes temp_list_file;
			temp_list_file.SetNameFile(temp_name);
			temp_list_file.LoadFromFile();
			vector_List_File.push_back(temp_list_file);
			count++;
		}
	}
	CloseHandle(hFile);
	return 1;
}

int DataFile::SaveDateNameToFile()
{
	const int BUFFERSIZE = 260;
	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, L"%s\\%s", curPath, file_name.c_str());
	HANDLE hFile = CreateFileW(filePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD NumberOfBytesWriten = 0;
	WCHAR *buffer = new WCHAR[255];

	swprintf(buffer, 255, L"%d\n", (int)vector_List_File.size());
	WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	for (int i = 0; i < (int)vector_List_File.size(); i++)
	{
		swprintf(buffer, 255, L"%s\n", vector_List_File[i].GetFileName().c_str());
		WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	}
	CloseHandle(hFile);
	All_Nodes.SaveToFile(All_Nodes.GetFileName());
	return 1;
}

