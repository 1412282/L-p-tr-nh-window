#pragma once
#include "Node.h"

class List_Nodes {
private:
	std::wstring path_scan;
	std::vector<Node_File> list_nodes;
	std::wstring last_file_save_name;
public:
	List_Nodes();
	int Add_List_Files(List_Nodes &list_src);
	int Sync_List(List_Nodes &list_add);
	std::wstring GetPath();
	std::wstring GetFileName();
	const int Size();
	void Create(std::wstring file_path);
	void UpdatePri(int index_node);
	void Clear();
	Node_File& operator[](int index);
	void SetNameFile(std::wstring filename);
	void SetPri(int index_node, int pri);
	void Find(std::wstring find_string, std::vector<int> &index_files);
	void Excute(int index);
	int SaveToFile(std::wstring file_name);
	void ReSortList();
	int LoadFromFile();
	int AddPathScan(std::wstring filename);
	~List_Nodes();
};