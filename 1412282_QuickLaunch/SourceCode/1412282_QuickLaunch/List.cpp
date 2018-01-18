#include "stdafx.h"
#include "List.h"
#include <queue>
#include <vector>
#include <fstream>
#include <string>
#include <shellapi.h>
#include "ScanApp.h"
using namespace std;
#define MAX_FIND_COUNT 11

List_Nodes::List_Nodes()
{
	list_nodes.clear();
}

int List_Nodes::Add_List_Files(List_Nodes &list_src)
{
	for (int i = 0; i < list_src.Size(); i++)
	{
		list_nodes.push_back(list_src[i]);
	}
	return 1;
}

int List_Nodes::Sync_List(List_Nodes & list_sync)
{
	int count = 0;
	while (list_nodes[count].GetPri() > 0)
	{
		for (int i = 0; i < list_sync.Size(); i++)
		{
			if (list_nodes[count].GetPath() == list_sync[i].GetPath())
			{
				list_sync[i].IncPri(list_nodes[count].GetPri());
			}
		}
		count++;
	}

	return 1;
}

wstring List_Nodes::GetPath()
{

	return path_scan;

}

wstring List_Nodes::GetFileName()
{
	return last_file_save_name;

}

const int List_Nodes::Size()
{
	return (int)list_nodes.size();
}

void List_Nodes::Create(wstring file_path)
{
	path_scan = file_path;
	vector<wstring> list_path;
	get_path_File_list(file_path, list_path);
	list_nodes.clear();
	int count = 0;
	int list_path_size = (int)list_path.size();
	while (count < list_path_size)
	{
		Node_File temp_node(list_path[count]);
		list_nodes.push_back(temp_node);
		count++;
	}
	list_path.clear();
}


void List_Nodes::UpdatePri(int index_node)
{
	if (index_node > 0)
	{
		Node_File *node = &list_nodes[index_node];
		int node_pri = node->GetPri();

		int count = index_node;
		do
		{
			count--;
		} while (count >= 0 && node_pri > list_nodes[count].GetPri());
		if (count < 0)
			node->Swap(list_nodes[0]);
		else
			node->Swap(list_nodes[count + 1]);
	}
	SaveToFile(last_file_save_name);
}


void List_Nodes::Clear()
{
	list_nodes.clear();
}


Node_File& List_Nodes::operator[](int index)
{
	return list_nodes[index];
}


void List_Nodes::SetNameFile(wstring filename)
{
	last_file_save_name = filename;
}

void List_Nodes::SetPri(int index_node, int pri)
{
	list_nodes[index_node].IncPri(pri);
}


void List_Nodes::Find(wstring find_string, vector<int> &index_files)
{
	int count = 0;
	int i = 0;
	index_files.clear();
	while ((count < MAX_FIND_COUNT) && (i < (int)list_nodes.size()))
	{
		if (list_nodes[i].HaveSubString(find_string))
		{
			index_files.push_back(i);
			count++;
		}
		i++;
	}
	if (index_files.size() < 10)
		for (int i = 0; i < 10; i++)
			index_files.push_back(-1);
}


void List_Nodes::Excute(int index)
{
	list_nodes[index].File_Execute();
	UpdatePri(index);
}


int List_Nodes::SaveToFile(wstring file_name)
{
	const int BUFFERSIZE = 260;
	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, L"%s\\%s.txt", curPath, file_name.c_str());

	HANDLE hFile = CreateFileW(filePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD NumberOfBytesWriten = 0;
	WCHAR *buffer = new WCHAR[500];
	//lưu số lượng
	swprintf(buffer, 255, L"%d\n", list_nodes.size());
	WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	//lưu file name
	swprintf(buffer, 255, L"%s\n", last_file_save_name.c_str());
	WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	//luu path scan
	swprintf(buffer, 255, L"%s\n", path_scan.c_str());
	WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	for (int i = 0; i < (int)list_nodes.size(); i++)
	{
		swprintf(buffer, 500, L"%s|%s|%d\n", list_nodes[i].GetName().c_str(), list_nodes[i].GetPath().c_str(), list_nodes[i].GetPri());
		WriteFile(hFile, buffer, 2 * wcslen(buffer), &NumberOfBytesWriten, NULL);
	}
	CloseHandle(hFile);
	return 1;
}

void List_Nodes::ReSortList()
{

	int max_pri = 0;
	for (int i = 0; i < (int)list_nodes.size(); i++)
	{
		if (list_nodes[i].GetPri() > max_pri)
			max_pri = list_nodes[i].GetPri();
	}
	int index_file_swap = 0;
	int index_file_count = index_file_swap + 1;

	list_nodes[list_nodes.size() - 1].IncPri(99999999);//lấy biên
	while (index_file_count < (int)list_nodes.size())
	{
		while (list_nodes[index_file_count].GetPri() <= list_nodes[index_file_swap].GetPri()) index_file_count++;
		if (index_file_count == list_nodes.size() - 1) {
			index_file_swap++;
			index_file_count = index_file_swap + 1;
			list_nodes[list_nodes.size() - 1].IncPri(-99999999);
			if (list_nodes[index_file_count].GetPri() > list_nodes[index_file_swap].GetPri())
				list_nodes[index_file_count].Swap(list_nodes[index_file_swap]);
			list_nodes[list_nodes.size() - 1].IncPri(99999999);//lấy biên
		}
		else {
			list_nodes[index_file_count].Swap(list_nodes[index_file_swap]);
		}

		index_file_count++;
	}


}

int List_Nodes::LoadFromFile()
{
	const int BUFFERSIZE = 260;
	WCHAR curPath[BUFFERSIZE];
	WCHAR filePath[BUFFERSIZE];
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(filePath, L"%s\\%s.txt", curPath, last_file_save_name.c_str());

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
		unsigned long begin = 0;
		unsigned long count = 0;
		//lấy số lượng file
		do
		{
			count++;
		} while (reader_string[count] != L'\n');
		buffer = reader_string.substr(begin, count);
		int list_size = _wtoi(buffer.c_str());
		count++;
		wstring buffer;
		//lấy file name
		while (reader_string[count] != L'\n') buffer += reader_string[count++];
		last_file_save_name = buffer;
		count++;
		//lấy path_scan
		while (reader_string[count] != L'\n') path_scan += reader_string[count++];
		//lấy từng file bỏ vào db
		count++;
		wstring temp_name;
		wstring temp_path;
		wstring temp_pri_str;
		int temp_pri = 0;
		for (int index_file = 0; index_file < list_size; index_file++)
		{
			temp_name.clear();
			temp_path.clear();
			temp_pri_str.clear();
			while (reader_string[count] != L'|') temp_name += reader_string[count++];
			count++;
			while (reader_string[count] != L'|') temp_path += reader_string[count++];
			count++;
			while (reader_string[count] != L'\n') temp_pri_str += reader_string[count++];
			temp_pri = _wtoi(temp_pri_str.c_str());
			count++;
			Node_File temp_node(temp_name, temp_path, temp_pri);
			list_nodes.push_back(temp_node);

		}
	}
	CloseHandle(hFile);
	return 1;
}

int List_Nodes::AddPathScan(wstring filename)
{

	vector<wstring> list_path;


	if (filename.empty()) return 0;

	path_scan = filename + L"\\";

	last_file_save_name = filename;

	get_path_File_list(filename, list_path);
	int list_path_size = (int)list_path.size();

	int count = 0;
	while (count < list_path_size)
	{

		Node_File temp_node(list_path[count]);
		list_nodes.push_back(temp_node);
		count++;
	}

	list_path.clear();
	while (last_file_save_name.find(L':') != string::npos)	last_file_save_name.erase(last_file_save_name.find(L':'), 1);
	while (last_file_save_name.find(L'\\') != string::npos) 	last_file_save_name.erase(last_file_save_name.find(L'\\'), 1);
	wstring buffer = last_file_save_name;
	SaveToFile(buffer);
	return 1;
}

List_Nodes::~List_Nodes()
{
	path_scan.clear();
	list_nodes.clear();
	last_file_save_name.clear();
}

int CheckPathExist(vector<Node_File> list_nodes, wstring path)
{
	for (int i = 0; i < (int)list_nodes.size(); i++)
		if (list_nodes[i].GetPath() == path)
			return true;
	return false;
}

