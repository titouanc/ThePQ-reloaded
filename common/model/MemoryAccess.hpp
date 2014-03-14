#ifndef __MEMORYACCESS_HPP
#define __MEMORYACCESS_HPP

#include <json/json.hpp>
#include <string>
#include <vector>
#include <deque>

class Installation;
class User;
class Player;
class Sale;
class Team;
class Broomstick;
class Jersey;
class Championship;
namespace MemoryAccess
{
	std::string getUserDirectory(std::string);
	std::string getUserPath(std::string);
	std::string getPlayersDirectory(std::string);
	std::string getPlayerPath(std::string, int);
	std::string getInstallationsDirectory(std::string);
	std::string getInstallationPath(std::string, std::string);
	std::string getSalePath(int);
	std::string getTeamInfosPath(std::string);
	std::string getSkelPath(std::string);
	std::string getTeamNamesPath();
	std::string getAdminPath(std::string);
	std::string getUserNamesPath();
	std::string getChampionshipPath(std::string);

	void save(Installation*);
	void save(User&);
	void save(Player&);
	void save(Sale&);
	void save(Team&);
	void save(Championship&);
	void save(std::vector<std::string>&,std::string);

	void load(Player&);
	void load(User&);
	void load(Sale&);
	void load(Installation*);
	void load(Team&);//Loads _funds, etc.
	void load(Championship&);

	JSON::List loadFilesInVec(std::string);
	void load(std::vector<User>&);
	void load(std::vector<Installation*>&,std::string);
	void load(std::vector<Player>&,std::string);
	void load(std::vector<Sale*>&);
	void load(std::deque<Championship*>&);
	void load(std::vector<std::string>&, std::string);

	void loadSkel(Broomstick&);
	void loadSkel(Jersey&);
	void loadSkel(std::vector<Installation*>&);

	void loadAdmin(User&);
	void saveAdmin(User&);

	void removeObject(Player&);
	void removeObject(Sale&);
	void removeObject(User&);
	void removeObject(Installation*);
	void removeObject(Championship&);
}
#endif
