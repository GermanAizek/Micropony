#include "stdafx.h"
#include "curl.h"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct List {
	std::vector<std::string> paths;
	std::vector<std::string> params;
};

TCHAR * getKeyValue(HKEY reg, const wchar_t * path, const wchar_t * param)
{
	HKEY rKey;
	TCHAR Reget[256] = { 0 };
	DWORD RegetPath = sizeof(Reget);
	RegOpenKeyEx(reg, path, NULL, KEY_QUERY_VALUE, &rKey);
	RegQueryValueEx(rKey, param, NULL, NULL, (LPBYTE)&Reget, &RegetPath);

	return Reget;
}

void createRegList()
{
	std::ifstream in("list.dic");
	std::string path, param;
	//while (in.good()) {
		std::getline(in, path, ':');
		std::cout << path;
	//}

	//return List;
}

int main()
{


	TCHAR * value = getKeyValue(HKEY_LOCAL_MACHINE, L"\SOFTWARE\\Microsoft\\Internet Explorer", L"Version");

    return 0;
}

