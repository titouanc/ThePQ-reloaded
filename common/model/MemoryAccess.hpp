#ifndef __MEMORYACCESS_HPP
#define __MEMORYACCESS_HPP

#include <json/json.hpp>
#include <string>
#include <vector>

class Installation;
class User;
class Player;
class Sale;
namespace MemoryAccess
{
	std::string getUserDirectory(std::string);
	std::string getUserPath(std::string);
	std::string getPlayersDirectory(std::string);
	std::string getPlayerPath(std::string, int);
	std::string getInstallationsDirectory(std::string);
	std::string getInstallationPath(std::string, std::string);
	std::string getSalePath(int);

	void save(Installation&);
	void save(User&);
	void save(Player&);
	void save(Sale&);

	Player load(Player&);
	User load(User&);
	Sale load(Sale&);
	Installation load(Installation&);
	JSON::List loadFilesInVec(std::string);
	void load(std::vector<Installation>*,std::string);
	void load(std::vector<Player>*,std::string);
	void load(std::vector<Sale>*);

	void removeFile(Player&);
	void removeFile(Sale&);
	void removeFile(User&);
	void removeFile(Installation&);
}
#endif