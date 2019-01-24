#pragma once

#include <windows.h>
#include <Shlobj.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct List
{
	std::vector<TCHAR *> paths;
	std::vector<TCHAR *> params;
};

struct Report
{
	std::vector<TCHAR *> paths;
	std::vector<TCHAR *> params;
	std::vector<TCHAR *> values;

	std::vector<LPCWSTR> names;
	std::vector<LPCWSTR> catalogs;
};

// Get path
TCHAR* getPath(unsigned int CSIDL)
{
	TCHAR szPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL, NULL, 0, szPath))) {
		return szPath;
	}
	else {
		return nullptr;
	}
}

std::vector<WIN32_FIND_DATA>& getCatalog(LPCWSTR path) {
	HANDLE search_file;
	WIN32_FIND_DATA aa;
	std::vector<WIN32_FIND_DATA> vecFiles;

	search_file = FindFirstFile(path, &aa);
	while (FindNextFile(search_file, &aa) != NULL) {
		vecFiles.push_back(aa);
	}

	return vecFiles;
}


LPCWSTR grabFile(LPCWSTR path, LPCWSTR namefile)
{
	LPCWSTR catalog = (getPath(CSIDL_APPDATA) + std::wstring(path)).c_str();
	//std::wcout << catalog << '\n';
	for (auto& name : getCatalog(catalog)) {
		if (name.cFileName == namefile && name.cFileName != NULL) {
			return name.cFileName, catalog;
		}
	}
}

TCHAR * getKeyValue(HKEY reg, const wchar_t * path, const wchar_t * param)
{
	HKEY rKey;
	TCHAR Reget[512];
	DWORD RegetPath = sizeof(Reget);
	RegOpenKeyEx(reg, path, 0, KEY_QUERY_VALUE, &rKey);
	RegQueryValueEx(rKey, param, NULL, NULL, (LPBYTE)Reget, &RegetPath);
	RegCloseKey(rKey);

	return Reget;
}

List* parseList(const char* name)
{
	std::ifstream conf(name);

	if (!conf) {
		std::clog << "[ERROR] " << name << " not find!\n";
		return nullptr;
	}

	WCHAR* dataPath = new WCHAR[MAX_PATH];
	WCHAR* dataParam = new WCHAR[MAX_PATH];

	LPWSTR path = nullptr, param = nullptr;

	GetPrivateProfileString(TEXT("PATH_PARAM"), (LPWSTR)path, TEXT("None"), (LPWSTR)dataPath, MAX_PATH, (LPWSTR)name);
	GetPrivateProfileString(TEXT("PATH_PARAM"), (LPWSTR)param, TEXT("None"), (LPWSTR)dataParam, MAX_PATH, (LPWSTR)name);

	List list;
	
	wprintf(L"%s\n", name);
	wprintf(L"%s %s\n", dataPath, path);
	wprintf(L"%s %s\n", dataParam, param);
	//std::wcout << "Parsing " << name << " ...\n";
	//std::wcout << "Parsing " << data_path << params1 << " ...\n";
	//std::wcout << "Parsing " << data_param << params2 << " ...\n";
	list.paths.push_back(dataPath);
	list.params.push_back(dataParam);

	delete dataPath, dataParam;

	return &list;
}
