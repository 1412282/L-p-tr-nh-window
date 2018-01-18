#pragma once
#include "List.h"

#define FILE_DATA L"FILE_DATA.txt"
#define data_file_name L"list_all_node"



class DataFile {
private:
	std::wstring file_name = FILE_DATA;
	std::vector<List_Nodes> vector_List_File;
	List_Nodes All_Nodes;
public:
	DataFile();
	void AddListFile(List_Nodes &list_add);
	void UpdateList(int index_node);
	std::vector<int> CheckExistList(List_Nodes list_node);
	int NumberList();
	List_Nodes& GetListNode();
	int LoadDataVector();
	int SaveDateNameToFile();
};