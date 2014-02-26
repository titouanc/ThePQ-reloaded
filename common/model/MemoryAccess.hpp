#ifndef __MEMORYACCESS_HPP
#define __MEMORYACCESS_HPP

#include <json/json.hpp>
#include <string>
#include <vector>


namespace MemoryAccess
{
	std::string getUserDirectory(std::string);
	std::string getUserPath(std::string);
	std::string getPlayersDirectory(std::string);
	std::string getPlayerPath(std::string, int);
	std::string getInstallationsDirectory(std::string);
	std::string getInstallationPath(std::string, std::string);
	std::string getSalePath(int);

	void save(std::string, JSON::Dict&);

	JSON::Dict load(std::string, std::string, int);
	JSON::Dict load(std::string, std::string);
	JSON::Dict load(std::string, std::string, std::string);
	JSON::Dict load(std::string, int);

	JSON::List loadFilesInDir(std::string);
	JSON::List loadList(std::string);
	JSON::List loadList(std::string, std::string);

	void removeFile(std::string, std::string);
	void removeFile(std::string, int);
	void removeFile(std::string, std::string, int);
	void removeFile(std::string, std::string, std::string);
}
#endif