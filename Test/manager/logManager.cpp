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

#include <sstream>
#include <fstream>
#include "../manager/logManager.h"

/**
* Constructor
*/
LogManager::LogManager()
{
	this->header = false;
	this->logFileName = "output.log";
}

/**
* Destructor
*/
LogManager::~LogManager()
{
	if(this->header)
	{
		std::ofstream file;
		file.open(this->logFileName.c_str(), std::ios::app);
		if(file)
			file << "Shutting down\n";
		file.close();
	}
}

/**
* Write the header of the file
*/
void LogManager::writeHeader()
{
	if(!this->header)
	{
		std::ostringstream stream;
		std::ofstream file;
		time_t timestamp = time(NULL);

		struct tm * t = NULL;
		t = localtime(&timestamp);
		stream << t->tm_mday << "/" << t->tm_mon << "/" << ((t->tm_year) + 1900) << ": " << t->tm_hour << "h: " << t->tm_min << "min : " << t->tm_sec << "sec";

		file.open(this->logFileName.c_str(), std::ios::app);
		if(file)
		{
			file << "// ================================================== //" << "\n";
			file << "          Log file - " << stream.str() << "\n";
			file << "// ================================================== //" << "\n";
		}
		file.close();

		this->header = true;
	}
}

/**
* Log a int variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, int v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log an unsigned int variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, unsigned int v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log a float variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, float v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log a string variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, std::string v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log a char variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, char v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log a char* variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, char * v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}

/**
* Log a boolean variable
* @param text Text
* @param v value
*/
void LogManager::logIt(std::string text, bool v)
{
	writeHeader();

	std::ofstream file;
	file.open(this->logFileName.c_str(), std::ios::app);
	if(file)
		file << text << " " << v << "\n";
	file.close();
}
