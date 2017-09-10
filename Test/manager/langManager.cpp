/*
==============================================================================================================================================================

Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#ifdef _WIN32
	#include <windows.h>
#else
	#include <dirent.h>
	#include <cstring.h>
#endif

#include <sstream>
#include <fstream>
#include "../manager/optionManager.h"
#include "../manager/langManager.h"

/**
* Constructor
*/
LangManager::LangManager()
{
	loadLangFileList();
	reloadLangDatabase();
}

/**
* Destructor
*/
LangManager::~LangManager()
{
	this->langDatabase.clear();
}

/**
* Reload language information into the Lang Database
*/
void LangManager::reloadLangDatabase()
{
	langDatabase.clear();

	std::ostringstream stringstream;

	if((int)this->langFilename.size() > 0)
	{
		// Pick the correct lang file
		if(OptionManager::getInstance()->getLanguage() >= 0 && OptionManager::getInstance()->getLanguage() < (int)this->langFilename.size())
			stringstream << "conf/lang/game/" << this->langFilename[OptionManager::getInstance()->getLanguage()];
		else
		{
			OptionManager::getInstance()->setLanguage(0);
			stringstream << "conf/lang/game/" << this->langFilename[0];
		}

		// Open the language file
		std::ifstream file;
		file.open(stringstream.str().c_str(), std::ios::in);

		char tmp[200];

		// Read file
		if(file)
		{
			while(!file.eof())
			{
				file.getline(tmp,200);
				langDatabase.push_back(tmp);
			}
			file.close();
		}
	}
}

/**
* Get the text choosen
* @return Text
*/
std::string LangManager::getText(unsigned int v)
{
	std::string ret;
	if(v >= 0 && v < langDatabase.size())
		ret = langDatabase[v];
	else
		ret = " ";
	return ret;
}

/**
* Get the number of lang files
* @return The number of lang files
*/
int LangManager::getNumberOfLang()
{
	return (int)this->langFilename.size();
}

/**
* Read the lang directory and fill the filename list
*/
void LangManager::loadLangFileList()
{
	#ifdef _WIN32

	HANDLE hfind;
	WIN32_FIND_DATA wfd;
	hfind = FindFirstFileW(L"conf/lang/game/*.*", &wfd);
	do
	{
		// If it's a directory, we increase numberOfMod value
		if (wfd.cFileName[0] != '.' && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			// Convert WCHAR to String
			std::wstring wtest = wfd.cFileName;
			std::string tmpString(wtest.begin(),wtest.end());
			tmpString.assign(wtest.begin(),wtest.end());

			size_t found = tmpString.rfind(".lang");
			if(found == (tmpString.length() - 5))
				this->langFilename.push_back(tmpString);
		}
	}
	while(FindNextFile(hfind, &wfd));
	FindClose(hfind);

#else

	DIR *pDIR;
    struct dirent *pDirEnt;
	pDIR = opendir("conf/lang/game/");
	pDirEnt = readdir( pDIR );

	while ( pDirEnt != NULL )
	{
		// Make sure it's not ./ or ../
		if(strcmp(pDirEnt->d_name,".") != 0 && strcmp(pDirEnt->d_name,"..") != 0)
		{
			// If it's a file
			if (pDirEnt->d_type == DT_REG)
			{
				std::string tmpString = pDirEnt->d_name;

				if(tmpString.rfind(".lang") == tmpString.length() - 5)
					this->langFilename.push_back(tmpString);
			}
		}
		pDirEnt = readdir( pDIR );
	}
	closedir( pDIR );

	delete pDirEnt;
	pDIR = NULL;
	pDirEnt = NULL;

#endif
}
