#pragma once
class Node_File {
private:

	std::wstring name_file;
	std::wstring path_file;
	int pri;
public:
	Node_File();
	Node_File(std::wstring path);
	Node_File(std::wstring name, std::wstring path, int pr);
	void IncPri(int p);
	int GetPri();
	std::wstring GetName();
	std::wstring GetPath();
	void File_Execute();
	void Swap(Node_File &node);
	bool HaveSubString(std::wstring sub);
};
